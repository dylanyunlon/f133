/*
 * UartContext.h
 *
 *  Created on: Sep 5, 2017
 *      Author: guoxs
 */

#ifndef _UART_UARTCONTEXT_H_
#define _UART_UARTCONTEXT_H_

#include <vector>
#include "system/Thread.h"
#include "CommDef.h"

class UartContext : public Thread {
public:
	virtual ~UartContext();

	bool openUart(const char *pFileName, UINT baudRate);
	void closeUart();

	bool isOpen() { return mIsOpen; }

	bool send(const BYTE *pData, UINT len);

	static void init();
	static void destroy();
	static bool sendTo(int uart, const BYTE *pData, UINT len);
//	static UartContext* getInstance();

protected:
	virtual bool readyToRun();
	virtual bool threadLoop();

private:
	UartContext(int uartnum);

private:
	bool mIsOpen;
	int mUartID;

	// 用于拼接缓存数据，可能存在上一包数据解析未完全，残留有数据
	BYTE *mDataBufPtr;
	int mDataBufLen;

	int mUartNumber; //串口代号
};

//#define UARTCONTEXT		UartContext::getInstance()

#endif /* _UART_UARTCONTEXT_H_ */
