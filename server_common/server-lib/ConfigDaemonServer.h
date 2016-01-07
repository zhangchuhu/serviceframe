#ifndef CONFIG_DAEMON_SERVER_H_FXW_
#define CONFIG_DAEMON_SERVER_H_FXW_
#include "DaemonServer.h"
#include "server_common/config/IServerConfig.h"

namespace core{

	class ConfigDaemonServer: public DaemonServer, public server::config::IServerConfigAwareW{
	public:
		virtual void onConfigRefresh();
	};
}


#endif
