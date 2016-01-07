#include "TinyXmlHADbConfigImp_cipher.h"
#include "./tinyxml.h"
#include "core/sox/logger.h"
#include <iostream>
#include "server_common/include/db_safe/IDBSafe.h"

using namespace server::config;

TinyXmlHADbCipherConfigImp::TinyXmlHADbCipherConfigImp():loaded(false){
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

void TinyXmlHADbCipherConfigImp::load(FILE *fp){
	loaded = true;
	TiXmlDocument doc;
	doc.LoadFile(fp);

	TiXmlHandle docH( &doc );

	TiXmlElement *id = docH.FirstChildElement( "conf" ).FirstChildElement( "dbha" ).FirstChildElement("dbnode_ip").Element();
	if(id){
		dbnode_ip = (id->GetText());		
		log(Info, "get Oracle-dbnode_ip[%s]\n",  dbnode_ip.c_str());		
	}
	else
	{
		dbnode_ip = "127.0.0.1";
	}
	id = docH.FirstChildElement( "conf" ).FirstChildElement( "dbha" ).FirstChildElement("dbnode_port").Element();
	if(id){
		dbnode_port = atoi(id->GetText());		
		log(Info, "get Oracle-dbnode_port[%d]\n",  dbnode_port);		
	}
	else
	{
		dbnode_port=12315;
	}
	//////////////

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

void TinyXmlHADbCipherConfigImp::watch(IDbConfigWatcher *w){
	WatchHelper<IDbConfigWatchBase, IDbConfigWatcher>::watch(w);
	if(loaded){
		w->onDbConfigRefresh();
	}
}

MysqlDataParam TinyXmlHADbCipherConfigImp::getMysql( const std::string &name )
{
	mysql_cfg_t::iterator it = mysqls.find(name);
	if(it != mysqls.end()){
		return it->second;
	}else{
		std::cout << "I don't found mysql config name:" << name << std::endl;
		assert(false);
	}
}



OraDatabaseParam TinyXmlHADbCipherConfigImp::getOracle( const std::string &name )
{
	oracle_cfg_t::iterator it = oracles.find(name);
	if(it != oracles.end()){
		return it->second;
	}else{
		std::cout << "I don't found oracle config name:" << name << std::endl;
		assert(false);
	}
}

std::string& TinyXmlHADbCipherConfigImp::get_dbnode_ip()
{
	return dbnode_ip;
}
int TinyXmlHADbCipherConfigImp::get_dbnode_port()
{
	return dbnode_port;
}

