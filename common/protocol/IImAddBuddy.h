#pragma once

#include <vector>
#include <string>
#include "common/unistr.h"
#include "common/protocol/const.h"


//Ϊʵ��IM 2.2 �¹��ܶ���Ľӿ�
namespace protocol
{
	namespace im
	{
		struct IImAddBuddyWatcher
		{
			//UIʵ�ֵĽӿ�
			//ʹ�û���������Ӻ��ѵ�UI�ص�
			virtual void OnReqAddBuddyByScoreRes(const uint32_t& buddyID,const bool& bIsOk,const uint32_t& uScore) = 0;
			//ʹ�ûش�����������Ӻ��ѵ�UI�ص�
			//virtual void OnReqAddBuddyByQuestionRes(const uint32_t& buddyID,const bool& bIsOk) = 0;
			virtual void OnReqAddBuddyByQuestionRes(const uint32_t& buddyID,const bool& bIsOk,const UniString& strBquestion,const UniString& strMyAnswer) = 0;

			//���Ի���/����Ļص�
			virtual void OnTestReqAddBuddyByScoreRes(const uint32_t& buddyID, const bool& bIsOk,const uint32_t& uScore) {}
			virtual void OnTestReqAddBuddyByQuestionRes(const uint32_t& buddyID,const bool& bIsOk, const uint32_t& uQuestionId, const UniString& strBquestion,const UniString& strMyAnswer) {}

			// 3.6 ++
			//���ּӺ�����Ҫ��֤��
			virtual void OnReqAddBuddyByScoreNeedVerify(const E_UI_CHANNEL_ADD_BUDDY_RES_TYPE &resCode, const uint32_t& uBid,
				const std::wstring& valid_key,  const char* const & bin, const uint32_t &size) {};
		};

		struct IImAddBuddyWatchBase 
		{
			virtual void watch(IImAddBuddyWatcher *) = 0;
			virtual void revoke(IImAddBuddyWatcher *) = 0;
		};

		//�ײ�ʵ�ֵĽӿ�
		struct IImAddBuddy:public virtual IImAddBuddyWatchBase
		{
			//��������Ҫͨ���ش�������������Ӻ��ѷ�ʽ
			virtual void ReqAddBuddyByQuestion(uint32_t uId/*������˵�uid*/, uint32_t uQuestionId, const UniString& msg_left)=0;
			//��������Ҫͨ��������֤����Ӻ��ѷ�ʽ
			virtual void ReqAddBuddyByScore(uint32_t uId/*������˵�uid*/) = 0;

			//3.3+ ���Ӳ��Ի����Ƿ�ﵽҪ��ӿ�
			virtual void TestReqAddBuddyByScore(uint32_t uId) {};
			//3.3+ ���Ӳ��������Ƿ���ȷ
			virtual void TestReqAddBuddyByQuestion(uint32_t uId, uint32_t uQuestionId, const UniString& msg_left) {};

			// 3.6 ++ 
			//���Ӵ���֤��ļӺ��ѽӿ�
			virtual void ReqAddBuddyByScoreVerify(uint32_t uId/*������˵�uid*/,
				const UniString& strKey= _TEXT(""), const UniString& strValue =_TEXT("")) {};
		};

	}
}
