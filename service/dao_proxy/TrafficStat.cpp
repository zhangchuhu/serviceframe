#include "TrafficStat.h"
#include "core/sox/logger.h"
#include "server_common/config/tinyxml.h"
#include "server_common/seda/common/any_cast.h"
#include "GlobalCommon.h"
#include "time_util.h"
#include <sstream>
#include "util.h"

using namespace server::dao_proxy;

TrafficStatTimer::TrafficStatTimer()
{
	timer_id=0;
}
TrafficStatTimer::~TrafficStatTimer()
{
}

void TrafficStatTimer::handle_timeout(void* param)
{
	gl_traffic_statstic->swap_stat();
}
void TrafficStatTimer::start_timer(int seconds)
{
	assert(seconds>0);
	if(timer_id!=0)
	{
		g_pTimerContainer->cancel(timer_id);
		timer_id=0;
	}
	timer_id=g_pTimerContainer->schedule( *this,seconds,seconds);
}

TrafficStat::TrafficStat()
{
	m_incseq=0;

	std::stringstream sts;
	debug_show_drop_resp(sts,m_incseq);
	log_histroy.push_front(sts.str());
	log_his_size=1;
}
TrafficStat::~TrafficStat()
{
}
int TrafficStat::init()
{
	start_timer.start_timer(10);
	return 0;
}

////struct TrafficStat::EachSTAT
TrafficStat::EachSTAT::EachSTAT()
{
	send_num=0;
	recv_num=0;
	drop_num=0;
	ave_resp_time=0.0;
}
TrafficStat::EachSTAT::~EachSTAT()
{
}
TrafficStat::EachSTAT::EachSTAT(const TrafficStat::EachSTAT& o)
{
	send_num=o.send_num;
	recv_num=o.recv_num;
	drop_num=o.drop_num;
	ave_resp_time=o.ave_resp_time;
}
TrafficStat::EachSTAT& TrafficStat::EachSTAT::operator = (const TrafficStat::EachSTAT& o)
{
	send_num=o.send_num;
	recv_num=o.recv_num;
	drop_num=o.drop_num;
	ave_resp_time=o.ave_resp_time;
	return *this;
}
void TrafficStat::EachSTAT::recv_one_resp(float rtime,int cnt)
{
	recv_num+=cnt;
	float K=((float)cnt)/((float)recv_num);
	ave_resp_time+=((rtime-ave_resp_time)*K);
}
void TrafficStat::EachSTAT::debug_show(std::stringstream& ss)
{
	float drop_rate=(((float)drop_num)/(float)(recv_num+drop_num+1));
	drop_rate*=100000;
	uint64_t val_p=drop_rate;
	float fttt=val_p;
	fttt/=1000.0;
	ss <<"send:" <<send_num <<" recv:" <<recv_num <<" drop:" <<drop_num <<" resp:"<<ave_resp_time <<" drop_rate:" <<fttt <<"%";
}
////////////
void TrafficStat::stat_send_one(int from_gid,int to_gid,std::string& daoserver,std::string* app_name,std::string* dao_name,int send_cnt)
{
	m_togroup_stat[to_gid].send_num+=send_cnt;
	if(app_name!=0)
	{
		m_app_stat[*app_name].send_num+=send_cnt;
		if(dao_name!=0)
		{
			m_app_dao_stat[*app_name][*dao_name].send_num+=send_cnt;
		}
	}
	if(dao_name!=0)
	{
		m_dao_stat[*dao_name].send_num+=send_cnt;
		m_server_dao_stat[daoserver][*dao_name].send_num+=send_cnt;
	}
	m_stat.send_num+=send_cnt;
}
void TrafficStat::stat_recv_one(int from_gid,int to_gid,std::string& daoserver,std::string* app_name,std::string* dao_name,int recv_cnt,float resp_time)
{
	if(recv_cnt<0)
	{
		m_togroup_stat[to_gid].drop_num++;
		if(app_name!=0)
		{
			m_app_stat[*app_name].drop_num++;
			if(dao_name!=0)
			{
				m_app_dao_stat[*app_name][*dao_name].drop_num++;
			}
		}
		if(dao_name!=0)
		{
			m_dao_stat[*dao_name].drop_num++;
			m_server_dao_stat[daoserver][*dao_name].drop_num++;
		}
		m_stat.drop_num++;
	}
	else
	{
		m_togroup_stat[to_gid].recv_one_resp(resp_time,recv_cnt);
		if(app_name!=0)
		{
			m_app_stat[*app_name].recv_one_resp(resp_time,recv_cnt);
			if(dao_name!=0)
			{
				m_app_dao_stat[*app_name][*dao_name].recv_one_resp(resp_time,recv_cnt);
			}
		}
		if(dao_name!=0)
		{
			m_dao_stat[*dao_name].recv_one_resp(resp_time,recv_cnt);
			m_server_dao_stat[daoserver][*dao_name].recv_one_resp(resp_time,recv_cnt);
		}
		m_stat.recv_one_resp(resp_time,recv_cnt);
	}

}

int TrafficStat::get_dao_server_last_sendcnt(std::string& dao_name,std::string& sname,uint32_t& last_send)
{
	std::map<std::string,std::map<std::string,EachSTAT> >::iterator isd=
		m_server_dao_last_stat.find(sname);
	if(isd==m_server_dao_last_stat.end())
	{
		return -1;
	}
	std::map<std::string,EachSTAT>::iterator b=isd->second.find(dao_name);
	if(b==isd->second.end())
	{
		return -1;
	}
	last_send=b->second.send_num;
	return 0;
}
void TrafficStat::swap_stat()
{
	m_togroup_last_stat.swap(m_togroup_stat);
	m_app_last_stat.swap(m_app_stat);
	m_dao_last_stat.swap(m_dao_stat);
	m_server_dao_last_stat.swap(m_server_dao_stat);
	m_app_dao_last_stat.swap(m_app_dao_stat);
	m_last_stat=m_stat;

	m_last_error_count.swap(m_error_count);
	////////////
	std::stringstream sts;
	++m_incseq;
	debug_show_drop_resp(sts,m_incseq);
	log_histroy.push_front(sts.str());++log_his_size;
	if(log_his_size>3600)
	{
		log_histroy.pop_back();--log_his_size;
	}
	////////////
	std::map<int,int>().swap(m_error_count);
	////
	std::map<int,EachSTAT>().swap(m_togroup_stat);
	std::map<std::string,EachSTAT>().swap(m_app_stat);
	std::map<std::string,EachSTAT>().swap(m_dao_stat);
	std::map<std::string,std::map<std::string,EachSTAT> >().swap(m_server_dao_stat);
	std::map<std::string,std::map<std::string,EachSTAT> >().swap(m_app_dao_stat);

	EachSTAT new_sta;
	m_stat=new_sta;
	////////////
	gl_daostatmanager->learning_lms_shrink();

	std::map<std::string,std::map<std::string,EachSTAT> >::iterator isd=m_server_dao_last_stat.begin();
	for(;isd!=m_server_dao_last_stat.end();isd++)
	{
		for(std::map<std::string,EachSTAT>::iterator b=isd->second.begin();
			b!=isd->second.end();b++)
		{
			double pred_respt=0;
			if(b->second.drop_num==0)
			{
				gl_daostatmanager->learning_lms(isd->first,b->first,b->second.ave_resp_time,b->second.recv_num,pred_respt);
			}
		}
	}
}

int TrafficStat::get_dao_last_sendcnt(std::string& dao_name,uint32_t& last_send)
{
	std::map<std::string,EachSTAT>::iterator vvv=m_dao_last_stat.find(dao_name);
	if(vvv==m_dao_last_stat.end())
	{
		return -1;
	}
	last_send=vvv->second.send_num;
	return 0;
}
void TrafficStat::debug_show_drop_resp(std::stringstream& ss,uint64_t seq_num)
{
	uint64_t curtime_s=time(0);
	std::string time_string;
	time_util::transfer_time(curtime_s,time_string);
	ss <<"<start: " <<gl_myconf->myip <<"_" <<gl_processpid <<"_" <<seq_num <<"> times:" <<curtime_s <<" T:" <<time_string <<endl;
	ss<<"<global> ";m_last_stat.debug_show(ss);
	ss <<endl;
	for(std::map<int,EachSTAT>::iterator b=m_togroup_last_stat.begin();
		b!=m_togroup_last_stat.end();b++)
	{
		ss<<"<groupid:" <<b->first <<"> ";
		b->second.debug_show(ss);
		ss <<endl;
	}
	for(std::map<std::string,EachSTAT>::iterator b=m_app_last_stat.begin();
		b!=m_app_last_stat.end();b++)
	{
		ss<<"<app:" <<b->first <<"> ";
		b->second.debug_show(ss);
		ss <<endl;
	}
	for(std::map<std::string,EachSTAT>::iterator b=m_dao_last_stat.begin();
		b!=m_dao_last_stat.end();b++)
	{
		ss<<"<dao:" <<b->first <<"> ";
		b->second.debug_show(ss);
		ss <<endl;
	}
	std::map<std::string,std::map<std::string,EachSTAT> >::iterator isd=m_server_dao_last_stat.begin();
	for(;isd!=m_server_dao_last_stat.end();isd++)
	{
		for(std::map<std::string,EachSTAT>::iterator b=isd->second.begin();
			b!=isd->second.end();b++)
		{
			ss<<"<sname:"<<isd->first <<" dao:" <<b->first <<"> ";
			b->second.debug_show(ss);
			double pred_respt=0;
			if(b->second.drop_num==0)
			{
				gl_daostatmanager->learning_predict_lms(isd->first,b->first,b->second.ave_resp_time,b->second.recv_num,pred_respt);
			}
			ss <<" predict_resp:" <<pred_respt ;
			ss <<endl;
		}
	}

	std::map<std::string,std::map<std::string,EachSTAT> >::iterator iad=m_app_dao_last_stat.begin();
	for(;iad!=m_app_dao_last_stat.end();iad++)
	{
		for(std::map<std::string,EachSTAT>::iterator b=iad->second.begin();
			b!=iad->second.end();b++)
		{
			ss<<"<app:"<<iad->first <<" dao:" <<b->first <<"> ";
			b->second.debug_show(ss);
			ss <<endl;
		}
	}
	/////
	ss <<"<error_code_count: > " <<parseMapII2String(m_last_error_count) <<endl;
	ss <<"####" <<endl;

}

void TrafficStat::get_some_histroy(std::vector<std::string>& his_str,int start,int leng)
{
	his_str.clear();
	////////////
	int cntt=0;
	std::list<std::string>::iterator i=log_histroy.begin();
	for(;i!=log_histroy.end();++i)
	{
		if(cntt>=start)
		{
			if(cntt>=start+leng)
			{
				break;
			}
			his_str.push_back(*i);
		}
		cntt++;
	}
}

void TrafficStat::stat_error_one(int error_code)
{
	std::map<int,int>::iterator v=m_error_count.find(error_code);
	if(v!=m_error_count.end())
	{
		v->second++;
	}
	else
	{
		m_error_count[error_code]=1;
	}
}

