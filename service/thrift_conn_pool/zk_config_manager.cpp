#include "zk_config_manager.h"

#include <sstream>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "core/sox/logger.h"


using namespace std;


const std::string kConfigHosts1 = "config1.zookeeper.com";
const std::string kConfigHosts2 = "config2.zookeeper.com";
const std::string kConfigHosts3 = "config3.zookeeper.com";

const string kConfigPort1 = "2181";
const string kConfigPort2 = "2182";
const string kConfigPort3 = "2183";

//const char hostPorts[] = kConfigHosts1.c_str()+":"+kConfigHosts1.c_str()+","+kConfigHosts2.c_str()+":"+kConfigPort2;

ZkConfigManager::ZkConfigManager(const std::string& proc_name )
{
	this->proc_name=proc_name;
	char hostPorts[128]={0};
	strncat(hostPorts,kConfigHosts1.c_str(),kConfigHosts1.length());
	strncat(hostPorts,":",1);
	strncat(hostPorts,kConfigPort1.c_str(),kConfigPort1.length());
	strncat(hostPorts,",",1);
	strncat(hostPorts,kConfigHosts2.c_str(),kConfigHosts2.length());
	strncat(hostPorts,":",1);
	strncat(hostPorts,kConfigPort2.c_str(),kConfigPort2.length());
	strncat(hostPorts,",",1);
	strncat(hostPorts,kConfigHosts3.c_str(),kConfigHosts3.length());
	strncat(hostPorts,":",1);
	strncat(hostPorts,kConfigPort3.c_str(),kConfigPort3.length());
	try
	{
	
		//string depend_path="/depend/"+proc_name;
		//zk_client->create(depend_path,proc_name,0);
	}
	catch (ZooException e)
	{
		log(Error,"[ZkConfigManager::%s]create node ZooException:%s",__FUNCTION__,e.what());
	}
	catch (...)
	{
		log(Error,"[ZkConfigManager::%s]create node exception",__FUNCTION__);
	}
	
}
ZkConfigManager::~ZkConfigManager()
{
}
bool ZkConfigManager::Init(  ) {
	std::set<std::string>::const_iterator cit = care_list.begin();
	for( ; cit != care_list.end(); ++cit ) {
		string  key = *cit;
		log(Info,"[ZkThriftConfig::%s] load  config,path=%s",__FUNCTION__,key.data());
		std::map<std::string,ZkConfigItem*>::iterator it = config_list_.find(key);

		if ( it != config_list_.end() ) {
			return 1;
		}

		ZkConfigItem* item = new ZkConfigItem();
		item->setZkClientManager(getZkClientManager());
  
		if ( !item->Init( key)  ) {

			delete item;

			return -1;
		}

	config_list_[key] = item; 
	}
}
ZkConfigItem* ZkConfigManager::GetConfigItem( const std::string& key )  {


	return config_list_[key];
}

std::string ZkConfigManager::GetConfigIp( const std::string& key ) {

	ZkConfigItem* item = GetConfigItem( key ) ;

	if ( NULL != item ) {

		return item->GetStringValue( kConfigIp );
	}

	return "";
}

int32_t ZkConfigManager::GetConfigPort( const std::string& key ) {

	ZkConfigItem* item = GetConfigItem( key ) ;

	if ( NULL != item ) {

		return item->GetIntValue( kConfigPort );
	}

	return 0;
}
void ZkConfigManager::RegisterConfig( const std::string& type,const std::string& name )
{
	try
	{
		string path;
		if (type.compare(kTypeThrift)==0)
		{
			path = "/thrift/"+name;
			care_list.insert(path);
			//服务注册
			string depend_path="/depend/"+name;
			zk_client->create(depend_path,name,0);
		}
		if (type.compare(kTypeRedis)==0)
		{
			path = "/redis/" + name;
			care_list.insert(path);
			//服务注册
			string depend_path="/depend/"+proc_name+"/"+name;
			zk_client->create(depend_path,name,0);
		}
	}
	catch (ZooException e)
	{
		log(Error,"[ZkConfigManager::%s]create node ZooException:%s",__FUNCTION__,e.what());
	}
	catch (...)
	{
		log(Error,"[ZkConfigManager::%s]create node exception",__FUNCTION__);
	}

	

}

