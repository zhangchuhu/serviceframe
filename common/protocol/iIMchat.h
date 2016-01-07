#pragma once

#include "common/protocol/ilist.h"
#include "common/protocol/pmsg.h"
#include "common/protocol/pmsg4grp.h"
#include "common/unistr.h"
#include "common/protocol/const.h"
#include "common/protocol/IGofflineMsg.h"
#include "common/protocol/pimmsg.h"

//wuji start
//�⼾�趨��Ľӿ����ֹ�������

//�������Asny��ͷ�������ú������첽��������������Ҫ�ص�������ӳ�����������)


//wuji end

namespace protocol{
	namespace im{

		enum E_CHAT_OP{GETOFFLINEMSG, DELOFFLINEMSG};

		//��UI�ӿڴ����Ⱥ��ʱ����������Ϣ�ṹ��
		struct  CGOfflineMsgClientData2
		{
			uint32_t m_uSenderUid;	//������ID
			uint32_t m_uSendTime;	//����ʱ��
			TextChat4Grp m_Text;	//Ⱥ��ʱ������Ϣ�壬����GID����Ϣ
		};

		struct IChatWatcher//UIʵ�ֵĽӿ�
		{
			virtual ~IChatWatcher(){}
			
			//IM��Ե���Ϣ�ص��������ӿڣ�
			//���յ���Ϣ��yy��Ϊ0ʱ�����ñ��ӿ�
			virtual void onArriveMsg(const HULI& fromid, const protocol::session::TextChat& text,const uint32_t & uSendTime, const uint32_t &uMsgId) = 0;
			//������ñ��ӿ�
			virtual void onArriveMsg(const HULI& fromid, const protocol::session::TextChat& text,const uint32_t & uSendTime, const uint32_t &uMsgId,
				const uint32_t& uFromImid) {};

			virtual void onStoreOfflineMsgFail(const HULI& toid, const uint32_t & uMsgId, const uint32_t& reason) = 0 ;

			//��Ϣ����ʧ�ܻص��ӿ�
			virtual void SendMsgFail(const HULI& toid, const protocol::session::TextChat& text, const uint32_t& reason) = 0;

			//����������Ϣ�ص��ӿڣ�����ͳ�ƣ�
			virtual void OnSendOfflineMsgNotify(const HULI& toid) {}

			//wuji stat

			//���Է��ͻ����յ������ı��󣬻��Ӧһ��������Ӧ��Ϣ����߿ͻ����յ��󣬻ᴥ��UI
			virtual void onArriveMsgPeerRes(const HULI& fromid, const uint32_t & uMsgId/*������Ϣ����ˮ��*/) = 0;
			virtual void onArriveInputStatus(const HULI& fromid) = 0;//�յ�����״̬����Ӧ

			//wuji end

			/*
			strFromId: the buddy's' uid 
			uMsgId:	the id of message.
			strGuid:	the unique id in access system. 
			*/
			virtual void OnMessageGuidRes(const HULI& strFromuId,  
										const uint32_t& uMsgId, 
										const std::wstring& strGuid, 
										const time_t &uSenderTime) = 0;

			// ֪ͨ������Ϣ
			// mapFromId2Count ��key: �����ˣ� value: ���߼�¼����
			virtual void OnArriveOfflineMsg(const std::map<HULI, uint32_t>& mapFromId2Count) = 0;

			// ǿ��������Ϣ
			//���յ���Ϣ��yy��Ϊ0ʱ�����ñ��ӿ�
			virtual void OnArriveOfflineMsgForce(const HULI& fromId, 
				const protocol::session::TextChat& text, 
				const uint32_t& uSendTime,
				const uint32_t& uSeqId) = 0;
			//������ñ��ӿ�
			virtual void OnArriveOfflineMsgForce(const HULI& fromId, 
										const protocol::session::TextChat& text, 
										const uint32_t& uSendTime,
										const uint32_t& uSeqId,
										const uint32_t& uFromImid) {};

			// ���ݰ汾������Ϣ
			//virtual void OnArriveOfflineMsg2(const HULI& fromId, 
			//	const protocol::session::TextChat& text, 
			//	const uint32_t& uSendTime,
			//	const uint32_t& uSeqId) = 0;

			//Ⱥ��ʱ������Ϣ
			virtual void onArriveMsg4Grp(const HULI& fromid, const protocol::im::TextChat4Grp& text, 
				const uint32_t& uSendTime, const uint32_t &uMsgId, const HULI& gid) /*= 0;*/{}
			//Ⱥ��ʱ������Ϣ��Ӧ
			virtual void onArriveMsg4GrpRes(const HULI& fromid, const uint32_t& uMsgId, const HULI& gid) /*= 0;*/{}
			////Ⱥ��ʱ����������Ϣ,��Ϣ��ʽΪ����Ϣ��TextChat4Grp, YY 2.x����gid
			virtual void onOfflineMsg4Grp(const std::vector<CGOfflineMsgClientData2*> &vecData) /*= 0;*/ {}

			virtual void onSayToGrpPeerVerifyCodeRes(const uint32_t& uRes,const uint64_t& ufactor,const uint32_t& timestamp,const uint64_t& old_key,
				const uint64_t& valid_key,  const char* const & bin, const uint32_t &size) {}

			virtual void OnWindowShock(const HULI &strFromId, const std::wstring &strNickName) {};

			//�Զ��ظ�����Ϣ�ص�
			virtual void OnAutoReplayMsg(const HULI& fromid, const protocol::session::TextChat& text,const uint32_t & uSendTime, const uint32_t &uMsgId,
				const uint32_t& uFromImid) {};
		};

		struct IOpChatWatcher//�ײ���Ҫʵ�ֵĽӿڣ���UI����
		{
			virtual ~IOpChatWatcher(){}
			virtual void watch(IChatWatcher *w) = 0;
			virtual void revoke(IChatWatcher *w) = 0;
		};

		struct IIMChat : public virtual IOpChatWatcher//�ײ�ʵ�ֵĽӿ�
		{
			virtual ~IIMChat(){}

			virtual void saytopeer(const HULI& peerid, const protocol::session::TextChat& text) = 0;//������������Ansy_saytopeer�滻
			//wuji start

			virtual void Ansy_saytopeer(const HULI& peerid, uint32_t uMsgId/*��������Ψһȫ�ֱ�ʶ����Ϣ*/,const protocol::session::TextChat& text) = 0;

						// ��ʱ�ط�������Ϣ
			virtual void SendMsgRetry(const HULI& peerid, 
									uint32_t uSeqId      /*��������Ψһȫ�ֱ�ʶ����Ϣ*/,
									uint32_t uRetryCnt,
									const protocol::session::TextChat& text) = 0;

			// ���ڽ�ͼ���ԣ���ӡURL
			virtual void ScreenShotDebug(const std::string& strUrl) = 0;

			// ��ȡ��½����������������
			virtual ISPType GetISP() = 0;

			virtual void Ansy_SendInputStatus(const HULI& peerid ) = 0;//���ʹ���״̬
			
			//ǿ�Ʒ���������Ϣ, ��������״̬�ж�
			virtual void AnsySayOfflineMessage(const HULI& peerid, uint32_t uMsgId/*��������Ψһȫ�ֱ�ʶ����Ϣ*/,const protocol::session::TextChat& text,
				uint32_t uPeerImid) = 0;

			//wuji end
			
			//����Ⱥ��ʱ����   
			virtual void SayToGrpPeer(const HULI& peerid, uint32_t uMsgId/*��������Ψһȫ�ֱ�ʶ����Ϣ*/,const protocol::im::TextChat4Grp& text) = 0;

			//Ⱥ��ʱ�����ط�
			virtual void SayToGrpPeerRetry(const HULI& peerid, 
									uint32_t uSeqId      /*��������Ψһȫ�ֱ�ʶ����Ϣ*/,
									uint32_t uRetryCnt,
									const protocol::im::TextChat4Grp& text) = 0;

			virtual void SayToGrpPeerVerifyCode(const uint64_t ufactor,const uint32_t timestamp,const uint64_t uKey,const std::wstring m_strValue)=0;

			//ȡȺ��ʱ����������Ϣ
			virtual void GetGOfflineMsg() = 0;
			virtual void SendWindowShock(const HULI &peer) = 0;

			//���ӵ������Զ��ظ��Ľӿ�
			virtual void AutoReplyToPeer(const HULI& peerid, uint32_t uMsgId/*��������Ψһȫ�ֱ�ʶ����Ϣ*/,const protocol::session::TextChat& text) = 0;
		};
	}
}
