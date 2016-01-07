#pragma once


#include <vector>

#include "common/int_types.h"
#include "common/protocol/pimmsg.h"

namespace protocol
{
	namespace gofflinemsg
	{
		struct CGofflineMsgClientData
		{
			uint32_t			m_uGid;		//群ID（因为只允许群内才能点对点聊天，所以既是发送方也是接收方所在的群）
			uint32_t			m_uSenderId;	//发送消息的用户ID
			protocol::im::ImTextChat m_objImText; //接收的消息体
			uint32_t			m_uSendTime;	//消息到达服务器的时间戳
		};


		struct IGofflineMsgWatcher//UI实现的接口
		{
			//取回离线消息触发的回调
			//参数：离线消息的发送人，消息，发送时间，群
			//virtual void OnArrive(const HULI& fromid, const protocol::session::TextChat& text,const uint32_t & uSendTime) = 0;
			//virtual void OnArrive(vector<>/*东杰的结构*/ ) = 0;
			virtual void OnArrive(const std::vector<CGofflineMsgClientData*> &vecData) = 0;
			
			//存储离线消息结果的回调
			//参数：离线消息的接收人所在群，离线消息的接收人，消息流水号，存储结果
			virtual void OnStore(const uint32_t &uGid, const uint32_t& uToId, const uint32_t & uMsgId, const uint32_t& uReason) = 0 ;

		};

		struct IOpGofflineMsgWatcher
		{
			virtual void watch(IGofflineMsgWatcher *w) = 0;
			virtual void revoke(IGofflineMsgWatcher *w) = 0;
		};

		//底层需要实现的接口，供UI调用
		struct IGofflineMsg:public virtual IOpGofflineMsgWatcher
		{
			virtual void Get()=0;

		};





	}
}