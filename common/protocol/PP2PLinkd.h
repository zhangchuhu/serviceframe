#pragma once
#include <time.h>
#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"
#include "PGCheck.h"

#define INVALID_P2P_CHAN_NUM		(0xffff)

namespace protocol
{
	namespace p2p
	{

		struct PCS_TcpLinkdLogin: public sox::Marshallable
		{
			enum {uri = (1 << 8) | P2PLINKD_SVID};
			uint32_t     m_uUID;
			uint32_t 	 m_uCookieVersion;
			std::string  m_strAccout;
			std::string  m_strShaPasswd;
			std::string	 m_strCookie;
			std::string  m_strGUID;
			uint32_t 	 m_uPeerUid;
			uint32_t     m_uPeerLinkdSid;
			bool 		 m_IsPreconnAction;
			uint16_t 	 m_wChanNum;

			PCS_TcpLinkdLogin() : 
				m_uUID(0),
				m_uCookieVersion(-1),
				m_strAccout(""),
				m_strShaPasswd(""),
				m_strCookie(""),
				m_strGUID(""),
				m_uPeerUid(0),
				m_uPeerLinkdSid(0),
				m_IsPreconnAction(false),
				m_wChanNum(0xffff)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUID << m_uCookieVersion << m_strAccout << m_strShaPasswd << m_strCookie << m_strGUID;
				p << m_uPeerUid << m_uPeerLinkdSid << m_IsPreconnAction << m_wChanNum;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUID >> m_uCookieVersion >> m_strAccout >> m_strShaPasswd >> m_strCookie >> m_strGUID;
				p >> m_uPeerUid >> m_uPeerLinkdSid >> m_IsPreconnAction;

				if(p.size() > 0)
				{
					p >> m_wChanNum;
				}
				else
				{
					m_wChanNum = INVALID_P2P_CHAN_NUM;
				}
			}
		};

		struct PCS_TcpLinkdLoginRes: public sox::Marshallable
		{
			enum {uri = (2 << 8) | P2PLINKD_SVID};
			std::string m_strDIP;
			std::string m_strWIP;
			std::vector<uint16_t> m_vecPort;
			std::string m_strGUID;
			bool m_IsPreconnAction;
			uint16_t m_wChanNum;

			PCS_TcpLinkdLoginRes() :
				m_strDIP(""),
				m_strWIP(""),
				m_strGUID(""),
				m_IsPreconnAction(false),
				m_wChanNum(INVALID_P2P_CHAN_NUM)
			{
				m_vecPort.clear();
			}
			
			virtual void marshal(sox::Pack &p) const
			{
				p << m_strDIP << m_strWIP << m_strGUID;
				marshal_container(p, m_vecPort);
				p << m_IsPreconnAction << m_wChanNum;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_strDIP >> m_strWIP >> m_strGUID;
				unmarshal_container(p, std::back_inserter(m_vecPort));
				p >> m_IsPreconnAction;

				if(p.size() > 0)
				{
					p >> m_wChanNum;
				}
				else
				{
					m_wChanNum = INVALID_P2P_CHAN_NUM;
				}
			}
		};

		struct P2P_TcpTransmitLinkdData: public sox::Marshallable
		{
			enum {uri = (3 << 8) | P2PLINKD_SVID};
			std::string 	m_strGuid;		 //guid
			uint32_t 		m_uMyUid;		 //my uid.
			E_P2P_MYROLE 	m_eMyRole;		 //my role.
			bool 			m_IsFromLinkd;   //indicate from linkd for protocol.
			uint32_t        m_uPacketID;	 // the packet sequeue id.
			uint32_t        m_uInnerURI;     // the inner msg uri.
			std::string     m_strInnerData;	 // the inner msg data.

			P2P_TcpTransmitLinkdData() :
				m_strGuid(""),
				m_uMyUid(0),
				m_eMyRole(E_P2P_SENDER),
				m_IsFromLinkd(false),
				m_uPacketID(0xffffffff),
				m_uInnerURI(0xffffffff),
				m_strInnerData("")
			{}
				

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strGuid << m_uMyUid << (uint32_t)m_eMyRole << m_IsFromLinkd;
				p << m_uPacketID << m_uInnerURI << m_strInnerData;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				uint32_t uMyRole;
				
				p >> m_strGuid >> m_uMyUid >> uMyRole >> m_IsFromLinkd;
				p >> m_uPacketID >> m_uInnerURI >> m_strInnerData;
				m_eMyRole = (E_P2P_MYROLE)uMyRole;
			}
		};

		struct P2P_TcpTransmitLinkdDataAck: public sox::Marshallable
		{
			enum {uri = (4 << 8) | P2PLINKD_SVID};

			std::string 	m_strGuid;		 //guid
			uint32_t 		m_uMyUid;		 //my uid.
			E_P2P_MYROLE 	m_eMyRole;		 //my role.
			bool 			m_IsFromLinkd;   //indicate from linkd for protocol.
			uint32_t	 	m_uPacketID;     // the packet sequeue id.

			P2P_TcpTransmitLinkdDataAck() :
				m_strGuid(""),
				m_uMyUid(0),
				m_eMyRole(E_P2P_SENDER),
				m_IsFromLinkd(false),
				m_uPacketID(0)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strGuid << m_uMyUid << (uint32_t)m_eMyRole << m_IsFromLinkd << m_uPacketID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				uint32_t uMyRole;
				
				p >> m_strGuid >> m_uMyUid >> uMyRole >> m_IsFromLinkd >> m_uPacketID;
				m_eMyRole = (E_P2P_MYROLE)uMyRole;
			}
		};


		/**
		 * UDP p2pLinkd transmit msg define.
		 *
		 **/
		struct P2P_UdpTransmitLinkdData: public sox::Marshallable
		{
			enum {uri = (33 << 8) | P2PLINKD_SVID};

			std::string 	m_strGuid;		 //guid
			uint32_t 		m_uMyUid;		 //my uid.
			E_P2P_MYROLE 	m_eMyRole;		 //my role.
			uint32_t 		m_uPacketID;	 // the packet sequeue id.
			uint32_t 		m_uInnerUri;	 // the inner msg uri.
			std::string 	m_strInnerData;  // the inner msg data.

			P2P_UdpTransmitLinkdData() :
				m_strGuid(""),
				m_uMyUid(0),
				m_eMyRole(E_P2P_SENDER),
				m_uPacketID(0xffffffff),
				m_uInnerUri(0xffffffff),
				m_strInnerData("")
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strGuid << m_uMyUid << (uint32_t)m_eMyRole << m_uPacketID << m_uInnerUri << m_strInnerData;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				uint32_t uMyRole;
				p >> m_strGuid >> m_uMyUid >> uMyRole >> m_uPacketID >> m_uInnerUri >> m_strInnerData;
				m_eMyRole = (E_P2P_MYROLE)uMyRole;
			}
		};

		struct P2P_UdpTransmitLinkdDataAck: public sox::Marshallable
		{
			enum {uri = (34 << 8) | P2PLINKD_SVID};

			std::string 	m_strGuid;		//guid
			uint32_t 		m_uMyUid;		// my uid.
			E_P2P_MYROLE 	m_eMyRole;		 //my role.
			uint32_t 		m_uPacketID;	// the ack packet sequeue id.

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strGuid << m_uMyUid << (uint32_t)m_eMyRole << m_uPacketID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				uint32_t uMyRole;
				p >> m_strGuid >> m_uMyUid >> uMyRole >> m_uPacketID;
				m_eMyRole = (E_P2P_MYROLE)uMyRole;
			}
		};

		/*UDP Linkd Login.*/
		struct PCS_UdpLinkdLogin: public sox::Marshallable
		{
			enum {uri = (31 << 8) | P2PLINKD_SVID};
			
			uint32_t 		m_uMyUid;
			E_P2P_MYROLE 	m_eMyRole;
			std::string 	m_strAccout;
			std::string 	m_strShaPasswd;
			std::string 	m_strCookie;
			std::string 	m_strGuid;
			std::string 	m_strTargetIp;
			uint32_t 		m_uCookieVersion;
			uint32_t 		m_uPeerUid;
			uint32_t 		m_uPeerLinkdSid;
			bool 			m_IsPreconnAction;
			uint16_t 		m_wChanNum;

			uint32_t 		m_uChanKey;

			PCS_UdpLinkdLogin() : 
				m_uMyUid(0),
				m_eMyRole(E_P2P_SENDER),
				m_strAccout(""),
				m_strShaPasswd(""),
				m_strCookie(""),
				m_strGuid(""),
				m_strTargetIp(""),
				m_uCookieVersion(-1),
				m_uPeerUid(0),
				m_uPeerLinkdSid(0),
				m_IsPreconnAction(false),
				m_wChanNum(INVALID_P2P_CHAN_NUM),
				m_uChanKey(0)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uMyUid << (uint32_t)m_eMyRole << m_strAccout;
				p << m_strShaPasswd << m_strCookie << m_strGuid << m_strTargetIp << m_uCookieVersion;
				p << m_uPeerUid << m_uPeerLinkdSid << m_IsPreconnAction << m_wChanNum << m_uChanKey;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				uint32_t uMyRole;
				p >> m_uMyUid >> uMyRole >> m_strAccout;
				p >> m_strShaPasswd >> m_strCookie >> m_strGuid >> m_strTargetIp;
				p >> m_uCookieVersion >> m_uPeerUid >> m_uPeerLinkdSid >> m_IsPreconnAction;

				if(p.size() > 0)
				{
					p >> m_wChanNum;

					if(p.size() > 0)
					{
						p >> m_uChanKey;
					}
					else
					{
						m_uChanKey = 0;
					}
				}
				else
				{
					m_wChanNum = INVALID_P2P_CHAN_NUM;
					m_uChanKey = 0;
				}
				
				m_eMyRole = (E_P2P_MYROLE)uMyRole;
			}
		};

		/*UDP Linkd Login Res.*/
		struct PCS_UdpLinkdLoginRes: public sox::Marshallable
		{
			enum {uri = (32 << 8) | P2PLINKD_SVID};
			std::string m_strGuid;
			std::string m_strDip;
			std::string m_strWip;
			std::vector<uint16_t> m_vecPort;
			std::string m_strTargetIp;
			bool m_IsPreconnAction;
			uint16_t m_wChanNum;

			uint32_t m_uChanKey;

			PCS_UdpLinkdLoginRes() :
				m_strGuid(""),
				m_strDip(""),
				m_strWip(""),
				m_strTargetIp(""),
				m_IsPreconnAction(false),
				m_wChanNum(INVALID_P2P_CHAN_NUM),
				m_uChanKey(0)
			{
				m_vecPort.clear();
			}
			
			virtual void marshal(sox::Pack &p) const
			{
				p << m_strGuid << m_strDip << m_strWip;
				marshal_container(p, m_vecPort);
				p << m_strTargetIp << m_IsPreconnAction << m_wChanNum << m_uChanKey;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_strGuid >> m_strDip >> m_strWip;
				unmarshal_container(p, back_inserter(m_vecPort));
				p >> m_strTargetIp >> m_IsPreconnAction;

				if(p.size() > 0)
				{
					p >> m_wChanNum;

					if(p.size() > 0)
					{
						p >> m_uChanKey;
					}
					else
					{
						m_uChanKey = 0;
					}
				}
				else
				{
					m_wChanNum = INVALID_P2P_CHAN_NUM;
					m_uChanKey = 0;
				}
			}
		};

		struct P2P_TcpLinkdContextCreate : public sox::Marshallable
		{
			enum {uri = (5 << 8) | P2PLINKD_SVID};
			
			std::string  		m_strGuid;
			uint32_t 			m_uSUID;
			uint32_t            m_uRUID;
			E_P2P_MYROLE 	    m_eMyRole; //my role.
			uint32_t			m_uTargetLinkdSid;
			bool 				m_IsPreconnAction;
			uint16_t 			m_wChanNum;

			P2P_TcpLinkdContextCreate(void) : 
				m_strGuid(""),
				m_uSUID(0),
				m_uRUID(0),
				m_eMyRole(E_P2P_SENDER),
				m_uTargetLinkdSid(0),
				m_IsPreconnAction(false),
				m_wChanNum(INVALID_P2P_CHAN_NUM)
			{}

			~P2P_TcpLinkdContextCreate(void){}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strGuid << m_uSUID << m_uRUID << (uint32_t)m_eMyRole << m_uTargetLinkdSid << m_IsPreconnAction << m_wChanNum;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				uint32_t uMyRole;
				
				p >> m_strGuid >> m_uSUID >> m_uRUID >> uMyRole >> m_uTargetLinkdSid >> m_IsPreconnAction;

				if(p.size() > 0)
				{
					p >> m_wChanNum;
				}
				else
				{
					m_wChanNum = INVALID_P2P_CHAN_NUM;
				}
				m_eMyRole = (E_P2P_MYROLE)uMyRole;
			}
		};

		struct P2P_TcpLinkdContextCreateRes : public P2P_TcpLinkdContextCreate
		{
			enum {uri = (6 << 8) | P2PLINKD_SVID};

            P2P_TcpLinkdContextCreateRes(void){}

            P2P_TcpLinkdContextCreateRes(const P2P_TcpLinkdContextCreate &objContext) :
                P2P_TcpLinkdContextCreate(objContext)
            {}
			
			virtual void marshal(sox::Pack &p) const
			{
				P2P_TcpLinkdContextCreate::marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				P2P_TcpLinkdContextCreate::unmarshal(p);
			}
		};

		struct P2P_UdpLinkdContextCreate : public sox::Marshallable
		{
			enum {uri = (7 << 8) | P2PLINKD_SVID};
			
			std::string  		m_strGuid;
			uint32_t 			m_uSUID;
			uint32_t            m_uRUID;
			E_P2P_MYROLE 	    m_eMyRole; //my role.
		//	uint32_t			m_uMyRouterSid;
			uint32_t			m_uTargetLinkdSid;
			bool 				m_IsPreconnAction;
			uint16_t 			m_wChanNum;

			P2P_UdpLinkdContextCreate(void) : 
				m_strGuid(""),
				m_uSUID(0),
				m_uRUID(0),
				m_eMyRole(E_P2P_SENDER),
		//		m_uMyRouterSid(0),
				m_uTargetLinkdSid(0),
				m_IsPreconnAction(false),
				m_wChanNum(INVALID_P2P_CHAN_NUM)
			{}

			~P2P_UdpLinkdContextCreate(void){}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strGuid << m_uSUID << m_uRUID << (uint32_t)m_eMyRole << m_uTargetLinkdSid << m_IsPreconnAction << m_wChanNum;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				uint32_t uMyRole;
				
				p >> m_strGuid >> m_uSUID >> m_uRUID >> uMyRole >> m_uTargetLinkdSid >> m_IsPreconnAction;

				if(p.size() > 0)
				{
					p >> m_wChanNum;
				}
				else
				{
					m_wChanNum = INVALID_P2P_CHAN_NUM;
				}
				
				m_eMyRole = (E_P2P_MYROLE)uMyRole;
			}
		};

		struct P2P_UdpLinkdContextCreateRes : public P2P_UdpLinkdContextCreate
		{
			enum {uri = (8 << 8) | P2PLINKD_SVID};

            P2P_UdpLinkdContextCreateRes(void){}

            P2P_UdpLinkdContextCreateRes(const P2P_UdpLinkdContextCreate &objContext) :
                P2P_UdpLinkdContextCreate(objContext)
            {}

            
			virtual void marshal(sox::Pack &p) const
			{
				P2P_UdpLinkdContextCreate::marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				P2P_UdpLinkdContextCreate::unmarshal(p);
			}
		};
		
		// Timing info in a single point
		struct DataLinkRouteInfo: public sox::Marshallable
		{
			std::string	m_strServerName;
			uint32_t m_uServerID; //0 for client
			uint32_t m_uServerIP;
			uint32_t m_uLocalTime;
			uint8_t m_uLevel; //0x00 for request, 0x80 for response
			
			DataLinkRouteInfo(const std::string& strSvrName, uint32_t uSvrID, uint32_t uSvrIP, bool bRequest)
				: m_strServerName(strSvrName), m_uServerID(uSvrID), m_uServerIP(uSvrIP), m_uLocalTime((uint32_t)time(NULL)), m_uLevel(bRequest?0x00:0x80)
			{
			
			}

			DataLinkRouteInfo() {}
			
			virtual void marshal(sox::Pack &p) const
			{
				p << m_strServerName << m_uServerID << m_uServerIP << m_uLocalTime << m_uLevel;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_strServerName >> m_uServerID >> m_uServerIP >> m_uLocalTime >> m_uLevel;
			}
		};
		
		// Overall route info
		struct PCS_DataLinkRouteBase : public sox::Marshallable
		{
			std::vector< DataLinkRouteInfo > m_vecRoute;
			uint32_t m_uTargetSvrId; //For DataDispatcher/DataBridge
			uint32_t m_uRespConnId; //Don't response if the connection has gone
			uint32_t m_uUid;	


			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_vecRoute);
				p << m_uTargetSvrId << m_uRespConnId << m_uUid;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				unmarshal_container(p, std::back_inserter(m_vecRoute));
				p >> m_uTargetSvrId >> m_uRespConnId >> m_uUid;
			}
		};
		
		struct PCS_TcpDataTransBase : public PCS_DataLinkRouteBase
		{
			PCS_TcpDataTransBase() {}
// 			PCS_TcpDataTransBase(uint32_t uid, uint32_t cookiever, const std::string &cookie, const std::string& passwd, const std::string& account, const uint16_t trycnt)
// 				: m_uUid(uid), m_uCookieVersion(cookiever), m_strCookie(cookie), m_strShaPasswd(passwd), m_strAccount(account),m_uTryCount(trycnt)/*, m_uFlag(0)*/
			PCS_TcpDataTransBase(uint32_t cookiever, const std::string &cookie, const uint16_t trycnt)
				: m_uCookieVersion(cookiever), m_strCookie(cookie), m_uTryCount(trycnt)/*, m_uFlag(0)*/
			{
			}

			uint32_t m_uCookieVersion;
			std::string m_strCookie;
			//std::string m_strShaPasswd;
			//std::string m_strAccount;
			uint16_t m_uTryCount;

			virtual void marshal(sox::Pack &pk) const 
			{
				PCS_DataLinkRouteBase::marshal(pk);
				//pk << m_uUid << m_uCookieVersion << m_strCookie << m_strShaPasswd << m_strAccount << m_uTryCount;
				pk << m_uCookieVersion << m_strCookie << m_uTryCount;
			}
			virtual void unmarshal(const sox::Unpack &up) 
			{
				PCS_DataLinkRouteBase::unmarshal(up);
				//up >> m_uUid >> m_uCookieVersion >> m_strCookie >> m_strShaPasswd >> m_strAccount >> m_uTryCount;
				up >> m_uCookieVersion >> m_strCookie >> m_uTryCount;
			}
		};
		
		struct PCS_TcpDataUpload : public PCS_TcpDataTransBase
		{
			enum {uri = (9 << 8) | P2PLINKD_SVID};
			
			std::string m_strUrl;
			std::string m_strFileName;
			std::string m_strData;
			std::string m_strAppData; //For UI
			std::string m_strExtInfo; //XML?

			PCS_TcpDataUpload(){}

			PCS_TcpDataUpload(const std::string& strUrl,
				const std::string& strFileName,
				const std::string& strData,
				const std::string& strAppData,
				const std::string& strExtInfo) 
				: m_strUrl(strUrl), m_strFileName(strFileName), m_strData(strData)
				, m_strAppData(strAppData), m_strExtInfo(strExtInfo)
			{}
			
			virtual void marshal(sox::Pack &p) const
			{
				PCS_TcpDataTransBase::marshal(p);
				p << m_strUrl << m_strFileName;
				p.push_varstr32(m_strData.data(), m_strData.size()); 
				p << m_strAppData << m_strExtInfo;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_TcpDataTransBase::unmarshal(p);
				p >> m_strUrl >> m_strFileName;
				m_strData = p.pop_varstr32();
				p >> m_strAppData >> m_strExtInfo;
			}
		};

		enum E_DATA_TRANS_RES
		{
			ENUM_CONTINUE = 100,
			ENUM_SUCCESS = 200,
			ENUM_DENY = 406,
			ENUM_BAD_AUTH = 401,
			ENUM_NOT_FOUND = 404,
			ENUM_SERVER_ERROR = 500,
			ENUM_NOT_CONNECT = 504,
			ENUM_BAD_URL = 403,
			ENUM_OTHER = 65535
		};

		struct PCS_TcpDataUploadRes : public PCS_DataLinkRouteBase
		{
			enum {uri = (10 << 8) | P2PLINKD_SVID};

			std::string m_strUrl;
			uint16_t m_uResCode;
			//uint32_t m_uDataSize;
			std::string m_strAppData;

			PCS_TcpDataUploadRes(const PCS_TcpDataUpload& objReq) : 
			PCS_DataLinkRouteBase(static_cast<PCS_DataLinkRouteBase>(objReq)), m_strUrl(objReq.m_strUrl)
				,m_strAppData(objReq.m_strAppData)
			{
				m_uResCode = ENUM_SUCCESS;
			}

			PCS_TcpDataUploadRes() 
			{
				m_uResCode = ENUM_SUCCESS;
			}

			virtual void marshal(sox::Pack &p) const
			{
				PCS_DataLinkRouteBase::marshal(p);
				p << m_strUrl<< m_uResCode << m_strAppData;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_DataLinkRouteBase::unmarshal(p);
				p >> m_strUrl >> m_uResCode >> m_strAppData;
			}
		};
		
		struct PCS_TcpDataDownload : public PCS_TcpDataTransBase
		{
			enum {uri = (11 << 8) | P2PLINKD_SVID};

			std::string m_strUrl;
			std::string m_strAppData; //For UI
			std::string m_strExtInfo; //XML?

			virtual void marshal(sox::Pack &p) const
			{
				PCS_TcpDataTransBase::marshal(p);
				p << m_strUrl << m_strAppData << m_strExtInfo;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_TcpDataTransBase::unmarshal(p);
				p >> m_strUrl >> m_strAppData >> m_strExtInfo;
			}		
		};
		
		struct PCS_TcpDataDownloadRes : public PCS_DataLinkRouteBase
		{
			enum {uri = (12 << 8) | P2PLINKD_SVID};

			std::string m_strUrl;
			std::string m_strData;
			uint16_t m_uResCode;
			std::string m_strAppData;
			uint16_t	m_uHitCache;

			PCS_TcpDataDownloadRes(const PCS_TcpDataDownload& objReq) : 
				PCS_DataLinkRouteBase(static_cast<PCS_DataLinkRouteBase>(objReq)), m_strUrl(objReq.m_strUrl)
				, m_strAppData(objReq.m_strAppData), m_uHitCache(0)
			{
				m_uResCode = ENUM_SUCCESS;
			}

			PCS_TcpDataDownloadRes() : m_uHitCache(0)
			{
				m_uResCode = ENUM_SUCCESS;
			}

			virtual void marshal(sox::Pack &p) const
			{
				PCS_DataLinkRouteBase::marshal(p);
				p << m_strUrl;
				p.push_varstr32(m_strData.data(), m_strData.size());
				p << m_uResCode << m_strAppData;
				p << m_uHitCache;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_DataLinkRouteBase::unmarshal(p);
				p >> m_strUrl;
				m_strData = p.pop_varstr32();
				p >> m_uResCode >> m_strAppData;

				if (p.size() > 0)
				{
					p >> m_uHitCache;
				}
			}
		};

		enum EServiceType
		{
			ENUM_SS_DOWNLOAD = 0,
			ENUM_SS_UPLOAD,
			ENUM_SS_CHECK,
			ENUM_SESSION_LOGO_UPLOAD,
			ENUM_IM_LOGO_UPLOAD,
			ENUM_HD_LOGO_UPLOAD,
			ENUM_SESSION_LOGO_DOWNLOAD,
			ENUM_IM_LOGO_DOWNLOAD,
			ENUM_HD_LOGO_DOWNLOAD,
			ENUM_PIC_UPLOAD,
			ENUM_DYNAMIC_LOGO_UPLOAD,
			ENUM_GROUP_LOGO_UPLOAD,
			ENUM_SERVICE_UNKNOWN = 0xff
		};

		enum EHDLogoType
		{
			ENUM_HD_640_640 = 0,                                                                                                                                             
			ENUM_HD_100_100,                                                                                                                                                 
			ENUM_HD_60_60,     
			ENUM_HD_144_144,
			ENUM_HD_UNKNOWN = 0xff                                                                                                                                           
		};

		enum ESeiCardType
		{
			ENUM_SCARD_BASIC = 0, 
			ENUM_SCARD_UNKNOWN = 0xff
		};

		struct PCS_DatalinkdLoginInfo : public sox::Marshallable
		{
			EServiceType m_eServiceType;
			std::string m_strGuid;
			uint32_t m_uUid;
			uint32_t m_uTryCount;
			uint32_t m_uFileSize;
			std::string m_strCookie;
			std::string m_strUrl;
			std::string m_strFileName;
			std::string m_strAppData;
			std::string m_strExtInfo;
			uint32_t	m_uSeiOrGrpId;
			uint32_t	m_uWidth;
			uint32_t	m_uHeight;

			uint32_t	m_uSign;
			uint32_t	m_uFldId;

			PCS_DatalinkdLoginInfo() {}

			PCS_DatalinkdLoginInfo(const PCS_DatalinkdLoginInfo &loginInfo)
				: m_eServiceType(loginInfo.m_eServiceType)
				, m_strGuid(loginInfo.m_strGuid)
				, m_uUid(loginInfo.m_uUid)	
				, m_uTryCount(loginInfo.m_uTryCount)
				, m_uFileSize(loginInfo.m_uFileSize)
				, m_strCookie(loginInfo.m_strCookie)
				, m_strUrl(loginInfo.m_strUrl)
				, m_strFileName(loginInfo.m_strFileName)
				, m_strAppData(loginInfo.m_strAppData)
				, m_strExtInfo(loginInfo.m_strExtInfo)
				, m_uSeiOrGrpId(loginInfo.m_uSeiOrGrpId)
				, m_uWidth(loginInfo.m_uWidth)
				, m_uHeight(loginInfo.m_uHeight)
				, m_uSign(loginInfo.m_uSign)
				, m_uFldId(loginInfo.m_uFldId)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p.push_uint8(m_eServiceType);
				p << m_strGuid << m_uUid << m_uTryCount << m_uFileSize << m_strCookie << m_strUrl 
					<< m_strFileName << m_strAppData << m_strExtInfo << m_uSeiOrGrpId << m_uWidth << m_uHeight
					<< m_uSign << m_uFldId;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				m_eServiceType = (EServiceType)p.pop_uint8();
				p >> m_strGuid >> m_uUid >> m_uTryCount >> m_uFileSize >> m_strCookie
					>> m_strUrl >> m_strFileName >> m_strAppData >> m_strExtInfo;
				if (p.size() > 0)
				{
					p >> m_uSeiOrGrpId;
					if (p.size() > 0)
					{
						p >> m_uWidth >> m_uHeight;
						if (p.size() > 0)
						{
							p >> m_uSign >> m_uFldId;
						}
					}
				}
				else
				{
					m_uSeiOrGrpId = 0;
					m_uWidth = 0;
					m_uHeight = 0;
					m_uSign = 0;
					m_uFldId = 0;
				}
			}
		};


		struct PCS_DatalinkdUdpLogin : public PCS_DatalinkdLoginInfo
		{
			enum {uri = (13 << 8) | P2PLINKD_SVID};

			PCS_DatalinkdUdpLogin() {}
			PCS_DatalinkdUdpLogin(const PCS_DatalinkdLoginInfo &loginInfo)
				: PCS_DatalinkdLoginInfo(loginInfo)
			{}			
		};

		struct PCS_DatalinkdTcpLogin : public PCS_DatalinkdLoginInfo
		{
			enum {uri = (14 << 8) | P2PLINKD_SVID};	

			PCS_DatalinkdTcpLogin() {}
			PCS_DatalinkdTcpLogin(const PCS_DatalinkdLoginInfo &loginInfo)
				: PCS_DatalinkdLoginInfo(loginInfo)
			{}
		};

		struct PCS_DatalinkdLoginRes : public sox::Marshallable
		{
			std::string	m_strGuid;
			uint32_t m_uUid;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strGuid << m_uUid;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_strGuid >> m_uUid;
			}
		};

		struct PCS_DatalinkdUdpLoginRes : public PCS_DatalinkdLoginRes
		{
			enum {uri = (15 << 8) | P2PLINKD_SVID};			
		};

		struct PCS_DatalinkdTcpLoginRes : public PCS_DatalinkdLoginRes
		{
			enum {uri = (16 << 8) | P2PLINKD_SVID};			
		};

		struct PCS_AckAckTcp : public sox::Marshallable
		{
			enum {uri = (17 << 8) | P2PLINKD_SVID};

			std::string	m_strGuid;
			uint32_t m_uUid;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strGuid << m_uUid;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_strGuid >> m_uUid;
			}
		};

		struct PCS_AckAckUdp : public sox::Marshallable
		{
			enum {uri = (18 << 8) | P2PLINKD_SVID};

			std::string	m_strGuid;
			uint32_t m_uUid;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strGuid << m_uUid;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_strGuid >> m_uUid;
			}
		};

		struct PCS_SSUdpDataBlock : public sox::Marshallable
		{
			enum {uri = (19 << 8) | P2PLINKD_SVID};

			std::string m_strData;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strData;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_strData;
			}
		};

		struct PCS_SSUdpTransmitData : public sox::Marshallable
		{
			enum {uri = (20 << 8) | P2PLINKD_SVID};

			std::string m_strGuid;
			uint32_t m_uUid;
			uint32_t m_uPacketId;
			uint32_t m_uInnerUri;
			std::string m_strInnerData;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strGuid << m_uUid << m_uPacketId << m_uInnerUri << m_strInnerData;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_strGuid >> m_uUid >> m_uPacketId >> m_uInnerUri >> m_strInnerData;
			}
		};

		struct PCS_SSUdpTransmitDataAck : public sox::Marshallable
		{
			enum {uri = (21 << 8) | P2PLINKD_SVID};

			std::string m_strGuid;
			uint32_t m_uPacketId;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strGuid << m_uPacketId;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_strGuid >> m_uPacketId;
			}
		};

		struct PCS_SSUdpTransmitFinish : public sox::Marshallable
		{
			enum {uri = (22 << 8) | P2PLINKD_SVID};

			std::string m_strGuid;
			E_DATA_TRANS_RES m_uResCode;
			std::vector<DataLinkRouteInfo>	m_vecRoute;
			uint16_t				m_uHitCache;
			std::string				m_strReserv;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strGuid;
				p.push_uint16(m_uResCode);
				marshal_container(p, m_vecRoute);
				p << m_uHitCache << m_strReserv;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_strGuid;
				m_uResCode = (E_DATA_TRANS_RES)p.pop_uint16();
				
				if (p.size() > 0)
				{
					unmarshal_container(p, std::back_inserter(m_vecRoute));
					p >> m_uHitCache >> m_strReserv;
				}				
			}
		};

		struct PCS_SSUdpTransmitFinishRes : public sox::Marshallable
		{
			enum {uri = (23 << 8) | P2PLINKD_SVID};

			std::string m_strGuid;
			E_DATA_TRANS_RES m_uResCode;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strGuid;
				p.push_uint16(m_uResCode);
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_strGuid;
				m_uResCode = (E_DATA_TRANS_RES)p.pop_uint16();
			}
		};

		struct PCS_ImageAliveCheck : public PCS_TcpDataTransBase
		{
			enum {uri = (24 << 8) | P2PLINKD_SVID};

			uint32_t	m_uTaskId;
			std::string m_strUrl;

			virtual void marshal(sox::Pack &p) const
			{
				PCS_TcpDataTransBase::marshal(p);
				p << m_uTaskId << m_strUrl;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_TcpDataTransBase::unmarshal(p);
				p >> m_uTaskId >> m_strUrl;
			}
		};

		struct PCS_ImageAliveCheckRes : public PCS_DataLinkRouteBase
		{
			enum {uri = (25 << 8) | P2PLINKD_SVID};

			bool		m_bAlive;
			uint32_t	m_uTaskId;
			std::string m_strUrl;

			PCS_ImageAliveCheckRes(){}
			PCS_ImageAliveCheckRes(const PCS_ImageAliveCheck &objCheck)
				: PCS_DataLinkRouteBase(static_cast<PCS_DataLinkRouteBase>(objCheck))
				, m_bAlive(false), m_uTaskId(objCheck.m_uTaskId), m_strUrl(objCheck.m_strUrl)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				PCS_DataLinkRouteBase::marshal(p);
				p << m_bAlive << m_uTaskId << m_strUrl;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_DataLinkRouteBase::unmarshal(p);
				p >> m_bAlive >> m_uTaskId >> m_strUrl;
			}
		};

		struct PCS_ImageUploadRes : public sox::Marshallable
		{
			enum {uri = (26 << 8) | P2PLINKD_SVID};

			std::string			m_strGuid;
			E_DATA_TRANS_RES	m_uResCode;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strGuid;
				p.push_uint16(m_uResCode);
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_strGuid;
				m_uResCode = (E_DATA_TRANS_RES)p.pop_uint16();
			}
		};

		struct PCS_LogoUpload : public PCS_TcpDataTransBase
		{
			enum {uri = (27 << 8) | P2PLINKD_SVID};

			EServiceType	m_enServType;
			std::string		m_strGuid;
			std::string		m_strFileName;
			std::string		m_strData;
			std::string		m_strContext;
			uint32_t		m_uSeiOrGrpId;	//session or group id
			uint32_t		m_uWidth;
			uint32_t		m_uHeight;
			uint32_t		m_uFolderId;	//群的分组id
			uint32_t		m_uSign;		//群头像要带的签名

			PCS_LogoUpload()
				: m_uSeiOrGrpId(0), m_uWidth(0), m_uHeight(0), m_uFolderId(0), m_uSign(0)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				PCS_TcpDataTransBase::marshal(p);
				p.push_uint8(m_enServType);
				p << m_strGuid << m_strFileName;
				p.push_varstr32(m_strData.data(), m_strData.size());
				p << m_strContext << m_uSeiOrGrpId << m_uWidth << m_uHeight << m_uFolderId << m_uSign;				
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_TcpDataTransBase::unmarshal(p);
				m_enServType = (EServiceType)p.pop_uint8();
				p >> m_strGuid >> m_strFileName;
				m_strData = p.pop_varstr32();
				p >> m_strContext >> m_uSeiOrGrpId;
				if (p.size() > 0)
				{
					p >> m_uWidth >> m_uHeight;
					if (p.size() > 0)
					{
						p >> m_uFolderId >> m_uSign;
					}
				}
			}			
		};

		struct LogoUploadResBase : public PCS_DataLinkRouteBase
		{
			EServiceType	m_enServType;
			std::string		m_strGuid;
			std::string		m_strUrl;
			uint16_t		m_uResCode;
			std::string		m_strContext;
			uint32_t		m_uSeiOrGrpId;	//session or group id

			virtual void marshal(sox::Pack &p) const
			{
				PCS_DataLinkRouteBase::marshal(p);
				p.push_uint8(m_enServType);
				p << m_strGuid << m_strUrl << m_uResCode << m_strContext << m_uSeiOrGrpId;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_DataLinkRouteBase::unmarshal(p);
				m_enServType = (EServiceType)p.pop_uint8();
				p >> m_strGuid >> m_strUrl >> m_uResCode >> m_strContext >> m_uSeiOrGrpId;
			}	
		};

		struct PCS_LogoUploadRes : public LogoUploadResBase
		{
			enum {uri = (28 << 8) | P2PLINKD_SVID};

			uint32_t		m_uFolderId;

			virtual void marshal(sox::Pack &p) const
			{
				LogoUploadResBase::marshal(p);
				p << m_uFolderId;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				LogoUploadResBase::unmarshal(p);

				if (p.size() > 0)
				{
					p >> m_uFolderId;
				}
			}	
		};

		struct PCS_HDLogoUpload : public PCS_LogoUpload
		{
			enum {uri = (29 << 8) | P2PLINKD_SVID};
		};

		struct PCS_HDLogoUploadRes : public LogoUploadResBase
		{
			enum {uri = (30 << 8) | P2PLINKD_SVID};

			std::map<uint16_t, std::string>	m_mapTypeUrl;
			PCS_HDLogoUploadRes() {}                                                                                                                                         
			PCS_HDLogoUploadRes(const LogoUploadResBase &objRes)                                                                                                             
				: LogoUploadResBase(objRes)                                                                                                                                    
			{}                                                                                                                                                               
			virtual void marshal(sox::Pack &p) const
			{
				LogoUploadResBase::marshal(p);
				marshal_container(p, m_mapTypeUrl);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				LogoUploadResBase::unmarshal(p);
				unmarshal_container(p, std::inserter(m_mapTypeUrl, m_mapTypeUrl.begin()));				
			}	
		};

		struct PSS_LogoSync : public PCS_LogoUpload                                                                                                                        
		{                                                                                                                                                                  
			enum {uri = (41 << 8) | P2PLINKD_SVID};                                                                                                                          
                                                                                                                                                                       
			PSS_LogoSync() {}                                                                                                                                                
			PSS_LogoSync(const PCS_HDLogoUpload &obj)                                                                                                                        
				: PCS_LogoUpload(static_cast<PCS_LogoUpload>(obj))                                                                                                             
			{}                                                                                                                                                               
		};                                                                                                                                                                 
                                                                                                                                                                       
		struct PSS_LogoSyncRes : public PCS_LogoUploadRes                                                                                                                  
		{                                                                                                                                                                  
			enum {uri = (42 << 8) | P2PLINKD_SVID};                                                                                                                          
                                                                                                                                                                       
			PSS_LogoSyncRes() {}                                                                                                                                             
			PSS_LogoSyncRes(const PCS_LogoUploadRes &objRes)                                                                                                                 
				: PCS_LogoUploadRes(objRes)                                                                                                                                    
			{}                                                                                                                                                               
		};        

		struct PCS_LogoDownload : public PCS_TcpDataTransBase
		{
			enum {uri = (43 << 8) | P2PLINKD_SVID};

			EServiceType	m_enServType;
			std::string		m_strGuid;
			std::string		m_strUrl;
			std::string		m_strContext;
			uint32_t		m_uSessionId;	//session id

			virtual void marshal(sox::Pack &p) const
			{
				PCS_TcpDataTransBase::marshal(p);
				p.push_uint8(m_enServType);
				p << m_strGuid << m_strUrl;
				p << m_strContext << m_uSessionId;				
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_TcpDataTransBase::unmarshal(p);
				m_enServType = (EServiceType)p.pop_uint8();
				p >> m_strGuid >> m_strUrl;
				p >> m_strContext >> m_uSessionId;
			}	
		};

		struct PCS_LogoDownloadRes : public PCS_DataLinkRouteBase
		{
			enum {uri = (44 << 8) | P2PLINKD_SVID};

			EServiceType	m_enServType;
			std::string		m_strGuid;
			std::string		m_strUrl;
			std::string		m_strData;
			uint16_t		m_uResCode;
			std::string		m_strContext;
			uint32_t		m_uSessionId;	//session id

			virtual void marshal(sox::Pack &p) const
			{
				PCS_DataLinkRouteBase::marshal(p);
				p.push_uint8(m_enServType);
				p << m_strGuid << m_strUrl;
				p.push_varstr32(m_strData.c_str(), m_strData.size());
				p << m_uResCode << m_strContext << m_uSessionId;				
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_DataLinkRouteBase::unmarshal(p);
				m_enServType = (EServiceType)p.pop_uint8();
				p >> m_strGuid >> m_strUrl;
				m_strData = p.pop_varstr32();
				p >> m_uResCode >> m_strContext >> m_uSessionId;
			}	
		};

		struct PCS_HDLogoDownload : public PCS_LogoDownload
		{
			enum {uri = (45 << 8) | P2PLINKD_SVID};
		};

		struct PCS_HDLogoDownloadRes : public PCS_LogoDownloadRes
		{
			enum {uri = (46 << 8) | P2PLINKD_SVID};
		};

		//上麦名片等业务额外数据
		struct PicExtInfo : public sox::Marshallable
		{
			std::string					m_strPicType;	//图片类型
			std::vector<std::string>	m_vecWantSize;	//要得到的缩放尺寸,如"100*100",无缩放也要填写"宽*高"

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strPicType;
				marshal_container(p, m_vecWantSize);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_strPicType;
				unmarshal_container(p, std::back_inserter(m_vecWantSize));
			}
		};

		struct PCS_PicUpload : public PCS_TcpDataTransBase
		{
			enum {uri = (47 << 8) | P2PLINKD_SVID};

			std::string		m_strPicType;	//图片类型
			std::string		m_strGuid;
			std::string		m_strFileName;
			std::string		m_strData;
			std::string		m_strContext;
			uint32_t		m_uSessionId;	//session id
			uint32_t		m_uWidth;
			uint32_t		m_uHeight;
			std::vector<std::string>	m_vecWantSize;	//要得到的缩放尺寸,如"100*100",无缩放也要填写"宽*高"

			virtual void marshal(sox::Pack &p) const
			{
				PCS_TcpDataTransBase::marshal(p);
				p << m_strPicType << m_strGuid << m_strFileName;
				p.push_varstr32(m_strData.data(), m_strData.size());
				p << m_strContext << m_uSessionId << m_uWidth << m_uHeight;	
				marshal_container(p, m_vecWantSize);
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_TcpDataTransBase::unmarshal(p);
				p >> m_strPicType >> m_strGuid >> m_strFileName;
				m_strData = p.pop_varstr32();
				p >> m_strContext >> m_uSessionId >> m_uWidth >> m_uHeight;
				unmarshal_container(p, std::back_inserter(m_vecWantSize));
			}			
		};

		struct PCS_PicUploadRes : public PCS_DataLinkRouteBase
		{
			enum {uri = (48 << 8) | P2PLINKD_SVID};

			std::string		m_strPicType;	//图片类型
			std::string		m_strGuid;
			std::string		m_strFileName;
			uint16_t		m_uResCode;
			std::string		m_strContext;
			uint32_t		m_uSessionId;	//session id
			std::map<std::string, std::string>	m_mapSizeUrl;	//缩放尺寸对应url

			virtual void marshal(sox::Pack &p) const
			{
				PCS_DataLinkRouteBase::marshal(p);
				p << m_strPicType << m_strGuid << m_strFileName << m_uResCode << m_strContext << m_uSessionId;
				marshal_container(p, m_mapSizeUrl);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_DataLinkRouteBase::unmarshal(p);
				p >> m_strPicType >> m_strGuid >> m_strFileName >> m_uResCode >> m_strContext >> m_uSessionId;
				unmarshal_container(p, std::inserter(m_mapSizeUrl, m_mapSizeUrl.begin()));
			}	
		};

		struct PCS_DatalinkdSync : public sox::Marshallable
		{
			enum {uri = (49 << 8) | P2PLINKD_SVID};

			std::string		m_strKey;
			std::string		m_strData;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strKey;
				p.push_varstr32(m_strData.data(), m_strData.size());
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_strKey;
				m_strData = p.pop_varstr32();
			}
		};

		struct PSS_UpdateLogoUrl : public sox::Marshallable
		{
			enum {uri = (50 << 8) | P2PLINKD_SVID};

			std::string m_strKey;	//不用
			uint32_t m_uUid;
			uint32_t m_uIndex;
			std::map<uint32_t, std::string> m_mapLogoUrl;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_strKey << m_uUid << m_uIndex;
				sox::marshal_container(p, m_mapLogoUrl);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_strKey >> m_uUid >> m_uIndex;
				sox::unmarshal_container(p, std::inserter(m_mapLogoUrl, m_mapLogoUrl.begin()));
			}
		};

		struct PSS_UpdateLogoUrlRes : public sox::Marshallable
		{
			enum {uri = (51 << 8) | P2PLINKD_SVID};

			std::string m_strKey;
			uint32_t m_uUid;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_strKey << m_uUid;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_strKey >> m_uUid;
			}
		};

		struct PSS_UpdateSeiCardUrl : public PSS_UpdateLogoUrl
		{
			enum {uri = (52 << 8) | P2PLINKD_SVID};
		};

		struct PSS_UpdateSeiCardUrlRes : public PSS_UpdateLogoUrl
		{
			enum {uri = (53 << 8) | P2PLINKD_SVID};
		};


		struct PCS_GrpLogoGetSign : public protocol::gcheck::PCS_GroupCommon
		{
			enum {uri = (60 << 8) | P2PLINKD_SVID};

			uint32_t	m_uTaskId;

			virtual void marshal(sox::Pack &p) const
			{
				protocol::gcheck::PCS_GroupCommon::marshal(p);
				p << m_uTaskId;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				protocol::gcheck::PCS_GroupCommon::unmarshal(p);
				p >> m_uTaskId;
			}	
		};

		struct PCS_CheckServiceForbid : public sox::Marshallable
		{
			enum {uri = (61 << 8) | P2PLINKD_SVID};

			uint32_t					m_uUid;
			std::vector<uint32_t>		m_vecType;
			std::string					m_strContext;
			
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid;
				sox::marshal_container(p, m_vecType);
				p << m_strContext;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid;
				sox::unmarshal_container(p, std::back_inserter(m_vecType));
				p >> m_strContext;
			}	
		};


		struct ServiceForbidInfo : public sox::Marshallable
		{
			EServiceType	m_enType;
			bool			m_bAllow;
			uint32_t		m_uForbidToTime;
			
			virtual void marshal(sox::Pack &p) const
			{
				p.push_uint16(m_enType);
				p << m_bAllow << m_uForbidToTime;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				m_enType = (EServiceType)p.pop_uint16();
				p >> m_bAllow >> m_uForbidToTime;
			}	
		};

		struct PCS_CheckServiceForbidRes : public sox::Marshallable
		{
			enum {uri = (62 << 8) | P2PLINKD_SVID};

			uint32_t						m_uUid;
			std::vector<ServiceForbidInfo>	m_vecForbidInfo;
			std::string						m_strContext;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid;
				sox::marshal_container(p, m_vecForbidInfo);	
				p << m_strContext;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid;
				sox::unmarshal_container(p, std::back_inserter(m_vecForbidInfo));
				p >> m_strContext;
			}	
		};
		struct PCS_CardSkinPicUpload : public PCS_TcpDataTransBase
		{
			enum {uri = (63 << 8) | P2PLINKD_SVID};

			std::string		m_strPicType;	//图片类型
			std::string		m_strGuid;
			std::string		m_strFileName;
			std::string		m_strData;
			std::string		m_strContext;
			uint32_t		m_uSessionId;	//session id
			uint32_t		m_uWidth;
			uint32_t		m_uHeight;
			std::vector<std::string>	m_vecWantSize;	//要得到的缩放尺寸,如"100*100",无缩放也要填写"宽*高"

			virtual void marshal(sox::Pack &p) const
			{
				PCS_TcpDataTransBase::marshal(p);
				p << m_strPicType << m_strGuid << m_strFileName;
				p.push_varstr32(m_strData.data(), m_strData.size());
				p << m_strContext << m_uSessionId << m_uWidth << m_uHeight;	
				marshal_container(p, m_vecWantSize);
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_TcpDataTransBase::unmarshal(p);
				p >> m_strPicType >> m_strGuid >> m_strFileName;
				m_strData = p.pop_varstr32();
				p >> m_strContext >> m_uSessionId >> m_uWidth >> m_uHeight;
				unmarshal_container(p, std::back_inserter(m_vecWantSize));
			}			
		};

		struct PCS_CardSkinPicUploadRes : public PCS_DataLinkRouteBase
		{
			enum {uri = (64 << 8) | P2PLINKD_SVID};

			std::string		m_strPicType;	//图片类型
			std::string		m_strGuid;
			std::string		m_strFileName;
			uint16_t		m_uResCode;
			std::string		m_strContext;
			uint32_t		m_uSessionId;	//session id
			std::map<std::string, std::string>	m_mapSizeUrl;	//缩放尺寸对应url

			virtual void marshal(sox::Pack &p) const
			{
				PCS_DataLinkRouteBase::marshal(p);
				p << m_strPicType << m_strGuid << m_strFileName << m_uResCode << m_strContext << m_uSessionId;
				marshal_container(p, m_mapSizeUrl);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_DataLinkRouteBase::unmarshal(p);
				p >> m_strPicType >> m_strGuid >> m_strFileName >> m_uResCode >> m_strContext >> m_uSessionId;
				unmarshal_container(p, std::inserter(m_mapSizeUrl, m_mapSizeUrl.begin()));
			}	
		};

	} //end p2p
} //end protocol


