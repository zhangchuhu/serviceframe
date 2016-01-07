#ifndef __LOGIN_I_H_
#define __LOGIN_I_H_

#include <iomanip>
#include<iostream>
#include<string>
#include<sstream>
#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/const.h"
#include "common/protocol/zip_tpl.h"
#include "protocol/CodeInfo.h"
#include <vector>
#include "common/res_code.h"

using namespace std;

namespace protocol{
	enum {LGNSV_SVID = 110};
}

namespace server{ namespace loginsv{
	// outer error code
	enum E_AUTH_RES
	{ 
		// 成功            对象不存在         代理失败         验证失败            
		ER_SUCCESS = 0,      ER_OBJ_NOTEXIST = 1, ER_UHS_FAIL = 2,  ER_AUTH_FAIL = 3, 
		// 登陆拒绝        服务超时           资源超过了限额   系统错误 
		ER_LOGIN_REJ = 4,    ER_TIMEOUT = 5,      ER_QUOTA = 6,     ER_SYSTEM = 7,
		// IP黑名单        冻结                      参数错误
		ER_IP_BLACKLIST = 8, ER_STATTUS_FROZEN = 12, ER_PARAM = 13
	};

	static const struct __S_AUTH_RES {
		int rc;
		const char* rmsg;
	} auth_res_code_list[] = {
		{ ER_SUCCESS, "SUCCESS"  },
		{ ER_OBJ_NOTEXIST, "OBJ_NOTEXIST"  },
		{ ER_UHS_FAIL, "UHS_FAIL"  },
		{ ER_AUTH_FAIL, "AUTH_FAIL"  },
		{ ER_LOGIN_REJ, "LOGIN_REJECT"  },
		{ ER_TIMEOUT, "TIMEOUT"  },
		{ ER_QUOTA, "OUT OF QUOTA"  },
		{ ER_SYSTEM, "SYSTEM ERROR"  },
		{ ER_IP_BLACKLIST, "IP_BLACKLIST"  },
		{ ER_STATTUS_FROZEN, "STATUS_FROZEN"  },
		{ ER_PARAM, "PARAM ERROR"  }
	};
	static const int count = sizeof(auth_res_code_list)/ sizeof(__S_AUTH_RES);

	inline const char* getAuthResErrmsg(int rcode)
	{
		for (int i = 0 ; i < count; i++) {
			if (rcode == auth_res_code_list[i].rc) {
				return auth_res_code_list[i].rmsg;
			}
		}
		return NULL;
	}

	// 最后追加的2个未知错误，是为了兼容以前status_frozen到12的情况
	const uint32_t RESCODE_INNER2OUTER[] = 
	{
		// 成功                        对象不存在                         代理失败
		ER_SUCCESS,                  ER_OBJ_NOTEXIST,                ER_UHS_FAIL,
		// 验证失败                    登陆拒绝                           服务超时
		ER_AUTH_FAIL,                ER_LOGIN_REJ,                   ER_TIMEOUT,
		// 资源超过了限额              系统错误                           IP黑名单
		ER_QUOTA,                    ER_SYSTEM,                      ER_IP_BLACKLIST,
		// 冻结                        特定版本的登录限制                 YY登录限制
		ER_STATTUS_FROZEN,           ER_LOGIN_REJ,                   ER_LOGIN_REJ,
		// 非常驻地登录限制
		ER_LOGIN_REJ,                 ER_LOGIN_REJ,                  ER_LOGIN_REJ
	};

	// inner error code
	enum E_INNER_RESCODE
	{ 
		// 成功                        对象不存在                         代理失败
		ER_INNER_SUCCESS = 0,        ER_INNER_OBJ_NOTEXIST = 1,      ER_INNER_UHS_FAIL = 2,
		// 验证失败                    登陆拒绝                           服务超时
		ER_INNER_AUTH_FAIL = 3,      ER_INNER_LOGIN_REJ = 4,         ER_INNER_TIMEOUT = 5,
		// 资源超过了限额              系统错误                           IP黑名单
		ER_INNER_QUOTA = 6,          ER_INNER_SYSTEM = 7,            ER_INNER_IP_BLACKLIST = 8,
		// 冻结                        特定版本的登录限制                 YY登录限制(禁止客户端登录)
		ER_INNER_STATTUS_FROZEN = 9, ER_INNER_VER_LOGIN_LIMIT = 10,  ER_INNER_YYLOGIN_LIMIT = 11, 
		// 非常驻地登录限制                    登录参数错误                yy号未开通登录功能,禁止用yy号登录
		ER_INNER_PERMANENTLOCATION_LIMIT = 12, ER_INNER_ERR_PARAM = 13, ER_INNER_YYID_LOGIN_LIMIT= 14
	};

	enum E_AUTH_SAFE_TYPE
	{
		// 安全
		ER_SAFE_OK = 0,
		// IP登陆验证           用户验证        IP密码错误验证         
		ER_IP_LT_VER = 1,     ER_ACCT_VER = 2,      ER_IP_PE_VER = 3,
		// IP登陆冻结           用户冻结            同密码错误验证
		ER_IP_LT_FROZEN = 4,  ER_ACCT_FROZEN = 5,   ER_PWD_VER = 6
	};

	const uint32_t SAFETYPE_INNER2OUTER[] =	
	{
		// 安全
		ER_SAFE_OK,
		// IP登陆验证     用户验证           IP密码错误验证         
		ER_IP_LT_VER,     ER_ACCT_VER ,      ER_IP_PE_VER ,
		// IP登陆冻结     用户冻结           同密码错误验证
		ER_IP_LT_FROZEN,  ER_ACCT_FROZEN,    ER_SAFE_OK 
	};

	// @ brief: lgsafe的行为码
	enum E_AUTH_SAFE_COMMAND_TYPE
	{
		E_SAFE_C_OK = 0,  // is ok
		E_SAFE_C_VER_PIC = 1, // do pic code verify
		E_SAFE_FROZEN = 2  // is frozen
	};

	// user_type 值
	enum EN_YY_USER_TYPE{ E_YYUSER_ERROR = -1, E_YYUSER_PASSPORT = 0, E_YYUSER_EMAIL = 1, E_YYUSER_YYID = 2, E_YYUSER_MOBILE = 3 };



// timer timeout ms ( timer_key2 )
#define KBS_ASREQ_TIMEOUT   1000*3
#define HWDKEY_AUTH_TIMEOUT 1000*60*2
#define PNDKEY_AUTH_TIMEOUT 1000*60*2
#define PICCODE_VERIFY_TIMEOUT 1000*60*2
#define DKEY_AUTHEN_TIMEOUT 1000*3
#define STATUS_DISABLE     4
#define STATUS_LOCK_PASSWD 8
#define KDC_VERSION_2 2

	// 生成UID的错误码
	enum E_GENUID_RES
	{ 
		// 成功                   ,超时               ,用户名已被使用
		E_GENUID_SUCCESS = 0,  E_GENUID_TIMEOUT,   E_GENUID_USED, 
		// 用户名空               ,服务失败           ,未知错误
		E_GENUI_UNAME_EMPTY,   E_GENUI_SERVER_FAIL, E_GENUID_UNKNOWN 
	};
	// 生成YYID的错误码
	enum E_GENYYID_RES
	{ 
		E_GENYYID_SUCCESS = 0, E_GENYYID_TIMEOUT,   E_GENYYID_SERVER_FAIL,
		E_GENYYID_UNKNOWN 
	};

	// Global fun
	template <class T> std::string toStr( T t)
	{
		ostringstream os;
		os << t;
		return os.str();
	}

	template <class T> T toNum( const char* s )
	{
		T tmp(0);
		istringstream is(s);
		is >> std::setbase(10) >> tmp;
		if(is.fail())
		{
			return 0;
		}
		return tmp;
	}

////////////////////////////////////////////////////////////////////////////////
// uri定义
/* 使用uri
   1. 63,64 验证手机令牌接口
   2. 128,129 验证硬件接口
   3. 130,131 新验证硬件令牌接口
   4. 132,133 新验证手机令牌接口
*/
enum E_LGNSV_URI
{
	URI_PERMISSION_REQ   = (1 << 8)|protocol::LGNSV_SVID,
	URI_PERMISSION_RES   = (2 << 8)|protocol::LGNSV_SVID,
	URI_AUTH_REQ         = (3 << 8)|protocol::LGNSV_SVID,
	URI_AUTH_RES         = (4 << 8)|protocol::LGNSV_SVID,
	URI_AUTHWITHSAFE_REQ = (5 << 8)|protocol::LGNSV_SVID,
	URI_AUTHWITHSAFE_RES = (6 << 8)|protocol::LGNSV_SVID,
	URI_GETUINFOBYUID_REQ = (7 << 8)|protocol::LGNSV_SVID,
	URI_GETUINFOBYUID_RES = (8 << 8)|protocol::LGNSV_SVID,
	URI_GETUINFOBYUNAME_REQ = (9 << 8)|protocol::LGNSV_SVID,
	URI_GETUINFOBYUNAME_RES = (10 << 8)|protocol::LGNSV_SVID,
	URI_GETUINFOBYEMAIL_REQ = (11 << 8)|protocol::LGNSV_SVID,
	URI_GETUINFOBYEMAIL_RES = (12 << 8)|protocol::LGNSV_SVID,
	URI_GETUINFOBYYYID_REQ = (13 << 8)|protocol::LGNSV_SVID,
	URI_GETUINFOBYYYID_RES = (14 << 8)|protocol::LGNSV_SVID,
	URI_GETUIDBYYYID_REQ = (15 << 8)|protocol::LGNSV_SVID,
	URI_GETUIDBYYYID_RES = (16 << 8)|protocol::LGNSV_SVID,
	URI_GETUIDBYUNAME_REQ = (17 << 8)|protocol::LGNSV_SVID,
	URI_GETUIDBYUNAME_RES = (18 << 8)|protocol::LGNSV_SVID,
	URI_GETUIDBYEMAIL_REQ = (19 << 8)|protocol::LGNSV_SVID,
	URI_GETUIDBYEMAIL_RES = (20 << 8)|protocol::LGNSV_SVID,
	URI_GETUNAMEBYUID_REQ = (21 << 8)|protocol::LGNSV_SVID,
	URI_GETUNAMEBYUID_RES = (22 << 8)|protocol::LGNSV_SVID,
	URI_GETUNAMEBYYYID_REQ = (23 << 8)|protocol::LGNSV_SVID,
	URI_GETUNAMEBYYYID_RES = (24 << 8)|protocol::LGNSV_SVID,
	URI_GETUNAMEBYEMAIL_REQ = (25 << 8)|protocol::LGNSV_SVID,
	URI_GETUNAMEBYEMAIL_RES = (26 << 8)|protocol::LGNSV_SVID,
	URI_UPDATEYYID_REQ = (27 << 8)|protocol::LGNSV_SVID,
	URI_UPDATEYYID_RES = (28 << 8)|protocol::LGNSV_SVID,
	URI_UPDATEEMAIL_REQ = (29 << 8)|protocol::LGNSV_SVID,
	URI_UPDATEEMAIL_RES = (30 << 8)|protocol::LGNSV_SVID,
	URI_REGISTERNORMAL_REQ = (31 << 8)|protocol::LGNSV_SVID,
	URI_REGISTERNORMAL_RES = (32 << 8)|protocol::LGNSV_SVID,
	URI_REGISTERPIP_REQ = (33 << 8)|protocol::LGNSV_SVID,
	URI_REGISTERPIP_RES = (34 << 8)|protocol::LGNSV_SVID,
	URI_GENYYUID_REQ = (35 << 8)|protocol::LGNSV_SVID,
	URI_GENYYUID_RES = (36 << 8)|protocol::LGNSV_SVID,
	URI_GENYYID_REQ = (37 << 8)|protocol::LGNSV_SVID,
	URI_GENYYID_RES = (38 << 8)|protocol::LGNSV_SVID,
	URI_AUTHSTATUS_REQ         = (39 << 8)|protocol::LGNSV_SVID,
	URI_AUTHSTATUS_RES         = (40 << 8)|protocol::LGNSV_SVID,
	URI_AUTHWITHSAFESTATUS_REQ = (41 << 8)|protocol::LGNSV_SVID,
	URI_AUTHWITHSAFESTATUS_RES = (42 << 8)|protocol::LGNSV_SVID,
	URI_GENYYIDWITHUNAME_REQ = (43 << 8)|protocol::LGNSV_SVID,
	URI_AUTHWITHSAFE_REQ2 = (44 << 8)|protocol::LGNSV_SVID,
	URI_REQUESTDKEY = (45 << 8)|protocol::LGNSV_SVID,
	URI_CHECKDKEY   = (46 << 8)|protocol::LGNSV_SVID,
	URI_CHECKDKEYFAILURE = (47 << 8)|protocol::LGNSV_SVID,
	URI_LOADCONFIG_REQ = (48 << 8)|protocol::LGNSV_SVID,
	URI_LOADCONFIG_RES = (49 << 8)|protocol::LGNSV_SVID,
	URI_HEARTBEAT_REQ = (50 << 8)|protocol::LGNSV_SVID,
	URI_HEARTBEAT_RES = (51 << 8)|protocol::LGNSV_SVID,
	URI_LOGIN_ACCTINFO_REQ = (52 << 8)|protocol::LGNSV_SVID,
	URI_LOGIN_ACCTINFO_RES = (53 << 8)|protocol::LGNSV_SVID,
	URI_LOGIN_REQ = (60 << 8)|protocol::LGNSV_SVID,
	URI_LOGIN_RES = (61 << 8)|protocol::LGNSV_SVID,
	URI_LOGIN3RD_REQ = (62 << 8)|protocol::LGNSV_SVID,
	URI_LOGIN3RD_RES = (63 << 8)|protocol::LGNSV_SVID, // 与硬件里的请求重叠，但不影响
	URI_LOGINMOB_REQ = (65 << 8)|protocol::LGNSV_SVID, // 硬件令牌响应使用了64
	URI_LOGINMOB_RES = (66 << 8)|protocol::LGNSV_SVID,
	URI_GETWEBYY_ACCT_REQ = (67 << 8)|protocol::LGNSV_SVID,
	URI_GETWEBYY_ACCT_RES = (68 << 8)|protocol::LGNSV_SVID,
	URI_GET_PREREGUID = (69 << 8)|protocol::LGNSV_SVID,
	URI_GET_PREREGUID_RES = (70 << 8)|protocol::LGNSV_SVID
};

////////////////////////////////////////////////////////////////////////////////
/* 认证接口，3个：
1 yyid + passwd
2 uname + passwd
3 email + passwd
uri range: (1 << 8)| protocol::LGNSV_SVID ~ (6 << 8)| protocol::LGNSV_SVID
*/
struct LgnsvPermissionReq:public sox::Marshallable
{
	enum{uri = URI_PERMISSION_REQ};
	std::string  _context;
	uint8_t      _user_type;   //0 - yyid, 1 - username, 2 - email
	std::string  _user;
	uint32_t     _ip;

	LgnsvPermissionReq():_ip(0){}
	virtual void marshal(sox::Pack &pk) const 
	{
		pk << _context << _user_type << _user << _ip;
	}

	virtual void unmarshal(const sox::Unpack &up) 
	{
		up >> _context >> _user_type >> _user >> _ip;
	}
};

struct LgnsvPermissionRes:public sox::Marshallable
{
	enum{uri = URI_PERMISSION_RES};
	std::string  _context;
	uint8_t      _perm_code;  // 许可编码
	LgnsvPermissionRes():_perm_code(0){}
	virtual void marshal(sox::Pack &pk) const 
	{
		pk << _context << _perm_code;
	}

	virtual void unmarshal(const sox::Unpack &up) 
	{
		up >> _context >> _perm_code;
	}
};

struct LgnsvLoginReqBase:public sox::Marshallable
{
	enum{ut_yyid = 0, ut_username, ut_email, ut_ambiguous };
	std::string _context;
	uint8_t     _client_type;    // 0 - pc, 1 - mobile, 2 - web, 3 - proxy
	uint32_t    _client_ip;
	uint32_t    _proxy_ip;       // 没有可以不填
	uint32_t    _caller_ip;
	uint8_t     _user_type;      //enum EN_YY_USER_TYPE
	std::string _user;
	std::string _passwd;
	uint8_t     _authType;       // 验证方式：0-用户名密码；1-手机令牌
	std::string _clien_version;
	uint32_t    _version_num;
	uint32_t    _lcid;           // 国际化区域ID

	LgnsvLoginReqBase():_client_type(0), _client_ip(0), _proxy_ip(0), 
		_caller_ip(0), _user_type(0), _authType(0), _version_num(0), _lcid(0){}

	LgnsvLoginReqBase& operator=( const LgnsvLoginReqBase& rth )
	{
		if( this == &rth )
		{
			return *this;
		}

		_context = rth._context;
		_client_type = rth._client_type;
		_client_ip = rth._client_ip;
		_proxy_ip = rth._proxy_ip;
		_caller_ip = rth._caller_ip;
		_user_type = rth._user_type;
		_user = rth._user;
		_passwd = rth._passwd;
		_authType = rth._authType;
		_clien_version = rth._clien_version;
		_version_num = rth._version_num;
		_lcid = rth._lcid;

		return *this;
	}

	virtual void marshal(sox::Pack &pk) const
	{
		pk << _context << _client_type << _client_ip << _proxy_ip
			<< _caller_ip << _user_type << _user << _passwd << _authType 
			<< _clien_version << _version_num << _lcid;
	}

	virtual void unmarshal(const sox::Unpack &up)
	{
		up >> _context >> _client_type >> _client_ip >> _proxy_ip
			>> _caller_ip >> _user_type >> _user >> _passwd >> _authType 
			>> _clien_version >> _version_num >> _lcid;
	}
};

struct LgnsvLoginResBase:public sox::Marshallable
{
	std::string _context;
	uint32_t    _yyid;
	uint32_t    _yyuid;
	std::string _uname;
	std::string _email;
	uint32_t    _safe_type;
	uint32_t    _resCode;
	uint32_t    _login_fail_errcode; // for pop msg(in cli_pop_msg.xml err-code, default 0)
	std::string _login_fail_msg;     // for pop msg(in cli_pop_msg.xml msg-content)
	uint32_t	_version;
	std::string _extension;

	LgnsvLoginResBase():_yyid(0), _yyuid(0), _safe_type(0), _resCode(0), _login_fail_errcode(0), _version(0){}

	LgnsvLoginResBase& operator=( const LgnsvLoginResBase& rth )
	{
		if( this == &rth )
		{
			return *this;
		}

		_context = rth._context;
		_yyid = rth._yyid;
		_yyuid = rth._yyuid;
		_uname = rth._uname;
		_email = rth._email;
		_safe_type = rth._safe_type;
		_resCode = rth._resCode;
		_login_fail_errcode = rth._login_fail_errcode;
		_login_fail_msg = rth._login_fail_msg;
		_version = rth._version;
		_extension = rth._extension;
		return *this;
	}

	virtual void marshal(sox::Pack &pk) const
	{
		// 如果登录失败，则清除票据部分
		std::string extension = _extension;
		if( ER_SUCCESS != _resCode || ER_IP_LT_FROZEN == _safe_type || ER_ACCT_FROZEN == _safe_type )
		{
			extension.clear();
		}
		pk << _context << _yyid << _yyuid << _uname << _email << _safe_type << _resCode
			<< _login_fail_errcode << _login_fail_msg << _version << extension;
	}

	virtual void unmarshal(const sox::Unpack &up)
	{
		up >> _context >> _yyid >> _yyuid >> _uname >> _email >> _safe_type >> _resCode
			>> _login_fail_errcode >> _login_fail_msg >> _version >> _extension;
	}
};

struct LgnsvLoginReq: public LgnsvLoginReqBase
{
	enum{uri = URI_LOGIN_REQ };
	std::string _mac_address;        // 客户端mac地址
	LgnsvLoginReq():LgnsvLoginReqBase(){}
	LgnsvLoginReq& operator=( const LgnsvLoginReq& rth )
	{
		if( this == &rth )
		{
			return *this;
		}
		LgnsvLoginReqBase::operator=( rth );
		_mac_address = rth._mac_address;
		return *this;
	}

	virtual void marshal(sox::Pack &pk) const
	{
		LgnsvLoginReqBase::marshal(pk);
		if (!_mac_address.empty())
		{
			pk << _mac_address;
		}
	}

	virtual void unmarshal(const sox::Unpack &up)
	{
		LgnsvLoginReqBase::unmarshal(up);
		if (!up.empty())
		{
			up >> _mac_address;
		}
	}
};

struct LgnsvLoginRes: public LgnsvLoginResBase
{
	enum{uri = URI_LOGIN_RES };
	LgnsvLoginRes():LgnsvLoginResBase(){}
	LgnsvLoginRes& operator=( const LgnsvLoginRes& rth )
	{
		if( this == &rth )
		{
			return *this;
		}
		LgnsvLoginResBase::operator=( rth );

		return *this;
	}
	
	virtual void marshal(sox::Pack &pk) const
	{
		LgnsvLoginResBase::marshal(pk);
	}

	virtual void unmarshal(const sox::Unpack &up)
	{
		LgnsvLoginResBase::unmarshal(up);
	}
};

struct LgnsvMobLoginReq:public LgnsvLoginReqBase
{
	std::string _appid;
	enum{uri = URI_LOGINMOB_REQ };
	LgnsvMobLoginReq():LgnsvLoginReqBase(){}
	LgnsvMobLoginReq& operator=( const LgnsvMobLoginReq& rth )
	{
		if( this == &rth )
		{
			return *this;
		}

		LgnsvLoginReqBase::operator =(rth);
		_appid = rth._appid;

		return *this;
	}

	virtual void marshal(sox::Pack &pk) const
	{
		LgnsvLoginReqBase::marshal(pk);
		pk << _appid;
	}

	virtual void unmarshal(const sox::Unpack &up)
	{
		LgnsvLoginReqBase::unmarshal(up);
		if( !up.empty() )
		{
			up >> _appid;
		}
	}
};

struct LgnsvMobLoginRes:public LgnsvLoginResBase
{
	uint64_t  _udbuid;
	enum{uri = URI_LOGINMOB_RES };
	LgnsvMobLoginRes():LgnsvLoginResBase(),_udbuid(0){}
	LgnsvMobLoginRes& operator=( const LgnsvMobLoginRes& rth )
	{
		if( this == &rth )
		{
			return *this;
		}

		LgnsvLoginResBase::operator =(rth);
		_udbuid = rth._udbuid;

		return *this;
	}

	virtual void marshal(sox::Pack &pk) const
	{
		LgnsvLoginResBase::marshal( pk );
		pk << _udbuid;
	}

	virtual void unmarshal(const sox::Unpack &up)
	{
		LgnsvLoginResBase::unmarshal(up);
		up >> _udbuid;
	}
};

struct LgnsvLoginBy3rdReq: public sox::Marshallable
{
	enum{uri = URI_LOGIN3RD_REQ };
	std::string _context;
	uint8_t     _client_type;    // 0 - pc, 1 - mobile, 2 - web, 3 - proxy
	uint32_t    _client_ip;
	uint32_t    _proxy_ip;       // 没有可以不填
	uint32_t    _caller_ip;
	std::string _user;
	std::string _account_info;   // 
	std::string _clien_version;
	uint32_t    _version_num;
	uint32_t    _lcid;           // 国际化区域ID
	uint8_t     _user_type;     // 用户类型, 不是由请求方发出, 由收到的一方自己判断

	LgnsvLoginBy3rdReq():_client_type(0), _client_ip(0), _proxy_ip(0), 
		_caller_ip(0), _version_num(0), _lcid(0), _user_type(0) {}

	LgnsvLoginBy3rdReq& operator=( const LgnsvLoginBy3rdReq& rth )
	{
		if( this == &rth )
		{
			return *this;
		}

		_context = rth._context;
		_client_type = rth._client_type;
		_client_ip = rth._client_ip;
		_proxy_ip = rth._proxy_ip;
		_caller_ip = rth._caller_ip;
		_user = rth._user;
		_account_info = rth._account_info;
		_clien_version = rth._clien_version;
		_version_num = rth._version_num;
		_lcid = rth._lcid;
		_user_type = rth._user_type;

		return *this;
	}

	virtual void marshal(sox::Pack &pk) const
	{
		pk << _context << _client_type << _client_ip << _proxy_ip
			<< _caller_ip << _user << _account_info 
			<< _clien_version << _version_num << _lcid;
	}

	virtual void unmarshal(const sox::Unpack &up)
	{
		up >> _context >> _client_type >> _client_ip >> _proxy_ip
			>> _caller_ip >> _user >> _account_info 
			>> _clien_version >> _version_num >> _lcid;
	}
};

struct LgnsvLoginBy3rdRes: public sox::Marshallable
{
	enum{uri = URI_LOGIN3RD_RES };
	std::string _context; 
	uint32_t    _yyid;
	uint32_t    _yyuid;
	std::string _uname;
	uint32_t    _safe_type;
	uint32_t    _resCode;
	uint32_t    _login_fail_errcode;
	std::string _login_fail_msg;
	uint32_t	_version;
	std::string _extension;

	LgnsvLoginBy3rdRes():_yyid(0), _yyuid(0), _safe_type(0), _resCode(0), _login_fail_errcode(0), _version(0){}

	LgnsvLoginBy3rdRes& operator=( const LgnsvLoginBy3rdRes& rth )
	{
		if( this == &rth )
		{
			return *this;
		}

		_context = rth._context;
		_yyid = rth._yyid;
		_yyuid = rth._yyuid;
		_uname = rth._uname;
		_safe_type = rth._safe_type;
		_resCode = rth._resCode;
		_login_fail_errcode = rth._login_fail_errcode;
		_login_fail_msg = rth._login_fail_msg;
		_version = rth._version;
		_extension = rth._extension;
		return *this;
	}

	virtual void marshal(sox::Pack &pk) const
	{
		pk << _context << _yyid << _yyuid << _uname << _safe_type << _resCode
			<< _login_fail_errcode << _login_fail_msg << _version << _extension;
	}

	virtual void unmarshal(const sox::Unpack &up)
	{
		up >> _context >> _yyid >> _yyuid >> _uname >> _safe_type >> _resCode
			>> _login_fail_errcode >> _login_fail_msg >> _version >> _extension;
	}
};

struct LgnsvAuthBase: public sox::Marshallable
{
	enum{ut_yyid = 0, ut_username, ut_email, ut_ambiguous };
	std::string  _context;
	uint8_t      _user_type;   //0 - yyid, 1 - username, 2 - email, 3 - ambiguous(username or yyid) 
	std::string  _user;
	std::string  _passwd;
	uint32_t     _client_ip;
#define _ip      _client_ip    // 为了兼容旧用法
	LgnsvAuthBase():_user_type(0), _ip(0){}

	virtual void marshal(sox::Pack &pk) const 
	{
		pk << _context << _user_type << _user << _passwd << _ip;
	}

	virtual void unmarshal(const sox::Unpack &up) 
	{
		up >> _context >> _user_type >> _user >> _passwd >> _ip;
	}
};

struct LgnsvAuthReq: public LgnsvAuthBase
{
	uint32_t _appid;
	enum{uri = URI_AUTH_REQ };
	LgnsvAuthReq():LgnsvAuthBase(),_appid(0){}
	virtual void marshal(sox::Pack &pk) const
	{
		LgnsvAuthBase::marshal(pk);
		pk << _appid;
	}

	virtual void unmarshal(const sox::Unpack &up)
	{
		LgnsvAuthBase::unmarshal(up);
		if( !up.empty() )
		{
			up >> _appid;
		}
	}
};

struct LgnsvAuthRes: public sox::Marshallable
{
	enum{uri = URI_AUTH_RES};
	std::string _context; 
	uint32_t    _yyid;
	uint32_t    _yyuid;
	std::string _uname;
	std::string _email;
	uint32_t    _resCode;
	uint32_t	_version;
	std::string _extension;

	LgnsvAuthRes():_yyid(0), _yyuid(0), _resCode(0), _version(0){}

	virtual void marshal(sox::Pack &pk) const 
	{
		pk << _context << _yyid << _yyuid << _uname << _email << _resCode;
		if( !_extension.empty() )
		{
			pk << _version << _extension;
		}
	}

	virtual void unmarshal(const sox::Unpack &up) 
	{
		up >> _context >> _yyid >> _yyuid >> _uname >> _email >> _resCode;
		if( !up.empty() )
		{
			up >> _version >> _extension;
		}
	}
};

struct LgnsvAuthWithSafeReq: public LgnsvAuthBase
{
	enum{ uri = URI_AUTHWITHSAFE_REQ };
	std::string  _clien_version;
	virtual void marshal(sox::Pack &pk) const 
	{
		LgnsvAuthBase::marshal(pk);
		if( !_clien_version.empty() )
		{
			pk << _clien_version;
		}
	}

	virtual void unmarshal(const sox::Unpack &up) 
	{
		LgnsvAuthBase::unmarshal(up);
		if( !up.empty() )
		{
			up >> _clien_version;
		}
	}
};

struct LgnsvAuthWithSafeReq2: public LgnsvAuthBase
{
	enum{ uri = URI_AUTHWITHSAFE_REQ2 };
	uint32_t _authType; // 验证方式：0-用户名密码；1-手机令牌
	std::string  _clien_version;

	LgnsvAuthWithSafeReq2() : _authType(0) {}

	virtual void marshal(sox::Pack &pk) const 
	{
		LgnsvAuthBase::marshal(pk);
		pk << _authType;
		if( !_clien_version.empty() )
		{
			pk << _clien_version;
		}
	}

	virtual void unmarshal(const sox::Unpack &up) 
	{
		LgnsvAuthBase::unmarshal(up);
		up >> _authType;
		if( !up.empty() )
		{
			up >> _clien_version;
		}
	}
};

struct LgnsvAuthWithSafeRes: public sox::Marshallable
{
	enum{ uri = URI_AUTHWITHSAFE_RES };
	std::string _context; 
	uint32_t    _yyid;
	uint32_t    _yyuid;
	std::string _uname;
	std::string _email;
	uint32_t    _acct_try_times;
	uint32_t    _ip_try_times;
	uint32_t    _ip_err_times;
	uint32_t    _safe_type;
	uint32_t    _resCode;
	uint32_t	_version;
	std::string _extension;

	LgnsvAuthWithSafeRes()
		:_yyid(0)
		,_yyuid(0)
		,_acct_try_times(0)
		,_ip_try_times(0)
		,_ip_err_times(0)
		,_safe_type(0)
		,_resCode(0)
		,_version(0)
	{}

	LgnsvAuthWithSafeRes& operator=( const LgnsvAuthWithSafeRes& rth )
	{
		if( this == &rth )
		{
			return *this;
		}
		_context = rth._context;
		_yyid = rth._yyid;
		_yyuid = rth._yyuid;
		_uname = rth._uname;
		_email = rth._email;
		_acct_try_times = rth._acct_try_times;
		_ip_try_times = rth._ip_try_times;
		_ip_err_times = rth._ip_err_times;
		_safe_type = rth._safe_type;
		_resCode = rth._resCode;
		_version = rth._version;
		_extension = rth._extension;
		return *this;
	}

	virtual void marshal(sox::Pack &pk) const 
	{
		pk << _context << _yyid << _yyuid << _uname << _email 
			<< _acct_try_times << _ip_try_times << _ip_err_times << _safe_type << _resCode;
		if( !_extension.empty() )
		{
			pk << _version << _extension;
		}
	}

	virtual void unmarshal(const sox::Unpack &up) 
	{
		up >> _context >> _yyid >> _yyuid >> _uname >> _email 
			>> _acct_try_times >> _ip_try_times >> _ip_err_times >> _safe_type >> _resCode;
		if( !up.empty() )
		{
			up >> _version >> _extension;
		}
	}
};

////////////////////////////////////////////////////////////////////////////////
// For status special
struct LgnsvAuthStatusReq: public LgnsvAuthBase
{
	std::string _appid;
	enum{uri = URI_AUTHSTATUS_REQ};
	LgnsvAuthStatusReq():LgnsvAuthBase(){}

	virtual void marshal(sox::Pack &pk) const
	{
		LgnsvAuthBase::marshal( pk );
		pk << _appid;
	}
	virtual void unmarshal(const sox::Unpack &up)
	{
		LgnsvAuthBase::unmarshal( up );
		if( !up.empty() )
		{
			up >> _appid;
		}
	}
};

struct LgnsvAuthStatusRes: public sox::Marshallable
{
	enum{uri = URI_AUTHSTATUS_RES};
	std::string _context; 
	uint32_t    _yyid;
	uint32_t    _yyuid;
	std::string _uname;
	std::string _email;
	uint32_t    _resCode;
	uint32_t	_version;
	std::string _extension;

	LgnsvAuthStatusRes():_yyid(0), _yyuid(0), _resCode(0), _version(0){}

	virtual void marshal(sox::Pack &pk) const 
	{
		pk << _context << _yyid << _yyuid << _uname << _email << _resCode;
		if( !_extension.empty() )
		{
			pk << _version << _extension;
		}
	}

	virtual void unmarshal(const sox::Unpack &up) 
	{
		up >> _context >> _yyid >> _yyuid >> _uname >> _email >> _resCode;
		if( !up.empty() )
		{
			up >> _version >> _extension;
		}
	}
};

//////////////////////////////////////////////////////////////////
// @brief: web yy acctinfo
struct PGetWebyyCertificateReq : public sox::Marshallable 
{
	enum{uri = URI_GETWEBYY_ACCT_REQ};

	std::string appid;  // 是哪个系统要求产的?
	std::string cli_ip; // 用户端的ip
	std::string user;   // passport|email|yyid|mobile
	std::string accessToken; // 请求系统发来的验证码(加密字段)

	virtual void marshal(sox::Pack &p) const {
		p << appid << cli_ip << user << accessToken;	
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> appid >> cli_ip >> user >> accessToken;	
	}
};

struct PGetWebyyCertificateRes : public sox::Marshallable
{
	enum{uri = URI_GETWEBYY_ACCT_RES};

	uint32_t resCode; // enum: E_AUTH_RES
	std::string err_msg; // 错误时的反馈信息
	std::string passport; // passport
	std::string certificate; // acctinfo data(加密字段)

	PGetWebyyCertificateRes() {
		resCode = 0;
		err_msg = "";
		passport = "";
		certificate = "";
	}

	virtual void marshal(sox::Pack &p) const {
		p << resCode << err_msg << passport << certificate;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> resCode >> err_msg >> passport >> certificate;	
	}
};


struct LgnsvAuthWithSafeStatusReq: public LgnsvAuthBase
{
	enum{ uri = URI_AUTHWITHSAFESTATUS_REQ };
};

struct LgnsvAuthWithSafeStatusRes: public sox::Marshallable
{
	enum{ uri = URI_AUTHWITHSAFESTATUS_RES };
	std::string _context; 
	uint32_t    _yyid;
	uint32_t    _yyuid;
	std::string _uname;
	std::string _email;
	uint32_t    _acct_try_times;
	uint32_t    _ip_try_times;
	uint32_t    _ip_err_times;
	uint32_t    _safe_type;
	uint32_t    _resCode;
	uint32_t	_version;
	std::string _extension;

	LgnsvAuthWithSafeStatusRes()
		:_yyid(0)
		,_yyuid(0)
		,_acct_try_times(0)
		,_ip_try_times(0)
		,_ip_err_times(0)
		,_safe_type(0)
		,_resCode(0)
		,_version(0)
	{}

	virtual void marshal(sox::Pack &pk) const 
	{
		pk << _context << _yyid << _yyuid << _uname << _email 
			<< _acct_try_times << _ip_try_times << _ip_err_times << _safe_type << _resCode;
		if( !_extension.empty() )
		{
			pk << _version << _extension;
		}
	}

	virtual void unmarshal(const sox::Unpack &up) 
	{
		up >> _context >> _yyid >> _yyuid >> _uname >> _email 
			>> _acct_try_times >> _ip_try_times >> _ip_err_times >> _safe_type >> _resCode;
		if( !up.empty() )
		{
			up >> _version >> _extension;
		}
	}
};

////////////////////////////////////////////////////////////////////////////////
/* 用户信息访问接口，10个：
1  通过yyuid获取uname, email, yyid
2  通过uname获取yyuid, email, yyid
3  通过email获取yyuid, uname, yyid
4  通过yyid获取yyuid, uname, email
5  通过uname获取yyuid
6  通过email获取yyuid
7  通过yyid获取yyuid
8  通过yyuid获取uname
9  通过yyid获取uname
10 通过email获取uname
uri range: (101 << 8)| protocol::LGNSV_SVID ~ (116 << 8)| protocol::LGNSV_SVID
*/
//==============================================================================
struct LgnsvGetUinfoRes: public sox::Marshallable
{
	std::string _uname;
	std::string _email;
	uint32_t    _yyuid;
	uint32_t    _yyid;
	uint32_t    _resCode;

	LgnsvGetUinfoRes():_yyuid(0), _yyid(0),_resCode(0){}

	LgnsvGetUinfoRes( const std::string &uname, const std::string &email, 
		const uint32_t yyuid, const uint32_t yyid, const uint32_t resCode):
	_uname(uname), _email(email), _yyuid(yyuid), _yyid(yyid), _resCode(resCode){}

	virtual void marshal(sox::Pack &pk) const 
	{
		pk << _uname << _email << _yyuid << _yyid << _resCode;
	}

	virtual void unmarshal(const sox::Unpack &up) 
	{
		up >> _uname >> _email >> _yyuid >> _yyid >> _resCode;
	}
};





////////////////////////////////////////////////////////////////////////////////
/* 获取yyuid,yyid，2个：
1 获取yyuid
2 获取yyid
*/
struct LgnsvGenYyuidReq: public sox::Marshallable
{
	enum{ uri = URI_GENYYUID_REQ };
	std::string  _seq;      // 消息序列，格式:YYYYMMDDHH24MISS_xxxx.yyyy，其中xxxx为数字序列，yyyy为host编码
	std::string  _uname;
	std::string  _passwd;
	std::string  _email;

	LgnsvGenYyuidReq(){}
	LgnsvGenYyuidReq( const std::string &seq, const std::string &uname, const std::string &passwd, 
		const std::string &email )
		:_seq( seq )
		,_uname(uname)
		,_passwd(passwd)
		,_email(email)
	{}

	virtual void marshal(sox::Pack &pk) const 
	{
		pk << _seq << _uname << _passwd << _email;
	}

	virtual void unmarshal(const sox::Unpack &up) 
	{
		up >> _seq >> _uname >> _passwd >> _email;
	}
};

struct LgnsvGenYyuidRes: public sox::Marshallable
{
	enum{ uri = URI_GENYYUID_RES };
	std::string  _seq;      // 消息序列，格式:YYYYMMDDHH24MISS_xxxx.yyyy，其中xxxx为数字序列，yyyy为host编码
	std::string  _uname;
	uint32_t     _yyuid;
	uint32_t     _resCode;

	LgnsvGenYyuidRes(): _yyuid(0), _resCode(0){}
	LgnsvGenYyuidRes( const std::string &seq, 
		const std::string &uname, 
		const uint32_t yyuid, 
		const uint32_t resCode ):
	_seq(seq), _uname(uname), _yyuid(yyuid), _resCode(resCode){}

	virtual void marshal(sox::Pack &pk) const 
	{
		pk << _seq << _uname << _yyuid << _resCode;
	}

	virtual void unmarshal(const sox::Unpack &up) 
	{
		up >> _seq >> _uname >> _yyuid >> _resCode;
	}
};

struct LgnsvGenYyidReq: public sox::Marshallable
{
	enum{ uri = URI_GENYYID_REQ };
	std::string  _seq;      // 消息序列，格式:YYYYMMDDHH24MISS_xxxx.yyyy，其中xxxx为数字序列，yyyy为host编码
	uint32_t     _yyuid;

	LgnsvGenYyidReq():_yyuid(0){}
	LgnsvGenYyidReq( const std::string &seq, const uint32_t yyuid ):_seq(seq), _yyuid(yyuid){}

	virtual void marshal(sox::Pack &pk) const 
	{
		pk << _seq << _yyuid;
	}

	virtual void unmarshal(const sox::Unpack &up) 
	{
		up >> _seq >> _yyuid;
	}
};

struct LgnsvGenYyidRes: public sox::Marshallable
{
	enum{ uri = URI_GENYYID_RES };
	std::string  _seq;      // 消息序列，格式:YYYYMMDDHH24MISS_xxxx.yyyy，其中xxxx为数字序列，yyyy为host编码
	uint32_t     _yyuid;
	uint32_t     _yyid;
	uint32_t     _resCode;

	LgnsvGenYyidRes():_yyuid(0), _yyid(0), _resCode(0){}
	LgnsvGenYyidRes( const std::string &seq, const uint32_t yyuid, 
		const uint32_t yyid, const uint32_t resCode ):
	_seq(seq), _yyuid(yyuid), _yyid(yyid), _resCode(resCode){}

	virtual void marshal(sox::Pack &pk) const 
	{
		pk << _seq << _yyuid << _yyid << _resCode;
	}

	virtual void unmarshal(const sox::Unpack &up) 
	{
		up >> _seq >> _yyuid >> _yyid >> _resCode;
	}
};

// 生成yyid，+username
struct LgnsvGenYyidWithUnameReq: public sox::Marshallable
{
	enum{ uri = URI_GENYYIDWITHUNAME_REQ };
	std::string  _seq;      // 消息序列，格式:YYYYMMDDHH24MISS_xxxx.yyyy，其中xxxx为数字序列，yyyy为host编码
	uint32_t     _yyuid;
	std::string  _username;

	LgnsvGenYyidWithUnameReq():_yyuid(0){}
	LgnsvGenYyidWithUnameReq( const std::string &seq, const uint32_t yyuid, const std::string uname )
		:_seq(seq), _yyuid(yyuid), _username(uname){}

	virtual void marshal(sox::Pack &pk) const 
	{
		pk << _seq << _yyuid << _username;
	}

	virtual void unmarshal(const sox::Unpack &up) 
	{
		up >> _seq >> _yyuid >> _username;
	}
};

// logind -> lbs
// 请求客户端手机令牌
//==============================================================================
struct LgnsvRequestDKey: public sox::Marshallable
{
  enum{ uri = URI_REQUESTDKEY };

  std::string  _context;
  std::string  _login_context;
  std::string  _uname;
  uint32_t     _yyuid;
  uint32_t     _timestamp;
  uint32_t     _serverId;
  LgnsvRequestDKey():_yyuid(0), _timestamp(0), _serverId(0){}

  virtual void marshal(sox::Pack &pk) const 
  {
    pk << _context << _login_context << _uname << _yyuid << _timestamp;
	if( 0 != _serverId )
	{
		pk << _serverId;
	}
  }

  virtual void unmarshal(const sox::Unpack &up) 
  {
    up >> _context >> _login_context >> _uname >> _yyuid >> _timestamp;
	if( !up.empty() )
	{
		up >> _serverId;
	}
  }
};

struct LgnsvCheckDKey: public sox::Marshallable
{
  enum{ uri = URI_CHECKDKEY };

  std::string  _context;
  std::string  _login_context;
  std::string  _uname;
  uint32_t     _yyuid;
  uint32_t     _dkey;

  virtual void marshal(sox::Pack &pk) const 
  {
    pk << _context << _login_context << _uname << _yyuid << _dkey;
  }

  virtual void unmarshal(const sox::Unpack &up) 
  {
    up >> _context >> _login_context >> _uname >> _yyuid >> _dkey;
  }
};

struct LgnsvCheckDKeyFailure: public sox::Marshallable
{
  enum{ uri = URI_CHECKDKEYFAILURE };

  std::string  _context;
  std::string  _uname;
  uint32_t     _yyuid;
  uint32_t     _resCode;

  virtual void marshal(sox::Pack &pk) const 
  {
    pk << _context << _uname << _yyuid << _resCode;
  }

  virtual void unmarshal(const sox::Unpack &up) 
  {
    up >> _context >> _uname >> _yyuid >> _resCode;
  }
};

// logind动态验证消息请求
struct LgnsvDynaCheckAsk: public sox::Marshallable
{
	enum{ uri = ( 91 << 8 )|protocol::SERVERLOGIN_SVID };  //protocol::SERVERLOGIN_SVID = 233
	std::string  _context;
	std::string  _login_context;
	std::string  _uname;
	uint32_t     _yyuid;
	uint32_t     _serverId;  // logind的服务标识
	std::string  _cli_load;  // 客户端子协议消息
	LgnsvDynaCheckAsk():_yyuid(0), _serverId(0){}
	virtual void marshal(sox::Pack &pk) const
	{
		pk << _context << _login_context << _uname << _yyuid << _serverId << _cli_load;
	}
	virtual void unmarshal(const sox::Unpack &up)
	{
		up >> _context >> _login_context >> _uname >> _yyuid >> _serverId >> _cli_load;
	}

	struct PPicCodeLoad: public sox::Marshallable
	{
		enum { uri = (215 << 8 |protocol::SERVERLOGIN_SVID) };
		/*
		0 - 第一次请求验证码
		1 - 验证码检验失败
		2 - 达到最大验证次数，客户端不能继续发了，发了服务端也丢弃了
		*/
		enum { RES_REQUEST_PICCODE = 0, RES_VERIFY_PICCODE_FAIL = 1, RES_VERIFY_TIMES_REACH_MAX = 2 };
		std::string  _pic_id;
		std::string  _pic_code;
		std::string  _reason;
		uint32_t     _rescode;

		virtual void marshal(sox::Pack &pk) const
		{
			pk << _pic_id << _pic_code << _reason << _rescode;
		}
		virtual void unmarshal(const sox::Unpack &up)
		{
			up >> _pic_id >> _pic_code >> _reason >> _rescode;
		}
	};
};

// 动态验证消息应答
struct LgnsvDynaCheckAck: public sox::Marshallable
{
	enum{ uri = ( 92 << 8 )|protocol::SERVERLOGIN_SVID };  //protocol::SERVERLOGIN_SVID = 233
	std::string  _context;
	std::string  _login_context;
	std::string  _uname;
	uint32_t     _yyuid;
	std::string  _cli_load;  // 客户端子协议消息
	LgnsvDynaCheckAck():_yyuid(0){}
	virtual void marshal(sox::Pack &pk) const
	{
		pk << _context << _login_context << _uname << _yyuid << _cli_load;
	}
	virtual void unmarshal(const sox::Unpack &up)
	{
		up >> _context >> _login_context >> _uname >> _yyuid >> _cli_load;
	}

	struct PPicCodeVerify
	{
		enum {uri = (216 << 8 |protocol::SERVERLOGIN_SVID)};
		std::string pic_id;
		std::string vali_code;

		virtual void marshal(sox::Pack &pk) const
		{
			pk << pic_id << vali_code;
		}
		virtual void unmarshal(const sox::Unpack &up)
		{
			up >> pic_id >> vali_code;
		}
	};
};

struct LgnsvHeartBeatReq: public sox::Marshallable
{
	uint32_t  _val;
	enum{ uri = URI_HEARTBEAT_REQ };
	LgnsvHeartBeatReq():_val(0){}
	virtual void marshal(sox::Pack &pk) const 
	{
		pk << _val;
	}

	virtual void unmarshal(const sox::Unpack &up) 
	{
		up >> _val;
	}
};

struct LgnsvHeartBeatRes: public sox::Marshallable
{
	uint32_t  _val;
	enum{ uri = URI_HEARTBEAT_RES };
	LgnsvHeartBeatRes():_val(0){}
	virtual void marshal(sox::Pack &pk) const 
	{
		pk << _val;
	}

	virtual void unmarshal(const sox::Unpack &up) 
	{
		up >> _val;
	}
};

struct LoginAcctInfoReq:public sox::Marshallable
{
	enum{uri = URI_LOGIN_ACCTINFO_REQ};
	std::string _context;
	std::string _client_ip;
	std::string _service_type;
	std::string _username;
	std::string _password;
	virtual void marshal(sox::Pack &pk)const
	{
		pk << _context << _client_ip << _service_type << _username << _password;
	}

	virtual void unmarshal(const sox::Unpack &up)
	{
		up >> _context >> _client_ip >> _service_type >> _username >> _password;
	}
};

struct LoginAcctInfoRes:public sox::Marshallable
{
	enum{uri = URI_LOGIN_ACCTINFO_RES};
	enum{AIRES_SUCCESS = 0, AIRES_AUTH_FAIL = 1, AIRES_SYSTEM_FAIL = 2, AIRES_PARAM_ERROR =3};
	std::string _context;
	std::string _acct_info;
	std::string _access_token;
	uint32_t    _rescode;
	std::string _passport;
	LoginAcctInfoRes():_rescode(AIRES_SUCCESS){}
	virtual void marshal(sox::Pack &pk)const
	{
		pk << _context << _acct_info << _access_token << _rescode << _passport;
	}

	virtual void unmarshal(const sox::Unpack &up)
	{
		up >> _context >> _acct_info >> _access_token >> _rescode >> _passport;
	}
};

struct LoadConfigReq: public sox::Marshallable
{
	enum{ uri = URI_LOADCONFIG_REQ };
	std::string  _op;
	std::string  _key;
	vector<std::string> _values;
	virtual void marshal( sox::Pack &pk )const
	{
		pk << _op << _key;
		sox::marshal_container(pk, _values);
	}

	virtual void unmarshal(const sox::Unpack &up) 
	{
		up >> _op >> _key;
		sox::unmarshal_container(up, std::back_inserter(_values) );
	}
};

struct LoadConfigRes: public sox::Marshallable
{
	enum{ uri = URI_LOADCONFIG_RES };
	enum{ RC_SUCCESS = 0, RC_KEY_UNKNOWN = 1, RC_OP_UNKNOWN = 2 };
	std::string  _key;
	uint32_t     _resCode;
	vector<std::string> _values;
	LoadConfigRes():_resCode(0){}
	virtual void marshal( sox::Pack &pk )const
	{
		pk << _key << _resCode;
		sox::marshal_container(pk, _values);
	}

	virtual void unmarshal(const sox::Unpack &up) 
	{
		up >> _key >> _resCode;
		sox::unmarshal_container(up, std::back_inserter(_values) );
	}
};

struct PGetPreRegUid: public sox::Marshallable
{
	enum{ uri = URI_GET_PREREGUID };

	std::string  _context;

	virtual void marshal( sox::Pack &pk )const
	{
		pk << _context;
	}

	virtual void unmarshal(const sox::Unpack &up) 
	{
		up >> _context;
	}
};

struct PGetPreRegUidRes: public sox::Marshallable
{
	enum{ uri = URI_GET_PREREGUID_RES };

	std::string  _context;
	uint64_t _yyuid; // fail: 0

	virtual void marshal( sox::Pack &pk )const
	{
		pk << _context << _yyuid;
	}

	virtual void unmarshal(const sox::Unpack &up) 
	{
		up >> _context >> _yyuid;
	}
};

}}   // namespace

#endif  //__LOGIN_I_H_

