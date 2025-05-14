/*
 * music_player.h
 *
 *  Created on: 2023年2月5日
 *      Author: ZKSWE Develop Team
 */

#ifndef _MEDIA_MUSIC_PLAYER_H_
#define _MEDIA_MUSIC_PLAYER_H_

#include "media_base.h"
#include "media/ZKMediaPlayer.h"


typedef void (*music_play_status_cb)(music_play_status_e status);

namespace media {

void music_init();
void music_play(storage_type_e type, int file_index, int msec = 0);
int music_get_play_index();		//获取播放索引
void music_play(const char *path, int msec = 0);
void music_pause();
void music_resume();
void music_seek(int msec);
void music_stop();
bool music_is_playing();
void music_next(bool isConvert = false);		//isConvert:单曲模式下是否进行顺序播放		应用场景:1.单曲手动切, 2.单曲出错
void music_prev(bool isConvert = false);
void music_set_play_mode(media_play_mode_e mode);
media_play_mode_e music_get_play_mode();
void music_set_volume(float volume);
void music_set_gain(float gain);
int music_get_duration();
int music_get_current_position();
std::string music_get_current_play_file(bool check_state = true);		// 返回带路径文件名
void music_add_play_status_cb(music_play_status_cb cb);
void music_remove_play_status_cb(music_play_status_cb cb);

}

#endif /* _MEDIA_MUSIC_PLAYER_H_ */
