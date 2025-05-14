#include "entry/EasyUIContext.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

void onEasyUIInit(EasyUIContext *pContext) {
	setenv("AW_CEDARX_CONF", "/res/bin/cedarx.conf", 0);
}

void onEasyUIDeinit(EasyUIContext *pContext) {

}

const char* onStartupApp(EasyUIContext *pContext) {
	return "mainActivity";
}

#ifdef __cplusplus
}
#endif  /* __cplusplus */
