#pragma once 

#include "common/unistr.h"
#include "common/packet.h"
#include "common/int_types.h"
#include "common/res_code.h"
#include "common/core/base_svid.h"
#include "common/protocol/IP2P.h"
#include "common/protocol/IPImStatistics.h"

#include <vector>
#include <string>
#include <utility>

using namespace protocol::p2p;

namespace protocol 
{
	namespace imstatistics 
	{

		struct PCS_P2PStaticsShot : public sox::Marshallable
		{
			enum {uri = (10 << 8 | protocol::IMSTAT_SVID)};

			//member list.
			std::string 	m_strGuid;
			uint32_t 		m_uSndUid;
			uint32_t 		m_uRcvUid;
			E_P2P_MYROLE		m_eMyRole;
			E_P2P_TRANSOBJ 	m_eTransObj;
			std::string 	m_strSrcPathName;
			std::string 	m_strDestPathName;
			std::string 	m_strSndingFileName;
			P2PTransmitMethod m_eTransMethod;
			E_P2P_ERRCODE	m_eFailureCode;
			E_P2P_ERRMODULE m_eFailModule;
			std::string 	m_strMyLastWords;
			
			uint64_t 		m_dwTotalBytes;
			uint64_t		m_dwTransBytes;
			uint32_t		m_uTotalFiles;
			uint32_t		m_uDoneFiles;
			bool			m_bResumable;
			bool			m_bUseResume;
			P2PNatInfo		m_myIPInfo;
			P2PNatInfo		m_peerIPInfo;
			std::vector<std::string> m_myLocalIPList;
			std::vector<std::string> m_peerLocalIPList;
			uint32_t 		m_uMyVersion;
			E_P2P_METHOD 	m_eBridgeMethodUsed;
			P2PTransmitStatus m_eCurrentTransState;
			std::string 	m_strBridgeIPInfo;
			std::string 	m_strReserveMsg;
			uint32_t 		m_uWaitListSize;

			uint32_t		m_uUserWaitTime;//ms
			uint32_t		m_uConnCostTime;//ms
			bool			m_isTransmitStart;
			bool			m_isPeerOffline;

			//Structor & Destructor.

			//Marshal & Unmarshal
			virtual void marshal(sox::Pack &p) const
			{
				p << m_strGuid << m_uSndUid << m_uRcvUid << (uint32_t)m_eMyRole;
				p << (uint32_t)m_eTransObj << m_strSrcPathName << m_strDestPathName;
				p <<m_strSndingFileName << (uint32_t)m_eTransMethod << (uint32_t)m_eFailureCode;
				p << (uint32_t)m_eFailModule << m_strMyLastWords << m_dwTotalBytes;
				p << m_dwTransBytes << m_uTotalFiles << m_uDoneFiles << m_bResumable << m_bUseResume;
				m_myIPInfo.marshal(p);
				m_peerIPInfo.marshal(p);
				marshal_container(p, m_myLocalIPList);
				marshal_container(p, m_peerLocalIPList);
				p << m_uMyVersion << (uint32_t)m_eBridgeMethodUsed << (uint32_t)m_eCurrentTransState;
				p << m_strBridgeIPInfo << m_strReserveMsg << m_uWaitListSize;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				uint32_t uMyRole;
				uint32_t uTransObj;
				uint32_t uTransMethod;
				uint32_t uFailureCode;
				uint32_t uFailModule;
				
				p >> m_strGuid >> m_uSndUid >> m_uRcvUid >> uMyRole >> uTransObj;
				p >> m_strSrcPathName >> m_strDestPathName >> m_strSndingFileName;
				p >> uTransMethod >> uFailureCode >> uFailModule >> m_strMyLastWords;
				p >> m_dwTotalBytes >> m_dwTransBytes >> m_uTotalFiles >> m_uDoneFiles;
				p >> m_bResumable >> m_bUseResume;
				m_myIPInfo.unmarshal(p);
				m_peerIPInfo.unmarshal(p);
				unmarshal_container(p, std::back_inserter(m_myLocalIPList));
				unmarshal_container(p, std::back_inserter(m_peerLocalIPList));

				m_eMyRole = (E_P2P_MYROLE)uMyRole;
				m_eTransObj = (E_P2P_TRANSOBJ)uTransObj;
				m_eTransMethod = (P2PTransmitMethod)uTransMethod;
				m_eFailureCode = (E_P2P_ERRCODE)uFailureCode;
				m_eFailModule = (E_P2P_ERRMODULE)uFailModule;

				if(p.size() >= 12)
				{
					uint32_t uBridgeMethodUsed;
					uint32_t uMyCuTransmitState;
					p >> m_uMyVersion >> uBridgeMethodUsed >> uMyCuTransmitState;
					m_eBridgeMethodUsed = (E_P2P_METHOD)uBridgeMethodUsed;
					m_eCurrentTransState = (P2PTransmitStatus)uMyCuTransmitState;
					
					if(p.size() > 0)
					{
						p >> m_strBridgeIPInfo;
						if(p.size() > 0)
						{
							p >> m_strReserveMsg;
							if(p.size() >= 4)
							{
								p >> m_uWaitListSize;
							}
							else
							{
								m_uWaitListSize = 0;
							}
						}
						else
						{
							m_strReserveMsg = "";
							m_uWaitListSize = 0;
						}
					}
					else
					{
						m_strBridgeIPInfo = "";
						m_strReserveMsg = "";
						m_uWaitListSize = 0;
					}
				}
				else
				{
					m_uMyVersion = 0;
					m_eBridgeMethodUsed = E_P2P_TCP;
					m_eCurrentTransState = ENUM_P2P_TRANSMIT_UNKNOW;
					m_strBridgeIPInfo = "";
					m_strReserveMsg = "";
					m_uWaitListSize = 0;
				}
 			}
			
		};

		/****************************************************
		 *					  ScreenShot 		    		*
		 ****************************************************/
		struct PCS_ImageTroubleShotNotify : public sox::Marshallable
		{
			enum {uri = (11 << 8 | protocol::IMSTAT_SVID)};

			//member list.
			CImageTroubleShot  m_ImageTroubleShot;
			
			//Structor & Destructor.
			PCS_ImageTroubleShotNotify(void) {}

			PCS_ImageTroubleShotNotify(const CImageTroubleShot &ImageTroubleShotObj) : 
				m_ImageTroubleShot(ImageTroubleShotObj)
				{}
				
			~PCS_ImageTroubleShotNotify(void){}

			//Marshal & Unmarshal
			virtual void marshal(sox::Pack &p) const
			{
				m_ImageTroubleShot.marshal(p);
			}
			
			virtual void unmarshal(const sox::Unpack &p)
			{
				m_ImageTroubleShot.unmarshal(p);
			}
			
		};		

		struct PCS_ImageStaticsShotNotify : public sox::Marshallable
		{
			enum {uri = (110 << 8 | protocol::IMSTAT_SVID)};

			//member list.
			std::vector<CImageStaticsShotSingle> m_vecImageStaticSingleList;
			std::vector<CImageStaticsShotFinalFail> m_vecImageStaticFinalFailList;
			std::vector<CImageStaticsShotFinalSuccess> m_vecImageStaticFinalSuccessList;

			//Structor & Destructor.
			PCS_ImageStaticsShotNotify(void)
			{
				m_vecImageStaticSingleList.clear();
				m_vecImageStaticFinalFailList.clear();
				m_vecImageStaticFinalSuccessList.clear();
			}

			~PCS_ImageStaticsShotNotify(void)
			{
				m_vecImageStaticSingleList.clear();
				m_vecImageStaticFinalFailList.clear();
				m_vecImageStaticFinalSuccessList.clear();
			}

			//Marshal & Unmarshal
			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_vecImageStaticSingleList);
				marshal_container(p, m_vecImageStaticFinalFailList);
				marshal_container(p, m_vecImageStaticFinalSuccessList);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				unmarshal_container(p, std::back_inserter(m_vecImageStaticSingleList));
				unmarshal_container(p, std::back_inserter(m_vecImageStaticFinalFailList));
				unmarshal_container(p, std::back_inserter(m_vecImageStaticFinalSuccessList));
			}

		}; 

	}
}

