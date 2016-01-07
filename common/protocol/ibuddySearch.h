#pragma once

#include "common/protocol/iImV2.h"

#include "common/protocol/ibuddylist.h"
#include "common/protocol/ilist.h"
#include "common/iproperty.h"

//wuji start
//吴季鸿定义的接口名字规则如下

//如果是以Asny开头，表明该函数是异步操作函数（即需要回调函数反映函数操作结果)


//wuji end
namespace protocol
{
	namespace im
	{
		struct IBuddySearchWatcher//UI要实现的接口，服务器收到消息后，要回调的UI接口
		{
			virtual ~IBuddySearchWatcher()
			{

			}

			//return the user's basic info and online status
			virtual void onSearchRes(const HULI& hid, const uint32_t& resCode, const protocol::E_IMSTATUS& st) = 0;
			//virtual void onSearchRes(const sox::properties_type& binfo) = 0;

			//用于表示请求被服务器接受(无需服务器回复)
			//virtual void onReqAddBuddyRes(const HULI& bid, const UniString& nickname) = 0;

			//用于表示添加好友请求被对方拒绝或是接收
			virtual void onPeerResp(const HULI& peerid, const E_WAITCHECKLIST_OP& op, const UniString& msg_left) = 0;

			virtual void onReqAddMe(const HULI& peerid, const UniString& left_msg) = 0;

			//wuji start
// 			*  @param hid  要搜索用户的UID。
// 				*  @param resCode  搜索用户的状态码，RES_SUCCESS表示存在，RES_ENONEXIST 表示不存在。
// 				*  @param SecurityCode  返回验证码。留空即为不需要验证码，如果有注册码，就是需要先让用户输入注册码，才能显示搜索的结果。

//			virtual void onSearchRes(const HULI& hid, const uint32_t& resCode, const protocol::E_IMSTATUS& st,const UniString& SecurityCode) = 0;
			virtual void onNeedCode(const UniString& SecurityCode) = 0;//服务器返回需要验证码

// 			/*
			//通过这个回调来确定请求用户的验证类型
// 			  @param buddyID , 所加好友的认证消息类型。通过返回值的不同，界面做不同的显示。*  当style 是问题的时候，msg 才非空，显示问题，其它情况下空。
// 			*/
			
			//Asny_GetBuddyCeLue的回调
			virtual void onBuddyValidateStyle(const  HULI& buddyID,const E_UI_CELUE_OP& uCeLue,const uint32_t& uQuestionId,const UniString& msg)=0;			


			virtual void OnReqAddBuddy_ByJiFenRes(const HULI& buddyID,const bool& bIsOk,const uint32_t& uJiFen) = 0;//使用积分类型添加好友的UI回调

			virtual void OnReqAddBuddy_QuestionRes(const  HULI& buddyID,const bool& bIsOk) = 0;//使用回答问题类型添加好友的UI回调

			virtual void OnReqAddBuddy_ByOpRes(const  HULI& buddyID,const bool& bIsOk) = 0;//使用类型1添加好友的UI回调

			

			//wuji end

			//add by myf

			/*频道添加好友UI回调
			* 对应接口:Asny_ReqChannelAddBuddy
			* @param uResCmd: 返回状态枚举
			* @param bid: 被添加用户的uid
			* @param valid_key: 验证码的Key,  当用户需要输入验证码时,valid_key为非空
			* @param bin: 验证码图片的首地址,当用户需要输入验证码时,bin为非空.
			* @param size: 验证码图片所占内存长度;当用户需要输入验证码时,size为非0
			*/
			virtual void OnChannelAddBuddyRes(const E_UI_CHANNEL_ADD_BUDDY_RES_TYPE &uResCmd, const HULI& bid, const std::wstring& valid_key,  const char* const & bin, const uint32_t &size) = 0;

			/*搜索好友UI回调
			* 对应接口:Asny_searchBuddy2
			* @param resCode: 返回状态枚举
			* @param uid: 被添加用户的uid
			* @param st: 被添加用户的在线状态
			* @param valid_key: 验证码的Key,  当用户需要输入验证码时,valid_key为非空
			* @param bin: 验证码图片的首地址,当用户需要输入验证码时,bin为非空.
			* @param size: 验证码图片所占内存长度;当用户需要输入验证码时,size为非0
			*/
			virtual void onSearchRes2(const E_UI_CHANNEL_ADD_BUDDY_RES_TYPE &resCode, const HULI& uid, const protocol::E_IMSTATUS& st, const std::wstring& valid_key,  const char* const & bin, const uint32_t &size) = 0;
			
			//myf add end

			// 3.6 ++
			//取好友策略需要验证码
			virtual void OnGetBuddyCeLueVerifyRes(const uint32_t &uBid, const E_UI_CELUE_OP& uCeLue, const UniString& msg,
				const std::wstring& valid_key,  const char* const & bin, const uint32_t &size) {};

			//验证加好友需要验证码
			virtual void OnReqAddBuddyNeedVerify(const E_UI_CHANNEL_ADD_BUDDY_RES_TYPE &resCode, const uint32_t& uBid, const uint32_t &uPid, const UniString& msg_left,
				const std::wstring& valid_key,  const char* const & bin, const uint32_t &size) {};
		};

		struct IOpBuddySearchWatcher
		{
			virtual ~IOpBuddySearchWatcher(){}
			virtual void watch(IBuddySearchWatcher *w) = 0;
			virtual void revoke(IBuddySearchWatcher *w) = 0;
		};	

		struct IBuddySearch : public virtual IOpBuddySearchWatcher//底层服务实现的接口,给UI使用
			
		{
			virtual ~IBuddySearch(){}
			virtual void searchBuddy(const UniString& imid) = 0;
			virtual void ReqAddBuddy(const HULI &hid/*被添加人的uid*/, const HULI &pid, const UniString& msg_left) = 0;//策略是需要被添加人认证的添加好友方式

			//wuji start

			/*
			*  扩展接口，增加了一个验证码参数，当用户搜索的次数超过3的时候，就会带有验证码。
			*  用于搜索好友的。回调接口onSearchRes
			*  @param hid, 用户UID
			*/
			virtual void Asny_searchBuddy(const UniString& imid, const UniString& securityCode) = 0;


			virtual void Asny_ReqAddBuddy_ByJiFen(const HULI &hid/*被添加人的uid*/, const HULI &pid) = 0;//策略是需要通过积分认证的添加好友方式
			virtual void Asny_ReqAddBuddy_Question(const HULI &hid/*被添加人的uid*/, const HULI &pid, uint32_t uQuestionId, const UniString& msg_left)=0;//策略是需要通过回答添加人问题的添加好友方式

			virtual void Asny_GetBuddyCeLue(const HULI &hid/*被添加人的uid*/) = 0;//获得被添加人的认证策略


			//wuji end

			//add by myf

			/*
			* 频道内添加好友
			* 回调接口:OnChannelAddBuddyRes
			* @param uid: 被添加者的uid
			* @param strKey: 验证码的Key,  当用户不需要输入验证码时,strKey赋空字符串即可
			* @param strValue: 用户输入的验证码;当用户不需要输入验证码时,strValue赋空字符串即可
			*/
			virtual void	Asny_ReqChannelAddBuddy(const HULI &uid,  const UniString& strKey= _TEXT(""), const UniString& strValue =_TEXT("")) = 0;

			/*
			*  扩展接口，增加了图片验证码参数，当用户搜索的次数超过3的时候，就需要带有验证码。
			*  用于搜索好友的。回调接口onSearchRes2
			*  @param imid: 用户的imid
			*  @param strKey: 验证码的Key,  当用户不需要输入验证码时,strKey赋空字符串即可
			*  @param strValue: 用户输入的验证码;当用户不需要输入验证码时,strValue赋空字符串即可
			*/
			virtual void Asny_searchBuddy2(const UniString& imid, const UniString& strKey= _TEXT(""), const UniString& strValue =_TEXT("")) = 0;

			//myf add end

			//用于添加好友前，先设置备注和分组
			//uBGid 表示分组id
			virtual void SetAddBuddyInfo(uint32_t uBid, uint32_t uBGid, const UniString &wstrRemark) {};

			// 3.6 ++

			//带验证码获取好友策略
			virtual void GetBuddyCeLueVerify(uint32_t uBid) {};
			//带验证码验证加好友
			virtual void ReqAddBuddyVerify(uint32_t uBid, uint32_t uPid, const UniString& msg_left,
				const UniString& strKey= _TEXT(""), const UniString& strValue =_TEXT("")) {};
		};
	}
}