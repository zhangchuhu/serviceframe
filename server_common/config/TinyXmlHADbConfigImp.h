#ifndef _TINYXML_HA_DBCONFIG_H_FXW_
#define _TINYXML_HA_DBCONFIG_H_FXW_

#include <string>
#include <vector>
#include "common/int_types.h"
#include "IDatabaseConfig.h"
#include "common/watch_helper.h"

class TiXmlHandle;

namespace server{
	namespace config{
		class TinyXmlHADbConfigImp: public virtual IDatabaseConfig2, public WatchHelper<IDbConfigWatchBase, IDbConfigWatcher>{
		public:
			TinyXmlHADbConfigImp();
			virtual MysqlDataParam getMysql(const std::string &name);
			virtual OraDatabaseParam getOracle(const std::string &name);
									
			virtual void load(FILE *fp);
			virtual void watch(IDbConfigWatcher *);

			std::string& get_dbnode_ip();
			int get_dbnode_port();
		private:
			bool loaded;
			MysqlDataParam defaultMysql;
			OraDatabaseParam defaultOracle;
			std::string dbnode_ip;
			int dbnode_port;

			typedef std::map<std::string, MysqlDataParam> mysql_cfg_t;
			mysql_cfg_t mysqls;
			typedef std::map<std::string, OraDatabaseParam> oracle_cfg_t;
			oracle_cfg_t oracles;
		};
	}
}
#endif

