#include "zk_thrift_config.h"
#include <vector>

#include "core/sox/logger.h"


using namespace std;
using namespace server::config;

ThriftConfigLoader::ThriftConfigLoader(const std::string& proc_name)
{
	this->proc_name=proc_name;
}
ThriftConfigLoader::~ThriftConfigLoader()
{

}
std::string ThriftConfigLoader::getParentPath()
{
	string path;
	path="/proc/"+proc_name;
	return path;
}
int ThriftConfigLoader::loadThriftConfig()
{
	log(Info,"[ThriftConfigLoader::loadConfig] load thrift config,proc=%s",proc_name.data());
	std::vector<string> children;
	zk_client->getChildren(getParentPath(),children,false);
	for (uint32_t i=0;i<children.size();i++)
	{
		string value;
		string path = getParentPath()+"/"+children[i];
		zk_client->getNodeData(path,value,false);
		config_item.insert(make_pair(children[i],value));
	}
	return 0;
}

uint32_t ThriftConfigLoader::getThriftPort()
{
	string key="port";
	map<string, string>::iterator it = config_item.find(key);
	if (it == config_item.end())
	{
		log (Error, "[ThriftConfig::getThriftPort] not found %s in zk config", proc_name.c_str());
		exit(0);
	}

	return atoi(it->second.data());
}
string ThriftConfigLoader::getThriftIp()
{
	string key="ip";
	map<string, string>::iterator it = config_item.find(key);
	if (it == config_item.end())
	{
		log (Error, "[ThriftConfig::getFrontPort] not found %s in zk config", proc_name.c_str());
		exit(0);
	}

	return it->second;
}