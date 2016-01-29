#ifndef __CONSOLE_SERVER_H__
#define __CONSOLE_SERVER_H__

#include "core/corelib/InnerConn.h"
#include "core/ilink.h"
#include "core/iserver.h"
#include "core/corelib/AStatLinkHandler.h"
#include "common/core/iserver.h"


namespace server
{
    namespace cache
    {

        typedef core::InnerConn console_handler;

        class ConsoleServer : public core::AStatLinkHandler,
                              public core::ILinkEvent
        {
            public:
              ConsoleServer();
              ~ConsoleServer();

                virtual console_handler*  create(SOCKET so, u_long ip, int port);
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
				void					  setDaemonServer(core::IDaemonServer *server);
				std::string				  getProcInfo2JSON();

            protected:
              typedef std::set<console_handler*>        console_handler_set;
              console_handler_set                       __conns;
			  core::IDaemonServer *						m_server;						
			  uint32_t									m_startTime;
			  uint32_t									m_testValue;
			  std::string								m_lastCmd;
        };
    }
}
#endif  //__CONSOLE_SERVER_H__



