#ifndef IM_DATAOP_HELPER_WUJI_H
#define IM_DATAOP_HELPER_WUJI_H

/************************************************************************/
/* 
����Ҫ��imDataOpHelper set SqlConnFactory��MemcachedFactory
*/
/************************************************************************/

#include "common/int_types.h"
#include "common/protocol/const.h"
#include "server_common/protocol/pmemcache.h"
#include "server_common/protocol/pimdb.h"
#include "server_common/dblib/ISqlConnFactory.h"
#include "server_common/dblib/Memcached.h"
#include "server_common/dblib/IMemcachedFactory.h"
#include "common/protocol/pimcommon.h"
#include <libmemcached/memcached.h>
#include <set>
#include <map>
#include <vector>
#include "common/protocol/pimdb.h"
#include "IMetaSqlFactoryAware.h"


namespace server
{
	namespace im
	{
	
		//wuji start
		struct CObjUserSessionInfo//��¼һ���û��ڱ��ε�¼�͵ǳ�֮����һЩ��ʱ����
		{
			uint32_t m_uSearchCount;//�û��ڱ��ε�¼�����еĲ�ѯ����
// 			uint32_t m_uChanel;
// 			uint32_t m_uGame;//������Ϸ
		};
		
		struct CObjUserSessionCheckCode//��¼�û��ڱ��ε�¼�͵ǳ�֮����һЩ��ʱ����
		{
			string m_strCode;//��֤��
			uint32_t m_uCreateTime;//��֤������ʱ��			
		};
		
		struct CObjUserInfoAboutIm
		{
			
			uint32_t m_uCeLie;//��֤����
			
		};
		//wuji end

		class imDataOpHelper_wuji:
			public imDataOpHelper
		{
		public:
			
			//wuji start

			bool get_buddylist_im_at_game(uint32_t uid, vector<uint32_t>& vecBid,vector<uint32_t>& vecGameId,vector<string>& vecStrData);
			bool get_buddylist_im_at_chanel(uint32_t uid, vector<uint32_t>& vecBid,vector<uint32_t>& vecGameId,vector<string>& vecStrData);
			bool get_buddylist_im_at_extradata(uint32_t uid, vector<uint32_t>& vecBid,vector<string>& vecStrData);

			bool get_user_im_imid(uint32_t uId, uint32_t &uImid);

			bool get_user_im_imid_set(const std::vector<uint32_t> &vtBid, std::map<uint32_t, uint32_t>& m_mapBidToImId);

			//bool get_userinfo_db(uint32_t uid, protocol::im::SUserinfo1& info);

			bool get_user_im_extra_data(uint32_t uId,string &strData);
			//bool set_user_im_extra_data(uint32_t uId,string &strData);
			//bool del_user_im_extra_data(uint32_t uId);

			void get_user_im_chanel_headphoto_game(uint32_t uId,uint32_t &uHeadPhoto,uint32_t& uChanelId,uint32_t &uGameId,string& strGameData);

//			bool check_simple_uinfo_db(uint32_t uid,bool &bIsExist);

			//�����û�������Ϸ
			bool get_user_im_at_game(uint32_t uid, uint32_t& uGameId,string& strData);
			bool get_user_im_at_game(const vector<uint32_t> &vecBid,vector<protocol::im::CObjList1> &m_vGame);
			bool del_user_im_at_game(uint32_t uid);
			bool set_user_im_at_game(uint32_t uid,  uint32_t uGameId,const string& strData);
			bool update_user_im_at_game(uint32_t uid,uint32_t uGameId);
			
			//�����û�����Ƶ��
			bool get_user_im_at_chanel(uint32_t uid,  uint32_t& uChanelId);
			bool get_user_im_at_chanel(uint32_t uid, uint32_t &uChanelId,string &strData);
			bool get_user_im_at_chanel(const vector<uint32_t> &vecBid,vector<protocol::im::CObjList1> & m_vChanel);
			bool del_user_im_at_chanel(uint32_t uid);
			//bool set_user_im_at_chanel(uint32_t uid,  uint32_t uChanelId);
			bool set_user_im_at_chanel(uint32_t uid, uint32_t  uChanelId,const string &strData);
			
			//�û�����֤��
// 			bool get_imuser_checkcode(uint32_t uid, CObjUserSessionCheckCode& objInfo );
// 			bool set_imuser_checkcode(uint32_t uid, const CObjUserSessionCheckCode& objInfo );
// 			bool del_imuser_checkcode(uint32_t uid );
			
			
			//�û��Ļ�������
			bool get_imuser_maxjifen(uint32_t uid, uint32_t &uJiFenMax );
			bool del_imuser_maxjifen(uint32_t uid );
			
			//���ù���IM��һЩ������Ϣ
			bool get_user_iminfo(uint32_t uid, CObjUserInfoAboutIm& objInfo );
			bool del_user_iminfo(uint32_t uid );
			
			//ȡ�û��ڱ��ε�¼�͵ǳ�֮����һЩ��ʱ����
// 			bool get_user_sessioninfo(uint32_t uid, CObjUserSessionInfo& objInfo );
 //			bool set_user_sessioninfo(uint32_t uid, const CObjUserSessionInfo& objInfo );
 //			bool del_user_sessioninfo(uint32_t uid );
			
			
			//ɾ��uid�ĺ�����,��������?
			bool del_blacklist(uint32_t uid);
			bool del_blocklist(uint32_t uid);
			
			bool IsInBlackList(uint32_t uid,uint32_t bid/*bid�Ƿ���uid��������*/,bool& bIsExist/*true������*/);
			bool IsInBlockList(uint32_t uid,uint32_t bid/*bid�Ƿ���uid��������*/,bool& bIsExist/*true������*/);
			bool IsInBuddyList(uint32_t uid,uint32_t bid/*bid�Ƿ���uid��������*/,bool& bIsExist/*true������*/);
			
			//ȡ����uid�ĺ�����
			bool set_blacklist(uint32_t uid, std::vector<uint32_t>& list);
			bool get_blacklist(uint32_t uid, std::vector<uint32_t>& list );
			
			//ȡ����uid����������?
			bool set_blocklist(uint32_t uid, std::vector<uint32_t>& list);
			bool get_blocklist(uint32_t uid, std::vector<uint32_t>& list );
			
			//wuji end

			//取自定义头像
			bool get_custom_head_photo(const uint32_t uid, std::string & custom_head_photo);

			bool GetCBuddyList(uint32_t uid, std::vector<protocol::im::CBuddyListNoRemark>& list);

			/*
			* 获取用于登陆时传送的好友的remark(备注)列表
			*/
			bool GetCBuddyListRemark(uint32_t uid, std::map<uint32_t, std::string>& mapBidToRemark);

			bool GetCFolderList(uint32_t uid, std::vector<protocol::im::CFolderList>& list);
		};
		
	}
}

#endif
