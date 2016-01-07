#include "server_common/helper/dbmain_inc.h"	
#include "server_common/server-lib/ImDaemonClient.h"
#include "server_common/config/tinyxml.h"
#include "../vip_dao/SingleInstance.h"

#include "app_imchat_db.h"
#include "../app_link_d/redis_helper.h"
#include "vip_lib/config_manager.h"
#include "../app_protocol/app_config_def.h"
#include "../vip_dao/SingleInstance.h"

using namespace app::imchat;
using namespace protocol;
using namespace libredis;
using namespace server::vip;
using namespace core;

int main(int sz, char *args[]) 
{	
	// dao_reflect::gl_show_all_regist_function(cout);
	init_daemon dm(sz, args);
	SET_LOG_LEVEL_CONTROL_MULTI_THREAD

	WrapServerStart::init();
	CONFIG_DBDAEMON_SERVER_INIT("app_imchat_d", 
		AsyncDBAppContext, 
		MultiConnManagerImp,
		InnerConnCreator, 
		InnerConnCreator, 
		AsyncLinkHandler, 
		AsyncSqlConnFactory,
		TinyXmlConfigDbd,
		TinyXmlDbConfigImp)

	SingleDaoIns<ConfigManager>::setInstance(new ConfigManager(Conf_Proc_AppImChat));
	SingleDaoIns<ConfigManager>::getInstance()->RegisterConfig(kTypeRedis,REDIS_APP_IMONLINE);
	SingleDaoIns<ConfigManager>::getInstance()->Init();
	SingleDaoIns<ConfigManager>::getInstance()->InitRedisFactory();

	SingleDaoIns<RedisHelper>::setInstance(new RedisHelper(REDIS_APP_IMONLINE));

	ImChatDb im_chat_db;
	__appContext.addEntry(ImChatDb::getFormEntries(), &im_chat_db, &im_chat_db);
	im_chat_db.setServerIdDispatcher(&__clientDaemon);
	im_chat_db.setServer1(&__server);
	DAEMON_SERVER_START
	WrapServerStart::run();
	//delete doc;
	return 0;
}
