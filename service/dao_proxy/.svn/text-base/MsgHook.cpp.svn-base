#include "MsgHook.h"
#include "core/sox/logger.h"
#include "server_common/config/tinyxml.h"
#include "server_common/seda/common/any_cast.h"
#include "GlobalCommon.h"

using namespace server::dao_proxy;



MsgHook::MsgHook()
{
	b_active=false;
}
MsgHook::~MsgHook()
{

}
void MsgHook::set_hook(uint64_t req_id,uint64_t connectid,std::string* app_name,std::string* dao_name,int sperten,int chargetimes)
{
	b_active=true;
	//////////////
	if(app_name==0)
	{
		m_app_name="";
	}
	else
	{
		m_app_name=*app_name;
	}
	if(dao_name==0)
	{
		m_dao_name="";
	}
	else
	{
		m_dao_name=*dao_name;
	}
	m_cur=sperten;
	m_max=sperten;
	m_cnt=chargetimes;
	m_cid=connectid;
	m_req_id=req_id;
	if(m_cnt>100)
	{
		m_cnt=100;
	}
	if(m_cnt<=0)
	{
		b_active=false;
	}
}
bool MsgHook::is_hooked(std::string* app_name,std::string* dao_name)
{
	if(!b_active)
	{
		return false;
	}
	if(m_cur<=0)
	{
		return false;
	}
	m_cur--;
	if(!m_app_name.empty())
	{
		if(app_name==0)
		{
			return false;
		}
		if(m_app_name!=(*app_name))
		{
			return false;
		}
	}
	if(!m_dao_name.empty())
	{
		if(dao_name==0)
		{
			return false;
		}
		if(m_dao_name!=(*dao_name))
		{
			return false;
		}
	}
	return true;
}
void MsgHook::clear_hook()
{
	b_active=false;
}

//return 0: normal  1: just finish timer
int MsgHook::timer_pool()
{
	if(b_active)
	{
		m_cur=m_max;
		m_cnt--;
		if(m_cnt<=0)
		{
			b_active=false;
			return 1;
		}
	}
	return 0;
}

