/*
 * @brief: C++ interface of Zooleeper client based on Zooleeper C client
 * @author: Rowan Zhu <rowanzhu@yeah.net>
 */
#ifndef _ZK_CPP_INTERFACE_
#define _ZK_CPP_INTERFACE_

#include "zk_watcher.h"
#include <zookeeper.h>
#include "zookeeper_error.h"
#include <string>
#include <vector>



namespace libzk
{

#define CHECK_ZOOCODE_AND_THROW(code)  \
	if (code != ZOK) { throw ZooException(code); }

typedef Stat NodeStat;

class WatcherAction;

class ZkClient
{
    public:
        ZkClient();

        virtual ~ZkClient();
    public:

		static const char *state2String(int p_iState);

		static const char *type2String(int p_iType);

		static void activeWatcher(zhandle_t *zh, int type, int state, const char *path,void* ctx);

    public:

        int connect(const char *p_szHostList);

        int close();

		bool isConnectedState();

		NodeStat Stat(const std::string& path);

		void getNodeData(const std::string& path,std::string& value,bool watch );

		void getChildren(const std::string& path,std::vector<std::string>& children,bool watch );


		std::string create(const std::string& path,const std::string& value = std::string(),int flag = 0);

        void setWatcherAction(WatcherAction* action);

    protected:

		virtual void onDataCompletion(int ret, const char *data,int len, void * user_data) {}
    private:
        struct UserData
        {
            ZkClient *self;
            void *user_data;
        };
		static void dataCompletion(int rc, const char *value, int value_len, const struct Stat *stat, const void *data);

        static void statCompletion(int rc,const struct Stat *stat, const void *data);

    private:
        zhandle_t *zk_handle;

    private:
        class WatcherAction *action;
};
class ZkClientAware {
protected:
	ZkClient *zk_client;
public:
	ZkClientAware() :
	  zk_client(NULL) {
	  }
	  virtual ~ZkClientAware() {
	  }
	  virtual void setZkClientManager(ZkClient *c) {
		  zk_client = c;
	  }

	  ZkClient *getZkClientManager() const{
		  return zk_client;
	  }
};

}
#endif
