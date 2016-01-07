#ifndef DAEMON_SERVER_H_FXW_
#define DAEMON_SERVER_H_FXW_

#include "common/core/ibase.h"
#include "common/core/sender.h"
#include "core/sox/tcpsock.h"
#include "core/corelib/MAbstractServer.h"
#include "server_common/protocol/daemon.h"
#include "server_common/server-lib/uuid.h"
#include "server_common/server-lib/iclient.h"

namespace core{
	class SimplePrxServer;
	class ServerSocketHelper:public sox::ServerSocket{
	protected:
		SimplePrxServer *imp;
	public:
		ServerSocketHelper(SimplePrxServer *, SOCKET so);
		virtual ~ServerSocketHelper(){}
		virtual void onAccept(SOCKET so, u_long ip, int port);
		void start();
	};

	class SimplePrxServer: public MAbstractServer{
	protected:
		std::vector<ServerSocketHelper *> helpers;

		friend class ServerSocketHelper;

		std::vector<uint16_t> expectPorts;

		std::vector<uint16_t> curports;

		// 保存新打开端口
		std::vector<uint16_t> updatePorts;

		std::string ip;

		std::set<uint32_t> allowIps;

	public:
		~SimplePrxServer();

		void onAccept(SOCKET so, u_long ip, int port);

		virtual ServerSocketHelper *createHelper(const char *ip);

		void refreshPort(uint16_t p);

		void refreshPorts(const std::vector<uint16_t> &ports);

		
		virtual std::string getIp();

		virtual std::vector<uint16_t> getPorts();

		std::vector<uint16_t> getUpdatePorts();

		virtual void startSV() ;		

		virtual void hangUp(){}

		virtual void wakeUp(){}

    void resetPorts(std::vector<uint16_t> ports); 

	protected:
		virtual ServerSocketHelper *create(const char* ip, uint16_t p, uint16_t &cur);
	};

	class DaemonServer: public PHClass, public IFormTarget, 
		public SimplePrxServer,
		public IServerIdDispatcherAware,
		public IDaemonServer,
		public core::IDaemonClientAware,
		public IDaemonConnectStatus
	{
		std::set<IServerWatcher *> watchers;
		
		MUUID uuid;

		bool bInit;

		int srvRegStatus;
	public:
		DECLARE_FORM_MAP
		
		DaemonServer();

		virtual void startSV();
		virtual void hangUp();

		virtual void wakeUp();

		void onServerRegister2SV(sdaemon::PRegisterServerRes2SV *res, uint32_t resCode);
		void onGetAllowIps(sdaemon::PAllowIps *allows);
		void onRecoverGetServersSV(sdaemon::PRegFavRecStrResSV *, uint32_t resCode);

		virtual void watch(IServerWatcher *w);

		virtual void revoke(IServerWatcher *w);

		virtual MUUID getUuid();

		virtual bool setInitStatus(int);
		
		virtual void onConnect(bool isRecord);
		virtual void onBroken();

		/** 
		 * @return	true	allow
					false	not allow
		 */
		bool isAllow(uint32_t ip);
	private:
		bool isInit(){
			return bInit;
		}
		int recoverSeq;
	};
}


#endif
