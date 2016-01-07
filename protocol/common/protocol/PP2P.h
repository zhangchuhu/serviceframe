#pragma once
#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"
#include "protocol/IP2P.h"
#include "PP2PBridge.h"

namespace protocol
{
namespace p2p
{

//enum P2PTransmitMethod
//{
//	ENUM_P2P_TRANSMIT_VIA_LOCAL = 0,	//直连对方内网ip
//	ENUM_P2P_TRANSMIT_VIA_PUBLIC,		//直连对方公网ip
//	ENUM_P2P_TRANSMIT_VIA_PUNCH,		//先打洞，再连接对方公网ip
//	ENUM_P2P_TRANSMIT_VIA_BRIDGE		//通过tcp bridge传输
//};

//struct P2P_TransmitData: public sox::Marshallable
//{
//	enum {uri = (1 << 8) | P2P_SVID};
//	uint32_t m_uPeerID;		//对方的ID
//	uint32_t m_uPacketID;
//	uint32_t m_uInnerURI;	//p2p内部协议
//	std::string m_strInnerData;
//
//	virtual void marshal(sox::Pack &p) const
//	{
//		p << m_uPeerID << m_uPacketID << m_uInnerURI << m_strInnerData;
//	}
//
//	virtual void unmarshal(const sox::Unpack &p)
//	{
//		p >> m_uPeerID >> m_uPacketID >> m_uInnerURI >> m_strInnerData;
//	}
//};
//
//struct P2P_TransmitDataAck: public sox::Marshallable
//{
//	enum {uri = (2 << 8) | P2P_SVID};
//	uint32_t m_uPeerID;		//对方的ID
//	uint32_t m_uPacketID;	//包序号
//
//	virtual void marshal(sox::Pack &p) const
//	{
//		p << m_uPeerID << m_uPacketID;
//	}
//
//	virtual void unmarshal(const sox::Unpack &p)
//	{
//		p >> m_uPeerID >> m_uPacketID;
//	}
//};

#ifdef WIN32
struct P2P_TransmitMessage: public sox::Marshallable
{
	enum {uri = (1 << 8) | P2P_SVID};
	std::string m_strGUID;
	uint32_t m_uURI;
	std::string m_strMsg;

	virtual void marshal(sox::Pack &p) const
	{
		p << m_strGUID << m_uURI << m_strMsg;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_strGUID >> m_uURI >> m_strMsg;
	}
};

struct P2P_TransmitRequest: public sox::Marshallable
{
	enum {uri = (3 << 8) | P2P_SVID};
	P2PTransmitInfo m_TransInfo;
	std::string m_strDigest;
	uint32_t m_uModifyTime;

	virtual void marshal(sox::Pack &p) const
	{
		p << m_TransInfo << m_strDigest << m_uModifyTime;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_TransInfo >> m_strDigest >> m_uModifyTime;
	}
};

struct P2P_TransmitResponse: public sox::Marshallable
{
	enum {uri = (4 << 8) | P2P_SVID};
	std::string m_strGUID;
	P2PTransmitAction m_enAction;
	uint64_t m_dwLastTransBytes;

	virtual void marshal(sox::Pack &p) const
	{
		p << m_strGUID;
		p.push_uint8((uint8_t) m_enAction);
		p << m_dwLastTransBytes;
	}

	//Define the Key function.
	bool operator == (const std::string &strGuid) const 
	{
		return (m_strGUID == strGuid);
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_strGUID;
		m_enAction = (P2PTransmitAction) p.pop_uint8();

		if(p.size() >= 8)
		{
			p >> m_dwLastTransBytes;
		}
		else
		{
			m_dwLastTransBytes = 0;
		}
	}
};

struct P2P_TransmitConnect: public sox::Marshallable
{
	enum {uri = (7 << 8) | P2P_SVID};
	std::string m_strGUID;
	std::string m_strFromIP;
	uint16_t m_uFromPort;
	uint32_t m_uBlockSize;

	P2P_TransmitConnect()
		: m_uBlockSize(1024)
	{
	}
	virtual void marshal(sox::Pack &p) const
	{
		p << m_strGUID << m_strFromIP << m_uFromPort << m_uBlockSize;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_strGUID >> m_strFromIP >> m_uFromPort;
		if(p.size() >=4) {p >> m_uBlockSize;} else {m_uBlockSize = 1024;}
	}
};

struct P2P_TransmitAccept: public sox::Marshallable
{
	enum {uri = (8 << 8) | P2P_SVID};
	std::string m_strGUID;
	std::string m_strFromIP;
	uint16_t m_uFromPort;
	uint32_t m_uBlockSize;

	P2P_TransmitAccept() :
		m_strGUID(""),
		m_strFromIP(""),
		m_uFromPort(0),
		m_uBlockSize(1024)
	{}

	void clear(void)
	{
		m_strGUID = "";
		m_strFromIP = "";
		m_uFromPort = 0;
		m_uBlockSize = 1024;
	}

	virtual void marshal(sox::Pack &p) const
	{
		p << m_strGUID << m_strFromIP << m_uFromPort << m_uBlockSize;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_strGUID >> m_strFromIP >> m_uFromPort;
		if(p.size() >=4) {p >> m_uBlockSize;} else {m_uBlockSize = 1024;}
	}
};

//文件传输协议
struct P2P_OpenFile: public sox::Marshallable
{
	enum {uri = (9 << 8) | P2P_SVID};
	std::wstring m_strFileName;
	uint64_t m_uFileSize;
	std::string m_strDigest;
	uint32_t m_uModifyTime;

	virtual void marshal(sox::Pack &p) const
	{
		p << m_strFileName << m_uFileSize << m_strDigest << m_uModifyTime;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_strFileName >> m_uFileSize >> m_strDigest >> m_uModifyTime;
	}
};

struct P2P_OpenFileRes: public sox::Marshallable
{
	enum {uri = (10 << 8) | P2P_SVID};
	uint64_t m_uOffset; //断点续传，传输偏移量

	virtual void marshal(sox::Pack &p) const
	{
		p << m_uOffset;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_uOffset;
	}
};

/**
 * Notify the receiver how many files to send.
 * Receiver checks if finished by: (Or)
 * 1 - All files have been received.
 * 2 - Sender notifies receiver state-finish.
 */
struct P2P_TotalFiles: public sox::Marshallable
{
	enum {uri = (22 << 8) | P2P_SVID};
	uint32_t m_uTotalFiles;

	P2P_TotalFiles(void) {}
	P2P_TotalFiles(const uint32_t uTotal) : m_uTotalFiles(uTotal) {}
	~P2P_TotalFiles(void) {}
	
	virtual void marshal(sox::Pack &p) const
	{
		p << m_uTotalFiles;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_uTotalFiles;
	}
};

struct P2P_FileBlock: public sox::Marshallable
{
	enum {uri = (11 << 8) | P2P_SVID};
	std::string m_strFileData;

	virtual void marshal(sox::Pack &p) const
	{
		p << m_strFileData;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_strFileData;
	}
};

struct P2P_TransmitStatusChange: public sox::Marshallable
{
	enum {uri = (12 << 8) | P2P_SVID};
	std::string m_strGUID;
	P2PTransmitStatus m_enStatus;

	virtual void marshal(sox::Pack &p) const
	{
		p << m_strGUID;
		p.push_uint8((uint8_t) m_enStatus);
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_strGUID;
		m_enStatus = (P2PTransmitStatus) p.pop_uint8();
	}
};

struct P2P_PrepareTransmit: public sox::Marshallable
{
	enum {uri = (13 << 8) | P2P_SVID};
	std::string m_strGUID;
	P2PNatInfo m_PeerInfo;
	std::vector<std::string> m_vecLocalIP;	//兼容字段，本机可能有多个ip！

	virtual void marshal(sox::Pack &p) const
	{
		p << m_strGUID << m_PeerInfo;
		marshal_container(p, m_vecLocalIP);
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_strGUID >> m_PeerInfo;
		try
		{
			unmarshal_container(p, std::back_inserter(m_vecLocalIP));
		}
		catch (sox::UnpackError e)
		{
			m_vecLocalIP.clear();
		}
		//保证至少有一个
		if (m_vecLocalIP.empty())
		{
			m_vecLocalIP.push_back(m_PeerInfo.m_strLocalIP);
		}
	}
};

struct P2P_PrepareTransmitRes: public sox::Marshallable
{
	enum {uri = (14 << 8) | P2P_SVID};
	P2PNatInfo m_PeerInfo;
	std::vector<std::string> m_vecLocalIP;	//兼容字段，本机可能有多个ip！

	virtual void marshal(sox::Pack &p) const
	{
		p << m_PeerInfo;
		marshal_container(p, m_vecLocalIP);
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_PeerInfo;
		try
		{
			unmarshal_container(p, std::back_inserter(m_vecLocalIP));
		}
		catch (sox::UnpackError e)
		{
			m_vecLocalIP.clear();
		}
		//保证至少有一个
		if (m_vecLocalIP.empty())
		{
			m_vecLocalIP.push_back(m_PeerInfo.m_strLocalIP);
		}
	}
};

struct P2P_TransmitDirectly: public sox::Marshallable
{
	enum {uri = (15 << 8) | P2P_SVID};
	P2PNatInfo m_PeerInfo;

	virtual void marshal(sox::Pack &p) const
	{
		p << m_PeerInfo;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_PeerInfo;
	}
};

struct P2P_TransmitViaBridge: public sox::Marshallable
{
	enum {uri = (16 << 8) | P2P_SVID};
	std::string m_strDIP;
	std::string m_strWIP;
	std::vector<uint16_t> m_vecPort;

	E_P2P_METHOD m_eBridgeMethod;

	virtual void marshal(sox::Pack &p) const
	{
		p << m_strDIP << m_strWIP;
		marshal_container(p, m_vecPort);
		p << (uint32_t)m_eBridgeMethod;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		uint32_t uBridgeMethod;
		
		p >> m_strDIP >> m_strWIP;
		unmarshal_container(p, std::back_inserter(m_vecPort));
		
		if(p.size() >= 4)
		{
			p >> uBridgeMethod;
			m_eBridgeMethod = (E_P2P_METHOD)uBridgeMethod;
		}
		else
		{
			m_eBridgeMethod = E_P2P_TCP;
		}
	}
};

struct P2P_TransmitViaBridgeRes: public sox::Marshallable
{
	enum {uri = (17 << 8) | P2P_SVID};

	E_P2P_METHOD m_eBridgeMethod;

	virtual void marshal(sox::Pack &p) const
	{
		p << (uint32_t)m_eBridgeMethod;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		uint32_t uBridgeMethod;
		
		if(p.size() >= 4)
		{
			p >> uBridgeMethod;
			m_eBridgeMethod = (E_P2P_METHOD)uBridgeMethod;
		}
		else
		{
			m_eBridgeMethod = E_P2P_TCP;
		}
	}
};

struct P2P_CloseFile: public sox::Voidmable
{
	enum {uri = (18 << 8) | P2P_SVID};
	std::wstring m_strFileName;

	virtual void marshal(sox::Pack &p) const
	{
		p << m_strFileName;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_strFileName;
	}
};

struct P2P_CloseFileRes: public sox::Voidmable
{
	enum {uri = (19 << 8) | P2P_SVID};
};

struct P2P_RetransmitRequest: public P2P_TransmitRequest
{
	enum {uri = (20 << 8) | P2P_SVID};
	bool m_bForceBridge;

	P2P_RetransmitRequest()
		: m_bForceBridge(false)
	{
	}

	virtual void marshal(sox::Pack &p) const
	{
		P2P_TransmitRequest::marshal(p);
		p << m_bForceBridge;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		P2P_TransmitRequest::unmarshal(p);
		p >> m_bForceBridge;
	}
};

struct P2P_RetransmitResponse: public sox::Marshallable
{
	enum {uri = (21 << 8) | P2P_SVID};
	std::string m_strGUID;

	virtual void marshal(sox::Pack &p) const
	{
		p << m_strGUID;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_strGUID;
	}
};
#endif

} //end p2p
} //end protocol
