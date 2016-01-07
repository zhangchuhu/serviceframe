#include "server_common/helper/main_inc.h"
#include "core/sox/logger.h"

#include "server_common/helper/unixdaemon.h"
#include "core/corelib/MultiConnManagerVecImp.h"
#include "server_common/server-lib/ServerWriter.h"
#include "../UdpPing.h"

using namespace core;
using namespace std;
using namespace server;
using namespace sdaemon;
using namespace server::imlbs;
using namespace protocol::imlbs;

#define  DEFAULT_PORT 22
#define  DAEMON_IP "221.238.18.167"

class CTesterImp : 
	public core::PHClass,
	public core::MfcAppcontext,
	public sox::Handler
{
public:

	DECLARE_FORM_MAP

	CTesterImp() { m_counter = 0; }

	void SendPing();

	void OnPingRes(protocol::imlbs::PCS_ImLbsPingRes*, IConn *conn);

	virtual void DefaultDispatch(Request &request, IConn *conn);

	void handle(int sig);

public:

	void onRefreshSV(const vector<FavorStrSV> vecFavSrv);

	uint32_t m_counter;
	uint32_t m_maxTimes;

	map<uint32_t, uint32_t> m_mapIp2Grp;
	UdpConnImp*  m_udpConn;
};


BEGIN_FORM_MAP(CTesterImp)
	ON_LINK(PCS_ImLbsPingRes, &CTesterImp::OnPingRes)
END_FORM_MAP()

void CTesterImp::SendPing()
{
	for (map<uint32_t, uint32_t>::iterator it = m_mapIp2Grp.begin(); it != m_mapIp2Grp.end(); ++it)
	{
		m_udpConn->reset (it->first, DEFAULT_PORT);
		PCS_ImLbsPing pingObj(123456, time(NULL));
		Sender req(pingObj.uri, pingObj);	
		m_udpConn->send(req);
	}

	select_timeout(5 * 1000);

	//cout << " Send Ping ok. timeFlag: " << pingObj.m_timeFlag << endl;
}

void CTesterImp::OnPingRes(PCS_ImLbsPingRes* res, IConn *conn)
{
	cout << "===> Recive Ping Res : groupId(" 
		<< res->m_uGroupId << ")" << endl
		<< "                   "
		<< sox::addr_ntoa(conn->getPeerIp()) << endl;
	
	m_mapIp2Grp.erase(conn->getPeerIp());	
}

void CTesterImp::DefaultDispatch(Request &request, IConn *conn)
{
	if (request.getUri() == PCS_ImLbsPingRes::uri)
	{
		PCS_ImLbsPingRes obj;
		try
		{
			obj.unmarshal(request.getPackData());
		}
		catch (sox::UnpackError& err) 
		{
			log(Warn, "[CTesterImp::DefaultDispatch] Unpack error, uri: 0x%02x", request.getUri());
			return ;
		}

		OnPingRes(&obj, conn);
	}
	else
	{
		log (Error, "[CLbsUdpPingHandler::doRequest] unknow request !!!");
		return ;
	}
}

void CTesterImp::onRefreshSV(const vector<FavorStrSV> vecFavSrv)
{
	for (vector<FavorStrSV>::const_iterator it = vecFavSrv.begin(); it != vecFavSrv.end(); ++it)
	{
		if (it->sstr == SUFFIX_IMLBSD && it->serverStatus == RUNNING)
		{
			if (it->dip != 0)
			{
				m_mapIp2Grp.insert(make_pair(it->dip, it->groupId));
			}
			else if (it->wip != 0)
			{	
				m_mapIp2Grp.insert(make_pair(it->wip, it->groupId));
			}
		}
	}

	cout << endl << "Get Faver Server num:  " << m_mapIp2Grp.size() << endl << endl;

	SendPing();
}

void CTesterImp::handle(int sig)
{
	if (sig == sox::SEL_TIMEOUT)
	{
		cout << endl << " ======= not come back ======" << m_mapIp2Grp.size() << endl;
		for (map<uint32_t, uint32_t>::iterator it = m_mapIp2Grp.begin(); it != m_mapIp2Grp.end(); ++it)
		{
			cout << sox::addr_ntoa(it->first) << endl;
		}

		SendPing();
		select_timeout(5 * 1000);
	}
}


class DaemonClientSample :
	public core::PHClass,
	public core::IWriterAware
{
public:
	DECLARE_FORM_MAP

	void OnGetFaverServerRes(sdaemon::PRegisterFavourStrResSV *res, uint32_t resCode)
	{
		m_vecFavSrv = res->favours;
		cout << "Revive faver: " << m_vecFavSrv.size() << endl;
		m_pTester->onRefreshSV(m_vecFavSrv);
	}

	void GetFaverServer(IConn* conn)
	{
		sdaemon::PRegisterFavourStr pr;
		Sender s(pr.uri, pr);
		conn->send(s);
	}

	std::vector<FavorStrSV> m_vecFavSrv;
	CTesterImp* m_pTester;
};

BEGIN_FORM_MAP(DaemonClientSample)
	ON_RESPONSE(PRegisterFavourStrResSV, &DaemonClientSample::OnGetFaverServerRes)
END_FORM_MAP()

//////////////////////////////////////////////////////////////////////////


int main(int sz, char *args[]) 
{
	string daemon_ip;
	if (sz != 2)
	{
		daemon_ip = DAEMON_IP;
	}
	else
	{
		daemon_ip = args[1];
	}

	WrapServerStart::init();

	int localPort = 7777;

	MultiConnManagerImp __connManager;
	BackLinkHandler __handler;
	CTesterImp __tester;
	CUdpPingServerConnCreator __srvConnCreator;
	__connManager.setServerConnCreator(&__srvConnCreator);


	// ³õÊ¼»¯daemonclient
	MultiConnManagerImp  connManager;
	InnerConnCreator connCreater;
	DaemonClientSample client;
	MfcAppcontext context;
	BackLinkHandler bhandler;

	bhandler.setAppContext(&context);
	connManager.setClientConnCreator(&connCreater);
	IConn* daemonConn = connManager.createClientConn(daemon_ip, 2000, &bhandler, &connManager);

	context.addEntry(DaemonClientSample::getFormEntries(), &client,  &client);
	// end


	__handler.setAppContext(&__tester);

	UdpConnImp *conn = new UdpConnImp(NULL, 0, DEFAULT_PORT, &__handler, &__connManager);	
	UdpServerImp *server = new UdpServerImp(localPort, &__handler, &__connManager);
	server->setConnManager(&__connManager);
	conn->setServer(server);
	
	__tester.m_udpConn = conn;
	client.m_pTester = &__tester;
	
	client.GetFaverServer(daemonConn);
	WrapServerStart::run();
}
