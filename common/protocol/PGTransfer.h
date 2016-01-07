#pragma once

#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"
#include "common/protocol/PGMsgCacheType.h"

using namespace protocol::gmsgcache;

#define TO_ALL_MODE 0x00000001
#define TO_ADM_INFLD_MODE 0x00000008

namespace protocol
{
	namespace gtransfer
	{

		//���߱���ģʽ
		enum EOfflineMsgMode
		{
			ENUM_NOT_SAVE_OFFLINE = 0,
			ENUM_SAVE_OFFLINE_ACTION,
			ENUM_SAVE_OFFLINE_MSG,
			ENUM_SAVE_OFFLINE_ACTION_ONLY
		};

		union OfflineActionMode
		{
			struct Bits {
				bool TO_ALL:1;
				bool TO_GRP_ADM:1;
				bool TO_FLD_ADM:1;
				bool TO_ADM_INFLD:1;
				bool TO_OWER:1;
			} b;

			enum
			{
				OFF_MODE_TO_ALL = 1<<0,
				OFF_MODE_TO_GRP_ADM = 1<<1,
				OFF_MODE_TO_FLD_ADM = 1<<2,
				OFF_MODE_TO_ADM_INFLD = 1<<3,
				OFF_MODE_TO_TO_OWER = 1<<4
			};

			uint32_t N;
		};

		//���ṹ��ת�����ַ���
		inline void Packet2String(const sox::Marshallable &objMarshal, std::string &strPacket)
		{
			//��ԭ�����л�����װ��
			sox::PackBuffer pb;
			sox::Pack pk(pb);
			objMarshal.marshal(pk);

			strPacket.assign(pk.data(), pk.size());
		}

		//�ͻ�������gtransfer���㲥
		//��Ҫimlinkd����ϣ hash(gid) -> gtransferid
		//imlinkd PRouter.from = uidFrom(ld->uid, ss->getServerId());
		//��Ϊ����Ҫ����imlinkd������ʹ�ó���routes()������PRouter.from��д����ss->getServerId()
		//������ʱ���Ǽ���m_uSenderIdΪ��

		struct SGBroadcastProtocol : public sox::Marshallable
		{
			SGBroadcastProtocol() {};
			SGBroadcastProtocol(uint32_t Uri, const std::string &strOriPacket)
			{
				m_uOriUri = Uri;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}
			SGBroadcastProtocol(uint32_t Uri, const sox::Marshallable &objMarshal)
			{
				m_uOriUri = Uri;
				protocol::gtransfer::Packet2String(objMarshal, m_strOriPacket);
			}

			void SetSendId(uint32_t Uid)
			{
				m_uSenderId = Uid;
			}

			void SetGid(uint32_t Gid)
			{
				m_uGid = Gid;
			}
			void SetKey(uint32_t key)
			{
				m_uHashKey = key;
			}
			void SetOfflineMode(protocol::gtransfer::EOfflineMsgMode emOfflineMode = ENUM_NOT_SAVE_OFFLINE)
			{
				m_emOfflineMode = emOfflineMode;
			}

			uint32_t m_uSenderId; //������ID
			uint32_t m_uHashKey; //hash key
			uint32_t m_uGid; //Ⱥ/��ID
			protocol::gtransfer::EOfflineMsgMode m_emOfflineMode; //������Ϣģʽ

			uint32_t m_uOriUri; //ԭ��uri���������ɷ�ԭ��ʱʹ��
			std::string m_strOriPacket; //ԭ������

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uSenderId;
				p << m_uHashKey;
				p << m_uGid;
				p.push_uint8(m_emOfflineMode);

				p << m_uOriUri;
				p << m_strOriPacket;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uSenderId;
				p >> m_uHashKey;
				p >> m_uGid;
				m_emOfflineMode = (protocol::gtransfer::EOfflineMsgMode)p.pop_uint8();

				p >> m_uOriUri;
				p >> m_strOriPacket;
			}
		};

		//�ͻ�������gtransferת����Э��
		//����imlinkd��ϣ��������͸���һ��gtransfer����
		//imlinkd���ù��ı�����Ĭ�Ϸ��ͼ���

		struct SGForwardProtocol : public sox::Marshallable
		{
			SGForwardProtocol() {};
			SGForwardProtocol(uint32_t Uri, const std::string &strOriPacket)
			{
				m_uOriUri = Uri;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}
			SGForwardProtocol(uint32_t Uri, const sox::Marshallable &objMarshal)
			{
				m_uOriUri = Uri;
				protocol::gtransfer::Packet2String(objMarshal, m_strOriPacket);
			}

			void SetSendId(uint32_t Uid)
			{
				m_uSenderId = Uid;
			}
			void SetRecvUidVect(const std::vector<uint32_t> &vecRecvUid)
			{
				m_vecRecvUid.clear();
				m_vecRecvUid.assign(vecRecvUid.begin(), vecRecvUid.end());
			}
			void SetOfflineMode(protocol::gtransfer::EOfflineMsgMode emOfflineMode = ENUM_NOT_SAVE_OFFLINE)
			{
				m_emOfflineMode = emOfflineMode;
			}

			uint32_t m_uSenderId; //������ID
			std::vector<uint32_t> m_vecRecvUid; //��Ҫ���ո���Ϣ���û��б�
			protocol::gtransfer::EOfflineMsgMode m_emOfflineMode; //������Ϣģʽ

			uint32_t m_uOriUri; //ԭ��uri���������ɷ�ԭ��ʱʹ��
			std::string m_strOriPacket; //ԭ������

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uSenderId;
				sox::marshal_container(p, m_vecRecvUid);
				p.push_uint8(m_emOfflineMode);

				p << m_uOriUri;
				p << m_strOriPacket;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uSenderId;
				sox::unmarshal_container(p, back_inserter(m_vecRecvUid));
				m_emOfflineMode = (protocol::gtransfer::EOfflineMsgMode)p.pop_uint8();

				p >> m_uOriUri;
				p >> m_strOriPacket;
			}
		};

		struct GTransDispatchProtocol : public sox::Marshallable
		{
			GTransDispatchProtocol() {};
			GTransDispatchProtocol(uint32_t Uri, const std::string &strOriPacket)
			{
				m_uOriUri = Uri;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}
			GTransDispatchProtocol(uint32_t Uri, const sox::Marshallable &objMarshal)
			{
				m_uOriUri = Uri;
				protocol::gtransfer::Packet2String(objMarshal, m_strOriPacket);
			}

			void SetGid(uint32_t Gid)
			{
				m_uGid = Gid;
			}

			uint32_t m_uGid; //Ⱥ/��ID

			uint32_t m_uOriUri; //ԭ��uri���������ɷ�ԭ��ʱʹ��
			std::string m_strOriPacket; //ԭ������

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGid;

				p << m_uOriUri;
				p << m_strOriPacket;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGid;

				p >> m_uOriUri;
				p >> m_strOriPacket;
			}
		};

		//protocol����gtransferͨ�ù㲥��Э��

		struct PCS_GBroadcast  : public sox::Marshallable
		{
			enum {uri = (101 << 8) | protocol::GTRANS_SVID};

			PCS_GBroadcast() {};
			PCS_GBroadcast(uint32_t Uri, const std::string &strOriPacket) : m_stGBoardcastProtocol(Uri, strOriPacket) {};
			PCS_GBroadcast(uint32_t Uri, const sox::Marshallable &objMarshal) : m_stGBoardcastProtocol(Uri, objMarshal) {};

			void SetParams(uint32_t uKey, uint32_t uGid, uint32_t uSenderId, protocol::gtransfer::EOfflineMsgMode emOfflineMode = ENUM_NOT_SAVE_OFFLINE)
			{
				m_stGBoardcastProtocol.SetKey(uKey);
				m_stGBoardcastProtocol.SetGid(uGid);
				m_stGBoardcastProtocol.SetSendId(uSenderId);
				m_stGBoardcastProtocol.SetOfflineMode(emOfflineMode);
			}

			SGBroadcastProtocol m_stGBoardcastProtocol;

			virtual void marshal(sox::Pack &p) const
			{
				m_stGBoardcastProtocol.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				m_stGBoardcastProtocol.unmarshal(p);
			}
		};

		//protocol����gtransferͨ��ת����Э��
		//���汾����Ҫ��λ��ȷ����gtransfer

		struct PCS_GForward : public sox::Marshallable
		{
			enum {uri = (102 << 8) | protocol::GTRANS_SVID};

			PCS_GForward() {};
			PCS_GForward(uint32_t Uri, const std::string &strOriPacket) : m_stGForwardProtocol(Uri, strOriPacket) {};
			PCS_GForward(uint32_t Uri, const sox::Marshallable &objMarshal) : m_stGForwardProtocol(Uri, objMarshal) {};

			SGForwardProtocol m_stGForwardProtocol;

			virtual void marshal(sox::Pack &p) const
			{
				m_stGForwardProtocol.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				m_stGForwardProtocol.unmarshal(p);
			}
		};
/*
		//protocol����gtransfer�ɷ���Э��
		//���汾����ָ����GID����λ��gtransfer
		//��imlinkd hash(gid) -> gtransferId
		//����Ⱥ����ʱ�Ự PCS_GChatMsg
		//����Ⱥ���������� PCS_GTopicMsg PCS_GTopicRecentMsgReq

		struct PCS_GTransDispatch : public sox::Marshallable
		{
			enum {uri = (103 << 8) | protocol::GTRANS_SVID};

			PCS_GTransDispatch() {};
			PCS_GTransDispatch(uint32_t Uri, const std::string &strOriPacket) : m_stGTransDispatchProtocol(Uri, strOriPacket) {};
			PCS_GTransDispatch(uint32_t Uri, const sox::Marshallable &objMarshal) : m_stGTransDispatchProtocol(Uri, objMarshal) {};

			GTransDispatchProtocol	m_stGTransDispatchProtocol;

			virtual void marshal(sox::Pack &p) const
			{
				m_stGTransDispatchProtocol.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				m_stGTransDispatchProtocol.unmarshal(p);
			}
		};
*/

		//Ⱥ��֮����ʱ�Ự��Ϣ

		struct PCS_GChatMsg2 : public sox::Marshallable
		{
			enum {uri = (103 << 8) | protocol::GTRANS_SVID};

			PCS_GChatMsg2() {};
			PCS_GChatMsg2(uint32_t uGrpID, uint32_t uUid, uint32_t uMsgId, const std::string &strText, uint32_t uSendTime)
			{
				m_uGrpID = uGrpID;
				m_uUid = uUid;
				m_uMsgId = uMsgId;
				m_strText = strText;
				m_uSendTime = uSendTime;
			}

			uint32_t m_uGrpID;
			uint32_t m_uUid; //����gtransfer��ǰ���ǽ�����ID������gtranfer�Ժ���дΪ������ID
			uint32_t m_uMsgId;
			std::string m_strText;
			uint32_t m_uSendTime; //��Ϣ�����������ʱ�������gtransfer��д

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID;
				p << m_uUid;
				p << m_uMsgId;
				p << m_strText;
				p << m_uSendTime;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID;
				p >> m_uUid;
				p >> m_uMsgId;
				p >> m_strText;
				p >> m_uSendTime;
			}
		};

		//Ⱥ����ʱ�Ự��Ϣ��Ӧ

		struct PCS_GChatMsgRes2 : public sox::Marshallable
		{
			enum {uri = (104 << 8) | protocol::GTRANS_SVID};

			PCS_GChatMsgRes2() {};
			PCS_GChatMsgRes2(uint32_t uGrpID, uint32_t uUid, uint32_t uMsgId)
			{
				m_uGrpID = uGrpID;
				m_uUid = uUid; //����gtransfer��ǰ���ǽ�����ID������gtranfer�Ժ���дΪ������ID
				m_uMsgId = uMsgId;
			}

			uint32_t m_uGrpID;
			uint32_t m_uUid;
			uint32_t m_uMsgId;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID;
				p << m_uUid;
				p << m_uMsgId;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID;
				p >> m_uUid;
				p >> m_uMsgId;
			}
		};

		//Ⱥ��֮����ʱ�Ự����״̬

		struct PCS_GChatInputStatus : public sox::Marshallable
		{
			enum {uri = (105 << 8) | protocol::GTRANS_SVID};

			PCS_GChatInputStatus() {};
			PCS_GChatInputStatus(uint32_t uUid)
			{
				m_uUid = uUid;
			}

			uint32_t m_uUid;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid;
			}
		};


		// �ͻ���������������������¼

		struct PCS_GTopicRecentMsgReq2 : public sox::Marshallable
		{
			enum {uri = (106 << 8) | protocol::GTRANS_SVID};

			uint32_t                m_uGroupId;
			std::vector<CTopicInfo2> m_vecTopics;

			PCS_GTopicRecentMsgReq2(){}
			PCS_GTopicRecentMsgReq2(uint32_t groudId, const std::vector<CTopicInfo2>& vec)
				: m_uGroupId(groudId), m_vecTopics(vec)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId;
				marshal_container(p, m_vecTopics);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId;
				unmarshal_containerEx(p, m_vecTopics);
			}
		};

		// ����Ϣ��protocol ����Gtransfer����������δ�������������
		struct PCS_GTopicUnreadMsgNumReq : public sox::Marshallable
		{
			enum {uri = (107 << 8) | protocol::GTRANS_SVID};
			uint32_t                m_uGroupId;  // ������Ⱥid
			std::vector<CTopicInfo> m_vecTopics; // ��������⼯��

			PCS_GTopicUnreadMsgNumReq(){}
			PCS_GTopicUnreadMsgNumReq(uint32_t groudId, const std::vector<CTopicInfo>& vec)
				: m_uGroupId(groudId), m_vecTopics(vec)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId;
				marshal_container(p, m_vecTopics);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId;
				unmarshal_containerEx(p, m_vecTopics);
			}
		};


		// ����Ϣ��protocol ����Gtransfer����������Ⱥ/�����������¼����
		struct PCS_GTopicPopInfoReq : public sox::Marshallable
		{
			enum {uri = (108 << 8) | protocol::GTRANS_SVID};
			uint32_t                    m_uGroupId;  // �����Ⱥid
			uint32_t                    m_uFolderId; // ��������id
			std::vector<STopicPopInfo>  m_vecTopics; // ��������� ����

			PCS_GTopicPopInfoReq(){}
			PCS_GTopicPopInfoReq(uint32_t groudId, uint32_t folderId, const std::vector<STopicPopInfo>& vec)
				: m_uGroupId(groudId), m_uFolderId(folderId)
			{
				m_vecTopics = vec;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uFolderId;
				marshal_container(p, m_vecTopics);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId >> m_uFolderId;
				unmarshal_containerEx(p, m_vecTopics);
			}
		};

		// ����Ϣ��protocol ����Gtransfer����������Ⱥ/���������
		struct PCS_GTopicRecentPinnedTopicReq : public sox::Marshallable
		{
			enum {uri = (109 << 8) | protocol::GTRANS_SVID};
			uint32_t                m_uGroupId;  // ������Ⱥid
			uint32_t                m_uFolderId; // �������id

			PCS_GTopicRecentPinnedTopicReq(){}
			PCS_GTopicRecentPinnedTopicReq(uint32_t groudId, uint32_t folderId)
				: m_uGroupId(groudId), m_uFolderId(folderId)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uFolderId;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId >> m_uFolderId;
			}
		};

		// ����Ϣ��protocol ����Gtransfer����������Ⱥ/��δ��������Ŀ
		struct PCS_GTopicUnreadTopicNumReq : public sox::Marshallable
		{
			enum {uri = (110 << 8) | protocol::GTRANS_SVID};
			uint32_t    m_uGroupId;    // ������Ⱥid
			std::vector<SUnreadTopicNum> m_vecFolderId; // �������id

			PCS_GTopicUnreadTopicNumReq(){}
			PCS_GTopicUnreadTopicNumReq(uint32_t groudId, const std::vector<SUnreadTopicNum>& vec)
				: m_uGroupId(groudId)
			{
				m_vecFolderId = vec;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId;
				marshal_container(p, m_vecFolderId);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId;
				unmarshal_containerEx(p, m_vecFolderId);
			}
		};



		// protocol send it to gtransfer, and to access via gmsgcache.
		struct PCS_GTopicRead : public sox::Marshallable
		{
			enum {uri = (113 << 8) | protocol::GTRANS_SVID};

			uint32_t                 m_uSenderId;  //sender uid, written by gtransfer
			uint32_t                 m_uGroupId;
			uint32_t                 m_uFolderId;
			std::vector<uint32_t>    m_vecTopicId;

			PCS_GTopicRead(){}
			PCS_GTopicRead(uint32_t groudId, uint32_t folderId, const std::vector<uint32_t>& vec)
				: m_uGroupId(groudId), m_uFolderId(folderId)
			{
				m_vecTopicId = vec;
			}

			void SetUid(uint32_t uid)
			{
				m_uSenderId = uid;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uSenderId << m_uGroupId << m_uFolderId;
				sox::marshal_container(p, m_vecTopicId);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uSenderId >> m_uGroupId >> m_uFolderId;
				sox::unmarshal_container(p, std::back_inserter(m_vecTopicId));
			}
		};

		// monitor trans servers.
		struct PCS_GTransMonitor : public sox::Marshallable
		{
			enum {uri = (114 << 8) | protocol::GTRANS_SVID};

			uint32_t                 m_uGroupId;
			uint32_t                 m_uSrvIp;

			PCS_GTransMonitor(){}
			PCS_GTransMonitor(uint32_t groudId)
				: m_uGroupId(groudId)
			{}

			void SetIP(uint32_t ip)
			{
				m_uSrvIp = ip;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uSrvIp;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId >> m_uSrvIp;
			}
		};

		struct PCS_GTopicRecentMsgReq : public sox::Marshallable
		{
			enum {uri = (115 << 8) | protocol::GTRANS_SVID};

			uint32_t                m_uGroupId;
			std::vector<CTopicInfo> m_vecTopics;

			PCS_GTopicRecentMsgReq(){}
			PCS_GTopicRecentMsgReq(uint32_t groudId, const std::vector<CTopicInfo>& vec)
				: m_uGroupId(groudId), m_vecTopics(vec)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId;
				marshal_container(p, m_vecTopics);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId;
				unmarshal_containerEx(p, m_vecTopics);
			}
		};
	}
}
