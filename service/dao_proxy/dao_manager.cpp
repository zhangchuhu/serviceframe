#include "dao_manager.h"
#include "core/sox/logger.h"
#include <assert.h>
#include "util.h"
#include "server_common/seda/common/any_cast.h"
#include "GlobalCommon.h"
#include "server_common/seda/syncall/remote_call.hpp"
#include "common/core/sender.h"
#include "common/core/request.h"
#include <algorithm>

using namespace std;
using namespace server::dao_proxy;

const float    gl_fine_response_time_us=2000.0;
const uint32_t gl_load_rank_diff=300;

const double    gl_lms_learn_rate=0.3;

namespace
{
class general_frame : public seda::IFraming
{
public:
	int frame(const char* dat,int len)
	{
		if(len<10)
		{
			return 10;
		}
		uint32_t pack_len=*(uint32_t*)dat ;
		if(pack_len>(2*1024*1024))
		{
			return -1;
		}
		return pack_len;
	}
	static general_frame* instance()
	{
		static general_frame obj;
		return &obj;
	}
};
}
DaoStatTimer::DaoStatTimer()
{
	timer_id=0;
}
DaoStatTimer::~DaoStatTimer()
{

}

void file_gl_send_poll_packet(const std::string& server_name,DaoStatManager::ServINFO* servinfo)
{
	DaoConnectionManager::IPINFO result_info;
	IPPORTYPE ipp;
	if(gl_daoconnectionmanager->select_best_connection(servinfo->ipports,ipp,&result_info)!=0)
	{
		return;
	}
	if(result_info.bad_level>2)
	{
		return;
	}
	uint64_t connect_id=0;
	gl_daoconnectionmanager->get_connection(ipp,connect_id);
	///////////////////
	//////prepare packetage
	::server::router::PProxyDaoPing ppdr;
	ppdr.type=1;
	ppdr.extra_info="1_"+server_name;

	core::Sender sdr;
	sdr.clear();
	sdr.setUri( ::server::router::PProxyDaoPing::uri);
	sdr.setResCode(200);
	sdr.marshall(ppdr);
	sdr.endPack();
	gl_daoconnectionmanager->get_connectionfactory()->send_pack(connect_id,sdr);
}

void DaoStatTimer::handle_timeout(void* param)
{
	std::map<std::string, DaoStatManager::ServINFO>::iterator i;
	for(i=gl_daostatmanager->serv_infos.begin();
		i!=gl_daostatmanager->serv_infos.end();
		++i)
	{
		file_gl_send_poll_packet(i->first,&i->second);
	}
	
}
void DaoStatTimer::start_timer(int seconds)
{
	assert(seconds>0);
	if(timer_id!=0)
	{
		g_pTimerContainer->cancel(timer_id);
		timer_id=0;
	}
	timer_id=g_pTimerContainer->schedule( *this,seconds,seconds);
}


DaoStatManager::DaoStatManager()
{
}
DaoStatManager::~DaoStatManager()
{
}

int DaoStatManager::init_from_conf()
{
	std::map<std::string, PxGlobalConf::PrcAdd> outdat;
	gl_globalconf->get_all_dao(outdat);
	std::map<std::string, PxGlobalConf::PrcAdd>::iterator n_ip;
	std::map<std::string, ServINFO>::iterator o_ip;
	for(n_ip=outdat.begin();n_ip!=outdat.end();++n_ip)
	{
		o_ip=serv_infos.find(n_ip->first);
		if(o_ip==serv_infos.end())
		{
			ServINFO srvinfo;
			srvinfo.ipports=n_ip->second.ipports;
			srvinfo.groupid=n_ip->second.groupid;
			srvinfo.last_respping_time=0;
			srvinfo.b_immreq_stat=0;
			inner_set_server(n_ip->first,srvinfo);
		}
		else
		{
			if(o_ip->second.ipports!=n_ip->second.ipports
				||o_ip->second.groupid!=n_ip->second.groupid)
			{
				ServINFO srvinfo;
				srvinfo.ipports=n_ip->second.ipports;
				srvinfo.groupid=n_ip->second.groupid;
				srvinfo.last_respping_time=0;
				srvinfo.b_immreq_stat=0;
				inner_set_server(n_ip->first,srvinfo);
			}
		}
	}
	for(o_ip=serv_infos.begin();o_ip!=serv_infos.end();)
	{
		n_ip=outdat.find(o_ip->first);
		if(n_ip==outdat.end())
		{
			std::string temname=o_ip->first;
			++o_ip;
			inner_del_server(temname);
		}
		else
		{
			++o_ip;
		}
	}
	return 0;
}

void DaoStatManager::send_all_empty_dao()
{
	std::map<std::string, DaoStatManager::ServINFO>::iterator i;
	for(i=serv_infos.begin();
		i!=serv_infos.end();
		++i)
	{
		if(i->second.dao_infos.empty())
		{
			file_gl_send_poll_packet(i->first,&i->second);
		}
	}
}

int DaoStatManager::rc_calltest()
{
	int good_send=0;
	int bad_send=0;

	for(std::map<std::string, ServINFO>::iterator i=serv_infos.begin();i!=serv_infos.end();)
	{
		seda::remote_call rc;

		IPPORTYPE ipp;
		if(gl_daoconnectionmanager->select_best_connection(i->second.ipports,ipp)!=0)
		{
			bad_send++;
			++i;
			continue;
		}
		///////////////////
		rc.setup(ipp.first.c_str(),ipp.second,2);

		//////prepare packetage
		::server::router::PProxyDaoPing ppdr;
		ppdr.type=1;
		ppdr.extra_info="1_"+i->first;

		core::Sender sdr;
		sdr.clear();
		sdr.setUri( ::server::router::PProxyDaoPing::uri);
		sdr.setResCode(200);
		sdr.marshall(ppdr);
		sdr.endPack();
		vector<char> send;
		vector<char> recv;
		send.assign(sdr.header(),sdr.header()+sdr.bodySize()+sdr.headerSize());

		int ret=rc.call( send, general_frame::instance(),recv, 500 );
		if(ret<0||recv.empty())
		{
			bad_send++;
			++i;
			continue;
		}
		core::Request reqest(&recv[0],recv.size());
		::server::router::PProxyDaoPingResp op_resp;
		try
		{
			reqest.head();
			if(reqest.getUri()!= ::server::router::PProxyDaoPingResp::uri)
			{
				bad_send++;
				++i;
				continue;
			}
			op_resp.unmarshal(reqest.getPackData());
		}
		catch(sox::PacketError pe)
		{
			bad_send++;
			++i;
			continue;
		}
		if(op_resp.extra_info!=ppdr.extra_info)
		{
			bad_send++;
			++i;
			continue;
		}
		//////////start process///////////////////
		std::string subname=op_resp.extra_info.substr(2);
		++i;
		gl_daostatmanager->update_dao_info(subname,op_resp.daoinfos);
		++good_send;
	}

	if(bad_send>good_send)
	{
		////too many fail
		return -1;
	}
	return 0;
}

int DaoStatManager::update_dao_info(std::string& srvname,std::vector< ::server::router::DAOLOAD>& daoinfos)
{
	std::map<std::string, ServINFO>::iterator z
		=serv_infos.find(srvname);
	if(z==serv_infos.end())
	{
		return -1;
	}
	ServINFO srvinf;
	srvinf.ipports=z->second.ipports;
	srvinf.groupid=z->second.groupid;
	srvinf.last_respping_time=get_current_time_S();
	srvinf.b_immreq_stat=0;

	for(size_t b=0;b<daoinfos.size();++b)
	{
		DaoINFO dinf;
		dinf.curren_load=daoinfos[b].load;
		dinf.tick_remain=daoinfos[b].free;
		dinf.dao_type=daoinfos[b].db_type;////1:master db  2:slave db
		dinf.dao_extent_info=daoinfos[b].extra_info;
		dinf.debug_info="";
		dinf.tick_remain_last_poll=dinf.tick_remain;
		dinf.sending_request=0;
		dinf.m_lms_k=1.0;
		dinf.m_lms_b=5000.0;
		srvinf.dao_infos[daoinfos[b].dao_name ]=dinf;
	}
	inner_set_server(srvname,srvinf);
	return 0;
}

int DaoStatManager::inner_del_server(const std::string& server_name)
{
	std::map<std::string, ServINFO>::iterator si=serv_infos.find(server_name);
	if(si==serv_infos.end())
	{
		return 0;
	}
	ServINFO& temser=si->second;
	std::map<std::string,DaoINFO>::iterator di=temser.dao_infos.begin();
	for(;di!=temser.dao_infos.end();++di)
	{
		/////
		std::map<std::string,DaoIndex>::iterator dx=dao_index.find(di->first);
		if(dx==dao_index.end())
		{
			assert(false);
		}
		DaoIndex& temdaof=dx->second;
		temdaof.servers.erase(server_name);
		if(temdaof.servers.empty())
		{
			dao_index.erase(dx);
		}
	}
	///////////////
	serv_infos.erase(si);
	return 0;
}
int DaoStatManager::inner_set_server(const std::string& server_name,ServINFO& info)
{
	std::map<std::string, ServINFO>::iterator si=serv_infos.find(server_name);
	if(si!=serv_infos.end())
	{
		//fill dynamic_stat
		std::map<std::string,DaoINFO>::iterator vv=info.dao_infos.begin();
		for(;vv!=info.dao_infos.end();++vv)
		{
			std::map<std::string,DaoINFO>::iterator fd_old
				=si->second.dao_infos.find(vv->first);
			if(fd_old!=si->second.dao_infos.end())
			{
				vv->second.sending_request=fd_old->second.sending_request;
				vv->second.m_lms_k=fd_old->second.m_lms_k;
				vv->second.m_lms_b=fd_old->second.m_lms_b;
			}
		}
		inner_del_server(server_name);
	}

	ServINFO& temser=info;
	std::map<std::string,DaoINFO>::iterator di=temser.dao_infos.begin();
	for(;di!=temser.dao_infos.end();++di)
	{
		std::map<std::string,DaoIndex>::iterator dx=dao_index.find(di->first);
		if(dx==dao_index.end())
		{
			DaoIndex nn;
			dao_index[di->first]=nn;
			dx=dao_index.find(di->first);
		}
		DaoIndex& temdaof=dx->second;
		temdaof.servers.insert(server_name);
	}

	serv_infos[server_name]=info;
	return 0;
}
int DaoStatManager::update_serverdao_free(const std::string& sername,const std::string& daoname,uint32_t free)
{
	std::map<std::string, ServINFO>::iterator i=serv_infos.find(sername);
	if(i==serv_infos.end())
	{
		return -1;
	}
	std::map<std::string,DaoINFO>::iterator v=i->second.dao_infos.find(daoname);
	if(v==i->second.dao_infos.end())
	{
		return -1;
	}
	v->second.tick_remain=free;
	return 0;
}
int DaoStatManager::get_server_ptr(const std::string& ser_name,ServINFO*& out_serv)
{
	std::map<std::string, ServINFO>::iterator i=serv_infos.find(ser_name);
	if(i==serv_infos.end())
	{
		return -1;
	}
	out_serv=&i->second;
	return 0;
}
////filter 'A'local 'B'master 'C'slave
int DaoStatManager::dao_select(std::string& dao_name,int localgroupid,ROUTESTRATEGY_TYPE& roustg,ServINFO*& out_serv,std::string& out_ser_name)
{
	std::string filter="A";
	ROUTESTRATEGY_TYPE::iterator stg_i;
	stg_i=roustg.find(std::string("S"));
	if(stg_i!=roustg.end())
	{
		filter=stg_i->second;
	}

	////////////////////////////
	///first select all
	std::map<std::string,DaoIndex>::iterator dx=dao_index.find(dao_name);
	if(dx==dao_index.end())
	{
		return -2;
	}
	DaoIndex& didx=dx->second;
	std::vector<std::string> server_list;
	std::set<std::string>::iterator si=didx.servers.begin();
	for(;si!=didx.servers.end();++si)
	{
		server_list.push_back(*si);
	}
	///////////
	char final_ch='D';
	for(size_t i=0;i<filter.size();++i)
	{
		if(filter[i]=='A')
		{
			inner_filter_help_A(server_list,dao_name,localgroupid);
		}
		else if(filter[i]=='B')
		{
			inner_filter_help_B(server_list,dao_name);
		}
		else if(filter[i]=='C')
		{
			inner_filter_help_C(server_list,dao_name);
		}
		else if(filter[i]=='E')
		{
			stg_i=roustg.find(std::string("E"));
			if(stg_i!=roustg.end())
			{
				std::string hash_id=stg_i->second;
				inner_filter_help_E(server_list,dao_name,hash_id);
			}
		}
		else if(filter[i]=='U')
		{
			stg_i=roustg.find(std::string("U"));
			if(stg_i!=roustg.end())
			{
				int usability_level=1;
				JLexical::Cast(stg_i->second,usability_level);
				inner_filter_help_U(server_list,dao_name,usability_level);
			}
		}
		else if(filter[i]=='F')
		{
			stg_i=roustg.find(std::string("F"));
			if(stg_i!=roustg.end())
			{
				std::set<int> groupids;
				parseStr2SetInt(stg_i->second,groupids);
				if(!groupids.empty())
				{
					inner_filter_help_F(server_list,dao_name,groupids);
				}
			}
		}
		else if(filter[i]=='D')
		{
			final_ch='D';
		}
		else if(filter[i]=='R')
		{
			final_ch='R';
		}
		else if(filter[i]=='L')
		{
			final_ch='L';
		}
		if(server_list.empty())
		{
			//no server
			return -1;
		}
	}
	if(server_list.empty())
	{
		//no server
		return -1;
	}
	std::string final_server;
	if(final_ch=='D')
	{
		if(inner_filter_highresponse_lmspredict(server_list,dao_name,final_server)<0)
		{return -1;}
	}
	else if(final_ch=='R')
	{
		if(inner_filter_highresponse_random(server_list,dao_name,final_server)<0)
		{return -1;}
	}
	else if(final_ch=='L')
	{
		if(inner_filter_highresponse_minsending(server_list,dao_name,final_server)<0)
		{return -1;}
	}
	
	/////////////
	std::map<std::string, ServINFO>::iterator fsi=serv_infos.find(final_server);
	if(fsi==serv_infos.end())
	{
		assert(false);
	}
	out_serv=&(fsi->second);
	out_ser_name=final_server;
	return 0;
}

int DaoStatManager::inner_filter_help_A(std::vector<std::string>& server_list,std::string& dao_name,int groupid)
{
	std::vector<std::string> result_list;
	for(size_t i=0;i<server_list.size();++i)
	{
		std::map<std::string, ServINFO>::iterator si=serv_infos.find(server_list[i]);
		if(si==serv_infos.end())
		{
			assert(false);
		}
		if(si->second.groupid==groupid)
		{
			result_list.push_back(server_list[i]);
		}
	}
	if(!result_list.empty())
	{
		server_list.swap(result_list);
	}
	return 0;
}
int DaoStatManager::inner_filter_help_B(std::vector<std::string>& server_list,std::string& dao_name)
{
	std::vector<std::string> result_list;
	for(size_t i=0;i<server_list.size();++i)
	{
		std::map<std::string, ServINFO>::iterator si=serv_infos.find(server_list[i]);
		if(si==serv_infos.end())
		{
			assert(false);
		}
		std::map<std::string,DaoINFO>::iterator temdi=si->second.dao_infos.find(dao_name);
		if(temdi==si->second.dao_infos.end())
		{
			assert(false);
		}
		if(temdi->second.dao_type==1)
		{
			result_list.push_back(server_list[i]);
		}
	}
	if(!result_list.empty())
	{
		server_list.swap(result_list);
	}
	return 0;
}
int DaoStatManager::inner_filter_help_C(std::vector<std::string>& server_list,std::string& dao_name)
{
	std::vector<std::string> result_list;
	for(size_t i=0;i<server_list.size();++i)
	{
		std::map<std::string, ServINFO>::iterator si=serv_infos.find(server_list[i]);
		if(si==serv_infos.end())
		{
			assert(false);
		}
		std::map<std::string,DaoINFO>::iterator temdi=si->second.dao_infos.find(dao_name);
		if(temdi==si->second.dao_infos.end())
		{
			assert(false);
		}
		if(temdi->second.dao_type==2)
		{
			result_list.push_back(server_list[i]);
		}
	}
	if(!result_list.empty())
	{
		server_list.swap(result_list);
	}
	return 0;
}

int DaoStatManager::inner_filter_highresponse_minsending(std::vector<std::string>& server_list,std::string& dao_name,std::string& out_serv)
{
	if(server_list.empty())
	{
		out_serv.clear();
		return -1;
	}

	bool bfound=false;
	uint32_t min_sending=0;
	////////////
	//std::vector<std::string> out_srv_list;
	//std::vector<long double> srv_wei;
	//long double sumwe=0;
	////////////
	for(size_t i=0;i<server_list.size();++i)
	{
		////get info, filter out by level
		std::map<std::string, ServINFO>::iterator si=serv_infos.find(server_list[i]);
		if(si==serv_infos.end())
		{assert(false);}
		std::map<std::string,DaoINFO>::iterator temdi=si->second.dao_infos.find(dao_name);
		if(temdi==si->second.dao_infos.end())
		{assert(false);}
		IPPORTYPE tem_get_ipp;
		DaoConnectionManager::IPINFO ipp_result_info;
		if(gl_daoconnectionmanager->select_best_connection(si->second.ipports,tem_get_ipp,&ipp_result_info)<0)
		{
			continue;
		}
		///////filter out by level
		if(temdi->second.tick_remain==0)
		{
			continue;
		}
		if(ipp_result_info.bad_level>2)
		{
			continue;
		}
		//////push into
		//out_srv_list.push_back(server_list[i]);
		//long double weig=(long double)1.0/(((long double)temdi->second.sending_request)+0.01);
		//srv_wei.push_back(weig);
		//sumwe+=weig;
		if(bfound)
		{
			if(temdi->second.sending_request<min_sending)
			{
				min_sending=temdi->second.sending_request;
				out_serv=server_list[i];
			}
		}
		else
		{
			min_sending=temdi->second.sending_request;
			bfound=true;
			out_serv=server_list[i];
		}
	}
	if(!bfound)
	{
		return -1;
	}

	//if(out_srv_list.size()==0)
	//{
	//	return -1;
	//}
	//long double gra=rand()%100000;
	//gra+=0.5;
	//gra/=100000.0;
	//long double cur_fr=0;
	//size_t hit_cnttt=0;
	//for(size_t t=0;t<srv_wei.size();++t)
	//{
	//	cur_fr+=(srv_wei[t]/sumwe);
	//	if(cur_fr>=gra)
	//	{
	//		hit_cnttt=t;
	//		break;
	//	}
	//}
	//out_serv=out_srv_list[hit_cnttt];
	return 0;
}
int DaoStatManager::inner_filter_highresponse_random(std::vector<std::string>& server_list,std::string& dao_name,std::string& out_serv)
{
	if(server_list.empty())
	{
		out_serv.clear();
		return -1;
	}
	////////////
	std::vector<std::string> out_srv_list;
	////////////
	for(size_t i=0;i<server_list.size();++i)
	{
		////get info, filter out by level
		std::map<std::string, ServINFO>::iterator si=serv_infos.find(server_list[i]);
		if(si==serv_infos.end())
		{assert(false);}
		std::map<std::string,DaoINFO>::iterator temdi=si->second.dao_infos.find(dao_name);
		if(temdi==si->second.dao_infos.end())
		{assert(false);}
		IPPORTYPE tem_get_ipp;
		DaoConnectionManager::IPINFO ipp_result_info;
		if(gl_daoconnectionmanager->select_best_connection(si->second.ipports,tem_get_ipp,&ipp_result_info)<0)
		{
			continue;
		}
		///////filter out by level
		if(temdi->second.tick_remain==0)
		{
			continue;
		}
		if(ipp_result_info.bad_level>2)
		{
			continue;
		}
		//////push into
		out_srv_list.push_back(server_list[i]);
	}

	if(out_srv_list.size()==0)
	{
		return -1;
	}
	size_t hit_cnttt=rand()%out_srv_list.size();
	out_serv=out_srv_list[hit_cnttt];
	return 0;
}
struct SortByRespTimeOrder
{
	SortByRespTimeOrder(std::vector<double>& u):v(u){}
	~SortByRespTimeOrder(){}
	bool operator()(int a,int b)
	{
		return v[a]<v[b];
	}
	std::vector<double>& v;
};
int DaoStatManager::inner_filter_highresponse_lmspredict(std::vector<std::string>& server_list,std::string& dao_name,std::string& out_serv)
{
	if(server_list.empty())
	{
		out_serv.clear();
		return -1;
	}
	////
	std::vector<long double> org_onway_wei;
	long double sumonway_wei=0;

	//////base_resp=ping_resp + ( load + onway ) * lms_k + lms_b 
	//////rsp_k   =lms_k;
	std::vector<std::string> out_srv_list;
	std::vector<double> out_base_resp;
	std::vector<double> out_k;
	std::vector<int> out_srt_idx;
	////////////
	for(size_t i=0;i<server_list.size();++i)
	{
		////get info, filter out by level
		std::map<std::string, ServINFO>::iterator si=serv_infos.find(server_list[i]);
		if(si==serv_infos.end())
		{assert(false);}
		std::map<std::string,DaoINFO>::iterator temdi=si->second.dao_infos.find(dao_name);
		if(temdi==si->second.dao_infos.end())
		{assert(false);}
		IPPORTYPE tem_get_ipp;
		DaoConnectionManager::IPINFO ipp_result_info;
		if(gl_daoconnectionmanager->select_best_connection(si->second.ipports,tem_get_ipp,&ipp_result_info)<0)
		{
			continue;
		}
		///////filter out by level
		if(temdi->second.tick_remain==0)
		{
			continue;
		}
		if(ipp_result_info.bad_level>2)
		{
			continue;
		}
		//////push into
		out_srv_list.push_back(server_list[i]);
		float rank_resp=ipp_result_info.resptime+ipp_result_info.bad_level*2000000.0;
		if(rank_resp<=0.01)
		{
			rank_resp=0.01;
		}
		double onway_wei=temdi->second.sending_request+2;
		org_onway_wei.push_back(onway_wei);
		sumonway_wei+=onway_wei;

		double th_base_resp=rank_resp + ( temdi->second.curren_load + temdi->second.sending_request )
			* temdi->second.m_lms_k + temdi->second.m_lms_b ;
		out_srt_idx.push_back(out_base_resp.size());
		out_base_resp.push_back(th_base_resp);
		out_k.push_back(temdi->second.m_lms_k);
	}

	if(out_srv_list.size()==0)
	{
		return -1;
	}
	////////////
	///get the last sum send
	uint32_t sumcnt=0;
	gl_traffic_statstic->get_dao_last_sendcnt(dao_name,sumcnt);
	if(sumcnt<10)
	{
		sumcnt=10;
	}

	////sort by resp
	SortByRespTimeOrder srter(out_base_resp);
	std::sort(out_srt_idx.begin(),out_srt_idx.end(),srter);
	/////
	uint32_t final_i=0;
	double sumOdK=0.0;
	double sumremain_cnt=sumcnt;
	double final_resp=0.0;
	for(uint32_t i=0;i<out_srt_idx.size();++i)
	{
		sumOdK+=(1.0/out_k[out_srt_idx[i] ]);
		if((i+1)==out_srt_idx.size())
		{
			final_i=i;
			final_resp=out_base_resp[out_srt_idx[i] ]
				+ sumremain_cnt/sumOdK;
		}
		else
		{
			double this_can_eat=sumOdK * 
				(out_base_resp[out_srt_idx[i+1] ]-out_base_resp[out_srt_idx[i] ]);
			if(this_can_eat<sumremain_cnt)
			{
				sumremain_cnt-=this_can_eat;
			}
			else
			{
				final_i=i;
				final_resp=out_base_resp[out_srt_idx[i] ]
					+ sumremain_cnt/sumOdK;
				break;
			}
		}
	}

	/////////gen distribute and rand
	std::vector<long double> srv_wei;
	long double sumwe=0;
	///////
	std::vector<long double> srv_old_wei;
	long double old_sumwe=0;
	for(uint32_t g=0;g<=final_i;++g)
	{
		long double current_weight=
			out_k[out_srt_idx[g] ]*(final_resp-out_base_resp[out_srt_idx[g] ]);
		srv_wei.push_back(current_weight);
		sumwe+=current_weight;

		uint32_t last_sendc=0;
		gl_traffic_statstic->get_dao_server_last_sendcnt(dao_name
			,out_srv_list[out_srt_idx[g] ],last_sendc );
		long double owwww=last_sendc+0.1;
		srv_old_wei.push_back(owwww);
		old_sumwe+=owwww;
	}
	long double averw=sumwe/srv_wei.size();
	averw/=1000.0;
	for(uint32_t g=final_i+1;g<out_srt_idx.size();++g)
	{
		srv_wei.push_back(averw);
		sumwe+=averw;

		uint32_t last_sendc=0;
		gl_traffic_statstic->get_dao_server_last_sendcnt(dao_name
			,out_srv_list[out_srt_idx[g] ],last_sendc );
		long double owwww=last_sendc+0.1;
		srv_old_wei.push_back(owwww);
		old_sumwe+=owwww;
	}

	/////
	std::vector<long double> final_wei;
	long double finalsumwei=0.0;
	for(size_t t=0;t<srv_wei.size();++t)
	{
		long double merge_wei=(0.2*(srv_wei[t]/sumwe) 
			+ 0.8*(srv_old_wei[t]/old_sumwe));
		long double diff_wei=merge_wei/
			(org_onway_wei[out_srt_idx[t] ]/sumonway_wei);
		diff_wei=diff_wei*diff_wei;

		final_wei.push_back(diff_wei);
		finalsumwei+=diff_wei;
	}
	size_t hit_cnttt=random_vector_pos(final_wei,finalsumwei);

	out_serv=out_srv_list[ out_srt_idx[hit_cnttt] ];
	return 0;
}
uint32_t DaoStatManager::random_vector_pos(std::vector<long double>& vec,long double sum)
{
	long double gra=rand()%10000000;
	gra+=0.5;
	gra/=((long double)10000000.0);
	long double cur_fr=0;
	uint32_t hit_cnttt=0;
	for(size_t t=0;t<vec.size();++t)
	{
		cur_fr+=(vec[t]/sum);
		if(cur_fr>=gra)
		{
			hit_cnttt=t;
			break;
		}
	}
	return hit_cnttt;
}
int DaoStatManager::inner_filter_help_E(std::vector<std::string>& server_list,std::string& dao_name,std::string& hashid)
{
	std::set<int> groupid_mask;
	std::vector<std::string> result_list;
	///////////////////
	
	for(size_t i=0;i<server_list.size();++i)
	{
		std::map<std::string, ServINFO>::iterator si=serv_infos.find(server_list[i]);
		if(si==serv_infos.end())
		{
			assert(false);
		}
		groupid_mask.insert(si->second.groupid);
	}
	if(groupid_mask.empty())
	{
		server_list.swap(result_list);
		return 0;
	}
	int groupid=0;
	if(gl_hash_ring->get_groupid(hashid,&groupid_mask,groupid)!=0)
	{
		server_list.swap(result_list);
		return 0;
	}

	for(size_t i=0;i<server_list.size();++i)
	{
		std::map<std::string, ServINFO>::iterator si=serv_infos.find(server_list[i]);
		if(si==serv_infos.end())
		{
			assert(false);
		}
		if(si->second.groupid==groupid)
		{
			result_list.push_back(server_list[i]);
		}
	}
	if(!result_list.empty())
	{
		server_list.swap(result_list);
	}
	return 0;
}
int DaoStatManager::inner_filter_help_U(std::vector<std::string>& server_list,std::string& dao_name,int usability_level)
{
	if(usability_filter(server_list,dao_name,usability_level)<0)
	{
		std::vector<std::string>().swap(server_list);
		return -1;
	}
	return 0;
}
int DaoStatManager::inner_filter_help_F(std::vector<std::string>& server_list,std::string& dao_name,std::set<int>& groupids)
{
	std::vector<std::string> result_list;
	for(size_t i=0;i<server_list.size();++i)
	{
		std::map<std::string, ServINFO>::iterator si=serv_infos.find(server_list[i]);
		if(si==serv_infos.end())
		{
			assert(false);
		}
		std::set<int>::iterator fg=groupids.find(si->second.groupid);
		if(fg!=groupids.end())
		{
			result_list.push_back(server_list[i]);
		}
	}
	server_list.swap(result_list);
	return 0;
}

struct UseInfo
{
	std::string server_id;
	uint32_t load;
	int bad_level;
	float resp_time;
	////
	int rank_sum;
};

struct UsabilitySortLoadOrder
{
	UsabilitySortLoadOrder(std::vector<UseInfo>& u):v(u){}
	~UsabilitySortLoadOrder(){}
	bool operator()(int a,int b)
	{
		return v[a].load<v[b].load;
	}
	std::vector<UseInfo>& v;
};
struct UsabilitySortResptimeOrder
{
	UsabilitySortResptimeOrder(std::vector<UseInfo>& u):v(u){}
	~UsabilitySortResptimeOrder(){}
	bool operator()(int a,int b)
	{
		if(v[a].bad_level<v[b].bad_level)
		{
			return true;
		}
		if(v[a].bad_level>v[b].bad_level)
		{
			return false;
		}
		return v[a].resp_time<v[b].resp_time;
	}
	std::vector<UseInfo>& v;
};
struct UsabilitySortRankOrder
{
	UsabilitySortRankOrder(std::vector<UseInfo>& u):v(u){}
	~UsabilitySortRankOrder(){}
	bool operator()(int a,int b)
	{
		return v[a].rank_sum<v[b].rank_sum;
	}
	std::vector<UseInfo>& v;
};

////basic usability, level==0
////good usability, level==1
////excellent usability, level==2
int DaoStatManager::usability_filter(std::vector<std::string>& server_list,std::string& dao_name,int level)
{
	std::vector<std::string> result_server_list;
	for(size_t i=0;i<server_list.size();++i)
	{
		////get info, filter out by level
		std::map<std::string, ServINFO>::iterator si=serv_infos.find(server_list[i]);
		if(si==serv_infos.end())
		{assert(false);}
		std::map<std::string,DaoINFO>::iterator temdi=si->second.dao_infos.find(dao_name);
		if(temdi==si->second.dao_infos.end())
		{assert(false);}
		IPPORTYPE tem_get_ipp;
		DaoConnectionManager::IPINFO ipp_result_info;
		if(gl_daoconnectionmanager->select_best_connection(si->second.ipports,tem_get_ipp,&ipp_result_info)<0)
		{
			continue;
		}
		///////filter out by level
		if(level==1)
		{
			////good usable
			if(temdi->second.tick_remain<=10)
			{
				continue;
			}
			if(ipp_result_info.bad_level>1)
			{
				continue;
			}
			if(ipp_result_info.resptime>(1000*1000))
			{
				continue;
			}
		}
		else if(level==2)
		{
			////excellent usable
			if(temdi->second.tick_remain<=100)
			{
				continue;
			}
			if(ipp_result_info.bad_level>0)
			{
				continue;
			}
			if(ipp_result_info.resptime>(300*1000))
			{
				continue;
			}
		}
		else
		{
			////basic usable
			if(temdi->second.tick_remain==0)
			{
				continue;
			}
			if(ipp_result_info.bad_level>2)
			{
				continue;
			}
		}
		//////push into
		result_server_list.push_back(server_list[i]);
	}
	if(result_server_list.empty())
	{
		return -1;
	}
	result_server_list.swap(server_list);
	return 0;
}



void DaoStatManager::debug_show(std::stringstream& ss)
{
	std::map<std::string, ServINFO>::iterator i=serv_infos.begin();
	for(;i!=serv_infos.end();++i)
	{
		ss <<"sname:"<<i->first ;
		ss <<" groupid:" <<i->second.groupid;
		ss <<" last_resp_time:" << i->second.last_respping_time ;
		ss <<" dao_size:" <<i->second.dao_infos.size();
		ss <<std::endl;
		std::map<std::string,DaoINFO>::iterator bb;
		std::map<std::string,DaoINFO>& daomap=i->second.dao_infos;
		for(bb=daomap.begin();bb!=daomap.end();++bb)
		{
			ss <<"-- daoname:" <<bb->first ;
			ss <<" dao_type:" <<bb->second.dao_type;
			ss <<" load:" <<bb->second.curren_load;
			ss <<" free:" <<bb->second.tick_remain;
			ss <<" onway:"<<bb->second.sending_request;
			ss <<" lmsk:" <<bb->second.m_lms_k;
			ss <<" lmsb:" <<bb->second.m_lms_b;
			ss <<" extent:" <<bb->second.dao_extent_info;
			ss << std::endl;
		}

	}
}

//////////
void DaoStatManager::predict_send_one(std::string& server_name,ServINFO* pserv,std::string& dao_name)
{
	std::map<std::string,DaoINFO>::iterator b=pserv->dao_infos.find(dao_name);
	if(b==pserv->dao_infos.end())
	{
		return;
	}
	//b->second.curren_load++;
	if(b->second.tick_remain>0)
	{
		b->second.tick_remain--;
	}

	///////immediate require free
	if(pserv->b_immreq_stat==0)
	{
		int ths=b->second.tick_remain_last_poll/2;
		if(b->second.tick_remain<ths&&ths>=30)
		{
			if(b->second.tick_remain==0||
				pserv->last_respping_time!=get_current_time_S())
			{
				file_gl_send_poll_packet(server_name,pserv);
				pserv->b_immreq_stat=1;
			}
		}
	}

}

void DaoStatManager::start_send_one(ServINFO* pserv,std::string& dao_name)
{
	std::map<std::string,DaoINFO>::iterator b=pserv->dao_infos.find(dao_name);
	if(b==pserv->dao_infos.end())
	{
		return;
	}
	b->second.sending_request++;
}
void DaoStatManager::end_recv_one(std::string& server_name,std::string& dao_name)
{
	std::map<std::string, ServINFO>::iterator psr=serv_infos.find(server_name);
	/////////////
	if(psr==serv_infos.end())
	{
		return;
	}
	std::map<std::string,DaoINFO>::iterator b=psr->second.dao_infos.find(dao_name);
	if(b==psr->second.dao_infos.end())
	{
		return;
	}
	if(b->second.sending_request>0)
	{
		b->second.sending_request--;
	}
}
int DaoStatManager::learning_predict_lms(const std::string& server_name,const std::string& dao_name,double actual_resp,uint32_t actual_send_cnt,double& predict_resp)
{
	//equation:
	//ping_resp + ( load + actual_send/2 ) * lms_k + lms_b = actual_resp;

	///find the server and dao
	std::map<std::string, ServINFO>::iterator psr=serv_infos.find(server_name);
	/////////////
	if(psr==serv_infos.end())
	{
		return -1;
	}
	std::map<std::string,DaoINFO>::iterator pdao=psr->second.dao_infos.find(dao_name);
	if(pdao==psr->second.dao_infos.end())
	{
		return -1;
	}
	////////////
	IPPORTYPE tem_get_ipp;
	DaoConnectionManager::IPINFO ipp_result_info;
	if(gl_daoconnectionmanager->select_best_connection(psr->second.ipports
		,tem_get_ipp,&ipp_result_info)<0)
	{
		return -1;
	}
	if(ipp_result_info.bad_level!=0)
	{
		return -1;
	}
	float rank_resp=ipp_result_info.resptime;
	if(rank_resp<=0.0001)
	{
		rank_resp=0.0001;
	}
	////////////////////CoffK
	//ping_resp + ( load + actual_send/2 ) * lms_k + lms_b = actual_resp;
	double CoffK=(double)(pdao->second.curren_load+actual_send_cnt);//+actual_send_cnt/2
	predict_resp=rank_resp+(CoffK*pdao->second.m_lms_k+pdao->second.m_lms_b);

	return 0;
}
int DaoStatManager::learning_lms(const std::string& server_name,const std::string& dao_name,double actual_resp,uint32_t actual_send_cnt,double& predict_resp)
{
	//equation:
	//ping_resp + ( load + actual_send/2 ) * lms_k + lms_b = actual_resp;

	if(actual_send_cnt<1)
	{
		return -1;
	}

	///find the server and dao
	std::map<std::string, ServINFO>::iterator psr=serv_infos.find(server_name);
	/////////////
	if(psr==serv_infos.end())
	{
		return -1;
	}
	std::map<std::string,DaoINFO>::iterator pdao=psr->second.dao_infos.find(dao_name);
	if(pdao==psr->second.dao_infos.end())
	{
		return -1;
	}
	////////////
	IPPORTYPE tem_get_ipp;
	DaoConnectionManager::IPINFO ipp_result_info;
	if(gl_daoconnectionmanager->select_best_connection(psr->second.ipports
			,tem_get_ipp,&ipp_result_info)<0)
	{
		return -1;
	}
	if(ipp_result_info.bad_level!=0)
	{
		return -1;
	}
	float rank_resp=ipp_result_info.resptime;
	if(rank_resp<=0.0001)
	{
		rank_resp=0.0001;
	}
	////////////////////CoffK
	//ping_resp + ( load + actual_send/2 ) * lms_k + lms_b = actual_resp;
	double CoffK=(double)(pdao->second.curren_load+actual_send_cnt);//+actual_send_cnt/2
	predict_resp=rank_resp+(CoffK*pdao->second.m_lms_k+pdao->second.m_lms_b);
	double diff_resp=actual_resp-predict_resp;
	///counter float overflow
	double CoffB=1.0;
	CoffK/=300.0;
	if(CoffK>1.0)
	{
		diff_resp/=CoffK;
		CoffB/=CoffK;
		CoffK=1.0;
	}
	///CoffK * dk + CoffB * db=diff_resp;
	double base_div=CoffK*CoffK+CoffB*CoffB;

	double step=(gl_lms_learn_rate*(diff_resp/base_div));

	CoffK/=300.0;
	pdao->second.m_lms_k+=(CoffK*step);
	if(pdao->second.m_lms_k<0.00001)
	{
		pdao->second.m_lms_k=0.00001;
	}
	else if(pdao->second.m_lms_k>1000)
	{
		pdao->second.m_lms_k=1000;
	}
	pdao->second.m_lms_b+=(CoffB*step);
	if(pdao->second.m_lms_b<1.0)
	{
		pdao->second.m_lms_b=1.0;
	}
	else if(pdao->second.m_lms_b>(10000000.0))
	{
		pdao->second.m_lms_b=(10000000.0);
	}
	return 0;
}
void DaoStatManager::learning_lms_shrink()
{
	std::map<std::string, ServINFO>::iterator i=serv_infos.begin();
	for(;i!=serv_infos.end();++i)
	{
		std::map<std::string,DaoINFO>::iterator bb;
		std::map<std::string,DaoINFO>& daomap=i->second.dao_infos;
		for(bb=daomap.begin();bb!=daomap.end();++bb)
		{
			bb->second.m_lms_k*=0.9999;
			bb->second.m_lms_b*=0.9999;
			if(bb->second.m_lms_k<0.00001)
			{
				bb->second.m_lms_k=0.00001;
			}
			else if(bb->second.m_lms_k>1000)
			{
				bb->second.m_lms_k=1000;
			}
			if(bb->second.m_lms_b<1.0)
			{
				bb->second.m_lms_b=1.0;
			}
			else if(bb->second.m_lms_b>(10000000.0))
			{
				bb->second.m_lms_b=(10000000.0);
			}
		}

	}
}

