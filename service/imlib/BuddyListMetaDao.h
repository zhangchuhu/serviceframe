#pragma once
// you had better use list or deque! instead of vector;
#include <list>
#include <string>
#include "IMetaDataDao.h"
#include "server_common/dblib/IMysqlDao.h"


namespace server
{
//namespace mysqldb
	namespace mysqlimdb
	{
		struct CMetaData
		{
			uint32_t m_uBegin;
			uint32_t m_uEnd;
			std::string m_strDbName;
		};

		class CBuddyListMetaDao:
			public server::mysqldb::MysqlHelperTempalteAware,
			public server::imdao::IMetaDataDao
		{
		public:
			CBuddyListMetaDao(server::mysqldb::MysqlHelperTempalte *p = NULL);

			virtual std::string &GetDbName(uint32_t uUsrID);

		private:
			std::string m_strDefDbName;

			std::list<CMetaData> m_listMetaData;
		};


	}

}
