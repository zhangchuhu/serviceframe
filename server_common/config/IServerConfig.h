#ifndef _ISERVER_CONFIG_H_FXW
#define _ISERVER_CONFIG_H_FXW
#include "common/int_types.h"
#include <string>
#include <vector>
#include "IConfigLoader.h"
#include "server_common/helper/helper.h"

namespace server{
	namespace config{

		struct IServerConfigWatcher{
			virtual ~IServerConfigWatcher(){};
			virtual void onConfigRefresh() = 0;
		};

		struct IServerConfigWatchBase{
			virtual ~IServerConfigWatchBase(){};
			virtual void watch(IServerConfigWatcher *)  = 0;
			virtual void revoke(IServerConfigWatcher *) = 0;
		};

		struct IServerConfig: public virtual IServerConfigWatchBase, public virtual IConfigLoader
		{
			virtual uint16_t getPort() = 0;
			virtual std::vector<uint16_t> getFrontPorts() = 0;
			virtual std::string getName() = 0;
			//×éid, Ä¬ÈÏ0
			virtual uint32_t getGroupId() = 0;
		}; 

		class IServerConfigAware{
		private:
			IServerConfig *config;
		public:
			virtual ~IServerConfigAware(){}
			virtual void setServerConfig(IServerConfig *c){
				config = c;
			}
			IServerConfig *getServerConfig(){
				return config;
			}
		};

		class IServerConfigAwareW: public IServerConfigWatcher, public IServerConfigAware{
		public:
			virtual void setServerConfig(IServerConfig *c){
				IServerConfigAware::setServerConfig(c);
				c->watch(this);
			}
		};



		struct IDbServerConfig: public virtual IServerConfig{
			//first master, second slave
			virtual std::pair<std::string, std::string> getDefaultName() = 0;
			virtual int getDefaultId() = 0;
			virtual ipPort_t getMemcaches() = 0;
		};

    struct ISessionServerConfig: public virtual IServerConfig{
      virtual uint32_t getMinCtlProxy() = 0;
      virtual uint32_t getMinCncProxy() = 0;
      virtual uint32_t getMaxProxyUser() = 0;
    };

		struct IUhsProxyServerConfig: public virtual IServerConfig{
			virtual std::string getProxyIp() = 0;
			virtual uint32_t getProxyPort() = 0;
		};
		struct IUhsProxyServerConfigNew: public virtual IServerConfig{
			virtual std::string getProxyMode() = 0;
		};
}
}
#endif
