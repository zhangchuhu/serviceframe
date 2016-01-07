#pragma once
#include "common/packet.h"
#include "common/int_types.h"
#include "common/protocol/const.h"
#include <string>
//#include <wstring>


namespace protocol
{
namespace p2p
{

struct P2PTransmitInfo: public sox::Marshallable
{
	//对端信息
	std::string m_strGUID;			//传输任务ID
	uint32_t m_uPeerID;				//对方ID
	
	//传输信息
	bool m_bIsSender;				//是否接收者
	P2PTransmitType m_enTransType;	//传输类型
	std::wstring m_strTransName;	//文件名
	uint64_t m_uTransSize;			//文件大小（如果类型是文件夹，则是整个文件夹的大小）
	std::wstring m_strStorePath;
	uint64_t m_uTransBytes;			//已传输的字节数

	P2PTransmitInfo()
		: m_uTransSize(0), m_uTransBytes(0)
	{

	}

	bool IsSender()
	{
		return m_bIsSender;
	}

	bool IsReceiver()
	{
		return !m_bIsSender;
	}

	virtual void marshal(sox::Pack &p) const
	{
		p << m_strGUID << m_uPeerID;
		p.push_uint8((uint8_t) m_bIsSender);
		p.push_uint8((uint8_t) m_enTransType);
		p << m_strTransName << m_uTransSize << m_strStorePath;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> m_strGUID >> m_uPeerID;
		m_bIsSender = (bool) p.pop_uint8();
		m_enTransType = (P2PTransmitType) p.pop_uint8();
		p >> m_strTransName >> m_uTransSize >> m_strStorePath;
	}
};

struct P2PNatInfo: public sox::Marshallable
{
	P2PNatType m_enNatType;
	std::string m_strPublicIP;
	uint16_t m_uPublicPort;
	std::string m_strLocalIP;
	uint16_t m_uLocalPort;

	P2PNatInfo()
		: m_enNatType(ENUM_P2P_NAT_UNKNOWN), m_uPublicPort(0), m_uLocalPort(0)
	{
	}

	virtual void marshal(sox::Pack &p) const
	{
		p.push_uint8((uint8_t) m_enNatType);
		p << m_strPublicIP << m_uPublicPort << m_strLocalIP << m_uLocalPort;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		m_enNatType = (P2PNatType) p.pop_uint8();
		p >> m_strPublicIP >> m_uPublicPort >> m_strLocalIP >> m_uLocalPort;
	}
};

class IP2PWatcher
{
public:
	virtual ~IP2PWatcher() {};

	//检测出自己的NAT类型
	virtual void OnNatTypeDetect(const P2PNatInfo &enType) = 0;

    virtual void OnTransmitAction(
		const P2PTransmitInfo &info,
        const P2PTransmitAction &enAction		//任务状态
        ) = 0;

    //传输任务的状态
    virtual void OnTransmitStatus(
        const P2PTransmitInfo &info,
        const P2PTransmitStatus &enStatus		//任务状态
        ) = 0;

    //收到接受传输任务请求
    virtual void OnTransmitRequest(
        const P2PTransmitInfo &info,
        const bool &bIsResume                    //是新任务还是断点续传
        ) = 0;

    //传输进度
    virtual void OnTransmitProgress(
        const P2PTransmitInfo &info,
		const uint32_t &uTransTime,		//传输时间
        const uint64_t &uTransBytes,   //已传输字节数
        const uint64_t &uCurrRate,     //瞬时传输速度
        const bool &bDone             //传输是否完成
        ) = 0;
};


class IP2PWatcherBase
{
public:
	virtual ~IP2PWatcherBase() {};

	virtual void watch(IP2PWatcher *w) = 0;
	virtual void revoke(IP2PWatcher *w) = 0;
};

class IP2PInterface: public virtual IP2PWatcherBase
{
public:
	virtual ~IP2PInterface() {};

	////初始化
	//virtual void Init(
	//	const uint32_t uMyID,						//UID
	//	const std::string &strAccount,		//通行证
	//	const std::string &strShaPasswd,	//密码
	//	const std::string &strCookie		//cookie
	//	) = 0;

	////清理
	//virtual void Clear() = 0;

    //发送文件
	virtual const std::string &SendFile(const uint32_t uPeerID, const std::wstring &strFileName, const uint64_t uSize) = 0;

    //发送文件夹
	virtual const std::string &SendDirectory(
		const uint32_t uPeerID,				//对方UID
		const std::wstring &strDirName,		//文件夹名
		const std::vector<std::wstring> &vecFiles,
		const uint64_t uSize
		) = 0;

    //取消传输任务
    virtual void CancelTransmit(const std::string &strGUID) = 0;

    //拒绝传输任务(只能用于被动接收时)
    virtual void RefuseTransmit(const std::string &strGUID) = 0;

    //接受传输任务(只能用于被动接收时)
    virtual bool AcceptTransmit(const std::string &strGUID, const bool bIsResume, const std::wstring &strPath) = 0;
};

} //end p2p
} //end protocol
