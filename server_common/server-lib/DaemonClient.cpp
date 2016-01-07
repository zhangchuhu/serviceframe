#include "DaemonClient.h"
#include "common/core/form.h"
#include "core/sox/sockethelper.h"
#include "core/sox/logger.h"
#include "core/sox/udpsock.h"
#include "core/corelib/AbstractConn.h"
#include "core/corelib/InnerConn.h"
#include "id_def.h"
#include "UrlHelper.h"
#ifndef WIN32
#include <signal.h>
#include <unistd.h>
#endif
using namespace core;
using namespace sox;
using namespace sdaemon;
using namespace protocol;
using namespace server::router;
using namespace common::router;
using namespace server::imrouter;


namespace
{
	const char* const DAEMON_HOST		= "balance.yy.duowan.com";
	const uint32_t DAEMON_PORT			= 2000;
	const uint32_t PING_DAEMON_TIME		= 1000 * 10;
	const uint32_t CHECK_DAEMON_TIME	= 1000 * 50 * 1;
	const uint32_t REPORT_TIME			= 1000 * 60 * 5;
	const uint32_t LBS_DNS_LOW			= 1000 * 60 * 3;
	const uint32_t LBS_DNS_HIGH			= 1000 * 60 * 5;
	const uint32_t RANDOM_TIME			= 1000 * 60 * 30;
	const uint32_t CHECKRUNNING_TIME	= 1000 * 10;

	/**
	 * This marco for Daemon behaviour optimization.
	 **/
	const uint32_t MAX_DAEMON_CONN_TIMEOUT = 150; //60 * 3 - 30 = 150s

	std::string Status2String(ServerStatus status)
	{
		switch (status)
		{
		case INITIAL:
			return "INITIAL";
		case WAITING:
			return "WAITING";
		case READY:
			return "READY";
		case RUNNING:
			return "RUNNING";
		default:
			return "";
		}
	}
}

BEGIN_FORM_MAP(DaemonClient)
	ON_REQUEST(PServerAddStrSV, &DaemonClient::onServerAddSV)	
	ON_REQUEST(PServerRemove2, &DaemonClient::onServerRemoveSV)
	ON_RESPONSE(PRegisterFavourStrResSV, &DaemonClient::onGetServersSV)		//接收来自daemon.initCheck的全局	
	ON_RESPONSE(PServersStatusChangeSV, &DaemonClient::onServersStatusChangeSV)		//有server状态发生变化
	
/*
#ifdef STATUS_NEWPROTOCOL
	
#else
	ON_REQUEST(PServerAddStr, &DaemonClient::onServerAdd)			 
	ON_REQUEST(PServerRemove, &DaemonClient::onServerRemove)
	ON_RESPONSE(PRegisterFavourStrRes, &DaemonClient::onGetServers)		//接收来自daemon.initCheck的全局
 #endif
 */
	//ON_RESPONSE(PDnsRes, &DaemonClient::onDns)
    ON_REQUEST(PServerPing2Res, &DaemonClient::onPing)
	//ON_REQUEST(PLbsDnsRes, &DaemonClient::onLbsDns)

    ON_REQUEST(PSS_RouteNotifyServer, &DaemonClient::onRouteNotify)	
END_FORM_MAP()

int randomLbsTime(){
	 return (int)((rand()/(float)RAND_MAX) * (LBS_DNS_HIGH - LBS_DNS_LOW)) + LBS_DNS_LOW;
}
bool isCanSend =false;
void CatchSig(int signo)
{
	log(Info, "[%d] catch sig:%d\n ====> gogogo", getpid(), signo);
	isCanSend = true;
}

 
DaemonClient::DaemonClient()
	: ct(this)
	, rpt(this)
	, pSVt(this)
{
	nullServerSV.serverId = NONE_SERVER_ID;

	pSVt.start(PING_DAEMON_TIME);
	ct.start(CHECK_DAEMON_TIME);	
	rpt.start(REPORT_TIME);

	int *xx = new int;
	srand(static_cast<unsigned int>((unsigned long)xx));
	delete xx;

	dStart = broken;
	signal(SIGUSR1, CatchSig);
	isCanSend = false;
	recoverSeq = 0;
	m_sendstat = 0;

	m_LastPingResTime = 0;
	m_IsProcessReady = false;
}

DaemonClient::~DaemonClient(){
	for(name_channel_t::iterator it = ncs.begin(); it != ncs.end(); ++it){
		delete it->second;
	}
	ncs.clear();
}

void DaemonClient::onServerAddSV(PServerAddStrSV *fs) {
	log(Info, "DaemonClient::onServerAddSV-->groupId=%d,serverId=%u,status=%s"
		, fs->groupId, fs->serverId, Status2String(fs->serverStatus).c_str());

	FavorStrSV ff = *fs;

	//record what group has how many imrouter
	if (0 == ff.sstr.compare(SUFFIX_IMROUTER))
	{
		sox::scope_lock sllk(m_MutexRoute);
		std::map<uint32_t, uint32_t>::iterator itCount = m_mapImrouterCount.find(ff.groupId);
		if (itCount != m_mapImrouterCount.end())
		{
			itCount->second++;
		}
		else
		{
			m_mapImrouterCount[ff.groupId] = 1;
		}

		log(Info, "DaemonClient::onServerAddSV: group(%d)'s imrouter count is %d", ff.groupId, m_mapImrouterCount[ff.groupId]);	
	}
	
	__SuffixMapDel(ff.serverId, ff.sstr);
	__SuffixMapAdd(ff);
	std::map<uint32_t, sdaemon::FavorStrSV>::iterator it = sid2FavorSV.find(fs->serverId);	
	if(it != sid2FavorSV.end()){
		if(ff == it->second){
			log(Warn, "SV-re add server id:%u", ff.serverId);
			return;
		}else{
			log(Warn, "SV-readd and server info changed!!! bug bug");
		}
	}
	sid2FavorSV[fs->serverId] = ff;	
	log(Info, "SV-watchers.size():%d", watchers.size());	
	if (watchers.size() != 0) {
		for (std::set<IClientWatcher *>::iterator it = watchers.begin(); it
			!= watchers.end(); ++it) {
				log(Debug, "SV-add server id:%u", fs->serverId);
				(*it)->onAddSV(*fs);
			}
	}	
}

void DaemonClient::watch(IClientWatcher *w){
	watchers.insert(w);
}
void DaemonClient::onServerRemoveSV(PServerRemove2 *rm) 
{
	log(Info, "SV-===> DaemonClient::onServerRemoveSV");
	std::map<uint32_t, FavorStrSV>::iterator fit = sid2FavorSV.find(rm->serverId);
	if(fit != sid2FavorSV.end()){
		FavorStrSV fstr = fit->second;
		__SuffixMapDel(fstr.serverId, fstr.sstr);

		if (0 == fstr.sstr.compare(SUFFIX_IMROUTER))		
		{
			sox::scope_lock sllk(m_MutexRoute);
			bool bCountIsZero = false;
			//if the removed imrouter is not in my group, just adjust the set
			std::map<uint32_t, uint32_t>::iterator itCount = m_mapImrouterCount.find(fstr.groupId);
			if (itCount != m_mapImrouterCount.end())
			{
				itCount->second--;
				if(0 == itCount->second)
				{
					m_mapImrouterCount.erase(itCount);
					bCountIsZero = true;
				}

				log(Info, "DaemonClient::onServerRemoveSV: group(%d)'s imrouter count is %d", itCount->first, itCount->second);	
			}
	
			//if all imrouter of my group is removed, disable all route.
			if (server && (server->getGroupId() == fstr.groupId) && (bCountIsZero))
			{
				m_mapGid2Route.clear();
				log(Info, "DaemonClient::onServerRemoveSV: clear my route list");
			}
		}

		sid2FavorSV.erase(fit);
		log(Debug, "SV-onServerRemoveSV remove server id:%u name=%s", rm->serverId, fstr.sstr.c_str());
		for (std::set<IClientWatcher *>::iterator it = watchers.begin(); it
			!= watchers.end(); ++it) {
				(*it)->onServerRemovedSV(fstr);
			}		
	}else{
		log(Error, "SV-DaemonClient favor:%u removed", rm->serverId);
	}

	std::map<uint32_t, uint32_t>::iterator it = sid2cid.find(rm->serverId);
	if(it != sid2cid.end()){
		eraseConnectById(it->second);	//call MultConnManagerImp::eraseConnectById
		//sid2cid.erase(it);
	}else{
		log(Warn, "SV-DaemonClient cid:%u removed", rm->serverId);
	}

	
}


void DaemonClient::eraseConnect(IConn *conn){
	if(conn){
		std::vector<uint32_t> sids;
		for(std::map<uint32_t, uint32_t>::iterator it = sid2cid.begin(); it != sid2cid.end(); ++it){
			if(it->second == conn->getConnId()){
				sids.push_back(it->first);
			}
		}

		for(std::vector<uint32_t>::iterator it = sids.begin(); it != sids.end(); ++it){
			if(*it == DAEMON_SERVERID)
			{
				dStart = broken;
				dcStatus->onBroken();
				__SetProcessReadyFlag(false);
			}

			sid2cid.erase(*it);
		}
	}
	MultiConnManagerImp::eraseConnect(conn);
}

void DaemonClient::onServerRegisteredSV(const std::vector<FavorStrSV> &fs, bool bRecover){
	sid2FavorSV.clear();
	m_mapSuffix2Srvs.clear();

	{
		sox::scope_lock sllk(m_MutexRoute);
		m_mapGid2Route.clear();
		m_mapImrouterCount.clear();
	}

	for(std::vector<FavorStrSV>::const_iterator it = fs.begin(); it != fs.end(); ++it){
		log(Debug, "DaemonClient::onServerRegisteredSV-->prefix=%s,serverid=%u,groupid=%d,status=%s"
			,it->sstr.c_str(), it->serverId, it->groupId, Status2String(it->serverStatus).c_str());
		sid2FavorSV[it->serverId] = *it;
		__SuffixMapAdd(*it);
		
		//record what group has how many imrouter
		if (0 == it->sstr.compare(SUFFIX_IMROUTER))
		{
			sox::scope_lock sllk(m_MutexRoute);
			std::map<uint32_t, uint32_t>::iterator itCount = m_mapImrouterCount.find(it->groupId);
			if (itCount != m_mapImrouterCount.end())
			{
				itCount->second++;
			}
			else
			{
				m_mapImrouterCount[it->groupId] = 1;
			}
			
			log(Info, "DaemonClient::onServerRegisteredSV: group(%d)'s imrouter count is %d", it->groupId, m_mapImrouterCount[it->groupId]);	
		}
	}
	
	if(bRecover){
		recoverStatus = true;
		for (std::set<IClientWatcher *>::iterator it = watchers.begin(); it	!= watchers.end(); ++it) {
			(*it)->onRecoverRefreshSV();
		}
		if(recoverStatus){
			changeMyServerStatusSV(RUNNING);
		}
	}else{
		for (std::set<IClientWatcher *>::iterator it = watchers.begin(); it	!= watchers.end(); ++it) {
			(*it)->onRefreshSV();		
		}
	}	
}


void DaemonClient::onGetServersSV(sdaemon::PRegisterFavourStrResSV *res, uint32_t resCode){
	if(Request::ifSuccess(resCode)){
		sid2FavorSV.clear();
		m_mapSuffix2Srvs.clear();
		for(std::vector<FavorStrSV>::iterator it = res->favours.begin(); it != res->favours.end(); ++it){
			log(Debug, "DaemonClient::onGetServersSV--> SUFFIX=%s,groupId:%u,serverId=%u,status=%s", 
				it->sstr.c_str(), it->groupId, it->serverId, Status2String(it->serverStatus).c_str()); 

			sid2FavorSV[it->serverId] = *it;
			__SuffixMapAdd(*it);
		}

		for (std::set<IClientWatcher *>::iterator it = watchers.begin(); it	!= watchers.end(); ++it) {
			(*it)->onRefreshSV();
		}
	}
	
}

void DaemonClient::onServersStatusChangeSV(sdaemon::PServersStatusChangeSV *ssc, uint32_t resCode){

	std::map<uint32_t, FavorStrSV>::iterator itr = sid2FavorSV.find(ssc->serverId);
	if(itr != sid2FavorSV.end())
	{
		log(Info, "DaemonClient::onServersStatusChangeSV-->serverId=%u,status=%s", ssc->serverId, Status2String(ssc->serverStatus).c_str());
		
		__SuffixMapDel(itr->second.serverId, itr->second.sstr);
		itr->second.serverStatus = ssc->serverStatus;
		itr->second.sstr = ssc->name;
		__SuffixMapAdd(itr->second);
		
		for (std::set<IClientWatcher*>::iterator it = watchers.begin(); it	!= watchers.end(); ++it) 
		{
			(*it)->onServerStatusChangeSV(itr->second);
		}
	}
	else
	{
		log(Info, "!!! bug bug. DaemonClient::onServersStatusChangeSV. ssc->serverId can't be found in sid2FavorSV");
	}
}

void DaemonClient::startSV(){	//加入初始值，功能上跟旧协议一样，只是应用的消息承载体不一样，新协议消息称载体包含状态信息还有机房组号
	FavorStrSV fs;
	struct hostent *host = gethostbyname(DAEMON_HOST);
	fs.dip = *(u_long *)host->h_addr;
	fs.wip = 0;
	fs.serverId = DAEMON_SERVERID;
	fs.port = DAEMON_PORT;
	//if not exist DaemonServer  don't send gourpId
	if(server){
		fs.groupId = server->getGroupId();
		log(Info, "----- groupid:%d", server->getGroupId());
	}else{
		fs.groupId = 0;
	}
	fs.serverStatus = INITIAL;
	sid2FavorSV[DAEMON_SERVERID] = fs;

	//realStart(handler);
}

std::vector<sdaemon::FavorStrSV> DaemonClient::getServersByNameSV(const std::string& name) 
{
	std::vector<sdaemon::FavorStrSV> ret;
	for (std::map<uint32_t, sdaemon::FavorStrSV>::iterator itr = sid2FavorSV.begin(); 
		itr != sid2FavorSV.end();
		++itr)
	{
		if (name == itr->second.sstr)
			ret.push_back(itr->second);
	}
	
	return ret;	
}

std::vector<sdaemon::FavorStrSV> DaemonClient::getServersByGroupSV(uint32_t groupID) 
{
	// TODO: imp
	assert(false);

	std::vector<sdaemon::FavorStrSV> t;
	return t;		
}
std::vector<sdaemon::FavorStrSV> DaemonClient::getServersSV()
{
	std::vector<sdaemon::FavorStrSV> ret;
	for (std::map<uint32_t, sdaemon::FavorStrSV>::iterator itr =  sid2FavorSV.begin(); 
		itr != sid2FavorSV.end();
		++itr)
	{
		ret.push_back(itr->second);
	}

	return ret;	
}

std::vector<uint32_t> DaemonClient::getServerIdsByName(const std::string &name) 	//SV
{
	// TODO: imp
	assert(false);

	std::vector<uint32_t> t;
	return t;
}

bool DaemonClient::isInTheSameGroupSV(uint32_t groupID) const
{	
	return (server->getGroupId() == groupID);
}

bool DaemonClient::isMeSV(uint32_t serverID) const
{
	uint32_t nMyServerId = server->getServerId();
	return serverID == nMyServerId;
}

sdaemon::ServerStatus DaemonClient::getMyServerStatusSV(void) const
{
	uint32_t nMyServerId = server->getServerId();
	std::map<uint32_t, sdaemon::FavorStrSV>::const_iterator itr = sid2FavorSV.find(nMyServerId);
	if (itr != sid2FavorSV.end())
	{
		return itr->second.serverStatus;
	}

	log(Info, "!!!!!!! =======>  DaemonClient::getMyServerStatusSV(void): Can't find me in sid2FavorSV");
	assert(false);

	return INITIAL;
}

// MapSuffix2Srvs 处理函数
void DaemonClient::__SuffixMapAdd(const sdaemon::FavorStrSV& fs)
{
	if (fs.serverStatus == RUNNING)
	{
		m_mapSuffix2Srvs[fs.sstr].m_vecSrvId.push_back(fs.serverId);
	}
}

void DaemonClient::__SuffixMapDel(uint32_t uSrvId, const std::string strSuffix)
{
	MapSuffix2Srvs_T::iterator itMap = m_mapSuffix2Srvs.find(strSuffix);
	if (itMap != m_mapSuffix2Srvs.end())
	{
		for (std::vector<uint32_t>::iterator it = itMap->second.m_vecSrvId.begin(); it != itMap->second.m_vecSrvId.end(); ++ it)
		{
			if (*it == uSrvId)
			{
				itMap->second.m_vecSrvId.erase(it);
				break;
			}
		}
	}
}

/**---------------------------------------------------------------------------
* Function	   : __IsDaemonConnTimeout
*
* Description  : This routine to check if Daemon connection has been broken for more than 150s.
*
* @param	   : uTimeIntv - the elapsed time 
*				
* @return	   : true - more than 150s. false - less than 150s.
*---------------------------------------------------------------------------*/
bool DaemonClient::__IsDaemonConnTimeout(uint32_t &uTimeIntv)
{
	uTimeIntv = (uint32_t)(time(NULL)-m_LastPingResTime);
	return ((uTimeIntv >= MAX_DAEMON_CONN_TIMEOUT) ? true : false);
}

/**---------------------------------------------------------------------------
* Function	   : __SetProcessReadyFlag
*
* Description  : This routine to set the process working ready flag.
*
* @param	   : isReady - the flag indicate the process is working ready or not.
*				
* @return	   : N/A
*---------------------------------------------------------------------------*/
void DaemonClient::__SetProcessReadyFlag(const bool isReady)
{
	log(Info, "[DaemonClient::%s]Update process ready indication(%u->%u).", __FUNCTION__, (uint32_t)m_IsProcessReady, (uint32_t)isReady);
	m_IsProcessReady = isReady;
}

class UdpClient: public sox::Udpsock, public AbstractConn{
public:
	UdpClient(const std::string & peerip, int peerport):sox::Udpsock(peerip, peerport)
	{
	}
	virtual void onRead(){}

	virtual void sendBin(const char *data, size_t sz, uint32_t uri){
		sendto(data, sz);
	}
	virtual void setEncKey(const unsigned char *key, size_t len){}
};

/*
std::vector<FavorStrSV> DaemonClient::getServersSV(){
	std::vector<FavorStrSV> fs;
	for(std::map<uint32_t, FavorStr>::iterator it = sid2FavorSV.begin(); it != sid2FavorSV.end(); ++it){
		fs.push_back(it->second);
	}
	return fs;
}
*/
bool DaemonClient::dispatchByServerIdSV(uint32_t serverId, uint32_t uri, sox::Marshallable &obj){
	Sender s(uri, obj);

	incStat(s.bodySize() + s.headerSize());

	std::map<uint32_t,uint32_t>::iterator it = sid2cid.find(serverId);
	if(it != sid2cid.end()){
		if(dispatchById(it->second, s)){	//USE OLD-PRO
			return true;
		}
	}else{
		IConn *c = connectServerIdSV(serverId);
		if(c){
			c->send(s);
			return true;
		}else{
			log(Error, "SV-create conn error serverid:%u", serverId);
		}
	}
	return false;
}


IConn *DaemonClient::connectServerIdSV(uint32_t serverId){
	FavorStrSV fa = getServerSV(serverId);
	IConn *conn = NULL;
	if(fa.serverId != NONE_SERVER_ID){
		uint16_t port = fa.port;
		std::string ip = "";
		if(server){
			if((server->getNetType() & CTL) == CTL && fa.dip != 0){
				ip = sox::addr_ntoa(fa.dip);
			}else if((server->getNetType() & CNC) == CNC && fa.wip != 0){
				ip = sox::addr_ntoa(fa.wip);
			}
		}
		if(ip.empty())	
			ip = fa.dip == 0 ? sox::addr_ntoa(fa.wip) : sox::addr_ntoa(fa.dip);
		
		

		conn = createClientConn(ip.data(), port, handler, this);

		if(conn){
			sid2cid[serverId] = conn->getConnId();
		}else{
			log(Error, "SV-connect to daemon error:%u", serverId);
		}
	}
	return conn;
}


/*
void DaemonClient::batchRoute(const std::vector<uint32_t> &uids, uint32_t uri, const sox::Marshallable &mo){
	PUidsRouter ur;
	Sender ss(uri, mo);
	ur.uids = uids;
	ur.ruri = uri;
	ur.load.assign(ss.body(), ss.bodySize());
	Sender s2(PUidsRouter::uri, ur);
	toRouter(s2);
}*/

FavorStrSV DaemonClient::getServerSV(uint32_t s){
	std::map<uint32_t, FavorStrSV>::iterator it = sid2FavorSV.find(s);
	if(it != sid2FavorSV.end()){
		return it->second;
	}else{
		return nullServerSV;
	}
}
std::vector<uint32_t> DaemonClient::getServerIdsSV(const std::string &name){
	std::vector<uint32_t> ids;
	for(std::map<uint32_t, sdaemon::FavorStrSV>::iterator it = sid2FavorSV.begin(); it != sid2FavorSV.end(); ++it){
		if(it->second.sstr == name){
			ids.push_back(it->first);
		}
	}
	return ids;
}


std::string DaemonClient::getName(){
	if(server==0)
	{
		return std::string("");
	}
	std::string tem;
	server->getFullName(tem);
	return tem;
}


bool DaemonClient::checkDaemon(){
	if(dStart == pinging){
		dStart = broken;
		dcStatus->onBroken();
	}else if(dStart == connected){
		dStart = pinging;
	}
	return true;
}

void DaemonClient::onPing(sdaemon::PServerPing2Res *p)
{
	dStart = connected;
	dcStatus->onConnect(p->isRecord);

	/**
	 * Record the latest pingRes timestamp as process ready.
	 */
	if(m_IsProcessReady)
	{
		m_LastPingResTime = time(NULL);
	}
}

//imrouter notify route update
void DaemonClient::onRouteNotify(server::imrouter::PSS_RouteNotifyServer *p)
{
	sox::scope_lock sllk(m_MutexRoute);
	for (MapGid2Route_T::iterator it = p->m_mapGidServerIds.begin();
		it != p->m_mapGidServerIds.end();
		it++)
	{
		//update the route
		if ((uint32_t)-1 != it->second)
		{		
			//imbridge is not 0, send packet to dest indirectly, insert the route.
			if (0 != it->second)
			{
				//check if the imbridge exist
				std::map<uint32_t, sdaemon::FavorStrSV>::iterator itSrv = sid2FavorSV.find(it->second);
				if (itSrv != sid2FavorSV.end())
				{
					//check if the imrouter exist
					std::map<uint32_t, uint32_t>::iterator itImrouterCount = m_mapImrouterCount.find(it->first);
					if (itImrouterCount != m_mapImrouterCount.end())
					{
						m_mapGid2Route[it->first] = it->second; 			
						log(Info, "[DaemonClient::onRouteNotify] add route to group(%d) via imbridge(%d)", it->first, it->second);
					}
					else
					{
						log(Info, "[DaemonClient::onRouteNotify] add route to group(%d) via imbridge(%d) fail, imrouter has been removed", 
							it->first, it->second);
					}
				}
				//imbridge does not exist
				else
				{
					log(Info, "[DaemonClient::onRouteNotify] add route to group(%d) via imbridge(%d) fail, imbridge has been removed", 
						it->first, it->second);
				}
			}
			//imbridge is 0, send packet to dest directly, clear the route.
			else
			{
				m_mapGid2Route.erase(it->first);
				log(Info, "[DaemonClient::onRouteNotify] add route to group(%d) via imbridge(%d)", it->first, it->second);
			}
		}		
		//imbridge is -1, disable the route and remove it
		else
		{
			m_mapGid2Route.erase(it->first);			
			log(Info, "[DaemonClient::onRouteNotify] remove route to group(%d)", it->first);
		}
	}
}

bool DaemonClient::pingSV(){
	if(dStart == broken){
		startSV();
	}
    PServerPing2 sp;
	if(server){
		sp.serverId = server->getServerId();
	}else{
		sp.serverId = 1;
	}

	if(!dispatchByServerIdSV(DAEMON_SERVERID, sp.uri, sp)){
		log(Info, "SV-daemon down");
		dStart = broken;
		dcStatus->onBroken();
	}
	return true;
}

//client端触发daemonclinet，向daemon发送指令
void DaemonClient::changeMyServerStatusSV(ServerStatus status) 
{
/*
	if (status == ServerData::READY)
	{
		log(Info , ">>>>>>DaemonClient::changeMyServerStatusSV sleep().....");
		//测试竞争
		while(!isCanSend);
		isCanSend = false;	
		log(Info, "<<<<<<<DaemonClient::changeMyServerStatusSV start...");

	}
*/

	if (NULL != server)
	{
		recoverStatus = false;
		if(!server->setInitStatus(status)){
			PChangeMyServerStatusSV cs;
			cs.serverId = server->getServerId();
			cs.serverStatus = status;
			dispatchByServerIdSV(DAEMON_SERVERID, PChangeMyServerStatusSV::uri, cs);	
			log(Info, "DaemonClient::changeMyServerStatusSV-->serverId=%u,status=%s", cs.serverId, Status2String(status).c_str());
		}else{
			log(Info, "start set status:%u", status);	
		}
	}else{
		assert(false);
	}
}
		

bool DaemonClient::reportLoad(){
	if(server){
		PLoadReport lr;
		lr.serverId = server->getServerId();
		lr.loadValue = stat->getAndRstIntevel();
		lr.total = stat->getTotal();
		dispatchByServerIdSV(DAEMON_SERVERID, PLoadReport::uri, lr);
	}
	return true;

}

/*
void DaemonClient::routeB(const char *name, uint32_t uri, sox::Marshallable &obj){
	PBroadCast pb;
	pb.sname = name;
	pb.ruri = uri;
	pb.resCode = RES_SUCCESS;
	pb.packLoad(obj);

	Sender s(PBroadCast::uri, pb);
	toRouter(s);
}*/

uint32_t DaemonClient::dispatchToServerRandom(const std::string &name, uint32_t uri, sox::Marshallable &obj){
	size_t pos = name.find_first_of('.');
	std::string rn = name;
	if(pos != std::string::npos){
		rn = name.substr(pos + 1);
	}

	
	name_channel_t::iterator it = ncs.find(rn);	

	NameChannel *ss = NULL;
	if(it != ncs.end()){
		ss = it->second;
	}else{
		NameChannel *nc = new NameChannel();
		nc->init(rn, this);
		nc->randomServer();
		ncs[rn] = nc;
		ss = nc;
	}

	if(uri == PServerIdRouter::uri || uri == PUidsRouter::uri){
		ss->send(uri, obj);
	}else{
		PServerIdRouter pr;
		pr.from = getName();
		pr.packLoad(obj);
		pr.ruri = uri;
		pr.resCode = RES_SUCCESS;
		pr.ttl = 2;
		ss->send(PServerIdRouter::uri, pr);
	}
	return ss->getServerId();
}

// 轮询的方式平均发送给服务器

void DaemonClient::DispatchToServerRoundRobin(const std::string& strSuffix, uint32_t uUri, sox::Marshallable& obj)
{
	MapSuffix2Srvs_T::iterator itMap = m_mapSuffix2Srvs.find(strSuffix);
	if (itMap == m_mapSuffix2Srvs.end() || itMap->second.m_vecSrvId.empty())
	{
		log (Error, "[DaemonClient::DispatchToServerRoundRobin] dispatch to server(%s) faild, srv not found.", strSuffix.data());
		return;
	}

	CSrvRollingInfo& tmpRes = itMap->second;
	std::vector<uint32_t>::iterator it = tmpRes.m_vecSrvId.begin() + (++tmpRes.m_uCounter % tmpRes.m_vecSrvId.size());

#ifdef _DEBUG_TEST_
	log (Info, "[DaemonClient::DispatchToServerRoundRobin] dispatch to server(%s)-id(%d) ok.", strSuffix.data(), *it);
#endif

	dispatchByServerIdSV(*it, uUri, obj);
}

void DaemonClient::dispatchToServers(const std::string &name, uint32_t uri, sox::Marshallable &obj){
	std::vector<uint32_t> ids = getServerIdsSV(name);
	for(std::vector<uint32_t>::iterator it = ids.begin(); it != ids.end(); ++it){
	dispatchByServerIdSV(*it, uri, obj);		

	}
}

bool DaemonClient::isServerRunning(uint32_t serverId){
	std::map<uint32_t, FavorStrSV>::iterator it = sid2FavorSV.find(serverId);
	if(it != sid2FavorSV.end()){
		return it->second.serverStatus == RUNNING;
	}else{
		return false;
	}
}

bool DaemonClient::getBridgeByDstSid(uint32_t uDstServerId, uint32_t &uBridgeIp, uint16_t &uBridgePort)
{
    sdaemon::FavorStrSV fa = getServerSV(uDstServerId);
    if (!server || fa.serverId == NONE_SERVER_ID)
    {
        return false;
    }
	sox::scope_lock sllk(m_MutexRoute);

	uint32_t uGid = fa.groupId;
	MapGid2Route_T::iterator it = m_mapGid2Route.find(uGid);
	if (it != m_mapGid2Route.end())
	{
		uint32_t uBridgeSid = 0;
		if (it->second != 0)
		{
			uBridgeSid = it->second;
			
			 sdaemon::FavorStrSV fs = getServerSV(uBridgeSid);
			if (fs.serverId != NONE_SERVER_ID)
			{
				uBridgeIp = fs.dip;
				uBridgePort = fs.port;
				return true;
			}
		}
	}
	return false;
}
size_t DaemonClient::get_output_size(uint32_t serverId){
	size_t rt = 0;
	//find cid by serverId
	std::map<uint32_t, uint32_t>::const_iterator it_sid2cid = sid2cid.find(serverId);
	if(it_sid2cid != sid2cid.end()){
		uint32_t cid = it_sid2cid->second;		
		connect_t::const_iterator it_cid2contion = connects.find(cid);
		if (it_cid2contion != connects.end()) {
			InnerConn * pic = dynamic_cast<InnerConn*>(it_cid2contion->second);
			rt = pic->get_output_buffer_size();
		}
	}
	
	return rt;
}

NameChannel::NameChannel():client(NULL), serverId(NONE_SERVER_ID), chkt(this), pt(this){
	pt.start(RANDOM_TIME);
	chkt.start(CHECKRUNNING_TIME);
}

void NameChannel::init(const std::string &n, DaemonClient *c){
	name = n;
	client = c;
}

bool NameChannel::checkRuning(){
	if(!client->isServerRunning(serverId)){
		randomServer();
	}
	return true;
}

bool NameChannel::randomServer(){
	//只对RUNNING的server发	
	//log(Info, "random server:%s", name.data());
	std::vector<uint32_t> mids = client->getServerIdsSV(name);
	std::vector<uint32_t> ids;

	for(std::vector<uint32_t>::iterator it = mids.begin(); it != mids.end(); ++it){
		if(client->isServerRunning(*it)){
			ids.push_back(*it);
		}
	}

	if(ids.empty()){
		if(serverId != NONE_SERVER_ID){
			log(Warn, "mid status send to waiting server:%d, name:%s", serverId, name.data());	
		}
		//serverId = NONE_SERVER_ID;
	}else{
		std::random_shuffle(ids.begin(), ids.end());
		serverId = ids.front();
		log(Info, "random server name:%s, id:%u", name.data(), serverId);
	}
	return true;
}

int NameChannel::send(uint32_t uri, sox::Marshallable &obj){
	int ret = 0;
	if(client->dispatchByServerIdSV(serverId, uri, obj)){
			return ret;			
	}
	randomServer();
	if(serverId != NONE_SERVER_ID){
		client->dispatchByServerIdSV(serverId, uri, obj);
	}else{
		log(Error, "name:%s retry error", name.data());
		ret = -1;
	}
	return ret;
}

uint32_t NameChannel::getServerId(){
	return serverId;
}
