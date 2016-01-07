#ifndef MAIN_INC2_H_
#define MAIN_INC2_H_
#include "server_common/server-lib/DaemonServer.h"
#include "server_common/server-lib2/DaemonClient2.h"
#include "server_common/server-lib/DBDaemonClient.h"
#include "core/corelib/MfcAppContext.h"
#include "core/corelib/BackLinkHandler.h"
#include "core/corelib/TServerConnCreator.h"
#include "core/corelib/InnerConn.h"
#include "core/corelib/MultiConnManagerImp.h"
#include "server_common/helper/server_const.h"
#include "server_common/helper/unixdaemon.h"
#include "core/corelib/WrapServerStart.h"
#include "core/corelib/WriterImp.h"
#include "server_common/server-lib/BRouteAppContext.h"
#include "server_common/server-lib/BRouteWriter.h"
#include "server_common/server-lib/id_def.h"
#include "server_common/server-lib/PacketImp.h"
#include "server_common/server-lib/ConfigDaemonServer.h"
#include "server_common/server-lib2/ServerDispatcherFacade.h"
#include "server_common/server-lib2/ServerNameView.h"
#include "server_common/server-lib2/ServerIdView.h"
#include "server_common/server-lib2/ServerConnMgr.h"
#include "server_common/server-lib2/ServerPing.h"
#include "server_common/server-lib2/TNotifyConnManager.h"
#include "server_common/config/TinyXmlServerConfigImp.h"
#include "server_common/config/LoaderContainer.h"

using namespace core;
using namespace server::config;

#define CONFIG_RDAEMON_SERVER_INIT2(ConfigName, AppCtxClz, ConnManagerClz, SConnCreatorClz, CConnCreatorClz, LinkHandlerClz, WriterClz, ConfigClz)  \
	\
	AppCtxClz __appContext;		\
	ConnManagerClz __connManager;	\
	SConnCreatorClz screator;	\
	CConnCreatorClz ccreator; \
	LinkHandlerClz __handler; \
	ConfigDaemonServer __server; \
	DaemonClient2 __clientDaemon;	\
	ServerDispatcherFacade __svidDispatcher; \
	ServerIdNameImp __id2Name; \
	NameViewImp __nameView;	\
	ServerPing __namePing; \
	CacheNameViewImp __cacheNameView; \
	ServerIdViewImp __serverIdView; \
	ServerConnMgr __serverConnMgr; \
	ConnManagerNotifyClose<MultiConnManagerImp> __notifyConnMgr; \
	\
	__clientDaemon.watch(&__serverIdView); \
	__clientDaemon.watch(&__nameView); \
	__clientDaemon.watch(&__cacheNameView); \
	__clientDaemon.watch(&__namePing); \
	__clientDaemon.setServerIdDispatcher(&__svidDispatcher); \
	__clientDaemon.setDaemonConnectWatcher(&__server); \
	__clientDaemon.setServer(&__server); \
	__clientDaemon.watch(&__id2Name); \
	\
	__nameView.setServerIdName(&__id2Name);\
	\
	__cacheNameView.setNameView(&__nameView); \
	__cacheNameView.setServerIdName(&__id2Name); \
	\
	__namePing.watch(&__nameView);\
	__namePing.watch(&__cacheNameView);\
	__namePing.setServerIdDispatcher(&__svidDispatcher); \
	__namePing.setServerConnMgr(&__serverConnMgr); \
	\
	__svidDispatcher.setDaemonClient(&__clientDaemon); \
	__svidDispatcher.setNameView(&__cacheNameView); \
	__svidDispatcher.setServerConnMgr(&__serverConnMgr); \
	__svidDispatcher.setServerIdView(&__serverIdView); \
	\
	__svidDispatcher.addRemoveTO(&__cacheNameView); \
	__svidDispatcher.addRemoveTO(&__serverIdView); \
	__svidDispatcher.addRemoveTO(&__nameView); \
	__svidDispatcher.addRemoveTO(&__namePing); \
	__svidDispatcher.addRemoveTO(&__id2Name); \
	\
	__serverConnMgr.setConnManager(&__notifyConnMgr); \
	__serverConnMgr.setLinkEvent(&__notifyConnMgr); \
	__serverConnMgr.setLinkHandler(&__handler); \
	__serverConnMgr.setServer1(&__server); \
	__serverConnMgr.watch(&__clientDaemon); \
	__serverConnMgr.watch(&__namePing); \
	__serverConnMgr.watch(&__nameView); \
	__serverConnMgr.watch(&__cacheNameView); \
	\
	__notifyConnMgr.watch(&__serverConnMgr); \
	__notifyConnMgr.setClientConnCreator(&ccreator); \
	\
	WriterClz __writer;\
	PacketImp __packet;\
	ConfigClz __serverConfig(ConfigName); \
	server::config::LoaderContainer __loader; \
	\
	__loader.addLoader(&__serverConfig); \
	__loader.loadAll();\
	__connManager.setServerConnCreator(&screator); \
	__connManager.setClientConnCreator(&ccreator); \
	\
	__handler.setAppContext(&__appContext); \
	\
	__server.setConnManager(&__connManager); \
	__server.setLinkHandler(&__handler);  \
	__server.setServerIdDispatcher(&__svidDispatcher);\
	__server.setDaemonClient(&__clientDaemon);\
	__server.setServerConfig(&__serverConfig);\
	__server.setLinkEvent(&__connManager); \
	__server.setGroupId(__serverConfig.getGroupId()); \
	\
	__appContext.setWriter(&__writer); \
	__writer.setServer(&__server); \
	__writer.setServerIdDispatcher(&__svidDispatcher); \
	__writer.setPacketProto(&__packet); \
	\
	__appContext.addEntry(DaemonClient2::getFormEntries(),	 &__clientDaemon, &__clientDaemon); \
	__appContext.addEntry(ConfigDaemonServer::getFormEntries(), &__server, &__server);\
	__appContext.addEntry(ServerPing::getFormEntries(), &__namePing, &__namePing);\





#define DAEMON_SERVER_START2 __server.startSV();


#endif /*MAIN_INC_H_*/

