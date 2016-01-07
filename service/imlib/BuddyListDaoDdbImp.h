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

			virtual bool Update(uint32_t uUid, uint32_t uBid, const std::string& strRemark);//�޸ĺ��ѱ�ע

			virtual bool Update(uint32_t uUid, uint32_t uBid, uint32_t uPid);//�ƶ����ѵ�ĳ����

			virtual bool Add(const server::imdao::CBuddyListDaoData& objData);

			virtual bool IsExist(uint32_t uUid, uint32_t uBid);

			virtual bool Get(uint32_t uUid, std::vector<server::imdao::CBuddyListDaoData>& vecData);//ȡ�����ĺ����б�

			virtual bool GetAll(uint32_t uUid, std::vector<server::imdao::CBuddyListDaoData>& vecData);//ȡ�����б����������ĺͱ�����ɾ���ģ�

			virtual bool Get(uint32_t uUid, std::vector<uint32_t>& vecData);//ȡ�����ĺ����б�

			virtual bool GetAll(uint32_t uUid, std::vector<uint32_t>& vecData);//ȡ�����б����������ĺͱ�����ɾ���ģ�

			virtual bool Get(uint32_t uUid, std::vector<protocol::im::CBuddyListNoRemark>& vecData);//ȡ�����ĺ����б�

			virtual bool Get(uint32_t uUid, std::map<uint32_t, std::string>& mapBidToRemark);//ȡ�����ĺ����б�

			virtual bool GetCount(uint32_t uId,uint32_t &nCount);//ȡ���Ѹ���

		};
	}
}


