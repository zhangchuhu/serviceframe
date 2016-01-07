#ifndef CORE_ICLIENT_H_
#define CORE_ICLIENT_H_
#include <vector>
#include "common/core/ilink.h"
#include "server_common/protocol/daemon.h"

namespace sdaemon{
	struct FavorStr;
	struct FavorStrSV;
}
namespace core {

struct IClientWatcher {
	virtual ~IClientWatcher() { }

	virtual void onRefreshSV() = 0;
	virtual void onRecoverRefreshSV()  = 0;
	virtual void onAddSV(const sdaemon::FavorStrSV & fs) = 0;
	virtual void onServerRemovedSV(const sdaemon::FavorStrSV & fs) = 0;
	virtual void onServerStatusChangeSV(const sdaemon::FavorStrSV & fs) = 0;
};

struct IClient: public ILinkHandlerAware{
	virtual ~IClient() {}

	virtual ILinkHandler *getLinkHandler() = 0;

};

struct IDaemonConnectStatus{
	virtual void onConnect(bool isRecord) = 0;
	virtual void onBroken() = 0;
};

struct IDaemonClient: public IClient{
	virtual ~IDaemonClient(){}

	virtual void watch(IClientWatcher *) = 0;

	virtual std::vector<sdaemon::FavorStrSV> getServersSV() = 0;

	virtual sdaemon::FavorStrSV getServerSV(uint32_t serverId) = 0;

	// 找出前缀为name的所有服务器的信息
	virtual std::vector<sdaemon::FavorStrSV> getServersByNameSV(const std::string& name) = 0;

	// 找出组groupID中的所有服务器信息
	virtual std::vector<sdaemon::FavorStrSV> getServersByGroupSV(uint32_t groupID) = 0;

	virtual void changeMyServerStatusSV(sdaemon::ServerStatus status) = 0;

	virtual void onServerRegisteredSV(const std::vector<sdaemon::FavorStrSV> &fs, bool bRecover) = 0;	

	// 判断和自己是否同组
	virtual bool isInTheSameGroupSV(uint32_t groupID) const = 0;

	// 判断是否是自己
	virtual bool isMeSV(uint32_t serverID) const = 0;

	virtual sdaemon::ServerStatus getMyServerStatusSV(void) const = 0;
};

struct IDaemonClientAware{
protected:
	IDaemonClient *client;
public:
	virtual ~IDaemonClientAware(){}
	virtual void setDaemonClient(IDaemonClient *d){
		client = d;
	}
};

struct IDaemonClientAwareW: public IDaemonClientAware, public IClientWatcher{
public:
	virtual ~IDaemonClientAwareW(){}
	virtual void setDaemonClient(IDaemonClient *d){
		client = d;
		client->watch(this);
	}
};
}
#endif /*ICLIENT_H_*/
