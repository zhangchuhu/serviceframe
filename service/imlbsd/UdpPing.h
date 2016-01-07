#ifndef IM_LBS_UDP_PING_SERVER
#define IM_LBS_UDP_PING_SERVER

#include <fstream>
#include "core/corelib/UdpConnImp.h"
#include "core/corelib/ConnManagerLinkImp.h"
#include "common/core/ilink.h"
#include "common/core/sender.h"
#include "common/core/request.h"
#include "core/corelib/AStatLinkHandler.h"
#include "LinkdManager.h"

namespace server
{
	namespace imlbs
	{
		class CUdpPingServer 
		{
		public:

			CUdpPingServer() { m_udpServer = NULL;}
			~CUdpPingServer() { if (m_udpServer != NULL)  delete m_udpServer; }

			CUdpPingServer(int localPort, core::ILinkHandler *h, core::ILinkEvent *eh, core::IConnManager *c)
			{
				m_udpServer = new core::UdpServerImp(localPort, h, eh);
				m_udpServer->setConnManager(c);
			}

			void Start(int localPort, core::ILinkHandler *h, core::ILinkEvent *eh, core::IConnManager *c)
			{
				if (m_udpServer != NULL)
				{
					delete m_udpServer;
					m_udpServer = NULL;
				}
				m_udpServer = new core::UdpServerImp(localPort, h, eh);
				m_udpServer->setConnManager(c);
			}
		private:
		
			core::UdpServerImp* m_udpServer;
		};

//////////////////////////////////////////////////////////////////////////

		class CUdpPingServerConnCreator : public core::IServerConnCreator
		{
		public:
			CUdpPingServerConnCreator();
			~CUdpPingServerConnCreator();

			virtual core::IConn *creatConnect(SOCKET so, uint32_t ip, int port, core::ILinkHandler *ih, core::ILinkEvent *ie, core::CreateCallback *cb);

		private:

			core::UdpConnImp* m_udpConn; 
		};

//////////////////////////////////////////////////////////////////////////

		class CUdpPingConnManagerImp : public core::ConnManagerLinkImp
		{
			uint32_t     m_connId;
			core::IConn* m_conn;

		public:

			CUdpPingConnManagerImp();

			virtual void eraseConnect(core::IConn *conn);
			virtual void eraseConnectById(uint32_t id);
			virtual core::IConn *getConnectById(uint32_t id);

			virtual core::IConn *createClientConn(const std::string& ip, uint32_t port, core::ILinkHandler *iH, core::ILinkEvent *iE)
			{
				assert(false);
				return NULL;
			}
			virtual core::IConn *createServerConn(SOCKET, uint32_t ip, int port, core::ILinkHandler *iH, core::ILinkEvent *iE);

			virtual void onConnCreate(core::IConn *conn);
			virtual bool dispatchById(uint32_t cid, core::Sender &);
			virtual bool dispatchByIds(const std::set<uint32_t> &ids, core::Sender &, uint32_t exp = NONEEXP_CID)
			{
				assert(false);
				return false;
			}
		};

//////////////////////////////////////////////////////////////////////////

		class CLbsUdpPingHandler :
			public core::AStatLinkHandler,
			public sox::Handler
		{

			uint32_t m_uCounter;

			CLinkdManager* m_pLinkdManager;

			std::ofstream  m_outProcLog;

		public:

			CLbsUdpPingHandler();
			~CLbsUdpPingHandler();

			void SetLinkdManager(CLinkdManager* p)
			{
				m_pLinkdManager = p;
			}

			int onData(const char*, size_t, core::IConn *conn, int type = 0);

			int doRequest(core::Request &request, core::IConn *rconn);

			void OutProcLog();

			void handle(int sig);
		};
	}
}

#endif
