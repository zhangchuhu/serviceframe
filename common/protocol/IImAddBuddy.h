#pragma once

#include <vector>
#include <string>
#include "common/unistr.h"
#include "common/protocol/const.h"


//为实现IM 2.2 新功能定义的接口
namespace protocol
{
	namespace im
	{
		struct IImAddBuddyWatcher
		{
			//UI实现的接口
			//使用积分类型添加好友的UI回调
			virtual void OnReqAddBuddyByScoreRes(const uint32_t& buddyID,const bool& bIsOk,const uint32_t& uScore) = 0;
			//使用回答问题类型添加好友的UI回调
			//virtual void OnReqAddBuddyByQuestionRes(const uint32_t& buddyID,const bool& bIsOk) = 0;
			virtual void OnReqAddBuddyByQuestionRes(const uint32_t& buddyID,const bool& bIsOk,const UniString& strBquestion,const UniString& strMyAnswer) = 0;

			//测试积分/问题的回调
			virtual void OnTestReqAddBuddyByScoreRes(const uint32_t& buddyID, const bool& bIsOk,const uint32_t& uScore) {}
			virtual void OnTestReqAddBuddyByQuestionRes(const uint32_t& buddyID,const bool& bIsOk, const uint32_t& uQuestionId, const UniString& strBquestion,const UniString& strMyAnswer) {}

			// 3.6 ++
			//积分加好友需要验证码
			virtual void OnReqAddBuddyByScoreNeedVerify(const E_UI_CHANNEL_ADD_BUDDY_RES_TYPE &resCode, const uint32_t& uBid,
				const std::wstring& valid_key,  const char* const & bin, const uint32_t &size) {};
		};

		struct IImAddBuddyWatchBase 
		{
			virtual void watch(IImAddBuddyWatcher *) = 0;
			virtual void revoke(IImAddBuddyWatcher *) = 0;
		};

		//底层实现的接口
		struct IImAddBuddy:public virtual IImAddBuddyWatchBase
		{
			//策略是需要通过回答添加人问题的添加好友方式
			virtual void ReqAddBuddyByQuestion(uint32_t uId/*被添加人的uid*/, uint32_t uQuestionId, const UniString& msg_left)=0;
			//策略是需要通过积分认证的添加好友方式
			virtual void ReqAddBuddyByScore(uint32_t uId/*被添加人的uid*/) = 0;

			//3.3+ 增加测试积分是否达到要求接口
			virtual void TestReqAddBuddyByScore(uint32_t uId) {};
			//3.3+ 增加测试问题是否正确
			virtual void TestReqAddBuddyByQuestion(uint32_t uId, uint32_t uQuestionId, const UniString& msg_left) {};

			// 3.6 ++ 
			//增加带验证码的加好友接口
			virtual void ReqAddBuddyByScoreVerify(uint32_t uId/*被添加人的uid*/,
				const UniString& strKey= _TEXT(""), const UniString& strValue =_TEXT("")) {};
		};

	}
}
