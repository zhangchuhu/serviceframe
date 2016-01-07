#ifndef __server_protocol_pimid_h__
#define __server_protocol_pimid_h__


#include "packet.h"
#include "core/base_svid.h"

namespace server
{
	namespace imid
	{
		//登陆时取imid
		struct PSS_GetImidReq : public sox::Marshallable
		{
			enum {uri = (3 << 8) | protocol::IMIDD_SVID};
			PSS_GetImidReq(){}
			
			
			PSS_GetImidReq(uint32_t uUid, uint32_t uLinkdId)
			{
				m_uId=uUid;
				m_uImLinkId=uLinkdId;
			}

			uint32_t m_uImLinkId;
			uint32_t m_uId;

			
			
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uId << m_uImLinkId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uId >> m_uImLinkId;
			}

			virtual ~PSS_GetImidReq()
			{

			}
		};

		//登陆时取imid的响应
		struct PSS_GetImidRes : public sox::Marshallable
		{
			enum {uri = (3 << 8) | protocol::IMIDD_SVID};
			PSS_GetImidRes(){}

			uint32_t m_uImLinkId;
			uint32_t m_uId;
			uint32_t m_uImid;
			uint32_t m_uRes;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uId << m_uImLinkId << m_uImid << m_uRes;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uId >> m_uImLinkId >> m_uImid >> m_uRes;
			}

			virtual ~PSS_GetImidRes()
			{

			}
		};

		struct PSS_GetImidV2Req : public sox::Marshallable
		{
			enum {uri = (4 << 8) | protocol::IMIDD_SVID};
			PSS_GetImidV2Req(){}
			PSS_GetImidV2Req(uint32_t uUid, uint32_t uLinkdId, const uint64_t cid )
			{
				m_uId = uUid;
				m_uImLinkId = uLinkdId;
				m_cid = cid;
			}
			uint32_t m_uImLinkId;
			uint32_t m_uId;
			uint64_t m_cid;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uId << m_uImLinkId << m_cid;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uId >> m_uImLinkId >> m_cid;
			}
			virtual ~PSS_GetImidV2Req()
			{
			}
		};

		struct PSS_GetImidV2Res : public sox::Marshallable
		{
			enum {uri = (5 << 8) | protocol::IMIDD_SVID};
			PSS_GetImidV2Res(){}
			uint32_t m_uImLinkId;
			uint32_t m_uId;
			uint32_t m_uImid;
			uint64_t m_cid;
			uint32_t m_uRes;

			virtual void marshal(sox::Pack &p) const{
				p << m_uId << m_uImLinkId << m_uImid << m_cid << m_uRes;
			}            
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uId >> m_uImLinkId >> m_uImid >> m_cid >> m_uRes;
			}
			virtual ~PSS_GetImidV2Res()
			{
			}
		};

		struct PSS_BindImidWithPassport : public sox::Marshallable
		{
			enum {uri = (6 << 8) | protocol::IMIDD_SVID};
			PSS_BindImidWithPassport():m_uid(0), m_contextId(0){}

			uint32_t m_uid;
			std::string m_passport;
			uint32_t m_contextId;

                        virtual void marshal(sox::Pack &p) const{
                                p << m_uid << m_passport << m_contextId;
                        }

                        virtual void unmarshal(const sox::Unpack &p)
                        {
                                p >> m_uid >> m_passport >> m_contextId;
                        }

		};
	}
}

#endif

