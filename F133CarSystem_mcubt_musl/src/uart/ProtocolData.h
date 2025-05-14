/*
 * ProtocolData.h
 *
 *  Created on: Sep 7, 2017
 *      Author: guoxs
 */

#ifndef _UART_PROTOCOL_DATA_H_
#define _UART_PROTOCOL_DATA_H_

#include <string>
#include "CommDef.h"


/******************** CmdID ***********************/
#define CMD_CHANNEL       0x20
#define CMD_SOUND_MODE    0x21
#define CMD_FIELD_GAIN    0x22
#define CMD_SOUND_EFFECT  0x23
#define CMD_FM_SW	  	  0x24
#define CMD_FM_FRE	  	  0x25

#define CMD_SOUND_MUTE	  0x26	// 功放mute控制
#define CMD_KEY_TONE	  0x27	// 按键音指令
#define CMD_BT_TIME	  	  0x28	// 蓝牙时间同步
#define CMD_BLE_TIRE	  0x29	// BLE胎压

#define CMD_BT            0x80  // len 2 bytes
#define CMD_UPDATE_MODE	  0x1F
#define CMD_QUERY_VER	  0x1E
/**************************************************/

/******************** 错误码 ***********************/
//#define ERROR_CODE_CMDID			1
/**************************************************/


typedef enum {
	SOUND_CHANNEL_ARM,
	SOUND_CHANNEL_FM,
	SOUND_CHANNEL_AUX,
	SOUND_CHANNEL_BT,
} sound_channel_e;

typedef enum {
	SOUND_FIELD_LU,
	SOUND_FIELD_RU,
	SOUND_FIELD_LD,
	SOUND_FIELD_RD,
} sound_field_e;


enum {
	UPD_M_SEND_START_SIGN,
	UPD_M_RECIVE_CMD,
	UPD_M_DONE,
};

enum {
	CMD_CHECK_UART_UPD = 0x01,
	CMD_SEND_DATA = 0x02,
	CMD_READ_STATUS = 0x03,
};

//#pragma pack(1)

typedef struct {
    uint16_t sign;
    uint8_t cmd;
    uint8_t status;
    uint32_t addr;
    uint32_t len;//当为读取数据时，为读取数据的长度
    uint16_t crc;
}uart_upd_m_rxcmd_t;

//总共14byte
typedef struct {
    uint16_t sign;
    uint8_t cmd;
    uint8_t status;
    uint32_t addr;
    uint32_t data_crc;//当为发送数据时，是发送数据对应的crc
    uint16_t crc;
} uart_upd_m_txcmd_t;

//#pragma pack()

typedef struct {
    uint8_t step = UPD_M_DONE;
    uart_upd_m_rxcmd_t rxcmd;
    uart_upd_m_txcmd_t txcmd;
    uint32_t addr;
} upd_uart_m_t;

typedef struct {
	int uart_from; //来自哪一个串口
	BYTE cmd;
	BYTE data[256];
	int len;
} SProtocolData;







#endif /* _UART_PROTOCOL_DATA_H_ */
