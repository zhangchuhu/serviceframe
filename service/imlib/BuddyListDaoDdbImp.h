#pragma once

#include "IMetaDataDao.h"
#include "IBuddyListDao.h"
#include "server_common/dblib/IMysqlDao.h"

namespace server
{
	namespace mysqlimdb
	{
		class CBuddyListDaoDdbImp:
			public server::imdao::IBuddyListDao,
			public server::mysqldb::MysqlHelperTempalteAware,
            public server::imdao::IMetaDataDaoAware
		{
		public:
			CBuddyListDaoDdbImp(server::mysqldb::MysqlHelperTempalte *poracleHT = NULL);

			virtual bool Delete(uint32_t uUid, uint32_t uBid);

			virtual bool Update(uint32_t uUid, uint32_t uBid, const std::string& strRemark);//修改好友备注

			virtual bool Update(uint32_t uUid, uint32_t uBid, uint32_t uPid);//移动好友到某个组

			virtual bool Add(const server::imdao::CBuddyListDaoData& objData);

			virtual bool IsExist(uint32_t uUid, uint32_t uBid);

			virtual bool Get(uint32_t uUid, std::vector<server::imdao::CBuddyListDaoData>& vecData);//取真正的好友列表

			virtual bool GetAll(uint32_t uUid, std::vector<server::imdao::CBuddyListDaoData>& vecData);//取好友列表（包括真正的和被单方删除的）

			virtual bool Get(uint32_t uUid, std::vector<uint32_t>& vecData);//取真正的好友列表

			virtual bool GetAll(uint32_t uUid, std::vector<uint32_t>& vecData);//取好友列表（包括真正的和被单方删除的）

			virtual bool Get(uint32_t uUid, std::vector<protocol::im::CBuddyListNoRemark>& vecData);//取真正的好友列表

			virtual bool Get(uint32_t uUid, std::map<uint32_t, std::string>& mapBidToRemark);//取真正的好友列表

			virtual bool GetCount(uint32_t uId,uint32_t &nCount);//取好友个数

		};
	}
}


