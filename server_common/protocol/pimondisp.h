#ifndef SERVER_PIMONDISP_H_
#define SERVER_PIMONDISP_H_

#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"

#include <iostream>
#include <string>

namespace server
{
	namespace im
	{
		//原来是imlinkd -> imonline  发送PSS_LoginNotifyIMOnline
		//现改为imlinkd -> imondispatcher 发送PSS_ImOnlineDispatch （这里将PSS_LoginNotifyIMOnline打包）
		//imondispatcher -> imonline 再使用routeS将原包发送给hash(imlinkd)后的imonline
		//imonline answer PSS_LoginNotifyIMOnlineRes给imlinkd

		struct PSS_ImOnline2Dispatch : public sox::Marshallable
		{
			enum {uri = (1 << 8 | protocol::IMONLINE2_SVID)};

			PSS_ImOnline2Dispatch() {};
			PSS_ImOnline2Dispatch(uint32_t uOriUri, const std::string &strOriPacket) 
			{
				m_uOriUri = uOriUri;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}

			void LoadPacket(uint32_t uImLinkdId, uint32_t uUri, const sox::Marshallable &objMarshal)
			{
				//将原包序列化
				sox::PackBuffer pb;
				sox::Pack pk(pb);
				objMarshal.marshal(pk);
				
				//将原包打好包
				m_uImlinkdId = uImLinkdId;
				m_uOriUri = uUri;
				m_strOriPacket.assign(pk.data(), pk.size());
			}

			uint32_t		m_uImlinkdId;		//来源imlinkd
			uint32_t		m_uOriUri;			//原包uri
			std::string		m_strOriPacket;		//原包数据

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uImlinkdId;
				p << m_uOriUri;
				p << m_strOriPacket;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uImlinkdId;
				p >> m_uOriUri;
				p >> m_strOriPacket;
			}
		};

	}
}

#endif
