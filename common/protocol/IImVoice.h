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
			uint8_t  selfConnType;         // 自己频道连接方式
			uint8_t  peerConnType;         // 对方频道连接方式

			uint32_t selfJoinDelay;        // 自己进入频道的时间
			uint32_t selfWaitDelay;        // 自己进入频道后，等待对方的时间
			uint32_t peerJoinDelay;        // 对方进入频道的时间
			uint32_t peerWaitDelay;        // 对方进入频道后，等待的时间
			uint32_t reqChannelDealy;      // 申请临时频道的时间
			
			uint32_t upPacket;             // 上行语音包
			uint32_t downPacket;           // 下行语音包
		};

		// 通知事件接口
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

		// 自由语音接口
		class IImVoice : public virtual IImVoiceWatcherBase
		{
		public:
			~IImVoice() {}

			// 获取参与该语音会话的用户
			virtual void GetMemberInfo(CImVoiceMemberInfo& memberInfo) = 0;
			
			virtual void GetMemberInfo(uint32_t uUid, UniString& str) = 0;
						
			// 获取isession指针
			virtual protocol::session::ISession* GetSession() = 0;

			// 连接关闭后，获取连接关闭原因
			virtual EImVoiceError GetLastError() = 0;			

			// 统计信息
			// return value : 
			//      true,   语音开始过，需要上报
			//      false,  语音未建立成功，不需要上报
			virtual bool GetVoiceStats(CImVoiceStatInfo& statInfo) = 0;                        
		};

		//////////////////////////////////////////////////////////////////////////
		
		class IImVoiceManagerWatcher
		{
		public:

			~IImVoiceManagerWatcher() {}

			virtual void OnPeerReqStartImVoice(const VoiceTag_T& mark) = 0;

			// 方便托盘闪动时，通过manager通知对方取消语音
			//virtual void OnVoiceStop(const VoiceTag_T& mark) = 0;
		};

		class IImVoiceManagerWatcherBase
		{
		public:

			~IImVoiceManagerWatcherBase() {}

			virtual void watch(IImVoiceManagerWatcher *w) = 0;
			virtual void revoke(IImVoiceManagerWatcher *w) = 0;
		};

		// 多语音实例管理类
		class IImVoiceManager 
		{
		public:

			virtual ~IImVoiceManager() {}
			
			// 请求开始自由语音
			virtual VoiceTag_T ReqStartImVoice(const CImVoiceMemberInfo& memberInfo) = 0;

			// 关闭自由语音
			virtual void StopImVoice(const VoiceTag_T& mark) = 0;

			// 忽略对方的语音请求
			virtual void IgnoreImVoice(const VoiceTag_T& mark) = 0;

			// 答复对方是否同意建立自由语音
			// @bAgree:      true    同意
			//               false   拒绝
			virtual void AnswerStartImVoiceReq(bool bAgree, const VoiceTag_T& mark) = 0;

			// 获取单个语音实例指针
			virtual IImVoice* GetPImVoice(const VoiceTag_T& mark) = 0;

			// 获取正在语音个数
			virtual int GetVoiceCount() = 0;

			virtual void watch(IImVoiceManagerWatcher *w) = 0;
			virtual void revoke(IImVoiceManagerWatcher *w) = 0;
		};

		class IImVoiceManagerMsgWatcher
		{
		public:
			~IImVoiceManagerMsgWatcher() {}

			//UI实现的接口
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

		//底层实现的接口
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
