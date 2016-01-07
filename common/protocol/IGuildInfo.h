#pragma  once
#include "iaction.h"
#include "psessionbase.h"
#include "common/unistr.h"

#include <vector>

namespace protocol{
	namespace session{
struct GInfo{
	UniString channelId;
	UniString channelName;
	ChannelRoler roler;
	UniString realSid;
	bool isSettle;			//是否常驻会员
	uinfo::SEX sex;
	uint32_t jifen;
};

struct IGuildWatcher{
	virtual void onRefresh() = 0;
	virtual void onSetSettleFail() = 0;
};

struct IGuildWatchBase{
	virtual void watch(IGuildWatcher *w) = 0;
	virtual void revoke(IGuildWatcher *) = 0;
};
struct IGuildInfo:	public virtual IGuildWatchBase, 
					public virtual IOperatorWatchBase{
	virtual void getGInfos(std::vector<GInfo> &)= 0;
	
	virtual void leaveGuild(const UniString &sid) = 0;

	virtual bool canLeave(const UniString &sid) = 0;

	virtual UniString getUid() = 0;
	
	/*设置sid为常驻工会*/
	virtual void setSettle(const UniString &sid) = 0;
	/*能否设置sid为常驻工会*/
	virtual bool canSetSettle(const UniString &sid) = 0;
	/*本月是否已经设置了常驻工会*/
	virtual bool isSettleCooldown() = 0;
	//virtual void release() = 0;
};

struct IGuildInfoManager{
	virtual IGuildInfo *getGuildInfo(const UniString &uid) = 0;
	//virtual void releaseGuildInfo(IGuildInfo *service) = 0;
	//			virtual void setOwner(const std::string &who) = 0;
};
	}
}