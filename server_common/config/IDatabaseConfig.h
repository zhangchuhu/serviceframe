#ifndef _IDATABASE_CONFIG_H_FXW
#define _IDATABASE_CONFIG_H_FXW

#include <string>
#include <vector>
#include <map>
#include "common/int_types.h"
#include "IConfigLoader.h"
namespace server{
	namespace config{
		struct DatabaseParam{
			std::string host;
			uint16_t port;
			std::string user;
			std::string passwd;
			std::string db;
			std::string encode;
			int id;
			bool operator== (const DatabaseParam &right){
				return host == right.host && port == right.port && db == right.db;
			}
		};
		// for oracle
		struct OraDatabaseParam{
			std::string host;
			uint16_t port;
			std::string user;
			std::string passwd;
			std::string db;
			uint16_t iMaxconn;	//连接池连接上限
			uint16_t iMinconn;	//连接池常态连接数
			uint16_t iIncrconn;	//超过常态连接数后的连接增长步长
			int id;
			bool operator== (const OraDatabaseParam &right){
				return host == right.host && port == right.port && db == right.db;
			}
		};


		struct IDbConfigWatcher{
			virtual ~IDbConfigWatcher(){};
			virtual void onDbConfigRefresh() = 0;
		};

		struct IDbConfigWatchBase{
			virtual ~IDbConfigWatchBase(){};
			virtual void watch(IDbConfigWatcher *) = 0;
			virtual void revoke(IDbConfigWatcher *) = 0;
		};

		struct IDatabaseConfig: public virtual IDbConfigWatchBase, public IConfigLoader
		{
			//virtual DatabaseParam getDefaultDb() = 0;
			//name to hosts
			virtual std::map<std::string, std::vector<DatabaseParam> > getDatabases() = 0;
			virtual std::map<std::string, std::vector<OraDatabaseParam> > getOraDatabases() = 0;
			
		};

		typedef DatabaseParam MysqlDataParam;
		struct IDatabaseConfig2: public virtual IDbConfigWatchBase, public IConfigLoader
		{
			//virtual DatabaseParam getDefaultDb() = 0;
			//name to hosts
			virtual MysqlDataParam getMysql(const std::string &name) = 0;
			virtual OraDatabaseParam getOracle(const std::string &name) = 0;
		};

		struct IDbConfigAwareW: public IDbConfigWatcher{
		protected:
			IDatabaseConfig *dbConfig;
		public:
			virtual void setDbConfig(IDatabaseConfig *c){
				dbConfig = c;
				dbConfig->watch(this);
			}
		};

		struct IDbConfig2AwareW: public IDbConfigWatcher{
		protected:
			IDatabaseConfig2 *dbConfig;
		public:
			virtual void setDbConfig(IDatabaseConfig2 *c){
				dbConfig = c;
				dbConfig->watch(this);
			}
		};
	}
}


#endif
