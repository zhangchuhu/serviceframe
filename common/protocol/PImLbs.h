#ifndef P_IM_LBS_H_INCLUDE
#define P_IM_LBS_H_INCLUDE
#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/const.h"

namespace protocol
{
	namespace imlbs
	{
		struct PCS_ImLbsPing : public sox::Marshallable
		{
			enum { uri = (101 << 8) | protocol::IMLBS_SVID};

			PCS_ImLbsPing() { m_timeFlag = 0; }
			PCS_ImLbsPing(uint32_t uid, uint32_t t) : m_uUid(uid), m_timeFlag(t) {}

			uint32_t m_uUid;
			uint32_t m_timeFlag;

			virtual void marshal(sox::Pack &pk) const 
			{
				pk << m_timeFlag << m_uUid;
			}
			virtual void unmarshal(const sox::Unpack &up) 
			{
				up >> m_timeFlag >> m_uUid;
			}
		};

		struct PCS_ImLbsPingRes : public sox::Marshallable
		{
			enum { uri = (102 << 8) | protocol::IMLBS_SVID};

			uint32_t     m_timeFlag;
			uint32_t     m_uIp;
			uint32_t     m_uGroupId;        // 该imlbs所属机房的groupId
			std::vector<uint16_t> m_vecPorts;

			virtual void marshal(sox::Pack &pk) const 
			{
				sox::marshal_container(pk, m_vecPorts);
				pk << m_timeFlag << m_uIp << m_uGroupId;
			}
			virtual void unmarshal(const sox::Unpack &up) 
			{
				sox::unmarshal_container(up, std::back_inserter(m_vecPorts));
				up >> m_timeFlag >> m_uIp >> m_uGroupId;
			}
		};

		struct PSS_ImLinkdUpdate : public sox::Marshallable
		{
			enum { uri = (103 << 8) | protocol::IMLBS_SVID};
			
			PSS_ImLinkdUpdate() {}
			PSS_ImLinkdUpdate(uint32_t onlineNum, uint32_t srvId) : m_onlineNum(onlineNum), m_srvId(srvId) {}

			uint32_t m_onlineNum;
			uint32_t m_srvId;
			virtual void marshal(sox::Pack &pk) const 
			{
				pk << m_onlineNum << m_srvId;
			}
			virtual void unmarshal(const sox::Unpack &up) 
			{
				up >> m_onlineNum >> m_srvId;
			}
		};

		struct PSS_ImLinkdPorts: public sox::Marshallable
		{
			enum {uri = 104 << 8 | protocol::IMLBS_SVID};
			
			uint32_t  m_uLinkdId;
			uint32_t  m_uOnline;
			uint32_t  m_uIp;
			std::vector<uint16_t> m_vecPorts;
			
			virtual void marshal(sox::Pack &pk) const 
			{
				pk << m_uLinkdId << m_uOnline << m_uIp;
				sox::marshal_container(pk, m_vecPorts);
			}
			virtual void unmarshal(const sox::Unpack &up) 
			{
				up >> m_uLinkdId >> m_uOnline >> m_uIp;
				sox::unmarshal_container(up, std::back_inserter(m_vecPorts));
			}
		};

		// 
		struct PSS_NotifyImlinkdReSendPorts : public sox::Voidmable
		{
			enum { uri = (105 << 8) | protocol::IMLBS_SVID};
		};

		struct PCS_GetP2PBridge: public sox::Marshallable
		{
			enum {uri = (200 << 8) | protocol::IMLBS_SVID};
			std::string m_strGUID;
			uint32_t m_uExceptSid;
			bool m_IsPreconnAction;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strGUID << m_uExceptSid << m_IsPreconnAction;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_strGUID;

				if(p.size() >= 4)
				{
					p >> m_uExceptSid;
					if(p.size() > 0)
					{
						p >> m_IsPreconnAction;
					}
					else
					{
						m_IsPreconnAction = false;
					}
				}
				else
				{
					m_uExceptSid = 0;
					m_IsPreconnAction = false;
				}
			}
		};

		struct PCS_GetP2PBridgeRes: public sox::Marshallable
		{
			enum {uri = (201 << 8) | protocol::IMLBS_SVID};
			std::string m_strGUID;
			std::string m_strDIP;
			std::string m_strWIP;
			std::vector<uint16_t> m_vecPort;
			protocol::p2p::E_P2P_METHOD m_eBridgeMethod;//indicate which method p2pbridge support.
			uint32_t m_uBridgeSid;
			bool m_IsPreconnAction;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strGUID << m_strDIP << m_strWIP;
				sox::marshal_container(p, m_vecPort);
				p << (uint32_t)m_eBridgeMethod << m_uBridgeSid << m_IsPreconnAction;
			}


			virtual void unmarshal(const sox::Unpack &p)
			{
				uint32_t uMethod;
				
				p >> m_strGUID >> m_strDIP >> m_strWIP;
				sox::unmarshal_container(p, std::back_inserter(m_vecPort));

				if(p.size() >= 8)
				{
					p >> uMethod >> m_uBridgeSid;
					m_eBridgeMethod = (protocol::p2p::E_P2P_METHOD)uMethod;

					if(p.size()> 0)
					{
						p >> m_IsPreconnAction;
					}
					else
					{
						m_IsPreconnAction = false;
					}
				}
				else
				{
					m_eBridgeMethod = protocol::p2p::E_P2P_TCP;
					m_uBridgeSid = 0;
					m_IsPreconnAction = false;
				}
			}
		};

		struct PSS_BridgeRegister: public sox::Marshallable
		{
			enum {uri = (202 << 8) | protocol::IMLBS_SVID};
			std::string m_strCtlIP;
			std::string m_strCncIP;
			std::vector<uint16_t> m_vecPort;
			protocol::p2p::E_P2P_METHOD m_eBridgeMethod;//indicate which method p2pbridge support.
			uint32_t m_uMySid;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strCtlIP << m_strCncIP;
				sox::marshal_container(p, m_vecPort);
				p << (uint32_t)m_eBridgeMethod << m_uMySid;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				uint32_t uMethod;
				
				p >> m_strCtlIP >> m_strCncIP;
				sox::unmarshal_container(p, std::back_inserter(m_vecPort));

				if(p.size() >= 8)
				{
					p >> uMethod >> m_uMySid;
					m_eBridgeMethod = (protocol::p2p::E_P2P_METHOD)uMethod;
				}
				else
				{
					m_eBridgeMethod = protocol::p2p::E_P2P_TCP;
					m_uMySid = 0;
				}
			}
		};

		// v2 client require imlinkd info

		enum EImLbsReqType
		{
			ENUM_IMLS_UNKNOW = 0,
			ENUM_IMLBS_TCP_CTL,
			ENUM_IMLBS_TCP_CNC,
			ENUM_IMLBS_UDP
		};

		struct PCS_GetImLinkInfo : sox::Marshallable
		{
			enum {uri = (111 << 8) | protocol::IMLBS_SVID};

			PCS_GetImLinkInfo() : m_uUid(INVALID_UID), m_uRetry(0) {}
			PCS_GetImLinkInfo(uint32_t uUid) 
				: m_uUid(uUid), m_emReqType(ENUM_IMLS_UNKNOW), m_uRetry(0)
			{

			}

			uint32_t      m_uUid;
			EImLbsReqType m_emReqType;
			uint32_t      m_uRetry;


			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid;
				p.push_uint16(m_emReqType);
				p << m_uRetry;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid;
				try
				{
					m_emReqType = ENUM_IMLS_UNKNOW;
					if (p.size() > 0)
					{
						m_emReqType = (EImLbsReqType)p.pop_uint16();
					}
				}
				catch (...)
				{
					m_emReqType = ENUM_IMLS_UNKNOW;
				}

				try
				{
					m_uRetry = 0;
					if (p.size() > 0)
					{
						m_uRetry = p.pop_uint32();
			}
				}
				catch (...)
				{
					m_uRetry = 0;
				}
			}
		};

		// res

		struct CImLinkdInfo : sox::Marshallable
		{
			uint32_t              m_uGrpId;          // 机房编号
			std::string           m_strIp;
			std::vector<uint16_t> m_vecPort;

			CImLinkdInfo() : m_uGrpId(NONE_SERVER_ID) {}
			CImLinkdInfo(uint32_t uGrpId, const std::string& strIp, const std::vector<uint16_t>& vecPort)
				: m_uGrpId(uGrpId), m_strIp(strIp), m_vecPort(vecPort)
			{
			}


			virtual void marshal(sox::Pack &p) const
			{
				p << m_strIp << m_uGrpId;
				sox::marshal_container(p, m_vecPort);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_strIp >> m_uGrpId;
				sox::unmarshal_container(p, std::back_inserter(m_vecPort));
			}
		};

		struct PCS_GetImLinkInfoRes : sox::Marshallable
		{
			enum {uri = (112 << 8) | protocol::IMLBS_SVID};

			uint32_t                  m_uAreaId;        // 区域编号
			std::vector<CImLinkdInfo> m_vecImLinkdInfo;

			EImLbsReqType             m_emReqType;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uAreaId;
				sox::marshal_container(p, m_vecImLinkdInfo);
				p.push_uint16(m_emReqType);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uAreaId;
				sox::unmarshal_container(p, std::back_inserter(m_vecImLinkdInfo));

				try
				{
					m_emReqType = ENUM_IMLS_UNKNOW;
					if (p.size() > 0)
					{
						m_emReqType = (EImLbsReqType)p.pop_uint16();
					}
				}
				catch (...)
				{
					m_emReqType = ENUM_IMLS_UNKNOW;
				}
			}
		};

		struct PSS_P2PLinkdRegister: public sox::Marshallable
		{
			enum {uri = (203 << 8) | protocol::IMLBS_SVID};
			std::string 		   m_strCtlIP;
			std::string 		   m_strCncIP;
			std::vector<uint16_t>  m_vecPort;
			ISPType 			   m_eMyISP;
			protocol::p2p::E_P2P_METHOD m_eP2PLinkdMethod;//indicate which method p2prouter support.
			uint32_t			   m_uMyGid;
			uint32_t			   m_uMySid;
		
			PSS_P2PLinkdRegister(void) :
				m_strCtlIP(""),
				m_strCncIP(""),
				m_eMyISP(AUTO_DETECT),
				m_eP2PLinkdMethod(protocol::p2p::E_P2P_TCP),
				m_uMyGid(0),
				m_uMySid(0)
			{}
			
			PSS_P2PLinkdRegister(const uint32_t uMySid) :
				m_strCtlIP(""),
				m_strCncIP(""),
				m_eMyISP(AUTO_DETECT),
				m_eP2PLinkdMethod(protocol::p2p::E_P2P_TCP),
				m_uMyGid(0),
				m_uMySid(uMySid)
			{}
			
			//Define the Key function.
			bool operator < (const PSS_P2PLinkdRegister &objP2PLinkd) const
			{
				return (m_uMySid < objP2PLinkd.m_uMySid);
			}
		
			virtual void marshal(sox::Pack &p) const
			{
				p << m_strCtlIP << m_strCncIP;
				sox::marshal_container(p, m_vecPort);
				p << (uint32_t)m_eMyISP << (uint32_t)m_eP2PLinkdMethod << m_uMyGid << m_uMySid;
			}
		
			virtual void unmarshal(const sox::Unpack &p)
			{
				uint32_t uMyISP;
				uint32_t uMethod;
				
				p >> m_strCtlIP >> m_strCncIP;
				sox::unmarshal_container(p, std::back_inserter(m_vecPort));
				p >> uMyISP >> uMethod >> m_uMyGid >> m_uMySid;
		
				m_eMyISP = (ISPType)uMyISP;
				m_eP2PLinkdMethod = (protocol::p2p::E_P2P_METHOD)uMethod;
			}
		};	
		
		struct PCS_GetP2PLinkd: public sox::Marshallable
		{
			enum {uri = (204 << 8) | protocol::IMLBS_SVID};
			std::string m_strGuid;
			uint32_t m_uMyPublicIp;
			uint32_t m_uExceptGid;
			bool m_IsPreconnAction;
		
			virtual void marshal(sox::Pack &p) const
			{
				p << m_strGuid << m_uMyPublicIp << m_uExceptGid << m_IsPreconnAction;
			}
		
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_strGuid >> m_uMyPublicIp >> m_uExceptGid;
				if(p.size() > 0)
				{
					p >> m_IsPreconnAction;
				}
				else
				{
					m_IsPreconnAction = false;
				}
			}
		};
		
		struct PCS_GetP2PLinkdRes: public PSS_P2PLinkdRegister
		{
			enum {uri = (205 << 8) | protocol::IMLBS_SVID};
			std::string m_strGuid;
			bool m_IsPreconnAction;
		
			PCS_GetP2PLinkdRes(void) 
			{}
		
			PCS_GetP2PLinkdRes(const PSS_P2PLinkdRegister objP2PLinkdReg) : PSS_P2PLinkdRegister(objP2PLinkdReg)
			{}
			
			virtual void marshal(sox::Pack &p) const
			{
				p << m_strGuid;
				PSS_P2PLinkdRegister::marshal(p);
				p << m_IsPreconnAction;
			}
		
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_strGuid;
				PSS_P2PLinkdRegister::unmarshal(p);
				if(p.size() > 0)
				{
					p >> m_IsPreconnAction;
				}
				else
				{
					m_IsPreconnAction = false;
				}
			}
		};


		struct PSS_DataLinkdRegister : public sox::Marshallable
		{
			enum {uri = (210 << 8) | protocol::IMLBS_SVID};

			uint32_t				m_uCtlIP;
			uint32_t				m_uCncIP;
			std::vector<uint16_t>	m_vecPort;
			ISPType 				m_eMyISP;
			uint32_t				m_uMyGid;
			uint32_t				m_uMySid;			

			PSS_DataLinkdRegister(void) :
			m_uCtlIP(0),
				m_uCncIP(0),
				m_eMyISP(AUTO_DETECT),
				m_uMyGid(0),
				m_uMySid(0)
			{}

			PSS_DataLinkdRegister(const uint32_t uMySid) :
			m_uCtlIP(0),
				m_uCncIP(0),
				m_eMyISP(AUTO_DETECT),
				m_uMyGid(0),
				m_uMySid(uMySid)
			{}

			//Define the Key function.
			bool operator < (const PSS_DataLinkdRegister &objDataLinkd) const
			{
				return (m_uMySid < objDataLinkd.m_uMySid);
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uCtlIP << m_uCncIP;
				sox::marshal_container(p, m_vecPort);
				p << (uint32_t)m_eMyISP << m_uMyGid << m_uMySid;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				uint32_t uMyISP;

				p >> m_uCtlIP >> m_uCncIP;
				sox::unmarshal_container(p, std::back_inserter(m_vecPort));
				p >> uMyISP >> m_uMyGid >> m_uMySid;

				m_eMyISP = (ISPType)uMyISP;
			}
		};	

		struct PCS_GetDataLinkd : public sox::Marshallable
		{
			enum {uri = (211 << 8) | protocol::IMLBS_SVID};

			//uint32_t m_uMyPublicIp;
			//uint32_t m_uExceptDatalinkdId;
			bool m_bMaster;
			PCS_GetDataLinkd() : m_bMaster(true)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p.push_uint8(m_bMaster);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				m_bMaster = (bool)p.pop_uint8();
			}
		};

		struct PCS_GetDataLinkdRes : public sox::Marshallable
		{
			enum {uri = (212 << 8) | protocol::IMLBS_SVID};

			std::vector<PSS_DataLinkdRegister> m_vecDataLinkd;
			bool m_bMaster;

			PCS_GetDataLinkdRes() : m_bMaster(true)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_vecDataLinkd);
				p.push_uint8(m_bMaster);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{				
				sox::unmarshal_container(p, std::back_inserter(m_vecDataLinkd));
				m_bMaster = (bool)p.pop_uint8();
			}
		};

	}
}

#endif
