#include "UdpPing.h"
#include "core/corelib/WrapForwardBuffer.h"
#include "common/protocol/PImLbs.h"
#include "common/core/sender.h"

using namespace std;
using namespace core;
using namespace server::imlbs;
using namespace protocol::imlbs;

#define UDP_HANDLE_TIMEOUT 10000

//#define OUT_FILE_PROC_LOG
#define UDP_PROC_LOG_FILE  "/data/yy/log/imlbs_d/procLog.log"

CLbsUdpPingHandler::CLbsUdpPingHandler()
{
	m_uCounter = 0;
	select_timeout(UDP_HANDLE_TIMEOUT);
	
#ifdef OUT_FILE_PROC_LOG
	m_outProcLog.open(UDP_PROC_LOG_FILE, ios::app);
#endif
}

CLbsUdpPingHandler::~CLbsUdpPingHandler()
{
#ifdef OUT_FILE_PROC_LOG
	m_outProcLog.close();
#endif
}


int CLbsUdpPingHandler::onData(const char* data, size_t size, IConn *conn, int type /* = 0 */)
{
	WrapForwardBuffer fb(data, size);
	m_uCounter ++;

	if (fb.size() < 4)
	{
		// 长度不够
		log (Error, "[BackLinkHandler::onData]: data is less than 4 bits");
		return -1;
	}
	uint32_t length = Request::peeklen(fb.data()); // 察看整个包的长度
	if(length > getPackLimit())
	{
		log(Info, "[BackLinkHandler::onData]: data is too long!  length=%ld,  Limit=%ld", length, getPackLimit());
		return -1;
	}

	if (fb.size() < length)
	{
		log (Error, "[BackLinkHandler::onData]: data size is %d , length is %d ", fb.size(), length);
		return -1;
	}

	Request request(fb.data(), length);
	request.head();
	request.setConnType(type);
	incProc();
	if(doRequest(request, conn) == -1)
	{
		return -1;
	}

	fb.erase(length);

	return (int)fb.offset();
}

// ping 请求在此处理，未走appContext过程

int CLbsUdpPingHandler::doRequest(Request &request, IConn *conn)
{
	if (request.getUri() == PCS_ImLbsPing::uri)
	{
		PCS_ImLbsPing obj;
		try
		{
			obj.unmarshal(request.getPackData());
		}
		catch (sox::UnpackError& err) 
		{
			log(Warn, "[CLbsUdpPingHandler::doRequest] Unpack error, uri: 0x%02x", request.getUri());
			return -1;
		}
		
		PCS_ImLbsPingRes resObj;
		resObj.m_timeFlag = obj.m_timeFlag;
		log (Info, "[CLbsUdpPingHandler::doRequest] Ping request come. uid(%d)-timeFlag(%d)", obj.m_uUid, obj.m_timeFlag);
		
		if (m_pLinkdManager->GetImlinkInfo2(resObj.m_uGroupId, resObj.m_uIp, resObj.m_vecPorts))
		{
			// 响应ping请求，并带回imlinkd信息
			Sender req(PCS_ImLbsPingRes::uri, resObj);
			conn->send(req);
		}
		else
		{
			log (Warn, "[CLbsUdpPingHandler::doRequest] user(%d) get imlinkd faild.", obj.m_uUid);
		}
	}
	else
	{
		log (Error, "[CLbsUdpPingHandler::doRequest] unknow request !!!");
		return -1;
	}

	return 0;
}

void CLbsUdpPingHandler::handle(int sig)
{
	if (sig == sox::SEL_TIMEOUT)
	{
		//static uint32_t traceCounter = 0;
		//if (++traceCounter % 4 == 0)
		//{
		//	m_pLinkdManager->Trace();
		//}

		log(Notice, "Ping ==> proc: %u", m_uCounter);

#ifdef OUT_FILE_PROC_LOG
		OutProcLog();
#endif

		m_uCounter = 0;
		select_timeout(UDP_HANDLE_TIMEOUT);
	}
}

// 生成日期

std::string safe_time()
{
	time_t t = time(NULL);
	char *ct = ctime(&t);
	if(ct)
	{
		size_t len = strlen(ct);
		if(len > 0)
		{
			ct[len - 1] = '\0';
			return std::string(ct, len - 1);
		}
	}
	return "empty time";
}

// 打印proc信息，方便在syslog-ng关闭的情况下定位问题

void CLbsUdpPingHandler::OutProcLog()
{

	m_outProcLog << safe_time() << " : Ping ==> proc: " << m_uCounter << endl;
}

//////////////////////////////////////////////////////////////////////////

CUdpPingConnManagerImp::CUdpPingConnManagerImp()
{
	m_connId = 1;
	m_conn = NULL;
}

void CUdpPingConnManagerImp::eraseConnect(core::IConn *conn)
{
	if (conn && conn == m_conn)
	{
		delete m_conn;
		m_conn = NULL;
	}
}

void CUdpPingConnManagerImp::eraseConnectById(uint32_t id)
{
	if(m_conn && m_conn->getConnId() == id)
	{
		eraseConnect(m_conn);
	}
}

IConn* CUdpPingConnManagerImp::getConnectById(uint32_t id)
{
	if (m_conn && m_conn->getConnId() == m_connId)
		return m_conn;

	return NULL;
}

IConn* CUdpPingConnManagerImp::createServerConn(SOCKET so, uint32_t ip, int port, ILinkHandler *iH, ILinkEvent *iE)
{
	return 	serverCreator->creatConnect(so, ip, port, iH, iE, this);
}

void CUdpPingConnManagerImp::onConnCreate(core::IConn *conn)
{
	if (conn != NULL)
	{
		conn->setConnId(++m_connId);
		conn->setSerialId(m_connId);
	}
}

bool CUdpPingConnManagerImp::dispatchById(uint32_t cid, core::Sender &request)
{
	if(m_conn)
		m_conn->send(request);
	else
		log(Warn, "[CUdpPingConnManagerImp::dispatchById]: dispatch svid connect not ready");

	return m_conn != NULL;
}

//////////////////////////////////////////////////////////////////////////

CUdpPingServerConnCreator::CUdpPingServerConnCreator()
{
	m_udpConn = new core::UdpConnImp();
}

CUdpPingServerConnCreator::~CUdpPingServerConnCreator()
{
	if (m_udpConn != NULL)
		delete m_udpConn;
}

IConn* CUdpPingServerConnCreator::creatConnect(SOCKET so, uint32_t ip, int port, ILinkHandler *ih, ILinkEvent *ie, CreateCallback *cb)
{
	if (m_udpConn == NULL)
	{
		m_udpConn = new core::UdpConnImp;
	}

	m_udpConn->reset(ip, port);
	m_udpConn->setHandler(ih);
	m_udpConn->setLinkEvent(ie);
	if(cb)
	{
		cb->onConnCreate(m_udpConn);
	}
	return m_udpConn;
}

