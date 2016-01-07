#ifndef _TINYXML_CONFIG_H_FXW_
#define _TINYXML_CONFIG_H_FXW_
#include <string>
#include <vector>
#include "common/int_types.h"
#include "IServerConfig.h"
#include "common/watch_helper.h"
#include <set>
#include <string>

class TiXmlHandle;

namespace server{
	namespace config{
		class TinyXmlServerConfigImp: public virtual IServerConfig, public WatchHelper<IServerConfigWatchBase, IServerConfigWatcher>{
		public:
			TinyXmlServerConfigImp(const std::string &);
			virtual uint16_t getPort();
			virtual std::vector<uint16_t> getFrontPorts();
			virtual std::string getName();
			virtual uint32_t getGroupId();

			virtual void load(FILE *fp);
			virtual void watch(IServerConfigWatcher *);
		protected:
			std::string serverElem;
			virtual void loadServer(TiXmlHandle &serverH);
		private:
			uint16_t port;
			std::vector<uint16_t> frontPorts;
			std::string name;
			std::vector<std::pair<std::string, std::string> > allowIps;
			uint32_t groupId;
			bool loaded;
		};

		class TinyXmlConfigDbd: public TinyXmlServerConfigImp, public IDbServerConfig{
		public:
			TinyXmlConfigDbd(const std::string &);
			virtual std::pair<std::string, std::string> getDefaultName();
			virtual int getDefaultId();
			virtual void loadServer(TiXmlHandle &serverH);
			virtual ipPort_t getMemcaches(){return ipPort_t();}
		private:
			std::pair<std::string, std::string> defaultNames;
			int defaultId;
		};
		class TinyXmlConfigOraDbd: public TinyXmlServerConfigImp, public IDbServerConfig{
		public:
			TinyXmlConfigOraDbd(const std::string &);
			virtual std::pair<std::string, std::string> getDefaultName();
			virtual int getDefaultId();
			virtual ipPort_t getMemcaches();
			virtual void loadServer(TiXmlHandle &serverH);
		private:
			std::pair<std::string, std::string> defaultNames;
			ipPort_t memcaches;
			int defaultId;
		};	
		class TinyXmlConfigHADbd: public TinyXmlServerConfigImp, public IDbServerConfig{
		public:
			TinyXmlConfigHADbd(const std::string &);
			virtual std::pair<std::string, std::string> getDefaultName();
			virtual int getDefaultId();
			virtual ipPort_t getMemcaches();
			virtual int get_max_connection();
			virtual int get_time_out_mill();
			virtual void loadServer(TiXmlHandle &serverH);
			virtual std::string getBrokerURI();
			virtual std::set<std::string> getWebdbAllowIps();
		private:
			std::pair<std::string, std::string> defaultNames;
			ipPort_t memcaches;
			int defaultId;
			int dbnode_max_connect;
			int dbnode_time_out_mill;
			std::string _brokerURI;
			std::set<std::string> webdbAllowIps;
		};
		class TinyXmlConfigSession: public TinyXmlServerConfigImp, public ISessionServerConfig{
		public:
			TinyXmlConfigSession(const std::string &);
			virtual void loadServer(TiXmlHandle &serverH);
			virtual uint32_t getMinCtlProxy();
			virtual uint32_t getMinCncProxy();
			virtual uint32_t getMaxProxyUser();
		private:
			uint32_t minCtlProxy;
			uint32_t minCncProxy;
			uint32_t maxProxyUser;
		};    
		class TinyXmlConfigUhsProxy: public TinyXmlConfigDbd, public IUhsProxyServerConfig{
		public:
			TinyXmlConfigUhsProxy(const std::string &);
			virtual void loadServer(TiXmlHandle &serverH);
			virtual std::string getProxyIp();
			virtual uint32_t getProxyPort();
		private:
			std::string proxyIp;
			uint32_t proxyPort;
		}; 
		class TinyXmlConfigKdc: public TinyXmlServerConfigImp, public IUhsProxyServerConfig{
		public:
			TinyXmlConfigKdc(const std::string &);
			virtual void loadServer(TiXmlHandle &serverH);
			virtual std::string getDbName();
			virtual std::string getTableName();
			virtual std::string getDbPwd();
			virtual std::string getProxyIp();
			virtual uint32_t getProxyPort();
		private:
			std::string dbName;
			std::string tableName;
			std::string dbPwd;
			std::string proxyIp;
			uint32_t proxyPort;
		};
		class TinyXmlConfigUhsProxyNew: public TinyXmlConfigDbd, public IUhsProxyServerConfigNew{
		public:
			TinyXmlConfigUhsProxyNew(const std::string &);
			virtual void loadServer(TiXmlHandle &serverH);
			virtual std::string getProxyMode();
		private:
			std::string proxyMode;
		}; 
	};
}
#endif

