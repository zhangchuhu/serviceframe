#pragma once
#include <list>
#include "common/unistr.h"
#include "common/int_types.h"
#include "ChannelRoler.h"

class MItem 
{
public:
	bool		bLimit;
	bool		bFolder;
	bool		bSettle;
	uint32_t	asid;
	uint32_t	sid;
	uint32_t	users;
	uint32_t	logoIndex;
	UniString	customLogoURL;
	UniString	nick;
	UniString	owner;
	protocol::session::ChannelRoler	uRoler;
public:
	MItem::MItem()
		: bLimit(false), bFolder(false), bSettle(false), asid(0)
		, sid(0), users(0), logoIndex(0), customLogoURL(_T(""))
		, nick(_T("")), owner(_T("")), uRoler(protocol::session::NORMAL) 
	{
	}
	bool operator < (const MItem& dest) const
	{
		return (uRoler < dest.uRoler || (uRoler == dest.uRoler && nick < dest.nick));
	}
};

namespace protocol
{
	namespace slist
	{
		struct IMyGuildListWatcher
		{
			virtual void onRefresh() = 0;
		};

		struct MyGuildListWatchBase
		{
			virtual void watch(IMyGuildListWatcher *) = 0;
			virtual void revoke(IMyGuildListWatcher *) = 0;
		};

		struct IMyGuildList
			: public virtual MyGuildListWatchBase
		{
			virtual ~IMyGuildList(){}

			virtual void getMyGuildList(std::list<MItem>& myGuildList) = 0;
		};
	}
}