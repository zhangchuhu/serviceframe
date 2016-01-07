#ifndef __IIMAPPCHAT_H__
#define __IIMAPPCHAT_H__

#include <string>
#include "common/unistr.h"
#include "const.h"
#include "iImV2.h"
#include "pmsg.h"
#include "iimsearch.h"
#include <exception>
#include "common/protocol/Im_Const.h"


namespace protocol{

	namespace imapp{
		struct IAppChatRegister  
		{

			virtual void OnArriveMessage(const uint32_t fromid, const protocol::session::TextChat& text,
				uint32_t uSendTime, uint32_t uSeqId, uint32_t uImId) = 0;

			/*
			strFromId: the buddy's' uid 
			uMsgId:	the id of message.
			strGuid:	the unique id in access system. 
			*/
			//virtual void OnMessageGuidRes(const uint32_t strFromuId, const uint32_t& uMsgId, const std::wstring& strGuid, const time_t &uSenderTime) = 0;


			// ֪ͨ������Ϣ��¼��
			// mapFromId2Count ��key: �����ˣ� value: ���߼�¼����
			virtual void OnArriveOfflineMsgCount(const std::map<uint32_t , uint32_t>& mapFromId2Count) = 0;

			// ������Ϣ
			virtual void OnArriveOfflineMsg(uint32_t uFrom, uint32_t uFromImId,
				uint32_t uSeqId, uint32_t uSendTime, const protocol::session::TextChat& text) = 0;

			
			// for UI, take action when PSS_ChatMsgSearchRes is received
			/*
			������Ϣ��ѯ���
			UID:
			FriendUID: �Զ˵�uid
			nCount: ��¼����
			records: ��Ϣ����
			nTaskID: ������
			*/
			/*
			virtual void OnGetFriendMsgRes(
				const uint32_t &nCount, 
				const std::vector<protocol::im::MessageDetail> &records, 
				const uint32_t &nTaskID) = 0;*/

			/*
			��Ӧ,���ز�ѯguid�Ľ��
			UID:
			FriendUID: �Զ˵�uid
			nCount: ��¼����
			strGuidStart: guid���Ͻ�
			strGuidEnd: guid���½�
			nTaskID: ������
			*/
			/*
			virtual void OnQueryFriendMsgIndexRes(
				const protocol::HULI &UID,
				const protocol::HULI &FriendUID, 
				const uint32_t &nCount, 
				const UniString &strGuidStart,
				const UniString &strGuidEnd,
				const uint32_t &nTaskID) = 0;*/


			virtual void OnDelFriendMsgRes(const uint32_t &nTaskID, const bool &res) = 0;

			/*�յ�����������֪ͨ
			uPeerId: �Զ˵�uid
			uUri: �����id(�൱��������)
			bin:  �����������ʵ��
			*/
			virtual void OnRecvBin(const uint32_t &uPeerId, const uint32_t &uUri, const std::string& bin) = 0;

			IAppChatRegister(uint32_t iAppID):m_iAppID(iAppID){}

			void SetAppID(uint32_t iAppID){if(iAppID < ENUM_APP_MAKE_FRIEND) {throw std::logic_error("appid must large than 17.");} m_iAppID = iAppID;}

			const uint32_t GetAppID() const{return m_iAppID;}

		private:
			uint32_t m_iAppID;
		};

		struct IAppChatWatcher//�ײ���Ҫʵ�ֵĽӿ�,��UI����
		{
			virtual ~IAppChatWatcher(){};
			virtual void watch(IAppChatRegister *w) = 0;
			virtual void revoke(IAppChatRegister *w) = 0;
		};


		class IAppChatBase : public IAppChatWatcher
		{
		public:
			virtual ~IAppChatBase(){}

			//������Ϣid(seqid)
			virtual uint32_t SayToPeer(const IAppChatRegister &w, const uint32_t peerid, const protocol::session::TextChat& text) = 0;
/*
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
				const IAppRegister &w,
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
				const IAppRegister &w,
				const protocol::HULI &UID,
				const protocol::HULI &FriendUID,
				const uint32_t lStartTime,
				const uint32_t lEndTime, 
				const UniString &Keyword, 
				const int32_t nPage, 
				const uint32_t nPageSize, 
				const uint32_t nTaskID) = 0;*/


			//ɾ�����������¼
			//UID:�û���UID
			//FriendUID:���ѵ�UID
			//vctGUID:��ϢGUID
			//nTaskID:�����ĵı�ʶ
			virtual void DelFriendMsg(
				const IAppChatRegister &w,
				const protocol::HULI &UID,
				const protocol::HULI &FriendUID, std::vector<std::wstring> vctGUID, 
				const uint32_t nTaskID)=0;


			// ��ȡ������Ϣ������
			virtual void GetOfflineMsgCount(const IAppChatRegister &w) = 0;

			// ȡ������Ϣ
			virtual void GetOfflineMsg(const IAppChatRegister& w) = 0;

			/*���Ͷ�����������Է�
			uPeerId: �Է���uid
			uUri: �����id(�൱��������)
			bin:  �����������ʵ��
			*/	
			virtual void SayBinToPeer(const IAppChatRegister& w, const uint32_t &uPeerId, const uint32_t uUri, const std::string& bin) = 0;
		};
	};
};


#endif //__IIMAPPCHAT_H__
