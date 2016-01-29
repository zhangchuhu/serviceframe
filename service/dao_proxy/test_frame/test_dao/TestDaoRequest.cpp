#include <curl/curl.h>
#include <time.h>
#include "TestDaoRequest.h"
#include "server_common/dblib/DbProc.h"
#include "core/sox/logger.h"
#include "common/res_code.h"
#include "server_common/server-lib/UrlHelper.h"
#include "server_common/server-lib/id_def.h"
#include "core/sox/snox.h"
#include "server_common/server-lib2/StringUtil.h"
#include "server_common/server-lib3/Tasks.h"

#include "server_common/helper/commonUtil.h"
#include "server_common/helper/server_const.h"
#include "server_common/seda/common/any_cast.h"

using namespace server::testdaodb;
using namespace server::db_node;
using namespace core;
using namespace ::server::router;
using namespace std;

BEGIN_FORM_MAP(TestDaoRequest)
	ON_REQUEST(PProxyDaoRequest, &TestDaoRequest::dao_normal_request)
	ON_REQUEST(PProxyDaoPing, &TestDaoRequest::ping_request)
	ON_REQUEST(DBStateReq, &TestDaoRequest::stateRequest)
	ON_REQUEST(DBAdminReq, &TestDaoRequest::adminRequest)
END_FORM_MAP()


int err_type=0;
int err_start_count_down=0;
int err_last_count_down=0;
int err_magnitude=0;


TestDaoRequest::TestDaoRequest() : m_timer(this)
{
	m_timer.start(1000);
	
}
TestDaoRequest::~TestDaoRequest()
{
}

bool TestDaoRequest::OnTimerCheckMalfunction()
{
	if(err_type!=0)
	{
		if(err_start_count_down>0)
		{
			err_start_count_down--;
		}
		else
		{
			if(err_last_count_down>0)
			{
				err_last_count_down--;
			}
			else
			{
				err_type=0;
			}
		}
	}
	return true;
}
PHClass* TestDaoRequest::getInstance(Request& request, IConn* conn, InstanceTarget** out)
{
	*out=this;
	return this;
}


void TestDaoRequest::dao_normal_request(::server::router::PProxyDaoRequest* req)
{
	if(err_start_count_down==0)
	{
	if(err_type==1)
	{
		if(rand()%10000<err_magnitude)
		{
			return;
		}
	}
	if(err_type==2)
	{
		usleep(err_magnitude);
	}
	if(err_type==3)
	{
		gl_sleep_us=m_tran_time_us+err_magnitude;
	}
	else
	{
		gl_sleep_us=m_tran_time_us;
	}
	}
	else
	{
		gl_sleep_us=m_tran_time_us;
	}
	usleep(m_proc_time_us);
	PProxyDaoResponse resp;
	resp.context=req->context;
	resp.upper_layer=req->upper_layer;
	resp.result_code=0;

	uint32_t queueszz=m_plink_hand->get_queue_size();
	resp.current_free=(m_plink_hand->queueLimit-queueszz);
	if(resp.current_free<=200)
	{
		resp.current_free=0;
	}
	else
	{
		resp.current_free-=200;
	}
	/////////
	uint64_t connntid=writer->get_connection_id_incurrent_contex();
	gl_mtask_lock.lock();
	Sender *s = new Sender(PProxyDaoResponse::uri, resp);
	s->setResCode(RES_SUCCESS);
	s->endPack();
	gl_mtask_q.push_back(new DispatchSendTask(_connect_managerr,s,connntid));
	gl_mtask_lock.unlock();

}
void TestDaoRequest::ping_request(::server::router::PProxyDaoPing* req)
{
	if(err_start_count_down==0)
	{
	if(err_type==1)
	{
		if(rand()%10000<err_magnitude)
		{
			return;
		}
	}
	if(err_type==3)
	{
		gl_sleep_us=m_tran_time_us+err_magnitude;
	}
	else
	{
		gl_sleep_us=m_tran_time_us;
	}
	}
	else
	{
		gl_sleep_us=m_tran_time_us;
	}

	if(req->type==2)
	{
		PProxyDaoPingResp opr;
		opr.extra_info=req->extra_info;
		opr.me="nobody";
		opr.timestamp=0;
		////////////////////////////////////////////////////////////////////////
		uint64_t connntid=writer->get_connection_id_incurrent_contex();
		gl_mtask_lock.lock();
		Sender *s = new Sender(PProxyDaoPingResp::uri, opr);
		s->setResCode(RES_SUCCESS);
		s->endPack();
		gl_mtask_q.push_back(new DispatchSendTask(_connect_managerr,s,connntid));
		gl_mtask_lock.unlock();
		return;
	}
	PProxyDaoPingResp pres;
	pres.extra_info=req->extra_info;
	pres.me="nobody";
	pres.timestamp=0;
	DAOLOAD dl;
	dl.dao_name="dao_malfunction";
	dl.db_type=1;
	dl.extra_info="well";
	uint32_t queueszz=m_plink_hand->get_queue_size();
	dl.free=(m_plink_hand->queueLimit-queueszz);
	if(dl.free<=200)
	{
		dl.free=0;
	}
	else
	{
		dl.free-=200;
	}
	if(queueszz<50)
	{
		dl.load=m_mtworkq->m_LastprocNum;
	}
	else
	{
		dl.load=(queueszz-50)*1000+m_mtworkq->m_LastprocNum;
	}	
	pres.daoinfos.push_back(dl);

	uint64_t connntid=writer->get_connection_id_incurrent_contex();
	gl_mtask_lock.lock();
	Sender *s = new Sender(PProxyDaoPingResp::uri, pres);
	s->setResCode(RES_SUCCESS);
	s->endPack();
	gl_mtask_q.push_back(new DispatchSendTask(_connect_managerr,s,connntid));
	gl_mtask_lock.unlock();

}
void TestDaoRequest::stateRequest(server::db_node::DBStateReq * req)
{
	DBStateResp r;
	r.which=req->which;
	stringstream ss;
	ss <<"TestDao Debug v0.1" <<endl;
	ss  <<"err_type:"<<err_type <<endl
		<<"err_magnitude:" <<err_magnitude <<endl
		<<"err_start_count_down:" <<err_start_count_down <<endl
		<<"err_last_count_down:" <<err_last_count_down <<endl;
	r.result=ss.str();
	answer(r.uri, RES_SUCCESS, r);
}


void g_cutter_space(string& in,vector<string>& slice)
{
	slice.clear();
	size_t pos=0;
	for(;;)
	{
		size_t r=in.find_first_of(' ',pos);
		if(r==string::npos)
		{
			if(pos<in.size())
			{
				slice.push_back(in.substr(pos));
			}
			break;
		}
		if(pos<r)
		{
			slice.push_back(in.substr(pos,r-pos));
		}
		pos=r+1;
	}
}
void TestDaoRequest::adminRequest(server::db_node::DBAdminReq * req)
{
	DBAdminResp r;
	r.result=0;
	r.req_id=req->req_id;
	r.option=req->option;
	if(req->option==1)
	{
		vector<string> paras;
		g_cutter_space(req->command,paras);
		if(paras.size()==4)
		{
			int a=0;
			int b=0;
			int c=0;
			JLexical::Cast(paras[1],a);
			JLexical::Cast(paras[2],b);
			JLexical::Cast(paras[3],c);
			if(paras[0]==std::string("Drop"))
			{
				err_type=1;
				err_start_count_down=b;
				err_last_count_down=c;
				err_magnitude=a;
			}
			else if(paras[0]==std::string("Proc"))
			{
				err_type=2;
				err_start_count_down=b;
				err_last_count_down=c;
				err_magnitude=a;
			}
			else if(paras[0]==std::string("Tran"))
			{
				err_type=3;
				err_start_count_down=b;
				err_last_count_down=c;
				err_magnitude=a;
			}
		}
	}
	answer(r.uri, RES_SUCCESS, r);
}

