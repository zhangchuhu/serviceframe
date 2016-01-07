#pragma once
#include <list>
#include "common/int_types.h"
#include "common/protocol/ilist.h"
#include "common/unistr.h"

#include "common/protocol/IImUInfo.h"

namespace protocol
{
	namespace im
	{

		//黑名单的UI实现的接口是使用IImListWatcher和IImBlackListUInfoWatcher

		//黑名单接口
		struct IBlackList:public virtual IImListWatchBase//底层实现的接口
		{
			//将好友拉到黑名单
			virtual void Ansy_MoveToBlackList(const HULI& strBuddyId ) = 0;
			//将好友从黑名单删除
			virtual void Ansy_DeleteFromBlackList(const HULI& strBuddyId) = 0;
			virtual void Ansy_GetBackList()=0;//取回黑名单列表

			virtual void GetBackList(std::list<uint32_t>& lstData)=0;//取回黑名单列表
			virtual void GetBackList(std::list<HULI>& lstData)=0;//取回黑名单列表
			virtual bool IsInBlackList(const HULI& strBuddyId ) = 0;//true代表在

			//IM 2.2新加入功能
			//将好友拉到黑名单
			virtual void MoveToBlackList(uint32_t uBid ) = 0;
			//将好友从黑名单删除
			virtual void DeleteFromBlackList(uint32_t uBid,bool bIsOnlyDel/*true代表在黑名单中加好友，false代表删除黑名单*/)=0;
		};
		
		struct IImBlackListUInfoWatcher
		{
			//UI实现的接口
			virtual void OnSimpleChange(const std::vector<CImUserInfoSimpleClientData>& vecData) = 0;
		};

		struct IImBlackListUInfoWatchBase 
		{
			virtual void watch(IImBlackListUInfoWatcher *) = 0;
			virtual void revoke(IImBlackListUInfoWatcher *) = 0;
		};

		//底层实现的接口
		struct IImBlackListUInfo:public virtual IImBlackListUInfoWatchBase 
		{
				virtual void GetUinfo()=0;
		};


	}

}
