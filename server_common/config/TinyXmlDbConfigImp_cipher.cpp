#include "TinyXmlDbConfigImp_cipher.h"
#include "./tinyxml.h"
#include "core/sox/logger.h"
#include <iostream>
#include "server_common/include/db_safe/IDBSafe.h"

#define DEAFAULT_XML "../bin/server.xml"

using namespace server::config;

TinyXmlDbCipherConfigImp::TinyXmlDbCipherConfigImp():loaded(false){
	defaultDb.host = "db.yy.duowan.com";
	defaultDb.port = 3306;
	defaultDb.user = "dspeak-db-admin";
	defaultDb.db = "dspeak";
	defaultDb.passwd = "dsqazxsw21";
	defaultDb.encode = "utf8";

}

DatabaseParam TinyXmlDbCipherConfigImp::getDefaultDb(){
	return defaultDb;
}

std::map<std::string, std::vector<DatabaseParam> > TinyXmlDbCipherConfigImp::getDatabases(){
	return databases;
}

extern std::string getStr(TiXmlElement *elm);

void TinyXmlDbCipherConfigImp::load(FILE *fp){
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

		databases[name].push_back(param);
		db = db->NextSiblingElement("db");
	}

	forEachWatcher0(&IDbConfigWatcher::onDbConfigRefresh);
}

void TinyXmlDbCipherConfigImp::watch(IDbConfigWatcher *w){
	WatchHelper<IDbConfigWatchBase, IDbConfigWatcher>::watch(w);
	if(loaded){
		w->onDbConfigRefresh();
	}
}
///// oracle config

TinyXmlDbOraCipherConfigImp::TinyXmlDbOraCipherConfigImp():loaded(false){
	defaultDb.host = "dbo.yy.duowan.com";
	defaultDb.port = 1521;
	defaultDb.user = "dspeak";
	defaultDb.db = "duowan";
	defaultDb.passwd = "YlXEBIxv";
	defaultDb.iMaxconn = 60;
	defaultDb.iMinconn  = 10;
	defaultDb.iIncrconn = 2;
}

OraDatabaseParam TinyXmlDbOraCipherConfigImp::getDefaultOraDb(){
	return defaultDb;
}

std::map<std::string, std::vector<OraDatabaseParam> > TinyXmlDbOraCipherConfigImp::getOraDatabases(){
	return databases;
}

void TinyXmlDbOraCipherConfigImp::load(FILE *fp){
	loaded = true;
	TiXmlDocument doc;
	doc.LoadFile(fp);

	TiXmlHandle docH( &doc );

//	TiXmlElement *db = docH.FirstChildElement( "conf" ).FirstChildElement( "dbs" ).FirstChildElement("db").Element();
	TiXmlElement *db = docH.FirstChildElement( "conf" ).FirstChildElement( "dbo" ).FirstChildElement("db").Element();
	
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
		
		databases[name].push_back(param);
		db = db->NextSiblingElement("db");
	}

	forEachWatcher0(&IDbConfigWatcher::onDbConfigRefresh);
}

void TinyXmlDbOraCipherConfigImp::watch(IDbConfigWatcher *w){
	WatchHelper<IDbConfigWatchBase, IDbConfigWatcher>::watch(w);
	if(loaded){
		w->onDbConfigRefresh();
	}


}
