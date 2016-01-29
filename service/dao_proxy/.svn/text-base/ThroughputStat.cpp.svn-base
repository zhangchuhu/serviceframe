#include "ThroughputStat.h"
#include "core/sox/logger.h"
#include "server_common/config/tinyxml.h"
#include "server_common/seda/common/any_cast.h"
#include "GlobalCommon.h"
#include "server_common/protocol/pdbnode.h"

using namespace server::dao_proxy;


ThroughputStatTimer::ThroughputStatTimer()
{
	timer_id=0;
}
ThroughputStatTimer::~ThroughputStatTimer()
{
}

void ThroughputStatTimer::handle_timeout(void* param)
{
	gl_throughput_statstic->init_load_conf();
	gl_throughput_statstic->timer_through_block();

	if(gl_msghook->timer_pool()==1)
	{
		////send commit package
		server::db_node::DBAdminResp resp;
		resp.req_id=gl_msghook->m_req_id;
		resp.option=111;
		resp.result=0;
		Sender sdr(server::db_node::DBAdminResp::uri, resp);
		sdr.setResCode(RES_SUCCESS);
		sdr.endPack();
		/////////////send to that cid
		vector<char> send;
		send.assign(sdr.header(),sdr.header()+sdr.bodySize()+sdr.headerSize());

		IConn* pcon = gl_service_connection_manager->getConnectById((uint32_t)gl_msghook->m_cid);
		if(pcon)
		{
			try{
			pcon->sendBin( &send[0] ,send.size()  , -1);
			}catch(std::exception &ex){
			pcon->setEnable(false);
			log(Error, "send error!:%s", ex.what());
			}
		}
	}
}
void ThroughputStatTimer::start_timer(int seconds)
{
	assert(seconds>0);
	if(timer_id!=0)
	{
		g_pTimerContainer->cancel(timer_id);
		timer_id=0;
	}
	timer_id=g_pTimerContainer->schedule( *this,seconds,seconds);
}
struct MyGetLicenceInfo : public ILicenceNotify
{
	MyGetLicenceInfo(){m_bGlobalLicence=false;m_global_licence=0;}
	~MyGetLicenceInfo(){}
	virtual void licence_notify(const std::map<std::string,std::string>& licence_key)
	{
		if(licence_key.empty())
		{
			m_bGlobalLicence=true;
			m_global_licence=1;
			m_app_licence.clear();
			m_dao_licence.clear();
			m_appdao_licence.clear();
			return;
		}
		if(m_bGlobalLicence)
		{
			return;
		}
		////////////////
		std::map<std::string,std::string>::const_iterator i;
		i=licence_key.find("G");
		if(i!=licence_key.end())
		{
			int tmpget_groupid=0;
			JLexical::Cast(i->second,tmpget_groupid);
			if(tmpget_groupid!=gl_myconf->groupid)
			{
				return;
			}
		}
		std::map<std::string,std::string>::const_iterator iapp=licence_key.find("A");
		std::map<std::string,std::string>::const_iterator idao=licence_key.find("D");
		if(iapp!=licence_key.end())
		{
			if(idao!=licence_key.end())
			{
				if(m_app_licence.find(iapp->second)!=m_app_licence.end())
				{return;}
				if(m_dao_licence.find(idao->second)!=m_dao_licence.end())
				{return;}
				std::pair<std::string,std::string> tky(iapp->second,idao->second);
				m_appdao_licence[tky]=1;
			}
			else
			{
				if(m_app_licence.find(iapp->second)!=m_app_licence.end())
				{return;}
				m_app_licence[iapp->second]=1;
			}
		}
		else
		{
			if(idao!=licence_key.end())
			{
				if(m_dao_licence.find(idao->second)!=m_dao_licence.end())
				{return;}
				m_dao_licence[idao->second]=1;
			}
			else
			{
				///error key mean nothing
			}
		}

	}
	std::map<std::string,int> m_app_licence;
	std::map<std::string,int> m_dao_licence;
	std::map<std::pair<std::string,std::string> ,int> m_appdao_licence;
	bool m_bGlobalLicence;
	int m_global_licence;
};
struct MyGetThrottleInfo : public IThrottleNotify
{
	MyGetThrottleInfo(){m_bGlobalTicket=false;}
	~MyGetThrottleInfo(){}
	virtual void throttle_notify(const std::map<std::string,std::string>& locate,int tickets)
	{
		if(locate.empty())
		{
			if(!m_bGlobalTicket)
			{
				m_bGlobalTicket=true;
				m_global_ticket=tickets;
			}
			return;
		}
		///check local
		std::map<std::string,std::string>::const_iterator i;
		i=locate.find("G");
		if(i!=locate.end())
		{
			int tmpget_groupid=0;
			JLexical::Cast(i->second,tmpget_groupid);
			if(tmpget_groupid!=gl_myconf->groupid)
			{
				return;
			}
			/////is local cover mode
			std::map<std::string,std::string>::const_iterator iapp=locate.find("A");
			std::map<std::string,std::string>::const_iterator idao=locate.find("D");
			if(iapp!=locate.end())
			{
				if(idao!=locate.end())
				{
					std::pair<std::string,std::string> t_k(iapp->second,idao->second);
					m_appdao_tickets[t_k]=tickets;
				}
				else
				{
					m_app_tickets[iapp->second]=tickets;
				}
			}
			else
			{
				if(idao!=locate.end())
				{
					m_dao_tickets[idao->second]=tickets;
				}
				else
				{
					m_bGlobalTicket=true;
					m_global_ticket=tickets;
				}
			}
			return;
		}
		else
		{
			////not local insert mode
			std::map<std::string,std::string>::const_iterator iapp=locate.find("A");
			std::map<std::string,std::string>::const_iterator idao=locate.find("D");
			if(iapp!=locate.end())
			{
				if(idao!=locate.end())
				{
					std::pair<std::string,std::string> t_k(iapp->second,idao->second);
					if(m_appdao_tickets.find(t_k)==m_appdao_tickets.end())
					{m_appdao_tickets[t_k]=tickets;}
				}
				else
				{
					if(m_app_tickets.find(iapp->second)==m_app_tickets.end())
					{m_app_tickets[iapp->second]=tickets;}
				}
			}
			else
			{
				if(idao!=locate.end())
				{
					if(m_dao_tickets.find(idao->second)==m_dao_tickets.end())
					{m_dao_tickets[idao->second]=tickets;}
				}
				else
				{
					if(!m_bGlobalTicket)
					{
						m_bGlobalTicket=true;
						m_global_ticket=tickets;
					}
				}
			}
		}
	}
	std::map<std::string,int> m_app_tickets;
	std::map<std::string,int> m_dao_tickets;
	std::map<std::pair<std::string,std::string> ,int> m_appdao_tickets;
	bool m_bGlobalTicket;
	int m_global_ticket;

};


ThroughputStat::ThroughputStat()
{
	m_bGlobalTicket=false;
	m_global_ticket=0;
	/////////////
	m_bGlobalLicence=true;
	m_global_licence=1;
	///////
	m_bGlobalBlock=false;
}
ThroughputStat::~ThroughputStat()
{
}
int ThroughputStat::init()
{
	if(init_load_conf()<0)
	{
		return -1;
	}
	start_timer.start_timer(10);
	return 0;
}
int ThroughputStat::init_load_conf()
{
	MyGetThrottleInfo gi;
	gl_globalconf->getall_throttle_limit(&gi);
	m_bGlobalTicket=gi.m_bGlobalTicket;
	m_global_ticket=gi.m_global_ticket;
	m_app_tickets.swap(gi.m_app_tickets);
	m_dao_tickets.swap(gi.m_dao_tickets);
	m_appdao_tickets.swap(gi.m_appdao_tickets);
	//////////////////////////
	MyGetLicenceInfo gll;
	gl_globalconf->getall_licence(&gll);
	m_app_licence.swap(gll.m_app_licence);
	m_dao_licence.swap(gll.m_dao_licence);
	m_appdao_licence.swap(gll.m_appdao_licence);
	m_bGlobalLicence=gll.m_bGlobalLicence;
	m_global_licence=gll.m_global_licence;
	return 0;
}

void ThroughputStat::timer_through_block()
{
	std::map<std::string,TBlock>::iterator appitr=m_app_block.begin();
	for(;appitr!=m_app_block.end();)
	{
		if(appitr->second.recharge<=1)
		{
			std::map<std::string,TBlock>::iterator td=appitr;
			++appitr;
			m_app_block.erase(td);
		}
		else
		{
			appitr->second.cur=appitr->second.max;
			appitr->second.recharge--;
			++appitr;
		}
	}
	std::map<std::string,TBlock>::iterator daoitr=m_dao_block.begin();
	for(;daoitr!=m_dao_block.end();)
	{
		if(daoitr->second.recharge<=1)
		{
			std::map<std::string,TBlock>::iterator td=daoitr;
			++daoitr;
			m_dao_block.erase(td);
		}
		else
		{
			daoitr->second.cur=daoitr->second.max;
			daoitr->second.recharge--;
			++daoitr;
		}
	}
	//////////////
	std::map<std::pair<std::string,std::string> ,TBlock>::iterator appdaoitr=m_appdao_block.begin();
	for(;appdaoitr!=m_appdao_block.end();)
	{
		if(appdaoitr->second.recharge<=1)
		{
			std::map<std::pair<std::string,std::string> ,TBlock>::iterator td=appdaoitr;
			++appdaoitr;
			m_appdao_block.erase(td);
		}
		else
		{
			appdaoitr->second.cur=appdaoitr->second.max;
			appdaoitr->second.recharge--;
			++appdaoitr;
		}
	}
	///
	if(m_bGlobalBlock)
	{
		if(m_global_block.recharge<=1)
		{
			m_bGlobalBlock=false;
		}
		else
		{
			m_global_block.cur=m_global_block.max;
			m_global_block.recharge--;
		}
	}

}

void ThroughputStat::set_temp_block(std::string* app_name,std::string* dao_name,int sperten,int chargetimes)
{
	TBlock tbk;
	tbk.cur=sperten;
	tbk.max=sperten;
	tbk.recharge=chargetimes;
	if(tbk.recharge>30)
	{
		tbk.recharge=30;
	}
	if(app_name!=0)
	{
		if(dao_name!=0)
		{
			m_appdao_block[std::pair<std::string,std::string>(*app_name,*dao_name) ]
					=tbk;
		}
		else
		{
			m_app_block[*app_name]=tbk;
		}
	}
	else
	{
		if(dao_name!=0)
		{
			m_dao_block[*dao_name]=tbk;
		}
		else
		{
			///global temp block must careful bound
			if(tbk.recharge>=1)
			{
				if(tbk.recharge>10)
				{
					tbk.recharge=10;
				}
				if(tbk.max<1000)
				{
					tbk.max=1000;
				}
				tbk.cur=tbk.max;
				m_bGlobalBlock=true;
				m_global_block=tbk;
			}
			else
			{
				m_bGlobalBlock=false;
			}
		}
	}
}

bool ThroughputStat::check_and_get_block(std::string* app_name,std::string* dao_name)
{
	std::map<std::string,TBlock>::iterator appitr=m_app_block.end();
	std::map<std::string,TBlock>::iterator daoitr=m_dao_block.end();
	std::map<std::pair<std::string,std::string> ,TBlock>::iterator appdaoitr=m_appdao_block.end();

	if(m_bGlobalBlock)
	{
		if(m_global_block.cur<=0)
		{
			return false;
		}
	}

	if(app_name!=0)
	{
		appitr=m_app_block.find(*app_name);
		if(appitr!=m_app_block.end())
		{
			if(appitr->second.cur<=0)
			{
				return false;
			}
		}
	}
	if(dao_name!=0)
	{
		daoitr=m_dao_block.find(*dao_name);
		if(daoitr!=m_dao_block.end())
		{
			if(daoitr->second.cur<=0)
			{
				return false;
			}
		}
	}
	if(app_name!=0&&dao_name!=0)
	{
		appdaoitr=m_appdao_block.find(std::pair<std::string,std::string>(*app_name,*dao_name));
		if(appdaoitr!=m_appdao_block.end())
		{
			if(appdaoitr->second.cur<=0)
			{
				return false;
			}
		}
	}

	////////////////check all ok, then all minus
	if(appitr!=m_app_block.end())
	{
		appitr->second.cur--;
	}
	if(daoitr!=m_dao_block.end())
	{
		daoitr->second.cur--;
	}
	if(appdaoitr!=m_appdao_block.end())
	{
		appdaoitr->second.cur--;
	}
	if(m_bGlobalBlock)
	{
		--m_global_block.cur;
	}
	return true;
}

//return value 0:ok get one ticket 1:ticket sold out 2:no licence 3:temp block
int ThroughputStat::check_and_get_tickets(std::string* app_name,std::string* dao_name)
{
	if(!inner_check_licence(app_name,dao_name))
	{
		///no licence
		return 2;
	}
	if(!check_and_get_block(app_name,dao_name))
	{
		///temp block
		return 3;
	}

	std::map<std::string,int>::iterator appitr=m_app_tickets.end();
	std::map<std::string,int>::iterator daoitr=m_dao_tickets.end();
	std::map<std::pair<std::string,std::string> ,int>::iterator appdaoitr=m_appdao_tickets.end();

	if(m_bGlobalTicket)
	{
		if(m_global_ticket<=0)
		{
			return 1;
		}
	}

	if(app_name!=0)
	{
		appitr=m_app_tickets.find(*app_name);
		if(appitr!=m_app_tickets.end())
		{
			if(appitr->second<=0)
			{
				return 1;
			}
		}
	}
	if(dao_name!=0)
	{
		daoitr=m_dao_tickets.find(*dao_name);
		if(daoitr!=m_dao_tickets.end())
		{
			if(daoitr->second<=0)
			{
				return 1;
			}
		}
	}
	if(app_name!=0&&dao_name!=0)
	{
		appdaoitr=m_appdao_tickets.find(std::pair<std::string,std::string>(*app_name,*dao_name));
		if(appdaoitr!=m_appdao_tickets.end())
		{
			if(appdaoitr->second<=0)
			{
				return 1;
			}
		}
	}

	////////////////check all ok, then all minus
	if(appitr!=m_app_tickets.end())
	{
		appitr->second--;
	}
	if(daoitr!=m_dao_tickets.end())
	{
		daoitr->second--;
	}
	if(appdaoitr!=m_appdao_tickets.end())
	{
		appdaoitr->second--;
	}
	if(m_bGlobalTicket)
	{
		--m_global_ticket;
	}
	return 0;
}

bool ThroughputStat::inner_check_licence(std::string* app_name,std::string* dao_name)
{
	if(m_bGlobalLicence)
	{
		return true;
	}
	if(app_name==0)
	{	return true;
		if(dao_name==0)
		{
			///not hit
		}
		else
		{
			if(m_dao_licence.find(*dao_name)!=m_dao_licence.end())
			{
				return true;
			}
		}
	}
	else
	{
		if(dao_name==0)
		{
			if(m_app_licence.find(*app_name)!=m_app_licence.end())
			{
				return true;
			}
		}
		else
		{
			if(m_app_licence.find(*app_name)!=m_app_licence.end())
			{
				return true;
			}
			if(m_dao_licence.find(*dao_name)!=m_dao_licence.end())
			{
				return true;
			}
			std::pair<std::string,std::string> tky(*app_name ,*dao_name);
			if(m_appdao_licence.find(tky)!=m_appdao_licence.end())
			{
				return true;
			}
		}
	}
	return false;
}

void ThroughputStat::debug_show(std::stringstream& ss)
{
	{
	if(m_bGlobalTicket)
	{
		if(m_global_ticket==0)
		{
			ss <<"global blocking" <<std::endl;
		}
		else
		{
			ss <<"global ticket:" <<m_global_ticket <<std::endl;
		}
	}
	std::map<std::string,int>::iterator i=m_app_tickets.begin();
	for(;i!=m_app_tickets.end();++i)
	{
		if(i->second==0)
		{
			ss <<"app blocking:"<<i->first <<std::endl;
		}
		else
		{
			ss <<"app:"<<i->first <<" ticket:" <<i->second <<std::endl;
		}
	}
	i=m_dao_tickets.begin();
	for(;i!=m_dao_tickets.end();++i)
	{
		if(i->second==0)
		{
			ss <<"dao blocking:"<<i->first <<std::endl;
		}
		else
		{
			ss <<"dao:"<<i->first <<" ticket:" <<i->second <<std::endl;
		}
	}

	std::map<std::pair<std::string,std::string> ,int>::iterator adi=m_appdao_tickets.begin();
	for(;adi!=m_appdao_tickets.end();++adi)
	{
		if(adi->second==0)
		{
			ss <<"adpapo blocking:"<<adi->first.first <<" " <<adi->first.second <<std::endl;
		}
		else
		{
			ss <<"adpapo:"<<adi->first.first <<" " <<adi->first.second <<" ticket:"<<adi->second <<std::endl;
		}
	}
	}

	//////////////////////////////////////////
	{
	if(m_bGlobalBlock)
	{
		if(m_global_block.cur==0)
		{
			ss <<"temp_blocking global max:" <<m_global_block.max << " charge:" <<m_global_block.recharge <<std::endl;
		}
		else
		{
			ss <<"temp_blocking ticket:" <<m_global_block.cur <<" max:" <<m_global_block.max <<" charge:" <<m_global_block.recharge <<std::endl;
		}
	}
	std::map<std::string,TBlock>::iterator i=m_app_block.begin();
	for(;i!=m_app_block.end();++i)
	{
		if(i->second.cur==0)
		{
			ss <<"temp_blocking app:"<<i->first <<" max:" <<i->second.max <<" charge:" <<i->second.recharge <<std::endl;
		}
		else
		{
			ss <<"temp_blocking app:"<<i->first <<" ticket:" <<i->second.cur <<" max:" <<i->second.max <<" charge:" <<i->second.recharge <<std::endl;
		}
	}
	i=m_dao_block.begin();
	for(;i!=m_dao_block.end();++i)
	{
		if(i->second.cur==0)
		{
			ss <<"temp_blocking dao:"<<i->first <<" max:" <<i->second.max <<" charge:" <<i->second.recharge <<std::endl;
		}
		else
		{
			ss <<"temp_blocking dao:"<<i->first <<" ticket:" <<i->second.cur <<" max:" <<i->second.max <<" charge:" <<i->second.recharge <<std::endl;
		}
	}

	std::map<std::pair<std::string,std::string> ,TBlock>::iterator adi=m_appdao_block.begin();
	for(;adi!=m_appdao_block.end();++adi)
	{
		if(adi->second.cur==0)
		{
			ss <<"temp_blocking adpapo:"<<adi->first.first <<" " <<adi->first.second <<" max:" <<adi->second.max <<" charge:" <<adi->second.recharge <<std::endl;
		}
		else
		{
			ss <<"temp_blocking adpapo:"<<adi->first.first <<" " <<adi->first.second <<" ticket:" <<adi->second.cur <<" max:" <<adi->second.max <<" charge:" <<adi->second.recharge <<std::endl;
		}
	}
	
	}
}

