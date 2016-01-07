#ifndef CLIENT_DAEMON
#define CLIENT_DAEMON
#include "iclient.h"
#include "common/core/iserver.h"
#include "common/core/ibase.h"
#include "server_common/protocol/daemon.h"
#include "common/protocol/prouter.h"
#include "server_common/protocol/prouter.h"
#include "core/corelib/MultiConnManagerImp.h"
#include "server_common/helper/TimerHandler.h"
#include "server_common/protocol/PImRouter.h"
#include "core/sox/mutex.h"
#include <time.h>


namespace core{

class DaemonClient;

class NameChannel{
	std::string name;
	DaemonClient *client;
	uint32_t serverId;
public:
	NameChannel();

	void init(const std::string &n, DaemonClient *c);

	bool randomServer();
	bool checkRuning();

	int send(uint32_t uri, sox::Marshallable &obj);
	uint32_t getServerId();
private:
	TimerHandler<NameChannel, &NameChannel::checkRuning> chkt;
	TimerHandler<NameChannel, &NameChannel::randomServer> pt;
};

struct CSrvRollingInfo 
{
	CSrvRollingInfo() 
	{
		m_uCounter = 0;
	}
	std::vector<uint32_t>::size_type  m_uCounter;
	std::vector<uint32_t> m_vecSrvId;
};

class DaemonClient: public PHClass, 
					public IFormTarget, 
					public IDaemonClient,
					public MultiConnManagerImp,
					public IServerIdDispatcher,
					public IDaemonServerAware,
					public IProcStatAware{
protected:
	std::set<IClientWatcher *>watchers;

//	std::map<uint32_t, sdaemon::FavorStr> sid2Favor;	//旧协议，只关注RUNNING

	std::map<uint32_t, sdaemon::FavorStrSV> sid2FavorSV; //新协议，关注全部状态

	// 只关注RUNNING状态，用于轮询发包
	typedef std::map<std::string, CSrvRollingInfo> MapSuffix2Srvs_T;
	MapSuffix2Srvs_T m_mapSuffix2Srvs;  

	typedef std::map<std::string, NameChannel *> name_channel_t;

	name_channel_t ncs;

	std::map<uint32_t, uint32_t> sid2cid;

	sdaemon::FavorStrSV nullServerSV;

	std::string routeName;

	bool status;

	//key:dest gid, value:imbridge server id
	typedef std::map<uint32_t, uint32_t> MapGid2Route_T;
	MapGid2Route_T m_mapGid2Route;

	//key:gid, value:imrouter count
	std::map<uint32_t, uint32_t> m_mapImrouterCount;

public:
	DECLARE_FORM_MAP

	DaemonClient();

	~DaemonClient();

	virtual ILinkHandler *getLinkHandler(){
		return handler;
	}

	virtual void startSV();

	virtual void watch(IClientWatcher *);

	virtual void eraseConnect(IConn *conn);

	void onServerAdd(sdaemon::PServerAddStr *);
	void onServerRemoveSV(sdaemon::PServerRemove2*);
	void onGetServersSV(sdaemon::PRegisterFavourStrResSV *, uint32_t resCode);	
	void onServersStatusChangeSV(sdaemon::PServersStatusChangeSV *, uint32_t resCode);	

	//void onDns(server::router::PDnsRes *dns, uint32_t resCode);

	//void onDnsChange(server::router::PDnsChange *dns);

	void onPing(sdaemon::PServerPing2Res *);

	void onRouteNotify(server::imrouter::PSS_RouteNotifyServer *p);

	virtual bool dispatchByServerIdSV(uint32_t serverId, uint32_t uri, sox::Marshallable &);
	
	virtual uint32_t dispatchToServerRandom(const std::string &name, uint32_t uri, sox::Marshallable &);

	// 轮询的方式平均发送给服务器
	virtual void DispatchToServerRoundRobin(const std::string& strSuffix, uint32_t uUri, sox::Marshallable& obj);

	virtual void dispatchToServers(const std::string &name, uint32_t uri, sox::Marshallable &);
	virtual sdaemon::FavorStrSV getServerSV(uint32_t);
	virtual std::vector<uint32_t> getServerIdsSV(const std::string &name);
	virtual void onServerRegisteredSV(const std::vector<sdaemon::FavorStrSV> &fs, bool bRecover);
	virtual void changeMyServerStatusSV(sdaemon::ServerStatus status) ;
	virtual std::vector<sdaemon::FavorStrSV> getServersByNameSV(const std::string& name) ;	
	virtual std::vector<sdaemon::FavorStrSV> getServersByGroupSV(uint32_t groupID) ;
	virtual std::vector<sdaemon::FavorStrSV> getServersSV();
	virtual std::vector<uint32_t> getServerIdsByName(const std::string &name) ;	//SV
	virtual bool isInTheSameGroupSV(uint32_t groupID) const;	
	virtual bool isMeSV(uint32_t serverID) const;	
	virtual sdaemon::ServerStatus getMyServerStatusSV(void) const;	
	
	//virtual void routeB(const char *name, uint32_t uri, sox::Marshallable &obj);
	//virtual void batchRoute(const std::vector<uint32_t> &uids, uint32_t uri, const sox::Marshallable &);
//	bool ping();
	bool pingSV();	

	bool checkDaemon();

	bool lbsDns();

	bool reportLoad();

	void onLbsDns(sdaemon::PLbsDnsRes *);
	void onServerAddSV(sdaemon::PServerAddStrSV *fs) ;
	bool isServerRunning(uint32_t serverId);

	void setDaemonConnectWatcher(IDaemonConnectStatus *s){
		dcStatus = s;
	}
	size_t get_output_size(uint32_t serverId);
protected:
	IConn *connectServerIdSV(uint32_t serverId);
	//void realStart(ILinkHandler *);
	std::string getName();

private:
	
	void __SuffixMapDel(uint32_t uSrvId, const std::string strSuffix);
	void __SuffixMapAdd(const sdaemon::FavorStrSV& obj);

private:

	enum DaemonStat{
		connected,
		broken,
		pinging
	} dStart;

	TimerHandler<DaemonClient, &DaemonClient::checkDaemon> ct;
	TimerHandler<DaemonClient, &DaemonClient::reportLoad> rpt;
	TimerHandler<DaemonClient, &DaemonClient::pingSV> pSVt;
	
	int	 recoverStatus; //xxxx 
	int recoverSeq;

	IDaemonConnectStatus *dcStatus;
	//TimerHandler<DaemonClient, &DaemonClient::lbsDns> lbst;

	/**
	 * The following members for Daemon behaviour optimization.
	 **/
    time_t m_LastPingResTime;//Recored the last PingRes arrives time.
	bool m_IsProcessReady;//The flag indicate process ready or not.
	
public:
	sox::mutex m_MutexRoute;
	uint64_t m_sendstat;

	void incStat(uint32_t size)
	{
		uint64_t utmp = size;
		m_sendstat += utmp;
	}
    uint64_t GetSendStat()
    {
		uint64_t data = m_sendstat;
		m_sendstat = 0;
		return data;
    }
	
	bool getBridgeByDstSid(uint32_t uDstServerId, uint32_t &uBridgeIp, uint16_t &uBridgePort);

    /**
     * Check if the lastest daemon connection timeout, 150s.
     **/
    bool __IsDaemonConnTimeout(uint32_t &uTimeIntv);
	void __SetProcessReadyFlag(const bool isReady);
	
};
}
#endif

