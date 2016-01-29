#include <cstdio>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "example.h"
#include "default_watcher.h"
#include "libzkclient.h"
#include "core/sox/logger.h"

using namespace libzk;
using namespace std;

void MyZkClient::onDataCompletion(int ret, const char *data,int len, void * user_data)
{
    if(0 != ret)
    {
        printf("MyZkClient::_OnDataCompletion|ret|%d\n",ret);
        return;
    }
}


const char* state2string(int p_iState)
{
    if (p_iState == 0)
        return "CLOSED_STATE";
    if (p_iState == ZOO_CONNECTING_STATE)
        return "CONNECTING_STATE";
    if (p_iState == ZOO_ASSOCIATING_STATE)
        return "ASSOCIATING_STATE";
    if (p_iState == ZOO_CONNECTED_STATE)
        return "CONNECTED_STATE";
    if (p_iState == ZOO_EXPIRED_SESSION_STATE)
        return "EXPIRED_SESSION_STATE";
    if (p_iState == ZOO_AUTH_FAILED_STATE)
        return "AUTH_FAILED_STATE";

    return "INVALID_STATE";
}

const char* type2string(int p_iType)
{
    if (p_iType == ZOO_CREATED_EVENT)
        return "CREATED_EVENT";
    if (p_iType == ZOO_DELETED_EVENT)
        return "DELETED_EVENT";
    if (p_iType == ZOO_CHANGED_EVENT)
        return "CHANGED_EVENT";
    if (p_iType == ZOO_CHILD_EVENT)
        return "CHILD_EVENT";
    if (p_iType == ZOO_SESSION_EVENT)
        return "SESSION_EVENT";
    if (p_iType == ZOO_NOTWATCHING_EVENT)
        return "NOTWATCHING_EVENT";

    return "UNKNOWN_EVENT_TYPE";
}
void node_watcher_fn(zhandle_t *zk_handle, int p_iType, int p_iState, 
        const char *path, void *p_pWatcherCtx)
{
    printf("watcher_fn|%s|%s\n", type2string(p_iType), state2string(p_iState));
}
void zktest_dump_stat(const struct Stat *stat)
{
    char tctimes[40];
    char tmtimes[40];
    time_t tctime;
    time_t tmtime;

    if (!stat) {
        fprintf(stderr,"null\n");
        return;
    }
    tctime = stat->ctime/1000;
    tmtime = stat->mtime/1000;
    ctime_r(&tmtime, tmtimes);
    ctime_r(&tctime, tctimes);
       
    fprintf(stderr, "\tctime = %s\tczxid=%llx\n"
    "\tmtime=%s\tmzxid=%llx\n"
    "\tversion=%x\taversion=%x\n"
    "\tephemeralOwner = %llx\n",
     tctimes, stat->czxid,
     tmtimes, stat->mzxid,
    (unsigned int)stat->version, (unsigned int)stat->aversion,
    stat->ephemeralOwner);
}
struct Watchertx{
    zhandle_t * zh;
    const char *path;
    watcher_fn watcher;
    stat_completion_t completion;
};
void stat_completion(int rc,const struct Stat *stat, const void *data)
{
    fprintf(stderr, "%s: rc = %d Stat:\n", (char*)data, rc);
    zktest_dump_stat(stat);
    /*
    Watchertx* ptr=(Watchertx*) data;
    int wflag =  zoo_awexists(ptr->zh, ptr->path, ptr->watcher, NULL,stat_completion,(void*) ptr);
    if(wflag!=ZOK){
        printf("watching..faild..path: %s result=%d\n",ptr->path,wflag);
        return ;
    }else{
        printf("watching..success..path: %s\n",ptr->path);
    }
    */
}

int main()
{
	std::string proc_name = "vip_vcard";
	ZkConfigManager config_manager(proc_name);
	DefaultWatcher *action =  new DefaultWatcher();
	ZkClient zk;
	zk.setWatcherAction(action);
	if(0 != zk.connect("127.0.0.1:2181"))
	{
		log(Info,"could not connect to zookeeper\n");
		exit(0);
	}
	config_manager.setZkClientManager(&zk);
	std::string redis_name="redis_master1";
	config_manager.RegisterConfig(kTypeThrift,proc_name);
	config_manager.RegisterConfig(kTypeRedis,redis_name);
	config_manager.Init();
	std::string ip = config_manager.GetConfigIp("/thrift/vip_vcard");
	int32_t port = config_manager.GetConfigPort("/thrift/vip_vcard");
	printf("===============ip=%s,port=%d============\n",ip.c_str(),port);
    return 0;
}
