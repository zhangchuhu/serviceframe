#include "TinyXmlServerConfigImp.h"
#include "TinyXmlDbConfigImp.h"
#include <cassert>
#include <map>
#include <string>


using namespace server::config;

bool checkDb(DatabaseParam &param, const std::string &host){
	return param.host == host && param.db == "smanager" && param.user == "user" && param.port == 3306 && param.passwd == "passwd";
}

int main(){
	FILE *fp = fopen("./server.xml", "r");

	TinyXmlServerConfigImp config1("linkd");
	config1.load(fp);
/*
	std::vector<std::pair<std::string, std::string> >  ips = config1.getAllowIps();
	assert(ips.size() == 3);
	assert(ips[0].first == "01" && ips[0].second == "02");
	assert(ips[1].first == "11" && ips[1].second == "");
	assert(ips[2].first == "" && ips[2].second == "22");
*/

	assert(config1.getPort() == 443);
	std::vector<uint16_t> frontPorts = config1.getFrontPorts();
	assert(frontPorts.size() == 3);
	assert(frontPorts.front() == 81);
	assert(frontPorts.back() == 8081);

	assert(config1.getName() == "_lk");
//	assert(config1.isHangup() == false);

	TinyXmlServerConfigImp config2("lbsd");
	config2.load(fp);

//	assert(config2.isHangup() == true);
	
	TinyXmlConfigDbd config3("smanager");
	config3.load(fp);
	assert(config3.getDefaultName().first == "master" && config3.getDefaultName().second == "slave");
//	assert(config3.isHangup() == false);
	assert(config3.getDefaultId() == 123);

	TinyXmlDbConfigImp config4;
	config4.load(fp);
	
	std::map<std::string, std::vector<DatabaseParam> > dbs = config4.getDatabases();
	std::vector<DatabaseParam>  &master = dbs["smanger_master"];

	assert(master.size() == 2);
	assert(checkDb(master[0], "host1"));
	assert(checkDb(master[1], "host2"));
	assert(checkDb(dbs["smanger_slave"][0], "host3"));

	fclose(fp);
	return 0;
}
