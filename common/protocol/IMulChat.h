#pragma once 

#include "common/unistr.h"
#include "common/int_types.h"
#include "common/protocol/const.h"

#include "pmsg.h"
#include "PMulChatDef.h"

#include <iostream>
#include <string>
#include <vector>

namespace protocol
{
	namespace im
	{

		struct IMulChatWatcher
		{
			virtual ~IMulChatWatcher() {};

			//-----------------------------------------------------------------------------------------------

			//通知UI创建多人会话的结果（成功和失败版本）
			//无论成功/失败，protocol内部都会根据PCS_ApplyCreateMulChatRes的离线用户执行PCS_BuddyStatusChange_toClinet状态修正操作
			//protocol本地判断也可能调用该OnCreateMulChatRes接口

			virtual void OnCreateMulChatRes(
				const uint64_t &uKey,							//创建时附带的KEY，原样返回
				const EMULCHAT_ERRCODE &uResCode				//错误码
				) = 0;
			virtual void OnCreateMulChatRes(
				const uint64_t &uKey,							//创建时附带的KEY，原样返回
				const EMULCHAT_ERRCODE &uResCode,				//返回码，ENUM_MULCHAT_SUCCESS表示成功
				const std::wstring &wstrGuid,					//返回的会话唯一标识，失败时该参数无效
				const std::vector<SWideUserItem> &vecInvitesOnline,	//在线的用户列表
				const std::vector<SWideUserItem> &vecInvitesOffline	//离线的用户列表
				) = 0;

			//通知UI邀请用户加入的结果（成功和失败版本）
			//无论成功/失败，protocol内部都会根据PCS_ApplyInviteMulChatRes的离线用户执行PCS_BuddyStatusChange_toClinet状态修正操作

			virtual void OnInviteMembersRes(
				const std::wstring &wstrGuid,					//会话唯一标识
				const EMULCHAT_ERRCODE &uResCode				//错误码
				) = 0;
			virtual void OnInviteMembersRes(
				const std::wstring &strGuid,					//会话唯一标识
				const EMULCHAT_ERRCODE &uResCode,				//返回码，ENUM_MULCHAT_SUCCESS表示成功
				const std::vector<SWideUserItem> &vecInvitesOnline,	//在线的用户列表
				const std::vector<SWideUserItem> &vecInvitesOffline	//离线的用户列表
				) = 0;

			//通知UI自己被邀请加入多人会话

			virtual void OnInviteMySelf(
				const std::wstring &wstrGuid,					//会话唯一标识	
				const SWideUserItem &stInviter,						//邀请者
				const std::vector<SWideUserItem> &vecMembers		//当前的成员列表
				) = 0;

			//通知UI增加了新成员

			virtual void OnMembersJoin(
				const std::wstring &wstrGuid,					//会话唯一标识	
				const SWideUserItem &stInviter,						//邀请者
				const std::vector<SWideUserItem> &vecInvites,		//新增加的用户列表
				const bool &bCreate									//是否初使化创建标志
				) = 0;

			//通知UI用户离开会话

			virtual void OnMembersLeave(
				const std::wstring &wstrGuid,					//会话唯一标识
				const SWideUserItem &stMember						//离开的用户
				) = 0;

			//当用户数 < 3 时，通知UI删除多人会话，并切换到IM模式

			virtual void OnDelMulChat(
				const std::wstring &wstrGuid,					//会话唯一标识
				const SWideUserItem &stOther						//切换回IM模式的接收人
				) = 0;

			//在用户发消息时，服务器检测到有用户离线，会通知UI
			//protocol内部会执行PCS_BuddyStatusChange_toClinet状态修正操作

			virtual void OnMemberOffline(
				const std::wstring &wstrGuid,					//会话唯一标识
				const std::vector<SWideUserItem> &vecMembersOffline	//离线的用户列表
				) = 0;

			//会话/成员恢复

			virtual void OnMemberResume(
				const std::wstring &wstrGuid,					//会话唯一标识
				const std::vector<SWideUserItem> &vecMembers		//增加的恢复的成员列表
				) = 0;

			//-----------------------------------------------------------------------------------------------

			//通知UI收到多人会话的消息

			virtual void OnMulChatMessage(
				const std::wstring &wstrGuid,					//会话唯一标识
				const SWideUserItem &stFrom,
				const protocol::session::TextChat& text,		//消息内容
				const uint32_t& uSendTime,						//消息发送时间
				const uint32_t& uSeqId,							//消息序列号

				const std::wstring &wstrMsgGuid					//消息GUID（s*1000000+us来表示）
				) = 0;

			//通知UI收到多人会话的消息回应

			virtual void OnMulChatMessageRes(
				const std::wstring &wstrGuid,					//会话唯一标识
				const SWideUserItem &stFrom,						//消息响应人
				const uint32_t &uSeqId							//消息序列号
				) = 0;

			//本人发送消息后，服务器会返回该消息的GUID（s*1000000+us来表示）

			virtual void OnMulChatMessageGuidRes(
				const std::wstring &wstrGuid,					//会话唯一标识
				const uint32_t &uSeqId,							//消息序列号
				const uint32_t& uSendTime,						//消息发送时间
				const std::wstring &wstrMsgGuid					//消息GUID（s*1000000+us来表示）
				) = 0;

			//-----------------------------------------------------------------------------------------------

			//通知UI更改会话主题

			virtual void OnSetMulChatTopic(
				const std::wstring &wstrGuid,					//会话唯一标识
				const std::wstring &wstrTopic					//会话主题
				) = 0;

			//收到成员的状态改变
			virtual void OnMulChatChangeStatus(
				const std::wstring &wstrGuid,					//会话唯一标识
				const uint32_t &uMemberId,						//成员id
				const protocol::E_IMSTATUS &emStatus			//状态
				) {};

			//下面会使用protocol来维护多人会话定时器
			//当自己发言后，会收到所有接收人的消息回应，如果超时还收不到回应的话，向上回调报告
			virtual void OnMulChatMessageFail(
				const std::wstring &wstrGuid,					//会话唯一标识
				const SWideUserItem &stUser,					//响应超时的用户
				const protocol::session::TextChat& objText,		//消息的内容
				const uint32_t &uSeqId							//消息的序列号
				) {};
		};

		struct IMulChatWatcherBase
		{
			virtual ~IMulChatWatcherBase() {};

			virtual void watch(IMulChatWatcher *w) = 0;
			virtual void revoke(IMulChatWatcher *w) = 0;
		};

		class IMulChatManager : public virtual IMulChatWatcherBase
		{
		public:
			virtual ~IMulChatManager() {};

			//设置本人昵称
			//当本人昵称发生改变后，需要再次设置
			virtual void SetMyNick(
				const std::wstring& wstrMyNick
				) = 0;

			//创建多人会话
			virtual void CreateMulChat(
				uint64_t uKey,									//创建的附带的KEY，会在创建成功后原样返回
				const std::vector<SWideUserItem> &vecInvites		//创建时的邀请的用户列表（不包括自己），必须是在线用户
				) = 0;

			//向指定的多人会话添加成员
			virtual void InviteMembers(
				const std::wstring& wstrGuid,					//会话唯一标识
				const std::vector<SWideUserItem> &vecInvites		//新邀请的用户列表（不包括自己），必须是在线用户
				) = 0;

			//离开多人会话（请在关闭YY前，调用本接口）
			virtual void LeaveMulChat(
				const std::wstring& wstrGuid					//会话唯一标识
				) = 0;

			//发送多人会话消息
			virtual void SayToMembers(
				const std::wstring& wstrGuid,					//会话唯一标识
				const protocol::session::TextChat& text,		//消息内容
				uint32_t uSeqId									//消息序列号
				) = 0;

			//设置主题
			virtual void SetTopic(
				const std::wstring& wstrGuid,					//会话唯一标识
				const std::wstring& wstrTopic					//主题
				) = 0;

			//向其他成员广播自己状态
			virtual void ChangeMyStatus(
				const std::wstring& wstrGuid,					//会话唯一标识
				const protocol::E_IMSTATUS& emStatus			//状态
				) = 0;

			//取其他成员的在线状态
			virtual void GetMemberStatus(
				const std::wstring& wstrGuid					//会话唯一标识
				) = 0;
		};
	}
}
