#include "request_fsm.h"
#include "GlobalCommon.h"
#include "time_util.h"
#include "util.h"

#include "server_common/config/IServerConfig.h"
#include "core/sox/logger.h"
#include "server_common/protocol/prouter.h"

#include "server_common/seda/common/any_cast.h"
#include "px_error_code.h"

using namespace server::dao_proxy;
using namespace server::router;

void coding_pid_fsmid(std::string& context,uint64_t fsmid)
{
	context.clear();
	char bf[sizeof(uint64_t)];
	*(uint64_t*)bf=gl_processpid;
	context.append(bf,bf+sizeof(uint64_t));
	*(uint64_t*)bf=fsmid;
	context.append(bf,bf+sizeof(uint64_t));
}


ReqFSM::ReqFSM()
{
	m_u64TimerId=0;
	m_u64FsmId=0;
}

ReqFSM::~ReqFSM()
{
	if(m_u64TimerId!=0)
	{
		g_pTimerContainer->cancel(m_u64TimerId);
		m_u64TimerId=0;
	}
}
void ReqFSM::start_timer(int seconds)
{
	if(m_u64TimerId!=0)
	{
		g_pTimerContainer->cancel(m_u64TimerId);
		m_u64TimerId=0;
	}
	m_u64TimerId=g_pTimerContainer->schedule(*this,seconds);
}

uint32_t ReqFSM::gl_m_fsm_num = 0;
ReqFSM* ReqFSM::createFSM()
{
	server::dao_proxy::ANYPTRTYPE* p;
	uint64_t u64NewFsmId = 0;
	if(g_pFSMContainer->new_id(u64NewFsmId,p)!=0)
	{
		return NULL;
	}
	ReqFSM* pNewFSM = new ReqFSM();
	pNewFSM->m_u64FsmId = u64NewFsmId;
	*p = reinterpret_cast<server::dao_proxy::ANYPTRTYPE>(pNewFSM);

	++gl_m_fsm_num;
	return pNewFSM;

}
ReqFSM* ReqFSM::findFSM(uint64_t u64FSMId)
{
	server::dao_proxy::ANYPTRTYPE* vv = NULL;
	if (0 == g_pFSMContainer->find_id(u64FSMId, vv))
	{
		ReqFSM* p = reinterpret_cast<ReqFSM*>(*vv);
		return p;
	}
	return 0;
}
void ReqFSM::destroyFSM(uint64_t u64FSMId)
{
	server::dao_proxy::ANYPTRTYPE* vv = NULL;
	if (0 == g_pFSMContainer->delete_id(u64FSMId, vv))
	{
		ReqFSM* p = reinterpret_cast<ReqFSM*>(*vv);
		delete p;
		--gl_m_fsm_num;
	}
}
std::string ReqFSM::trace_debug_mytag()
{
	std::string time_readable;
	time_util::transfer_time(get_current_time_S(),time_readable);
	std::string strpidd;
	JLexical::Cast(gl_processpid,strpidd);
	return (gl_myconf->server_name+"_"+gl_myconf->myip+"_"+strpidd+":["+time_readable+"]");
}
int gl_routestrategy_getU(server::dao_proxy::ROUTESTRATEGY_TYPE& rs)
{
	std::map<std::string,std::string>::iterator ivv=rs.find("U");
	if(ivv==rs.end())
	{
		return 0;//default U=0
	}
	int val=0;
	JLexical::Cast(ivv->second,val);
	return val;
}
void gl_add_client_spe(server::dao_proxy::ROUTESTRATEGY_TYPE& rs,std::string& cli_str)
{
	std::map<std::string,std::string> cval;
	parseString2MapSS(cli_str,cval);
	std::map<std::string,std::string>::iterator ivv=cval.find("E");
	if(ivv!=cval.end())
	{
		rs["E"]=ivv->second;
	}
	ivv=cval.find("U");
	if(ivv!=cval.end())
	{
		if(ivv->second=="0"
			||ivv->second=="1"
			||ivv->second=="2")
		{
			rs["U"]=ivv->second;
		}
	}
}
void ReqFSM::start(int32_t thhedid,uint64_t cid,std::string& return_function,RouteReq* proute_header,std::string& fname,const char* dat,uint32_t len)
{
	thrift_headid=thhedid;
	req_connection_id=cid;
	m_fname=fname;
	return_function_name=return_function;
	std::string* myname=0;
	if(proute_header==0)
	{
		m_app_name.clear();
	}
	else
	{
		m_app_name=proute_header->caller_name;
		myname=&m_app_name;
		m_routeresp_pk.context=proute_header->context;
		m_routeresp_pk.result_code=0;
		m_routeresp_pk.trace_debug=proute_header->trace_debug;
		m_routeresp_pk.coding=0;
		m_routeresp_pk.__isset.context=true;
	}
	//////throttle control
	int licence_rt=gl_throughput_statstic->check_and_get_tickets(myname,&fname);
	if(licence_rt!=0)
	{
		if(licence_rt==2)
		{
			static ErrorCodeObj te(std::string("no_licence"));
			gl_traffic_statstic->stat_error_one(te.code);
		}
		else if(licence_rt==1)
		{
			static ErrorCodeObj te(std::string("throttled"));
			gl_traffic_statstic->stat_error_one(te.code);
		}
		else if(licence_rt==3)
		{
			static ErrorCodeObj te(std::string("throttled"));
			gl_traffic_statstic->stat_error_one(te.code);
		}
		end();
		return;
	}
	///////////////
	//choose dao
	server::dao_proxy::ROUTESTRATEGY_TYPE cur_rst_type;
	gl_route_strategy->get_route_str(myname,&fname,cur_rst_type);
	if(proute_header!=0)
	{
		gl_add_client_spe(cur_rst_type,proute_header->route_option);
	}

	DaoStatManager::ServINFO* p_select_srv=0;
	std::string final_server_selected;
	int sele_r=gl_daostatmanager->dao_select(fname,gl_myconf->groupid,cur_rst_type,p_select_srv,final_server_selected);
	if(sele_r==-2)
	{
		std::string thrift_ret;
		gl_thriftproxymsghandler->generate_fname_not_found_pack(thrift_headid,return_function_name,thrift_ret);
		std::string no_dao="no_dao";
		send_resp_thrift_packet(thrift_ret,req_connection_id,&no_dao);
		log(Warn, "ReqFSM::start no dao selected , dao:%s",fname.c_str() );
		end();
		static ErrorCodeObj te(std::string("nodao"));
		gl_traffic_statstic->stat_error_one(te.code);
		return;
	}
	else if(sele_r<0)
	{
		std::string thrift_ret;
		std::string excep_desc="dao busy";
		gl_thriftproxymsghandler->generate_fname_unknown_exception_pack(thrift_headid,return_function_name,excep_desc,thrift_ret);
		std::string dao_busy="dao_busy";
		send_resp_thrift_packet(thrift_ret,req_connection_id,&dao_busy);
		log(Warn, "ReqFSM::start dao busy selected , dao:%s",fname.c_str() );
		end();
		static ErrorCodeObj te(std::string("daobusy"));
		gl_traffic_statstic->stat_error_one(te.code);
		return;
	}
	assert(p_select_srv!=0);

	///connect select
	IPPORTYPE act_ipp;
	if(gl_daoconnectionmanager->select_best_connection(p_select_srv->ipports,act_ipp)<0)
	{
		log(Warn, "ReqFSM::start dao ipport empty select_best_connection");
		end();
		static ErrorCodeObj te(std::string("unknown_error"));
		gl_traffic_statstic->stat_error_one(te.code);
		return;
	}

	//////
	gl_daostatmanager->predict_send_one(final_server_selected,p_select_srv,fname);
	to_groupid=p_select_srv->groupid;

	//////get connection
	uint64_t send_connectid=0;
	if(gl_daoconnectionmanager->get_connection(act_ipp,send_connectid)<0)
	{
		log(Warn, "ReqFSM::start get_connection fail");
		end();
		static ErrorCodeObj te(std::string("unknown_error"));
		gl_traffic_statstic->stat_error_one(te.code);
		return;
	}

	//////prepare packetage
	PProxyDaoRequest ppdr;
	coding_pid_fsmid(ppdr.context,m_u64FsmId);
	if(proute_header==0)
	{
		ppdr.caller_name="nohead";
		ppdr.authorize_token="It's me!!";
		ppdr.route_option="";
		ppdr.priority_hint=0;
		ppdr.trace_debug=trace_debug_mytag();
	}
	else
	{
		ppdr.caller_name=proute_header->caller_name;
		ppdr.authorize_token=proute_header->authorize_token;
		ppdr.route_option=proute_header->route_option;
		ppdr.priority_hint=proute_header->priority_hint;
		ppdr.trace_debug=trace_debug_mytag()+"//@"+proute_header->trace_debug;
	}
	ppdr.coding=0;
	ppdr.upper_layer.assign(dat,dat+len);

	///////////////
	core::Sender sdr;
	sdr.clear();
	sdr.setUri(PProxyDaoRequest::uri);
	sdr.setResCode(200);
	sdr.marshall(ppdr);
	sdr.endPack();
	if(gl_daoconnectionmanager->get_connectionfactory()->send_pack(send_connectid,sdr)<0)
	{
		std::string thrift_ret;
		std::string excep_desc="network error";
		gl_thriftproxymsghandler->generate_fname_unknown_exception_pack(thrift_headid,return_function_name,excep_desc,thrift_ret);
		std::string send_fail="send_fail";
		send_resp_thrift_packet(thrift_ret,req_connection_id,&send_fail);
		log(Warn, "ReqFSM::start send_pack fail");
		end();
		static ErrorCodeObj te(std::string("network_error"));
		gl_traffic_statstic->stat_error_one(te.code);
		return;
	}
	
	abs_send_time_us=get_current_time_US();
	gl_traffic_statstic->stat_send_one(gl_myconf->groupid,to_groupid,final_server_selected,myname,&fname,1);
	////get u
	if(gl_routestrategy_getU(cur_rst_type)==0)
	{
		start_timer(13);
	}
	else
	{
		start_timer(5);
	}
	gl_daostatmanager->start_send_one(p_select_srv,fname);
	m_select_server_name=final_server_selected;
}
void ReqFSM::onResponse(uint32_t result_code,std::string& trace_debug,uint32_t coding,std::string& upper_layer,uint32_t free)
{
	uint64_t udiff_time=get_current_time_US()-abs_send_time_us;
	//log(Warn, "enter ReqFSM::onResponse time:%llu trace_debug:%s",udiff_time,trace_debug.c_str());

	std::string* is_papp_name=0;
	if(!m_app_name.empty())
	{
		is_papp_name=&m_app_name;
	}
	if(udiff_time>10000000)
	{
		udiff_time=10000000;
	}
	gl_traffic_statstic->stat_recv_one(gl_myconf->groupid,to_groupid,m_select_server_name,is_papp_name,&m_fname,1,(float)udiff_time);
	gl_daostatmanager->end_recv_one(m_select_server_name,m_fname);
	gl_daostatmanager->update_serverdao_free(m_select_server_name,m_fname,free);

	m_routeresp_pk.trace_debug=trace_debug;
	send_resp_thrift_packet(upper_layer,req_connection_id,0);

	end();
	return;
}

void ReqFSM::end()
{
	ReqFSM::destroyFSM(m_u64FsmId);
}

void ReqFSM::handle_timeout(void* param)
{
	std::string thrift_ret;
	std::string excep_desc="network error";
	gl_thriftproxymsghandler->generate_fname_unknown_exception_pack(thrift_headid,return_function_name,excep_desc,thrift_ret);
	std::string s_timeout="timeout";
	send_resp_thrift_packet(thrift_ret,req_connection_id,&s_timeout);

	log(Warn, "enter ReqFSM::handle_timeout fsm terminate fsmid:%llu",m_u64FsmId);
	std::string* is_papp_name=0;
	if(!m_app_name.empty())
	{
		is_papp_name=&m_app_name;
	}
	gl_traffic_statstic->stat_recv_one(gl_myconf->groupid,to_groupid,m_select_server_name,is_papp_name,&m_fname,-1,(float)0.0);
	gl_daostatmanager->end_recv_one(m_select_server_name,m_fname);

	DaoStatManager::ServINFO* p_select_srv=0;
	gl_daostatmanager->get_server_ptr(m_select_server_name,p_select_srv);
	if(p_select_srv!=0)
	{
		std::map<std::string,DaoStatManager::DaoINFO>::iterator fdi=
			p_select_srv->dao_infos.find(m_fname);
		if(fdi!=p_select_srv->dao_infos.end())
		{
			fdi->second.m_lms_b+=100.0;
		}
		gl_daoconnectionmanager->predict_tiny_add_bad_level(p_select_srv->ipports);
	}	
	end();
}

void ReqFSM::send_resp_thrift_packet(std::string& thriftpack,uint64_t connectid,std::string* ret_addition_debug)
{
	std::string* loc_final_send=&thriftpack;
	std::string with_rresp_head_thrift;
	if(m_routeresp_pk.__isset.context==true)
	{
		////call with head
		m_routeresp_pk.__isset.coding=true;
		m_routeresp_pk.__isset.result_code=true;
		m_routeresp_pk.__isset.trace_debug=true;
		m_routeresp_pk.__isset.upper_layer=true;
		m_routeresp_pk.result_code=0;
		if(ret_addition_debug!=0)
		{
			m_routeresp_pk.trace_debug=trace_debug_mytag()+(*ret_addition_debug)+"//@"+m_routeresp_pk.trace_debug;
		}
		else
		{
			m_routeresp_pk.trace_debug=trace_debug_mytag()+"//@"+m_routeresp_pk.trace_debug;
		}
		m_routeresp_pk.coding=0;
		m_routeresp_pk.upper_layer=thriftpack;
		///////////////////
		gl_thriftproxymsghandler->generate_routeresp_pack(headofthrift_headid,&m_routeresp_pk,with_rresp_head_thrift);
		loc_final_send=&with_rresp_head_thrift;
	}
	/////////fix resp string
	std::string p_str;
	char dtt[4];
	*((int*)dtt) = htonl(loc_final_send->size());
	p_str.assign(dtt,4);
	p_str.append(*loc_final_send);

	////send
	IConn* pcon = gl_service_connection_manager->getConnectById((uint32_t)connectid);
	if(pcon)
	{
		try{
			pcon->sendBin(p_str.data(), p_str.size(), -1);
		}catch(std::exception &ex){
			pcon->setEnable(false);
			log(Error, "send error!:%s", ex.what());
			end();
			return;
		}
	}
}

