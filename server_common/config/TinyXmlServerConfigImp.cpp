#include "TinyXmlServerConfigImp.h"
#include "./tinyxml.h"
#include "core/sox/logger.h"

using namespace server::config;

TinyXmlServerConfigImp::TinyXmlServerConfigImp(const std::string &server): serverElem(server), port(0),  loaded(false){
}

uint16_t TinyXmlServerConfigImp::getPort(){
	return port;
}
std::vector<uint16_t> TinyXmlServerConfigImp::getFrontPorts(){
	return frontPorts;
}
std::string TinyXmlServerConfigImp::getName(){
	return name;
}

uint32_t TinyXmlServerConfigImp::getGroupId() {
	return groupId;
}
/*
bool TinyXmlServerConfigImp::isHangup(){
	return hangup;
}
*/

void TinyXmlServerConfigImp::loadServer(TiXmlHandle &serverH){
	TiXmlElement *portE = serverH.FirstChildElement("port").Element();
	if(portE){
		port = atoi(portE->GetText());
	}

	if(port == 0){
		log(Fatal, "config port first");
		exit(-1);
	}

	TiXmlElement *nameE = serverH.FirstChildElement("name").Element();
	if(nameE){
		//null to die;
		name = nameE->GetText();
	}

	TiXmlElement *fportE = serverH.FirstChildElement("front_port").Element();
	while(fportE){
		uint16_t p = atoi(fportE->GetText());
		if(p != 0)
			frontPorts.push_back(p);
		fportE = fportE->NextSiblingElement("front_port");
	}


	TiXmlElement *groupE = serverH.FirstChildElement("group").Element();
	if(groupE){
		groupId = atoi(groupE->GetText());
	}else{
		groupId = 0;
	}
}

void TinyXmlServerConfigImp::load(FILE *fp){
	loaded = true;
	TiXmlDocument doc;
	doc.LoadFile(fp);

	TiXmlHandle docH( &doc );
	TiXmlHandle servers = docH.FirstChildElement( "conf" ).FirstChildElement( "servers" );
	if(servers.Element()){
		
		TiXmlHandle server = servers.FirstChildElement(serverElem.data());
		if(server.Element()){
			loadServer(server);
		}else{
			log(Fatal, "%s config not found", serverElem.data());
			exit(-1);
		}
	}else{
		log(Error, "servers element is NULL");
	}

	forEachWatcher0(&IServerConfigWatcher::onConfigRefresh);
}

void TinyXmlServerConfigImp::watch(IServerConfigWatcher *w){
	WatchHelper<IServerConfigWatchBase, IServerConfigWatcher>::watch(w);
	if(loaded){
		w->onConfigRefresh();
	}
}
TinyXmlConfigDbd::TinyXmlConfigDbd(const std::string &e):TinyXmlServerConfigImp(e){

}

std::pair<std::string, std::string> TinyXmlConfigDbd::getDefaultName(){
	return defaultNames;
}

int TinyXmlConfigDbd::getDefaultId(){
	return defaultId;
}

void TinyXmlConfigDbd::loadServer(TiXmlHandle &serverH){

	TiXmlElement *db = serverH.FirstChildElement("default-db").Element();
	if(db){
		std::string master = db->Attribute("master");
		std::string slave = db->Attribute("slave");

		defaultNames = make_pair(master, slave);
	}

	TiXmlElement *id = serverH.FirstChildElement("default-db-id").Element();
	if(id){
		defaultId = atoi(id->GetText());
	}

	TinyXmlServerConfigImp::loadServer(serverH);
}
TinyXmlConfigOraDbd::TinyXmlConfigOraDbd(const std::string &e):TinyXmlServerConfigImp(e){

}

std::pair<std::string, std::string> TinyXmlConfigOraDbd::getDefaultName(){
	return defaultNames;
}

int TinyXmlConfigOraDbd::getDefaultId(){
	return defaultId;
}

std::string getStr(TiXmlElement *elm){
	if(elm){
		return elm->GetText();
	}else{
		return "";
	}
}

void TinyXmlConfigOraDbd::loadServer(TiXmlHandle &serverH){
	//提取进程所需的数据库id配置
	TiXmlElement *id = serverH.FirstChildElement("default-ora-db-id").Element();
	if(id){
		defaultId = atoi(id->GetText());		
		log(Info, "get Oracle-dbid[%d]\n",  defaultId);		
	}

	memcaches.clear();
	TiXmlElement *mc = serverH.FirstChildElement("memcache").Element();
	while(mc){
		
		std::string host = getStr(mc->FirstChildElement("host"));
		int port = atoi(getStr(mc->FirstChildElement("port")).data());

		memcaches.push_back(make_pair(host, port));
		mc = mc->NextSiblingElement("memcache");
	}

	TinyXmlServerConfigImp::loadServer(serverH);
}


ipPort_t TinyXmlConfigOraDbd::getMemcaches()
{
	return memcaches;
}



TinyXmlConfigHADbd::TinyXmlConfigHADbd(const std::string &e):TinyXmlServerConfigImp(e){

}

std::pair<std::string, std::string> TinyXmlConfigHADbd::getDefaultName(){
	return defaultNames;
}

int TinyXmlConfigHADbd::getDefaultId(){
	return defaultId;
}


void TinyXmlConfigHADbd::loadServer(TiXmlHandle &serverH){
	//提取进程所需的数据库id配置
	TiXmlElement *id = serverH.FirstChildElement("default-ora-db-id").Element();
	if(id){
		defaultId = atoi(id->GetText());		
		log(Info, "get Oracle-dbid[%d]\n",  defaultId);		
	}
	id = serverH.FirstChildElement("dbnode_max_connect").Element();
	if(id){
		dbnode_max_connect = atoi(id->GetText());		
		log(Info, "get Oracle-dbnode_max_connect[%d]\n",  dbnode_max_connect);
	}
	else
	{
		dbnode_max_connect=100;
	}
	id = serverH.FirstChildElement("dbnode_time_out_mill").Element();
	if(id){
		dbnode_time_out_mill = atoi(id->GetText());		
		log(Info, "get Oracle-dbnode_time_out_mill[%d]\n",  dbnode_time_out_mill);
	}
	else
	{
		dbnode_time_out_mill=5000;
	}
	id = serverH.FirstChildElement("broker_uri").Element();
	if(id){
		_brokerURI = id->GetText();
		log(Info, "get Oracle-broker_uri[%s]\n",  _brokerURI.c_str());
	}
	else
	{
		_brokerURI.clear();
	}
	memcaches.clear();
	TiXmlElement *mc = serverH.FirstChildElement("memcache").Element();
	while(mc){
		
		std::string host = getStr(mc->FirstChildElement("host"));
		int port = atoi(getStr(mc->FirstChildElement("port")).data());

		memcaches.push_back(make_pair(host, port));
		mc = mc->NextSiblingElement("memcache");
	}

	TiXmlElement *webdbAllow = serverH.FirstChildElement("webdbAllows").FirstChildElement("webdbAllow").Element();
	webdbAllowIps.clear();
	while(webdbAllow){
		const char *ip = webdbAllow->Attribute("ip");

		webdbAllowIps.insert(ip);
		webdbAllow = webdbAllow->NextSiblingElement("webdbAllow");
	}

	TinyXmlServerConfigImp::loadServer(serverH);
}

ipPort_t TinyXmlConfigHADbd::getMemcaches()
{
	return memcaches;
}

int TinyXmlConfigHADbd::get_max_connection()
{
	return dbnode_max_connect;
}
int TinyXmlConfigHADbd::get_time_out_mill()
{
	return dbnode_time_out_mill;
}
std::string TinyXmlConfigHADbd::getBrokerURI()
{
	return _brokerURI;
}
std::set<std::string> TinyXmlConfigHADbd::getWebdbAllowIps()
{
	return webdbAllowIps;
}

TinyXmlConfigSession::TinyXmlConfigSession(const std::string &e):TinyXmlServerConfigImp(e){

}

void TinyXmlConfigSession::loadServer(TiXmlHandle &serverH){

  TiXmlElement *ctlProxyE = serverH.FirstChildElement("min_ctl_proxy").Element();
  if(ctlProxyE){
    minCtlProxy = atoi(ctlProxyE->GetText());
  }else{
    minCtlProxy = 0;
  }

  TiXmlElement *cncProxyE = serverH.FirstChildElement("min_cnc_proxy").Element();
  if(cncProxyE){
    minCncProxy = atoi(cncProxyE->GetText());
  }else{
    minCncProxy = 0;
  }

  TiXmlElement *proxyUserE = serverH.FirstChildElement("max_proxy_user").Element();
  if(proxyUserE){
    maxProxyUser = atoi(proxyUserE->GetText());
  }else{
    maxProxyUser = 0;
  }

  TinyXmlServerConfigImp::loadServer(serverH);
}

uint32_t TinyXmlConfigSession::getMinCtlProxy()
{
  return minCtlProxy;
}

uint32_t TinyXmlConfigSession::getMinCncProxy()
{
  return minCncProxy;
}

uint32_t TinyXmlConfigSession::getMaxProxyUser()
{
  return maxProxyUser;
}

TinyXmlConfigUhsProxy::TinyXmlConfigUhsProxy(const std::string &e):TinyXmlConfigDbd(e){

}

void TinyXmlConfigUhsProxy::loadServer(TiXmlHandle &serverH)
{
	TiXmlElement *confProxyIp = serverH.FirstChildElement("uhs_proxy_ip").Element();
	if(confProxyIp){
		proxyIp = confProxyIp->GetText();
	}

	TiXmlElement *confProxyPort = serverH.FirstChildElement("uhs_proxy_port").Element();
	if(confProxyPort){
		proxyPort = atoi(confProxyPort->GetText());
	}else{
		proxyPort = 0;
	}
	TinyXmlConfigDbd::loadServer(serverH);
}

std::string TinyXmlConfigUhsProxy::getProxyIp()
{
	return proxyIp;
}
uint32_t TinyXmlConfigUhsProxy::getProxyPort()
{
	return proxyPort;
}

TinyXmlConfigKdc::TinyXmlConfigKdc(const std::string &e):TinyXmlServerConfigImp(e){

}

void TinyXmlConfigKdc::loadServer(TiXmlHandle &serverH)
{
	TiXmlElement *confDb = serverH.FirstChildElement("kerberos_db").Element();
	if(confDb){
		dbName = confDb->GetText();
	}

	TiXmlElement *confTable = serverH.FirstChildElement("service_table").Element();
	if(confTable){
		tableName = confTable->GetText();
	}

	TiXmlElement *confDbPwd = serverH.FirstChildElement("secret").Element();
	if(confDbPwd){
		dbPwd = confDbPwd->GetText();
	}

	TiXmlElement *confProxyIp = serverH.FirstChildElement("uhs_proxy_ip").Element();
	if(confProxyIp){
		proxyIp = confProxyIp->GetText();
	}

	TiXmlElement *confProxyPort = serverH.FirstChildElement("uhs_proxy_port").Element();
	if(confProxyPort){
		proxyPort = atoi(confProxyPort->GetText());
	}else{
		proxyPort = 0;
	}
	TinyXmlServerConfigImp::loadServer(serverH);
}

std::string TinyXmlConfigKdc::getDbName()
{
	return dbName;
}

std::string TinyXmlConfigKdc::getTableName()
{
	return tableName;
}

std::string TinyXmlConfigKdc::getDbPwd()
{
	return dbPwd;
}

std::string TinyXmlConfigKdc::getProxyIp()
{
	return proxyIp;
}

uint32_t TinyXmlConfigKdc::getProxyPort()
{
	return proxyPort;
}

TinyXmlConfigUhsProxyNew::TinyXmlConfigUhsProxyNew(const std::string &e):TinyXmlConfigDbd(e){

}

void TinyXmlConfigUhsProxyNew::loadServer(TiXmlHandle &serverH)
{
	TiXmlElement *confProxyMode = serverH.FirstChildElement("proxy_mode").Element();
	if(confProxyMode){
		proxyMode = confProxyMode->GetText();
	}

	TinyXmlConfigDbd::loadServer(serverH);
}

std::string TinyXmlConfigUhsProxyNew::getProxyMode()
{
	return proxyMode;
}
