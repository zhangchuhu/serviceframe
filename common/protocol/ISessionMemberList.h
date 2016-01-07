#pragma once
#include "common/int_types.h"
#include "ilist.h"
#include "const.h"
#include "ChannelRoler.h"

#define MAX_CHANNEL_NUMBER_LIMIT 2001

namespace protocol{
	namespace session{
		struct IUserGuild;

		struct ISessionMemberList 
			: public IListBaseEx
			, public virtual IOperatorWatchBase
		{
			virtual bool isLimit() const = 0;
			virtual bool hasPasswd(const UniString &subId) const = 0;
			virtual bool hasSessMgmtPasswd() const = 0;
			virtual uinfo::SEX getSex(const UniString& uid) = 0;
			virtual UniString getSign(const UniString &uid) = 0;
			virtual void changeFolder(const UniString &from, const UniString &to, const std::string &passwd) = 0;
			virtual void changeTo(const UniString &to, const std::string &passwd) = 0;
			virtual ChannelRoler getRoler(const UniString &hid, const UniString &channelId) = 0;
			virtual void changeMemberRight(const UniString &uid, ChannelRoler nr, const UniString &channelId) = 0;

			virtual void kickOff(const HULI & sid, const HULI &uid, const UniString& reason, uint32_t minutes) = 0;
			virtual void tuoren(const HULI &who, const HULI &from, const HULI &to) = 0;

			virtual void enableText(const UniString &uid, bool bEnable) = 0;
			virtual void enableVoice(const UniString &uid, bool bEnable) = 0;

			virtual bool isUserTextable(const UniString &uid) = 0;
			virtual bool isUserVoiceable(const UniString &uid) = 0;
			virtual bool isBanUserChat(const UniString& uid) = 0;
			virtual bool canGuestVoiceChat(const UniString& uid) = 0;
			virtual bool canVoiceOpt(const UniString &who,const UniString &target) = 0;

			virtual void pushToMicrophoneList(const UniString& uid) = 0;

			virtual void setKeyActive(const UniString &uid) = 0;

			virtual uint32_t getUserJF(const UniString &uid) =0;
			virtual uint32_t getUserSessJF(const UniString &uid) =0;

			virtual uint32_t getUserCountBySid(const UniString &sid) const = 0;

			virtual void enableAllText(const UniString &channelId, bool bEnable) = 0;

			virtual void banID(const UniString &uid, const UniString& reason) = 0;
			virtual void banIP(const UniString &uid) = 0;
			virtual void banPC(const UniString& uid, const UniString& reason) = 0;

			virtual void setAllKeyActive(const UniString &channelId) = 0;

			virtual void ignoreText(const UniString &uid,bool ignore) = 0;
			virtual void ignoreVoice(const UniString &uid,bool ignore) = 0;

			virtual void leaveGuild() = 0;

			virtual void directKickoff(const HULI & sid, const HULI &uid, const UniString& reason, uint32_t minutes) = 0;

			virtual std::vector<std::pair<uint32_t, uint32_t> > getChannelTree() const = 0;

			virtual IUserGuild *getUserGuilds(const UniString &uid) = 0;

			virtual bool isIgnoreText(const UniString &uid) = 0;
			virtual bool isIgnoreVoice(const UniString &uid) = 0;

			//virtual bool isUserTextableInCh(const UniString &uid) = 0;
			virtual bool isUserTextableInCh(const UniString &uid, const UniString &chId) = 0;

			virtual bool isChannelNumberOverFlow() const = 0;

			/**
			 * return sub channel's order
			 *
			 * @param sid	channel id
			 * @return =0	channel not sorted
			 *		   >0	channel's order
			 */
			virtual int getOrder(const HULI& sid) const = 0;
			/*
				是否常驻
			*/
			virtual bool isSettle(const HULI& sid) const = 0;

			// 用户当前游戏信息相关
			virtual void UpdateMyCurrentGame(const HULI& strGameId) = 0;
			virtual int GetCurrentPlayGame(const HULI& hid) = 0;
			virtual void UpdateMyHistoryGame(const HULI& strGameId) = 0;
			virtual int GetHistoryPlayGame(const HULI& hid) = 0;

			//设置接待频道，bset为true set; false remove
			virtual void setJiedai(const HULI &channelId, bool bset) = 0;
			virtual bool isJiedai(const HULI &channelId) = 0;
			virtual bool hasJiedai(const HULI &channelId) = 0;

			/**
			 * setChannelTextStatus的status目前可以赋3个值，1表示允许所有人打字，2表示禁止所有人打字，3表示禁止游客打字
			 * getChannelTextStatus的返回值含义和setChannelTextStatus的status一样
			 */
			virtual void setChannelTextStatus(const HULI &channelId, uint32_t status) = 0;
			virtual uint32_t getChannelTextStatus(const HULI &channelId) = 0;

			//第三方图标（俗称尾灯）相关，type目前有两种分类，0表示游戏，1表示活动
			virtual void SetCurrentThirdPartyIcon(uint32_t type, const HULI& strId) = 0;
			virtual void SetHistoryThirdPartyIcon(uint32_t type, const HULI& strId) = 0;
			virtual void setThirdPartyIcon(const HULI& curGame, const HULI& oldGame,
										   const HULI& curAction, const HULI& oldAction) = 0;
			
			/**
			 * 设置指定类型的第三方图标
			 * @param type 类型，目前有1种分类，2表示imid
			 * @param strId 对应类型的id
			 */
//			virtual void SetThirdPartyIcon(uint32_t type, const HULI& strId) = 0;
			
			/**
			 * 获得指定类型的第三方图标
			 * @param type 类型
			 * @param hid 用户uid
			 * @return first 当前第三方图标的id，second 历史第三方图标的id。如果不存在，则返回值为-1
			 */
			virtual std::pair<int, int> GetThirdPartyIcon(uint32_t type, const HULI& hid) = 0;

			virtual void Watch(IMiscWatcher* ) = 0;

			virtual void Revoke(IMiscWatcher* ) = 0;
		};
	}
}
