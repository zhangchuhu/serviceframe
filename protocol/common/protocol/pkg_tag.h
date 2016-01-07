#ifndef _PKG_TAG_H
#define _PKG_TAG_H

#include "common/int_types.h"
#include <map>
#include <string>

typedef uint16_t IM_TAG_TYPE;
/*
typedef std::map<IM_TAG_TYPE, std::string> MapTag2String;
typedef MapTag2String::iterator Iter_MapTag2String;
typedef MapTag2String::const_iterator cIter_MapTag2String;
typedef std::pair<IM_TAG_TYPE, std::string> PairTag2String;
typedef MapTag2String::value_type vt_MapTag2String;

typedef std::map<std::string, uint32_t> MapString2Uint32;
typedef MapString2Uint32::iterator Iter_MapString2Uint32;
typedef MapString2Uint32::const_iterator cIter_MapString2Uint32;
typedef std::pair<std::string, uint32_t> PairString2Uint32;
typedef MapString2Uint32::value_type vt_MapString2Uint32;
*/
#define TAG_NO_USE	0x0000
// Tag for uinfo
#define UIT_ACCOUNT		0x0001
#define UIT_NICK        0x0002
//#define UIT_CONF_VER	0x0002
#define UIT_BIRTHDAY	0x0003
//#define UIT_SCRNAME	0x0004
#define UIT_GENDER	0x0005
//#define UIT_MOBILE_PHONE	0x0006
#define UIT_RESUME		0x0007
#define UIT_AREA	0x0008
#define UIT_PROVINCE	0x0009
#define UIT_CITY		0x000A
#define UIT_VALIDATE    0x000B
//#define UIT_MMCHAT_ID	0x000B
//#define UIT_PORTRAIT_IDX 0x000C
#define UIT_NAME		0x000D
#define UIT_PHONE		0x000E
#define UIT_FAX		0x000F
#define UIT_ADDRESS	0x0010
#define UIT_ZIP_CODE	0x0011
#define UIT_S_PUBLIC_KEY	0x0012
#define UIT_C_PUBLIC_KEY	0x0013
#define UIT_PREF_INFO	0x0014
#define UIT_PREF_BAN	0x0015
#define UIT_PREF_AUTH	0x0016
#define UIT_LAST_LOGIN	0x0017
#define UIT_EMAIL		0x0018
#define UIT_SPARE_FIELD	0x0019
#define UIT_CONF_BITS	0x001A
#define UIT_IDLE_TIME	0x001B
#define UIT_HOTKEY_MAINWIN	0x001C
#define UIT_HOTKEY_MSGWIN	0x001D
#define UIT_SUBJECT	0x001E
#define UIT_EMAIL_CHK	0x001F
#define UIT_EMAIL_PASS	0x0021
#define UIT_UPDATE_VER	0x0023
#define UIT_RESCODE	0x0025
#define UIT_VIDEOINFO	0x002B
#define UIT_MSGKEY		0x002C
#define UIT_COOKIE      0x002D
#define UIT_PASS	0x0031
//#define UIT_PORT	0x0032
//#define UIT_LOCALIP 0x0033
//#define UIT_EXPIP 	0x0034
#define UIT_LAST_TIME 0x0032
#define UIT_LAST_IP 0x0033
#define UIT_VERSION   0x0034
#define UIT_USERJF 0x0035
#define UIT_SIGN 0x0036
#define UIT_PASSPORT 0x0037
#define UIT_INTRO 0x0038
#define UIT_ID	0x0039
#define UIT_UDBID	0x003a
#define UIT_IMID	0x003b
#define UIT_PASSWD	0x003c
#define UIT_WORLDCUP2010 0x003d
#define UIT_REGISTER_TIME 0x003e
//added by Linyue
//Should be consistent with GrpInfoMask in pginfo.h
#define GIT_NAME 0x0300
#define GIT_BULLETIN 0x0301
#define GIT_DESC 0x0302
#define GIT_OWID 0x0303
#define GIT_CAT1 0x0304
#define GIT_CAT2 0x0314
#define GIT_AUTH 0x0305
#define GIT_CHAN_INFO 0x0316
#define GIT_SHORT_CHAN_INFO 0x0317
#define GIT_TOPIC 0x0315
#define GIT_LOGO_IDX 0x0306
#define GIT_LOGO_URL 0x0307
#define GIT_ADHOC 0x0308
#define GIT_PRIV 0x0309
#define GIT_FLD_LST 0x030a
#define GIT_FLD_NAME 0x030b
#define GIT_FLD_MEM_LST 0x030c
#define GIT_IS_FLD_MEM 0x030d

#define SIT_NAME 0x0100
#define SIT_SID 0x0101
#define SIT_ISPUB 0x0102
#define SIT_ISLIMIT 0x0103
#define SIT_BULLETIN 0x0104
#define SIT_LIMITPASSWD 0x0105
#define SIT_PID 0x0106
#define SIT_OWNER 0x0107
#define SIT_MEMBERLIMIT 0x0108
#define SIT_ISLIMITPASSWD 0x0109
#define SIT_CREATETIME 0x0110
#define SIT_CODECRATE 0x0111
#define SIT_BHASPASSWD 0x0112
#define SIT_STYLE 0x0113
#define SIT_MICROTIME 0x0114
#define SIT_ALIASESID 0x0115
#define SIT_TYPE 0x0116
#define SIT_TYPESTRING 0x0117
#define SIT_JIFEN 0x0118
#define SIT_PAIMING 0x0119
#define SIT_REALTIME 0x011a
#define SIT_BULLETINTIMESTAMP 0x011b
#define SIT_INTRODUCE 0x011c
#define SIT_VOICEQC 0x011d
#define SIT_INFO_OPERATOR 0x011e
#define SIT_ISPTYPE 0x011f
#define SIT_ISTXTLIMIT 0x0120
#define SIT_TXTLIMITTIME 0x0121
#define SIT_SUBCHANNEL_ORDER	0x0122	// 子频道的顺序
#define SIT_GUILD_LOGO			0x0123	// 公会logo
#define SIT_GUILD_LOGO_URL		0x0124	// 公会logo的url
#define SIT_JIEDAI				0x0125
#define SIT_GUEST_WAITTIME		0x0126	// 游客禁言等待时间
#define SIT_GUEST_MAXTXTLEN		0x0127	// 游客最大发言长度
#define SIT_GUEST_JOINMAIXU		0x0128	// 禁止游客上麦
#define SIT_GUEST_ENABLELIMIT	0x0129	// 启用游客限制
#define SIT_GUEST_TXTSPEED		0x0130	// 禁止打字是否只对游客做限制
#define SIT_GUEST_VOICE			0x0132	// 在自由模式下禁止游客语音
#define SIT_GUESTCONTROL		0x0133	// 游客控制string , sdbd使用
#define SIT_SEND_URL_TXT        0x0134  // 是否限制发送带url的公聊信息
#define SIT_APPLY_JIFEN			0x0135	// 申请入会积分
#define SIT_APPLY_ANNOUNCE		0x0136	// 入会申请公告
#define SIT_GUEST_SEND_MSG_LIMIT 	0x0137
#define SIT_GUEST_APPLY_NOTIFY 0x0141 // 入会申请通知设置 0 - 不通知  1 - 通知
#define SIT_GUEST_SEND_PICTURE 0x0142 // session send pictures control  0 - not allow  1 - allow  
#define SIT_GUEST_TXT_BINDMOBILE 0x143 // 限制只能允许手机绑定的用户发公屏

#define SIT_MEMBER_SEND_URL_TXT  0x013E  // 会员是否限制发送带url的公聊信息

#define SIT_LANGUAGE            0x144  // 频道语言属性
#define SIT_SUBCHANNEL_SEND_PICTURE 0x145 //管理员控制子频道是否可以发图片

#define SIT_BUY_SESSID_FLAG 0x146  //是否购买频道靓号，1 购买 0 无购买，默认为 0
#define SIT_SHOW_RECRUIT_INFO 0x147 //是否显示在公屏展现公会招募的信息，1 是 0 否，默认为1
#define SIT_HIDE_RECOMMENDED_LAYER 0x148 // 隐藏节目推荐蒙层，1为隐藏；0为不隐藏；默认为0
#define SIT_BROADCAST_ENABLELIMIT  0x149 // 启用管理员频道广播频率限制
#define SIT_BROADCAST_INTERVAL     0x14a // 管理员频道广播频率限制的间隔秒数，每条广播间隔的秒数
#define SIT_IS_EDU                 0x14b // 判断是否为100教育创建频道，0为不是；1为是；默认为0
#define SIT_HIDE_SHARE_MSG         0x14c // 禁止公屏显示分享消息，0为不禁止；1为禁止；默认为0
#define SIT_APP_TYPE               0x14d // 创建频道的APP类型，0为YY；1为100教育；2为语聊；默认为0
#define SIT_ENABLE_QIPAO           0x14e // 公屏启用多彩气泡聊天，0为不启用；1为启用；默认为0

#define SIT_JIEDAI_ROLE            0x160 // 接待跳转角色配置，跳转全频道管理员为1；跳转子频道管理员为2；跳转会员为4；不跳转为0；多选的话就做或操作；默认为0。
#define SIT_AUTO_BROADCAST         0x161 // 自动广播的发送机制配置，麦序模式下麦序有人启用为1；主席模式下启用为2；普通模式下启用为4；不启用为0；多选的话就做或操作；默认为1。
#define SIT_ANONYMOUS_USER_LIMIT   0x162 // 频道匿名用户最大人数限制，默认为25000。

#define SIT_APPAPPTYPEID        0x0151  //appsession typeid
#define SIT_APPPLUGINURL        0x0152  //appsession PluginURL
#define SIT_APPPLUGINVERSION    0x0153  //appsession PluginVersion
#define SIT_APPPLUGINMD5        0x0154  //appsession PluginMD5
#define SIT_FORCEUPDATEVERSION  0x0155  //appsession forceUpdateVersion
#define SIT_APPNAME             0x0156  //appsession Name

#define SIT_MEDIA_TRANFPORT_STYLE     0x180 // 媒体前端连接模式
#define SIT_GUEST_ACCESS_LIMIT        0x183 // 游客进去子频道限制
#define SIT_GUEST_ACCESS_TOP_LIMIT	  0x184 // 游客进入顶级频道限制
#define SIT_ENABLE_RECEPTION_CONFIG	  0x185 // 设置接待语
#define SIT_RECEPTION_PROMPT		  0x186 // 接待语
#define SIT_SESSION_CARD_PREFIX		  0x187 // 频道名片前缀
#define SIT_SESSION_CARD_SUFFIX		  0x188 // 频道名片后缀
#define SIT_ROLER_CHANGED                 0x189 // roler changed
#define SIT_USER_LIMIT                    0x192 // 频道人数限制

#define SIT_UINFO_DISPLAY_CONTROL	  0x2000 // V4解耦数据UI显示控制
#define SIT_SERVICE_STATUS			  0x2001 // V4服务存在状态

#define SIT_CHARGE_ACCESS    0x2003 // 收费子频道类型

#define SIT_TEMPLATE_ID    0x2004 // 频道模板 
#define SIT_QUFU_ID    0x2005 // 游戏区服
#define SIT_BIGCH_SWITCH            0x2006 // 大频道标记


// 频道勋章
#define SIT_MEDAL_VIOLET_GOLD_SESSION    0x2011    // 是否紫金公会
#define SIT_MEDAL_LOGO_URL    0x2012    // 勋章logo的url
#define SIT_MEDAL_NAME    0x2013    // 勋章名称
#define SIT_MEDAL_DESCRIPTION    0x2014    // 勋章简介
#define SIT_MEDAL_LINK_URL    0x2015    // 点击勋章跳转的url
#define SIT_MEDAL_ID    0x2016    // 勋章id
#define SIT_MEDAL_GRADE    0x2021

//频道认证
#define SIT_AUTHENTICATED      0X2017  //认证标志 0或1  布尔值
#define SIT_CREDIT_VALUE       0X2018   // 频道信用  0~100 整型数

//频道解散
#define SIT_DISBAND_APPLY_TIME  0X2019  //申请频道解散的时间
#define SIT_DISBAND_STATUS  0X2020   //频道解散的状态，1 解散有效，0 解散失效

// tag for 广告
#define ADT_TAG		0x8000

#define TAG_OBSOLETE_KEY    0xffff   // 被废弃的key
#endif
