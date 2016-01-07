#pragma once
#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"


namespace protocol
{
namespace p2p
{

struct PCS_TcpBridgeLogin: public sox::Marshallable
{
	enum {uri = (1 << 8) | P2PBRIDGE_SVID};
	uint32_t m_uUID;
	std::string m_strAccout;
	std::string m_strShaPasswd;
	std::string m_strCookie;
	std::string m_strGUID;

	virtual void marshal(sox::Pack &p) const
	{
		p << m_uUID << m_strAccout << m_strShaPasswd << m_strCookie << m_strGUID;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_uUID >> m_strAccout >> m_strShaPasswd >> m_strCookie >> m_strGUID;
	}
};

struct PCS_TcpBridgeLoginRes: public sox::Marshallable
{
	enum {uri = (2 << 8) | P2PBRIDGE_SVID};
	std::string m_strDIP;
	std::string m_strWIP;
	std::vector<uint16_t> m_vecPort;
	std::string m_strGUID;
	
	virtual void marshal(sox::Pack &p) const
	{
		p << m_strDIP << m_strWIP << m_strGUID;
		marshal_container(p, m_vecPort);

	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_strDIP >> m_strWIP >> m_strGUID;
		unmarshal_container(p, std::back_inserter(m_vecPort));
	}
};

struct P2P_TransmitData: public sox::Marshallable
{
	enum {uri = (3 << 8) | P2PBRIDGE_SVID};
	uint32_t m_uPeerID;		//对方的ID
	uint32_t m_uPacketID;
	uint32_t m_uInnerURI;	//p2p内部协议
	std::string m_strInnerData;

	virtual void marshal(sox::Pack &p) const
	{
		p << m_uPeerID << m_uPacketID << m_uInnerURI << m_strInnerData;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_uPeerID >> m_uPacketID >> m_uInnerURI >> m_strInnerData;
	}
};

struct P2P_TransmitDataAck: public sox::Marshallable
{
	enum {uri = (4 << 8) | P2PBRIDGE_SVID};
	uint32_t m_uPeerID;		//对方的ID
	uint32_t m_uPacketID;	//包序号

	virtual void marshal(sox::Pack &p) const
	{
		p << m_uPeerID << m_uPacketID;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_uPeerID >> m_uPacketID;
	}
};


/**
 * UDP p2pBridge transmit msg define.
 *
 **/
struct CIPRouteTarget : public sox::Marshallable
{
	uint32_t m_uTargetIp; 	// the receiver's public ip address(dec).
	uint32_t m_uTargetPort;	// the receiver's public port.

	CIPRouteTarget(void) : 
		m_uTargetIp(0), 
		m_uTargetPort(0) 
		{}
	CIPRouteTarget(const uint32_t uIpAddr, const uint32_t uPort) : 
		m_uTargetIp(uIpAddr), 
		m_uTargetPort(uPort) 
		{}
	~CIPRouteTarget(void) {}
	
	virtual void marshal(sox::Pack &p) const
	{
		p << m_uTargetIp << m_uTargetPort;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_uTargetIp >> m_uTargetPort;
	}
	
};

/*UPD tranmist data.*/
struct P2P_UdpTransmitData: public sox::Marshallable
{
	enum {uri = (33 << 8) | P2PBRIDGE_SVID};

	std::string 	m_strGuid;		 //guid
	uint32_t 		m_uMyUid;		 //my uid.
	E_P2P_MYROLE 	m_eMyRole;		 //my role.
	uint32_t 		m_uPacketID;	 // the packet sequeue id.
	uint32_t 		m_uInnerUri;	 // the inner msg uri.
	std::string 	m_strInnerData;  // the inner msg data.

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

/*UPD tranmist data ack.*/
struct P2P_UdpTransmitDataAck: public sox::Marshallable
{
	enum {uri = (34 << 8) | P2PBRIDGE_SVID};

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

/*UPD Bridge Login.*/
struct PCS_UdpBridgeLogin: public sox::Marshallable
{
	enum {uri = (31 << 8) | P2PBRIDGE_SVID};
	
	uint32_t 		m_uMyUid;
	E_P2P_MYROLE 	m_eMyRole;
	std::string 	m_strAccout;
	std::string 	m_strShaPasswd;
	std::string 	m_strCookie;
	std::string 	m_strGuid;
	std::string 	m_strTargetIp;
	uint32_t 		m_uCookieVersion;

	PCS_UdpBridgeLogin() : m_uCookieVersion(-1) {}

	virtual void marshal(sox::Pack &p) const
	{
		p << m_uMyUid << (uint32_t)m_eMyRole << m_strAccout;
		p << m_strShaPasswd << m_strCookie << m_strGuid << m_strTargetIp << m_uCookieVersion;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		uint32_t uMyRole;
		p >> m_uMyUid >> uMyRole >> m_strAccout;
		p >> m_strShaPasswd >> m_strCookie >> m_strGuid;
		m_eMyRole = (E_P2P_MYROLE)uMyRole;

		if(p.size() > 0)
		{
			p >> m_strTargetIp;

			if(p.size() >= 4)
			{
				p >> m_uCookieVersion;
			}
			else
			{
				m_uCookieVersion = (uint32_t) (-1);
			}
			
		}
		else
		{
			m_strTargetIp = "";
			m_uCookieVersion = (uint32_t) (-1);
		}
	}
};

/*UPD Bridge Login Res.*/
struct PCS_UdpBridgeLoginRes: public sox::Marshallable
{
	enum {uri = (32 << 8) | P2PBRIDGE_SVID};
	std::string m_strGuid;
	std::string m_strDip;
	std::string m_strWip;
	std::vector<uint16_t> m_vecPort;
	std::string m_strTargetIp;
	
	virtual void marshal(sox::Pack &p) const
	{
		p << m_strGuid << m_strDip << m_strWip;
		marshal_container(p, m_vecPort);
		p << m_strTargetIp;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_strGuid >> m_strDip >> m_strWip;
		unmarshal_container(p, back_inserter(m_vecPort));

		if(p.size() > 0)
		{
			p >> m_strTargetIp;
		}
		else
		{
			m_strTargetIp = "";
		}
	}
};


} //end p2p
} //end protocol

