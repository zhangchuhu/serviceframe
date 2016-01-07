#ifndef ISLIST_H_
#define ISLIST_H_
#include "common/protocol/ilist.h"
#include "common/unistr.h"
#include "common/protocol/const.h"
#include <map>
#include <vector>
#include <string>
#include "isessioninfo.h"

namespace protocol{
namespace slist{
	struct IMyGuildList;
	struct FolderItem{
		UniString sid;
		UniString nick;
		bool operator < (const FolderItem &right) const{
			return sid < right.sid;
		}

		bool operator == (const FolderItem &right) const{
			return sid == right.sid;
		}
	};
	struct SItem2{
		UniString				asid;
		UniString				sid;
		UniString				nick;
		UniString				owner;
		uint32_t				users;
		bool					bLimit;
		uint32_t				logoIndex;
		UniString				customLogoURL;	// 用户定制logo的url
		SItem2():users(0), logoIndex(0), customLogoURL(_T("")){}
	};

	struct ISessionListWatcher{
		typedef std::vector< std::pair<FolderItem, std::vector<SItem2> > >  slist_tree_t;
		virtual void onRefresh(const slist_tree_t &trees) = 0;
		virtual void onFavorAdd(const SItem2 &) = 0;
		virtual void onFavorRemove(const SItem2 &) = 0;
	};

	struct ISessionListWatchBase{
		virtual void watch(ISessionListWatcher *) = 0;
		virtual void revoke(ISessionListWatcher *) = 0;
	};

	struct ISessionList: public ISessionListWatchBase{
		virtual void sync() = 0;
		virtual bool isInMyFavor(const UniString &sid) const= 0;
		/**
			在我的收藏中并且ow不是自己 返回true
		**/
		virtual bool canRemoveFavor(const UniString &sid) const = 0;
		virtual session::ISessionInfoBase *getSessionInfo(const UniString &sid) = 0;
		virtual void resync() = 0;
		virtual int getMyChannelCount() =0;
		virtual void addFavor(const HULI &sid) = 0;
		virtual void removeFavor(const HULI &fid) = 0;
		virtual void setIdle() = 0;
		virtual void wakeUp() = 0;
		virtual IMyGuildList* getMyGuildList() = 0;
	};


}
}
#endif /*ISLIST_H_*/
