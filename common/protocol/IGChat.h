#pragma once

#include "common/unistr.h"
#include "common/int_types.h"
#include "common/protocol/PImMsg.h"

namespace protocol
{
	namespace gtransfer
	{

		//群临时会话观察者接口定义

		class IGChatWatcher
		{
		public:
			virtual ~IGChatWatcher() {};

			//聊天消息到达通知
			virtual void OnMsgArrive(const uint32_t			&uGrpID,	//群ID
				const uint32_t			&uSenderId,	//发送消息的群成员
				const protocol::im::ImTextChat&  ImText, //接收的消息体
				const uint32_t			&uSendTime	//消息到达服务器的时间戳
				) = 0;

			//gchat msg timeout
			virtual void OnMsgTimeout(const uint32_t			&uGrpID,	//群ID
				const uint32_t			&uReceiverId,    //接收消息的成员
				const uint32_t 			&uMsgId,         //聊天消息流水号
				const protocol::im::ImTextChat&  ImText //接收的消息体
				) = 0;

			//发送的聊天消息回应通知
			virtual void OnMsgRes(const uint32_t			&uGrpID,	//群ID
				const uint32_t			&uRecverId,	//发送回应的接收者
				const uint32_t			&uMsgId		//聊天消息的流水号
				) = 0;

			//收到打字状态通知
			virtual void OnInputStatus(const uint32_t &uSenderId) = 0;	//发送输入状态的群成员
		};

		//欢察者操作接口定义

		class IOpGChatWatcher
		{
		public:
			virtual ~IOpGChatWatcher() {};

			virtual void watch(IGChatWatcher *w) = 0;
			virtual void revoke(IGChatWatcher *w) = 0;
		};

		//群临时会话接口定义

		class IGChat : public virtual IOpGChatWatcher
		{
		public:
			virtual ~IGChat() {};

			//发送聊天消息
			//时间戳由服务器来打
			virtual void SendMessage(uint32_t			uGrpID,		//群ID 
				uint32_t			uRecverId,	//接收消息的群成员
				uint32_t			uMsgId,		//自增量，唯一全局标识该消息
				const protocol::im::ImTextChat&	ImText //发送的消息内容
				) = 0;

			//发送打字状态
			virtual void SendInputStatus(uint32_t uRecverId) = 0;	//接收输入状态的群成员
		};

	}
}