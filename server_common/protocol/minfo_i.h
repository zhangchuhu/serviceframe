/********************************************************************
	created:	2011-9-23   10:41
	filename: 	minfo_i.h
	author:		Luke Chou
	copyright:  Guangzhou huaduo Co., Ltd.
	purpose:
    description:
                
---------------------------------------------------------------------
sequence      date                  author          update history
1             2011/9/23             Luke Chou       initial
*********************************************************************/

#ifndef _minfo_i_H_2011_9_23
#define _minfo_i_H_2011_9_23

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
	enum {LG_MINFO_SVID = 111};
}

enum EN_USER_TYPE{ E_USER_UNAME = 0, E_USER_EMAIL, E_USER_YYID, E_USER_YYUID };

enum E_MINFO_URI
{
	URI_MIFMODIFYPWD_REQ   = (1 << 8)|protocol::LG_MINFO_SVID,
	URI_MIFMODIFYPWD_RES   = (2 << 8)|protocol::LG_MINFO_SVID,
	URI_MIFQUERYMBL_REQ = (3 << 8)|protocol::LG_MINFO_SVID,
	URI_MIFQUERYMBL_RES = (4 << 8)|protocol::LG_MINFO_SVID,
	URI_MIFBINDMBL_REQ = (5 << 8)|protocol::LG_MINFO_SVID,
	URI_MIFBINDMBL_RES = (6 << 8)|protocol::LG_MINFO_SVID,
	URI_MIFEXCHANGEKEY_RES = (7 << 8)|protocol::LG_MINFO_SVID,
	URI_MIFQRYUNAME_REQ = (8 << 8)|protocol::LG_MINFO_SVID,
	URI_MIFQRYUNAME_RES = (9 << 8)|protocol::LG_MINFO_SVID
};

//ExChangeKey res
struct ExChangeKey_MInfoRes:public sox::Marshallable
{
	enum{uri=URI_MIFEXCHANGEKEY_RES};
	std::string _encSessionKey;

	virtual void marshal(sox::Pack &pk) const {
		pk << _encSessionKey;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> _encSessionKey;
	}
};

//For mobile error code
enum EN_MOBILE
{
	// 成功                   用户类型错误                      验证码错误
	EM_OK = 0,              EM_USER_TYPE_ERR,              EM_VERIFY_FAIL,
	// IP限制                 用户无效                          服务失败
	EM_IP_RESTRICT,         EM_INVAL_USER,                 EM_SERVICE_FAIL,
	// 超出手机绑定数         未知错误
	EM_MOBILE_EXCEED,	    EM_UNKNOWN_ERR
};

//modify password
struct ModifyPasswdReq:public sox::Marshallable
{
	enum {uri=URI_MIFMODIFYPWD_REQ};
	std::string  _from;          // yycli, mobile
	std::string  _context;
	uint8_t      _user_type;     //0 - yyid, 1 - username, 2 - email 
	std::string  _user;
	std::string  _new_passwd;
	uint64_t     _ip;
	std::string  _shaString;
	uint32_t     _server_id;
	ModifyPasswdReq():_user_type(0), _ip(0), _server_id(0){}

	ModifyPasswdReq( const std::string& from, const std::string &context, const uint8_t user_type, const std::string& user,
		const std::string& new_passwd, const uint64_t ip, const std::string &shaString, const uint32_t server_id )
		:_from(from), _context(context), _user_type(user_type), _user(user)
		,_new_passwd(new_passwd), _ip(ip), _shaString(shaString), _server_id(server_id)
	{
	}

	virtual void marshal(sox::Pack &pk) const
	{
		pk << _from << _context << _user_type << _user << _new_passwd << _ip << _shaString;
		if( 0 != _server_id )
		{
			pk << _server_id;
		}
	}

	virtual void unmarshal(const sox::Unpack &up)
	{
		up >> _from >> _context >> _user_type >> _user >> _new_passwd >> _ip >> _shaString;
		if( !up.empty() )
		{
			up >> _server_id;
		}
	}
};

struct ModifyPasswdRes:public sox::Marshallable
{
	enum {uri=URI_MIFMODIFYPWD_RES};
	std::string  _context;
	uint8_t      _user_type;     //0 - yyid, 1 - username, 2 - email 
	std::string  _user;
	uint8_t      _err_code;
	ModifyPasswdRes():_user_type(0), _err_code(0)
	{
	}

	ModifyPasswdRes( const std::string &context, const uint8_t user_type, 
		const std::string& user, const uint8_t err_code, const uint8_t safe_code )
		:_context(context), _user_type(user_type), _user(user), _err_code(err_code)
	{
	}

	virtual void marshal(sox::Pack &pk) const
	{
		pk << _context << _user_type << _user << _err_code;
	}

	virtual void unmarshal(const sox::Unpack &up)
	{
		up >> _context >> _user_type >> _user >> _err_code;
	}
};

// query mobile
struct QueryMobileReq:public sox::Marshallable
{
	enum {uri=URI_MIFQUERYMBL_REQ};
	std::string  _from;          // yycli, mobile
	std::string  _context;
	uint8_t      _user_type;     //0 - yyid, 1 - username, 2 - email 
	std::string  _user;
	uint64_t     _ip;
	std::string  _shaString;
	QueryMobileReq():_user_type(0),_ip(0){}

	QueryMobileReq( const std::string& from, const std::string& context, const uint8_t user_type,
		const std::string& user, const uint64_t ip, const std::string& shaString )
		:_from(from), _context(context), _user_type(user_type), _user(user)
		,_ip(ip), _shaString(shaString)
	{
	}

	virtual void marshal(sox::Pack &pk) const
	{
		pk << _from << _context << _user_type << _user << _ip << _shaString;
	}

	virtual void unmarshal(const sox::Unpack &up)
	{
		up >> _from >> _context >> _user_type >> _user >> _ip >> _shaString;
	}
};

struct QueryMobileRes:public sox::Marshallable
{
	enum{uri=URI_MIFQUERYMBL_RES};
	std::string  _context;
	uint8_t      _user_type;     //0 - yyid, 1 - username, 2 - email 
	std::string  _user;
	std::string  _mobile;
	uint8_t      _err_code;
	QueryMobileRes():_user_type(0), _err_code(0){}

	QueryMobileRes(const std::string &context, const uint8_t user_type, const std::string &user,
		const std::string& mobile, const uint8_t err_code )
		:_context(context), _user_type(user_type), _user(user), _mobile(mobile), _err_code(err_code)
	{
	}

	virtual void marshal(sox::Pack &pk) const
	{
		pk << _context << _user_type << _user << _mobile << _err_code;
	}

	virtual void unmarshal(const sox::Unpack &up)
	{
		up >> _context >> _user_type >> _user >> _mobile >> _err_code;
	}
};

// bind mobile
struct BindMobileReq:public sox::Marshallable
{
	enum{uri=URI_MIFBINDMBL_REQ};
	std::string  _from;          // yycli, mobile
	std::string  _context;
	uint8_t      _user_type;     //0 - yyid, 1 - username, 2 - email 
	std::string  _user;
	std::string  _mobile;
	uint64_t     _ip;
	std::string  _shaString;
	BindMobileReq():_user_type(0), _ip(0){}

	BindMobileReq(const std::string &from, const std::string &context, const uint8_t user_type,
		const std::string &user, const std::string &mobile, const uint64_t ip, const std::string shaString)
		:_from(from), _context(context), _user_type(user_type), _user(user)
		,_mobile(mobile), _ip(ip), _shaString(shaString)
	{
	}

	virtual void marshal(sox::Pack &pk) const
	{
		pk << _from << _context << _user_type << _user << _mobile << _ip << _shaString;
	}

	virtual void unmarshal(const sox::Unpack &up)
	{
		up >> _from >> _context >> _user_type >> _user >> _mobile >> _ip >> _shaString;
	}
};

struct BindMobileRes:public sox::Marshallable
{
	enum{uri=URI_MIFBINDMBL_RES};
	std::string  _context;
	uint8_t      _user_type;     //0 - yyid, 1 - username, 2 - email 
	std::string  _user;
	uint8_t      _err_code;
	BindMobileRes():_user_type(0), _err_code(0){}

	BindMobileRes(const std::string &context, const uint8_t user_type, 
		const std::string &user, const uint8_t err_code)
		:_context(context), _user_type(user_type), _user(user), _err_code(err_code)
	{
	}

	virtual void marshal(sox::Pack &pk) const
	{
		pk << _context << _user_type << _user << _err_code;
	}

	virtual void unmarshal(const sox::Unpack &up)
	{
		up >> _context >> _user_type >> _user >> _err_code;
	}
};

// Query username(by email, yyid, yyuid)
struct QueryUsernameReq:public sox::Marshallable
{
	enum{uri=URI_MIFQRYUNAME_REQ};
	std::string  _context;
	std::string  _from;
	uint64_t     _ip;
	uint8_t      _user_type; //EN_USER_TYPE
	std::string  _user;
	std::string  _shaString;
	QueryUsernameReq():_ip(0), _user_type(0){}

	virtual void marshal(sox::Pack &pk) const
	{
		pk << _context << _from << _ip << _user_type << _user << _shaString;
	}

	virtual void unmarshal(const sox::Unpack &up)
	{
		up >> _context >> _from >> _ip >> _user_type >> _user >> _shaString;
	}
};

struct QueryUsernameRes:public sox::Marshallable
{
	enum{uri=URI_MIFQRYUNAME_RES};
	std::string  _context;
	uint8_t      _user_type;
	std::string  _user;
	std::string  _uname;
	uint8_t      _err_code;
	QueryUsernameRes():_user_type(0), _err_code(0){}

	virtual void marshal(sox::Pack &pk) const
	{
		pk << _context << _user_type << _user << _uname << _err_code;
	}

	virtual void unmarshal(const sox::Unpack &up)
	{
		up >> _context >> _user_type >> _user >> _uname >> _err_code;
	}
};

#endif //_minfo_i_H_2011_9_23

