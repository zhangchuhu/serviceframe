#include "RouteStrategy.h"
#include "core/sox/logger.h"
#include "server_common/config/tinyxml.h"
#include "server_common/seda/common/any_cast.h"
#include "GlobalCommon.h"
#include "util.h"

using namespace server::dao_proxy;

RouteStrategy::RouteStrategy()
{

}
RouteStrategy::~RouteStrategy()
{

}


struct MYGetRouteS: public IRouteNotify
{
	MYGetRouteS(){}
	~MYGetRouteS(){}
	virtual void route_notify(const std::map<std::string,std::string>& locate,const std::map<std::string,std::string>& rule)
	{
		if(locate.empty())
		{
			if(m_default_strategy.empty())
			{
				m_default_strategy=rule;
			}
			return;
		}
		///check local
		std::map<std::string,std::string>::const_iterator i;
		i=locate.find(std::string("G"));
		if(i!=locate.end())
		{
			int tmpget_groupid=0;
			JLexical::Cast(i->second,tmpget_groupid);
			if(tmpget_groupid!=gl_myconf->groupid)
			{
				return;
			}
			/////is local cover mode
			std::map<std::string,std::string>::const_iterator iapp=locate.find(std::string("A"));
			std::map<std::string,std::string>::const_iterator idao=locate.find(std::string("D"));
			if(iapp!=locate.end())
			{
				if(idao!=locate.end())
				{
					std::pair<std::string,std::string> t_k(iapp->second,idao->second);
					m_appdao_route[t_k]=rule;
				}
				else
				{
					m_app_route[iapp->second]=rule;
				}
			}
			else
			{
				if(idao!=locate.end())
				{
					m_dao_route[idao->second]=rule;
				}
				else
				{
					m_default_strategy=rule;
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
					if(m_appdao_route.find(t_k)==m_appdao_route.end())
					{m_appdao_route[t_k]=rule;}
				}
				else
				{
					if(m_app_route.find(iapp->second)==m_app_route.end())
					{m_app_route[iapp->second]=rule;}
				}
			}
			else
			{
				if(idao!=locate.end())
				{
					if(m_dao_route.find(idao->second)==m_dao_route.end())
					{m_dao_route[idao->second]=rule;}
				}
				else
				{
					if(locate.empty())
					{
						if(m_default_strategy.empty())
						{
							m_default_strategy=rule;
						}
						return;
					}
				}
			}
		}
	}
	std::map<std::string,ROUTESTRATEGY_TYPE> m_app_route;
	std::map<std::string,ROUTESTRATEGY_TYPE> m_dao_route;
	std::map<std::pair<std::string,std::string> ,ROUTESTRATEGY_TYPE> m_appdao_route;
	ROUTESTRATEGY_TYPE m_default_strategy;
};

int RouteStrategy::init_load_conf()
{
	MYGetRouteS gi;
	gl_globalconf->getall_route_rule(&gi);
	m_default_strategy=gi.m_default_strategy;
	m_app_route.swap(gi.m_app_route);
	m_dao_route.swap(gi.m_dao_route);
	m_appdao_route.swap(gi.m_appdao_route);
	return 0;
}

//return value 0:ok -1:fail
int RouteStrategy::get_route_str(std::string* app_name,std::string* dao_name,ROUTESTRATEGY_TYPE& filter)
{
	std::map<std::pair<std::string,std::string> ,ROUTESTRATEGY_TYPE>::iterator apd;
	std::map<std::string,ROUTESTRATEGY_TYPE>::iterator api;
	std::map<std::string,ROUTESTRATEGY_TYPE>::iterator di;
	if(app_name!=0&&dao_name!=0)
	{
		std::pair<std::string,std::string> tmpk(*app_name,*dao_name);
		apd=m_appdao_route.find(tmpk);
		if(apd!=m_appdao_route.end())
		{
			filter=apd->second;
			return 0;
		}
	}
	if(dao_name!=0)
	{
		di=m_dao_route.find(*dao_name);
		if(di!=m_dao_route.end())
		{
			filter=di->second;
			return 0;
		}
	}
	if(app_name!=0)
	{
		api=m_app_route.find(*app_name);
		if(api!=m_app_route.end())
		{
			filter=api->second;
			return 0;
		}
	}
	filter=m_default_strategy;
	return 0;
}

void RouteStrategy::debug_show(std::stringstream& ss)
{
	std::string tem;
	tem=parseMapSS2String(m_default_strategy);
	ss <<"global strategy:" <<tem <<std::endl;

	std::map<std::string,ROUTESTRATEGY_TYPE>::iterator i=m_app_route.begin();
	for(;i!=m_app_route.end();++i)
	{
		tem=parseMapSS2String(i->second);
		ss <<"app:"<<i->first <<" strategy:"<<tem <<std::endl;
	}
	i=m_dao_route.begin();
	for(;i!=m_dao_route.end();++i)
	{
		tem=parseMapSS2String(i->second);
		ss <<"dao:"<<i->first <<" strategy:"<<tem <<std::endl;
	}

	std::map<std::pair<std::string,std::string> ,ROUTESTRATEGY_TYPE>::iterator adi
		=m_appdao_route.begin();
	for(;adi!=m_appdao_route.end();++adi)
	{
		tem=parseMapSS2String(adi->second);
		ss <<"adpapo:"<<adi->first.first <<" in " <<adi->first.second
		  <<" strategy:"<<tem <<std::endl;
	}

}

