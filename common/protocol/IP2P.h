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
	//�Զ���Ϣ
	std::string m_strGUID;			//��������ID
	uint32_t m_uPeerID;				//�Է�ID
	
	//������Ϣ
	bool m_bIsSender;				//�Ƿ������
	P2PTransmitType m_enTransType;	//��������
	std::wstring m_strTransName;	//�ļ���
	uint64_t m_uTransSize;			//�ļ���С������������ļ��У����������ļ��еĴ�С��
	std::wstring m_strStorePath;
	uint64_t m_uTransBytes;			//�Ѵ�����ֽ���

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

	//�����Լ���NAT����
	virtual void OnNatTypeDetect(const P2PNatInfo &enType) = 0;

    virtual void OnTransmitAction(
		const P2PTransmitInfo &info,
        const P2PTransmitAction &enAction		//����״̬
        ) = 0;

    //���������״̬
    virtual void OnTransmitStatus(
        const P2PTransmitInfo &info,
        const P2PTransmitStatus &enStatus		//����״̬
        ) = 0;

    //�յ����ܴ�����������
    virtual void OnTransmitRequest(
        const P2PTransmitInfo &info,
        const bool &bIsResume                    //���������Ƕϵ�����
        ) = 0;

    //�������
    virtual void OnTransmitProgress(
        const P2PTransmitInfo &info,
		const uint32_t &uTransTime,		//����ʱ��
        const uint64_t &uTransBytes,   //�Ѵ����ֽ���
        const uint64_t &uCurrRate,     //˲ʱ�����ٶ�
        const bool &bDone             //�����Ƿ����
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

	////��ʼ��
	//virtual void Init(
	//	const uint32_t uMyID,						//UID
	//	const std::string &strAccount,		//ͨ��֤
	//	const std::string &strShaPasswd,	//����
	//	const std::string &strCookie		//cookie
	//	) = 0;

	////����
	//virtual void Clear() = 0;

    //�����ļ�
	virtual const std::string &SendFile(const uint32_t uPeerID, const std::wstring &strFileName, const uint64_t uSize) = 0;

    //�����ļ���
	virtual const std::string &SendDirectory(
		const uint32_t uPeerID,				//�Է�UID
		const std::wstring &strDirName,		//�ļ�����
		const std::vector<std::wstring> &vecFiles,
		const uint64_t uSize
		) = 0;

    //ȡ����������
    virtual void CancelTransmit(const std::string &strGUID) = 0;

    //�ܾ���������(ֻ�����ڱ�������ʱ)
    virtual void RefuseTransmit(const std::string &strGUID) = 0;

    //���ܴ�������(ֻ�����ڱ�������ʱ)
    virtual bool AcceptTransmit(const std::string &strGUID, const bool bIsResume, const std::wstring &strPath) = 0;
};

} //end p2p
} //end protocol
