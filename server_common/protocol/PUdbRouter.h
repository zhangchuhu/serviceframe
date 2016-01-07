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

	std::string context;    // ��������Ϣ�������ظ���Ϣ���ֶ���������Ϣһ��
	std::string extra_info; // ��������Ϣ���ã�Ŀǰ���ֶ�����Я���ͻ��˵�IP��ַ; �ظ���Ϣ���ֶο��ÿ�
	std::string load;       // ԭʼYY��Ϣ��������ͷ�����

	virtual void marshal(sox::Pack &p) const {					
		p << context << extra_info << load;
	}

	virtual void unmarshal(const sox::Unpack &p){			
		p >> context >> extra_info >> load;
	}
};

// AP��dispatcher��UDB ͸��Э������
struct PSS_APUDBLoginAuth2 : public sox::Marshallable
{
	enum {uri = (3046 << 8) | protocol::LINKD_SVID};

	std::string context;
	uint32_t userIp;
	uint64_t serverId; // ap daemon serverid(������־ͳ��)
	uint32_t ruri;
	std::string payLoad; // ��header
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


// UDB��dispatcher��AP ͸��Э������
struct PSS_APUDBLoginAuth2Res : public sox::Marshallable
{
	enum {uri = (3047 << 8) | protocol::LINKD_SVID};

	std::string context;
	uint32_t rescode; // 0�ɹ�(��̨�з��أ�����֤ʧ��)��1ʧ��(��̨�޷��أ���ʱ)
	uint64_t serverId; // logind daemon serverid(������־ͳ��)
	uint32_t ruri;
	std::string payLoad; // ��header

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

        