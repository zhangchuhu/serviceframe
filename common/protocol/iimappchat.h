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


			// 通知离线消息记录数
			// mapFromId2Count （key: 发送人， value: 离线记录数）
			virtual void OnArriveOfflineMsgCount(const std::map<uint32_t , uint32_t>& mapFromId2Count) = 0;

			// 离线消息
			virtual void OnArriveOfflineMsg(uint32_t uFrom, uint32_t uFromImId,
				uint32_t uSeqId, uint32_t uSendTime, const protocol::session::TextChat& text) = 0;

			
			// for UI, take action when PSS_ChatMsgSearchRes is received
			/*
			返回消息查询结果
			UID:
			FriendUID: 对端的uid
			nCount: 记录总数
			records: 消息内容
			nTaskID: 上下文
			*/
			/*
			virtual void OnGetFriendMsgRes(
				const uint32_t &nCount, 
				const std::vector<protocol::im::MessageDetail> &records, 
				const uint32_t &nTaskID) = 0;*/

			/*
			响应,返回查询guid的结果
			UID:
			FriendUID: 对端的uid
			nCount: 记录总数
			strGuidStart: guid的上界
			strGuidEnd: guid的下界
			nTaskID: 上下文
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

			/*收到二进制信令通知
			uPeerId: 对端的uid
			uUri: 信令的id(相当于上下文)
			bin:  二进制信令的实体
			*/
			virtual void OnRecvBin(const uint32_t &uPeerId, const uint32_t &uUri, const std::string& bin) = 0;

			IAppChatRegister(uint32_t iAppID):m_iAppID(iAppID){}

			void SetAppID(uint32_t iAppID){if(iAppID < ENUM_APP_MAKE_FRIEND) {throw std::logic_error("appid must large than 17.");} m_iAppID = iAppID;}

			const uint32_t GetAppID() const{return m_iAppID;}

		private:
			uint32_t m_iAppID;
		};

		struct IAppChatWatcher//底层需要实现的接口,供UI调用
		{
			virtual ~IAppChatWatcher(){};
			virtual void watch(IAppChatRegister *w) = 0;
			virtual void revoke(IAppChatRegister *w) = 0;
		};


		class IAppChatBase : public IAppChatWatcher
		{
		public:
			virtual ~IAppChatBase(){}

			//返回消息id(seqid)
			virtual uint32_t SayToPeer(const IAppChatRegister &w, const uint32_t peerid, const protocol::session::TextChat& text) = 0;
/*
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
				const IAppRegister &w,
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
				const IAppRegister &w,
				const protocol::HULI &UID,
				const protocol::HULI &FriendUID,
				const uint32_t lStartTime,
				const uint32_t lEndTime, 
				const UniString &Keyword, 
				const int32_t nPage, 
				const uint32_t nPageSize, 
				const uint32_t nTaskID) = 0;*/


			//删除好友聊天记录
			//UID:用户的UID
			//FriendUID:好友的UID
			//vctGUID:消息GUID
			//nTaskID:上下文的标识
			virtual void DelFriendMsg(
				const IAppChatRegister &w,
				const protocol::HULI &UID,
				const protocol::HULI &FriendUID, std::vector<std::wstring> vctGUID, 
				const uint32_t nTaskID)=0;


			// 获取离线消息的数量
			virtual void GetOfflineMsgCount(const IAppChatRegister &w) = 0;

			// 取离线消息
			virtual void GetOfflineMsg(const IAppChatRegister& w) = 0;

			/*发送二进制信令给对方
			uPeerId: 对方的uid
			uUri: 信令的id(相当于上下文)
			bin:  二进制信令的实体
			*/	
			virtual void SayBinToPeer(const IAppChatRegister& w, const uint32_t &uPeerId, const uint32_t uUri, const std::string& bin) = 0;
		};
	};
};


#endif //__IIMAPPCHAT_H__
