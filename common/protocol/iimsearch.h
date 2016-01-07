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
			time_t uDateTime;		// 日期和时间
			UniString strFromId;			//发送者uid
			UniString strToId;				//接收者uid
			protocol::session::TextChat MsgContent;		//消息内容
			UniString strGuid;				//消息的GUID
			uint32_t uImid; //yy号
		};

		//带群ID的消息版本
		struct MessageDetailGrp
		{
			time_t uDateTime;		// 日期和时间
			UniString strFromId;			//发送者uid
			UniString strToId;				//接收者uid
			protocol::session::TextChat MsgContent;		//消息内容
			UniString strGuid;				//消息的GUID
			uint32_t uImid; //yy号
			uint32_t uGrpId; //群ID
		};

		struct IImSearchWatcher
		{
			virtual ~IImSearchWatcher(){};

			// for UI, take action when PSS_ChatMsgSearchRes is received
			/*
			返回消息查询结果
			UID:
			FriendUID: 对端的uid
			nCount: 记录总数
			records: 消息内容
			nTaskID: 上下文
			*/
			virtual void OnGetFriendMsgRes(
				const uint32_t &nCount, 
				const std::vector<MessageDetail> &records, 
				const uint32_t &nTaskID) = 0;

			//带群ID的消息版本
			virtual void OnGetFriendMsgGrpRes(
				const uint32_t &nCount, 
				const std::vector<MessageDetailGrp> &records, 
				const uint32_t &nTaskID) {}

			/*
			响应,返回查询guid的结果
			UID:
			FriendUID: 对端的uid
			nCount: 记录总数
			strGuidStart: guid的上界
			strGuidEnd: guid的下界
			nTaskID: 上下文
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

		struct IOpImSearchWatcher//底层需要实现的接口,供UI调用
		{
			virtual ~IOpImSearchWatcher(){};
			virtual void watch(IImSearchWatcher *w) = 0;
			virtual void revoke(IImSearchWatcher *w) = 0;
		};

		struct IImSearch : public virtual IOpImSearchWatcher
		{
			virtual ~IImSearch(){};

			//查询好友聊天记录条数和区间Index
			//UID：用户的UID
			//FriendUID：好友的UID
			//lStartTime: 开始时间
			//lEndTime: 结束时间
			//nPage：第几页
			//nPageSize：每页大小
			//nTaskID:上下文的标识
			//   异步返回符合的条数,返回符合条件区域的两个Index(GUID)
			virtual void QueryFriendMsgIndex(
				const protocol::HULI &UID,
				const protocol::HULI &FriendUID,
				const uint32_t lStartTime, 
				const uint32_t lEndTime, 
				const int32_t nPage, 
				const uint32_t nPageSize, 
				const uint32_t nTaskID) =0;

			//获取好友聊天记录
			//UID：用户的UID
			//FriendUID：好友的UID
			//lStartTime: 开始时间
			//lEndTime: 结束时间
			//Keyword: 关键字
			//nPage：第几页
			//nPageSize：每页大小
			//nTaskID:上下文的标识
			//异步返回用户信息,用vector返回
			virtual void GetFriendMsg (
				const protocol::HULI &UID,
				const protocol::HULI &FriendUID,
				const uint32_t lStartTime,
				const uint32_t lEndTime, 
				const UniString &Keyword, 
				const int32_t nPage, 
				const uint32_t nPageSize, 
				const uint32_t nTaskID) = 0;

			//带群ID的消息版本
			virtual void GetFriendMsgGrp (
				const protocol::HULI &UID,
				const protocol::HULI &FriendUID,
				const uint32_t lStartTime,
				const uint32_t lEndTime, 
				const UniString &Keyword, 
				const int32_t nPage, 
				const uint32_t nPageSize, 
				const uint32_t nTaskID) {}; //暂时定义空函数

			//删除好友聊天记录
			//UID:用户的UID
			//FriendUID:好友的UID
			//vctGUID:消息GUID
			//nTaskID:上下文的标识
			virtual void DelFriendMsg(const protocol::HULI &UID,
				const protocol::HULI &FriendUID, std::vector<std::wstring> vctGUID, 
				const uint32_t nTaskID)=0;

		};
	}
}
#endif
