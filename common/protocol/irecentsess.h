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
	UniString				customLogoURL;	// �û�����logo��url

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
			//������б�
			virtual std::list<SItem> getRecentList() = 0;
			//�����������б�
			virtual void clear() = 0;
			//����������б���ɾ��ָ����Ƶ��
			virtual void DelFromRecentList(const std::wstring& strId) = 0;
			//����������б��ȡƵ��ip��ע�����ip����д���ļ�������û�����YY�����ᱣ����ip
			virtual std::wstring getRecentIp(const std::wstring &strId) = 0;
		};
	}
}