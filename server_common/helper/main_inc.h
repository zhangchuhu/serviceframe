#ifndef MAIN_INC_H_
#define MAIN_INC_H_
#include "server_common/server-lib/DaemonServer.h"
#include "server_common/server-lib/DaemonClient.h"
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
#include "server_common/config/TinyXmlServerConfigImp.h"
#include "server_common/config/LoaderContainer.h"
#include "server_common/server-lib/ReliableWriter.h"
#include "server_common/server-lib/ReliableAppContext.h"

using namespace core;
using namespace server::config;

#define CONFIG_RDAEMON_SERVER_INIT(ConfigName, AppCtxClz, ConnManagerClz, SConnCreatorClz, CConnCreatorClz, LinkHandlerClz, WriterClz, ConfigClz)  \
	\
	AppCtxClz __appContext;		\
	ConnManagerClz __connManager;	\
	SConnCreatorClz screator;	\
	CConnCreatorClz ccreator; \
	LinkHandlerClz __handler; \
	ConfigDaemonServer __server; \
	DaemonClient __clientDaemon;	\
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
	__server.setServerIdDispatcher(&__clientDaemon);\
	__server.setDaemonClient(&__clientDaemon);\
	__server.setServerConfig(&__serverConfig);\
	__server.setLinkEvent(&__connManager); \
	__server.setGroupId(__serverConfig.getGroupId()); \
	\
	__clientDaemon.setClientConnCreator(&ccreator);\
	__clientDaemon.setLinkHandler(&__handler);	\
	__clientDaemon.setProcStat(&__handler); \
	__clientDaemon.setDaemonConnectWatcher(&__server); \
	\
	__appContext.setWriter(&__writer); \
	__writer.setServer(&__server); \
	__writer.setServerIdDispatcher(&__clientDaemon); \
	__writer.setPacketProto(&__packet); \
	__clientDaemon.setServer(&__server); \
	\
	__appContext.addEntry(DaemonClient::getFormEntries(),	 &__clientDaemon, &__clientDaemon); \
	__appContext.addEntry(ConfigDaemonServer::getFormEntries(), &__server, &__server);

#ifndef CONFIG_RELIABLE_TRANSFER
#define CONFIG_RELIABLE_TRANSFER\
    CReliableWriter __rWriter;\
    CReliableWriter::SetInstance(&__rWriter);\
    __rWriter.setServer(&__server);\
    __rWriter.setDaemonClient(&__clientDaemon);\
    __rWriter.setServerIdDispatcher(&__clientDaemon);\
    __appContext.addEntry(CReliableWriter::getFormEntries(), &__rWriter, &__rWriter);\
    \
    CReliableAppContext __rAppContext;\
    __rAppContext.setLinkHandler(&__handler);\
    __rAppContext.setAppContext(&__appContext);\
    __rAppContext.setDaemonClient(&__clientDaemon);\
    __appContext.addEntry(CReliableAppContext::getFormEntries(), &__rAppContext, &__rAppContext);
#endif

#define DAEMON_SERVER_INIT(AppCtxClz, ConnManagerClz, SConnCreatorClz, CConnCreatorClz, LinkHandlerClz, ServerClz, WriterClz)  \
	\
AppCtxClz __appContext;		\
ConnManagerClz __connManager;	\
SConnCreatorClz screator;	\
CConnCreatorClz ccreator; \
LinkHandlerClz __handler; \
ServerClz __server; \
DaemonClient __clientDaemon;	\
WriterClz __writer;\
PacketImp __packet;\
\
__connManager.setServerConnCreator(&screator); \
__connManager.setClientConnCreator(&ccreator); \
	\
__handler.setAppContext(&__appContext); \
	\
__server.setConnManager(&__connManager); \
__server.setLinkHandler(&__handler);  \
__server.setServerIdDispatcher(&__clientDaemon);\
__server.setDaemonClient(&__clientDaemon);\
	\
__clientDaemon.setLinkHandler(&__handler);	\
__clientDaemon.setClientConnCreator(&ccreator); \
__clientDaemon.setServer(&__server); \
__clientDaemon.setProcStat(&__handler); \
__clientDaemon.setDaemonConnectWatcher(&__server); \
\
__appContext.setWriter(&__writer); \
__writer.setPacketProto(&__packet); \
__writer.setServerIdDispatcher(&__clientDaemon); \
	\
__appContext.addEntry(DaemonClient::getFormEntries(),	 &__clientDaemon, &__clientDaemon); \
__appContext.addEntry(DaemonServer::getFormEntries(), &__server, &__server);	

#define DAEMON_SERVER_START __clientDaemon.startSV(); __server.startSV();


#endif /*MAIN_INC_H_*/

