#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "common/core/base_svid.h"
#include "common/core/iserver.h"
#include "common/packet.h"
#include "common/protocol/const.h"
#include "common/protocol/zip_tpl.h"
#include "common/res_code.h"
#include "common/core/ibase.h"
#include "protocol/CodeInfo.h"
#include "server_common/protocol/prouter.h"
#include "server_common/protocol/login_i.h"
#include "server_common/server-lib3/interface.h"
#include "server_login/common/protocol/PBbase.h"

#ifndef _PUdbRouter_H_2014_3_18
#define _PUdbRouter_H_2014_3_18

struct PUdbRouter: public sox::Marshallable
{
	enum {uri = (1 << 8) | 233};

	std::string context;    // 由请求消息带来，回复消息该字段与请求消息一致
	std::string extra_info; // 由请求消息设置，目前该字段用以携带客户端的IP地址; 回复消息该字段可置空
	std::string load;       // 原始YY消息包，含包头与包体

	virtual void marshal(sox::Pack &p) const {					
		p << context << extra_info << load;
	}

	virtual void unmarshal(const sox::Unpack &p){			
		p >> context >> extra_info >> load;
	}
};

// AP经dispatcher到UDB 透传协议外层包
struct PSS_APUDBLoginAuth2 : public sox::Marshallable
{
	enum {uri = (3046 << 8) | protocol::LINKD_SVID};

	std::string context;
	uint32_t userIp;
	uint64_t serverId; // ap daemon serverid(仅作日志统计)
	uint32_t ruri;
	std::string payLoad; // 无header
	uint16_t userPort;

	PSS_APUDBLoginAuth2():userIp(0),serverId(0),ruri(0){}
	virtual void marshal(sox::Pack &pk) const 
	{
		pk << context << userIp << serverId << ruri;
		pk.push_varstr32(payLoad.data(), payLoad.size());
		pk << userPort;
	}

	virtual void unmarshal(const sox::Unpack &up)
	{
		up >> context >> userIp >> serverId >> ruri;
		payLoad = up.pop_varstr32();
		if(!up.empty())
		{
			up >> userPort;
		}
	}

};


// UDB经dispatcher到AP 透传协议外层包
struct PSS_APUDBLoginAuth2Res : public sox::Marshallable
{
	enum {uri = (3047 << 8) | protocol::LINKD_SVID};

	std::string context;
	uint32_t rescode; // 0成功(后台有返回，含验证失败)、1失败(后台无返回，超时)
	uint64_t serverId; // logind daemon serverid(仅作日志统计)
	uint32_t ruri;
	std::string payLoad; // 无header

	PSS_APUDBLoginAuth2Res():rescode(0),serverId(0),ruri(0){}

	virtual void marshal(sox::Pack &pk) const 
	{
		pk << context << rescode << serverId << ruri;
		pk.push_varstr32(payLoad.data(), payLoad.size());
	}

	virtual void unmarshal(const sox::Unpack &up)
	{
		up >> context >> rescode >> serverId >> ruri;
		payLoad = up.pop_varstr32();
	}
};


#endif //_PUdbRouter_H_2014_3_18

        