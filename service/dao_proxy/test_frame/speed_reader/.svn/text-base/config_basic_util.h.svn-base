#ifndef _CONfiG_BASE_UTILL_H_20120530_
#define _CONfiG_BASE_UTILL_H_20120530_
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include "PxGlobalConf.h"

using namespace server::dao_proxy;
using namespace std;

struct GLConf
{
	PxGlobalConf* gl_pglobal_conf;
	void init();
	bool get_proxy_by_oneip(string& ipp,int& group_id,string& ipps);
	bool getdao_by_sname(string& sname,string& ipp,int& group_id);
	bool get_proxyips_by_group(int group_id,std::vector<std::string>& ipps);
	std::map<std::string, PxGlobalConf::PrcAdd> outdao;
	std::map<std::string, PxGlobalConf::PrcAdd> outproxy;
	std::map<string,string> m_ipp_sn;
	std::map<string,string> m_ipp_multi_ipps;

	std::map<int, std::vector<std::string> > m_groupid_inf;
};

extern GLConf gl_global_conf_mp;



#endif	// _CONfiG_BASE_UTILL_H_20120530_

