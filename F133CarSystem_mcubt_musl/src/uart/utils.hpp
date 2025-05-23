/*
 * utils.hpp
 *
 *  Created on: 2023年12月6日
 *      Author: Admin
 */

#ifndef JNI_UART_UTILS_HPP_
#define JNI_UART_UTILS_HPP_

#include <stdint.h>
#include <sys/time.h>

static inline uint16_t _get_check_sum(const uint8_t *data, int len) {
	int sum = 0;
	for (int i = 0; i < len; i++) {
		sum += data[i];
	}
	return (uint16_t) sum;
}

uint32_t uart_cal_calc_crc(void *buf, uint32_t len) {
    uint8_t *ptr=(uint8_t*)buf;
    uint32_t crc_sum=0;
    for(uint32_t i=0;i<len;i++){
        crc_sum += ptr[i];
    }
    return crc_sum;
}

//获取时间戳函数
static long long get_timestamp(void) {
    long long tmp;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    tmp = tv.tv_sec;
    tmp = tmp * 1000;
    tmp = tmp + (tv.tv_usec / 1000);
    return tmp;
}

template <typename T>
static inline T range_ctrl(const T val, const T min, const T max) {
	if (val < min) {
		return min;
	} else if (val > max) {
		return max;
	}
	return val;
}



#endif /* JNI_UART_UTILS_HPP_ */
