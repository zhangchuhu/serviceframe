#include "dao_connection_manager.h"
#include "core/sox/logger.h"
#include <assert.h>
#include "util.h"
#include "server_common/seda/common/any_cast.h"
#include "GlobalCommon.h"
#include "server_common/seda/syncall/remote_call.hpp"
#include "common/core/sender.h"
#include "common/core/request.h"
#include "server_common/protocol/prouter.h"

using namespace server::dao_proxy;

const float gl_delay_factor=4.0;

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
void tinyaddbadlevel(int& badlevel)
{
	if(badlevel<3)
	{
		++badlevel;
	}
}
void addbadlevel(int& badlevel)
{
	if(badlevel<6)
	{
		++badlevel;
	}
}
void okrespbadlevel(int& badlevel)
{
	if(badlevel>2)
	{
		badlevel=2;
	}
	else if(badlevel>0)
	{
		badlevel--;
	}
}
int badlevel2time(int badlevel)
{
	if(badlevel>=6)
	{
		return 320;
	}
	if(badlevel<=0)
	{
		return 5;
	}
	int ret=5;
	ret <<= badlevel;
	return ret;
}

DaoConnectionTimer::DaoConnectionTimer()
{
	timer_id=0;
}
DaoConnectionTimer::~DaoConnectionTimer()
{
}

void DaoConnectionTimer::handle_timeout(void* param)
{
	bool is_all_bad=true;
	std::map<IPPORTYPE,DaoConnectionManager::IPINFO>::iterator ci=
		gl_daoconnectionmanager->m_ipinfo.begin();
	for(;ci!=gl_daoconnectionmanager->m_ipinfo.end();++ci)
	{
		if(ci->second.bad_level<=2)
		{
			is_all_bad=false;
		}
		ci->second.ping_counter--;
		if(ci->second.ping_counter==0)
		{
			if(ci->second.m_state==1)
			{
				///trigger an time_out event
				addbadlevel(ci->second.bad_level);
			}
			ci->second.m_state=1;

			////////reset counter
			ci->second.ping_counter=badlevel2time(ci->second.bad_level)/5;
			ci->second.last_ping_time=get_current_time_US();

			////send ping, prepare packetage
			::server::router::PProxyDaoPing ppdr;
			ppdr.type=2;
			//////////////////
			std::string ipprtid;
			std::vector<std::pair<std::string,int> > vvvipp;
			vvvipp.resize(1);
			vvvipp[0]=ci->first;
			std::string tem_ping_time;
			JLexical::Cast(ci->second.last_ping_time,tem_ping_time);
			parseVecIPP2Str(vvvipp, ipprtid);
			ppdr.extra_info="2_"+tem_ping_time+"_"+ipprtid;
			///////////////////
			core::Sender sdr;
			sdr.clear();
			sdr.setUri( ::server::router::PProxyDaoPing::uri);
			sdr.setResCode(200);
			sdr.marshall(ppdr);
			sdr.endPack();

			///////////////
			uint64_t connnniddd=0;
			gl_daoconnectionmanager->get_connection(ci->first,connnniddd);
			gl_daoconnectionmanager->get_connectionfactory()->send_pack(connnniddd,sdr);
			/////////////////
		}
	}

	if(is_all_bad)
	{
		std::map<IPPORTYPE,DaoConnectionManager::IPINFO>::iterator ci=
			gl_daoconnectionmanager->m_ipinfo.begin();
		for(;ci!=gl_daoconnectionmanager->m_ipinfo.end();++ci)
		{
			if(ci->second.ping_counter>2)
			{
				ci->second.ping_counter=2;
			}
		}
	}
}
void DaoConnectionTimer::start_timer(int seconds)
{
	assert(seconds>0);
	if(timer_id!=0)
	{
		g_pTimerContainer->cancel(timer_id);
		timer_id=0;
	}
	timer_id=g_pTimerContainer->schedule( *this,seconds,seconds);
}


DaoConnectionManager::DaoConnectionManager()
{
}
DaoConnectionManager::~DaoConnectionManager()
{

}
int DaoConnectionManager::init(core::IClientConnFactory* iccf,core::ILinkHandler* ilh)
{
	m_client_connfactory=iccf;
	m_linkhandler=ilh;
	init_from_conf();
	return 0;
}
int DaoConnectionManager::rc_calltest()
{
	int good_send=0;
	int bad_send=0;

	std::map<IPPORTYPE,DaoConnectionManager::IPINFO>::iterator ci=
		gl_daoconnectionmanager->m_ipinfo.begin();
	for(;ci!=gl_daoconnectionmanager->m_ipinfo.end();++ci)
	{
		seda::remote_call rc;

		IPPORTYPE ipp=ci->first;
		rc.setup(ipp.first.c_str(),ipp.second,1);

		ci->second.last_ping_time=get_current_time_US();

		//////prepare packetage
		::server::router::PProxyDaoPing ppdr;
		ppdr.type=2;
		//////////
		std::string ipprtid;
		std::vector<std::pair<std::string,int> > vvvipp;
		vvvipp.resize(1);
		vvvipp[0]=ci->first;
		std::string tem_ping_time;
		JLexical::Cast(ci->second.last_ping_time,tem_ping_time);
		parseVecIPP2Str(vvvipp, ipprtid);
		ppdr.extra_info="2_"+tem_ping_time+"_"+ipprtid;

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
				continue;
			}
			op_resp.unmarshal(reqest.getPackData());
		}
		catch(sox::PacketError pe)
		{
			bad_send++;
			continue;
		}
		if(op_resp.extra_info!=ppdr.extra_info)
		{
			bad_send++;
			continue;
		}
		//////////start process///////////////////
		uint64_t time_diff=get_current_time_US()-ci->second.last_ping_time;
		if(time_diff<50000000)
		{
			ci->second.resptime=((float)time_diff);
		}
		++good_send;
	}

	if(bad_send>good_send)
	{
		////too many fail
		return -1;
	}
	return 0;
}
int DaoConnectionManager::ping_resp(IPPORTYPE& ipp,uint64_t start_time_us,uint64_t recv_time_us)
{
	std::map<IPPORTYPE,IPINFO>::iterator itrr=m_ipinfo.find(ipp);
	if(itrr==m_ipinfo.end())
	{
		return -1;
	}
	if(itrr->second.last_ping_time!=start_time_us)
	{
		////
		return 1;
	}
	if(itrr->second.m_state!=1)
	{
		///state not match
		return 2;
	}
	uint64_t time_diff=recv_time_us-start_time_us;
	if(time_diff>50000000)
	{
		////time error?
		itrr->second.m_state=0;
		return 3;
	}
	itrr->second.m_state=0;
	okrespbadlevel(itrr->second.bad_level);
	int new_cntr=badlevel2time(itrr->second.bad_level)/5;
	if(new_cntr < itrr->second.ping_counter)
	{
		itrr->second.ping_counter=new_cntr;
	}
	//////////////////
	itrr->second.resptime+=(( ((float)time_diff)- itrr->second.resptime )/gl_delay_factor);

	return 0;
}


int DaoConnectionManager::init_from_conf()
{
	std::map<IPPORTYPE,IPINFO> m_ipinfo_new;
	/////////////
	std::map<std::string, PxGlobalConf::PrcAdd> outdat;
	gl_globalconf->get_all_dao(outdat);
	//////////////
	std::map<std::string, PxGlobalConf::PrcAdd>::iterator n_ip;
	for(n_ip=outdat.begin();n_ip!=outdat.end();++n_ip)
	{
		std::vector<IPPORTYPE>& tem_ipport=n_ip->second.ipports;
		for(size_t v=0;v<tem_ipport.size();++v)
		{
			std::map<IPPORTYPE,IPINFO>::iterator odi=m_ipinfo.find(tem_ipport[v]);
			if(odi!=m_ipinfo.end())
			{
				m_ipinfo_new[tem_ipport[v]]=odi->second;
				m_ipinfo.erase(odi);
			}
			else
			{
				//struct IPINFO
				//uint64_t connectid;
				//float resptime;////us 1000 * 1000
				//int bad_level;///0 1 2 3 4 5 6
				IPINFO ne_inf;
				ne_inf.connectid=0;
				ne_inf.bad_level=1;
				ne_inf.resptime=1000000;
				///////////
				ne_inf.ping_counter=badlevel2time(ne_inf.bad_level)/5;
				ne_inf.last_ping_time=get_current_time_US();
				ne_inf.m_state=0;
				///////////
				m_ipinfo_new[tem_ipport[v]]=ne_inf;
			}
		}
	}
	//////////
	std::map<IPPORTYPE,IPINFO>::iterator o_ip;
	for(o_ip=m_ipinfo.begin();o_ip!=m_ipinfo.end();++o_ip)
	{
		if(o_ip->second.connectid!=0)
		{
			gl_daoconnectionmanager->get_connectionfactory()->disconnect(o_ip->second.connectid);
		}
	}
	//////
	m_ipinfo.swap(m_ipinfo_new);
	return 0;
}
int DaoConnectionManager::get_connection(const IPPORTYPE& ipport,uint64_t& connect_id)
{
	std::map<IPPORTYPE,IPINFO>::iterator v=m_ipinfo.find(ipport);
	bool need_reconnect=true;
	if(v==m_ipinfo.end())
	{
		return -1;
	}
	connect_id=v->second.connectid;
	if(connect_id!=0)
	{
		core::ConnStatus wuliao;
		if(m_client_connfactory->connection_status(connect_id,wuliao)==0)
		{
			need_reconnect=false;
		}
		else
		{
			addbadlevel(v->second.bad_level);
		}
	}
	///////////////
	if(need_reconnect)
	{
		uint64_t connidd=0;
		if(m_client_connfactory->create_connection(ipport.first,ipport.second,m_linkhandler,connidd)<0)
		{
			return -1;
		}
		v->second.connectid=connidd;
		connect_id=connidd;
	}
	/////////
	return 0;
}
int DaoConnectionManager::predict_add_bad_level(std::vector<IPPORTYPE>& ipports)
{
	for(size_t i=0;i<ipports.size();++i)
	{
		std::map<IPPORTYPE,IPINFO>::iterator ipi=m_ipinfo.find(ipports[i]);
		if(ipi==m_ipinfo.end())
		{
			continue;
		}
		addbadlevel(ipi->second.bad_level);
	}
	return 0;
}
int DaoConnectionManager::predict_tiny_add_bad_level(std::vector<IPPORTYPE>& ipports)
{
	for(size_t i=0;i<ipports.size();++i)
	{
		std::map<IPPORTYPE,IPINFO>::iterator ipi=m_ipinfo.find(ipports[i]);
		if(ipi==m_ipinfo.end())
		{
			continue;
		}
		tinyaddbadlevel(ipi->second.bad_level);
	}
	return 0;
}

int DaoConnectionManager::select_best_connection(std::vector<IPPORTYPE>& ipports,IPPORTYPE& ipp,IPINFO* presult_info)
{
	if(ipports.empty())
	{
		return -1;
	}
	int best_level=-1;
	float best_resp;
	ipp=ipports[0];
	for(size_t i=0;i<ipports.size();++i)
	{
		std::map<IPPORTYPE,IPINFO>::iterator ipi=m_ipinfo.find(ipports[i]);
		if(ipi==m_ipinfo.end())
		{
			continue;
		}
		if(best_level==-1
			||ipi->second.bad_level<best_level
			||(ipi->second.bad_level==best_level&&ipi->second.resptime<best_resp)
			)
		{
			best_level=ipi->second.bad_level;
			best_resp=ipi->second.resptime;
		}
	}
	if(best_level==-1)
	{
		return -1;
	}
	if(presult_info!=0)
	{
		presult_info->bad_level=best_level;
		presult_info->resptime=best_resp;
	}
	return 0;
}

core::ILinkHandler* DaoConnectionManager::get_linkhandler()
{
	return m_linkhandler;
}
core::IClientConnFactory* DaoConnectionManager::get_connectionfactory()
{
	return m_client_connfactory;
}

void DaoConnectionManager::debug_show(std::stringstream& ss)
{
	std::map<IPPORTYPE,IPINFO>::iterator i=m_ipinfo.begin();
	for(;i!=m_ipinfo.end();++i)
	{
		ss<<i->first.first <<":" <<i->first.second;
		ss<<" c:" <<i->second.connectid;
		ss<<" resp:" <<i->second.resptime;////us 1000 * 1000
		ss<<" lv:" <<i->second.bad_level;///0 1 2 3 4 5 6
		ss<<" ctr:" <<i->second.ping_counter;
		ss<<" lpt:" <<i->second.last_ping_time;
		ss<<" st:"<<i->second.m_state;//0:ok 1:waiting_resp
		ss<<std::endl;
	}
}

