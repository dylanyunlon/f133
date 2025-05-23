/*
 * media_utils.cpp
 *
 *  Created on: 2022年6月16日
 *      Author: ZKSWE Develop Team
 */

#include <dlfcn.h>

#include "media_parser.h"
#include "CdxParser.h"
#include "misc/sconv.h"

typedef struct {
	void *handle;
	int (*prepare)(CdxDataSourceT *source, cdx_uint32 flags, pthread_mutex_t *mutex,
	                     cdx_bool *exit, CdxParserT **parser, CdxStreamT **stream,
	                     ContorlTask *parserTasks, ContorlTask *streamTasks);
} cdx_parser_t;

static cdx_parser_t _s_cdx_parser;

static int _load_lib() {
	if (_s_cdx_parser.handle) {
		return 0;
	}

	void *handle = dlopen("libcdx_parser.so", RTLD_LAZY);
	if (!handle) {
		return -1;
	}

	_s_cdx_parser.prepare = (int (*)(CdxDataSourceT*, cdx_uint32, pthread_mutex_t*, cdx_bool*, CdxParserT**,
								CdxStreamT**, ContorlTask*, ContorlTask*)) dlsym(handle, "CdxParserPrepare");
	_s_cdx_parser.handle = handle;

	return 0;
}

static bool _is_utf8_str(const uint8_t *str, int len) {
	int nBytes = 0; //UFT8可用1-6个字节编码,ASCII用一个字节
	uint8_t chr;
	bool bAllAscii = true;
	for (int i = 0; i < len; ++i) {
		chr = str[i];
		//判断是否ASCII编码,如果不是,说明有可能是UTF8,ASCII用7位编码,最高位标记为0,0xxxxxxx
		if (nBytes == 0 && (chr & 0x80) != 0) {
			bAllAscii = false;
		}
		if (nBytes == 0) {
			//如果不是ASCII码,应该是多字节符,计算字节数
			if (chr >= 0x80) {
				if (chr >= 0xFC && chr <= 0xFD) {
					nBytes = 6;
				} else if (chr >= 0xF8) {
					nBytes = 5;
				} else if (chr >= 0xF0) {
					nBytes = 4;
				} else if (chr >= 0xE0) {
					nBytes = 3;
				} else if (chr >= 0xC0) {
					nBytes = 2;
				} else {
					return false;
				}
				nBytes--;
			}
		} else {
			//多字节符的非首字节,应为 10xxxxxx
			if ((chr & 0xC0) != 0x80) {
				return false;
			}
			//减到为零为止
			nBytes--;
		}
	}
	//违返UTF8编码规则
	if (nBytes != 0) {
		return false;
	}
	if (bAllAscii) { //如果全部都是ASCII, 也是UTF8
		return true;
	}
	return true;
}

static bool _is_gbk_str(const uint8_t *str, int len) {
	int nBytes = 0; //GBK可用1-2个字节编码,中文两个 ,英文一个
	uint8_t chr;
	bool bAllAscii = true; //如果全部都是ASCII,
	for (int i = 0; i < len; ++i) {
		chr = str[i];
		if ((chr & 0x80) != 0 && nBytes == 0) { // 判断是否ASCII编码,如果不是,说明有可能是GBK
			bAllAscii = false;
		}
		if (nBytes == 0) {
			if (chr >= 0x80) {
				if (chr >= 0x81 && chr <= 0xFE) {
					nBytes = +2;
				} else {
					return false;
				}
				nBytes--;
			}
		} else {
			if (chr < 0x40 || chr > 0xFE) {
				return false;
			}
			nBytes--;
		} //else end
	}
	if (nBytes != 0) {   //违返规则
		return false;
	}
	if (bAllAscii) { //如果全部都是ASCII, 也是GBK
		return true;
	}
	return true;
}

static std::string _gbk_to_utf8(const char *gbk_str, int len) {
	int size = sconv_gbk_to_unicode(gbk_str, len, NULL, 0);
	wchar unicode_str[size / 2 + 1];
	size = sconv_gbk_to_unicode(gbk_str, len, unicode_str, size);
	unicode_str[size / 2] = 0;

	size = sconv_unicode_to_utf8(unicode_str, -1, NULL, 0);
	char utf8_str[size + 1];
	size = sconv_unicode_to_utf8(unicode_str, -1, utf8_str, size);
	utf8_str[size] = 0;
	std::string utf8(utf8_str, size);
	return utf8;
}

static void _convert(char *dst, int dst_len, const uint8_t *src, int src_len) {
	if (_is_utf8_str(src, src_len)) {
		int len = (dst_len > src_len) ? src_len : dst_len;
		memcpy(dst, src, len);
		dst[len] = '\0';
	} else if (_is_gbk_str(src, src_len)) {
		std::string utf8 = _gbk_to_utf8((const char *) src, src_len);
		if (utf8.length() <= dst_len) {
			strcpy(dst, utf8.c_str());
		} else {
			strncpy(dst, utf8.c_str(), dst_len);
			dst[dst_len] = '\0';
		}
	}
}

static int _parser_prepare(CdxDataSourceT *source, cdx_bool *exit, CdxParserT **parser, CdxStreamT **stream) {
	if ((_load_lib() != 0) || !_s_cdx_parser.prepare) {
		return -1;
	}

	return _s_cdx_parser.prepare(source, 0, NULL, exit, parser, stream, NULL, NULL);
}

static CdxParserT* _cdx_open(const char *file, CdxMediaInfoT *mi) {
	CdxStreamT *stream = NULL;
	CdxParserT *parser = NULL;
	CdxDataSourceT source = { 0 };
	cdx_bool forceExit = 0;
	char tmp[MEDIA_FILE_MAX_PATH];
	int ret;

	snprintf(tmp, sizeof(tmp) - 1, "file://%s", file);
	source.uri = tmp;
	LOGD("url :%s", source.uri);

	ret = _parser_prepare(&source, &forceExit, &parser, &stream);
	if ((ret < 0) || !parser) {
		LOGE("we can not get the right parser, maybe we not support this file\n");
		if (stream) {
			CdxStreamClose(stream);
		}
		return NULL;
	}

	if (0 != CdxParserGetMediaInfo(parser, mi)) {
		LOGE("get media info err\n");
		CdxParserClose(parser);
		return NULL;
	}

	return parser;
}

namespace media {

bool parse_id3_info(const char *file, id3_info_t *info) {
	if (!file || !info) {
		return false;
	}

	CdxMediaInfoT mi = { 0 };
	CdxParserT *parser = _cdx_open(file, &mi);
	if (!parser) {
		return false;
	}

	_convert(info->title, sizeof(info->title) - 1, mi.title, mi.titlesz);
	_convert(info->artist, sizeof(info->artist) - 1, mi.artist, mi.artistsz);
	_convert(info->album, sizeof(info->album) - 1, mi.album, mi.albumsz);

	CdxParserClose(parser);

	return true;
}

bool parse_id3_pic(const char *file, const char *save_pic) {
	if (!file || !save_pic) {
		return false;
	}

	CdxMediaInfoT mi = { 0 };
	CdxParserT *parser = _cdx_open(file, &mi);
	if (!parser) {
		return false;
	}
	bool ret = false;
	if (mi.pAlbumArtBuf && (mi.nAlbumArtBufSize > 0)) {
		FILE *pf = fopen(save_pic, "wb");
		if (pf) {
			if (fwrite(mi.pAlbumArtBuf, 1, mi.nAlbumArtBufSize, pf) == mi.nAlbumArtBufSize) {
				ret = true;
			}
			fclose(pf);
		}
	}
	CdxParserClose(parser);

	return ret;
}

bool parse_video_info(const char *file, video_info_t *info) {
	if (!file || !info) {
		return false;
	}

	CdxMediaInfoT mi = { 0 };
	CdxParserT *parser = _cdx_open(file, &mi);
	if (!parser) {
		return false;
	}

	info->width = mi.program[0].video[0].nWidth;
	info->height = mi.program[0].video[0].nHeight;
	info->bit_rate = mi.bitrate;
	info->codec_format = mi.program[0].video[0].eCodecFormat;

	if ((info->bit_rate == 0) && (mi.program[0].duration != 0)) {
		info->bit_rate = mi.fileSize * 8 / (mi.program[0].duration / 1000);
	}

	CdxParserClose(parser);

	return true;
}

}
