/********************************************************************
	created:	2015-6-3   22:07
	filename: 	login_udb_v2_i.h
	copyright:  Guangzhou huaduo Co., Ltd.
	purpose:
    description:
                
---------------------------------------------------------------------
sequence      date                  author          update history
1             2015/6/3             Luke Chou       initial
*********************************************************************/
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/const.h"
#include "common/protocol/zip_tpl.h"
#include "protocol/CodeInfo.h"
#include "common/res_code.h"

using namespace sox;
// 组外使用，需将以下宏:_INTERFACE_USER定义打开
// #define _INTERFACE_USER
#ifndef _INTERFACE_USER
#include "server_login/common/resource/login_uri.h" 
#endif // _INTERFACE_USER

using namespace std;

#ifndef _login_udb_v2_2015_6_3
#define _login_udb_v2_2015_6_3

#ifdef _INTERFACE_USER
// 以下取值请不要随意改动，需要与udb保持一致
#define URI_VALUE(mud_id, mud_uri, svid) ((mud_uri&0xFF) << 24)|((mud_id&0xFFFF) << 8)|(svid&0xFF)
#define LM_UNI_LOGIN 100
enum LOGIN_URI
{
	LG_URI_UL2_YYLOGIN_REQ = URI_VALUE(LM_UNI_LOGIN, 20, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_YYLOGIN_RES = URI_VALUE(LM_UNI_LOGIN, 21, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_APPLOGIN_REQ = URI_VALUE(LM_UNI_LOGIN, 22, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_APPLOGIN_RES = URI_VALUE(LM_UNI_LOGIN, 23, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_WEBLOGIN_REQ = URI_VALUE(LM_UNI_LOGIN, 24, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_WEBLOGIN_RES = URI_VALUE(LM_UNI_LOGIN, 25, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_SENDSMS_REQ = URI_VALUE(LM_UNI_LOGIN, 26, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_SENDSMS_RES = URI_VALUE(LM_UNI_LOGIN, 27, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_SMS_YYREGISTERLOGIN_REQ = URI_VALUE(LM_UNI_LOGIN, 28, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_SMS_YYREGISTERLOGIN_RES = URI_VALUE(LM_UNI_LOGIN, 29, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_SMS_APPREGISTERLOGIN_REQ = URI_VALUE(LM_UNI_LOGIN, 30, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_SMS_APPREGISTERLOGIN_RES = URI_VALUE(LM_UNI_LOGIN, 31, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_SMS_YYMODPASSWD_REQ = URI_VALUE(LM_UNI_LOGIN, 32, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_SMS_YYMODPASSWD_RES = URI_VALUE(LM_UNI_LOGIN, 33, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_SMS_APPMODPASSWD_REQ = URI_VALUE(LM_UNI_LOGIN, 34, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_SMS_APPMODPASSWD_RES = URI_VALUE(LM_UNI_LOGIN, 35, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_REFRESHPIC_REQ = URI_VALUE(LM_UNI_LOGIN, 36, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_REFRESHPIC_RES = URI_VALUE(LM_UNI_LOGIN, 37, protocol::SERVERLOGIN_SVID),
};

//规范错误码
// sys_err, bus_err范围必须在100以内
#define MK_SYSERR_CODE(mud_id,sys_err)   (mud_id)*10000 + (sys_err)*100
#define MK_BUSERR_CODE(mud_id,bus_err)   (mud_id)*10000 + (bus_err)
#endif // _INTERFACE_USER

namespace server{ namespace loginUDB{
	enum UNI_LOGIN_ERR_CODE
	{
		URES_SUCCESS               = 0,       // 成功
		// 需要进一步验证
		URES_NEED_NEXT_VERIFY      = MK_BUSERR_CODE(LM_UNI_LOGIN, 1),  // 需要进一步验证（可能包括：短信验证码、手机令牌、硬件令牌）
		// 以下错误，由服务端下发错误提示，客户端向用户展示错误信息(2 ~ 9)
		URES_LOCKED                = MK_BUSERR_CODE(LM_UNI_LOGIN, 2),  // 账号被锁定（用户已设置账号锁），错误提示服务端下发
		URES_FROZEN                = MK_BUSERR_CODE(LM_UNI_LOGIN, 3),  // 账号临时冻结（违反安全策略），错误提示服务端下发
		URES_SERVER_BANNED         = MK_BUSERR_CODE(LM_UNI_LOGIN, 4),  // 运营封禁，错误提示服务端下发
		URES_IP_BLACKLIST          = MK_BUSERR_CODE(LM_UNI_LOGIN, 5),  // IP黑名单，错误提示服务端下发
		URES_SERVER_REJECTED       = MK_BUSERR_CODE(LM_UNI_LOGIN, 6),  // 拒绝提供服务，拒绝原因服务端下发（可能是外挂，拥塞控制等）
		// 口令错误部分(10 ~ 29)
		URES_PASSWD_ERR            = MK_BUSERR_CODE(LM_UNI_LOGIN, 10), // 密码错误，需要重新输入密码进行验证
		URES_PICCODE_ERR           = MK_BUSERR_CODE(LM_UNI_LOGIN, 11), // 图片验证码错误，返回新图片，需要重新输入图片验证码验证
		URES_SMSCODE_ERR           = MK_BUSERR_CODE(LM_UNI_LOGIN, 12), // 短信验证码错误，用户可以重新获取短信验证码后再提交验证
		URES_SMSCODE_EXPIRED       = MK_BUSERR_CODE(LM_UNI_LOGIN, 13), // 短信验证码已失效，用户可以重新获取短信验证码后再提交验证
		URES_SMSCODE_NOT_SENT      = MK_BUSERR_CODE(LM_UNI_LOGIN, 14), // 短信验证码未下发，用户可以重新获取短信验证码后再提交验证
		URES_MOBTOKEN_ERR          = MK_BUSERR_CODE(LM_UNI_LOGIN, 15), // 手机令牌错误，需要重新输入手机令牌进行验证
		URES_HWTOKEN_ERR           = MK_BUSERR_CODE(LM_UNI_LOGIN, 16), // 硬件令牌错误，需要重新输入硬件令牌进行验证
		URES_MODIFY_PSW_SAME_ERR   = MK_BUSERR_CODE(LM_UNI_LOGIN, 17), // 修改密码，但是密码和原密码一致
		URES_MODIFY_PSW_TIMES_LIMIT= MK_BUSERR_CODE(LM_UNI_LOGIN, 18), // 修改密码次数过多
		// 凭证错误部分（30 ~ 49）
		URES_CRED_EXPIRED          = MK_BUSERR_CODE(LM_UNI_LOGIN, 30), // 凭证过失效期，需要重新进行口令验证
        URES_CRED_ERR              = MK_BUSERR_CODE(LM_UNI_LOGIN, 31), // 凭证错误/无效，需要重新进行口令验证
		URES_CRED_DEVICE_UNMATCHED = MK_BUSERR_CODE(LM_UNI_LOGIN, 32), // 凭证产和验证时设备不匹配，需要重新进行口令验证
		URES_CRED_APPID_UNMATCHED  = MK_BUSERR_CODE(LM_UNI_LOGIN, 33), // 凭证产和验证时appid不匹配，需要重新进行口令验证
		URES_CRED_DESTROYED        = MK_BUSERR_CODE(LM_UNI_LOGIN, 34), // 凭证已被销毁，需要重新进行口令验证
        URES_CRED_OTHERS           = MK_BUSERR_CODE(LM_UNI_LOGIN, 35), // 凭证其他错误，需要重新进行口令验证
        URES_CRED_CREATE           = MK_BUSERR_CODE(LM_UNI_LOGIN, 36), // 产凭证错误，客户端需要重试
		URES_OTP_EXPIRED           = MK_BUSERR_CODE(LM_UNI_LOGIN, 40), // 一次性凭证过失效期，需要重新获取一次性凭证来验证
		URES_OTP_ERR               = MK_BUSERR_CODE(LM_UNI_LOGIN, 41), // 一次性凭证错误/无效，需要重新获取一次性凭证来验证
		URES_OTP_USED              = MK_BUSERR_CODE(LM_UNI_LOGIN, 42), // 一次性凭证已被使用过，需要重新获取一次性凭证来验证
		URES_OTP_DEVICE_UNMATCHED  = MK_BUSERR_CODE(LM_UNI_LOGIN, 43), // 一次性凭证产和验证时设备不匹配，需要重新获取一次性凭证来验证
		URES_OTP_APPID_UNMATCHED   = MK_BUSERR_CODE(LM_UNI_LOGIN, 44), // 一次性凭证产和验证时appid不匹配，需要重新获取一次性凭证来验证
		URES_OTP_DESTROYED         = MK_BUSERR_CODE(LM_UNI_LOGIN, 45), // 一次性凭证已被销毁，需要重新获取一次性凭证来验证
		// 票据错误部分(50 ~ 59)
		URES_TICKET_ENCODING_ERR   = MK_BUSERR_CODE(LM_UNI_LOGIN, 50), // 票据编码错误
		URES_TICKET_EXPIRED        = MK_BUSERR_CODE(LM_UNI_LOGIN, 51), // 票据过期
		URES_TICKET_ERR            = MK_BUSERR_CODE(LM_UNI_LOGIN, 52), // 票据无效
		URES_TICKET_USED           = MK_BUSERR_CODE(LM_UNI_LOGIN, 53), // 票据已被使用过
		URES_TICKET_DESTROYED      = MK_BUSERR_CODE(LM_UNI_LOGIN, 54), // 票据已被废除
        URES_TICKET_CREATE         = MK_BUSERR_CODE(LM_UNI_LOGIN, 55), // 产票据错误，客户端需要重试
		// AccountInfo错误部分
        URES_ACCOUNTINFO_ERR       = MK_BUSERR_CODE(LM_UNI_LOGIN, 56), // AccountInfo错误
		// 短信相关错误部分
		URES_SENT_SMSCODE_FAIL     = MK_BUSERR_CODE(LM_UNI_LOGIN, 60), // 下发短信验证码失败，可以重新尝试获取短信
		URES_SENT_SMSCODE_LIMIT    = MK_BUSERR_CODE(LM_UNI_LOGIN, 61), // 下发短信验证码次数超出次数限制，需要第二天恢复次数后再尝试
		URES_SMS_VFY_TIMES_LIMIT   = MK_BUSERR_CODE(LM_UNI_LOGIN, 62), // 短信验证次数过多，可以重新获取短信
		
		// 其它业务逻辑错误(70 ~ 79)
		URES_YYID_LOGIN_PROHIBITED = MK_BUSERR_CODE(LM_UNI_LOGIN, 70), // 未开通YY号登录权限
		URES_USER_NOT_EXIST        = MK_BUSERR_CODE(LM_UNI_LOGIN, 71), // 用户不存在（登录或改密时，请求的用户不存在）
		URES_USER_ALREADY_EXIST    = MK_BUSERR_CODE(LM_UNI_LOGIN, 72), // 用户已存在（注册时该手机已注册）
		URES_DEV_LOGIN_LIMIT       = MK_BUSERR_CODE(LM_UNI_LOGIN, 73), // 用户没有内测版登录权限

		// 参数错误定义部分(80 ~ 89)
        URES_PARAM_INVALID         = MK_BUSERR_CODE(LM_UNI_LOGIN, 80), // 参数错误，具体错误原因参照errmsg
		URES_APPID_INVALID         = MK_BUSERR_CODE(LM_UNI_LOGIN, 81), // appid无效，未在udb申请
		URES_UID_INVALID           = MK_BUSERR_CODE(LM_UNI_LOGIN, 82), // uid无效：uid为0或过长
		URES_PHONE_NUM_INVALID     = MK_BUSERR_CODE(LM_UNI_LOGIN, 83), // 手机号无效

		// 协议交互错误(90 ~ 99)
		URES_SESSION_DATA_EXPIRED  = MK_BUSERR_CODE(LM_UNI_LOGIN, 90), // session_data已失效，客户端需要退回到没有session_data就能执行的那一步重新开始整个流程
		URES_SESSION_DATA_INVALID  = MK_BUSERR_CODE(LM_UNI_LOGIN, 91), // session_data不能解析，客户端需要退回到没有session_data就能执行的那一步重新开始整个流程
		URES_SESSION_INTERACTION_ERR     = MK_BUSERR_CODE(LM_UNI_LOGIN, 92), // 协议交互错误（如：session_data业务逻辑校验失败）
		
		UNI_RES_RETRY              = MK_SYSERR_CODE(LM_UNI_LOGIN, 99), // 客户端需重试，系统错误可以暂时不需要细分
	};

	enum VERIFY_TOKEN_TYPE
	{
		TOKEN_CREDIT   = 1,//凭证登陆	
		TOKEN_PASSWORD = 2,//密码登陆	
		TOKEN_TICKET   = 3,//票据登陆	
		TOKEN_ACCTINFO = 4,//acctinfo登陆	
	};

	enum NEXT_VERIFY_STRATEGY
	{
		DS_NONE     = 0x0,      // 无动态策略
		DS_PICCODE  = 0x01,     // 图片验证码
		DS_MOBTOKEN = 0x02,     // 手机令牌
		DS_HWTOKEN  = 0x04,     // 硬件令牌
		DS_SMSCODE  = 0x08,     // 短信验证码
	};

	struct device_info
	{
		std::string _device_name;     // 设备名称。手机型号：MINOTELTE,IPHONE 6；PC的计算机名：YY-20141112WEZU
		std::string _device_id;       // 设备ID。手机IMEI， PC mac-address
		uint64_t    _terminal_type;   // 终端类型，由客户端定义（高32位相当于appid/appkey，低16-31 是platform类型，0-15是os类型），具体取值可找戈宇
		std::string _sys_info;        // 操作系统信息
		std::string _lcid;            // 语言标志，其中2052简体中文
		device_info():_terminal_type(0){}
	};

	inline Pack & operator << (Pack & p, const device_info& r)
	{
		sox::PackBuffer pb;
		sox::Pack pk(pb);
		pk << r._device_name << r._device_id << r._terminal_type << r._sys_info << r._lcid;
		p.push_varstr(pk.data(), pk.size());
		return p;
	}

	inline const Unpack & operator >> (const Unpack & p, device_info& r)
	{
		std::string str = p.pop_varstr();
		if (str.empty())
		{
			return p;
		}
		sox::Unpack up(str.c_str(), str.size());
		up >> r._device_name >> r._device_id >> r._terminal_type >> r._sys_info >> r._lcid;
		return p;
	}

	struct proto_header
	{
		uint16_t    _app_type;        // 应用分类:1 - YYPC（桌面YY）, 2 - YYMOB（手机YY）, 3 - YYWEB（webYY）, 4 - APPPC, 5 - APPMOB, 6 - WEB
		std::string _appid;           // 应用id，原则上应由udb统一分配，所有登录策略由appid来配置
		std::string _app_sign;        // 应用签名，算法是md5(_appid+_app_ver+_app_key)的前8位，其中app_key是申请appid的时候已经生成的websdk key.
		std::string _app_ver;         // 应用版本号，由业务方维护（方便出业务统计数据）, 开发版本以dev_开头
		std::string _sdk_ver;         // SDK版本号，由SDK开发者维护（方便出SDK统计数据）
		std::string _client_ip;       // 客户端IP（接入层获取的对端出口IP）
		device_info _device_info;     // 设备信息
		std::string _channel;  		  // 渠道信息
		std::string _reserve;  		  // 预留字段（允许接入方json格式输入）
		proto_header():_app_type(0){}
	};

	inline Pack & operator << (Pack & p, const proto_header& r)
	{
		sox::PackBuffer pb;
		sox::Pack pk(pb);
		pk << r._app_type << r._appid << r._app_sign << r._app_ver << r._sdk_ver << r._client_ip << r._device_info << r._channel << r._reserve;
		p.push_varstr(pk.data(), pk.size());
		return p;
	}

	inline const Unpack & operator >> (const Unpack & p, proto_header& r)
	{
		std::string str = p.pop_varstr();
		if (str.empty())
		{
			return p;
		}
		sox::Unpack up(str.c_str(), str.size());
		up >> r._app_type >> r._appid >> r._app_sign >> r._app_ver >> r._sdk_ver >>  r._client_ip >> r._device_info  >> r._channel >>  r._reserve;
		return p;
	}

	struct proto_password_login
	{
		std::string _user;         // 通行证，帐号id，邮箱，手机号之一
		uint32_t    _enc_type;     // 加密类型，0 - sha1（40字节）
		std::string _password;     // 密码
		proto_password_login():_enc_type(0){}
	};

	inline Pack & operator << (Pack & p, const proto_password_login& r)
	{
		sox::PackBuffer pb;
		sox::Pack pk(pb);
		pk << r._user << r._enc_type << r._password;
		p.push_varstr(pk.data(), pk.size());
		return p;
	}

	inline const Unpack & operator >> (const Unpack & p, proto_password_login& r)
	{
		std::string str = p.pop_varstr();
		if (str.empty())
		{
			return p;
		}
		sox::Unpack up(str.c_str(), str.size());
		up >> r._user >> r._enc_type >> r._password;
		return p;
	}

	struct proto_ticket_login
	{
		uint64_t    _uid;          // 帐号内部标识
		std::string _ticket;       // 票据
		proto_ticket_login():_uid(0){}
	};

	inline Pack & operator << (Pack & p, const proto_ticket_login& r)
	{
		sox::PackBuffer pb;
		sox::Pack pk(pb);
		pk << r._uid << r._ticket;
		p.push_varstr(pk.data(), pk.size());
		return p;
	}

	inline const Unpack & operator >> (const Unpack & p, proto_ticket_login& r)
	{
		std::string str = p.pop_varstr();
		if (str.empty())
		{
			return p;
		}
		sox::Unpack up(str.c_str(), str.size());
		up >> r._uid >> r._ticket;
		return p;
	}

	struct proto_credit_login
	{
		uint64_t    _uid;          // 帐号内部标识
		std::string _credit;       // 凭证
		proto_credit_login():_uid(0){}
	};

	inline Pack & operator << (Pack & p, const proto_credit_login& r)
	{
		sox::PackBuffer pb;
		sox::Pack pk(pb);
		pk << r._uid << r._credit;
		p.push_varstr(pk.data(), pk.size());
		return p;
	}

	inline const Unpack & operator >> (const Unpack & p, proto_credit_login& r)
	{
		std::string str = p.pop_varstr();
		if (str.empty())
		{
			return p;
		}
		sox::Unpack up(str.c_str(), str.size());
		up >> r._uid >> r._credit;
		return p;
	}

	struct proto_acctinfo_login
	{
		std::string _user;         // 通行证，帐号id，邮箱，手机号之一
		std::string _acctinfo;
	};

	inline Pack & operator << (Pack & p, const proto_acctinfo_login& r)
	{
		sox::PackBuffer pb;
		sox::Pack pk(pb);
		pk << r._user << r._acctinfo;
		p.push_varstr(pk.data(), pk.size());
		return p;
	}

	inline const Unpack & operator >> (const Unpack & p, proto_acctinfo_login& r)
	{
		std::string str = p.pop_varstr();
		if (str.empty())
		{
			return p;
		}
		sox::Unpack up(str.c_str(), str.size());
		up >> r._user >> r._acctinfo;
		return p;
	}

	struct proto_token
	{
		uint32_t              _token_type;    // 参考enum VERIFY_TOKEN_TYPE; 1 - 凭证, 2 - 帐号/密码,  3 - ticket票据, 4 - acctinfo
		proto_password_login  _proto_passwd;  // 当_token_type = 2时出现
		proto_credit_login    _proto_credit;  // 当_token_type = 1时出现
		proto_ticket_login    _proto_ticket;  // 当_token_type = 3时出现
		proto_acctinfo_login  _proto_acctinfo;// 当_token_type = 4时出现
		uint32_t    _strategy;                // 验证策略（手机令牌、短信、图片）
		std::string _stra_token;              // 对应策略的口令
		proto_token():_token_type(0), _strategy(0){}
	};

	inline Pack & operator << (Pack & p, const proto_token& r)
	{
		sox::PackBuffer pb;
		sox::Pack pk(pb);
		pk << r._token_type;
		if(TOKEN_PASSWORD == r._token_type)
		{
			pk << r._proto_passwd;
		}
		else if(TOKEN_CREDIT == r._token_type)
		{
			pk << r._proto_credit;
		}
		else if(TOKEN_TICKET == r._token_type)
		{
			pk << r._proto_ticket;
		}
		else if(TOKEN_ACCTINFO == r._token_type)
		{
			pk << r._proto_acctinfo;
		}
		pk << r._strategy << r._stra_token;
		p.push_varstr(pk.data(), pk.size());
		return p;
	}

	inline const Unpack & operator >> (const Unpack & p, proto_token& r)
	{
		std::string str = p.pop_varstr();
		if (str.empty())
		{
			return p;
		}
		sox::Unpack up(str.c_str(), str.size());
		up >> r._token_type;
		if(TOKEN_PASSWORD == r._token_type)
		{
			up >> r._proto_passwd;
		}
		else if(TOKEN_CREDIT == r._token_type)
		{
			up >> r._proto_credit;
		}
		else if(TOKEN_TICKET == r._token_type)
		{
			up >> r._proto_ticket;
		}
		else if(TOKEN_ACCTINFO == r._token_type)
		{
			up >> r._proto_acctinfo;
		}
		up >> r._strategy >> r._stra_token;
		return p;
	}

	struct proto_jump_to // 需要跳转的业务
	{
		std::string _appid;     // 应用id
		std::string _sessionid; // 会话id
	};

	inline Pack & operator << (Pack & p, const proto_jump_to& r)
	{
		sox::PackBuffer pb;
		sox::Pack pk(pb);
		pk << r._appid << r._sessionid;
		p.push_varstr(pk.data(), pk.size());
		return p;
	}

	inline const Unpack & operator >> (const Unpack & p, proto_jump_to& r)
	{
		std::string str = p.pop_varstr();
		if (str.empty())
		{
			return p;
		}
		sox::Unpack up(str.c_str(), str.size());
		up >> r._appid >> r._sessionid;
		return p;
	}

	struct proto_login_data
	{
		uint64_t    _yyuid;
		uint64_t    _yyid;
		std::string _passport;
		std::string _credit;
		proto_login_data():_yyuid(0), _yyid(0){}
		void clear(){_yyuid = 0; _yyid = 0; _passport.clear(); _credit.clear();}
	};

	inline Pack & operator << (Pack & p, const proto_login_data& r)
	{
		sox::PackBuffer pb;
		sox::Pack pk(pb);
		pk << r._yyuid << r._yyid << r._passport << r._credit;
		p.push_varstr(pk.data(), pk.size());
		return p;
	}

	inline const Unpack & operator >> (const Unpack & p, proto_login_data& r)
	{
		std::string str = p.pop_varstr();
		if (str.empty())
		{
			return p;
		}
		sox::Unpack up(str.c_str(), str.size());
		up >> r._yyuid >> r._yyid >> r._passport >> r._credit;
		return p;
	}

	struct proto_jump_otp
	{
		uint32_t    _result;    // 结果码
		std::string _reason;    // 失败原因描述
		std::string _appid;     // 跳转的appid
		std::string _otp;       // 跳转凭证
		proto_jump_otp():_result(0){}
	};

	inline Pack & operator << (Pack & p, const proto_jump_otp& r)
	{
		sox::PackBuffer pb;
		sox::Pack pk(pb);
		pk << r._result << r._reason << r._appid << r._otp;
		p.push_varstr(pk.data(), pk.size());
		return p;
	}

	inline const Unpack & operator >> (const Unpack & p, proto_jump_otp& r)
	{
		std::string str = p.pop_varstr();
		if (str.empty())
		{
			return p;
		}
		sox::Unpack up(str.c_str(), str.size());
		up >> r._result >> r._reason >> r._appid >> r._otp;
		return p;
	}

    struct proto_anti_code
    {
        std::string _biz_name;// 反外挂虚拟机类别
        std::string _anti_code; 
    };

	inline Pack & operator << (Pack & p, const proto_anti_code& r)
	{
		sox::PackBuffer pb;
		sox::Pack pk(pb);
        pk << r._biz_name << r._anti_code;
		p.push_varstr(pk.data(), pk.size());
		return p;
	}

	inline const Unpack & operator >> (const Unpack & p, proto_anti_code& r)
	{
		std::string str = p.pop_varstr();
		if (str.empty())
		{
			return p;
		}
		sox::Unpack up(str.c_str(), str.size());
        up >> r._biz_name >> r._anti_code;
		return p;
	}

	struct proto_strategy_detail //策略内容
	{
		uint32_t _strategy;
		std::string _verification_selection_title;
		std::string _verification_title;
		std::string _token_data;
		proto_strategy_detail():_strategy(0){}
	};

	inline Pack & operator << (Pack & p, const proto_strategy_detail& r)
	{
		sox::PackBuffer pb;
		sox::Pack pk(pb);
		pk << r._strategy << r._verification_selection_title << r._verification_title << r._token_data;
		p.push_varstr(pk.data(), pk.size());
		return p;
	}

	inline const Unpack & operator >> (const Unpack & p, proto_strategy_detail& r)
	{
		std::string str = p.pop_varstr();
		if (str.empty())
		{
			return p;
		}
		sox::Unpack up(str.c_str(), str.size());
		up >> r._strategy >> r._verification_selection_title >> r._verification_title >> r._token_data;
		return p;
	}

	struct proto_session //session data version and string data
	{
		uint32_t       _proto_ver;     // 协议版本号，方便协议升级
		uint32_t       _session_type;  // session类型，直接回传既可（1是yyuid类型，2是mobile类型）
		std::string    _session_data;  // 1.如果是第一次执行请求，该参数为空，2.除第一次外，后续请求都用前一个请求返回的session_data来设置该参数
		proto_session():_proto_ver(0),_session_type(0){}
	};

	inline Pack & operator << (Pack & p, const proto_session& r)
	{
		sox::PackBuffer pb;
		sox::Pack pk(pb);
		pk << r._proto_ver << r._session_type << r._session_data;
		p.push_varstr(pk.data(), pk.size());
		return p;
	}

	inline const Unpack & operator >> (const Unpack & p, proto_session& r)
	{
		std::string str = p.pop_varstr();
		if (str.empty())
		{
			return p;
		}
		sox::Unpack up(str.c_str(), str.size());
		up >> r._proto_ver >> r._session_type >>  r._session_data;
		return p;
	}

	//--------------------------------------------------------------------------
	// 协议基础结构体
	struct proto_login_req:public sox::Marshallable
	{
		uint32_t       _proto_ver;     // 协议版本号，方便协议升级
		std::string    _context;       // 消息序列号
		proto_header   _proto_hdr;     // 公用协议头
		proto_token    _proto_token;   // 口令协议
		proto_session  _proto_session_data;  // 1.如果是第一次执行请求，该参数为空，2.除第一次外，后续请求都用前一个请求返回的session_data来设置该参数
		proto_anti_code   _proto_anti_code;  // 反外挂信息
		proto_login_req():_proto_ver(0){}

		virtual void marshal( sox::Pack &p )const  // 以上字段打包成一个字符串
		{
			sox::PackBuffer pb;
			sox::Pack pk(pb);
			pk << _proto_ver << _context << _proto_hdr << _proto_token << _proto_session_data << _proto_anti_code;
			p.push_varstr(pk.data(), pk.size());
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			std::string str = up.pop_varstr();
			if (!str.empty())
			{
				sox::Unpack t(str.c_str(), str.size());
				t >> _proto_ver >> _context >> _proto_hdr >> _proto_token >> _proto_session_data >> _proto_anti_code;
			}
		}
	};

	struct proto_login_res:public sox::Marshallable
	{
		uint32_t    _proto_ver;       // 协议版本号，方便协议升级
		std::string _context;         // 消息序列号
		uint32_t    _errcode;         // 错误码
		std::string _errmsg;          // 针对错误码的错误信息,主要用于日志打印 
		std::string _description;     // 失败原因描述，用于对用户展示,成功时为空，格式可定制
		std::string _pic;             // 图片验证码图片
		uint32_t    _strategy;        // 当rescode = URES_NEED_NEXT_VERIFY（需要进一步验证）的登录策略选项，bit位组合，枚举值见：NEXT_VERIFY_STRATEGY
		proto_login_data _login_data; // 登录返回基本信息
		proto_session    _proto_session_data; // 1.如果是第一次执行请求，该参数为空，2.除第一次外，后续请求都用前一个请求返回的session_data来设置该参数
		vector<proto_strategy_detail> _proto_strategy_detail; // 当_strategy非空，需进一步验证的验证内容，如手机验证码的手机号码提示134*****322；
		proto_login_res():_proto_ver(0), _errcode(0), _strategy(0){}
		virtual void marshal( sox::Pack &p )const  // 以上字段打包成一个字符串
		{
			sox::PackBuffer pb;
			sox::Pack pk(pb);
			pk << _proto_ver << _context << _errcode << _errmsg << _description << _pic << _strategy << _login_data << _proto_session_data << _proto_strategy_detail;
			p.push_varstr(pk.data(), pk.size());
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			std::string str = up.pop_varstr();
			if (!str.empty())
			{
				sox::Unpack t(str.c_str(), str.size());
				t >> _proto_ver >> _context >> _errcode >> _errmsg >> _description >> _pic >> _strategy >> _login_data >> _proto_session_data >> _proto_strategy_detail;
			}
		}
	};

	struct proto_sms_registerorlogin_req:public sox::Marshallable
	{
		uint32_t       _proto_ver;     // 协议版本号，方便协议升级
		std::string    _context;       // 消息序列号
		proto_header   _proto_hdr;     // 公用协议头
		std::string    _mobile;        // 手机号码
		std::string    _smscode;       // 短信验证码
		proto_session  _proto_session_data;  // 上一步操作返回的session_data
		proto_sms_registerorlogin_req():_proto_ver(0){}
		virtual void marshal( sox::Pack &p )const  // 以上字段打包成一个字符串
		{
			sox::PackBuffer pb;
			sox::Pack pk(pb);
			pk << _proto_ver << _context << _proto_hdr << _mobile << _smscode << _proto_session_data;
			p.push_varstr(pk.data(), pk.size());
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			std::string str = up.pop_varstr();
			if (!str.empty())
			{
				sox::Unpack t(str.c_str(), str.size());
				t >> _proto_ver >> _context >> _proto_hdr >> _mobile >> _smscode >> _proto_session_data;
			}
		}
	};

	struct proto_sms_modifypasswd_req:public sox::Marshallable
	{
		uint32_t       _proto_ver;     // 协议版本号，方便协议升级
		std::string    _context;       // 消息序列号
		proto_header   _proto_hdr;     // 公用协议头
		std::string    _mobile;        // 手机号码
		std::string    _smscode;       // 短信验证码
		uint32_t       _enc_type;      // 加密类型，0 - sha1（40字节）
		std::string    _password;      // 密码
		proto_session  _proto_session_data;  // 上一步操作返回的session_data
		proto_sms_modifypasswd_req():_proto_ver(0), _enc_type(0){}
		virtual void marshal( sox::Pack &p )const  // 以上字段打包成一个字符串
		{
			sox::PackBuffer pb;
			sox::Pack pk(pb);
			pk << _proto_ver << _context << _proto_hdr << _mobile << _smscode << _enc_type << _password << _proto_session_data;
			p.push_varstr(pk.data(), pk.size());
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			std::string str = up.pop_varstr();
			if (!str.empty())
			{
				sox::Unpack t(str.c_str(), str.size());
				t >> _proto_ver >> _context >> _proto_hdr >> _mobile >> _smscode >> _enc_type >> _password >> _proto_session_data;
			}
		}
	};

	//--------------------------------------------------------------------------
	// yy客户端登录(进频道类的)
	struct proto_yylogin_req:public proto_login_req
	{
		enum{uri = LG_URI_UL2_YYLOGIN_REQ};
	};

	struct proto_yylogin_res:public proto_login_res
	{
		enum{uri = LG_URI_UL2_YYLOGIN_RES};
		std::string _ticket;
		std::string _yycookie;
		virtual void marshal( sox::Pack &pk )const
		{
			proto_login_res::marshal(pk);
			pk << _ticket << _yycookie;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			proto_login_res::unmarshal(up);
			up >> _ticket >> _yycookie;
		}
	};

	//--------------------------------------------------------------------------
	// yy客户端以外的app登录
	struct proto_applogin_req:public proto_login_req
	{
		enum{uri = LG_URI_UL2_APPLOGIN_REQ};
		std::vector<proto_jump_to>  _jump_tos;
		virtual void marshal( sox::Pack &pk )const
		{
			proto_login_req::marshal(pk);
			pk << _jump_tos;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			proto_login_req::unmarshal(up);
			up >> _jump_tos;
		}
	};

	struct proto_applogin_res:public proto_login_res
	{
		enum{uri = LG_URI_UL2_APPLOGIN_RES};
		std::string _webcookie;
		std::vector<proto_jump_otp> _jump_otps;
		virtual void marshal( sox::Pack &pk )const
		{
			proto_login_res::marshal(pk);
			pk << _webcookie << _jump_otps;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			proto_login_res::unmarshal(up);
			up >> _webcookie >> _jump_otps;
		}
	};

	//--------------------------------------------------------------------------
	// web登录
	struct proto_weblogin_req:public proto_login_req
	{
		enum{uri = LG_URI_UL2_WEBLOGIN_REQ};
	};

	struct proto_weblogin_res:public proto_login_res
	{
		enum{uri = LG_URI_UL2_WEBLOGIN_RES};
	};

	//--------------------------------------------------------------------------
	// 通过手机号下发短信
	struct proto_sendsms_req:public sox::Marshallable
	{
		enum{uri = LG_URI_UL2_SENDSMS_REQ};
		uint32_t       _proto_ver;     // 协议版本号，方便协议升级
		std::string    _context;       // 消息序列号
		proto_header   _proto_hdr;     // 公用协议头
		std::string    _mobile;        // 手机号码
		uint8_t        _type;          // 1 - 用于注册或登录（没有注册进行注册，已注册就登录），2 - 用于登录或改密（手机号未注册，返回用户不存在）
        std::string    _piccode;       // 图片验证码
		proto_session  _proto_session_data;  // 预留
		proto_sendsms_req():_proto_ver(0),_type(0){}
		virtual void marshal( sox::Pack &pk )const
		{
			pk << _proto_ver << _context << _proto_hdr << _mobile << _type << _piccode << _proto_session_data;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			up >> _proto_ver >> _context >> _proto_hdr >> _mobile >> _type >> _piccode >> _proto_session_data;
		}
	};

	struct proto_sendsms_res:public sox::Marshallable
	{
		enum{uri = LG_URI_UL2_SENDSMS_RES};
		uint32_t    _proto_ver;       // 协议版本号，方便协议升级
		std::string _context;         // 消息序列号
		uint32_t    _errcode;         // 错误码
		std::string _errmsg;          // 针对错误码的错误信息,主要用于日志打印
		std::string _description;     // 失败原因描述，用于对用户展示,成功时为空，格式可定制
        std::string   _pic;           // 图片验证码
		proto_session _proto_session_data;    // 下一步操作时使用
		proto_sendsms_res():_proto_ver(0), _errcode(0){}
		virtual void marshal( sox::Pack &pk )const
		{
			pk << _proto_ver << _context << _errcode << _errmsg << _description << _pic << _proto_session_data;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			up >> _proto_ver >> _context >> _errcode >> _errmsg >> _description >> _pic >> _proto_session_data;
		}
	};

	//--------------------------------------------------------------------------
	// yy移动端使用短信验证码注册或登录
	struct proto_sms_yyregisterorlogin_req:public proto_sms_registerorlogin_req
	{
		enum{uri = LG_URI_UL2_SMS_YYREGISTERLOGIN_REQ};
	};

	struct proto_sms_yyregisterorlogin_res:public proto_login_res
	{
		enum{uri = LG_URI_UL2_SMS_YYREGISTERLOGIN_RES};
		std::string _ticket;
		std::string _yycookie;
		bool 		_is_new_user; //是否为新未注册用户
		virtual void marshal( sox::Pack &pk )const
		{
			proto_login_res::marshal(pk);
			pk << _ticket << _yycookie << _is_new_user;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			proto_login_res::unmarshal(up);
			up >> _ticket >> _yycookie >> _is_new_user;
		}
	};

	//--------------------------------------------------------------------------
	// 其它app移动端使用短信验证码注册或登录
	struct proto_sms_appregisterorlogin_req:public proto_sms_registerorlogin_req
	{
		enum{uri = LG_URI_UL2_SMS_APPREGISTERLOGIN_REQ};
		std::vector<proto_jump_to>  _jump_tos;
		virtual void marshal( sox::Pack &pk )const
		{
			proto_sms_registerorlogin_req::marshal(pk);
			pk << _jump_tos;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			proto_sms_registerorlogin_req::unmarshal(up);
			up >> _jump_tos;
		}
	};

	struct proto_sms_appregisterorlogin_res:public proto_login_res
	{
		enum{uri = LG_URI_UL2_SMS_APPREGISTERLOGIN_RES};
		std::string _webcookie;
		std::vector<proto_jump_otp> _jump_otps;
		virtual void marshal( sox::Pack &pk )const
		{
			proto_login_res::marshal(pk);
			pk << _webcookie << _jump_otps;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			proto_login_res::unmarshal(up);
			up >> _webcookie >> _jump_otps;
		}
	};

	//--------------------------------------------------------------------------
	//yy移动端使用短信验证码改密
	struct proto_sms_yymodifypasswd_req:public proto_sms_modifypasswd_req
	{
		enum{uri = LG_URI_UL2_SMS_YYMODPASSWD_REQ};
	};

	struct proto_sms_yymodifypasswd_res:public proto_login_res
	{
		enum{uri = LG_URI_UL2_SMS_YYMODPASSWD_RES};
		std::string _ticket;
		std::string _yycookie;
		virtual void marshal( sox::Pack &pk )const
		{
			proto_login_res::marshal(pk);
			pk << _ticket << _yycookie;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			proto_login_res::unmarshal(up);
			up >> _ticket >> _yycookie;
		}
	};

	//--------------------------------------------------------------------------
	// 其它app移动端使用短信验证码改密
	struct proto_sms_appmodifypasswd_req:public proto_sms_modifypasswd_req
	{
		enum{uri = LG_URI_UL2_SMS_APPMODPASSWD_REQ};
		std::vector<proto_jump_to>  _jump_tos;
		virtual void marshal( sox::Pack &pk )const
		{
			proto_sms_modifypasswd_req::marshal(pk);
			pk << _jump_tos;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			proto_sms_modifypasswd_req::unmarshal(up);
			up >> _jump_tos;
		}
	};

	struct proto_sms_appmodifypasswd_res:public proto_login_res
	{
		enum{uri = LG_URI_UL2_SMS_APPMODPASSWD_RES};
		std::string _webcookie;
		std::vector<proto_jump_otp> _jump_otps;
		virtual void marshal( sox::Pack &pk )const
		{
			proto_login_res::marshal(pk);
			pk << _webcookie << _jump_otps;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			proto_login_res::unmarshal(up);
			up >> _webcookie >> _jump_otps;
		}
	};

	//-----------------------------------------------------------------------------
	// 刷新图片
	struct proto_refresh_pic_req:public sox::Marshallable
	{
		enum{uri = LG_URI_UL2_REFRESHPIC_REQ};
		uint32_t       _proto_ver;     // 协议版本号，方便协议升级
		std::string    _context;       // 消息序列号
		proto_header   _proto_hdr;     // 公用协议头
		proto_session    _proto_session_data;  // 上一步操作返回的session_data
		proto_refresh_pic_req():_proto_ver(0){}
		virtual void marshal( sox::Pack &pk )const
		{
			pk << _proto_ver << _context << _proto_hdr << _proto_session_data;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			up >> _proto_ver >> _context >> _proto_hdr >> _proto_session_data;
		}
	};

	struct proto_refresh_pic_res:public sox::Marshallable
	{
		enum{uri = LG_URI_UL2_REFRESHPIC_RES};
		uint32_t    _proto_ver;       // 协议版本号，方便协议升级
		std::string _context;         // 消息序列号
		uint32_t    _errcode;         // 错误码
		std::string _errmsg;          // 针对错误码的错误信息,主要用于日志打印 
		std::string _pic;             // 获得的图片
		proto_session _proto_session_data;    // 用于下一步操作时使用
		proto_refresh_pic_res():_proto_ver(0), _errcode(0){}
		virtual void marshal( sox::Pack &pk )const
		{
			pk << _proto_ver << _context << _errcode << _errmsg << _pic << _proto_session_data;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			up >> _proto_ver >> _context >> _errcode >> _errmsg >> _pic >> _proto_session_data;
		}
	};
}} // namespace server{ namespace loginUDB

#endif // _login_udb_v2_2015_6_3
