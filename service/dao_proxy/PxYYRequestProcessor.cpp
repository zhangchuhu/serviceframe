#include "PxYYRequestProcessor.h"
#include <curl/curl.h>
#include "util.h"
#include "server_common/seda/common/any_cast.h"

#include "core/sox/logger.h"
#include "common/res_code.h"
#include "server_common/server-lib/UrlHelper.h"
#include "server_common/server-lib/id_def.h"
#include "core/sox/snox.h"
#include "server_common/server-lib2/StringUtil.h"
#include "server_common/helper/commonUtil.h"
#include "server_common/helper/server_const.h"
#include "request_fsm.h"
#include "GlobalCommon.h"
#include "server_common/seda/encryption/swiftzlib.h"

using namespace core;
using namespace server::dao_proxy;
using namespace std;
using namespace server::router;
using namespace server::db_node;

int decoding_pid_fsmid(std::string& context,uint64_t& fsmid)
{
	fsmid=0;
	if(context.size()!=(sizeof(uint64_t)+sizeof(uint64_t)))
	{
		return -1;
	}
	char* bf=const_cast<char*>(context.c_str());
	if(*(uint64_t*)bf!=gl_processpid)
	{
		return -1;
	}
	bf+=sizeof(uint64_t);
	fsmid=*(uint64_t*)bf;
	return 0;
}


BEGIN_FORM_MAP(PxYYRequestProcessor)
	ON_REQUEST(PProxyDaoResponse, &PxYYRequestProcessor::daoheadresponse)
	ON_REQUEST(PProxyDaoPingResp, &PxYYRequestProcessor::pingresponse)
	ON_REQUEST(DBStateReq, &PxYYRequestProcessor::stateRequest)
	ON_REQUEST(DBAdminReq, &PxYYRequestProcessor::adminRequest)
END_FORM_MAP()


PxYYRequestProcessor::PxYYRequestProcessor()
{
}
PxYYRequestProcessor::~PxYYRequestProcessor()
{
}

PHClass* PxYYRequestProcessor::getInstance(Request& request, IConn* conn, InstanceTarget** out)
{
	*out=this;
	return this;
}

void PxYYRequestProcessor::daoheadresponse(::server::router::PProxyDaoResponse* resp)
{
	uint64_t fsmid=0;
	if(decoding_pid_fsmid(resp->context,fsmid)<0)
	{
		log(Warn, "PxYYRequestProcessor::daoheadresponse fsmid parse fail");
		return;	
	}
	
	ReqFSM* pfsm=ReqFSM::findFSM(fsmid);
	if(pfsm==0)
	{
		log(Warn, "PxYYRequestProcessor::daoheadresponse fsm not found");
		return;	
	}
	pfsm->onResponse(resp->result_code,resp->trace_debug,resp->coding,resp->upper_layer,resp->current_free);

}

void PxYYRequestProcessor::pingresponse(::server::router::PProxyDaoPingResp* resp)
{
	log(Warn, "PxYYRequestProcessor::pingresponse recv package extra_info:%s", resp->extra_info.c_str() );
	if(resp->extra_info.size()<2)
	{
		log(Warn, "PxYYRequestProcessor::pingresponse package extra_info size too_small");
		return;
	}
	if(resp->extra_info[0]=='1')
	{
		///dao stat resp process
		std::string subname=resp->extra_info.substr(2);
		gl_daostatmanager->update_dao_info(subname,resp->daoinfos);
		return;
	}
	else if(resp->extra_info[0]=='2')
	{
		///ping stat resp process
		IPPORTYPE get_ipp;
		uint64_t get_start_time=0;
		uint64_t get_end_time=get_current_time_US();

		std::string subinfo=resp->extra_info.substr(2);

		size_t divp=subinfo.find_first_of('_',0);
		if(divp==std::string::npos)
		{
			//package format error
			log(Warn, "PxYYRequestProcessor::pingresponse package format error without _");
			return;
		}
		std::string stim_srt=subinfo.substr(0,divp);
		std::string ipp_srt=subinfo.substr(divp+1);
		std::vector<IPPORTYPE> get_ipp_vec;
		parseStr2VecIPP(ipp_srt,get_ipp_vec);
		if(get_ipp_vec.size()!=1)
		{
			//package format error
			log(Warn, "PxYYRequestProcessor::pingresponse package format error get_ipp_vec.size()!=1");
			return;
		}
		get_ipp=get_ipp_vec[0];
		JLexical::Cast(stim_srt,get_start_time);
		gl_daoconnectionmanager->ping_resp(get_ipp,get_start_time,get_end_time);

	}
}

void PxYYRequestProcessor::stateRequest(server::db_node::DBStateReq * req)
{
	DBStateResp r;
	r.which=req->which;

	stringstream ss;
	ss <<"DaoProxy Debug v0.1  by:" <<gl_myconf->myip <<":" <<gl_myconf->port <<endl;
	if(req->which==0)
	{
		ss<<"[CONTROL]" <<endl;
		ss<<"NetQueueSize:" <<g_pHybirdLinkHandler->get_queue_size() <<endl;
		ss<<"NetQueueLimit:"<<g_pHybirdLinkHandler->get_queue_limit() <<endl;
		ss<<"[STATISTIC]" <<endl;
		ss<<"ReqFSM:"<<ReqFSM::gl_m_fsm_num <<endl;
		ss<<"Package Info" <<endl;

		vector<string> infos;
		gl_traffic_statstic->get_some_histroy(infos,0,10);
		for(size_t b=0;b<infos.size();++b)
		{
			ss<<infos[b];
		}
	}
	else if(req->which==1)
	{
		ss <<"[RouteStrategy]" <<endl;
		gl_route_strategy->debug_show(ss);
		ss <<"[DaoStatus]" <<endl;
		gl_daostatmanager->debug_show(ss);
		ss <<"[ConnectionManager]" <<endl;
		gl_daoconnectionmanager->debug_show(ss);
		ss <<"[ThroughputStat]" <<endl;
		gl_throughput_statstic->debug_show(ss);
	}
	else if(req->which==108)
	{
		r.result=gl_globalconf->getGlobalConfMd5();
		answer(r.uri, RES_SUCCESS, r);
		return;
	}
	else if(req->which==109)
	{
		int start=0;
		int length=0;
		/////////////
		std::map<std::string,std::string> externinf;
		parseString2MapSS(req->reserved,externinf);
		std::string temk;
		temk="start";
		if(externinf.find(temk)==externinf.end())
		{return;}
		JLexical::Cast(externinf[temk],start);
		////////////////////////
		temk="length";
		if(externinf.find(temk)==externinf.end())
		{return;}
		JLexical::Cast(externinf[temk],length);
		//////////
		if(length<0)
		{
			length=0;
		}
		if(length>300)
		{
			length=300;
		}
		stringstream onlyss;
		vector<string> infos;
		gl_traffic_statstic->get_some_histroy(infos,start,length);
		for(size_t b=0;b<infos.size();++b)
		{
			onlyss<<infos[b];
		}
		/////////////
		encryption::zlib_compress(onlyss.str(),r.result);
		answer(r.uri, RES_SUCCESS, r);
		return;
	}

	r.result=ss.str();
	answer(r.uri, RES_SUCCESS, r);
}

void PxYYRequestProcessor::adminRequest(server::db_node::DBAdminReq * req)
{
	server::db_node::DBAdminResp resp;
	resp.req_id=req->req_id;
	resp.option=req->option;
	resp.result=0;

	if(resp.option==111)
	{
		string* pappname;
		string* pdaoname;
		int sendperten=0;
		int charge=0;
		//
		std::map<std::string,std::string> tem_map;
		parseString2MapSS(req->command, tem_map);
		std::map<std::string,std::string>::iterator i;
		i=tem_map.find("app");
		if(i==tem_map.end())
		{
			pappname=0;
		}
		else
		{
			pappname=&i->second;
		}
		i=tem_map.find("dao");
		if(i==tem_map.end())
		{
			pdaoname=0;
		}
		else
		{
			pdaoname=&i->second;
		}
		i=tem_map.find("sendperten");
		if(i==tem_map.end())
		{return;}
		JLexical::Cast(i->second,sendperten);
		i=tem_map.find("charge");
		if(i==tem_map.end())
		{return;}
		JLexical::Cast(i->second,charge);
		///
		gl_msghook->set_hook(req->req_id,writer->get_connection_id_incurrent_contex()
			,pappname,pdaoname,sendperten,charge);

		return;
	}
	else if(resp.option==110)
	{
		//command:app=,dao=,sendperten=,charge=
		//decode command as map
		string* pappname;
		string* pdaoname;
		int sendperten=0;
		int charge=0;
		//
		std::map<std::string,std::string> tem_map;
		parseString2MapSS(req->command, tem_map);
		std::map<std::string,std::string>::iterator i;
		i=tem_map.find("app");
		if(i==tem_map.end())
		{
			pappname=0;
		}
		else
		{
			pappname=&i->second;
		}
		i=tem_map.find("dao");
		if(i==tem_map.end())
		{
			pdaoname=0;
		}
		else
		{
			pdaoname=&i->second;
		}
		i=tem_map.find("sendperten");
		if(i==tem_map.end())
		{return;}
		JLexical::Cast(i->second,sendperten);
		i=tem_map.find("charge");
		if(i==tem_map.end())
		{return;}
		JLexical::Cast(i->second,charge);
		///
		gl_throughput_statstic->set_temp_block(pappname,pdaoname,sendperten,charge);

		answer(resp.uri, RES_SUCCESS, resp);
		return;
	}

	for(;;)
	{

	if(resp.option!=109)
	{
		resp.result=1;//command unrecognize
		break;
	}
	///update global conf
	PxGlobalConf gcf;
	if(gcf.init_from_mem(req->command)<0)
	{
		resp.result=2;//xml verify fail
		break;
	}
	
	if(gl_globalconf->saveToBackUpFile()<0)
	{
		resp.result=3;
		break;
	}
	if(gl_globalconf->updateGlobalConf(req->command)!=0)
	{
		resp.result=4;
		break;
	}
	//update KTM
	gl_hash_ring->init(gl_globalconf->get_hash_ring());
	//update RouteStrategy
	gl_route_strategy->init_load_conf();
	//update DaoManager
	gl_daostatmanager->init_from_conf();
	//update DaoConnection
	gl_daoconnectionmanager->init_from_conf();
	gl_daostatmanager->send_all_empty_dao();
	//update ThroughPut
	resp.result=0;
	break;
	}
	answer(resp.uri, RES_SUCCESS, resp);
}

