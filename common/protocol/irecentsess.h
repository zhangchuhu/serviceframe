#pragma once
#include <list>
#include <vector>
#include "common/unistr.h"
#include "common/int_types.h"

class SItem 
{
public:
	std::vector<UniString>	pids;
	uint32_t				asid;
	uint32_t				sid;
	UniString				nick;
	UniString				owner;
	uint32_t				users;
	bool					bLimit;
	bool					bFolder;
	uint32_t				logoIndex;
	UniString				customLogoURL;	// 用户定制logo的url

public:
	SItem::SItem(): users(0), bFolder(false), logoIndex(0){}
};

namespace protocol{
	namespace slist{
		struct IRecentSessionListWatcher{
			virtual void onRefresh() = 0;
		};

		struct RecentSessionListWatchBase{
			virtual void watch(IRecentSessionListWatcher *) = 0;
			virtual void revoke(IRecentSessionListWatcher *) = 0;
		};

		struct IRecentSessionList: public virtual RecentSessionListWatchBase{
			virtual ~IRecentSessionList(){}
			//有序的列表
			virtual std::list<SItem> getRecentList() = 0;
			//清空最近访问列表
			virtual void clear() = 0;
			//从最近访问列表中删除指定的频道
			virtual void DelFromRecentList(const std::wstring& strId) = 0;
			//从最近访问列表获取频道ip，注意这个ip不会写入文件，如果用户重启YY将不会保留该ip
			virtual std::wstring getRecentIp(const std::wstring &strId) = 0;
		};
	}
}