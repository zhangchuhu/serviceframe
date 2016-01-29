#include <cstdio>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "zk_client.h"
#include "zk_watcher.h"
#include "core/sox/logger.h"


using namespace libzk; 
using namespace std;

ZkClient::ZkClient()
{
    zk_handle = NULL;
}

ZkClient::~ZkClient()
{
    close();
}
// watcher action implementation
void ZkClient::activeWatcher(zhandle_t *zh, int type, int state, const char *path,void* ctx){
    if(zh==0 || ctx==0) return;
    printf("activeWatcher|%s|%s\n", type2String(type), state2String(state));
    WatcherAction* action=(WatcherAction*)ctx;
    
    if(type==ZOO_SESSION_EVENT){
        if(state==ZOO_EXPIRED_SESSION_STATE)
            action->onSessionExpired(zh);
        else if(state==ZOO_CONNECTING_STATE)
            action->onConnectionLost(zh);
        else if(state==ZOO_CONNECTED_STATE)
            action->onConnectionEstablished(zh);
    }
    else
    {
        if(type==ZOO_CHANGED_EVENT)
        action->onNodeValueChanged(zh,path);
        else if(type==ZOO_DELETED_EVENT)
        action->onNodeDeleted(zh,path);
        else if(type==ZOO_CHILD_EVENT)
        action->onChildChanged(zh,path);
    }
    int wflag =  zoo_aexists(zh,"/xyza",1,ZkClient::statCompletion,NULL);
    if(wflag!=ZOK){
        printf("watching..faild..path: %s result=%d\n",path,wflag);
        return ;
    }else{
        printf("watching..success..path: %s\n",path);
    }
    action->setWatcherTriggered();    
}
void ZkClient::statCompletion(int rc,const struct Stat *stat, const void *data)
{

}

void ZkClient::setWatcherAction(WatcherAction* action)
{
    this->action = action;
}
int ZkClient::connect(const char *host_list)
{
    const int TIME_OUT = 30000;
    zk_handle = zookeeper_init(host_list, ZkClient::activeWatcher, TIME_OUT, 0, this->action, 0);
    if(NULL == zk_handle)
    {
        printf("zookeeper_init failed \r\n");
        return -1;
    }

    return 0;
}

int ZkClient::close()
{
    if(NULL != zk_handle)
    {
        zookeeper_close(zk_handle);
        zk_handle = NULL;
    }
    
    return 0;
}
NodeStat ZkClient::Stat(const std::string& path) {
	NodeStat stat;
	int32_t zoo_code = zoo_exists(zk_handle, path.c_str(), false, &stat);
	CHECK_ZOOCODE_AND_THROW(zoo_code);
	return stat;
}
void ZkClient::getNodeData(const std::string& path,std::string& value, bool watch) {

	NodeStat node_stat = Stat(path);

	value.resize(node_stat.dataLength);

	int len = value.size();
	int32_t zoo_code = zoo_get(zk_handle,
		path.c_str(),
		watch,
		const_cast<char*>(value.data()),
		&len,
		&node_stat);

	CHECK_ZOOCODE_AND_THROW(zoo_code);
}
std::string ZkClient::create(const std::string& path, const std::string& value, int flag) {
	log(Info,"[ZkClient::%s] path=%s,value=%s",__FUNCTION__,path.c_str(),value.c_str());
	std::string path_buffer;
	path_buffer.resize(path.size() + 64);

	int32_t zoo_code = zoo_create(zk_handle,
		path.c_str(),
		value.data(),
		value.size(),
		&ZOO_OPEN_ACL_UNSAFE,
		flag,
		const_cast<char*>(path_buffer.data()),
		path_buffer.size());

	CHECK_ZOOCODE_AND_THROW(zoo_code);

	path_buffer.resize(strlen(path_buffer.data()));
	return path_buffer;
}
void ZkClient::getChildren(const std::string& path,std::vector<std::string>& children,bool watch )
{
	struct String_vector child_vec;

	int32_t zoo_code = zoo_get_children(zk_handle, path.data(), watch, &child_vec);
	CHECK_ZOOCODE_AND_THROW(zoo_code);

	children.reserve(child_vec.count);
	printf("[ZkClient::getChildren] count = %d \n",child_vec.count);
	for (int i = 0; i < child_vec.count; ++i) {
		printf("[ZkClient::getChildren] data = %s \n",child_vec.data[i]);
		children.push_back(string(child_vec.data[i],strlen(child_vec.data[i])));

	}
}
void ZkClient::dataCompletion(int rc, const char *value, int len, 
        const struct Stat *stat, const void *data)
{
    if(NULL == data)
    {
		printf("ZkClient::statCompletion:Null Data\n");
        return;
    }

    UserData *ptr = reinterpret_cast<UserData*>(const_cast<void*>(data));
    if(NULL == ptr->self)
    {
		printf("ZkClient::statCompletion:NULL Self\n");
        return;
    }

    ptr->self->onDataCompletion(rc, value, len, ptr->user_data);

    delete ptr;
}

const char *ZkClient::state2String(int state)
{
    if (state == 0)
        return "CLOSED_STATE";
    if (state == ZOO_CONNECTING_STATE)
        return "CONNECTING_STATE";
    if (state == ZOO_ASSOCIATING_STATE)
        return "ASSOCIATING_STATE";
    if (state == ZOO_CONNECTED_STATE)
        return "CONNECTED_STATE";
    if (state == ZOO_EXPIRED_SESSION_STATE)
        return "EXPIRED_SESSION_STATE";
    if (state == ZOO_AUTH_FAILED_STATE)
        return "AUTH_FAILED_STATE";

    return "INVALID_STATE";
}

const char *ZkClient::type2String(int type)
{
    if (type == ZOO_CREATED_EVENT)
        return "CREATED_EVENT";
    if (type == ZOO_DELETED_EVENT)
        return "DELETED_EVENT";
    if (type == ZOO_CHANGED_EVENT)
        return "CHANGED_EVENT";
    if (type == ZOO_CHILD_EVENT)
        return "CHILD_EVENT";
    if (type == ZOO_SESSION_EVENT)
        return "SESSION_EVENT";
    if (type == ZOO_NOTWATCHING_EVENT)
        return "NOTWATCHING_EVENT";

    return "UNKNOWN_EVENT_TYPE";
}

bool ZkClient::isConnectedState()
{
    if(NULL == zk_handle)
    {
        return false;
    }
    
    if(ZOO_CONNECTED_STATE == zoo_state(zk_handle))
    {
        return true;
    }else
    {
        return false;
    }
}