#ifndef __CONSOLE_SERVER_H__
#define __CONSOLE_SERVER_H__

#include <string>

#include "core/corelib/InnerConn.h"
#include "core/ilink.h"
#include "core/iserver.h"
#include "core/corelib/AStatLinkHandler.h"
#include "server_common/server-lib/DaemonServer.h"

namespace core
{
	typedef core::InnerConn console_handler;
		
	class ConsoleServerAware{};	
	typedef std::string (ConsoleServerAware::*consoleCall)(std::string params, core::IConn* conn);
	typedef std::string (ConsoleServerAware::*procInfoCall)();
	struct resourceStat{
		uint64_t requestCount;
		uint64_t lostCount;
		resourceStat(){
			requestCount = 0;
			lostCount = 0;
		}
	};
	
    class ConsoleServer : public core::ConsoleServerAware,
			public core::AStatLinkHandler,
            public core::ILinkEvent
    {

		struct consoleCallback{
			std::string help;
			ConsoleServerAware *ica;
			consoleCall icc;
		};
		struct procInfoCallback{
			std::string name;
			ConsoleServerAware *ica;
			procInfoCall icc;
		};
	
        public:
          ConsoleServer(IConnManager *conM, IDaemonServerAware *ds, std::string serviceName, uint16_t port, bool statByFrom = false, bool statByIp = false);
          ~ConsoleServer();

            virtual console_handler*  create(SOCKET so, uint32_t ip, int port);
            virtual void              destroy(console_handler* handler);

            virtual void              onAccept(SOCKET so, u_long ip, int port);
            virtual int               onData(const char*, size_t, core::IConn *conn, int type = 0);

          
            virtual void              onClose(core::IConn *conn);
            virtual void              onError(int ev, const char *msg, core::IConn *conn);

            virtual void              onInitiativeClose(core::IConn *conn) {};
            virtual void              onConnected(core::IConn *conn) {};
            virtual void              onTimeout(core::IConn *conn) {};
            virtual void              onMultiError(int ev, const char *msg, std::vector<core::IConn *> &conn) {};

            virtual void              process_command(std::string cmd, core::IConn* conn);
			std::string getProcInfo2JSON();  
			std::string resourceStat2JSON();
			std::string cmdAgent(std::string params, core::IConn* conn);
			std::string cmdHelp(std::string params, core::IConn* conn);
			void addEntry(std::string cmd, std::string help, ConsoleServerAware *ica, consoleCall icc);
			void addResourceStat(std::string resourceName, resourceStat* rs);
			void addProcInfo(std::string infoName, ConsoleServerAware *ica, procInfoCall rs);
			void start();

        protected:
		std::map<std::string, procInfoCallback>		m_procInfoMap;
          typedef std::set<console_handler*>        console_handler_set;
          console_handler_set                       __conns;
		  uint32_t									m_startTime;
		  std::string								m_serviceName;
		  IDaemonServerAware						*m_daemonServer;
		  std::string								m_lastCmd;
			std::map<std::string, consoleCallback> 		m_cmdMap;
			std::map<std::string, resourceStat*>			m_resourceStat;
			SimplePrxServer 						outerServer;
		  bool										m_bStatByIp;
		  bool										m_bStatFrom;
    };

}
#endif  //__CONSOLE_SERVER_H__

