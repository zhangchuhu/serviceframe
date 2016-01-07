#ifndef SERVER_MYSQLDB_IMETA_SQL_FACTORY_AWARE_H
#define SERVER_MYSQLDB_IMETA_SQL_FACTORY_AWARE_H

#include "MetaSqlFactory.h"

namespace server
{
	namespace mysqldb
	{
		class IMetaSqlFactoryAware
		{
		public:
			void SetMetaFactory(CMetaSqlFactory *p)
			{
				m_pMetaSqlFactory = p;
			}

			CMetaSqlFactory *GetMetaFactory()
			{
				return m_pMetaSqlFactory;
			}

		private:
			CMetaSqlFactory *m_pMetaSqlFactory;
		};
	}
}

#endif
