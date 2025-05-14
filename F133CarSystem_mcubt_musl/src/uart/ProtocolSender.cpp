/*
 * ProtocolSender.cpp
 *
 *  Created on: Sep 8, 2017
 *      Author: guoxs
 */

#include "uart/ProtocolSender.h"
#include "uart/UartContext.h"
#include <stdio.h>
#include "utils/log.h"

//extern BYTE getCheckSum(const BYTE *pData, int len);
//extern UINT16 Crc16(const BYTE *ptr, UINT len);
extern UINT16 calculate_crc16(const BYTE* data, UINT16 len);
/**
 * 需要根据协议格式进行拼接，以下只是个模板
 */
bool sendProtocolTo(int uart, const BYTE cmdID, const BYTE *pData, BYTE len) {
	BYTE dataBuf[256];

	dataBuf[0] = CMD_HEAD1;
	dataBuf[1] = CMD_HEAD2;			// 同步帧头

//	dataBuf[2] = HIBYTE(cmdID);
//	dataBuf[3] = LOBYTE(cmdID);		// 命令字节

	dataBuf[2] = cmdID;		// 命令字节

	dataBuf[3] = len;

	UINT frameLen = 4;

	// 数据
	for (int i = 0; i < len; ++i) {
		dataBuf[frameLen] = pData[i];
		frameLen++;
	}

#ifdef PRO_SUPPORT_CHECK_SUM
	// 校验码
//	dataBuf[frameLen] = getCheckSum(dataBuf, frameLen);
	UINT16 crcCheck = calculate_crc16(dataBuf, frameLen);
	dataBuf[frameLen] =  (BYTE)(crcCheck >> 8);
	frameLen++;
	dataBuf[frameLen] =  (BYTE)crcCheck;
	frameLen++;
#endif

	LOGD("发送数据!  uart:%d, len:%d\n", uart, len);
	for (UINT i=0; i<frameLen; i++) {
		LOGD("--%d-- --%s-- dataBuf[%d]:%02x\n", __LINE__, __FILE__, i, dataBuf[i]);
	}

//	return UARTCONTEXT->send(dataBuf, frameLen);
	return UartContext::sendTo(uart, dataBuf, frameLen);
}
