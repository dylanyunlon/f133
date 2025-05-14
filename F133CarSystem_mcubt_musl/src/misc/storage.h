/*
 * storage.h
 *
 *  Created on: Mar 5, 2023
 *      Author: guoxs
 */

#ifndef _MISC_STORAGE_H_
#define _MISC_STORAGE_H_

#include <stdint.h>
#include <string>

namespace storage {

/**
 * @brief 存储string值
 */
bool put_string(const std::string &key, const std::string &val);

/**
 * @brief 存储int值
 */
bool put_int(const std::string &key, int val);

/**
 * @brief 存储bool值
 */
bool put_bool(const std::string &key, bool val);

/**
 * @brief 存储float值
 */
bool put_float(const std::string &key, float val);

/**
 * @brief 删除key对应的数据
 */
bool remove(const std::string &key);

/**
 * @brief 清空存储数据
 */
bool clear();

/**
 * @brief 获取key对应的string值，获取不到则返回defVal默认值
 */
std::string get_string(const std::string &key, const std::string &defVal);

/**
 * @brief 获取key对应的int值，获取不到则返回defVal默认值
 */
int get_int(const std::string &key, int defVal);

/**
 * @brief 获取key对应的bool值，获取不到则返回defVal默认值
 */
bool get_bool(const std::string &key, bool defVal);

/**
 * @brief 获取key对应的float值，获取不到则返回defVal默认值
 */
float get_float(const std::string &key, float defVal);

/**
 * @brief 读取otp offset偏移位置数据，数据最长256个字节
 */
bool otp_read_data(int offset, uint8_t *data, uint32_t size);

/**
 * @brief 往otp offset偏移位置写数据，数据最长256个字节
 */
bool otp_write_data(int offset, uint8_t *data, uint32_t size);

/**
 * @brief 加载otp数据保存到文件
 */
bool otp_load_data_to_file(int offset, uint32_t size, const char *file);

}

#endif /* _MISC_STORAGE_H_ */
