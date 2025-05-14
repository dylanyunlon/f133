/*
 * media_parser.h
 *
 *  Created on: 2022年6月16日
 *      Author: ZKSWE Develop Team
 */

#ifndef _MEDIA_MEDIA_PARSER_H_
#define _MEDIA_MEDIA_PARSER_H_

#include "media_base.h"

namespace media {

/**
 * @brief 解析id3信息
 * @param file    音频文件
 * @param info    信息结果
 * @return true 成功， false 失败
 */
bool parse_id3_info(const char *file, id3_info_t *info);

/**
 * @brief 解析id3图片
 * @param file        音频文件
 * @param save_pic    图片存储路径
 * @return true 成功， false 失败
 */
bool parse_id3_pic(const char *file, const char *save_pic);

/**
 * @brief 解析视频信息
 * @param file        视频文件
 * @param info        信息结果
 * @return true 成功， false 失败
 */
bool parse_video_info(const char *file, video_info_t *info);

}

#endif /* _MEDIA_MEDIA_PARSER_H_ */
