#pragma once

#include "common/protocol/ilist.h"
#include "common/protocol/pmsg.h"
#include "common/protocol/pmsg4grp.h"
#include "common/unistr.h"
#include "common/protocol/const.h"
#include "common/protocol/IGofflineMsg.h"
#include "common/protocol/pimmsg.h"

//wuji start
//吴季鸿定义的接口名字规则如下

//如果是以Asny开头，表明该函数是异步操作函数（即需要回调函数反映函数操作结果)


//wuji end

namespace protocol{
	namespace im{

		enum E_CHAT_OP{GETOFFLINEMSG, DELOFFLINEMSG};

		//给UI接口处理的群临时聊天离线消息结构体
		struct  CGOfflineMsgClientData2
		{
			uint32_t m_uSenderUid;	//发送者ID
			uint32_t m_uSendTime;	//发送时间
			TextChat4Grp m_Text;	//群临时聊天消息体，包含GID等信息
		};

		struct IChatWatcher//UI实现的接口
		{
			virtual ~IChatWatcher(){}
			
			//IM点对点消息回调，两个接口：
			//当收到消息的yy号为0时，调用本接口
			virtual void onArriveMsg(const HULI& fromid, const protocol::session::TextChat& text,const uint32_t & uSendTime, const uint32_t &uMsgId) = 0;
			//否则调用本接口
			virtual void onArriveMsg(const HULI& fromid, const protocol::session::TextChat& text,const uint32_t & uSendTime, const uint32_t &uMsgId,
				const uint32_t& uFromImid) {};

			virtual void onStoreOfflineMsgFail(const HULI& toid, const uint32_t & uMsgId, const uint32_t& reason) = 0 ;

			//消息发送失败回调接口
			virtual void SendMsgFail(const HULI& toid, const protocol::session::TextChat& text, const uint32_t& reason) = 0;

			//发送离线消息回调接口（用于统计）
			virtual void OnSendOfflineMsgNotify(const HULI& toid) {}

			//wuji stat

			//当对方客户端收到聊天文本后，会回应一条聊天响应消息，这边客户端收到后，会触发UI
			virtual void onArriveMsgPeerRes(const HULI& fromid, const uint32_t & uMsgId/*聊天消息的流水号*/) = 0;
			virtual void onArriveInputStatus(const HULI& fromid) = 0;//收到打字状态的响应

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

			// 通知离线消息
			// mapFromId2Count （key: 发送人， value: 离线记录数）
			virtual void OnArriveOfflineMsg(const std::map<HULI, uint32_t>& mapFromId2Count) = 0;

			// 强制离线消息
			//当收到消息的yy号为0时，调用本接口
			virtual void OnArriveOfflineMsgForce(const HULI& fromId, 
				const protocol::session::TextChat& text, 
				const uint32_t& uSendTime,
				const uint32_t& uSeqId) = 0;
			//否则调用本接口
			virtual void OnArriveOfflineMsgForce(const HULI& fromId, 
										const protocol::session::TextChat& text, 
										const uint32_t& uSendTime,
										const uint32_t& uSeqId,
										const uint32_t& uFromImid) {};

			// 兼容版本离线消息
			//virtual void OnArriveOfflineMsg2(const HULI& fromId, 
			//	const protocol::session::TextChat& text, 
			//	const uint32_t& uSendTime,
			//	const uint32_t& uSeqId) = 0;

			//群临时聊天消息
			virtual void onArriveMsg4Grp(const HULI& fromid, const protocol::im::TextChat4Grp& text, 
				const uint32_t& uSendTime, const uint32_t &uMsgId, const HULI& gid) /*= 0;*/{}
			//群临时聊天消息响应
			virtual void onArriveMsg4GrpRes(const HULI& fromid, const uint32_t& uMsgId, const HULI& gid) /*= 0;*/{}
			////群临时聊天离线消息,消息格式为新消息体TextChat4Grp, YY 2.x忽略gid
			virtual void onOfflineMsg4Grp(const std::vector<CGOfflineMsgClientData2*> &vecData) /*= 0;*/ {}

			virtual void onSayToGrpPeerVerifyCodeRes(const uint32_t& uRes,const uint64_t& ufactor,const uint32_t& timestamp,const uint64_t& old_key,
				const uint64_t& valid_key,  const char* const & bin, const uint32_t &size) {}

			virtual void OnWindowShock(const HULI &strFromId, const std::wstring &strNickName) {};

			//自动回复的消息回调
			virtual void OnAutoReplayMsg(const HULI& fromid, const protocol::session::TextChat& text,const uint32_t & uSendTime, const uint32_t &uMsgId,
				const uint32_t& uFromImid) {};
		};

		struct IOpChatWatcher//底层需要实现的接口，供UI调用
		{
			virtual ~IOpChatWatcher(){}
			virtual void watch(IChatWatcher *w) = 0;
			virtual void revoke(IChatWatcher *w) = 0;
		};

		struct IIMChat : public virtual IOpChatWatcher//底层实现的接口
		{
			virtual ~IIMChat(){}

			virtual void saytopeer(const HULI& peerid, const protocol::session::TextChat& text) = 0;//这个函数将会给Ansy_saytopeer替换
			//wuji start

			virtual void Ansy_saytopeer(const HULI& peerid, uint32_t uMsgId/*自增量，唯一全局标识该消息*/,const protocol::session::TextChat& text) = 0;

						// 超时重发聊天消息
			virtual void SendMsgRetry(const HULI& peerid, 
									uint32_t uSeqId      /*自增量，唯一全局标识该消息*/,
									uint32_t uRetryCnt,
									const protocol::session::TextChat& text) = 0;

			// 用于截图调试，打印URL
			virtual void ScreenShotDebug(const std::string& strUrl) = 0;

			// 获取登陆服务器的网络类型
			virtual ISPType GetISP() = 0;

			virtual void Ansy_SendInputStatus(const HULI& peerid ) = 0;//发送打字状态
			
			//强制发送离线消息, 不作在线状态判断
			virtual void AnsySayOfflineMessage(const HULI& peerid, uint32_t uMsgId/*自增量，唯一全局标识该消息*/,const protocol::session::TextChat& text,
				uint32_t uPeerImid) = 0;

			//wuji end
			
			//发起群临时聊天   
			virtual void SayToGrpPeer(const HULI& peerid, uint32_t uMsgId/*自增量，唯一全局标识该消息*/,const protocol::im::TextChat4Grp& text) = 0;

			//群临时聊天重发
			virtual void SayToGrpPeerRetry(const HULI& peerid, 
									uint32_t uSeqId      /*自增量，唯一全局标识该消息*/,
									uint32_t uRetryCnt,
									const protocol::im::TextChat4Grp& text) = 0;

			virtual void SayToGrpPeerVerifyCode(const uint64_t ufactor,const uint32_t timestamp,const uint64_t uKey,const std::wstring m_strValue)=0;

			//取群临时聊天离线消息
			virtual void GetGOfflineMsg() = 0;
			virtual void SendWindowShock(const HULI &peer) = 0;

			//增加的用于自动回复的接口
			virtual void AutoReplyToPeer(const HULI& peerid, uint32_t uMsgId/*自增量，唯一全局标识该消息*/,const protocol::session::TextChat& text) = 0;
		};
	}
}
