#pragma once
#include "im_inc.h"
#include "server_common/dblib/AsyncLinkHandler.h"
#include "server_common/dblib//AsyncSqlConnFactory.h"
#include "server_common/dblib/AsyncDBAppContext.h"
#include "server_common/dblib/AsyncWriter.h"
#include "server_common/dblib/AsyncPacketImp.h"
#include "server_common/dblib/MysqlConnCreator.h"
#include "server_common/config/TinyXmlDbConfigImp.h"
#include "server_common/config/TinyXmlDbConfigImp2.h"
#include "server_common/dblib/IOracleDao.h"
#include "server_common/dblib/OracleConnFactory2.h"
#include "server_common/dblib/IMysqlDao.h"
#include "server_common/config/MemcachedConfigImp.h"
#include "server_common/dblib/MemcachedFactory.h"
#include "server_common/dblib/AsyncReliableLinkHandler.h"


using namespace core;
using namespace server::config;
using namespace server::db;
using namespace server::mc;
using namespace server::mysqldb;


#define CONFIG_DBDAEMON_SERVER_INIT(ConfigName, DaemonClientClz, AppCtxClz, ConnManagerClz, SConnCreatorClz, CConnCreatorClz, LinkHandlerClz, SqlFacClz, ConfigClz, DBConfigClz)  \
	\
	AppCtxClz __appContext;		\
	ConnManagerClz __connManager;	\
	SConnCreatorClz screator;	\
	CConnCreatorClz ccreator; \
	LinkHandlerClz __handler; \
	DaemonClientClz __clientDaemon;	\
	BackLinkHandler __innerHandler;	\
	ConfigDaemonServer __server; \
	SqlFacClz __sqlFactory; \
	AsyncWriter __writer; \
	MysqlConnCreator __mysqlCreator;\
	AsyncPacketImp __packet;\
	ConfigClz __serverConfig(ConfigName); \
	DBConfigClz __dbConfig; \
	MysqlHelperTempalte __mysqlHelper;	\
	MemcachedFactory __memcachedFactory;\
	MemcachedConfigImp __memcachedConfImp;\
	server::config::LoaderContainer __loader; \
	\
	__loader.addLoader(&__serverConfig); \
	__loader.addLoader(&__dbConfig); \
	__loader.addLoader(&__memcachedConfImp);\
	__loader.loadAll();\
	\
	__sqlFactory.setConnectionCreator(&__mysqlCreator); \
	__sqlFactory.setDbServerConfig(&__serverConfig) ;\
	__sqlFactory.setDbConfig(&__dbConfig) ;\
	\
	__mysqlHelper.setSqlConnFactory(&__sqlFactory); \
	\
	__connManager.setServerConnCreator(&screator); \
	__connManager.setClientConnCreator(&ccreator); \
	\
	__handler.setAppContext(&__appContext); \
	__handler.setSqlConnFactory(&__sqlFactory); \
	__handler.setWorkerCount(20); \
	\
	__innerHandler.setAppContext(&__appContext); \
	\
	__server.setConnManager(&__connManager); \
	__server.setLinkHandler(&__handler);  \
	__server.setServerIdDispatcher(&__clientDaemon);\
	__server.setDaemonClient(&__clientDaemon);\
	__server.setServerConfig(&__serverConfig);\
	__server.setLinkEvent(&__connManager); \
	__server.setGroupId(__serverConfig.getGroupId()); \
	\
	__clientDaemon.setLinkHandler(&__innerHandler);	\
	__clientDaemon.setClientConnCreator(&ccreator); \
	__clientDaemon.setProcStat(&__handler); \
	__clientDaemon.setDaemonConnectWatcher(&__server); \
	\
	__writer.setServer(&__server); \
	__clientDaemon.setServer(&__server); \
	__writer.setServerIdDispatcher(&__clientDaemon); \
	__writer.setPacketProto(&__packet);\
	\
	__memcachedFactory.setMcConfig(&__memcachedConfImp);\
	\
	__appContext.setSqlConnFactory(&__sqlFactory); \
	__appContext.setWriter(&__writer); \
	__appContext.setDbConfig(&__serverConfig);\
	\
	__appContext.addEntry(DaemonServer::getFormEntries(), &__server, &__server);	\
	__appContext.addEntry(DBClientDaemon::getFormEntries(),	 &__clientDaemon,	 &__clientDaemon);


#define CONFIG_ORACLEPOOL_INIT(ConfigName) \
	OraSqlConnFactory __OsqlFactory;	\
	TinyXmlConfigOraDbd __OserverConfig(ConfigName);  \
	TinyXmlDbOraConfigImp __OdbConfig;  \
	server::config::LoaderContainer __Oloader;	\
	\
	__Oloader.addLoader(&__OserverConfig);  \
	__Oloader.addLoader(&__OdbConfig);  \
	__Oloader.loadAll(); \
	\
	__OsqlFactory.setDbServerConfig(&__OserverConfig);	\
	__OsqlFactory.setDbConfig(&__OdbConfig);	\
	__OsqlFactory.createPool();	\
	\
	OracleHelperTempalte __oracleHelper;	\
	__oracleHelper.setOracleConnFactory(&__OsqlFactory);	


