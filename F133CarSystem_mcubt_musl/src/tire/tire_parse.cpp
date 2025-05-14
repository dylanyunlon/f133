/*
 * tire_parse.cpp
 *
 *  Created on: 2024年12月5日
 *      Author: zkswe
 */

#include "tire_parse.h"
#include "system/Mutex.h"
#include <map>
#include "common.h"
#include <set>
#include "system/setting.h"
#include <memory>
#include <cmath>

#define LOCK_OP(lock, op)           { Mutex::Autolock _l(lock); op; }
#define TIRE_INFO_TIMEOUT 	1000 * 60 * 30

#define STANDARD_ATMOSPHERIC_PRESSURE   14.6  // 标准大气压强 单位psi

typedef struct {
	tire_dev_t front_tire_info;
	tire_dev_t rear_tire_info;
	std::map<std::string, tire_dev_t> tire_dev_map;
}tire_info_t;

static tire_info_t _s_tire_info;
static Mutex _s_tire_info_mutex;

static std::set<tire_misc_info_cb> _s_tire_cb_set;
static Mutex _s_tire_cb_mutex;

static void _notify_tire_misc_info_cb(tire_misc_info_e info) {
	Mutex::Autolock _l(_s_tire_cb_mutex);
	SET_FOREACH(_s_tire_cb_set, tire_misc_info_cb, it) {
		(*it)(info);
	}
}

static int hexStr_to_int(std::string hexStr) {
	int retVal = 0;
	int len = hexStr.length();//0X1234
	for (int i = 0; i < len; i++) {
		if(hexStr[i] >= '0' && hexStr[i] <= '9') {
			retVal |= (hexStr[i] - '0') << (len-i-1)*4;
		}
		else if(hexStr[i] >= 'a' && hexStr[i] <= 'f') {
			retVal |= (hexStr[i] - 'a' + 0x0a) << (len-i-1)*4;
		}
		else if(hexStr[i] >= 'A' && hexStr[i] <= 'F') {
			retVal |= (hexStr[i] - 'A' + 0x0a) << (len-i-1)*4;
		}
	}
	return retVal;
}

static std::string int_to_hexStr(int value, int len) {
	std::string hexStr(len, '0');
	for (int i = len - 1; i >= 0; --i) {
		int low = value & 0xF;
		if (low < 10) {
			hexStr[i] = '0' + low;
		} else {
			hexStr[i] = 'A' + (low - 10);
		}
		value >>= 4;
	}
	return hexStr;
}

void TireOptionManager::initialize() {
	// 初始化胎压配置信息
	std::string data = sys::setting::get_tire_option_info();
	if (data.size() < 8) {
		reset_option();
		return ;
	}
	pressure_unit = (pressure_unit_e)data[0];
	temp_unit = (temp_unit_e)data[1];
	pressure_upper = ((uint8_t)data[2] << 8) | (uint8_t)data[3];
	pressure_lower = ((uint8_t)data[4] << 8) | (uint8_t)data[5];
	temp_limit = ((uint8_t)data[6] << 8) | (uint8_t)data[7];
	pressure_step = (pressure_unit == E_PRESSURE_PSI) ? 100 : 10;
}
int TireOptionManager::get_temp_limit() {
    return temp_limit;
}
int TireOptionManager::get_pressure_lower() {
    return pressure_lower;
}
int TireOptionManager::get_pressure_upper() {
    return pressure_upper;
}
void TireOptionManager::add_temp_limit() {
    temp_limit = std::min(temp_limit + 1, convertTemperature(TEMP_MAX, E_TEMP_FAHRENHEIT, get_tire_temp_unit()));
}
void TireOptionManager::dec_temp_limit() {
    temp_limit = std::max(temp_limit - 1, convertTemperature(TEMP_MIN, E_TEMP_FAHRENHEIT, get_tire_temp_unit()));
}
void TireOptionManager::add_pressure_upper() {
    pressure_upper = std::min(pressure_upper + pressure_step, convertPressure(PRESSURE_MAX, E_PRESSURE_PSI, get_tire_pressure_unit()));
}
void TireOptionManager::dec_pressure_upper() {
    pressure_upper = std::max(pressure_upper - pressure_step, pressure_lower);
}
void TireOptionManager::add_pressure_lower() {
    pressure_lower = std::min(pressure_lower + pressure_step, pressure_upper);
}
void TireOptionManager::dec_pressure_lower() {
    pressure_lower = std::max(pressure_lower - pressure_step, convertPressure(PRESSURE_MIN, E_PRESSURE_PSI, get_tire_pressure_unit()));
}
void TireOptionManager::set_tire_pressure_unit(pressure_unit_e unit) {
	if (pressure_unit == unit) return ;
	pressure_lower = convertPressure(pressure_lower, pressure_unit, unit);
	pressure_upper = convertPressure(pressure_upper, pressure_unit, unit);
	pressure_step = (unit == E_PRESSURE_PSI) ? 100 : 10;
	pressure_unit = unit;
}
pressure_unit_e TireOptionManager::get_tire_pressure_unit() {
	return pressure_unit;
}
void TireOptionManager::set_tire_temp_unit(temp_unit_e unit) {
	if (temp_unit == unit) return ;
	temp_limit = convertTemperature(temp_limit, temp_unit, unit);
	temp_unit = unit;
}
temp_unit_e TireOptionManager::get_tire_temp_unit() {
	return temp_unit;
}
// 胎压配置信息保存数据 胎压单位(1Byte) + 温度单位(1Byte) + 胎压上限(2Byte) + 胎压下限(2Byte) + 温度上限(2Byte)
void TireOptionManager::save_option() {
	// 保存配置信息
	std::string buffer;
	buffer.reserve(8);
	buffer.push_back((uint8_t)pressure_unit);
	buffer.push_back((uint8_t)temp_unit);
	buffer.push_back((uint8_t)(pressure_upper >> 8) & 0xFF);
	buffer.push_back((uint8_t)(pressure_upper) & 0xFF);
	buffer.push_back((uint8_t)(pressure_lower >> 8) & 0xFF);
	buffer.push_back((uint8_t)(pressure_lower) & 0xFF);
	buffer.push_back((uint8_t)(temp_limit >> 8) & 0xFF);
	buffer.push_back((uint8_t)(temp_limit) & 0xFF);
	sys::setting::set_tire_option_info(buffer);
}
void TireOptionManager::reset_option() {
	// 重置配置信息
	pressure_unit = E_PRESSURE_PSI;
	pressure_lower = 0;
	pressure_upper = 5800;
	pressure_step = 100;
	temp_unit = E_TEMP_CELSIUS;
	temp_limit = 60;
}

int TireOptionManager::convertTemperature(int value, temp_unit_e fromUnit, temp_unit_e targetUnit) {
	if (fromUnit == targetUnit) return value;
	if (fromUnit == E_TEMP_CELSIUS) {
		return round((float)value * 9 / 5 + 32);
	} else {
		return round((float)(value - 32) * 5 / 9);
	}
}
int TireOptionManager::convertPressure(int value, pressure_unit_e fromUnit, pressure_unit_e targetUnit) {
	if (fromUnit == targetUnit) return value;
	if (fromUnit == E_PRESSURE_BAR) return  value * 14.5;
	else return value / 14.5;
}

namespace tire {
// 解析胎压数据
// 0C FF 01 1E 14 00AF ABEF3465DFEA
void parseTirePressureProtocol(const unsigned char *data, unsigned int len) {
	char tmp[4] = {0};
	std::string str;
	for(int level1 = 0; level1 < len; ++level1) {
		snprintf(tmp, sizeof(tmp), "%.2X ", data[level1]);
		str.append(tmp);
	}
	LOGD("[tire] adv:%s  len(%d) \n",str.c_str(), len);
	// 数据+mac地址共11字节
	if (len < 11) return ;
	// 判断是否为胎压数据
	LOGD("[tire] get tire data");
	tire_dev_t tire_info;
	tire_info.dev_state = data[0];
	tire_info.voltage = data[1] * 0.1;
	tire_info.temperature = data[2];
	if (tire_info.temperature > 127) tire_info.temperature -= 256;
	float barometric = (int)((data[3] << 8) | data[4]) * 0.1;
	tire_info.barometric = (barometric >= STANDARD_ATMOSPHERIC_PRESSURE) ? barometric - STANDARD_ATMOSPHERIC_PRESSURE : 0.0;
	tire_info.addr = std::string((const char*)(data + 5), 6);
	tire_info.update_time = fy::time::uptime();
#if 1
	LOGD("tire_info.voltage = %.1f", tire_info.voltage);
	LOGD("tire_info.temperature = %d", tire_info.temperature);
	LOGD("tire_info.barometric = %.1f", tire_info.barometric);
	LOGD("tire_info.mac = %02X %02X %02X %02X %02X %02X", tire_info.addr[0], tire_info.addr[1], tire_info.addr[2],
			 	 	 	 	 	 	 	 	 	 	 tire_info.addr[3], tire_info.addr[4], tire_info.addr[5]);
#endif
	bool is_matching = false;
	bool is_new_dev = false;
	{
		Mutex::Autolock _l(_s_tire_info_mutex);
		if (tire_info.addr == _s_tire_info.front_tire_info.addr) {
			_s_tire_info.front_tire_info = tire_info;
			_s_tire_info.front_tire_info.bind_state = E_BT_TIRE_BIND_STATE_CONNECTED;
			is_matching = true;
		} else if (tire_info.addr == _s_tire_info.rear_tire_info.addr) {
			_s_tire_info.rear_tire_info = tire_info;
			_s_tire_info.rear_tire_info.bind_state = E_BT_TIRE_BIND_STATE_CONNECTED;
			is_matching = true;
		}

		auto it = _s_tire_info.tire_dev_map.find(tire_info.addr);
		if (it != _s_tire_info.tire_dev_map.end()) {
			it->second = tire_info;
		} else {
			_s_tire_info.tire_dev_map.emplace(tire_info.addr, tire_info);
			is_new_dev = true;
		}
	}
	if (is_matching) _notify_tire_misc_info_cb(E_MISC_INFO_TIRE_BIND_DEV_UPDATE);
	if (is_new_dev) _notify_tire_misc_info_cb(E_MISC_INFO_TIRE_DEV_MAP_UPDATE);
}

void add_tire_info_update_cb(tire_misc_info_cb cb) {
	Mutex::Autolock _l(_s_tire_cb_mutex);
	_s_tire_cb_set.insert(cb);
}

void remove_tire_info_update_cb(tire_misc_info_cb cb) {
	Mutex::Autolock _l(_s_tire_cb_mutex);
	_s_tire_cb_set.erase(cb);
}

bool get_front_tire_info(tire_dev_t& tire_info) {
	Mutex::Autolock _l(_s_tire_info_mutex);
	if (_s_tire_info.front_tire_info.addr != "") {
		tire_info = _s_tire_info.front_tire_info;
		return true;
	}
	return false;
}

bool get_rear_tire_info(tire_dev_t& tire_info) {
	Mutex::Autolock _l(_s_tire_info_mutex);
	if (_s_tire_info.rear_tire_info.addr != "") {
		tire_info = _s_tire_info.rear_tire_info;
		return true;
	}
	return false;
}

void match_front_tire_dev(tire_dev_t& tire_info) {
	{
		Mutex::Autolock _l(_s_tire_info_mutex);
		_s_tire_info.front_tire_info = tire_info;
		if (_s_tire_info.rear_tire_info.addr == tire_info.addr) _s_tire_info.rear_tire_info = tire_dev_t();
		_s_tire_info.front_tire_info.bind_state = E_BT_TIRE_BIND_STATE_CONNECTED;
	}
	_notify_tire_misc_info_cb(E_MISC_INFO_TIRE_BIND_DEV_UPDATE);
}

void match_rear_tire_dev(tire_dev_t& tire_info) {
	{
		Mutex::Autolock _l(_s_tire_info_mutex);
		_s_tire_info.rear_tire_info = tire_info;
		if (_s_tire_info.front_tire_info.addr == tire_info.addr) _s_tire_info.front_tire_info = tire_dev_t();
		_s_tire_info.rear_tire_info.bind_state = E_BT_TIRE_BIND_STATE_CONNECTED;
	}
	_notify_tire_misc_info_cb(E_MISC_INFO_TIRE_BIND_DEV_UPDATE);
}

void del_match_front_tire_dev() {
	LOCK_OP(_s_tire_info_mutex, _s_tire_info.front_tire_info.addr = "");
	_notify_tire_misc_info_cb(E_MISC_INFO_TIRE_BIND_DEV_UPDATE);
}

void del_match_rear_tire_dev() {
	LOCK_OP(_s_tire_info_mutex, _s_tire_info.rear_tire_info.addr = "");
	_notify_tire_misc_info_cb(E_MISC_INFO_TIRE_BIND_DEV_UPDATE);
}

void check_tire_info_valid_time() {
	long long current_time = fy::time::uptime();
	bool is_match_dev_invalid = false;
	{
		Mutex::Autolock _l(_s_tire_info_mutex);
		for (auto it = _s_tire_info.tire_dev_map.begin(); it != _s_tire_info.tire_dev_map.end();) {
			if (current_time - it->second.update_time > TIRE_INFO_TIMEOUT) {
				it = _s_tire_info.tire_dev_map.erase(it);
			} else {
				++ it;
			}
		}
		if ((_s_tire_info.front_tire_info.addr != "") && \
				(current_time - _s_tire_info.front_tire_info.update_time > TIRE_INFO_TIMEOUT)) {
			_s_tire_info.front_tire_info.bind_state = E_BT_TIRE_BIND_STATE_NOTFOUND;
			is_match_dev_invalid = true;
		}
		if ((_s_tire_info.rear_tire_info.addr != "") && \
				(current_time - _s_tire_info.rear_tire_info.update_time > TIRE_INFO_TIMEOUT)) {
			_s_tire_info.rear_tire_info.bind_state = E_BT_TIRE_BIND_STATE_NOTFOUND;
			is_match_dev_invalid = true;
		}
	}
	if (is_match_dev_invalid) _notify_tire_misc_info_cb(E_MISC_INFO_TIRE_BIND_DEV_UPDATE);
	_notify_tire_misc_info_cb(E_MISC_INFO_TIRE_DEV_MAP_UPDATE);
}

static std::string _serialize_tire_data(tire_dev_t& tire_info) {
	if (tire_info.addr == "") return "";
	std::string buffer;
	buffer.reserve(12);
	buffer.append(tire_info.addr);
	buffer.push_back(uint8_t(tire_info.dev_state));
	buffer.push_back(uint8_t(tire_info.bind_state));
	buffer.push_back(uint8_t(tire_info.temperature + 128));
	uint16_t front_barometric = tire_info.barometric * 10;
	buffer.push_back(uint8_t((front_barometric >> 8) & 0xFF));
	buffer.push_back(uint8_t(front_barometric & 0xFF));
	buffer.push_back(uint8_t(tire_info.voltage * 10));
	LOGD("[tire] data size = %ld", buffer.size());
	return buffer;
}

static tire_dev_t _deserialize_tire_data(const std::string& data) {
	if (data.size() < 12) return tire_dev_t();
	tire_dev_t tire_info;
	tire_info.addr = data.substr(0, 6);
	tire_info.dev_state = (uint8_t)data[6];
	tire_info.bind_state = (bt_tire_bind_state_e)data[7];
	tire_info.temperature = (uint8_t)data[8] - 128;
	tire_info.barometric = (uint16_t)((data[9] << 8) | data[10]) * 0.1;
	tire_info.voltage = (uint8_t)data[11] * 0.1;
#if 1
	LOGD("tire_info.voltage = %.1f", tire_info.voltage);
	LOGD("tire_info.temperature = %d", tire_info.temperature);
	LOGD("tire_info.barometric = %.1f", tire_info.barometric);
	LOGD("tire_info.bind_state = %d", tire_info.bind_state);
	LOGD("tire_info.mac = %02X %02X %02X %02X %02X %02X", tire_info.addr[0], tire_info.addr[1], tire_info.addr[2],
			 	 	 	 	 	 	 	 	 	 	 tire_info.addr[3], tire_info.addr[4], tire_info.addr[5]);
#endif
	return tire_info;
}

// 胎压信息保存数据 mac地址(6Byte) + 设备状态(1Byte) + 绑定状态(1Byte) + 温度(1Byte)℃  + 压力(2Byte)psi + 电压(1Byte)
void save_tire_data() {
	tire_dev_t front_info;
	tire_dev_t rear_info;
	get_front_tire_info(front_info);
	get_rear_tire_info(rear_info);
	std::string front_buffer = _serialize_tire_data(front_info);
	std::string rear_buffer = _serialize_tire_data(rear_info);
	sys::setting::set_front_tire_data(front_buffer);
	sys::setting::set_rear_tire_data(rear_buffer);
}

void tire_data_init() {
	Mutex::Autolock _l(_s_tire_info_mutex);
	std::string front_data = sys::setting::get_front_tire_data();
	std::string rear_data = sys::setting::get_rear_tire_data();
	_s_tire_info.front_tire_info =_deserialize_tire_data(front_data);
	_s_tire_info.rear_tire_info = _deserialize_tire_data(rear_data);
}

uint32_t get_scan_tire_dev_sise() {
	Mutex::Autolock _l(_s_tire_info_mutex);
	return _s_tire_info.tire_dev_map.size();
}

bool get_scan_tire_dev_by_index(int index, tire_dev_t &dev) {
	Mutex::Autolock _l(_s_tire_info_mutex);
	auto it = _s_tire_info.tire_dev_map.begin();
	for (int i = 0; it != _s_tire_info.tire_dev_map.end(); ++i, ++it) {
		if (i == index) {
			dev = it->second;
			return true;
		}
	}
	return false;
}

}



