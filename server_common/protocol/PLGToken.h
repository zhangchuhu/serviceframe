/*
 * PLGAcctSafety.h
 *
 *  Created on: Sep 19, 2011
 *      Author: jeffcai
 */

#ifndef PLGTOKEN_H_
#define PLGTOKEN_H_

#include "common/packet.h"
#include "common/core/base_svid.h"
#include "login_i.h"
#include "common/protocol/const.h"
#include "server_login/common/resource/login_uri.h"
#include <string>

namespace server {
namespace login {

struct PLGHwTokenAuthReq: public sox::Marshallable {
	enum {
		uri = (128 << 8 | protocol::LGNSV_SVID)
	};

	std::string passport;
	uint32_t yyuid;
	std::string dynaPassword;
	uint32_t contextId;

	virtual void marshal(sox::Pack &p) const {
		p << passport << yyuid << dynaPassword << contextId;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> passport >> yyuid >> dynaPassword >> contextId;
	}

};

struct PLGHwTokenAuthRes: public sox::Marshallable {
	enum {
		uri = (129 << 8 | protocol::LGNSV_SVID)
	};

	uint16_t res_code;
	uint32_t contextId;

	virtual void marshal(sox::Pack &p) const {
		p << res_code << contextId;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> res_code >> contextId;
	}

};

struct PLGPhnTokenAuthReq: public sox::Marshallable {
	enum {
		uri = (63 << 8 | protocol::LGNSV_SVID)
	};

	std::string passport;
	uint32_t yyuid;
	std::string dynaPassword;
	uint32_t contextId;

	virtual void marshal(sox::Pack &p) const {
		p << passport << yyuid << dynaPassword << contextId;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> passport >> yyuid >> dynaPassword >> contextId;
	}

};

struct PLGPhnTokenAuthRes: public sox::Marshallable {
	enum {
		uri = (64 << 8 | protocol::LGNSV_SVID)
	};

	uint16_t res_code;
	uint32_t contextId;

	virtual void marshal(sox::Pack &p) const {
		p << res_code << contextId;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> res_code >> contextId;
	}

};


struct PLGHwTokenVerifyReq: public sox::Marshallable {
	enum {
		uri = (130 << 8 | protocol::LGNSV_SVID)
	};

	std::string passport;
	uint32_t yyuid;
	std::string dynaPassword;
	std::string contextId;

	virtual void marshal(sox::Pack &p) const {
		p << passport << yyuid << dynaPassword << contextId;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> passport >> yyuid >> dynaPassword >> contextId;
	}

};

struct PLGHwTokenVerifyRes: public sox::Marshallable {
	enum {
		uri = (131 << 8 | protocol::LGNSV_SVID)
	};

	uint16_t res_code;
	std::string contextId;

	virtual void marshal(sox::Pack &p) const {
		p << res_code << contextId;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> res_code >> contextId;
	}

};

struct PLGPMobTokenVerifyReq: public sox::Marshallable {
	enum {
		uri = (132 << 8 | protocol::LGNSV_SVID)
	};

	std::string passport;
	uint32_t yyuid;
	std::string dynaPassword;
	std::string contextId;

	virtual void marshal(sox::Pack &p) const {
		p << passport << yyuid << dynaPassword << contextId;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> passport >> yyuid >> dynaPassword >> contextId;
	}

};

struct PLGPMobTokenVerifyRes: public sox::Marshallable {
	enum {
		uri = (133 << 8 | protocol::LGNSV_SVID)
	};

	uint16_t res_code;
	std::string contextId;

	virtual void marshal(sox::Pack &p) const {
		p << res_code << contextId;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> res_code >> contextId;
	}
};

struct VerifyMobTokenReq:public sox::Marshallable
{
	enum{uri=URI_VERIY_MOBTOKEN_REQ};
	std::string _context;
	std::string _app_inst_id; // 手机安全中心客户端实例id，也是令牌序列号
	uint64_t    _yyuid;       // 如果为0，输入账号以user为准
	std::string _appid;       // 业务标识
	std::string _user;        // 通行证、手机号、email、YY号
	std::string _token;       // 令牌口令
	VerifyMobTokenReq():_yyuid(0){}
	virtual void marshal(sox::Pack &p) const
	{
		p << _context << _app_inst_id << _yyuid << _appid << _user << _token;
	}

	virtual void unmarshal(const sox::Unpack &u)
	{
		u >> _context >> _app_inst_id >> _yyuid >> _appid >> _user >> _token;
	}
};

struct VerifyMobTokenRes:public sox::Marshallable
{
	enum{uri=URI_VERIY_MOBTOKEN_RES};
	enum
	{
		VMTRES_SUCCESS = 0,           // 成功
		VMTRES_TOKEN_ERR = 1,         // 用户不存在/令牌错误/未绑定令牌
		VMTRES_INVALID_AIID = 2,      // 无效的令牌客户端（请求中的令牌客户端实例id和绑定的不一致）
		VMTRES_SYSTEM_ERR = 99,       // 系统错误，需要客户重试
		VMTRES_INVALID_PARAM = 100    // 参数无效
	};
	std::string _context;
	uint32_t    _rescode;
	std::string _uname;
	uint64_t    _yyuid;
	VerifyMobTokenRes():_rescode(0),_yyuid(0){}
	virtual void marshal(sox::Pack &p) const
	{
		p << _context << _rescode << _uname << _yyuid;
	}

	virtual void unmarshal(const sox::Unpack &u)
	{
		u >> _context >> _rescode >> _uname >> _yyuid;
	}
};

}} //namespace server {namespace login {

#endif /* PLGTOKEN_H_ */
