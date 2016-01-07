#ifndef SERVER_MYSQLDB_META_SQL_FACTORY_H
#define SERVER_MYSQLDB_META_SQL_FACTORY_H

#include <string>
#include <map>
#include <mysql++/mysql++.h>
#include <boost/thread/tss.hpp>
#include <boost/thread/mutex.hpp>

namespace server
{
	namespace mysqldb
	{
		struct CMetaData
		{
			uint32_t m_uSrcID;

			std::string m_strDBName;
			std::string m_strHost;
			uint32_t m_uPort;
			std::string m_strUser;
			std::string m_strPasswd;
		};

		class CMetaSqlFactory
		{
		public:
			typedef boost::thread_specific_ptr<mysqlpp::Connection> TssSqlConn;

			CMetaSqlFactory();

			~CMetaSqlFactory();

			virtual const CMetaData &GetMetaData(const char *pName, uint32_t uKeyID);

			mysqlpp::Connection *GetConnection(const char *pName, uint32_t uKeyID);

		private:
			mysqlpp::Connection *__CreateConnection(const CMetaData &md);

		private:
			boost::mutex m_mutexLock;
			std::map<uint32_t, TssSqlConn *> m_mapPtrConn;
			std::map<uint32_t, CMetaData> m_mapMetaCfg;
		};
	}
}

#endif
