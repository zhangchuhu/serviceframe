#include "MetaSqlFactory.h"
#include "Logger.h"

using namespace mysqlpp;
using namespace server::mysqldb;


static struct MetaConfig {
	const char *db;
	const char *host;
	uint32_t port;
	const char *user;
	const char *passwd;
} s_arrMetaCfg[] = {
	{"im1", "im_master.yy.duowan.com", 3306, "im-db-admin", "dsqazxsw21"},
	{"im2", "im_master.yy.duowan.com", 3306, "im-db-admin", "dsqazxsw21"},
	{"im3", "im_master.yy.duowan.com", 3306, "im-db-admin", "dsqazxsw21"},
	{"im4", "im_master.yy.duowan.com", 3306, "im-db-admin", "dsqazxsw21"},
	{"im5", "im_master.yy.duowan.com", 3306, "im-db-admin", "dsqazxsw21"},
	{"im6", "im_master.yy.duowan.com", 3306, "im-db-admin", "dsqazxsw21"},
	{"im7", "im_master.yy.duowan.com", 3306, "im-db-admin", "dsqazxsw21"},
	{"im8", "im_master.yy.duowan.com", 3306, "im-db-admin", "dsqazxsw21"},
};



#undef __CLASS__
#define __CLASS__ "CMetaSqlFactory"

CMetaSqlFactory::CMetaSqlFactory()
{
	uint32_t j = (uint32_t) (sizeof(s_arrMetaCfg) / sizeof(MetaConfig));
	for (uint32_t i = 0; i < j; i++)
	{
		CMetaData &md = m_mapMetaCfg[i];
		md.m_uSrcID = i;
		md.m_strDBName = s_arrMetaCfg[i].db;
		md.m_strHost = s_arrMetaCfg[i].host;
		md.m_uPort = s_arrMetaCfg[i].port;
		md.m_strUser = s_arrMetaCfg[i].user;
		md.m_strPasswd = s_arrMetaCfg[i].passwd;
		m_mapPtrConn[i] = new TssSqlConn;

		log(Debug, "source:%u {db:%s host:%s port:%u}",\
			i, md.m_strDBName.c_str(), md.m_strHost.c_str(), md.m_uPort);
	}
}

CMetaSqlFactory::~CMetaSqlFactory()
{
	std::map<uint32_t, TssSqlConn *>::iterator iter;
	for (iter = m_mapPtrConn.begin(); iter != m_mapPtrConn.end(); iter++)
	{
		delete iter->second;
	}
}

const CMetaData &CMetaSqlFactory::GetMetaData(const char *pName, uint32_t uKeyID)
{
	if (uKeyID <= 6100000)
	{
		return m_mapMetaCfg[0];
	}
	else if (uKeyID > 6100000 && uKeyID <= 12000000)
	{
		return m_mapMetaCfg[1];
	}
	else if (uKeyID > 12000000 && uKeyID <= 18000000)
	{
		return m_mapMetaCfg[2];
	}
	else if (uKeyID > 18000000 && uKeyID <= 60500000)
	{
		return m_mapMetaCfg[3];
	}
	else if (uKeyID > 60500000 && uKeyID <= 66000000)
	{
		return m_mapMetaCfg[4];
	}
	else if (uKeyID > 66000000 && uKeyID <= 70500000)
	{
		return m_mapMetaCfg[5];
	}
	else if (uKeyID > 70500000 && uKeyID <= 76000000)
	{
		return m_mapMetaCfg[6];
	}
    else if (uKeyID > 76000000 && uKeyID <= 95880000)
    {
        return m_mapMetaCfg[7];
    }
	else
	{
		return m_mapMetaCfg[0];
	}
}

Connection *CMetaSqlFactory::GetConnection(const char *pDbName, uint32_t uKeyID)
{
	boost::mutex::scoped_lock sl(m_mutexLock);

	const CMetaData &md = GetMetaData(pDbName, uKeyID);
	SYSLOG(Debug, "key:%u => source:%u {db:%s host:%s port:%u}",\
		uKeyID, md.m_uSrcID, md.m_strDBName.c_str(), md.m_strHost.c_str(), md.m_uPort);

	std::map<uint32_t, TssSqlConn *>::iterator iter;

	TssSqlConn *pptr = m_mapPtrConn[md.m_uSrcID];
	Connection *pConn;

	if ((pConn = pptr->get()) == NULL)
	{
		pConn = __CreateConnection(md);
		if (pConn != NULL)
		{
			pptr->reset(pConn);
		}
	}
	return pConn;
}

Connection *CMetaSqlFactory::__CreateConnection(const CMetaData &md)
{
	Connection *pConn = new Connection;
	try
	{
        pConn->set_option(Connection::opt_set_charset_name, "utf8");
        pConn->set_option(Connection::opt_reconnect, true);
		pConn->connect(md.m_strDBName.c_str(), md.m_strHost.c_str(), md.m_strUser.c_str(), md.m_strPasswd.c_str(), md.m_uPort);
	} 
	catch (mysqlpp::Exception e)
	{
		delete pConn;
		SYSLOG(Error, "failed! %s", e.what());
		return NULL;
	}

	SYSLOG(Info, "success! source:%u", md.m_uSrcID);
	return pConn;
}
