/*
 * storage.cpp
 *
 *  Created on: Mar 5, 2023
 *      Author: guoxs
 */

#include "security/SecurityManager.h"
#include "utils/JsonHelper.h"
#include "system/Mutex.h"
#include "storage.h"
#include "common.h"

namespace storage {

static Mutex _s_lock;

#define ZKAPP_CFG        "/data/zkapp.cfg"

#define PUT_VAL(key, val, asType) \
	Mutex::Autolock _l(_s_lock); \
	Json::Value json; \
	JsonHelper::readJsonFile(ZKAPP_CFG, json); \
	if (!json.isMember(key) || (json[key].asType() != val)) { \
		json[key] = val; \
		return JsonHelper::writeJsonFile(ZKAPP_CFG, json); \
	} \
	return true;

#define GET_VAL(key, defVal, isType, asType) \
	Mutex::Autolock _l(_s_lock); \
	Json::Value json; \
	if (JsonHelper::readJsonFile(ZKAPP_CFG, json) && json.isMember(key) && json[key].isType()) { \
		return json[key].asType(); \
	} \
	return defVal;

bool put_string(const std::string &key, const std::string &val) {
	PUT_VAL(key, val, asString);
}

bool put_int(const std::string &key, int val) {
	PUT_VAL(key, val, asInt);
}

bool put_bool(const std::string &key, bool val) {
	PUT_VAL(key, val, asBool);
}

bool put_float(const std::string &key, float val) {
	PUT_VAL(key, val, asFloat);
}

bool remove(const std::string &key) {
	Mutex::Autolock _l(_s_lock);
	Json::Value json;
	if (JsonHelper::readJsonFile(ZKAPP_CFG, json) && json.isMember(key)) {
		json.removeMember(key);
		return JsonHelper::writeJsonFile(ZKAPP_CFG, json);
	}
	return true;
}

bool clear() {
	Mutex::Autolock _l(_s_lock);
	return (::remove(ZKAPP_CFG) == 0);
}

std::string get_string(const std::string &key, const std::string &defVal) {
	GET_VAL(key, defVal, isString, asString);
}

int get_int(const std::string &key, int defVal) {
	GET_VAL(key, defVal, isInt, asInt);
}

bool get_bool(const std::string &key, bool defVal) {
	GET_VAL(key, defVal, isBool, asBool);
}

float get_float(const std::string &key, float defVal) {
	GET_VAL(key, defVal, isNumeric, asFloat);
}


#define TRY_COUNT          10
#define MAX_DATA_SIZE      255

static bool _otp_read_data_core(uint8_t *data, uint32_t size) {
	Mutex::Autolock _l(_s_lock);
	for (int i = 0; i < TRY_COUNT; ++i) {
		if (SECURITYMANAGER->readSecData(data, size) == (int) size) {
			return true;
		}

		LOGE("_otp_read_data_core err %d\n", i);
		DELAY(50);
	}
	return false;
}

static bool _otp_write_data_core(uint8_t *data, uint32_t size) {
	Mutex::Autolock _l(_s_lock);
	if (SECURITYMANAGER->isSecDataLock()) {
		LOGE("otp page 3 is lock, can't write data!!!\n");
		return false;
	}

	for (int i = 0; i < TRY_COUNT; ++i) {
		if (SECURITYMANAGER->writeSecData(data, size) == (int) size) {
			return true;
		}

		LOGE("_otp_write_data_core err %d\n", i);
		DELAY(50);
	}
	return false;
}

bool otp_read_data(int offset, uint8_t *data, uint32_t size) {
	if (offset + size > MAX_DATA_SIZE) {
		return false;
	}

	uint8_t otp_data[MAX_DATA_SIZE];
	if (!_otp_read_data_core(otp_data, MAX_DATA_SIZE)) {
		return false;
	}

	memcpy(data, otp_data + offset, size);
	return true;
}

bool otp_write_data(int offset, uint8_t *data, uint32_t size) {
	if (offset + size > MAX_DATA_SIZE) {
		return false;
	}

	uint8_t otp_data[MAX_DATA_SIZE];
	if (!_otp_read_data_core(otp_data, MAX_DATA_SIZE)) {
		return false;
	}

	memcpy(otp_data + offset, data, size);
	return _otp_write_data_core(otp_data, MAX_DATA_SIZE);
}

bool otp_load_data_to_file(int offset, uint32_t size, const char *file) {
	uint8_t otp_data[size];
	if (!otp_read_data(offset, otp_data, size)) {
		LOGE("otp_read_data fail !!!\n");
		return false;
	}

    FILE *pf = fopen(file, "wb");
    if (!pf) {
    	LOGE("otp open %s fail\n", file);
    	return false;
    }

    bool ret = fwrite(otp_data, 1, size, pf) == size;
    if (!ret) {
    	LOGE("otp_load_data_to_file %s fail\n", file);
    }

    fclose(pf);

    return ret;
}

}
