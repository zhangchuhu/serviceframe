#ifndef _IMMEMCACHEHELPER_H_
#define _IMMEMCACHEHELPER_H_

/************************************************************************/
/* 
����Ҫ��imDataOpHelper set SqlConnFactory��MemcachedFactory
*/
/************************************************************************/

#include "common/int_types.h"
#include "protocol/const.h"
#include "server_common/protocol/pmemcache.h"
#include "server_common/protocol/pimdb.h"
#include "common/protocol/pimdb.h"
#include "server_common/dblib/ISqlConnFactory.h"
#include "server_common/dblib/Memcached.h"
#include "server_common/dblib/IMemcachedFactory.h"

#include <libmemcached/memcached.h>
#include <set>
#include <map>
#include <vector>
// 
// #include <stdlib.h>
// #include <ctype.h>
//#include "imlib/CUinfoDaoImp.h"
#include "IMetaSqlFactoryAware.h"

namespace server
{
	namespace im
	{
		#define IMDB_MEMCACHED "imdb"
		#define UINFO_SLAVE_DB "uinfo_slave"


		class imDataOpHelper:
			public db::ISqlConnFactoryAware,
			public mc::MemcachedFactoryAware,
			public mysqldb::IMetaSqlFactoryAware
		{
		public:
			
			//ȡ����uid�ĺ����б�
			//bool set_buddylist(uint32_t uid, std::vector<uint32_t>& list);
			bool get_buddylist(uint32_t uid, std::vector<uint32_t>& list);

			//ȡ����uid�İ���������Ϣ�ĺ����б�����Ժ��ѵı�ע�ͺ������ڵĸ�Ŀ¼��
			//bool set_simple_buddylist(uint32_t uid, std::vector<server::im::SimpleBInfo>& blist);
			bool get_simple_buddylist(uint32_t uid, std::vector<server::im::SimpleBInfo>& buddylist);

			//ȡ����uid��״̬
			//void set_status(uint32_t uid, protocol::E_IMSTATUS st, uint32_t linkd_id);
			void get_status(uint32_t uid, protocol::E_IMSTATUS& st, uint32_t& linkd_id);

			//ȡuid���������ߵĺ���
			bool get_not_offline_buddylist(uint32_t uid, std::vector<uint32_t>& onlinebuddies);

			bool get_not_offline_buddylist_and_imlinkid(uint32_t uid, vector<uint32_t>& onlinebuddies, vector<uint32_t>& imlinkids);

			//ȡuid�����к��ѵ�״̬
			void get_buddy_status(uint32_t uid, std::vector<protocol::im::Budid_St>& bid_st_vec);

			//�ж�uid�Ƿ�����
			bool isoffline(uint32_t uid);

			//ȡuid���ڵ�imlinkd������false˵��������
			bool getImLinkd(uint32_t uid, uint32_t& imlinkd);

			//ͨ�������б�ȡ�б��е��˵�uinfo
			//void buddylist_get_uinfo(std::vector<uint32_t>& buddylist, std::map<uint32_t, protocol::im::SUserinfo>& uinfo_map);

			//ͨ�������б�ȡ�б��е��˵�״̬
			void buddylist_get_status(std::vector<uint32_t>& buddylist, std::vector<protocol::im::Budid_St>& bid_st_vec);
			//wuji start
			void buddylist_get_status(uint32_t uId,  std::vector<protocol::im::Budid_St>&bid_st_vec);//取某个用户的所有在线状态
			bool get_simple_buddylist(uint32_t uid, vector<CSimpleBInfo1>& buddylist);
			bool GetBuddyList4Brocast(uint32_t uId, vector<uint32_t>& vecData);//取用于广播的好友列表
			bool GetNoRemarkBuddylist(uint32_t uId, vector<protocol::im::CBuddyListNoRemark>& vecData);
			//wuji end

			//ȡuid�����к��ѵ�uinfo
			//void get_buddy_uinfo(uint32_t uid, std::map<uint32_t, protocol::im::SUserinfo>& uinfo_map);

			//ȡuid��uinfo
			//bool get_userinfo(uint32_t uid, protocol::im::SUserinfo& info);

			//ȡuid��nickname
			//void get_budnick(uint32_t uid, std::string& nickname);

			//ȡ��������uinfo������״̬�ֶΣ�
			//void set_simple_uinfo(uint32_t uid, server::mc::PMC_Simple_Uinfo& info);
			//void get_simple_uinfo(uint32_t uid, server::mc::PMC_Simple_Uinfo& mcinfo);


		public:
			//typedef memcached_return (*mcm_callback)(memcached_st *ptr, memcached_result_st *result, void *context);
			//typedef void (*key_method)(uint32_t uid, std::string& key);

			//bool _multi_get(server::mc::Memcached *m,  const std::vector<uint32_t> &ids, key_method km, mcm_callback cb, void *context);

			//wuji start
			//void SetUinfoDao(server::oracledb::im::CUInfoDaoImp* pObj)
			//{
			//	m_pobjUinfo=pObj;
			//}

			imDataOpHelper()
			{
				//m_pobjUinfo=NULL;
			}
			
			//wuji end


		private:
			//void _set_status(uint32_t uid, server::mc::PMC_OnlineStat& onlinest, server::mc::Memcached* mc);
			//void _get_status(uint32_t uid, server::mc::PMC_OnlineStat& onlinest, mysqlpp::Connection* conn, server::mc::Memcached* mc);

		protected:
			//wuji start
			//server::oracledb::im::CUInfoDaoImp *m_pobjUinfo;
			
			//wuji end


			
		};
	}
}

#endif
