#pragma once 

#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"
//#include "common/protocol/pgtopicbase.h"
#include "server_common/protocol/PGTransferSrv.h"  // For E_GMEM_UPDATE_OPER_FLAG

//Forward definition for gtransfer
namespace protocol
{
	namespace gtransfer
	{
		void Packet2String(const sox::Marshallable &objMarshal, std::string &strPacket);
	}
}

namespace server
{
	namespace gtopic
	{
		struct PSS_GTopicNewGroup:public sox::Marshallable
		{
			enum {uri = (110 << 8 | protocol::GTOPIC_SVID)};

			uint32_t m_uGrpID;
			uint32_t m_uFldrID;
			std::string m_strGrpName;

			PSS_GTopicNewGroup()
			{

			}

			PSS_GTopicNewGroup(const uint32_t& uGrpID, 
				const uint32_t& uFldrID,
				const std::string& strGrpName):
				m_uGrpID(uGrpID),
				m_uFldrID(uFldrID),
				m_strGrpName(strGrpName)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID
				<< m_uFldrID
				<< m_strGrpName;
			}
			
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID
				>> m_uFldrID
				>> m_strGrpName;
			}
		};

		struct PSS_GetGrpTopicNum : public sox::Marshallable
		{
			enum {uri = (111 << 8 | protocol::GTOPIC_SVID)};

			uint32_t m_uGrpID;
			uint32_t m_uUID;
			uint32_t m_uSvrID;
			uint32_t m_uAppURI;
			std::string m_strAppCtx;

			PSS_GetGrpTopicNum ()
			{
				m_uGrpID = 0;
				m_uUID = 0;
				m_uSvrID = 0;
				m_uAppURI = 0;
			}

			PSS_GetGrpTopicNum (uint32_t uGrpID, uint32_t uUID, uint32_t uSvrID,
				uint32_t uAppURI, const sox::Marshallable &objMarshal)
			{
				m_uGrpID = uGrpID;
				m_uUID = uUID;
				m_uSvrID = uSvrID;
				m_uAppURI = uAppURI;
				protocol::gtransfer::Packet2String(objMarshal, m_strAppCtx);
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID << m_uUID << m_uSvrID << m_uAppURI << m_strAppCtx;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID >> m_uUID >> m_uSvrID >> m_uAppURI >> m_strAppCtx;
			}

		};

		struct PSS_GetGrpTopicNumRes : public PSS_GetGrpTopicNum
		{
		
			uint32_t m_uSumNum;
			
			PSS_GetGrpTopicNumRes()
			{
				m_uSumNum = 0;
			}

			PSS_GetGrpTopicNumRes(PSS_GetGrpTopicNum& objOri):
				PSS_GetGrpTopicNum(objOri), 
				m_uSumNum(0)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				PSS_GetGrpTopicNum::marshal(p);
				p << m_uSumNum; 
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PSS_GetGrpTopicNum::unmarshal(p);
				p >> m_uSumNum; 
			}
		};

	}
}
