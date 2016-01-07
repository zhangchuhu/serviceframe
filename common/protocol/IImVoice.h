#ifndef I_IM_VOICE_INCLUDE_H
#define I_IM_VOICE_INCLUDE_H

#include "int_types.h"
#include "protocol/const.h"
#include "common/unistr.h"
#include "common/protocol/isession.h"
#include "ImVoiceCommonDef.h"

namespace protocol
{
	namespace imvoice
	{

		struct CImVoiceStatInfo
		{
			uint8_t  selfConnType;         // �Լ�Ƶ�����ӷ�ʽ
			uint8_t  peerConnType;         // �Է�Ƶ�����ӷ�ʽ

			uint32_t selfJoinDelay;        // �Լ�����Ƶ����ʱ��
			uint32_t selfWaitDelay;        // �Լ�����Ƶ���󣬵ȴ��Է���ʱ��
			uint32_t peerJoinDelay;        // �Է�����Ƶ����ʱ��
			uint32_t peerWaitDelay;        // �Է�����Ƶ���󣬵ȴ���ʱ��
			uint32_t reqChannelDealy;      // ������ʱƵ����ʱ��
			
			uint32_t upPacket;             // ����������
			uint32_t downPacket;           // ����������
		};

		// ֪ͨ�¼��ӿ�
		class IImVoiceWatcher
		{
		public:
			~IImVoiceWatcher() {}

			virtual void OnImVoiceStatusChange(const uint32_t& uUid, const EImVoiceStatus& status) = 0;
		};

		class IImVoiceWatcherBase
		{
		public:
			virtual ~IImVoiceWatcherBase() {}

			virtual void watch(IImVoiceWatcher *w) = 0;
			virtual void revoke(IImVoiceWatcher *w) = 0;
		};

		// ���������ӿ�
		class IImVoice : public virtual IImVoiceWatcherBase
		{
		public:
			~IImVoice() {}

			// ��ȡ����������Ự���û�
			virtual void GetMemberInfo(CImVoiceMemberInfo& memberInfo) = 0;
			
			virtual void GetMemberInfo(uint32_t uUid, UniString& str) = 0;
						
			// ��ȡisessionָ��
			virtual protocol::session::ISession* GetSession() = 0;

			// ���ӹرպ󣬻�ȡ���ӹر�ԭ��
			virtual EImVoiceError GetLastError() = 0;			

			// ͳ����Ϣ
			// return value : 
			//      true,   ������ʼ������Ҫ�ϱ�
			//      false,  ����δ�����ɹ�������Ҫ�ϱ�
			virtual bool GetVoiceStats(CImVoiceStatInfo& statInfo) = 0;                        
		};

		//////////////////////////////////////////////////////////////////////////
		
		class IImVoiceManagerWatcher
		{
		public:

			~IImVoiceManagerWatcher() {}

			virtual void OnPeerReqStartImVoice(const VoiceTag_T& mark) = 0;

			// ������������ʱ��ͨ��manager֪ͨ�Է�ȡ������
			//virtual void OnVoiceStop(const VoiceTag_T& mark) = 0;
		};

		class IImVoiceManagerWatcherBase
		{
		public:

			~IImVoiceManagerWatcherBase() {}

			virtual void watch(IImVoiceManagerWatcher *w) = 0;
			virtual void revoke(IImVoiceManagerWatcher *w) = 0;
		};

		// ������ʵ��������
		class IImVoiceManager 
		{
		public:

			virtual ~IImVoiceManager() {}
			
			// ����ʼ��������
			virtual VoiceTag_T ReqStartImVoice(const CImVoiceMemberInfo& memberInfo) = 0;

			// �ر���������
			virtual void StopImVoice(const VoiceTag_T& mark) = 0;

			// ���ԶԷ�����������
			virtual void IgnoreImVoice(const VoiceTag_T& mark) = 0;

			// �𸴶Է��Ƿ�ͬ�⽨����������
			// @bAgree:      true    ͬ��
			//               false   �ܾ�
			virtual void AnswerStartImVoiceReq(bool bAgree, const VoiceTag_T& mark) = 0;

			// ��ȡ��������ʵ��ָ��
			virtual IImVoice* GetPImVoice(const VoiceTag_T& mark) = 0;

			// ��ȡ������������
			virtual int GetVoiceCount() = 0;

			virtual void watch(IImVoiceManagerWatcher *w) = 0;
			virtual void revoke(IImVoiceManagerWatcher *w) = 0;
		};

		class IImVoiceManagerMsgWatcher
		{
		public:
			~IImVoiceManagerMsgWatcher() {}

			//UIʵ�ֵĽӿ�
			virtual void OnImVoiceStartReq(const uint32_t& m_uTmpSid,const uint32_t& m_uInitiator,const std::string& m_strText) = 0;
			virtual void OnImVoiceStartRes(const VoiceTag_T& m_strTag,const uint8_t& m_uAgree,const uint32_t& m_uUid) = 0;
			virtual void OnImVoiceExit(const uint32_t& m_uUid,const VoiceTag_T& m_strTag,const EImVoiceError& m_error,const EImVoiceErrorStat& m_errSt) = 0;
			virtual void OnOnImVoiceStatInfo(const VoiceTag_T& m_strTag,const uint8_t&m_uConnType,const uint32_t& m_uJoinDelay, const uint32_t& m_uWaitDelay) =0;
			virtual void onChannelAllocated(const UniString &teamString, const DWORD &channelId) = 0;
			virtual void ReSet(const bool& bFlg) = 0;
			virtual void SetUid(const uint32_t& uUid) = 0;
		};


		class IImVoiceManagerMsgWatchBase 
		{
		public:
			~IImVoiceManagerMsgWatchBase() {}

			virtual void watch(IImVoiceManagerMsgWatcher *) = 0;
			virtual void revoke(IImVoiceManagerMsgWatcher *) = 0;
		};

		//�ײ�ʵ�ֵĽӿ�
		class IImVoiceManagerMsg:public virtual IImVoiceManagerMsgWatchBase
		{
		public:
			virtual void SendImVoiceStartRes(const std::vector<uint32_t> vecInvitee,const VoiceTag_T m_strTag,const uint8_t m_uAgree,const uint32_t m_uUid) = 0;
			virtual void SendImVoiceStartReq(const std::vector<uint32_t> vecInvitee,const uint32_t m_uTmpSid,const uint32_t m_uInitiator,const std::string m_strText) = 0;
			virtual void SendImVoiceExit(const std::vector<uint32_t> vecPeers,const uint32_t uid,const VoiceTag_T mark, const EImVoiceError err, const EImVoiceErrorStat errSt) = 0;
			virtual void SendImVoiceStatInfo(const uint32_t m_uInitiator,const VoiceTag_T m_strTag, const uint8_t m_uConnType, const uint32_t m_uJoinDelay,uint32_t m_uWaitDelay) = 0;
			virtual void AddToRecentConnList(const std::vector<uint32_t> list) =0;
		};

	}	
}

#endif
