#ifndef _TINYXML_DBCONFIG2_H_FXW_
#define _TINYXML_DBCONFIG2_H_FXW_
#include <string>
#include <vector>
#include "common/int_types.h"
#include "IDatabaseConfig.h"
#include "common/watch_helper.h"

class TiXmlHandle;

namespace server{
	namespace config{
		class TinyXmlDbConfigImp2: public virtual IDatabaseConfig2, public WatchHelper<IDbConfigWatchBase, IDbConfigWatcher>{
		public:
			TinyXmlDbConfigImp2();
			virtual MysqlDataParam getMysql(const std::string &name);
			virtual OraDatabaseParam getOracle(const std::string &name);
									
			virtual void load(FILE *fp);
			virtual void watch(IDbConfigWatcher *);
		private:
			bool loaded;
			MysqlDataParam defaultMysql;
			OraDatabaseParam defaultOracle;

			typedef std::map<std::string, MysqlDataParam> mysql_cfg_t;
			mysql_cfg_t mysqls;
			typedef std::map<std::string, OraDatabaseParam> oracle_cfg_t;
			oracle_cfg_t oracles;
		};
	}
}
#endif

