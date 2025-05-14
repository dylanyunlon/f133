/*
	深圳辽原科技有限公司
	Shenzhen LiaoYuan Technology Limited company
	
	简介：本软件用于车机集成正向Carplay/AndroidAuto/Carlife等手机互联技术
	
	作者：陈工

	创建日期：2021/08/30
*/


#ifndef __LY_LINK_API_H__
#define __LY_LINK_API_H__


#ifdef __cplusplus
extern "C" {
#endif


typedef enum LinkErrCode_e {
	LINK_ERRCODE_NONE,
	LINK_ERRCODE_VERIFY,
	LINK_ERRCODE_MFI,
	LINK_ERRCODE_DISC
} LYLINK_ERRCODE;

typedef enum LinkType_e{
	LINK_TYPE_NULL = 0,
	LINK_TYPE_WIFICP,
	LINK_TYPE_USBCP,
	LINK_TYPE_USBAUTO,
	LINK_TYPE_WIFIAUTO,
	LINK_TYPE_USBLIFE,
	LINK_TYPE_WIFILIFE,
	LINK_TYPE_USBHICAR,
	LINK_TYPE_WIFIHICAR,
	LINK_TYPE_APPLELIFE,
	LINK_TYPE_AIRPLAY,
	LINK_TYPE_MIRACAST,
	LINK_TYTP_IOSMIRROR,
	LINK_TYPE_USBLY,
	LINK_TYPE_WIFILY,
} LYLINK_TYPE_E;

typedef enum lylink_driver_pos_e {
	DrivingPosition_Left,
	DrivingPosition_Right
} LYLINK_DRIVER_POS;

typedef enum lylink_night_mode_e {
	LYLINK_NIGHTMODE_DEFAULT,
	LYLINK_NIGHTMODE_DAY,
	LYLINK_NIGHTMODE_NIGHT
} LYLINK_NIGHTMODE_E;

#define WIFIFLAG_CARPLAY	(1 << LINK_TYPE_WIFICP)
#define WIFIFLAG_ANDROID	(1 << LINK_TYPE_WIFIAUTO)
#define WIFIFLAG_CARLIFE	(1 << LINK_TYPE_WIFILIFE)
#define WIFIFLAG_HICAR		(1 << LINK_TYPE_WIFIHICAR)
#define WIFIFLAG_AIRPLAY	(1 << LINK_TYPE_AIRPLAY)
#define WIFIFLAG_MIRACAST	(1 << LINK_TYPE_MIRACAST)
#define WIFIFLAG_LYLINK		(1 << LINK_TYPE_WIFILY)
#define USBFLAG_CARPLAY		(1 << LINK_TYPE_USBCP)
#define USBFLAG_ANDROID		(1 << LINK_TYPE_USBAUTO)
#define USBFLAG_CARLIFE		((1 << LINK_TYPE_USBLIFE) | (1 << LINK_TYPE_APPLELIFE))
#define USBFLAG_HICAR		(1 << LINK_TYPE_USBHICAR)
#define USBFLAG_IOSMIRROR	(1 << LINK_TYTP_IOSMIRROR)
#define USBFLAG_LYLINK		(1 << LINK_TYPE_USBLY)
#define USBFLAG_VERIFY		(0x80000000)											// 启用AOA激活授权
#define USBFLAG_AOA			(USBFLAG_ANDROID | USBFLAG_CARLIFE | USBFLAG_VERIFY)  	// 启用所有AOA,包括授权,carlife和android auto

typedef enum TouchMode_e
{
	TouchMode_Down = 0,
	TouchMode_Up,
	TouchMode_Move
} LYLINK_TOUCHMODE_E;

typedef enum KeyMode_e
{
	KeyMode_Down = 0,
	KeyMode_Up,
} LYLINK_KEYMODE_E;

typedef enum MouseMode_e
{
	MouseMode_Down = 0,
	MouseMode_Up,
	MouseMode_Move
} LYLINK_MOUSEMODE_E;

typedef enum AudioDuck_e
{
	AudioUnduck = 0,
	AudioDuck
} LYLINK_DUCKMODE_E;

typedef enum AudioType_e
{
	AudioType_Default = 0,
	AudioType_Media,  // 音乐
	AudioType_VR,  // 语音
	AudioType_TTS,  // 导航
	AudioType_Phone,  // 电话
	AudioType_Max,
	AudioType_BtMusic = 0xFD,
	AudioType_Native = 0xFE,
	AudioType_Invalid = 0xFF
} LYLINK_AUDIOTYPE_E;

typedef enum CallStateFocus_e
{
	CallState_Hang = 0,
	CallState_Ring,
	CallState_Conn
} LYLINK_CALLSTATEFOCUS_E;

typedef enum ly_bt_cmd_type_e {
	LYLINK_BTCMD_RFCOMM,
	LYLINK_BTCMD_GOCSDK
} LyBtCmdType;

typedef enum KeyCode_e
{
	LY_ConsumerActionACHome = 0x100,
	LY_ConsumerActionACBack,
	LY_ConsumerActionRecord,
	LY_ConsumerActionScanNext,
	LY_ConsumerActionScanPrev,
	LY_ConsumerActionPlay,
	LY_ConsumerActionPause,
	LY_ConsumerActionPlayOrPause,
	LY_ConsumerActionMedia,
	LY_ConsumerActionNav,
	LY_ConsumerActionRadio,
	LY_ConsumerActionTel,

	LY_TelePhonyActionHold = 0x200,
	LY_TelePhonyActionDial,
	LY_TelePhonyActionHang,
	LY_TelePhonyActionMute,
	LY_TelePhonyActionPhoneKey0,
	LY_TelePhonyActionPhoneKey1,
	LY_TelePhonyActionPhoneKey2,
	LY_TelePhonyActionPhoneKey3,
	LY_TelePhonyActionPhoneKey4,
	LY_TelePhonyActionPhoneKey5,
	LY_TelePhonyActionPhoneKey6,
	LY_TelePhonyActionPhoneKey7,
	LY_TelePhonyActionPhoneKey8,
	LY_TelePhonyActionPhoneKey9,
	LY_TelePhonyActionPhoneKeyStar,
	LY_TelePhonyActionPhoneKeyPound,
	LY_TelePhonyActionKeyboardDelete,

	LY_VRAction = 0x300,
	LY_VRLongPress,

	LY_knobActionLeft = 0x400,
	LY_knobActionRight,
	LY_knobActionUp,
	LY_knobActionDown,
	LY_knobActionSelect,
	LY_knobActionHome,
	LY_knobActionBack,
	LY_knobActionCounterClockWise,  // 逆时针旋转，不区分按下和抬起动作
	LY_knobActionClockWise,  // 顺时针旋转，不区分按下和抬起动作
	
	LY_dpadActionLeft = 0x500,
	LY_dpadActionRight,
	LY_dpadActionUp,
	LY_dpadActionDown,
	
	LY_forceKeyFrame = 0x600,
	LY_BLE
} LYLINK_KEYCODE_E;

typedef struct lylinkapi_audio_param_t {
	unsigned short sampleRate;
	unsigned char channelNum;
	unsigned char bitWidth;
} LYLINKAPI_AUDIO_PARAM;

typedef struct lylinkapi_video_param_t {
	unsigned short width;
	unsigned short height;
} LYLINKAPI_VIDEO_PARAM;

typedef struct lylinkapi_time_t {
	signed char dayLight;
	signed short zone;
	long long time;
} LYLINKAPI_TIME;

/* 回调消息，未作特殊说明时，一般para0为字节数，para1为数据指针 */
typedef enum lylinkapi_event_e {
	/* 
		LYLINK_TYPE_E para0 
		注意：收到LYLINK_PHONE_CONNECT消息para0为LINK_TYPE_WIFICP或LINK_TYPE_WIFIAUTO时，需启用AP模式
	*/
	LYLINK_LINK_ESTABLISH = 1,
	LYLINK_LINK_DISCONN,
	LYLINK_PHONE_CONNECT,
	LYLINK_PHONE_DISCONN,
	
	LYLINK_FOREGROUND = 5,
	LYLINK_BACKGROUND,
	
	/* const LYLINKAPI_VIDEO_PARAM *para1 */
	LYLINK_VIDEO_START = 7,
	LYLINK_VIDEO_STOP,
	
	/* LYLINK_AUDIOTYPE_E para0, const LYLINKAPI_AUDIO_PARAM* para1, para1仅在NOMIX模式有效 */
	LYLINK_AUDIO_START = 9,
	LYLINK_AUDIO_STOP,
	
	LYLINK_RECORD_START = 11,
	LYLINK_RECORD_STOP,

	/* const LYLINKAPI_AUDIO_PARAM* para1 */
	LYLINK_RECORD_INIT,
	
	/* 视频数据,该数据内存由lylink管理 */
	LYLINK_VIDEO_HEAD,  
	LYLINK_VIDEO_DATA,

	/* NODEC模式回调此消息,播放器阻塞模式,混音成一路48k双声道,请无视LYLINK_AUDIO_START参数,该数据内存由lylink管理 */
	LYLINK_AUDIO_DATA,
	
	LYLINK_HICAR_PINCODE,
	
	/* 
		LYLINK_ERRCODE para0 
		const char* para1 : e.g. V2.00.00
		如果支持无线，此时需要回复wifi状态，有两种情况：
		1. AP模式下，发送LYLINK_WIFI_AP_OPENED以及每一个终端的LYLINK_WIFI_AP_CONNECTED
		2. STA模式下，发送LYLINK_WIFI_STA_CONNECTED
		3. 自动模式，发送LYLINK_WIFI_MONITOR，同一进程发送一次即可
		注意：此时无需发送LYLINK_WIFI_AP_CLOSED和LYLINK_WIFI_STA_DISCONNECTED
	*/
	LYLINK_VERSION_CODE,

	LYLINK_HID_COMMAND,

	/* LYLINK_CALLSTATEFOCUS_E para0 */
	LYLINK_CALL_STATE,

	/* NOMIX模式下四种音频类型数据,禁止阻塞,请转存后送给播放器播放,该数据内存由客户自行管理,调用lylinkapi_free释放 */
	LYLINK_MEDIA_DATA,
	LYLINK_VR_DATA,
	LYLINK_TTS_DATA,
	LYLINK_PHONE_DATA,

	/* para0 = 1: duck, 0: unduck*/
	LYLINK_AUDIO_DUCK,

	LYLINK_BT_RFCOMM,
	LYLINK_BT_GOCSDK,

	LYLINK_WIFI_REQUEST,

	/* const LYLINKAPI_TIME *time */
	LYLINK_UPDATE_TIME,

	/* para0 = length */
	LYLINK_MEDIA_SONG,
	LYLINK_MEDIA_ARTIST,
	LYLINK_MEDIA_ALBUM,
	LYLINK_MEDIA_ALBUMART,

	/* para0 = value */
	LYLINK_MEDIA_DURATION,
	LYLINK_MEDIA_PROGRESS,
	LYLINK_MEDIA_VOLUME,

	LYLINK_LICENSE_KEY
} LYLINKAPI_EVENT;

typedef enum lylinkapi_cmd_e {
	/* 切换本地音频命令 */
	LYLINK_AUDIO_NATIVE,
	/* 切换互联音频命令 */
	LYLINK_AUDIO_LYLINK,
	/* 切换本地画面命令 */
	LYLINK_VIDEO_NATIVE,
	/* 切换互联画面命令 */
	LYLINK_VIDEO_LYLINK,
	/* 切换白天/黑夜模式,暂时仅支持Carplay&Android Auto */
	LYLINK_CMD_DAYMODE,
	LYLINK_CMD_NIGHTMODE,
	/* 断开所有手机无线连接,此命令需要开启Wifi自动监控模式 */
	LYLINK_DISCONNECT_AP,
	/* 4种音源切换,此功能需要内部解码方式,并实现liblylinkav.so */
	LYLINK_AUDIOSOURCE_NATIVE,
	LYLINK_AUDIOSOURCE_BT,
	LYLINK_AUDIOSOURCE_LYLINK,
	LYLINK_AUDIOSOURCE_PHONE,
	/* 检查授权 */
	LYLINK_CHECK_VERIFY,

	LYLINK_DISCONNECT_CARPLAY
} LYLINKAPI_CMD;

/* 音视频流处理方式 */
typedef enum lylinkapi_start_mode_e {
	LYLINK_START_ASYNC_DECODE,  // 异步方式内部解码，不推荐
	LYLINK_START_SYNC_DECODE,  // deprecated
	LYLINK_START_ASYNC_NODEC,  // 异步方式回调方式，带混音
	LYLINK_START_SYNC_NODEC,  // deprecated
	LYLINK_START_ASYNC_NOMIX,  // 异步方式回调方式，不带混音
	LYLINK_START_SYNC_NOMIX  // deprecated
} LYLINKAPI_STARTMODE;

typedef enum lylinkapi_start_flags_e {
	LYLINK_FLAGS_DIRECT_TOUCH = 0x01,  // 不转换触摸坐标
	LYLINK_FLAGS_SHOW_SELECTED = 0x02,  // 显示选中区域
	LYLNIK_FLAGS_SET_MFI_I2C = 0x04,  // 配置mFi芯片
	LYLINK_FLAGS_SET_AUTH_I2C = 0x08,  // 配置加密芯片
	LYLINK_FLAGS_VIDEO_LYLINK = 0x10,  // 配置自动推视频流
	LYLINK_FLAGS_VIDEO_NATIVE = 0x20,  // 配置手动推视频流
	LYLINK_FLAGS_AMPLIFY_CALL = 0x40,  // 配置放大CarPlay通话声音
	LYLINK_FLAGS_SET_LYLINKOS = 0x80,  // 覆盖os接口
	LYLINK_FLAGS_MANUAL_BTCON = 0x100,  // 不自动连接auto蓝牙
	LYLINK_FLAGS_CARPLAY_AVCC = 0x200,	// CarPlay视频采用h264 avcc
	LYLINK_FLAGS_BTRFCOMM_EXT = 0x400,  // 不打开/dev/rf_serial
	LYLINK_FLAGS_ANDROID_HD = 0x800,    // android auto采用高分辨率
	LYLINK_FLAGS_FPS_60 = 0x1000,       // fps = 60
	LYLINK_FLAGS_VIDEO_ONLY = 0x2000,   // 只有视频，目前仅支持CarPlay,Auto
	LYLINK_FLAGS_NOT_RECONNECT = 0x4000 // 不自动回连蓝牙
} LYLINKAPI_STARTFLAGS;

typedef struct lylinkapi_conf_t {
	unsigned int mfi:3;  // /dev/i2c-n
	unsigned int mfiaddr:1;	// 0: 0x10, 1: 0x11 
	unsigned int auth:3;  // /dev/i2c-n
	unsigned int amplify:3;  // 0: 1.5x, 7: 5x 通话放大倍数
	unsigned int reserved:22;
} LYLINKAPI_CONF;

typedef struct lylinkapi_param_t {
	unsigned short width;  // 屏幕像素宽度
	unsigned short height;  // 屏幕像素高度
	LYLINK_DRIVER_POS pos;  // 方向盘位置
	LYLINKAPI_STARTMODE mode;  // 启动模式，建议使用LYLINK_START_ASYNC_DECODE
	void (*callback) (LYLINKAPI_EVENT evt, int para0, void *para1);  // 消息回调
	unsigned short physical_width;  // 屏幕物理尺寸宽度
	unsigned short physical_height;  // 屏幕物理尺寸高度
	LYLINK_NIGHTMODE_E night;  // 是否启用夜间模式
	unsigned int funmode;  // e.g. = USBFLAG_CARLIFE | USBFLAG_VERIFY
	char huName[32];
	int flags;  // see LYLINKAPI_STARTFLAGS
	LYLINKAPI_CONF conf;
	struct lylinkos_t *os;
	int reserved;
} LYLINKAPI_PARAM;

typedef enum lylinkapi_wifi_msg_e {
	/* 车机热点已开启 */
	LYLINK_WIFI_AP_OPENED,

	/* 车机热点已关闭 */
	LYLINK_WIFI_AP_CLOSED,

	/* 有设备连上车机热点 */
	LYLINK_WIFI_AP_CONNECTED,

	/* 有设备断开车机热点 */
	LYLINK_WIFI_AP_DISCONNECTED,

	/* 车机连上无线热点 */
	LYLINK_WIFI_STA_CONNECTED,

	/* 车机断开无线热点 */
	LYLINK_WIFI_STA_DISCONNECTED,

	/* 车机自动检测模式 */
	LYLINK_WIFI_MONITOR,

	/* 有设备连上车机热点 */
	LYLINK_WIFI_P2P_CONNECTED,
} LYLINKAPI_WIFI_MSG;

typedef enum lylinkapi_wifi_mode_e {
	LYLINK_AP_MODE,
	LYLINK_AP_5G_MODE = LYLINK_AP_MODE,
	LYLINK_AP_2_4G_MODE = LYLINK_AP_MODE,
	LYLINK_STA_MODE,
	LYLINK_P2P_MODE,
	LYLINK_WIFI_CLOSE = 255
} LYLINKAPI_WIFI_MODE;


/* 注意：AP模式需设置lylinkapp环境变量LOCAL_IP_ADDRESS=本机IP，默认192.168.1.101 */
typedef struct lylinkapi_wifi_ap_opened_t {
	LYLINKAPI_WIFI_MSG id;	// LYLINK_WIFI_AP_OPENED
	char ssid[64];			// 本机热点名
	char interface[64];     // 一般是wlan0, hicar需要填写
	char psk[40];			// 本机热点密码
	char bssid[18];			// bssid 格式为00:11:22:33:44:55，可以填""
	unsigned char channel;	// 信道：一般2.4G填6，5G填36即可
} LYLINKAPI_WIFI_AP_OPENED_PARAM;

typedef struct lylinkapi_wifi_ap_closed_t {
	LYLINKAPI_WIFI_MSG id;	// LYLINK_WIFI_AP_CLOSED
} LYLINKAPI_WIFI_AP_CLOSED_PARAM;

typedef struct lylinkapi_wifi_ap_connect_t {
	LYLINKAPI_WIFI_MSG id;	// LYLINK_WIFI_AP_CONNECTED
	char mac[18];			// AP模式下新增的终端设备的mac
} LYLINKAPI_WIFI_AP_CONN_PARAM;

typedef struct lylinkapi_wifi_p2p_connect_t {
	LYLINKAPI_WIFI_MSG id;	// LYLINK_WIFI_P2P_CONNECTED
	char mac[18];			// GO模式下新增的终端设备的mac
	char wfd_subelems[42];
} LYLINKAPI_WIFI_P2P_CONN_PARAM;

typedef struct lylinkapi_wifi_ap_disconnect_t {
	LYLINKAPI_WIFI_MSG id;	// LYLINK_WIFI_AP_DISCONNECTED
	char mac[18];			// AP模式下断开的终端设备的mac
} LYLINKAPI_WIFI_AP_DISC_PARAM;

typedef struct lylinkapi_wifi_sta_connect_t {
	LYLINKAPI_WIFI_MSG id;	// LYLINK_WIFI_STA_CONNECTED
	char gateway[16];		// STA模式下网关IP地址
} LYLINKAPI_WIFI_STA_CONN_PARAM;

typedef struct lylinkapi_wifi_sta_disconnect_t {
	LYLINKAPI_WIFI_MSG id;	// LYLINK_WIFI_STA_DISCONNECTED
	char gateway[16];		// STA模式下网关IP地址
} LYLINKAPI_WIFI_STA_DISC_PARAM;

typedef struct lylinkapi_wifi_monitor_t {
	LYLINKAPI_WIFI_MSG id;	// LYLINK_WIFI_MONITOR
	LYLINKAPI_WIFI_MODE mode;// 当前工作模式
	char hostapd[80];		// e.g. /var/run/hostapd/wlan0，不支持AP填""
	char wpa_supplicant[80];// e.g. /var/run/wpa_supplicant/wlan0，不支持STA填""
	char hostapd_psk[24];
} LYLINKAPI_WIFI_MONITOR_PARAM;

typedef union lylinkapi_wifi_param_t {
	LYLINKAPI_WIFI_MSG id;
	LYLINKAPI_WIFI_AP_OPENED_PARAM apopen;
	LYLINKAPI_WIFI_AP_CLOSED_PARAM apclose;
	LYLINKAPI_WIFI_AP_CONN_PARAM apconn;
	LYLINKAPI_WIFI_P2P_CONN_PARAM p2pconn;
	LYLINKAPI_WIFI_AP_DISC_PARAM apdisc;
	LYLINKAPI_WIFI_STA_CONN_PARAM staconn;
	LYLINKAPI_WIFI_STA_DISC_PARAM stadisc;
	LYLINKAPI_WIFI_MONITOR_PARAM monitor;
} LYLINKAPI_WIFI_PARAM;


/**
 *	\brief 启动lylink服务
 *
 *	\param params 连接参数
 *
 *  \return 0：成功，其他：失败
 */
int lylinkapi_start(const LYLINKAPI_PARAM *params);

/**
 *	\brief 停止lylink服务
 */
void lylinkapi_stop(void);

/**
 *	\brief 同步模式中处理消息，建议每100ms左右执行一次
 */
void lylinkapi_work(void);

/**
 *	\brief 发送触摸事件
 *
 *	\param id 	第几指，目前仅支持单指，固定0
 *	\param mode 手指状态
 *	\param x 	横轴坐标
 *	\param y 	纵轴坐标
 */
void lylinkapi_touch(
	unsigned int id,
	LYLINK_TOUCHMODE_E mode,
	unsigned short x,
	unsigned short y);

/**
 *	\brief 发送按键消息
 *
 *	\param code 键值
 *	\param mode 状态
 */
void lylinkapi_key(
	LYLINK_KEYCODE_E code,
	LYLINK_KEYMODE_E mode);

/**
 *	\brief 发送鼠标事件
 *
 *	\param mode 按键状态
 *	\param x 	横轴坐标
 *	\param y 	纵轴坐标
 */
void lylinkapi_mouse(
	LYLINK_MOUSEMODE_E mode,
	unsigned short x,
	unsigned short y);

/**
 *	\brief 发送命令
 *
 *	\param cmd 包括焦点类型等参数
 */
void lylinkapi_cmd(LYLINKAPI_CMD cmd);

/**
 *	\brief 在wifi状态改变时通知linkapp客户端
 *
 *	\param params wifi消息内容
 */
void lylinkapi_wifi(LYLINKAPI_WIFI_PARAM *params);

/**
 *	\brief 发送录音数据
 *
 *	\param buf 数据指针
 *	\param len 数据字节数
 */
void lylinkapi_record(const void *buf, unsigned int len);

/**
 *	\brief 发送hid数据
 *
 *	\param cmd 指令(0~63)
 *	\param buf 数据指针
 *	\param len 数据字节数
 */
void lylinkapi_hid(int cmd, const void *buf, unsigned int len);

/**
 *	\brief 设置音量,需实现内部解码
 *
 *	\param vol 音量(0~100)
 */
void lylinkapi_volume(int vol);

/**
 *	\brief 发送蓝牙命令,建议CarPlay和Auto调用lylinkapi_rfcomm,建议HiCar调用lylinkapi_gocsdk
 */
void lylinkapi_bt(LyBtCmdType type, const char *str, unsigned int len);

/**
 *  \brief 释放不带混音时的音频数据
 */
void lylinkapi_free(void *ptr);


#define lylinkapi_focus(x) 			lylinkapi_cmd(x)
#define lylinkapi_rfcomm(s,l)		lylinkapi_bt(LYLINK_BTCMD_RFCOMM, s, l)
#define lylinkapi_gocsdk(s,l)		lylinkapi_bt(LYLINK_BTCMD_GOCSDK, s, l)
#define lylinkapi_mute()			lylinkapi_volume(-1)
#define lylinkapi_unmute()			lylinkapi_volume(101)


#ifdef __cplusplus
}
#endif


#endif
