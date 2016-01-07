#ifndef DBMAIN_INC2_H_FOR_BACK
#define DBMAIN_INC2_H_FOR_BACK
#include "main_inc2.h"
#include "server_common/dblib/AsyncLinkHandler.h"
#include "server_common/dblib/AsyncDBAppContext.h"
#include "server_common/dblib/AsyncWriter.h"
#include "server_common/dblib/AsyncPacketImp.h"
#include "server_common/config/TinyXmlDbConfigImp.h"
#include "server_common/config/TinyXmlDbConfigImp2.h"
#include "server_common/dblib/IOracleDao.h"
#include "server_common/dblib/OracleConnFactory2.h"


using namespace ::server::db;
using namespace ::server::config;

#define ORACLE_CONFIG_DBDAEMON_SERVER_INIT(ConfigName, AppCtxClz, ConnManagerClz, SConnCreatorClz, CConnCreatorClz, LinkHandlerClz, ConfigClz, DBConfigClz)  \
	\
	AppCtxClz __appContext;		\
	ConnManagerClz __connManager;	\
	SConnCreatorClz screator;	\
	CConnCreatorClz ccreator; \
	LinkHandlerClz __handler; \
	BackLinkHandler __innerHandler;	\
	DaemonClient2 __clientDaemon;	\
	ServerDispatcherFacade __svidDispatcher; \
	ServerIdNameImp __id2Name;\
	NameViewImp __nameView;	\
	ServerPing __namePing; \
	CacheNameViewImp __cacheNameView; \
	ServerIdViewImp __serverIdView; \
	ServerConnMgr __serverConnMgr; \
	ConnManagerNotifyClose<MultiConnManagerImp> __notifyConnMgr; \
	ConfigDaemonServer __server; \
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
	__serverConnMgr.setLinkHandler(&__innerHandler); \
	__serverConnMgr.setServer1(&__server); \
	__serverConnMgr.watch(&__clientDaemon); \
	__serverConnMgr.watch(&__namePing); \
	__serverConnMgr.watch(&__nameView); \
	__serverConnMgr.watch(&__cacheNameView); \
	\
	__notifyConnMgr.watch(&__serverConnMgr); \
	__notifyConnMgr.setClientConnCreator(&ccreator); \
	\
	__innerHandler.setAppContext(&__appContext); \
	\
	AsyncWriter __writer; \
	AsyncPacketImp __packet; \
	ConfigClz __serverConfig(ConfigName); \
	DBConfigClz __dbConfig; \
	::server::config::LoaderContainer __loader; \
	\
	__loader.addLoader(&__serverConfig); \
	__loader.addLoader(&__dbConfig); \
	__loader.loadAll();\
	\
	__connManager.setServerConnCreator(&screator); \
	__connManager.setClientConnCreator(&ccreator); \
	\
	__handler.setAppContext(&__appContext); \
	__handler.setWorkerCount(20); \
	\
	__server.setConnManager(&__connManager); \
	__server.setLinkHandler(&__handler);  \
	__server.setServerIdDispatcher(&__svidDispatcher);\
	__server.setDaemonClient(&__clientDaemon);\
	__server.setServerConfig(&__serverConfig);\
	__server.setLinkEvent(&__connManager); \
	__server.setGroupId(__serverConfig.getGroupId()); \
	\
	__writer.setServer(&__server); \
	__writer.setServerIdDispatcher(&__svidDispatcher); \
	__writer.setPacketProto(&__packet);\
	\
	__appContext.setWriter(&__writer); \
	__appContext.setDbConfig(&__serverConfig);\
	\
	__appContext.addEntry(DaemonServer::getFormEntries(), &__server, &__server);	\
	__appContext.addEntry(DaemonClient2::getFormEntries(),	 &__clientDaemon,	 &__clientDaemon); \
	__appContext.addEntry(ServerPing::getFormEntries(), &__namePing, &__namePing);\


#define ORACLE_CONFIG_DBDAEMON_SERVER_INIT2(ConfigName, AppCtxClz, ConnManagerClz, SConnCreatorClz, CConnCreatorClz, LinkHandlerClz, ConfigClz, DBConfigClz, ConfigFile)  \
	\
	AppCtxClz __appContext;		\
	ConnManagerClz __connManager;	\
	SConnCreatorClz screator;	\
	CConnCreatorClz ccreator; \
	LinkHandlerClz __handler; \
	BackLinkHandler __innerHandler;	\
	DaemonClient2 __clientDaemon;	\
	ServerDispatcherFacade __svidDispatcher; \
	ServerIdNameImp __id2Name;\
	NameViewImp __nameView;	\
	ServerPing __namePing; \
	CacheNameViewImp __cacheNameView; \
	ServerIdViewImp __serverIdView; \
	ServerConnMgr __serverConnMgr; \
	ConnManagerNotifyClose<MultiConnManagerImp> __notifyConnMgr; \
	ConfigDaemonServer __server; \
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
	__serverConnMgr.setLinkHandler(&__innerHandler); \
	__serverConnMgr.setServer1(&__server); \
	__serverConnMgr.watch(&__clientDaemon); \
	__serverConnMgr.watch(&__namePing); \
	__serverConnMgr.watch(&__nameView); \
	__serverConnMgr.watch(&__cacheNameView); \
	\
	__notifyConnMgr.watch(&__serverConnMgr); \
	__notifyConnMgr.setClientConnCreator(&ccreator); \
	\
	__innerHandler.setAppContext(&__appContext); \
	\
	AsyncWriter __writer; \
	AsyncPacketImp __packet; \
	ConfigClz __serverConfig(ConfigName); \
	DBConfigClz __dbConfig; \
	::server::config::LoaderContainer __loader(ConfigFile); \
	\
	__loader.addLoader(&__serverConfig); \
	__loader.addLoader(&__dbConfig); \
	__loader.loadAll();\
	\
	__connManager.setServerConnCreator(&screator); \
	__connManager.setClientConnCreator(&ccreator); \
	\
	__handler.setAppContext(&__appContext); \
	__handler.setWorkerCount(20); \
	\
	__server.setConnManager(&__connManager); \
	__server.setLinkHandler(&__handler);  \
	__server.setServerIdDispatcher(&__svidDispatcher);\
	__server.setDaemonClient(&__clientDaemon);\
	__server.setServerConfig(&__serverConfig);\
	__server.setLinkEvent(&__connManager); \
	__server.setGroupId(__serverConfig.getGroupId()); \
	\
	__writer.setServer(&__server); \
	__writer.setServerIdDispatcher(&__svidDispatcher); \
	__writer.setPacketProto(&__packet);\
	\
	__appContext.setWriter(&__writer); \
	__appContext.setDbConfig(&__serverConfig);\
	\
	__appContext.addEntry(DaemonServer::getFormEntries(), &__server, &__server);	\
	__appContext.addEntry(DaemonClient2::getFormEntries(),	 &__clientDaemon,	 &__clientDaemon); \
	__appContext.addEntry(ServerPing::getFormEntries(), &__namePing, &__namePing);\


#define ORACLE_CONFIG_BRANCHDBDAEMON_SERVER_INIT(ConfigName, AppCtxClz, ConnManagerClz, SConnCreatorClz, CConnCreatorClz, LinkHandlerClz, ConfigClz, DBConfigClz)  \
	\
	AppCtxClz __appContext;		\
	ConnManagerClz __connManager;	\
	SConnCreatorClz screator;	\
	CConnCreatorClz ccreator; \
	LinkHandlerClz __handler; \
	BackLinkHandler __innerHandler;	\
	DaemonClient2 __clientDaemon;	\
	ServerDispatcherFacade __svidDispatcher; \
	ServerIdNameImp __id2Name;\
	NameViewImp __nameView;	\
	ServerPing __namePing; \
	CacheNameViewImp __cacheNameView; \
	ServerIdViewImp __serverIdView; \
	ServerConnMgr __serverConnMgr; \
	ConnManagerNotifyClose<MultiConnManagerImp> __notifyConnMgr; \
	ConfigDaemonServer __server; \
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
	__serverConnMgr.setLinkHandler(&__innerHandler); \
	__serverConnMgr.setServer1(&__server); \
	__serverConnMgr.watch(&__clientDaemon); \
	__serverConnMgr.watch(&__namePing); \
	__serverConnMgr.watch(&__nameView); \
	__serverConnMgr.watch(&__cacheNameView); \
	\
	__notifyConnMgr.watch(&__serverConnMgr); \
	__notifyConnMgr.setClientConnCreator(&ccreator); \
	\
	__innerHandler.setAppContext(&__appContext); \
	\
	AsyncWriter __writer; \
	RouteIgnorPacketImp __packet;\
	ConfigClz __serverConfig(ConfigName); \
	DBConfigClz __dbConfig; \
	server::config::LoaderContainer __loader; \
	\
	__loader.addLoader(&__serverConfig); \
	__loader.addLoader(&__dbConfig); \
	__loader.loadAll();\
	\
	__connManager.setServerConnCreator(&screator); \
	__connManager.setClientConnCreator(&ccreator); \
	\
	__handler.setAppContext(&__appContext); \
	__handler.setWorkerCount(20); \
	\
	__server.setConnManager(&__connManager); \
	__server.setLinkHandler(&__handler);  \
	__server.setServerIdDispatcher(&__svidDispatcher);\
	__server.setDaemonClient(&__clientDaemon);\
	__server.setServerConfig(&__serverConfig);\
	__server.setLinkEvent(&__connManager); \
	__server.setGroupId(__serverConfig.getGroupId()); \
	\
	__writer.setServer(&__server); \
	__writer.setServerIdDispatcher(&__svidDispatcher); \
	__writer.setPacketProto(&__packet);\
	\
	__appContext.setWriter(&__writer); \
	__appContext.setDbConfig(&__serverConfig);\
	\
	__appContext.addEntry(DaemonServer::getFormEntries(), &__server, &__server);	\
	__appContext.addEntry(DaemonClient2::getFormEntries(),	 &__clientDaemon,	 &__clientDaemon); \
	__appContext.addEntry(ServerPing::getFormEntries(), &__namePing, &__namePing);\


#define ORACLE_TEMPLATE_INIT(OracleName, TemplateName) \
	OraSqlConnFactory2 TemplateName##_sqlFactory;	\
	TemplateName##_sqlFactory.setDbName(OracleName); \
	TemplateName##_sqlFactory.setDbConfig(&__dbConfig);\
	TemplateName##_sqlFactory.createPool();	\
	OracleHelperTempalte TemplateName;	\
	TemplateName.setOracleConnFactory(&TemplateName##_sqlFactory);	

#endif /*DBMAIN_INC_H_*/
