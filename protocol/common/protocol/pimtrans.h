#ifndef PROTOCOL_PIMTRNAS_H_
#define PROTOCOL_PIMTRNAS_H_

#include "common/int_types.h"
#include "common/protocol/const.h"
#include "common/packet.h"
#include "common/core/base_svid.h"
#include "pimcommon.h"
#include "common/protocol/Im_Const.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

namespace protocol
{
	namespace im
	{

		//�ͻ�������imtransfer�㲥��Э��
		struct PCS_Broadcast  : public sox::Marshallable 
		{
			enum {uri = (1 << 8) | protocol::IMTRANS_SVID};

			PCS_Broadcast() {};
			PCS_Broadcast(uint32_t uri, const string &strOriPacket) 
			{
				m_ouri = uri;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}
			void SetSenderUid(uint32_t uSenderUid)
			{
				m_uSenderUid = uSenderUid;
			}

			uint32_t m_ouri; //ԭ��uri������imtrans�㲥ʱʹ��
			uint32_t m_uSenderUid; //�������û�Id
			string   m_strOriPacket; //ԭ������

			virtual void marshal(sox::Pack &p) const
			{
				p << m_ouri;
				p << m_uSenderUid;
				p << m_strOriPacket;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_ouri;
				p >> m_uSenderUid;
				p >> m_strOriPacket;
			}
		};

		//�ͻ�������imtransferת����Э��
		struct PCS_Forward : public sox::Marshallable
		{
			enum {uri = (2 << 8) | protocol::IMTRANS_SVID};

			PCS_Forward() {};
			PCS_Forward(uint32_t uri, const string &strOriPacket) 
			{
				m_ouri = uri;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}
			void SetSenderUid(uint32_t uSenderUid)
			{
				m_uSenderUid = uSenderUid;
			}
			void SetRecvUidVect(const vector<uint32_t> &vecRecvUid)
			{
				m_vecRecvUid.clear();
				m_vecRecvUid.assign(vecRecvUid.begin(), vecRecvUid.end());
			}
			void SetRecvUid(uint32_t uSenderUid)
			{
				m_vecRecvUid.push_back(uSenderUid);
			}
			void ResetRecvUid()
			{
				m_vecRecvUid.clear();
			}

			uint32_t m_ouri; //ԭ��uri������imtrans�㲥ʱʹ��
			uint32_t m_uSenderUid; //�������û�Id
			vector<uint32_t> m_vecRecvUid;
			string   m_strOriPacket; //ԭ������

			virtual void marshal(sox::Pack &p) const
			{
				p << m_ouri;
				p << m_uSenderUid;
				sox::marshal_container(p, m_vecRecvUid);
				p << m_strOriPacket;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_ouri;
				p >> m_uSenderUid;
				sox::unmarshal_container(p, back_inserter(m_vecRecvUid));
				p >> m_strOriPacket;
			}
		};

		//�����ȡȫ�����ѵ�����״̬
		struct PCS_GetBuddyStatusReq  : public protocol::im::CUidSet
		{
			enum {uri = (3 << 8) | protocol::IMTRANS_SVID};

			PCS_GetBuddyStatusReq() {}
		};

		//����ȫ�����ѵ�����״̬
		struct PCS_GetBuddyStatusRes  : public sox::Marshallable 
		{
			enum {uri = (4 << 8) | protocol::IMTRANS_SVID};

			std::map<uint32_t, protocol::E_IMSTATUS> m_mapStatus;

			PCS_GetBuddyStatusRes() {}

			virtual void marshal(sox::Pack &p) const
			{
				p.push_uint32(m_mapStatus.size());
				for (std::map<uint32_t, protocol::E_IMSTATUS>::const_iterator iter = m_mapStatus.begin(); iter != m_mapStatus.end(); ++iter)
				{
					p.push_uint32(iter->first);
					p.push_uint16(iter->second);
				}
				//sox::marshal_container(p, m_mapStatus);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				uint32_t size = p.pop_uint32();
				for (uint32_t i = 0; i < size; ++i)
				{
					uint32_t bid = p.pop_uint32();
					protocol::E_IMSTATUS status = static_cast<protocol::E_IMSTATUS>(p.pop_uint16());
					m_mapStatus.insert(std::make_pair(bid, status));
				}
				//sox::unmarshal_container(p, std::inserter(m_mapStatus, m_mapStatus.begin()));
			}
		};

		struct PCS_PeerMessage: public sox::Marshallable
		{
			enum {uri = (5 << 8) | protocol::IMTRANS_SVID};
			uint32_t m_uFromId;		//������id
			uint32_t m_uToId;		//������id
			uint32_t m_uURI;
			std::string m_strMsg;
			bool m_bSaveOffline;	//�ͻ��˷����ʾ�Ƿ��������Ϣ�������������ʾ�Ƿ���У��Ϣ
			uint32_t m_uMsgId;		//ֻ��m_bSaveOffline == trueʱ����Ч

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uFromId << m_uToId << m_uURI << m_strMsg << m_bSaveOffline << m_uMsgId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uFromId >> m_uToId >> m_uURI >> m_strMsg >> m_bSaveOffline >> m_uMsgId;
			}
		};


		struct PCS_PeerMessageRes: public sox::Marshallable
		{
			enum {uri = (6 << 8) | protocol::IMTRANS_SVID};
			uint32_t m_uFromId;
			uint32_t m_uPeerId;
			uint32_t m_uMsgId;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uFromId << m_uPeerId << m_uMsgId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uFromId >> m_uPeerId >> m_uMsgId;
			}
		};

//////////////////////////////////////////////////////////////////////////
		// �ֻ�yy֧�����Э��

		// �������к���������Ϣ

		struct PCS_GetBuddyStatusReq2  : public protocol::im::CUidSet
		{
			enum {uri = (7 << 8) | protocol::IMTRANS_SVID};
		};


		// ����ȫ�����ѵ�����״̬��Ϣ�����ֻ�yyʹ�õ���Э�飬���ص��ֶ��д����˰汾��Ϣ

		struct CVersionStatus : public sox::Marshallable
		{
			CVersionStatus() {};
			CVersionStatus(uint32_t uVersion, protocol::E_IMSTATUS emStatus)
				: m_uVersion(uVersion), m_emStatus(emStatus)
			{

			}

			uint32_t  m_uVersion;
			protocol::E_IMSTATUS m_emStatus;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uVersion;
				p.push_uint8(m_emStatus);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uVersion;
				m_emStatus = (protocol::E_IMSTATUS)p.pop_uint8();
			}
		};

		struct PCS_GetBuddyStatusRes2  : public sox::Marshallable 
		{
			enum {uri = (8 << 8) | protocol::IMTRANS_SVID};
			PCS_GetBuddyStatusRes2(){}

			std::map<uint32_t, CVersionStatus> m_mapUid2Status;

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_mapUid2Status);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				sox::unmarshal_container(p, std::inserter(m_mapUid2Status, m_mapUid2Status.begin()));
			}
		};


		//////////////////////////////////////////////////////////////////////////

		struct CImForwardInfo : public sox::Marshallable
		{
			uint32_t        m_uFlag;                 
			uint32_t        m_uSuffixId;             // Ŀ�������SrvId����imlinkd·��
			uint32_t        m_uSenderUid;            // ������
			uint32_t        m_uToUid;                // ������
			uint32_t        m_uSeqId;                // ȫ��ϵ�к�

			CImForwardInfo() { m_uFlag = 0; }

			// ���ͷ�����������͵ķ�ʽ
			void SetUpType(EImFrontEndNo type)
			{
				m_uFlag &= 0xfffffff0;
				m_uFlag |= ((uint32_t)type) & 0x0f;
			}

			EImFrontEndNo GetUpType()
			{
				return (EImFrontEndNo)(m_uFlag & 0x0f);
			}

			// ��������Զ�ת���ķ�ʽ
			void SetForwardType(EImFrontEndNo type)
			{
				m_uFlag &= 0xffffff0f;
				m_uFlag |= ((uint32_t)type << 4) & 0xf0;
			}

			EImFrontEndNo GetForwardType()
			{
				return (EImFrontEndNo)((m_uFlag >> 4) & 0x0f);
			}

			// ·������ת׼��
			void SetNum(uint32_t uNum)
			{
				m_uFlag &= 0xffff00ff;
				m_uFlag |= (uNum << 8) & 0xff00;
			}

			uint32_t GetNum()
			{
				return (m_uFlag >> 8) & 0xff;
			}

			// ������Դ�����������ͻ���
			void SetSenderType(EImMultForwardFrom emFrom)
			{
				m_uFlag &= 0xfff0ffff;
				m_uFlag |= ((uint32_t)emFrom << 16) & 0xf0000;
			}

			EImMultForwardFrom GetSenderType()
			{
				return (EImMultForwardFrom)((m_uFlag >> 16) & 0x0f);
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uFlag << m_uSenderUid << m_uToUid << m_uSeqId << m_uSuffixId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uFlag >> m_uSenderUid >> m_uToUid >> m_uSeqId >> m_uSuffixId;	
			}
		};

		// ˫·ת��
		struct PCS_ForwardToPeer : public sox::Marshallable
		{
			enum {uri = (9 << 8) | protocol::IMTRANS_SVID};
	
			CImForwardInfo  m_forwardInfo;
			uint32_t        m_uOUri;
			std::string     m_strPacket;

			virtual void marshal(sox::Pack &p) const
			{
				m_forwardInfo.marshal(p);
				p << m_uOUri;
				p.push_varstr32(m_strPacket.data(), m_strPacket.size());
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				m_forwardInfo.unmarshal(p);
				p >> m_uOUri;
				m_strPacket = p.pop_varstr32();
			}
		};

		struct PCS_ForwardToPeerRes : public CImForwardInfo
		{
			enum {uri = (10 << 8) | protocol::IMTRANS_SVID};
		};

		struct PSS_ForwardToPeer : public PCS_ForwardToPeer
		{
			enum {uri = (40 << 8) | protocol::IMTRANS_SVID};
		};

		struct PSS_ForwardToPeerRes : public PCS_ForwardToPeerRes
		{
			enum {uri = (41 << 8) | protocol::IMTRANS_SVID};
		};


		//Ӧ�ö˵���֪ͨ����
		struct PCC_APPForwardToPeer : public sox::Marshallable
		{
			enum {uri = (42 << 8) | protocol::IMTRANS_SVID};

			uint32_t        m_uOUri;
			uint32_t        m_uAppID;
			std::string     m_strPacket;
			uint32_t        m_uFromId;
			uint32_t        m_uToId;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uOUri << m_uAppID << m_strPacket << m_uFromId << m_uToId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uOUri >> m_uAppID >> m_strPacket >> m_uFromId >> m_uToId;
			}
		};
	}
}

#endif
