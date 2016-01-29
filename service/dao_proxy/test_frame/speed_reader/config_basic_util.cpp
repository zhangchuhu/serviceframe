#include "config_basic_util.h"
#include <vector>
#include <string>
#include <map>
#include <set>
#include <fstream>
#include "util.h"
#include "server_common/seda/common/any_cast.h"

using namespace std;

GLConf gl_global_conf_mp;

void GLConf::init()
{
	gl_pglobal_conf->get_all_dao(outdao);
	gl_pglobal_conf->get_all_proxy(outproxy);
	///////////
	std::map<std::string, PxGlobalConf::PrcAdd>::iterator it;
	for(it=outdao.begin();it!=outdao.end();++it)
	{
		string full_ippss;
		parseVecIPP2Str(it->second.ipports,full_ippss);
		string ky;
		for(size_t yu=0;yu<it->second.ipports.size();++yu)
		{
			string tem;
			JLexical::Cast(it->second.ipports[yu].second,tem);
			ky=it->second.ipports[yu].first+":"+tem;
			m_ipp_sn[ky]=it->first;
			m_ipp_multi_ipps[ky]=full_ippss;
		}
	}


	///////////////addition fill m_groupid_inf
	m_groupid_inf.clear();
	for(it=outproxy.begin();it!=outproxy.end();++it)
	{
		string full_ippss;
		parseVecIPP2Str(it->second.ipports,full_ippss);
		string ky;
		for(size_t yu=0;yu<it->second.ipports.size();++yu)
		{
			string tem;
			JLexical::Cast(it->second.ipports[yu].second,tem);
			ky=it->second.ipports[yu].first+":"+tem;
			m_ipp_sn[ky]=it->first;
			m_ipp_multi_ipps[ky]=full_ippss;

			///addition fill m_groupid_inf
			m_groupid_inf[it->second.groupid].push_back(ky);
		}
	}
}
bool GLConf::get_proxyips_by_group(int group_id,std::vector<std::string>& ipps)
{
	std::map<int, std::vector<std::string> >::iterator itr
		=m_groupid_inf.find(group_id);
	if(itr!=m_groupid_inf.end())
	{
		ipps=itr->second;
		return true;
	}
	return false;
}
bool GLConf::get_proxy_by_oneip(string& ipp,int& group_id,string& ipps)
{
	std::map<string,string>::iterator itr=m_ipp_sn.find(ipp);
	if(itr==m_ipp_sn.end())
	{
		return false;
	}
	std::map<std::string, PxGlobalConf::PrcAdd>::iterator iv=
		outproxy.find(itr->second);
	if(iv==outproxy.end())
	{
		return false;
	}
	group_id=iv->second.groupid;
	parseVecIPP2Str(iv->second.ipports,ipps);
	return true;
}
bool GLConf::getdao_by_sname(string& sname,string& ipp,int& group_id)
{
	std::map<std::string, PxGlobalConf::PrcAdd>::iterator iv=
		outdao.find(sname);
	if(iv==outdao.end())
	{
		return false;
	}
	group_id=iv->second.groupid;
	parseVecIPP2Str(iv->second.ipports,ipp);
	return true;
}

