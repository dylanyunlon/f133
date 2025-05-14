/*
 * context.cpp
 *
 *  Created on: 2022年6月13日
 *      Author: ZKSWE Develop Team
 */

#include "context.h"
#include "resampler.h"
#include "net/context.h"
#include "bt/context.h"
#include "media/audio_context.h"
#include "media/audio_recorder.h"
#include "media/h264_player.h"
#include "media/wav_utils.h"
#include "media/audio_stream.h"
#include "system/Mutex.h"
#include "system/setting.h"
#include "manager/ConfigManager.h"
#include "misc/storage.h"
#include "misc/parseSps.h"
#include "fy/files.hpp"
#include <fy/handler.hpp>

namespace lk {

#define DEBUG_SAVE_WAV          0  // 1 保存音频数据
#define DEBUG_SAVE_H264         0  // 1 保存h264视频数据
#define ENABLE_PHONE_TRANSFER   0  // 1 使能通话数据转到独立线程播放
#define ENABLE_LINK_START_HANDELER 0 // 1 互联启动、停止、重启放入handler中运行

#define MIX_MODE           1  // 是否是混音模式

#define LYLINK_LIC         "/tmp/lylink.lic"
#define RECORD_CHN         1
#define RECORD_RATE        16000    // 录音固定 1通道 16k
#define RECORD_RES_SIZE    8192*3

static std::set<lylink_callback> _s_cb_set;
static Mutex _s_mutex;
static LYLINK_TYPE_E _s_link_type;


static bool _s_lylink_is_authorized;
static bool _s_mfi_is_ok = true;

static bool bt_music_is_mute;

static bool _s_lylink_is_started = false;

typedef struct {
	audio::StreamTransfer st;
	void *player;
	void *player_wav;      // debug
} audio_transfer_t;

static struct {
	void *player;
	int player_ref;        // 引用计数，处理场景: media -> tts播报 -> tts停止 这种穿插场景，导致停止后meida不能恢复声音
	void *player_wav;      // debug

#if ENABLE_PHONE_TRANSFER
	audio::StreamTransfer phone_transfer;
#endif

#if !MIX_MODE
	audio_transfer_t audio_transfer[AudioType_Max];
#endif

	bool music_playing;
	LYLINK_CALLSTATEFOCUS_E call_state;  // 通话状态
	bool ringing;          // 通话中
	void *phone_player;    // 打电话与其他音频类型采样率及通道不一样，另外定义个播放器输出音频
	void *phone_wav;       // debug

	RESAMPLER record_res;      // 录音重采样
	uint8_t *record_res_buf;   // 重采样数据缓存

	Mutex lock;
} _s_audio_info;

static struct {
	bool inited;
	LYLINKAPI_VIDEO_PARAM param;
	FILE *h264_pf; // debug
	Mutex lock;
}_s_video_info;

#if ENABLE_LINK_START_HANDELER
static void _message_handler(const fy::message *msg, const void *user_data);
static fy::handler _handler(_message_handler, NULL);
#endif
static void _stream_data_cb(uint8_t *data, uint32_t size, void *user_data);

#if MIX_MODE
static void _mix_mode_player_init(LYLINK_AUDIOTYPE_E type, uint32_t channels, uint32_t rate) {
	if (type == AudioType_Phone) {
		LOGD("[link] phone player init...\n");
		_s_audio_info.phone_player = zk_audio_multi_player_init(channels, rate, 1024, 4);
		zk_audio_multi_player_enable_loopback(_s_audio_info.phone_player);

#if ENABLE_PHONE_TRANSFER
		// 1、通话数据回调, 2、延时ms开始传输
		_s_audio_info.phone_transfer.start(_stream_data_cb, NULL, true, 100);
#endif

#if DEBUG_SAVE_WAV
		_s_audio_info.phone_wav = zk_wav_init("/mnt/extsd/ly_phone.wav", channels, rate);
#endif
		return;
	}

	if (!_s_audio_info.player) {
		_s_audio_info.player = zk_audio_multi_player_init(channels, rate, 1024, 4);

#if DEBUG_SAVE_WAV
		_s_audio_info.player_wav = zk_wav_init("/mnt/extsd/ly_audio.wav", channels, rate);
#endif

		if (_s_link_type == LINK_TYPE_WIFICP) {
			if (type == AudioType_TTS) {
				// carplay导航播报数据有时喂得不及时，出现alsa xrun，表现为声音卡顿
				// 填充点静音数据   1920 * 20 / (48000 * 2 / 1000) = 400ms
				uint8_t silence[1920] = { 0 };
				for (uint32_t i = 0; i < 20; ++i) {
					zk_audio_multi_player_put_frame(_s_audio_info.player, silence, sizeof(silence));
				}
			}
		}
	}
	if (_s_audio_info.player) {
		_s_audio_info.player_ref++;
	}

	LOGD("[link] audio player init ref: %d\n", _s_audio_info.player_ref);
}

static void _mix_mode_player_deinit(LYLINK_AUDIOTYPE_E type) {
	if (type == AudioType_Phone) {
		LOGD("[link] phone player deinit...\n");
#if ENABLE_PHONE_TRANSFER
		_s_audio_info.phone_transfer.stop();
#endif

		zk_audio_multi_player_deinit(_s_audio_info.phone_player);
		_s_audio_info.phone_player = NULL;

#if DEBUG_SAVE_WAV
		zk_wav_deinit(_s_audio_info.phone_wav);
		_s_audio_info.phone_wav = NULL;
#endif

		return;
	}

	LOGD("[link] audio player deinit ref: %d\n", _s_audio_info.player_ref);

	if ((_s_audio_info.player_ref == 0) || (--_s_audio_info.player_ref > 0)) {
		return;
	}

	LOGD("[link] audio player deinit...\n");

	zk_audio_multi_player_deinit(_s_audio_info.player);
	_s_audio_info.player = NULL;

#if DEBUG_SAVE_WAV
	zk_wav_deinit(_s_audio_info.player_wav);
	_s_audio_info.player_wav = NULL;
#endif
}

static void _mix_mode_player_put_frame(bool phoning, uint8_t *data, uint32_t size) {
	if (phoning) {
		zk_audio_multi_player_put_frame(_s_audio_info.phone_player, data, size);
		zk_wav_add_data(_s_audio_info.phone_wav, data, size);
	} else {
		zk_audio_multi_player_put_frame(_s_audio_info.player, data, size);
		zk_wav_add_data(_s_audio_info.player_wav, data, size);
	}
}
#else
static void _nomix_mode_player_init(LYLINK_AUDIOTYPE_E type, uint32_t channels, uint32_t rate) {
	RETURN_IF_FAIL((type >= AudioType_Default) && (type < AudioType_Max));

	const char *type_str = _audio_type_to_str(type);

	LOGD("[link] %s audio player init...\n", type_str);

	audio_transfer_t &transfer = _s_audio_info.audio_transfer[type];
	if (transfer.player) {
		LOGE("[link] %s audio player has init\n", type_str);
		return;
	}

	transfer.player = zk_audio_multi_player_init(channels, rate, 1024, 4);
	if (!transfer.player) {
		LOGE("[link] %s audio player init fail !!!\n", type_str);
		return;
	}

	if (type == AudioType_Phone) {
		// 通话使能回采
		zk_audio_multi_player_enable_loopback(transfer.player);
	}

#if DEBUG_SAVE_WAV
	char debug_file[64];
	sprintf(debug_file, "/mnt/extsd/ly_%s_audio.wav", type_str);
	transfer.player_wav = zk_wav_init(debug_file, channels, rate);
#endif

	if (!transfer.st.start(_stream_data_cb, (void *) &transfer, false)) {
		LOGE("[link] %s audio transfer start fail !!!\n", type_str);
	}
}

static void _nomix_mode_player_deinit(LYLINK_AUDIOTYPE_E type) {
	RETURN_IF_FAIL((type >= AudioType_Default) && (type < AudioType_Max));

	LOGD("[link] %s audio player deinit...\n", _audio_type_to_str(type));

	audio_transfer_t &transfer = _s_audio_info.audio_transfer[type];

	transfer.st.stop();

	zk_audio_multi_player_deinit(transfer.player);
	transfer.player = NULL;

#if DEBUG_SAVE_WAV
	zk_wav_deinit(transfer.player_wav);
	transfer.player_wav = NULL;
#endif
}

static void _nomix_mode_player_put_frame(audio_transfer_t *transfer, uint8_t *data, uint32_t size) {
	zk_audio_multi_player_put_frame(transfer->player, data, size);
	zk_wav_add_data(transfer->player_wav, data, size);
}

static bool _nomix_mode_transfer_put_frame(LYLINK_AUDIOTYPE_E type, uint8_t *data, uint32_t size) {
	RETURN_VALUE_IF_FAIL((type >= AudioType_Default) && (type < AudioType_Max), false);

	audio_transfer_t &transfer = _s_audio_info.audio_transfer[type];
	if (!transfer.st.put(data, size)) {
		LOGE("[link] %s audio transfer put frame fail !!!\n", _audio_type_to_str(type));
		return false;
	}

	return true;
}
#endif

static void _stream_data_cb(uint8_t *data, uint32_t size, void *user_data) {
#if MIX_MODE
#if ENABLE_PHONE_TRANSFER
	_mix_mode_player_put_frame(true, data, size);
#endif
#else
	_nomix_mode_player_put_frame((audio_transfer_t *) user_data, data, size);

	// 释放不带混音时的音频数据
	lylinkapi_free(data);
#endif
}

static void _video_player_init(const LYLINKAPI_VIDEO_PARAM *param) {
	Mutex::Autolock _l(_s_video_info.lock);

	if(_s_video_info.inited) {
		return;
	}
	int w = param->width, h = param->height;
	if ((w == 0) || (h == 0)) {
		return;
	}

	enum disp_rot_e rot = (enum disp_rot_e) (CONFIGMANAGER->getScreenRotate() / 90);
	int crop_x = 0, crop_y = 0, crop_w = 0, crop_h = 0;    // crop pos
	int flag = E_H264_PLAYER_FLAG_STREAM_EOF;

	switch (_s_link_type) {
	case LINK_TYPE_AIRPLAY:
		flag = 0;
		break;

	case LINK_TYPE_USBAUTO:
	case LINK_TYPE_WIFIAUTO: {
		// auto有黑边需要裁剪,  w、h为去掉黑边后实际宽高
		int dw = w, dh = h;

		crop_w = w;
		crop_h = h;

		if (w == 1280) {  // 1280x720  上下有黑边
			dh = 720;
			crop_y = (dh - h) / 2;
		} else if (h == 480) {  // 800x480  左右有黑边
			dw = 800;
			crop_x = (dw - w) / 2;
		} else if (h == 720) {  // 1280x720 HD 左右有黑边
			dw = 1280;
			crop_x = (dw - w) / 2;
		} else if (w == 800) {
			dh = 480;
			crop_y = (dh - h) / 2;
		}

		if ((rot == E_DISP_ROT_90) || (rot == E_DISP_ROT_270)) {
			std::swap(crop_x, crop_y);
			std::swap(crop_w, crop_h);
		}

		w = dw;
		h = dh;
		break;
	}

	default:
		break;
	}
	zk_h264_player_init(w, h, rot, flag);
	zk_h264_player_hide();

	if ((crop_w != 0) && (crop_h != 0)) {
		zk_h264_player_set_crop(crop_x, crop_y, crop_w, crop_h);
		LOGD("[link] video player crop(%d, %d, %d, %d)\n", crop_x, crop_y, crop_w, crop_h);
	}

	_s_video_info.inited = true;
	_s_video_info.param = *param;

	LOGD("[link] video player init end\n");

#if DEBUG_SAVE_H264
	_s_video_info.h264_pf = fopen("/mnt/extsd/lylink.h264", "wb");
#endif
}

static void _video_player_deinit() {
	Mutex::Autolock _l(_s_video_info.lock);
	if(!_s_video_info.inited) {
		return;
	}

	zk_h264_player_deinit();
	_s_video_info.inited = false;

	LOGD("[link] video player deinit end\n");

#if DEBUG_SAVE_H264
	fflush(_s_video_info.h264_pf);
	fclose(_s_video_info.h264_pf);
	_s_video_info.h264_pf = NULL;
#endif
}

static void _video_player_put_frame(uint8_t *data, uint32_t size) {
	Mutex::Autolock _l(_s_video_info.lock);
	if(!_s_video_info.inited) {
		return;
	}
	zk_h264_player_put_frame(data, size);
#if DEBUG_SAVE_H264
	fwrite(data, 1, size, _s_video_info.h264_pf);
#endif
}

static void _clear_video_info() {
	LOGD("[link] clear video info\n");
	_video_player_deinit();
	memset(&_s_video_info.param, 0, sizeof(LYLINKAPI_VIDEO_PARAM));
}

static void _notify_callback(LYLINKAPI_EVENT evt, int para0, void *para1) {
	Mutex::Autolock _l(_s_mutex);
	SET_FOREACH(_s_cb_set, lylink_callback, it) {
		(*it)(evt, para0, para1);
	}
}

static void _proc_audio(LYLINKAPI_EVENT evt, int para0, void *para1) {
	// data与start、stop不同线程，需加锁
	switch (evt) {
	case LYLINK_AUDIO_START: {
		LOGD("[link] LYLINK_AUDIO_START %s", _audio_type_to_str((LYLINK_AUDIOTYPE_E) para0));
		const LYLINKAPI_AUDIO_PARAM *params = (const LYLINKAPI_AUDIO_PARAM *) para1;
		LOGD("[link] LYLINK_AUDIO_START SampleRate = %u, Channels = %u, Bits = %u\n",
				params->sampleRate, params->channelNum, params->bitWidth);

		Mutex::Autolock _l(_s_audio_info.lock);

#if MIX_MODE
		_mix_mode_player_init((LYLINK_AUDIOTYPE_E) para0, 2, 48000);
#else
		_nomix_mode_player_init((LYLINK_AUDIOTYPE_E) para0, params->channelNum, params->sampleRate);
#endif

		if (para0 == AudioType_Media) {         // 音乐
			_s_audio_info.music_playing = true;
			audio::change_audio_type(E_AUDIO_TYPE_LYLINK_MUSIC);
		} else if (para0 == AudioType_TTS) {    // 导航播报
			audio::handle_tts(E_AUDIO_TYPE_LYLINK_TTS, true);
		} else if (para0 == AudioType_VR) {     // 语音、 来电铃声
			LOGD("E_AUDIO_TYPE_LYLINK_VR = %d",E_AUDIO_TYPE_LYLINK_VR);
			audio::handle_phone(E_AUDIO_TYPE_LYLINK_VR, true);
			if (_s_audio_info.call_state == CallState_Ring) {
				_s_audio_info.ringing = true;
			}
		} else if (para0 == AudioType_Phone) {  // 电话
			audio::handle_phone(E_AUDIO_TYPE_LYLINK_PHONE, true);
		}

		break;
	}

	case LYLINK_AUDIO_STOP: {
		LOGD("[link] LYLINK_AUDIO_STOP %s\n", _audio_type_to_str((LYLINK_AUDIOTYPE_E) para0));
		// 处理auto导航播报音播放不完整问题
		if ((_s_link_type == LINK_TYPE_USBAUTO) || (_s_link_type == LINK_TYPE_WIFIAUTO)) {
			if (para0 == AudioType_TTS) {
				if (_s_audio_info.player_ref == 1) {
					DELAY(300);
				}
			}
		}

		Mutex::Autolock _l(_s_audio_info.lock);

#if MIX_MODE
		_mix_mode_player_deinit((LYLINK_AUDIOTYPE_E) para0);
#else
		_nomix_mode_player_deinit((LYLINK_AUDIOTYPE_E) para0);
#endif

		if (para0 == AudioType_TTS) {        // 导航播报
			audio::handle_tts(E_AUDIO_TYPE_LYLINK_TTS, false);
		} else if (para0 == AudioType_Media) {  // 音乐
			_s_audio_info.music_playing = false;
		} else if (para0 == AudioType_Phone) {  // 电话
			audio::handle_phone(E_AUDIO_TYPE_LYLINK_PHONE, false);
		}

		break;
	}

#if MIX_MODE
	case LYLINK_AUDIO_DATA: {
		/* not support LYLINK_START_ASYNC_DECODE & LYLINK_START_SYNC_DECODE */
		//LOGD("[link] audio data size: %d\n", para0);

		Mutex::Autolock _l(_s_audio_info.lock);

		if (_s_audio_info.call_state == CallState_Conn) {
#if ENABLE_PHONE_TRANSFER
			_s_audio_info.phone_transfer.put((uint8_t *) para1, para0);
#else
			_mix_mode_player_put_frame(true, (uint8_t *) para1, para0);
#endif
		} else {
			_mix_mode_player_put_frame(false, (uint8_t *) para1, para0);
		}

		break;
	}
#else
	case LYLINK_MEDIA_DATA:
	case LYLINK_VR_DATA:
	case LYLINK_TTS_DATA:
	case LYLINK_PHONE_DATA: {
		LYLINK_AUDIOTYPE_E type = (LYLINK_AUDIOTYPE_E) (evt - LYLINK_MEDIA_DATA + 1);
		if (!_nomix_mode_transfer_put_frame(type, (uint8_t *) para1, para0)) {
			lylinkapi_free(para1);
		}
		break;
	}
#endif

	default:
		break;
	}
}

static void _audio_record_cb(const void *data, uint32_t size) {
	if (_s_audio_info.record_res) {
		int out_len = _s_audio_info.record_res->process(_s_audio_info.record_res, data, size,
													_s_audio_info.record_res_buf, RECORD_RES_SIZE);
		if (out_len > 0) {
			lylinkapi_record(_s_audio_info.record_res_buf, out_len);
		}
	} else {
		lylinkapi_record(data, size);
	}
}

static void _proc_record(LYLINKAPI_EVENT evt, int para0, void *para1) {
	switch (evt) {
	case LYLINK_RECORD_INIT: {
		const LYLINKAPI_AUDIO_PARAM *params = (const LYLINKAPI_AUDIO_PARAM *) para1;
		LOGD("[link] LYLINK_RECORD_INIT SampleRate = %u, Channels = %u, Bits = %u\n",
				params->sampleRate, params->channelNum, params->bitWidth);
		// 录音固定 1通道 16k，如果不一致需重采样
#if 0
		if ((params->sampleRate != RECORD_RATE) || (params->channelNum != RECORD_CHN)) {
			RESAMPLER res = lylink_resampler_create();
			if (res) {
				res->src(res, RECORD_RATE, 16, RECORD_CHN);
				res->dst(res, params->sampleRate, 16, params->channelNum);
				_s_audio_info.record_res = res;
				_s_audio_info.record_res_buf = (uint8_t *) malloc(RECORD_RES_SIZE);
			} else {
				LOGE("[link] lylink_resampler_create record_res err!!!\n");
			}
		}
#endif
		audio::record_start(RECORD_RATE, _audio_record_cb);
		break;
	}
	case LYLINK_RECORD_START:
		LOGD("[link] LYLINK_RECORD_START\n");
		break;
	case LYLINK_RECORD_STOP:
		LOGD("[link] LYLINK_RECORD_STOP\n");
		audio::record_stop();
#if 0
		if (_s_audio_info.record_res) {
			lylink_resampler_delete(_s_audio_info.record_res);
			free(_s_audio_info.record_res_buf);
			_s_audio_info.record_res = NULL;
			_s_audio_info.record_res_buf = NULL;
		}
#endif
		break;
	default:
		break;
	}
}

static void _proc_video(LYLINKAPI_EVENT evt, int para0, void *para1) {
	switch (evt) {
	case LYLINK_VIDEO_START: {
		const LYLINKAPI_VIDEO_PARAM * param = (const LYLINKAPI_VIDEO_PARAM*) para1;
		LOGD("[link] LYLINK_VIDEO_START width: %d, height: %d", param->width, param->height);
		if(param->width != _s_video_info.param.width || param->height != _s_video_info.param.height) {
			_video_player_deinit();
		}
		_video_player_init(param);
	}
		break;
	case LYLINK_VIDEO_HEAD:
		LOGD("[link] LYLINK_VIDEO_HEAD");
		if(_s_link_type == LINK_TYPE_MIRACAST) {// || _s_video_info.param.width == 0 || _s_video_info.param.height == 0) {
			int width = 0, height = 0, fps = 0;
			if (h264_decode_sps((BYTE *) para1 + 4, para0 - 4, width, height, fps)) {
				LOGD("[link] LYLINK_VIDEO_HEAD --width:%d-- --height:%d--\n", width, height);
				if ((_s_video_info.param.width != width) || (_s_video_info.param.height != height)) {
					LYLINKAPI_VIDEO_PARAM param;
					param.width = width;
					param.height = height;
					_proc_video(LYLINK_VIDEO_START, 0, &param);
					_notify_callback(LYLINK_VIDEO_START, 0, &param);
				}
			}
		}
		_video_player_put_frame((uint8_t *) para1, para0);
		break;
	case LYLINK_VIDEO_DATA:{
		//LOGD("[link] LYLINK_VIDEO_DATA\n");
		uint8_t *data = (uint8_t *) para1;
		if (_s_link_type == LINK_TYPE_MIRACAST) { // 兼容部分手机投屏无画面的问题
//			uint8_t *data = (uint8_t *) para1;
//			LOGD("LYLINK_VIDEO_DATA len:%d, 0x%x 0x%x 0x%x 0x%x 0x%x", para0, data[0], data[1], data[2], data[3], data[4]);
			if ((para0 >= 5) && (data[0] == 0x00) && (data[1] == 0x00) && (data[2] == 0x01) && (data[3] == 0x01)) {
				LOGE("[link] video data nal unit type error\n");
				data[2] = 0;
			}
		}
		_video_player_put_frame((uint8_t *) para1, para0);
	}
		break;
	case LYLINK_VIDEO_STOP:
		LOGD("[link] LYLINK_VIDEO_STOP\n");
		_clear_video_info();
		break;
	default:
		break;
	}
}

static void _lylink_callback(LYLINKAPI_EVENT evt, int para0, void *para1) {
	switch (evt) {
	case LYLINK_AUDIO_START:
	case LYLINK_AUDIO_STOP:
	case LYLINK_AUDIO_DATA:
	case LYLINK_MEDIA_DATA:  // 不带混音模式
	case LYLINK_VR_DATA:
	case LYLINK_TTS_DATA:
	case LYLINK_PHONE_DATA:
		_proc_audio(evt, para0, para1);
		return; //声音只在本文件中处理，不通知回调；避免视频数据过大，音频数据等锁影响性能（声音卡顿）
//		break;

	case LYLINK_RECORD_INIT:
	case LYLINK_RECORD_START:
	case LYLINK_RECORD_STOP:
		_proc_record(evt, para0, para1);
		return;

	case LYLINK_VIDEO_START:
	case LYLINK_VIDEO_STOP:
	case LYLINK_VIDEO_HEAD:
	case LYLINK_VIDEO_DATA:
		if (evt == LYLINK_VIDEO_START) {
			// miracast这里给的分辨率是不对的，需从LYLINK_VIDEO_HEAD sps里面解析分辨率
			if (_s_link_type == LINK_TYPE_MIRACAST) {
				return;
			}
		}

		_proc_video(evt, para0, para1);
		break;

	case LYLINK_VERSION_CODE:
		LOGD("errcode: %d, version: %s", (LYLINK_ERRCODE) para0, (const char *) para1);
		if (para0 == 0) {
			_s_lylink_is_authorized = true;
		} else if (para0 == 1) {
			LOGD("lylink authorization failure!");
		} else if (para0 == 2) {	// mfi出错优先解决
			_s_mfi_is_ok = false;
			LOGD("mfi error, please check whether the mfi chip is properly attached!");
		}
		break;
	case LYLINK_LICENSE_KEY:
		LOGD("[link] LYLINK_LICENSE_KEY size: %d\n", para0);
		// 保存设备授权码
//		if (storage::otp_write_data(LINK_LIC_OFFSET, (uint8_t *) para1, para0)) {
//			LOGD("[link] --%d--%s--保存设备授权码!!!--\n", __LINE__, __FILE__);
//		}
		break;

	case LYLINK_CALL_STATE:
		// para0: 2 通话中、1 来电、0 挂断
		LOGD("[link] LYLINK_CALL_STATE para0 %d...\n", para0);
		if(para0 == CallState_Conn){
			if(_s_audio_info.ringing){
				_s_audio_info.ringing = false;
			}
		}
		if(para0 == CallState_Hang){
			if(_s_audio_info.call_state == CallState_Ring){
				if(_s_audio_info.ringing){
					audio::handle_phone(E_AUDIO_TYPE_LYLINK_VR, false);
				}
				_s_audio_info.ringing = false;
			}
		}
		_s_audio_info.call_state = (LYLINK_CALLSTATEFOCUS_E) para0;
		break;

	case LYLINK_LINK_ESTABLISH:
		LOGD("--%d-- --%s-- LYLINK_LINK_ESTABLISH %s", __LINE__, __FILE__, lk::_link_type_to_str((LYLINK_TYPE_E) para0));
		bt_music_is_mute = bt::get_bt_is_mute();
		_s_link_type = (LYLINK_TYPE_E) para0;
		if (_s_link_type == LINK_TYPE_MIRACAST || _s_link_type == LINK_TYPE_AIRPLAY) {
			audio::change_audio_type(E_AUDIO_TYPE_LYLINK_MUSIC);
		} else if (_s_link_type == LINK_TYPE_WIFIAUTO) {
			bt::set_bt_mute(false);
		}
		break;

	case LYLINK_LINK_DISCONN:
		LOGD("--%d-- --%s-- LYLINK_LINK_DISCONN %s", __LINE__, __FILE__, lk::_link_type_to_str((LYLINK_TYPE_E) para0));
		if ((_s_link_type != LINK_TYPE_NULL) && (bt::get_bt_is_mute() != bt_music_is_mute)) {
			bt::set_bt_mute(bt_music_is_mute);
		}
		_s_link_type = LINK_TYPE_NULL;
//		_video_player_deinit();
		_clear_video_info();
		break;
//	case LYLINK_PHONE_CONNECT:
//		if (((para0 == LINK_TYPE_WIFICP) || (para0 == LINK_TYPE_WIFIAUTO)) && (net::get_mode() == E_NET_MODE_AP)) {
//			// 设置里边清除cp连接会kill hostapd, 有蓝牙连上再启动hostapd
//			START_SERVICE(NET_AP_SUPP);
//		}
//		break;

	case LYLINK_BACKGROUND:
	case LYLINK_HID_COMMAND:
		_clear_video_info();
		break;
	case LYLINK_UPDATE_TIME: {
		const LYLINKAPI_TIME *param = (const LYLINKAPI_TIME *) para1;
		char c_zone[8] = {0};
		int zone = param->zone / 60;
		sprintf(c_zone,"GMT%c%d",zone >= 0 ? '-' : '+',abs(zone));
		setenv("TZ", c_zone, 1);
		tzset();
		struct tm *t;
		time_t timeStamp = param->time;
		t = localtime(&timeStamp);
		sys::setting::set_DateTime(t);
	}
	break;
	default:
		break;
	}

	_notify_callback(evt, para0, para1);
}

static void _start_monitor_wifi() {
	LYLINKAPI_WIFI_MONITOR_PARAM param;
	param.id = LYLINK_WIFI_MONITOR;
	param.mode = LYLINK_AP_5G_MODE;
	strcpy(param.hostapd, NET_AP_CTRL_PATH);
	strcpy(param.wpa_supplicant, NET_P2P_CTRL_PATH);
	strcpy(param.hostapd_psk, "12345678");

	lylinkapi_wifi((LYLINKAPI_WIFI_PARAM *) &param);
	LOGD("[link] start monitor wifi ...\n");
}

static bool _start_link() {
	LOGD("[link] start\n");
	if (!FILE_EXIST(LYLINK_LIC)) {
		storage::otp_load_data_to_file(LINK_LIC_OFFSET, LINK_LIC_SIZE, LYLINK_LIC);
	}

	if (FILE_EXIST("/data/home.png")) {
		mkdir("/tmp/lylink", 0666);
		symlink("/data/home.png", "/tmp/lylink/home.png");
	}

	// 设置airplay名称
	char script[128];
	sprintf(script, LINK_SCRIPT, sys::setting::get_dev_name().c_str());
	sprintf(script, LINK_SCRIPT_CN, sys::setting::get_dev_name().c_str());
//	fy::files::create_script(LINK_BIN_SH, script);
	fy::files::remove(LINK_BIN_SH);
	if (sys::setting::get_link_mode() == E_LINK_MODE_CARLIFE) {
		fy::files::create_script(LINK_BIN_SH, LINK_SCRIPT_CN);
	} else {
		fy::files::create_script(LINK_BIN_SH, LINK_SCRIPT);
	}

	int funmode = 0;
	switch (sys::setting::get_link_mode()) {
	case E_LINK_MODE_CARPLAY:
		funmode = WIFIFLAG_CARPLAY;// | USBFLAG_CARPLAY;
		break;
	case E_LINK_MODE_CARLIFE:
		funmode = WIFIFLAG_CARLIFE;
		break;
	case E_LINK_MODE_ANDROIDAUTO:
		funmode = WIFIFLAG_ANDROID;
		break;
	case E_LINK_MODE_AIRPLAY:
		funmode = WIFIFLAG_AIRPLAY;
		break;
	case E_LINK_MODE_MIRACAST:
		funmode = WIFIFLAG_MIRACAST;
		break;
	case E_LINK_MODE_LYLINK:
		funmode = WIFIFLAG_LYLINK;
		break;
	default:
		break;
//		return false;
	}
	_s_lylink_is_started = true;
	START_SERVICE(LINK_SERVICE);


	LYLINKAPI_PARAM params = { 0 };
	params.width = LINK_VIEW_WIDTH;
	params.height = LINK_VIEW_HEIGHT;
	params.pos = DrivingPosition_Left;
	params.mode = MIX_MODE ? LYLINK_START_ASYNC_NODEC : LYLINK_START_ASYNC_NOMIX;
	params.callback = _lylink_callback;
	params.physical_width = LINK_VIEW_WIDTH;
	params.physical_height = LINK_VIEW_HEIGHT;
	params.night = LYLINK_NIGHTMODE_DAY;
	params.funmode = funmode;
	params.flags |= LYLINK_FLAGS_VIDEO_LYLINK;

	sys::setting::get_linksound_type() == E_TYPE_ENGINE
			? params.flags |= LYLINK_FLAGS_VIDEO_LYLINK
			: params.flags |= LYLINK_FLAGS_VIDEO_ONLY;

	// 可配置mfi i2c号及地址
#if LINK_MFI_SET_ENABLE
	params.flags |= LYLNIK_FLAGS_SET_MFI_I2C;
	params.conf.mfi = LINK_MFI_I2C_N;
	params.conf.mfiaddr = LINK_MFI_I2C_ADDR;
#endif

	params.flags |= LYLINK_FLAGS_SET_AUTH_I2C;
	params.conf.auth = 4;

	strcpy(params.huName, "Home");
	if (0 != lylinkapi_start(&params)) {
		LOGE("[link] lylinkapi_start error \n");
		return false;
	}

	LOGD("[link] lylinkapi_start width:%d, height:%d\n", params.width, params.height);

	_start_monitor_wifi();

	return true;
}

static void _stop_link() {
	LOGD("[link] stop\n");
	lylinkapi_stop();
	STOP_SERVICE(LINK_SERVICE);
	_s_lylink_is_started = false;
}
static void _restart_link() {
	LOGD("[link] restart\n");
	_stop_link();
	DELAY(50);
	_start_link();
}

#if ENABLE_LINK_START_HANDELER
void start_lylink() {
	_handler.send_message(E_START_LYLINK);
}
void stop_lylink() {
	_handler.send_message(E_STOP_LYLINK);
}
void restart_lylink() {
	_handler.send_message(E_RESTART_LYLINK);
}
static void _message_handler(const fy::message *msg, const void *user_data) {
	LOGD("[link] handle what: %d\n", msg->what);
	switch (msg->what) {
		case E_START_LYLINK:
			_start_link();
			break;
		case E_STOP_LYLINK:
			_stop_link();
			break;
		case E_RESTART_LYLINK:
			_restart_link();
			break;
		default:
			break;
	}
}
#else
void start_lylink() {
	_start_link();
}
void stop_lylink() {
	_stop_link();
}
void restart_lylink() {
	_restart_link();
}
#endif
void add_lylink_callback(lylink_callback cb) {
	Mutex::Autolock _l(_s_mutex);
	_s_cb_set.insert(cb);
}

void remove_lylink_callback(lylink_callback cb) {
	Mutex::Autolock _l(_s_mutex);
	_s_cb_set.erase(cb);
}

bool get_is_authorized() {
	return _s_lylink_is_authorized;
}

void query_is_authorized() {
	lylinkapi_cmd(LYLINK_CHECK_VERIFY);
}

bool get_mfi_is_OK() {
	return _s_mfi_is_ok;
}

void music_pause() {
	LOGD("[link] lylink music_pause\n");
	lylinkapi_key(LY_ConsumerActionPause, KeyMode_Down);
	lylinkapi_key(LY_ConsumerActionPause, KeyMode_Up);
	lylinkapi_cmd(LYLINK_AUDIO_NATIVE);
}

void music_resume() {
	LOGD("[link] lylink music_resume\n");
	lylinkapi_key(LY_ConsumerActionPlay, KeyMode_Down);
	lylinkapi_key(LY_ConsumerActionPlay, KeyMode_Up);
	lylinkapi_cmd(LYLINK_AUDIO_LYLINK);
}

void video_start() {
	LOGD("[link] send LYLINK_VIDEO_LYLINK");
	lylinkapi_cmd(LYLINK_VIDEO_LYLINK);
}

void video_stop() {
	lylinkapi_cmd(LYLINK_VIDEO_NATIVE);
	_clear_video_info();
}

void video_show(int x, int y, int w, int h) {
	Mutex::Autolock _l(_s_video_info.lock);
	if (!_s_video_info.inited) {
		LOGE("[link] video not inited, can't show video!\n");
		return;
	}

	LOGD("[link] video_show pos(%d, %d, %d, %d)\n", x, y, w, h);
	zk_h264_player_set_pos(x, y, w, h);
	zk_h264_player_show();
}

void video_hide() {
	Mutex::Autolock _l(_s_video_info.lock);
	if (!_s_video_info.inited) {
		LOGE("[link] video not inited, can't hide video!\n");
		return;
	}

	LOGD("[link] video_hide\n");
	zk_h264_player_hide();
}

bool get_video_param(LYLINKAPI_VIDEO_PARAM &param) {
	if(_s_video_info.param.width == 0 || _s_video_info.param.height == 0) {
		return false;
	}
	param = _s_video_info.param;
	return true;
}

bool get_is_ringing() {
	return _s_audio_info.ringing;
}

LYLINK_CALLSTATEFOCUS_E get_is_call_state() {
	Mutex::Autolock _l(_s_audio_info.lock);
	return _s_audio_info.call_state;
}

bool music_is_playing() {
	return _s_audio_info.music_playing;
}

LYLINK_TYPE_E get_lylink_type() {
	return _s_link_type;
}

bool is_connected() {
	return LINK_TYPE_NULL != get_lylink_type();
}

bool is_lylink_started() {
	return _s_lylink_is_started;
}

void set_link_night_mode(bool is_night) {
	lylinkapi_cmd(is_night ? LYLINK_CMD_NIGHTMODE : LYLINK_CMD_DAYMODE);
}

}
