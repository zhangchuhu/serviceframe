#ifndef _TINYXML_DBCONFIG_H_FXW_CIPHER_
#define _TINYXML_DBCONFIG_H_FXW_CIPHER_
#include <string>
#include <vector>
#include "common/int_types.h"
#include "IDatabaseConfig.h"
#include "common/watch_helper.h"

class TiXmlHandle;

namespace server{
	namespace config{
		class TinyXmlDbCipherConfigImp: public virtual IDatabaseConfig, public WatchHelper<IDbConfigWatchBase, IDbConfigWatcher>{
		public:
			TinyXmlDbCipherConfigImp();
			virtual DatabaseParam getDefaultDb();
			virtual std::map<std::string, std::vector<DatabaseParam> > getDatabases();
			//for oracle
			OraDatabaseParam getDefaultOraDb(){ OraDatabaseParam t; return t;}
			virtual std::map<std::string, std::vector<OraDatabaseParam> > getOraDatabases(){
				std::map<std::string, std::vector<OraDatabaseParam> > t;
				return t;
			}			
			
			virtual void load(FILE *fp);
			virtual void watch(IDbConfigWatcher *);
		private:
			DatabaseParam defaultDb;
			bool loaded;
			std::map<std::string, std::vector<DatabaseParam> > databases;
		};

		class TinyXmlDbOraCipherConfigImp: public virtual IDatabaseConfig, public WatchHelper<IDbConfigWatchBase, IDbConfigWatcher>{
		public:
			TinyXmlDbOraCipherConfigImp();
			//for mysql
			virtual DatabaseParam getDefaultDb(){ DatabaseParam t; return t;}
			virtual std::map<std::string, std::vector<DatabaseParam> > getDatabases(){ std::map<std::string, std::vector<DatabaseParam> >  t ; return t;}
			//for oracle
			OraDatabaseParam getDefaultOraDb();
			virtual std::map<std::string, std::vector<OraDatabaseParam> > getOraDatabases();			
			virtual void load(FILE *fp);
			virtual void watch(IDbConfigWatcher *);
		private:
//			DatabaseParam defaultDb;
			OraDatabaseParam defaultDb;
			bool loaded;
//			std::map<std::string, std::vector<DatabaseParam> > databases;
			std::map<std::string, std::vector<OraDatabaseParam> > databases;
		};		
	}
}
#endif

