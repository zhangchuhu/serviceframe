#include "TinyXmlDbConfigImp2_cipher.h"
#include "./tinyxml.h"
#include "core/sox/logger.h"
#include "server_common/include/db_safe/IDBSafe.h"
#include <iostream>

using namespace server::config;

TinyXmlDbConfigImp2::TinyXmlDbConfigImp2():loaded(false){
	defaultMysql.host = "db.yy.duowan.com";
	defaultMysql.port = 3306;
	defaultMysql.user = "dspeak-db-admin";
	defaultMysql.db = "dspeak";
	defaultMysql.passwd = "dsqazxsw21";
	defaultMysql.encode = "utf8";

	defaultOracle.host = "dbo.yy.duowan.com";
	defaultOracle.port = 1521;
	defaultOracle.user = "dspeak";
	defaultOracle.db = "duowan";
	defaultOracle.passwd = "YlXEBIxv";
	defaultOracle.iMaxconn = 60;
	defaultOracle.iMinconn  = 10;
	defaultOracle.iIncrconn = 2;
}

extern std::string getStr(TiXmlElement *elm);

void TinyXmlDbConfigImp2::load(FILE *fp){
	loaded = true;
	TiXmlDocument doc;
	doc.LoadFile(fp);

	TiXmlHandle docH( &doc );

	TiXmlElement *db = docH.FirstChildElement( "conf" ).FirstChildElement( "dbs" ).FirstChildElement("db").Element();
	
	while(db){
		DatabaseParam param;
		std::string name = db->Attribute("name");

		param.host = getStr(db->FirstChildElement("host"));
		param.user = getStr(db->FirstChildElement("user"));
                if (0 != server::dbPasswdDecrypt(getStr(db->FirstChildElement("passwd")), param.passwd))
                {
                        log(Warn, "db name: %s, passwd decrypt fail", name.c_str());
                        assert(false);
                }
		param.db = getStr(db->FirstChildElement("database"));
		param.port = atoi(getStr(db->FirstChildElement("port")).data());
		param.id = atoi(getStr(db->FirstChildElement("id")).data());

		mysqls[name] = param;
		db = db->NextSiblingElement("db");
	}

	db = docH.FirstChildElement( "conf" ).FirstChildElement( "dbo" ).FirstChildElement("db").Element();

	while(db){
		OraDatabaseParam param;
		std::string name = db->Attribute("name");

		param.host		= getStr(db->FirstChildElement("host"));
		param.user		= getStr(db->FirstChildElement("user"));

                if (0 != server::dbPasswdDecrypt(getStr(db->FirstChildElement("passwd")), param.passwd))
                {
                        log(Warn, "db name: %s, passwd decrypt fail", name.c_str());
                        assert(false);
                }
		param.db			= getStr(db->FirstChildElement("database"));
		param.port		= atoi(getStr(db->FirstChildElement("port")).data());
		param.id			= atoi(getStr(db->FirstChildElement("id")).data());
		param.iMaxconn	= atoi(getStr(db->FirstChildElement("maxconn")).data());
		param.iMinconn	= atoi(getStr(db->FirstChildElement("minconn")).data());
		param.iIncrconn	= atoi(getStr(db->FirstChildElement("incrconn")).data());		

		oracles[name] = param;
		db = db->NextSiblingElement("db");
	}


	forEachWatcher0(&IDbConfigWatcher::onDbConfigRefresh);
}

void TinyXmlDbConfigImp2::watch(IDbConfigWatcher *w){
	WatchHelper<IDbConfigWatchBase, IDbConfigWatcher>::watch(w);
	if(loaded){
		w->onDbConfigRefresh();
	}
}

MysqlDataParam TinyXmlDbConfigImp2::getMysql( const std::string &name )
{
	mysql_cfg_t::iterator it = mysqls.find(name);
	if(it != mysqls.end()){
		return it->second;
	}else{
		std::cout << "I don't found mysql config name:" << name << std::endl;
		assert(false);
	}
}



OraDatabaseParam TinyXmlDbConfigImp2::getOracle( const std::string &name )
{
	oracle_cfg_t::iterator it = oracles.find(name);
	if(it != oracles.end()){
		return it->second;
	}else{
		std::cout << "I don't found oracle config name:" << name << std::endl;
		assert(false);
	}
}

