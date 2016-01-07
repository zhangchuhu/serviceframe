#include "DaemonServer.h"
#include "common/core/form.h"
#include "core/sox/logger.h"
#include "./id_def.h"
#include "core/sox/sockethelper.h"

using namespace core;
using namespace sdaemon;

#define UPDATE_PORT_SIZE 30

BEGIN_FORM_MAP(DaemonServer)
	ON_RESPONSE(PRegisterServerRes2SV, &DaemonServer::onServerRegister2SV)	
	ON_REQUEST(PAllowIps, &DaemonServer::onGetAllowIps)
	ON_RESPONSE(PRegFavRecStrResSV, &DaemonServer::onRecoverGetServersSV)		//接收来自daemon.initCheck的全局	
END_FORM_MAP()

ServerSocketHelper::ServerSocketHelper(SimplePrxServer *d, SOCKET so):sox::ServerSocket(so), imp(d){
	
}
void ServerSocketHelper::onAccept(SOCKET so, u_long ip, int port){
	imp->onAccept(so, ip, port);
}

void ServerSocketHelper::start(){
	select(0, sox::SEL_READ);
}

SimplePrxServer::~SimplePrxServer(){
	for(std::vector<ServerSocketHelper *>::iterator it = helpers.begin(); it != helpers.end(); ++it){
		delete *it;
	}
}

ServerSocketHelper *SimplePrxServer::create(const char* ip, uint16_t p, uint16_t &cur){
	sox::Sockethelper so;
	so.socket();
	so.setreuse();
	so.setblocking(false);
	so.setnodelay();
	so.bind(p, ip);

	return new ServerSocketHelper(this, so.detach());
}


ServerSocketHelper *SimplePrxServer::createHelper(const char *ip){
	if(!expectPorts.empty()){
		for(int i = 0; i < 1000; ++i){
			if(expectPorts.front() + i == 445)
				continue;
			try{
				uint16_t tmp;
				ServerSocketHelper *ret = create(ip, expectPorts.front() + i, tmp);

				curports.push_back(expectPorts.front() + i);
				log(Info, "create listen port %d success", expectPorts.front() + i);

				expectPorts.erase(expectPorts.begin());
				return ret;
			}catch(sox::socket_error se){
				log(Info, "alloc port conflict port: %u", expectPorts.front() + i);
			}
		}
	}

	return NULL;
}



void SimplePrxServer::refreshPorts(const std::vector<uint16_t> &ports){
	expectPorts = ports;
}

void SimplePrxServer::refreshPort(uint16_t port){
	expectPorts.clear();
	expectPorts.push_back(port);
}

std::vector<uint16_t> SimplePrxServer::getPorts(){
	return curports;
}

std::vector<uint16_t> SimplePrxServer::getUpdatePorts(){
	return updatePorts;
}

/*
void SimplePrxServer::setGroupId(uint32_t pGroupId)
{
	groupId = pGroupId;
}

uint32_t SimplePrxServer::getGroupId()
{
	return groupId;
}
*/

std::string SimplePrxServer::getIp(){
	return ip;
}

void SimplePrxServer::resetPorts(std::vector<uint16_t> ports)
{ 
#if 0
  for (std::vector<ServerSocketHelper *>::iterator it = helpers.begin(); it != helpers.end(); ++it) 
    delete (*it);
#endif

  helpers.clear();
  curports.clear(); 
  expectPorts.clear();

  std::copy(ports.begin(), ports.end(), std::back_inserter(expectPorts));

  while(!expectPorts.empty())
    helpers.push_back(createHelper(NULL));

  for (std::vector<ServerSocketHelper *>::iterator it = helpers.begin(); it != helpers.end(); ++it) 
    (*it)->start();
}

void SimplePrxServer::startSV()
{
	while(!expectPorts.empty())
		helpers.push_back(createHelper(NULL));

	for (std::vector<ServerSocketHelper *>::iterator it = helpers.begin(); it
		!= helpers.end(); ++it) {
			(*it)->start();
		}
}
void SimplePrxServer::onAccept(SOCKET so, u_long ip, int port){
	if(!allowIps.empty() &&allowIps.find(ip) == allowIps.end()){
			log(Warn, "deny connection from :%s", sox::addr_ntoa(ip).data());
			sox::Sockethelper::soclose(so);
			return;
	}
	log(Info, "Accept from from :%s : %d", sox::addr_ntoa(ip).data(), port);
	connManager->createServerConn(so, ip, port, handler, getLinkEvent());
	
}


DaemonServer::DaemonServer(){
	recoverSeq = 100;
}

void DaemonServer::startSV(){//向daemon发送注册信号
	SimplePrxServer::startSV();

	PRegisterStr2SV rs;
	rs.sstr = _name;
	rs.port = getPorts().front();
	rs.regGroupId = getGroupId();	/*送groupid注册	*/
	log(Info , "daemonServer:groupid (%d)", getGroupId());
	//std::copy(fs.begin(), fs.end(), std::back_inserter(rs.favours));

	sDispatcher->dispatchByServerIdSV(DAEMON_SERVERID, rs.uri, rs);
}

void DaemonServer::hangUp(){
	PServerHangUp hup;
//	hup.serverid = getServerId();
//	sDispatcher->dispatchByServerId(DAEMON_SERVERID, hup.uri, hup);
	hup.serverid = getServerId();
	sDispatcher->dispatchByServerIdSV(DAEMON_SERVERID, hup.uri, hup);
}

void DaemonServer::wakeUp(){
	PServerWakeUp wup;
	wup.serverid = getServerId();
//	sDispatcher->dispatchByServerId(DAEMON_SERVERID, wup.uri, wup);
	sDispatcher->dispatchByServerIdSV(DAEMON_SERVERID, wup.uri, wup);
}
/*
void DaemonServer::onServerRegister2(sdaemon::PRegisterServerRes2 *res, uint32_t resCode){
	if(resCode == RES_SUCCESS){
		onServerRegister(res, resCode);
		client->onServerRegistered(res->favours);
	}
}
*/
struct AutoFalse{
	bool &b;
	AutoFalse(bool &x):b(x){}
	~AutoFalse(){
		b = false;
	}
};

void DaemonServer::onServerRegister2SV(sdaemon::PRegisterServerRes2SV *res, uint32_t resCode){
	if(resCode == RES_SUCCESS){

		_server_id = res->serverId;
		uuid = res->uuid;
		_net_type = res->yourType;

		log(Debug, "bind server in CTL ip: %s  CNC ip: %s", res->dip.data(), res->wip.data());
		helpers.push_back(createHelper(NULL));
		ip = res->dip != ""? res->dip : res->wip;
		srvRegStatus = sdaemon::RUNNING;

		bInit = true;
		AutoFalse af(bInit);

		for(std::set<IServerWatcher *>::iterator it = watchers.begin(); it != watchers.end(); ++it){
			(*it)->onServerRegistered();	
		}

		client->onServerRegisteredSV(res->favours, false); /* srvRegStatus 可能在这里进行修改*/

		PServerStartSV pss;
		pss.serverId = res->serverId;
		pss.port = getPorts().front();
		pss.srvRegStatus = (sdaemon::ServerStatus)srvRegStatus;
		log(Debug, "daemon start: %u", _server_id);
		sDispatcher->dispatchByServerIdSV(DAEMON_SERVERID, pss.uri, pss);
	}else{
		log(Error, "register server permission deny");
	}
}


void DaemonServer::onRecoverGetServersSV(sdaemon::PRegFavRecStrResSV *res, uint32_t resCode){
	log(Info, "recover from daemon connect broken rescode:%u", resCode);
	if(Request::ifSuccess(resCode)){
		recoverSeq += 100;

		client->onServerRegisteredSV(res->favours, true);
	}
}


void DaemonServer::watch(IServerWatcher *w){
	log(Info, "daemonserver watch this");
	watchers.insert(w);
}
void DaemonServer::revoke(IServerWatcher *w){
	watchers.erase(w);
}

MUUID DaemonServer::getUuid(){
	return uuid;
}


void DaemonServer::onGetAllowIps(sdaemon::PAllowIps *allows){
	allowIps.clear();
	if(allows != NULL)
	{
		log(Info, "OnGetAllowIps.  allows size=%d", allows->allowIps.size());
		std::copy(allows->allowIps.begin(), allows->allowIps.end(), std::inserter(allowIps, allowIps.begin()));
	}	
}

bool DaemonServer::setInitStatus(int init){
	if(bInit){
		srvRegStatus = init;
	}
	return bInit;
}

void DaemonServer::onConnect(bool isRecord){
	log(Debug, "server connect");
	if(!isRecord){
		//bConnected = true;
		if(getServerId() != NONE_SERVER_ID){
			if(!getPorts().empty()){
				PServerRecoverSV rc;
				rc.serverId = getServerId();
				rc.port = getPorts().front();
				rc.name = getName();
				log(Info, "pingSV:%s", getName().data());
				rc.uuid = getUuid();
				rc.groupId = getGroupId();				// should get groupid first!
				rc.serverStatus = WAITING;	// 这个状态对故障进程无意义。
				rc.seq			= recoverSeq;
				sDispatcher->dispatchByServerIdSV(DAEMON_SERVERID, PServerRecoverSV::uri, rc);
			}
		}else{
			startSV();
		}
	}
}

void DaemonServer::onBroken(){
	log(Debug, "server broken");
}

bool DaemonServer::isAllow(uint32_t ip) 
{ 
	return (allowIps.find(ip) != allowIps.end()); 
}



