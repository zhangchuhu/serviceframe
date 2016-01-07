#ifndef SERVERMAINHELPER_H_
#define SERVERMAINHELPER_H_
#include "core/corelib/DaemonClient.h"
#include "core/corelib/BaseAppContext.h"

template<class S, class C, class N, class F> class ServerMainHelper {
protected:
	S sfactory;  //server conn factory;
	C clientFactory;  // client conn factory;
	N creator;        // client conn creator;
	
	F fac;	  // server factory;
	sdaemon::DaemonClient dc;
	core::BaseAppContext daemonContext;
public:
	ServerMainHelper(protocol::svid id){
		dc.setServerType(id);
		dc.setServerFactory(&fac);
		dc.setConnFactory(&clientFactory);
		dc.init(&daemonContext);
		
		clientFactory.setConnCreator(&creator);
		fac.setServerConnFactory(&sfactory);
		fac.setClientDispatcher(clientFactory.getDispatcher());
		fac.setClientDaemon(&dc);
		fac.daemonBind();
	}

	virtual ~ServerMainHelper(){}	
	F &getServerFactory(){
		return fac;
	}
	
	S &getServerConnFactory(){
		return sfactory;
	}
	
	virtual void start(){
		dc.start();
	}
};
#endif /*SERVERMAINHELPER_H_*/
