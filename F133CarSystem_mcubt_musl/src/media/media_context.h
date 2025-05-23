/*
 * media_context.h
 *
 *  Created on: 2023年2月2日
 *      Author: ZKSWE Develop Team
 */

#ifndef _MEDIA_MEDIA_CONTEXT_H_
#define _MEDIA_MEDIA_CONTEXT_H_

#include "media_base.h"

// 文件扫描回调函数	 ( 扫描路径, 路径类型, 是否正在扫描 )
typedef void (*media_scan_cb)(const char *dir, storage_type_e type, bool started);

namespace media {

void init();

void add_scan_cb(media_scan_cb cb);
void remove_scan_cb(media_scan_cb cb);
bool is_scanning();

// audio file
int get_audio_list_size(storage_type_e type);
std::string get_audio_file(storage_type_e type, int index);

// video file
int get_video_list_size(storage_type_e type);
std::string get_video_file(storage_type_e type, int index);

// image file
int get_image_list_size(storage_type_e type);
std::string get_image_file(storage_type_e type, int index);
}

#endif /* _MEDIA_MEDIA_CONTEXT_H_ */
