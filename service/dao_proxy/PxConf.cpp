#include "PxConf.h"
#include "FileUtils.h"
#include "core/sox/logger.h"
#include "server_common/config/tinyxml.h"
#include "server_common/seda/common/any_cast.h"

using namespace server::dao_proxy;

PxConf::PxConf()
{
	groupid=0;
	port=0;
}
PxConf::~PxConf()
{
}

// -1 fail and exit, 0 success
int32_t PxConf::init(std::string conf_path)
{
//	std::string server_name;
//	int groupid;
	std::string strXml;
	if(gl_read_from_file(conf_path,strXml)<0)
	{
		log(Warn, "PxConf::init. read file error: %s", conf_path.c_str());
		return -1;
	}

	TiXmlDocument doc;
	doc.Parse(strXml.c_str());

	TiXmlElement* pConf = doc.FirstChildElement("conf");
	if(pConf==NULL)
	{
		log(Warn, "PxConf::init. conf parse error: can't find <conf>");
		return -1;
	}
	TiXmlElement* pServers = pConf->FirstChildElement("servers");
	if(pServers==NULL)
	{
		log(Warn, "PxConf::init. conf parse error: can't find <servers>");
		return -1;
	}
	TiXmlElement* pProxyy = pServers->FirstChildElement("dao_proxy");
	if(pProxyy==NULL)
	{
		log(Warn, "PxConf::init. conf parse error: can't find <dao_proxy>");
		return -1;
	}
	////////////////////pProxyy

	TiXmlElement* pItem=0;

	pItem= pProxyy->FirstChildElement("name");
	if(pItem==NULL)
	{
		log(Warn, "PxConf::init. conf parse error: can't find <name>");
		return -1;
	}
	JLexical::Cast(pItem->GetText(), server_name);
	pItem= pProxyy->FirstChildElement("group");
	if(pItem==NULL)
	{
		log(Warn, "PxConf::init. conf parse error: can't find <group>");
		return -1;
	}
	JLexical::Cast(pItem->GetText(), groupid);
	pItem= pProxyy->FirstChildElement("port");
	if(pItem==NULL)
	{
		log(Warn, "PxConf::init. conf parse error: can't find <port>");
		return -1;
	}
	JLexical::Cast(pItem->GetText(), port);
	pItem= pProxyy->FirstChildElement("myip");
	if(pItem==NULL)
	{
		log(Warn, "PxConf::init. conf parse error: can't find <myip>");
		return -1;
	}
	JLexical::Cast(pItem->GetText(), myip);

	return 0;
}


