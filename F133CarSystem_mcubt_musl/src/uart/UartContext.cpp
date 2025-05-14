/*
 * UartContext.cpp
 *
 *  Created on: Sep 5, 2017
 *      Author: guoxs
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <memory.h>
#include <termio.h>
#include <sys/ioctl.h>

#include "common.h"
#include "uart/UartContext.h"

#define UART_DATA_BUF_LEN			16384	// 16KB

extern int parseProtocol(int uart, const BYTE *pData, UINT len);

UartContext::UartContext(int uartNum) :
	mIsOpen(false),
	mUartID(0),
	mDataBufPtr(NULL),
	mDataBufLen(0),
	mUartNumber(uartNum){

}

UartContext::~UartContext() {
	delete[] mDataBufPtr;
	closeUart();
}

bool UartContext::openUart(const char *pFileName, UINT baudRate) {
	LOGD("openUart pFileName = %s, baudRate = %d\n", pFileName, baudRate);
	mUartID = open(pFileName, O_RDWR|O_NOCTTY);

	if (mUartID <= 0) {
		mIsOpen = false;
		LOGE("openUart err %s\n", strerror(errno));
	} else {
		struct termios oldtio = { 0 };
		struct termios newtio = { 0 };
		tcgetattr(mUartID, &oldtio);

		newtio.c_cflag = baudRate|CS8|CLOCAL|CREAD;
		newtio.c_iflag = 0;	// IGNPAR | ICRNL
		newtio.c_oflag = 0;
		newtio.c_lflag = 0;	// ICANON
		newtio.c_cc[VTIME] = 0; /* inter-character timer unused */
		newtio.c_cc[VMIN] = 1; /* blocking read until 1 character arrives */
		tcflush(mUartID, TCIOFLUSH);
		tcsetattr(mUartID, TCSANOW, &newtio);

		// 设置为非阻塞
		fcntl(mUartID, F_SETFL, O_NONBLOCK);

		mIsOpen = run("uart");
		if (!mIsOpen) {
			close(mUartID);
			mUartID = 0;
		}

		LOGD("openUart mIsOpen = %d\n", mIsOpen);
	}

	return mIsOpen;
}

void UartContext::closeUart() {
	LOGD("closeUart mIsOpen: %d...\n", mIsOpen);
	if (mIsOpen) {
		requestExit();

		close(mUartID);
		mUartID = 0;
		mIsOpen = false;
	}
}

bool UartContext::send(const BYTE *pData, UINT len) {
	if (!mIsOpen) {
	    LOGD("uart%d not opend", mUartNumber);
		return false;
	}

	if (write(mUartID, pData, len) != (int) len) {	// fail
		LOGD("send Fail\n");
		return false;
	}

//	for (int i = 0; i < len ; ++i) {
//	    LOGD("%x", pData[i]);
//	}
	// success
	LOGD("send %d Success\n", mUartNumber);

	return true;
}

/*
UartContext* UartContext::getInstance() {
	static UartContext sUC;
	return &sUC;
}
*/

bool UartContext::readyToRun() {
	if (mDataBufPtr == NULL) {
		mDataBufPtr = new BYTE[UART_DATA_BUF_LEN];
	}

	if (mDataBufPtr == NULL) {
		closeUart();
	}

	return (mDataBufPtr != NULL);
}

bool UartContext::threadLoop() {
	if (mIsOpen) {
		// 可能上一次解析后有残留数据，需要拼接起来
#if 1
		int readNum = read(mUartID, mDataBufPtr + mDataBufLen, UART_DATA_BUF_LEN - mDataBufLen);
#else
		int readNum = 7;
		char buf[7] = {0xff, 0x55, 0, 1, 2, 0, 0};
		memcpy(mDataBufPtr, buf, 7);
		::sleep(1);
#endif

		if (readNum > 0) {
			mDataBufLen += readNum;

			// 解析协议
			int len = parseProtocol(mUartNumber, mDataBufPtr, mDataBufLen);
			if ((len > 0) && (len < mDataBufLen)) {
				// 将未解析的数据移到头部
				memcpy(mDataBufPtr, mDataBufPtr + len, mDataBufLen - len);
			}

			mDataBufLen -= len;
		} else {
			Thread::sleep(50);
		}

		return true;
	}

	return false;
}


static UartContext* uart0 = NULL;
static UartContext* uart5 = NULL;

void UartContext::init() {
    uart0 = new UartContext(UART_TTYS0);
    uart0->openUart("/dev/ttyS0", B9600);

    uart5 = new UartContext(UART_TTYS5);
    uart5->openUart("/dev/ttyS5", B115200);;
}

void UartContext::destroy() {
    if (uart0) {
        delete uart0;
        uart0 = NULL;
    }
    if (uart5) {
        delete uart5;
        uart5 = NULL;
    }
}

bool UartContext::sendTo(int uart, const BYTE* pData, UINT len) {
    switch (uart) {
    case UART_TTYS0:
        return uart0->send(pData, len);
    case UART_TTYS5:
        return uart5->send(pData, len);
    }
    LOGD("无效的串口号");
    return false;
}
