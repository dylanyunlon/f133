/*
 * h264_player.h
 *
 *  Created on: 2022年6月21日
 *      Author: ZKSWE Develop Team
 */

#ifndef _MEDIA_H264_PLAYER_H_
#define _MEDIA_H264_PLAYER_H_

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <stdint.h>

enum disp_rot_e {
	E_DISP_ROT_0,
	E_DISP_ROT_90,
	E_DISP_ROT_180,
	E_DISP_ROT_270,
};

enum h264_player_flag_e {
	E_H264_PLAYER_FLAG_STREAM_EOF = 0x01,
	E_H264_PLAYER_FLAG_DISP_UNCACHE = 0x02,
	E_H264_PLAYER_FLAG_ENABLE_G2D = 0x04,
};

void zk_h264_player_preload();
int zk_h264_player_init(int w, int h, enum disp_rot_e rot, int flag);
void zk_h264_player_deinit();
void zk_h264_player_flush();
void zk_h264_player_show();
void zk_h264_player_hide();
void zk_h264_player_set_pos(int x, int y, int w, int h);
void zk_h264_player_set_crop(int x, int y, int w, int h);
void zk_h264_player_put_frame(uint8_t *data, uint32_t size);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MEDIA_H264_PLAYER_H_ */
