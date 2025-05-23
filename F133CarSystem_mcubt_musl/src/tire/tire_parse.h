/*
 * tire_parse.h
 *
 *  Created on: 2024年12月5日
 *      Author: zkswe
 */

#ifndef MISC_TIRE_PARSE_H_
#define MISC_TIRE_PARSE_H_

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <fy/time.hpp>

typedef enum {
	E_PRESSURE_BAR,
	E_PRESSURE_PSI,
	E_PRESSURE_MAX,
} pressure_unit_e;

typedef enum {
	E_TEMP_CELSIUS,
	E_TEMP_FAHRENHEIT,
	E_TEMP_MAX,
} temp_unit_e;

typedef enum {
	E_BT_TIRE_DEV_STATE_LEAKAGE = 2, // 漏气
	E_BT_TIRE_DEV_STATE_LOW_VOLTAGE = 4, // 低电压
} bt_tire_dev_state_e;

typedef enum {
	E_BT_TIRE_BIND_STATE_UNMATCH,	// 未匹配
	E_BT_TIRE_BIND_STATE_NOTFOUND,  // 未找到设备
	E_BT_TIRE_BIND_STATE_CONNECTED, // 已连接
} bt_tire_bind_state_e;

typedef enum {
	E_MISC_INFO_TIRE_BIND_DEV_UPDATE, // 匹配的胎压设备信息更新
	E_MISC_INFO_TIRE_DEV_MAP_UPDATE,  // 胎压扫描列表更新
} tire_misc_info_e;

class TireOptionManager {
public:
	static TireOptionManager &getInstance() {
		static TireOptionManager singleton;
		return singleton;
	}
	TireOptionManager(const TireOptionManager&) = delete;
	TireOptionManager& operator=(const TireOptionManager&) = delete;
public:
	void initialize();
	int get_temp_limit();
	// 获取的胎压值为 实际值*100
	int get_pressure_lower();
	int get_pressure_upper();
	void add_temp_limit();
	void dec_temp_limit();
	void add_pressure_upper();
	void dec_pressure_upper();
	void add_pressure_lower();
	void dec_pressure_lower();
	void set_tire_pressure_unit(pressure_unit_e unit);
	pressure_unit_e get_tire_pressure_unit();
	void set_tire_temp_unit(temp_unit_e unit);
	temp_unit_e get_tire_temp_unit();
	void save_option();
	void reset_option();
	// 胎压  温度 单位转换函数 (转换后的值需要四舍五入)
	static int convertPressure(int value, pressure_unit_e fromUnit, pressure_unit_e targetUnit);
	static int convertTemperature(int value, temp_unit_e fromUnit, temp_unit_e targetUnit);

private:
	TireOptionManager(){}
private:
	const int TEMP_MAX = 200;
	const int TEMP_MIN = 60;
	const int PRESSURE_MAX = 5800; // psi * 100
	const int PRESSURE_MIN = 0;
	int temp_limit;
	int pressure_lower;
	int pressure_upper;
	int pressure_step; // 胎压自增/自减步长
	pressure_unit_e pressure_unit;
	temp_unit_e temp_unit;
};

struct tire_dev_t{
	std::string addr;
	uint8_t dev_state;
	bt_tire_bind_state_e bind_state;
    float barometric;   // 气压 uint0.1 psi  0xAF = 17.5psi
    int temperature;  // 温度 uint 1℃   0x14 = 20℃ 0xEC = -20℃
    float voltage;      // 电压 num * 100mv 0x1E = 3.0V
    long long update_time;
    tire_dev_t() : addr(""),
    		dev_state(0),
			bind_state(E_BT_TIRE_BIND_STATE_UNMATCH),
	        temperature(0),
			barometric(0.0f),
	        voltage(0.0f),
	        update_time(fy::time::uptime()) {} // 默认设置为当前时间
};

typedef void (*tire_misc_info_cb)(tire_misc_info_e info); // 胎压更新回调信息

namespace tire {
//void parseCommonProtocol(const char *data, int len);
void parseTirePressureProtocol(const unsigned char *data, unsigned int len);
void add_tire_info_update_cb(tire_misc_info_cb cb);
void remove_tire_info_update_cb(tire_misc_info_cb cb);

bool get_front_tire_info(tire_dev_t& tire_info);
bool get_rear_tire_info(tire_dev_t& tire_info);

void match_front_tire_dev(tire_dev_t& tire_info);
void match_rear_tire_dev(tire_dev_t& tire_info);
void del_match_front_tire_dev();
void del_match_rear_tire_dev();
void check_tire_info_valid_time();
void save_tire_data();
void tire_data_init();

uint32_t get_scan_tire_dev_sise();
bool get_scan_tire_dev_by_index(int index, tire_dev_t &dev);
}

#endif /* MISC_TIRE_PARSE_H_ */
