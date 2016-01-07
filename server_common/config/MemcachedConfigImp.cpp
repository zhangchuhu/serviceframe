#include "MemcachedConfigImp.h"
#include "config/tinyxml.h"
#include "core/sox/logger.h"

using namespace std;
using namespace server::config;


//load xml config file
void MemcachedConfigImp::load(FILE *fp){
	loaded = true;
	TiXmlDocument doc;
	doc.LoadFile(fp);

	TiXmlHandle docH( &doc );

	//conf.memcacheds.memcached
	TiXmlElement *memcached_element = docH.FirstChildElement("conf").FirstChildElement("memcacheds").FirstChildElement("memcached").Element();
	while(memcached_element){
		string name = memcached_element->Attribute("name");
		TiXmlElement *host_element = memcached_element->FirstChildElement("host");
		while(host_element){
			string ip = host_element->Attribute("ip");
			uint16_t port = atoi(host_element->Attribute("port"));

			memcachedParam param;
			param.ip = ip;
			param.port = port;
			memcachedInfo[name].push_back(param);

			log(Info, "memcached name: %s, ip: %s, port: %d", name.data(), ip.data(), port);
			host_element = host_element->NextSiblingElement("host");
		}
		

		memcached_element = memcached_element->NextSiblingElement("memcached");
	}

	forEachWatcher0(&IMemcachedWatcher::onMemcachedConfRefreshed);
}

void MemcachedConfigImp::getMemcachedInfo(std::map<std::string, std::vector<memcachedParam> >& info){
	log(Info, "getMemcachedInfo");
	info = memcachedInfo;
}

void MemcachedConfigImp::getMemcachedInfoByName(const std::string& name, std::vector<memcachedParam>& info){
	//log(Info, "getMemcachedInfoByName");
	info = memcachedInfo[name];
}
