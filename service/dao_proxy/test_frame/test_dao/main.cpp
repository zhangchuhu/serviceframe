#include <unistd.h>
#include <curl/curl.h>
#include "common/core/base_svid.h"
#include "common/core/form.h"
#include "core/corelib/WrapServerStart.h"
#include "server_common/helper/unixdaemon.h"
#include "server_common/server-lib3/IServerStategy.h"
#include "server_common/server-lib3/interface.h"
#include "server_common/server-lib3/TMTInstanceFactory.h"
#include "server_common/server-lib3/MTLinkHandler.h"
#include "server_common/config/TinyXmlServerConfigImp.h"
#include "server_common/config/TinyXmlHADbConfigImp.h"
#include "server_common/config/LoaderContainer.h"

#include "server_common/server-lib3/MTLinkHandler.h"
#include "server_common/server-lib3/WorkerQueue.h"
#include "server_common/server-lib3/SimpleProxyServer.h"

#include "server_common/seda/common/any_cast.h"
#include "server_common/seda/syncall/remote_call.hpp"

#include "TestDaoRequest.h"
#include "server_common/seda/stage/stage_base.h"

using namespace seda;
using namespace core;
using namespace server::config;
using namespace server::testdaodb;

IMainThreadQueue * _mainQueue=0;
IConnManager * _connect_managerr=0;
uint32_t gl_sleep_us=0;
seda::linux_mutex gl_mtask_lock;
std::vector<ITask* > gl_mtask_q;

std::vector<ITask* > gl_mtask_qex;

class PeriodStage;
seda::Stage<PeriodStage>* gl_mstage;

class PeriodStage
{
public:
	PeriodStage(){}
	~PeriodStage(){}
public:
	void lop()
	{
		std::vector<ITask* > temp_mtask_q;
		gl_mtask_qex.swap(temp_mtask_q);

		gl_mtask_lock.lock();
		gl_mtask_q.swap(gl_mtask_qex);
		gl_mtask_lock.unlock();
		//////////////////
		for(size_t i=0;i<temp_mtask_q.size();++i)
		{
			_mainQueue->push(temp_mtask_q[i]);
		}
		//////////////////
		usleep(gl_sleep_us);
		gl_mstage->push(&PeriodStage::lop,true);
	}
};

int main(int sz, char *args[])
{
	if(sz!=6)
	{
		cerr <<"Usage:" <<args[0] <<" <port> <group> <queue> <prc_time_us> <tran_time_us>" <<std::endl;
		return -1;
	}
	int arg_port=0;JLexical::Cast(args[1],arg_port);
	int arg_groupid=0;JLexical::Cast(args[2],arg_groupid);
	uint32_t arg_queuelength=0;JLexical::Cast(args[3],arg_queuelength);
	uint32_t arg_prc_time_us=0;JLexical::Cast(args[4],arg_prc_time_us);
	uint32_t arg_tran_time_us=0;JLexical::Cast(args[5],arg_tran_time_us);

	gl_sleep_us=arg_tran_time_us;

	init_daemon dm(sz, args);
	WrapServerStart::init();

	curl_global_init(CURL_GLOBAL_DEFAULT);

    ServerStategy ss;
    ss.type = NORMALSERVER;
    ss.threadStategy = MULTITHREAD;
    ss.groupId = arg_groupid;
    ss.port = arg_port;
    ss.name = "";
	ss.queue_pack_size=arg_queuelength;
	ss.thread_num=20;

    IServerFacade *sf = initEviroment(ss);
	IAppContextEx2* __appContext=sf->getAppContext();

	TestDaoRequest __testdaoRequest;

	_mainQueue=sf->getMainThreadWriteQueue();
	_mainQueue->set_queue_limit(800*1000*1000);
	_connect_managerr=sf->getConnManager();


	__testdaoRequest.m_tran_time_us=arg_tran_time_us;
	__testdaoRequest.m_proc_time_us=arg_prc_time_us;

	__appContext->addEntry(TestDaoRequest::getFormEntries(), &__testdaoRequest);

	IServer* pclient_srv=sf->getServer(arg_port, MULTITHREAD, arg_queuelength, 20);
	dynamic_cast<SimplePrxServer2*>(pclient_srv)->set_port_shift_range(1);

	MTLinkHandler* plink_hand=dynamic_cast<MTLinkHandler*>(dynamic_cast<ILinkHandlerAware*>(pclient_srv)->handler);
	assert(plink_hand!=0);
	MTWorkerQueue* mtworkq=dynamic_cast<MTWorkerQueue*>(plink_hand->wQueue);

	__testdaoRequest.m_plink_hand=plink_hand;
	__testdaoRequest.m_mtworkq=mtworkq;

	RunEnv thenv;
	gl_mstage=thenv.create_stage<PeriodStage>(65536, 1);
	gl_mstage->push(&PeriodStage::lop,true);

	thenv.start();
	sf->startAllServer();
	sf->loop();
	return 0;
}

