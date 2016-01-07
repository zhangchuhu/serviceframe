/*
Note: In protocol, all data with std::string type must be coded with UTF8
*/
#ifndef IImSearch_H 
#define IImSearch_H

#include <vector>
#include "ilist.h"
//#include "unistr.h"
//#include "common/int_types.h"
#include "common/protocol/pmsg.h"
#include "pmsg.h"
#include "PImMsg.h"

//namespace protocol
//{
//	namespace session
//	{
//		struct TextChat;
//	}
//}

namespace protocol
{
	namespace im
	{
		struct MessageDetail
		{
			time_t uDateTime;		// ���ں�ʱ��
			UniString strFromId;			//������uid
			UniString strToId;				//������uid
			protocol::session::TextChat MsgContent;		//��Ϣ����
			UniString strGuid;				//��Ϣ��GUID
			uint32_t uImid; //yy��
		};

		//��ȺID����Ϣ�汾
		struct MessageDetailGrp
		{
			time_t uDateTime;		// ���ں�ʱ��
			UniString strFromId;			//������uid
			UniString strToId;				//������uid
			protocol::session::TextChat MsgContent;		//��Ϣ����
			UniString strGuid;				//��Ϣ��GUID
			uint32_t uImid; //yy��
			uint32_t uGrpId; //ȺID
		};

		struct IImSearchWatcher
		{
			virtual ~IImSearchWatcher(){};

			// for UI, take action when PSS_ChatMsgSearchRes is received
			/*
			������Ϣ��ѯ���
			UID:
			FriendUID: �Զ˵�uid
			nCount: ��¼����
			records: ��Ϣ����
			nTaskID: ������
			*/
			virtual void OnGetFriendMsgRes(
				const uint32_t &nCount, 
				const std::vector<MessageDetail> &records, 
				const uint32_t &nTaskID) = 0;

			//��ȺID����Ϣ�汾
			virtual void OnGetFriendMsgGrpRes(
				const uint32_t &nCount, 
				const std::vector<MessageDetailGrp> &records, 
				const uint32_t &nTaskID) {}

			/*
			��Ӧ,���ز�ѯguid�Ľ��
			UID:
			FriendUID: �Զ˵�uid
			nCount: ��¼����
			strGuidStart: guid���Ͻ�
			strGuidEnd: guid���½�
			nTaskID: ������
			*/
			virtual void OnQueryFriendMsgIndexRes(
				const protocol::HULI &UID,
				const protocol::HULI &FriendUID, 
				const uint32_t &nCount, 
				const UniString &strGuidStart,
				const UniString &strGuidEnd,
				const uint32_t &nTaskID) = 0;

			virtual void OnDelFriendMsgRes(const uint32_t &nTaskID, const bool &res) = 0;
		};

		struct IOpImSearchWatcher//�ײ���Ҫʵ�ֵĽӿ�,��UI����
		{
			virtual ~IOpImSearchWatcher(){};
			virtual void watch(IImSearchWatcher *w) = 0;
			virtual void revoke(IImSearchWatcher *w) = 0;
		};

		struct IImSearch : public virtual IOpImSearchWatcher
		{
			virtual ~IImSearch(){};

			//��ѯ���������¼����������Index
			//UID���û���UID
			//FriendUID�����ѵ�UID
			//lStartTime: ��ʼʱ��
			//lEndTime: ����ʱ��
			//nPage���ڼ�ҳ
			//nPageSize��ÿҳ��С
			//nTaskID:�����ĵı�ʶ
			//   �첽���ط��ϵ�����,���ط����������������Index(GUID)
			virtual void QueryFriendMsgIndex(
				const protocol::HULI &UID,
				const protocol::HULI &FriendUID,
				const uint32_t lStartTime, 
				const uint32_t lEndTime, 
				const int32_t nPage, 
				const uint32_t nPageSize, 
				const uint32_t nTaskID) =0;

			//��ȡ���������¼
			//UID���û���UID
			//FriendUID�����ѵ�UID
			//lStartTime: ��ʼʱ��
			//lEndTime: ����ʱ��
			//Keyword: �ؼ���
			//nPage���ڼ�ҳ
			//nPageSize��ÿҳ��С
			//nTaskID:�����ĵı�ʶ
			//�첽�����û���Ϣ,��vector����
			virtual void GetFriendMsg (
				const protocol::HULI &UID,
				const protocol::HULI &FriendUID,
				const uint32_t lStartTime,
				const uint32_t lEndTime, 
				const UniString &Keyword, 
				const int32_t nPage, 
				const uint32_t nPageSize, 
				const uint32_t nTaskID) = 0;

			//��ȺID����Ϣ�汾
			virtual void GetFriendMsgGrp (
				const protocol::HULI &UID,
				const protocol::HULI &FriendUID,
				const uint32_t lStartTime,
				const uint32_t lEndTime, 
				const UniString &Keyword, 
				const int32_t nPage, 
				const uint32_t nPageSize, 
				const uint32_t nTaskID) {}; //��ʱ����պ���

			//ɾ�����������¼
			//UID:�û���UID
			//FriendUID:���ѵ�UID
			//vctGUID:��ϢGUID
			//nTaskID:�����ĵı�ʶ
			virtual void DelFriendMsg(const protocol::HULI &UID,
				const protocol::HULI &FriendUID, std::vector<std::wstring> vctGUID, 
				const uint32_t nTaskID)=0;

		};
	}
}
#endif
