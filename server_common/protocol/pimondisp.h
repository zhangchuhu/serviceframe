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
		//ԭ����imlinkd -> imonline  ����PSS_LoginNotifyIMOnline
		//�ָ�Ϊimlinkd -> imondispatcher ����PSS_ImOnlineDispatch �����ｫPSS_LoginNotifyIMOnline�����
		//imondispatcher -> imonline ��ʹ��routeS��ԭ�����͸�hash(imlinkd)���imonline
		//imonline answer PSS_LoginNotifyIMOnlineRes��imlinkd

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
				//��ԭ�����л�
				sox::PackBuffer pb;
				sox::Pack pk(pb);
				objMarshal.marshal(pk);
				
				//��ԭ����ð�
				m_uImlinkdId = uImLinkdId;
				m_uOriUri = uUri;
				m_strOriPacket.assign(pk.data(), pk.size());
			}

			uint32_t		m_uImlinkdId;		//��Դimlinkd
			uint32_t		m_uOriUri;			//ԭ��uri
			std::string		m_strOriPacket;		//ԭ������

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
