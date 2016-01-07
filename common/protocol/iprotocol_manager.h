#ifndef IProtocolManager_Protocol_H
#define IProtocolManager_Protocol_H
#include "common/unistr.h"



namespace core{
	struct IClientConnFactory;
	struct ILinkHandler;
}
namespace protocol {
	namespace login{
		struct ILogin;
		struct ICheckVersion;
	}
	namespace conf{
		struct IConfigManager;
	}
	namespace uinfo{
		struct IUInfo;
		struct IApplyGuild;
		struct IApplyGuildList;
		//struct IUInfoFactory;
	}
	namespace session
	{
		struct ISessionManager;
		struct IGuildInfoManager;
		struct ILeaveGuild;
	};
	namespace slist{
		struct ISessionList;
		struct IRecentSessionList;
	}
	namespace qlist{
		struct ISessionQList;
	}

	namespace catalog{
		struct ICatalogList;
	}

	namespace im{
		struct IBuddyList;
		struct IWaitCheckList;
		struct IRecentConnList;
		struct IBuddySearch;
		struct IRecentConnList;
		struct IIMChat;
		class IImLogin;
		class IImLBS;
		//wuji start
		struct IBlackList;
		struct IBlockList ;
		struct IImPlayIM ;
		struct IImChanelSet ;
		struct IImSet ;
		struct IImOtherSet ;
		struct IImUInfo;
		struct IImIdInfo;
		struct IImHeadPhotoInfo;
		struct IImAddBuddy;
		struct IImSearch;
		struct IImBlackListUInfo;


		//wuji end
	}
		

	//wuji start
	namespace goffline
	{
		struct IGofflineAction;
	}

	namespace gofflinemsg
	{
		struct IGofflineMsg;
	}

	namespace gmemberinfo
	{
		struct IGmemberInfo;
	}

	// added by linyue
	// iginfo.h is not included. so add a forward definition here.
	namespace ginfo
	{
		struct IGInfo;
	}
	namespace imapp
	{	
		class IAppChatBase;
		class IAppJyFansBase;
	}
	//added by long
	namespace gcheck
	{
		class IGOwner;
	}

	namespace gactions
	{
		struct IGroupActions;
	}

	namespace glist{
		struct IGroupList;
	} // luowl

	// xiangjie add
	namespace gsearch
	{
		struct IGSearch;
	}
	namespace search
	{
		struct ISearch;
	}
	
	namespace sysmsg{
		struct ISysMsg;
	}
	
	// add by linjp
	namespace gtopic
	{
		struct IGTopic;
	}

	//add by kdjie
	namespace ginfo2
	{
		class IGInfoManager;
	}
	namespace goffline2
	{
		class IGOffline;
	}
	namespace im
	{
		class IMulChatManager;
	}
	//add end
	namespace p2p
	{
		class IP2PInterface;
	}

	namespace pip
	{
		struct IPipManager;
	}

	// add by grj
	namespace gprops{
		class IGProperty;
	}

	namespace gmsgcache{
		class IGTopicMsgCache;
	}

	namespace imgame{
		class IBottomInfo;
	}

	namespace store{
		struct IStoreGTopic;
	}

	namespace gmsgcache{
		class IGChatMsgCache;
	}

	namespace gmsgcache{
		class IGTopicMsgManager;
	}

	namespace imreqtimeout
	{
		class IImReqTimeout;
	}
	// end

	//add by kdjie
	namespace gtransfer
	{
		class IGChat;
	}
	namespace glistcache
	{
		class IGUserStats;
	}
	//add end

	namespace imvoice
	{
		class IImVoiceManagerMsg;
	}

	namespace imtroubleshotstats
	{
		struct IImTroubleShot;
	} 

	namespace imapp
	{
		class IAppLogin;
	}

	struct IProxy;
	
	struct IProtocolManager{
		virtual login::ICheckVersion *getCheckVersion() = 0;
		virtual login::ILogin* getLogin() = 0;
		virtual conf::IConfigManager* getConfigManager() = 0;
		virtual uinfo::IUInfo* getUInfo(const UniString &uid) = 0;
		virtual ginfo::IGInfo* getGInfo() = 0; //added by linyue
		virtual gactions::IGroupActions *getGroupActions() = 0;
#ifdef HAS_GTOPIC
		virtual gtopic::IGTopic* getGTopic()=0; // add by LJP
#endif
		//????应该不用后缀
		virtual UniString whoAreU() = 0;
		virtual slist::ISessionList* getSessionList(void) = 0;
		virtual qlist::ISessionQList * getSessionQList() = 0;
		virtual session::ISessionManager* getSessionManager(void) = 0;
		virtual session::IGuildInfoManager *getGuildManager() = 0;
		virtual catalog::ICatalogList *getCatalogList() = 0;
		virtual im::IBuddyList *getBuddyList() = 0;
		virtual im::IWaitCheckList *getWatiCheckList() = 0;
		virtual im::IRecentConnList *getRecentConnList() = 0;
		virtual im::IIMChat *getImChat() = 0;
		virtual im::IBuddySearch *getSearchBuddy() = 0;
		virtual im::IImLogin *getImLogin() = 0;
		//added by zzw , 20091014
		virtual im::IImLBS *getImLbsLogin() = 0;
		//end
		virtual sysmsg::ISysMsg *getSysMsg() = 0;
		//wuji start
		virtual im::IBlackList *getBlackList()=0;
		virtual im::IImBlackListUInfo *getBlackListUinfo()=0;
		virtual im::IBlockList *getBlockList()=0;
		virtual im::IImPlayIM *getWorkLogin()=0;
		virtual im::IImChanelSet *getWorkChannel()=0;
		virtual im::IImSet *getWorkSet()=0;
		virtual im::IImOtherSet *getWorkOther()=0;
		virtual im::IImAddBuddy *getImAddBuddy()=0;
		virtual im::IImSearch *getImSearch() = 0;

		virtual goffline::IGofflineAction *getGofflineAction()=0;
		virtual gofflinemsg::IGofflineMsg *getGofflineMsg()=0;
		virtual gmemberinfo::IGmemberInfo *getGmemberInfo()=0;

		virtual im::IImUInfo *getImUInfo()=0;
		virtual im::IImIdInfo *getImIdInfo()=0;
		virtual im::IImHeadPhotoInfo *getImHeadPhotoInfo()=0;





		//wuji end
		//virtual uinfo::IUInfoFactory *getUInfoFactory(const UniString &uid) = 0;
		// xiangjie add
		virtual gsearch::IGSearch *getGSearch() = 0;
		virtual search::ISearch *getSearch() = 0;

		//add by kdjie
		virtual im::IMulChatManager *GetMulChatManager() = 0;
		//add end

		virtual void init(core::IClientConnFactory *f = NULL) = 0;
		virtual void releaseProtocol() = 0;
		virtual bool isMe(const UniString &h) = 0;
		//virtual void release() = 0;

		virtual protocol::IProxy *getProxy(void) = 0;
		virtual slist::IRecentSessionList *getRecentSessionList() = 0;
		virtual p2p::IP2PInterface *GetP2P() = 0;
		virtual pip::IPipManager *getPipManager() = 0;

		//add by kdjie, 2010.1.22
		virtual gtransfer::IGChat *GetGChat() = 0;
		virtual glistcache::IGUserStats *GetGUserStats() = 0;
		//add end

		// add by grj
#ifdef HAS_GTOPIC
		virtual protocol::gmsgcache::IGTopicMsgCache *GetTopicMsgCache() = 0;
#endif
		//add by lijin
		virtual protocol::store::IStoreGTopic *GetStoreGTopic() = 0;

		virtual protocol::gmsgcache::IGChatMsgCache *GetChatMsgCache() = 0;

		virtual protocol::imreqtimeout::IImReqTimeout *GetImReqTimeout() = 0;

		virtual protocol::gprops::IGProperty *GetGProperty() = 0;
		//

		// add by lbc
		virtual protocol::gmsgcache::IGTopicMsgManager *GetTopicMsgManager() = 0;

		virtual protocol::imgame::IBottomInfo *GetBottomInfo() = 0;

		// add by long
		virtual gcheck::IGOwner *getGOwner() = 0;

		// ********* for group list ************************
		// return the interface of group list. // luowl
		virtual glist::IGroupList * getGrpListHandler() = 0;

		virtual protocol::uinfo::IApplyGuild *getApplyGuild() = 0;

		virtual protocol::uinfo::IApplyGuildList *getApplyGuildList() = 0;

		virtual protocol::session::ILeaveGuild *getLeaveGuild() = 0;

		// 一对一语音
		virtual imvoice::IImVoiceManagerMsg* GetImVoiceManagerMsg() = 0;

		//im对外聊天应用
		virtual protocol::imapp::IAppChatBase* GetImappChat() = 0;

		//提供给UI: 上报失败统计的接口(截图失败,...)
		virtual imtroubleshotstats::IImTroubleShot* GetImTroubleShot(void) = 0;

		//对外应用--交友粉丝系统
		virtual protocol::imapp::IAppJyFansBase* GetJyAppFans() = 0;

		virtual protocol::imapp::IAppLogin* GetImAppLogin() = 0;
	};
}

#endif
