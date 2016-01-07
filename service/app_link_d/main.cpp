#include "server_common/helper/im_inc.h"
#include "server_common/server-lib/ImDaemonClient.h"
#include "server_common/config/tinyxml.h"
#include "../vip_dao/SingleInstance.h"

#include "link_online.h"
#include "link_app_context.h"
#include "redis_helper.h"
#include "vip_lib/config_manager.h"
#include "../app_protocol/app_config_def.h"
#include "../vip_dao/SingleInstance.h"

using namespace server::applinkd;
using namespace protocol;
using namespace libredis;
using namespace server::vip;

int main(int sz, char *args[]) 
{	
	// dao_reflect::gl_show_all_regist_function(cout);
	init_daemon dm(sz, args);
	SET_LOG_LEVEL_CONTROL_MULTI_THREAD
	WrapServerStart::init();

	CONFIG_RDAEMON_SERVER_INIT("app_link_d",
	CImDaemonClient,
	LinkAppContext, 
	MultiConnManagerImp,
	InnerConnCreator,
	InnerConnCreator, 
	BackLinkHandler, 
	RouteWriter,
	TinyXmlConfigDbd)

	SingleDaoIns<ConfigManager>::setInstance(new ConfigManager(Conf_Proc_AppLinkd));
	SingleDaoIns<ConfigManager>::getInstance()->RegisterConfig(kTypeRedis,REDIS_APP_IMONLINE);
	SingleDaoIns<ConfigManager>::getInstance()->Init();
	SingleDaoIns<ConfigManager>::getInstance()->InitRedisFactory();

	SingleDaoIns<RedisHelper>::setInstance(new RedisHelper(REDIS_APP_IMONLINE) );

    

	LinkOnline link_online;
	link_online.setServer1(&__server);
	link_online.setConnManager(&__connManager);
	__appContext.addEntry(LinkOnline::getFormEntries(), &link_online, &link_online);
	__appContext.addEntry(LinkAppContext::getFormEntries(),&__appContext,&__appContext);
	__appContext.setUidDispatcher(&link_online);
	__appContext.setServer1(&__server);
	__appContext.setServerIdDispatcher(&__clientDaemon);

	DAEMON_SERVER_START
	WrapServerStart::run();
	//delete doc;
	return 0;
}
