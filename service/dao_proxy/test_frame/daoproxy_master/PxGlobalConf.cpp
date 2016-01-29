#include "PxGlobalConf.h"

#include "server_common/server-lib2/StringUtil.h"
#include "server_common/config/tinyxml.h"
#include "server_common/seda/common/any_cast.h"
#include "server_common/server-lib/PublicFun.h"
#include "server_common/sq_client/ISQClient.h"
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include "FileUtils.h"
#include "util.h"

using namespace std;

using namespace server::dao_proxy;


class MtxAutoUnlock
{
	seda::linux_mutex& mtx; 
public:
	MtxAutoUnlock(seda::linux_mutex& m):mtx(m)
	{
		mtx.lock();
	}
	~MtxAutoUnlock()
	{
		mtx.unlock();
	}

};

namespace
{
	std::string loc_parseMapSS2String(std::map<std::string,std::string>& original)
	{
		if(original.empty())
		{
			return std::string("empty");
		}
		return parseMapSS2String(original);
	}
	void loc_parseString2MapSS(std::string& str, std::map<std::string,std::string>& value)
	{
		if(str==std::string("empty"))
		{
			value.clear();
			return;
		}
		parseString2MapSS(str,value);
	}

	std::string gl_getStr(TiXmlElement *elm){
		if(elm){
			const char* p=elm->GetText();
			if(p==0)
			{
				return "";
			}
			return std::string(p);
		}else{
			return "";
		}
	}
	int convert_str2ipport(std::string& io,std::vector<std::pair<std::string,int> >& ipports)
	{
		if(io=="empty")
		{
			ipports.clear();
			return 0;
		}
		std::vector<std::string> slice;
		size_t pos=0;
		for(;;)
		{
			size_t r=io.find_first_of('|',pos);
			if(r==std::string::npos)
			{
				if(pos<io.size())
				{
					slice.push_back(io.substr(pos));
				}
				break;
			}
			if(pos<r)
			{
				slice.push_back(io.substr(pos,r-pos));
			}
			pos=r+1;
		}
		////////
		for(size_t i=0;i<slice.size();++i)
		{
			size_t r=slice[i].find_first_of(':',0);
			if(r!=std::string::npos&&r!=0&&(r!=slice[i].size()-1))
			{
				std::string get_ip=slice[i].substr(0,r);
				std::string get_port=slice[i].substr(r+1);
				int real_prt=0;
				real_prt=atoi(get_port.c_str());
				ipports.push_back( std::pair<std::string,int>(get_ip,real_prt) );
			}
		}
		/////////
		return 0;
	}
	int convert_ipport2str(std::vector<std::pair<std::string,int> >& ipports,std::string& io)
	{
		if(ipports.empty())
		{
			io="empty";
			return 0;
		}
		std::stringstream result_ss;
		for(size_t i=0;i<ipports.size();++i)
		{
			std::pair<std::string,int>& itm=ipports[i];
			if(i!=0)
			{
				result_ss<<"|";
			}
			result_ss<<itm.first <<":" <<itm.second;
		}
		result_ss.str().swap(io);
		return 0;
	}
};


PxGlobalConf::PxGlobalConf()
{
}

PxGlobalConf::~PxGlobalConf()
{
}

// -1 fail and exit, 0 success
int PxGlobalConf::init(std::string conf_file_path)
{
	m_strGlobalConfFilePath=conf_file_path;
	//g_strDataPath + "sq_global.xml"
	if (0 == loadFromFile())
		return 0;
	log(Warn, "SQGlobalConf::init. loadFromFile fail.");
	exit(-1);
	return -1;
}

int PxGlobalConf::init_from_mem(std::string xml_content)
{
	if (0 != parseGlobalConfXml(xml_content, StringUtil::getMD5(xml_content)))
	{
		log(Warn, "PxGlobalConf::init_from_mem. parseGlobalConfXml error. ");
		return -1;
	}
	return 0;
}
/**
 * @return 
	0	success
	-1	parse xml fail
	-2	save fail
	-3	md5 not match
*/
int32_t PxGlobalConf::updateGlobalConf(const std::string& strXmlContent)
{
	MtxAutoUnlock lck(m_mtxConf);
	std::string md5= StringUtil::getMD5(strXmlContent);
	if (0 != parseGlobalConfXml(strXmlContent, md5) )
	{
		return -1;
	}
	if (0 != gl_write_to_file(m_strGlobalConfFilePath,strXmlContent))
	{
		return -2;
	}
	m_strGlobalConfMd5 = md5;
	return 0;
}

// change md5
// -1 save fail, -2 read file fail
int32_t PxGlobalConf::saveToFile()
{
	MtxAutoUnlock lck(m_mtxConf);

	if(gl_inner_saveToFile(m_strGlobalConfFilePath)<0)
	{
		return -1;
	}

	std::string strXml;
	if(gl_read_from_file(m_strGlobalConfFilePath,strXml)!=0)
	{
		return -2;
	}
	// md5
	m_strGlobalConfMd5 = StringUtil::getMD5(strXml);

	return 0;
}

int32_t PxGlobalConf::saveToBackUpFile()
{
	std::string back_upf=m_strGlobalConfFilePath+"_backup";
	if(gl_inner_saveToFile(back_upf)<0)
	{
		return -1;
	}
	return 0;
}

int32_t PxGlobalConf::gl_inner_saveToFile(std::string& fpath)
{
	// save
	TiXmlDocument xmlDoc;
	TiXmlDeclaration* xmlDecl = new TiXmlDeclaration("1.0", "", "");
	xmlDoc.LinkEndChild(xmlDecl);

	TiXmlElement* root = new TiXmlElement("conf");
	xmlDoc.LinkEndChild(root);

	TiXmlElement* xml_dao_list = new TiXmlElement("dao_list");
	root->LinkEndChild(xml_dao_list);

	for (std::map<std::string, PrcAdd>::iterator it = dao_list.begin();
		it != dao_list.end(); ++it)
	{
		TiXmlElement* xml_dao = new TiXmlElement("dao");
		xml_dao_list->LinkEndChild(xml_dao);

		////fill id
		TiXmlElement* xml_dao_id = new TiXmlElement("id");
		xml_dao->LinkEndChild(xml_dao_id);
		std::string strTmp=it->first;
		TiXmlText* textId = new TiXmlText(strTmp.c_str());
		xml_dao_id->LinkEndChild(textId);

		TiXmlElement* xml_ipports = new TiXmlElement("ipports");
		xml_dao->LinkEndChild(xml_ipports);
		strTmp.clear();
		convert_ipport2str(it->second.ipports,strTmp);
		TiXmlText* textIpPorts = new TiXmlText(strTmp.c_str());
		xml_ipports->LinkEndChild(textIpPorts);

		TiXmlElement* xml_groupid = new TiXmlElement("groupid");
		xml_dao->LinkEndChild(xml_groupid);
		strTmp.clear();
		JLexical::Cast(it->second.groupid, strTmp);
		TiXmlText* textGroupid = new TiXmlText(strTmp.c_str());
		xml_groupid->LinkEndChild(textGroupid);
	}

	////////////////////////
	TiXmlElement* xml_proxy_list = new TiXmlElement("proxy_list");
	root->LinkEndChild(xml_proxy_list);
	for (std::map<std::string, PrcAdd>::iterator it = proxy_list.begin();
		it != proxy_list.end(); ++it)
	{
		TiXmlElement* xml_proxy = new TiXmlElement("proxy");
		xml_proxy_list->LinkEndChild(xml_proxy);

		////fill id
		TiXmlElement* xml_dao_id = new TiXmlElement("id");
		xml_proxy->LinkEndChild(xml_dao_id);
		std::string strTmp=it->first;
		TiXmlText* textId = new TiXmlText(strTmp.c_str());
		xml_dao_id->LinkEndChild(textId);

		TiXmlElement* xml_ipports = new TiXmlElement("ipports");
		xml_proxy->LinkEndChild(xml_ipports);
		strTmp.clear();
		convert_ipport2str(it->second.ipports,strTmp);
		TiXmlText* textIpPorts = new TiXmlText(strTmp.c_str());
		xml_ipports->LinkEndChild(textIpPorts);

		TiXmlElement* xml_groupid = new TiXmlElement("groupid");
		xml_proxy->LinkEndChild(xml_groupid);
		strTmp.clear();
		JLexical::Cast(it->second.groupid, strTmp);
		TiXmlText* textGroupid = new TiXmlText(strTmp.c_str());
		xml_groupid->LinkEndChild(textGroupid);
	}

	///////////////////
	//std::map<LOCATE_TYPE, int> licence_rule;
	TiXmlElement* xml_licence = new TiXmlElement("licence");
	root->LinkEndChild(xml_licence);
	for (std::map<LOCATE_TYPE, int>::iterator it = licence_rule.begin();
		it != licence_rule.end(); ++it)
	{
		TiXmlElement* xml_rule = new TiXmlElement("rule");
		xml_licence->LinkEndChild(xml_rule);
		std::string strTmp=loc_parseMapSS2String(const_cast<LOCATE_TYPE&>(it->first));
		TiXmlText* textK = new TiXmlText(strTmp.c_str());
		xml_rule->LinkEndChild(textK);
	}

	///////////////////
	TiXmlElement* xml_throttle = new TiXmlElement("throttle");
	root->LinkEndChild(xml_throttle);
	for (std::map<LOCATE_TYPE, int>::iterator it = throttle_rule.begin();
		it != throttle_rule.end(); ++it)
	{
		TiXmlElement* xml_rule = new TiXmlElement("rule");
		xml_throttle->LinkEndChild(xml_rule);

		////fill kv
		TiXmlElement* xml_k = new TiXmlElement("k");
		xml_rule->LinkEndChild(xml_k);
		std::string strTmp=loc_parseMapSS2String(const_cast<LOCATE_TYPE&>(it->first));
		TiXmlText* textK = new TiXmlText(strTmp.c_str());
		xml_k->LinkEndChild(textK);

		TiXmlElement* xml_v = new TiXmlElement("v");
		xml_rule->LinkEndChild(xml_v);
		strTmp.clear();
		JLexical::Cast(it->second,strTmp);
		TiXmlText* textV = new TiXmlText(strTmp.c_str());
		xml_v->LinkEndChild(textV);
	}

	///////////////////
	TiXmlElement* xml_route = new TiXmlElement("route");
	root->LinkEndChild(xml_route);
	for (std::map<LOCATE_TYPE, ROUTESTRATEGY_TYPE>::iterator it = routestrategy_rule.begin();
		it != routestrategy_rule.end(); ++it)
	{
		TiXmlElement* xml_rule = new TiXmlElement("rule");
		xml_route->LinkEndChild(xml_rule);

		////fill kv
		TiXmlElement* xml_k = new TiXmlElement("k");
		xml_rule->LinkEndChild(xml_k);
		std::string strTmp=loc_parseMapSS2String(const_cast<LOCATE_TYPE&>(it->first));
		TiXmlText* textK = new TiXmlText(strTmp.c_str());
		xml_k->LinkEndChild(textK);

		TiXmlElement* xml_v = new TiXmlElement("v");
		xml_rule->LinkEndChild(xml_v);
		strTmp=loc_parseMapSS2String(it->second);
		TiXmlText* textV = new TiXmlText(strTmp.c_str());
		xml_v->LinkEndChild(textV);
	}

	TiXmlElement* xml_hashring = new TiXmlElement("hashring");
	root->LinkEndChild(xml_hashring);
	for (std::map<int, int>::iterator it = m_hashring_group2weight.begin();
		it != m_hashring_group2weight.end(); ++it)
	{
		////fill kv
		TiXmlElement* xml_nod = new TiXmlElement("node");
		xml_hashring->LinkEndChild(xml_nod);

		std::string strgroup;
		std::string strweig;
		JLexical::Cast(it->first,strgroup);
		JLexical::Cast(it->second,strweig);
		std::string final_str=strgroup+"w"+strweig;

		TiXmlText* textN = new TiXmlText(final_str.c_str());
		xml_nod->LinkEndChild(textN);
	}

	/////////
	TiXmlElement* parse_okcheck = new TiXmlElement("zzz_xml_parse_okcheck");
	root->LinkEndChild(parse_okcheck);

	//////////////////
	if (!xmlDoc.SaveFile(fpath.c_str()))
	{
		return -1;
	}
	return 0;
}

std::string PxGlobalConf::getGlobalConfMd5()
{
	std::string strRet = m_strGlobalConfMd5;
	return strRet;
}


int32_t PxGlobalConf::loadFromFile()
{
	std::string strXml;
	if(gl_read_from_file(m_strGlobalConfFilePath,strXml)<0)
	{
		log(Warn, "PxGlobalConf::loadFromFile. read file error: %s", m_strGlobalConfFilePath.c_str());
		return -1;
	}

	if (0 != parseGlobalConfXml(strXml, StringUtil::getMD5(strXml)))
	{
		log(Warn, "PxGlobalConf::loadFromFile. parseGlobalConfXml error. file: %s", m_strGlobalConfFilePath.c_str());
		return -2;
	}
	last_load_from_file=strXml;
	return 0;
}




#define RETURN_MINUS_1_WHEN_PTR_NULL(p) \
	if (NULL == p)\
	return -1;

int32_t PxGlobalConf::parseGlobalConfXml(const std::string& strXml, const std::string& md5)
{	
	std::map<std::string, PrcAdd> load_dao_list;///key is serverid
	std::map<std::string, PrcAdd> load_proxy_list;///key is serverid
	std::map<LOCATE_TYPE, int> load_licence_rule;
	std::map<LOCATE_TYPE, int> load_throttle_rule;
	std::map<LOCATE_TYPE, ROUTESTRATEGY_TYPE > load_routestrategy_rule;
	std::map<int,int> load_m_hashring_group2weight;
	
	TiXmlDocument doc;
	doc.Parse(strXml.c_str());

	TiXmlElement* pConf = doc.FirstChildElement("conf");
	RETURN_MINUS_1_WHEN_PTR_NULL(pConf);
	TiXmlElement* pParseCheckOk = pConf->FirstChildElement("zzz_xml_parse_okcheck");
	RETURN_MINUS_1_WHEN_PTR_NULL(pParseCheckOk);

	TiXmlElement* pDaoList = pConf->FirstChildElement("dao_list");
	RETURN_MINUS_1_WHEN_PTR_NULL(pDaoList);

	TiXmlElement* pDaoElm = pDaoList->FirstChildElement("dao");
	while (NULL != pDaoElm)
	{
		// base info
		std::string tmpkey;
		PrcAdd tmpAddInfo;
		TiXmlElement* pId = pDaoElm->FirstChildElement("id");
		RETURN_MINUS_1_WHEN_PTR_NULL(pId);
		JLexical::Cast(gl_getStr(pId), tmpkey);

		TiXmlElement* pIpPorts = pDaoElm->FirstChildElement("ipports");
		RETURN_MINUS_1_WHEN_PTR_NULL(pIpPorts);
		std::string tmp_strr;
		JLexical::Cast(gl_getStr(pIpPorts), tmp_strr);
		convert_str2ipport(tmp_strr,tmpAddInfo.ipports);

		TiXmlElement* pGroupId = pDaoElm->FirstChildElement("groupid");
		RETURN_MINUS_1_WHEN_PTR_NULL(pGroupId);
		JLexical::Cast(gl_getStr(pGroupId), tmpAddInfo.groupid);

		load_dao_list[tmpkey] = tmpAddInfo;
		pDaoElm = pDaoElm->NextSiblingElement("dao");
	}

	TiXmlElement* pProxyList = pConf->FirstChildElement("proxy_list");
	RETURN_MINUS_1_WHEN_PTR_NULL(pProxyList);
	TiXmlElement* pProxyElm = pProxyList->FirstChildElement("proxy");
	while (NULL != pProxyElm)
	{
		// base info
		std::string tmpkey;
		PrcAdd tmpAddInfo;
		TiXmlElement* pId = pProxyElm->FirstChildElement("id");
		RETURN_MINUS_1_WHEN_PTR_NULL(pId);
		JLexical::Cast(gl_getStr(pId), tmpkey);

		TiXmlElement* pIpPorts = pProxyElm->FirstChildElement("ipports");
		RETURN_MINUS_1_WHEN_PTR_NULL(pIpPorts);
		std::string tmp_strr;
		JLexical::Cast(gl_getStr(pIpPorts), tmp_strr);
		convert_str2ipport(tmp_strr,tmpAddInfo.ipports);

		TiXmlElement* pGroupId = pProxyElm->FirstChildElement("groupid");
		RETURN_MINUS_1_WHEN_PTR_NULL(pGroupId);
		JLexical::Cast(gl_getStr(pGroupId), tmpAddInfo.groupid);

		load_proxy_list[tmpkey] = tmpAddInfo;
		pProxyElm = pProxyElm->NextSiblingElement("proxy");
	}
	////////////////////
	
	//std::map<LOCATE_TYPE, int> load_licence_rule;
	TiXmlElement* pLicence = pConf->FirstChildElement("licence");
	RETURN_MINUS_1_WHEN_PTR_NULL(pLicence);
	TiXmlElement* pLicenceElm = pLicence->FirstChildElement("rule");
	while (NULL != pLicenceElm)
	{
		// base info
		LOCATE_TYPE tmpkey;
		std::string tmp;JLexical::Cast(gl_getStr(pLicenceElm), tmp);
		loc_parseString2MapSS(tmp,tmpkey);	
		load_licence_rule[tmpkey] = 1;
		pLicenceElm = pLicenceElm->NextSiblingElement("rule");
	}

	///////////////////
	TiXmlElement* pThrottle = pConf->FirstChildElement("throttle");
	RETURN_MINUS_1_WHEN_PTR_NULL(pThrottle);
	TiXmlElement* pThrottleElm = pThrottle->FirstChildElement("rule");
	while (NULL != pThrottleElm)
	{
		// base info
		LOCATE_TYPE tmpkey;
		int tmpValue;
		TiXmlElement* pK = pThrottleElm->FirstChildElement("k");
		RETURN_MINUS_1_WHEN_PTR_NULL(pK);
		std::string tmp;JLexical::Cast(gl_getStr(pK), tmp);
		loc_parseString2MapSS(tmp,tmpkey);	

		TiXmlElement* pV = pThrottleElm->FirstChildElement("v");
		RETURN_MINUS_1_WHEN_PTR_NULL(pV);
		JLexical::Cast(gl_getStr(pV), tmpValue);

		load_throttle_rule[tmpkey] = tmpValue;
		pThrottleElm = pThrottleElm->NextSiblingElement("rule");
	}

	TiXmlElement* pRoute = pConf->FirstChildElement("route");
	RETURN_MINUS_1_WHEN_PTR_NULL(pRoute);
	TiXmlElement* pRouteElm = pRoute->FirstChildElement("rule");
	while (NULL != pRouteElm)
	{
		// base info
		LOCATE_TYPE tmpkey;
		ROUTESTRATEGY_TYPE tmpValue;
		std::string tmp;

		TiXmlElement* pK = pRouteElm->FirstChildElement("k");
		RETURN_MINUS_1_WHEN_PTR_NULL(pK);
		JLexical::Cast(gl_getStr(pK), tmp);
		loc_parseString2MapSS(tmp,tmpkey);	

		TiXmlElement* pV = pRouteElm->FirstChildElement("v");
		RETURN_MINUS_1_WHEN_PTR_NULL(pV);
		JLexical::Cast(gl_getStr(pV), tmp);
		loc_parseString2MapSS(tmp,tmpValue);

		load_routestrategy_rule[tmpkey] = tmpValue;
		pRouteElm = pRouteElm->NextSiblingElement("rule");
	}

	TiXmlElement* pHashRing = pConf->FirstChildElement("hashring");
	RETURN_MINUS_1_WHEN_PTR_NULL(pHashRing);
	TiXmlElement* pHashRingElm = pHashRing->FirstChildElement("node");
	while (NULL != pHashRingElm)
	{
		std::string groupweight=gl_getStr(pHashRingElm);
		size_t pos = groupweight.find_first_of("w");
		if (pos != string::npos)
		{
			std::string groupidstr=groupweight.substr(0,pos);
			std::string weightstr=groupweight.substr(pos+1);
			int groupid=0;
			int weighti=0;
			JLexical::Cast(groupidstr,groupid);
			JLexical::Cast(weightstr,weighti);
			load_m_hashring_group2weight[groupid]=weighti;
		}
		else
		{
			return -1;
		}
		pHashRingElm = pHashRingElm->NextSiblingElement("node");
	}

	std::string errinfoo;
	if(validate_conf_para(errinfoo,load_dao_list
		,load_proxy_list,load_throttle_rule,load_routestrategy_rule
		,load_m_hashring_group2weight,load_licence_rule)!=0)
	{
		log(Warn, "PxGlobalConf::validate_conf_para. error: %s", errinfoo.c_str());
		return -1;
	}
	load_dao_list.swap(dao_list);
	load_proxy_list.swap(proxy_list);
	load_licence_rule.swap(licence_rule);
	load_throttle_rule.swap(throttle_rule);
	load_routestrategy_rule.swap(routestrategy_rule);
	load_m_hashring_group2weight.swap(m_hashring_group2weight);
	m_strGlobalConfMd5 = md5;
	return 0;
}


void PxGlobalConf::set_dao_item(std::string& serverid,PrcAdd& address)
{
	MtxAutoUnlock lck(m_mtxConf);
	dao_list[serverid]=address;
}
void PxGlobalConf::set_proxy_item(std::string& serverid,PrcAdd& address)
{
	MtxAutoUnlock lck(m_mtxConf);
	proxy_list[serverid]=address;
}
void PxGlobalConf::del_dao_item(std::string& serverid)
{
	MtxAutoUnlock lck(m_mtxConf);
	dao_list.erase(serverid);
}
void PxGlobalConf::del_proxy_item(std::string& serverid)
{
	MtxAutoUnlock lck(m_mtxConf);
	proxy_list.erase(serverid);
}
void PxGlobalConf::get_all_dao(std::map<std::string, PrcAdd>& outdat)
{
	MtxAutoUnlock lck(m_mtxConf);
	outdat=dao_list;
}
void PxGlobalConf::get_all_proxy(std::map<std::string, PrcAdd>& outdat)
{
	MtxAutoUnlock lck(m_mtxConf);
	outdat=proxy_list;
}

int PxGlobalConf::check_validate()
{
	std::set<IPPORTYPE> stt;
	std::map<std::string, PrcAdd>::iterator it=dao_list.begin();
	for(;it!=dao_list.end();++it)
	{
		std::vector<IPPORTYPE>& ipports=it->second.ipports;
		for(size_t n=0;n<ipports.size();++n)
		{
			if(stt.find(ipports[n])!=stt.end())
			{
				///ip port conflict!
				return -1;
			}
			stt.insert(ipports[n]);
		}
	}
	///////////////////
	it=proxy_list.begin();
	for(;it!=proxy_list.end();++it)
	{
		std::vector<IPPORTYPE>& ipports=it->second.ipports;
		for(size_t n=0;n<ipports.size();++n)
		{
			if(stt.find(ipports[n])!=stt.end())
			{
				///ip port conflict!
				return -1;
			}
			stt.insert(ipports[n]);
		}
	}
	return 0;
}


void PxGlobalConf::getall_daoadd(IProcessNotify* prcdao_cb)
{
	std::map<std::string, PrcAdd>::iterator i;
	for(i=dao_list.begin();i!=dao_list.end();++i)
	{
		prcdao_cb->process_notify(i->first,i->second.ipports,i->second.groupid);
	}
}
void PxGlobalConf::getall_proxyadd(IProcessNotify* prcproxy_cb)
{
	std::map<std::string, PrcAdd>::iterator i;
	for(i=proxy_list.begin();i!=proxy_list.end();++i)
	{
		prcproxy_cb->process_notify(i->first,i->second.ipports,i->second.groupid);
	}
}
void PxGlobalConf::getall_licence(ILicenceNotify* licence_cb)
{
	std::map<LOCATE_TYPE, int>::iterator i;
	for(i=licence_rule.begin();i!=licence_rule.end();++i)
	{
		licence_cb->licence_notify(i->first);
	}
}
void PxGlobalConf::getall_throttle_limit(IThrottleNotify* thr_cb)
{
	std::map<LOCATE_TYPE, int>::iterator i;
	for(i=throttle_rule.begin();i!=throttle_rule.end();++i)
	{
		thr_cb->throttle_notify(i->first,i->second);
	}
}
void PxGlobalConf::getall_route_rule(IRouteNotify* route_cb)
{
	std::map<LOCATE_TYPE, ROUTESTRATEGY_TYPE >::iterator i;
	for(i=routestrategy_rule.begin();i!=routestrategy_rule.end();++i)
	{
		route_cb->route_notify(i->first,i->second);
	}
}

std::map<int,int>& PxGlobalConf::get_hash_ring()
{
	return m_hashring_group2weight;
}


//return value
////0:ok -1:fail
////1:iplist empty
////2:ipport conflict
////3:locate out of bound
////4:throttle less then zero
////5:strategy value unrecognize
////6:strategy seq unrecognize
////7:usability level error
////8:force group empty
////9:hash ring out of bound
int PxGlobalConf::validate_conf_para(
					  std::string& error_info
					 ,std::map<std::string, PxGlobalConf::PrcAdd>& load_dao_list
					 ,std::map<std::string, PxGlobalConf::PrcAdd>& load_proxy_list
					 ,std::map<PxGlobalConf::LOCATE_TYPE, int>& load_throttle_rule
					 ,std::map<PxGlobalConf::LOCATE_TYPE, ROUTESTRATEGY_TYPE >& load_routestrategy_rule
					 ,std::map<int,int>& load_m_hashring_group2weight
					 ,std::map<PxGlobalConf::LOCATE_TYPE, int>& load_licence_rule)
{
	std::set<IPPORTYPE> ipport_set;////check srv ips
	for(std::map<std::string, PxGlobalConf::PrcAdd>::iterator idl
		=load_dao_list.begin();idl!=load_dao_list.end();++idl)
	{
		std::vector<IPPORTYPE>& temipport=idl->second.ipports;
		if(temipport.empty())
		{
			error_info+="iplist empty";
			return 1;
		}
		for(size_t b=0;b<temipport.size();++b)
		{
			if(ipport_set.find(temipport[b])!=ipport_set.end())
			{
				error_info+="ipport conflict";
				return 2;
			}
			ipport_set.insert(temipport[b]);
		}
	}
	for(std::map<std::string, PxGlobalConf::PrcAdd>::iterator idl
		=load_proxy_list.begin();idl!=load_proxy_list.end();++idl)
	{
		std::vector<IPPORTYPE>& temipport=idl->second.ipports;
		if(temipport.empty())
		{
			error_info+="iplist empty";
			return 1;
		}
		for(size_t b=0;b<temipport.size();++b)
		{
			if(ipport_set.find(temipport[b])!=ipport_set.end())
			{
				error_info+="ipport conflict";
				return 2;
			}
			ipport_set.insert(temipport[b]);
		}
	}
	/////////////////throttling
	for(std::map<PxGlobalConf::LOCATE_TYPE, int>::iterator b
		=load_throttle_rule.begin();
		b!=load_throttle_rule.end();
		++b)
	{
		PxGlobalConf::LOCATE_TYPE& lmp=const_cast<PxGlobalConf::LOCATE_TYPE&>(b->first);
		for(PxGlobalConf::LOCATE_TYPE::iterator v
			=lmp.begin();
			v!=lmp.end();
		++v)
		{
			if(v->first==std::string("A"))
			{}
			else if(v->first==std::string("D"))
			{}
			else if(v->first==std::string("G"))
			{}
			else
			{
				error_info+="locate out of bound";
				return 3;
			}
		}
		if(b->second<0)
		{
			error_info+="throttle less then zero";
			return 4;
		}
	}
	////////////////////routestrategy
	for(std::map<PxGlobalConf::LOCATE_TYPE, ROUTESTRATEGY_TYPE >::iterator lr
		=load_routestrategy_rule.begin();
		lr!=load_routestrategy_rule.end();
		++lr)
	{
		PxGlobalConf::LOCATE_TYPE& lmp=const_cast<PxGlobalConf::LOCATE_TYPE&>(lr->first);
		for(PxGlobalConf::LOCATE_TYPE::iterator v
			=lmp.begin();
			v!=lmp.end();
		++v)
		{
			if(v->first==std::string("A"))
			{}
			else if(v->first==std::string("D"))
			{}
			else if(v->first==std::string("G"))
			{}
			else
			{
				error_info+="locate out of bound";
				return 3;
			}
		}
		//////////////
		ROUTESTRATEGY_TYPE& vmp=lr->second;
		for(ROUTESTRATEGY_TYPE::iterator v
			=vmp.begin();
			v!=vmp.end();
			++v)
		{
			if(v->first==std::string("S"))
			{
				std::string& ss=v->second;
				for(size_t g=0;g<ss.size();++g)
				{
					if((g+1)==ss.size())
					{
						if(ss[g]!='A'&&ss[g]!='B'&&ss[g]!='C'
							&&ss[g]!='E'&&ss[g]!='U'&&ss[g]!='F'
							&&ss[g]!='D'&&ss[g]!='R'&&ss[g]!='L')
						{
							error_info+="strategy seq unrecognize";
							return 6;
						}
					}
					else
					{
						if(ss[g]!='A'&&ss[g]!='B'&&ss[g]!='C'
							&&ss[g]!='E'&&ss[g]!='U'&&ss[g]!='F')
						{
							error_info+="strategy seq unrecognize";
							return 6;
						}
					}
				}
			}
			else if(v->first==std::string("U"))
			{
				int levl_l=-1;
				JLexical::Cast(v->second,levl_l);
				if(levl_l<0||levl_l>2)
				{
					error_info+="usability level error";
					return 7;
				}
			}
			else if(v->first==std::string("F"))
			{
				std::set<int> groupids;
				parseStr2SetInt(v->second,groupids);
				if(groupids.empty())
				{
					error_info+="force group empty";
					return 8;
				}
			}
			else if(v->first==std::string("E"))
			{				
			}
			else
			{
				error_info+="strategy value unrecognize";
				return 5;
			}
		}

	}

	////////hashring
	if(load_m_hashring_group2weight.empty())
	{
		error_info+="hash ring empty";
		return 10;
	}
	for(std::map<int,int>::iterator iv=
		load_m_hashring_group2weight.begin();
		iv!=load_m_hashring_group2weight.end();
		++iv)
	{
		if(iv->second<=0)
		{
			error_info+="hash ring out of bound";
			return 9;
		}
		else if(iv->second>1000)
		{
			error_info+="hash ring out of bound";
			return 9;
		}
	}

	///////licences
	for(std::map<PxGlobalConf::LOCATE_TYPE, int>::iterator r
		=load_licence_rule.begin();
		r!=load_licence_rule.end();
		++r)
	{
		PxGlobalConf::LOCATE_TYPE& lmp=const_cast<PxGlobalConf::LOCATE_TYPE&>(r->first);
		for(PxGlobalConf::LOCATE_TYPE::iterator v
			=lmp.begin();
			v!=lmp.end();
		++v)
		{
			if(v->first==std::string("A"))
			{}
			else if(v->first==std::string("D"))
			{}
			else if(v->first==std::string("G"))
			{}
			else
			{
				error_info+="locate out of bound";
				return 3;
			}
		}
	}
	return 0;
}
