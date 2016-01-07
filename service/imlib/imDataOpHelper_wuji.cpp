#include "imDataOpHelper.h"
#include "imDataOpHelper_wuji.h"
#include "core/sox/logger.h"
#include "server_common/server-lib/PublicFun.h"

#include <mysql++/mysql++.h>
#include <mysql++/custom.h>

using namespace server::mc;
using namespace std;
using namespace protocol::im;
using namespace server::im;
using namespace sox;
using namespace protocol;
using namespace mysqlpp;
using namespace protocol::uinfo;
using namespace server;
//using namespace server::oracledb::im;

//wuji start
void _get_blacklist_key(uint32_t uid, std::string& key);
void _get_blocklist_key(uint32_t uid, std::string& key);
void _get_user_sessioninfo_key(uint32_t uid, std::string& key);
void _get_user_iminfo_key(uint32_t uid, std::string& key);
void _get_user_immaxjifen_key(uint32_t uid, std::string& key);

void _get_user_imcheckcode_key(uint32_t uid, std::string& key);
void _get_user_im_at_game_key(uint32_t uid, std::string& key);
void _get_user_im_at_chanel_key(uint32_t uid, std::string& key);
void _get_user_im_extra_data_key(uint32_t uid, std::string& key);

//wuji end


/*
* 获取用于登陆时传送的好友列表
*/
bool imDataOpHelper_wuji::GetCBuddyList(uint32_t uid, std::vector<CBuddyListNoRemark>& list)
{
	log(Debug, "[imDataOpHelper_wuji::GetCBuddyList]: uid = %d", uid);

	bool bIsOk=true;
	std::ostringstream ostr;


	Connection* conn = GetMetaFactory()->GetConnection("buddylist", uid);
	Query query = conn->query();
	query << "select bid,pid from buddylist where uid=%0 and n_isdel=0";
	query.parse();
	Result res = query.store(uid);

	if (res)
	{
		log(Info, "[imDataOpHelper_wuji::GetCBuddyList]: uid:%u, size:%u", uid, res.size());
		for(Result::iterator it = res.begin(); it != res.end(); ++it)
		{
			CBuddyListNoRemark info;
			info.m_uBid = (*it).at(0);
			info.m_uPid = (*it).at(1);
			//info.m_strRemark = (const char*)(*it).at(2) ? (const char*)(*it).at(2) : "";
			//info.m_imId = 0;

			ostr << "," << info.m_uBid;
			
			list.push_back(info);
		}
	}
	else
	{
		bIsOk = false;
		log(Error, "[imDataOpHelper_wuji::GetCBuddyList] sql error: %s, %s", query.preview(uid).data(), query.error().data());
	}


	/* 写入imID */
	/*
	if (bIsOk)
	{
		query.reset();
		query << "select uid, imid from imid where uid in (%0)";
		query.parse();
		std::string tmpStr = ostr.str();
		res = query.store(tmpStr.substr(1));

		std::map<int, int> tmpMap;
		if (res && !res.empty()) 
		{
			for(Result::iterator it = res.begin(); it != res.end(); ++it)
			{
				tmpMap[(*it).at(0)] = (*it).at(1);
			}
		}
		else
		{
			bIsOk = false;
			log(Error, "[imDataOpHelper_wuji::GetCBuddyList] sql error: %s, %s", query.preview(uid).data(), query.error().data());	
		}

		std::vector<CBuddyList>::iterator iter1;
		std::vector<CBuddyList>::iterator iter_end1 = list.end();
		std::map<int, int>::iterator iter2;
		std::map<int, int>::iterator iter_end2 = tmpMap.end();
		for (iter1 = list.begin(); iter1 != iter_end1; iter1++)
		{
			iter2 = tmpMap.find(iter1->m_uBid);			
			if (iter2 != iter_end2)
			{
				iter1->m_imId = iter2->second;
			}
			else
			{
				log(Error, "[imDataOpHelper_wuji::GetCBuddyList] imid not found, uid is %ld!", uid);
			}
		}
	}*/

	if (bIsOk)
	{
		log(Debug, "[imDataOpHelper_wuji::GetCBuddyList] uid: %ld ok", uid);	
	}
	else
	{
		log(Debug, "[imDataOpHelper_wuji::GetCBuddyList] uid: %ld failed", uid);
	}


	return bIsOk;
}

/*
* 获取用于登陆时传送的好友的remark(备注)列表
*/
bool imDataOpHelper_wuji::GetCBuddyListRemark(uint32_t uid, std::map<uint32_t, std::string>& mapBidToRemark)
{
	log(Debug, "[imDataOpHelper_wuji::GetCBuddyListRemark]: uid = %d", uid);

	bool bIsOk=true;

	Connection* conn = GetMetaFactory()->GetConnection("buddylist", uid);
	Query query = conn->query();
	query << "select bid, remark  from buddylist where uid = %0 and n_isdel=0";
	query.parse();
	Result res = query.store(uid);	
	if(res)
	{
		mapBidToRemark.clear();
		for(Result::iterator it = res.begin(); it != res.end(); ++it)
		{
			uint32_t bid = (*it).at(0);
			std::string strRemark = (const char*)(*it).at(1) ? (const char*)(*it).at(1) : "";
			mapBidToRemark.insert(make_pair(bid, strRemark));
		}
	}
	else
	{
		bIsOk = false;
		log(Error, "[imDataOpHelper_wuji::GetCBuddyListRemark] sql error: %s, %s", query.preview(uid).data(), query.error().data());
	}

	if (bIsOk)
	{
		log(Debug, "[imDataOpHelper_wuji::GetCBuddyListRemark] uid: %ld ok", uid);	
	}
	else
	{
		log(Debug, "[imDataOpHelper_wuji::GetCBuddyListRemark] uid: %ld failed", uid);
	}


	return bIsOk;
}


/*
* 获取用于登录时传送的分组列表
*/
bool imDataOpHelper_wuji::GetCFolderList(uint32_t uid, std::vector<CFolderList>& list)
{
	log(Debug, "[imDataOpHelper_wuji::GetCFolderList]: uid = %d", uid);

	bool bIsOk=true;

	Connection* conn = getSqlConnFactory()->getConnection();
	Query query = conn->query();
	query << "select fid, remark from folderlist where uid = %0";
	query.parse();
	Result res = query.store(uid);	
	if(res )
	{
		for(Result::iterator it = res.begin(); it != res.end(); ++it)
		{
			CFolderList info;
			info.m_uFid = (*it).at(0);
			info.m_strRemark = (const char*)(*it).at(1) ? (const char*)(*it).at(1) : "";

			list.push_back(info);
		}
	}
	else
	{
		bIsOk = false;
		log(Error, "[imDataOpHelper_wuji::GetCFolderList] sql error: %s, %s", query.preview(uid).data(), query.error().data());
	}


	if (bIsOk)
	{
		log(Debug, "[imDataOpHelper_wuji::GetCFolderList] uid: %ld ok", uid);	
	}
	else
	{
		log(Debug, "[imDataOpHelper_wuji::GetCFolderList] uid: %ld failed", uid);
	}


	return bIsOk;
}


//wuji start


bool imDataOpHelper_wuji::get_user_im_imid(uint32_t uId, uint32_t &uImid)
{
	bool bIsOk=true;
	log(Debug, "[imDataOpHelper_wuji::get_user_im_imid] uid: %ld start", uId);

	Connection* conn = getSqlConnFactory()->getConnection();
	Query query = conn->query();
	query << "select imid from imid where uid = %0";
	query.parse();
	Result res = query.store(uId);
	if(res && (!res.empty()))
	{
		uImid=res.at(0).at(0);
	}
	else 
	{
		bIsOk=false;
		if(!res)
			log(Error, "[imDataOpHelper_wuji::get_user_im_imid]sql error: %s, %s", query.preview(uId).data(), query.error().data());
		else if (res.empty())
		{
			log(Info, "[imDataOpHelper_wuji::get_user_im_imid] uid: %ld not found", uId);	
		}
	}

	if (bIsOk)
	{
		log(Debug, "[imDataOpHelper_wuji::get_user_im_imid] uid: %ld ok", uId);	
	}
	else
	{
		log(Debug, "[imDataOpHelper_wuji::get_user_im_imid] uid: %ld failed", uId);
	}

	return bIsOk;
}


bool imDataOpHelper_wuji::get_user_im_imid_set(const std::vector<uint32_t> &vtBid, std::map<uint32_t, uint32_t>& m_mapBidToImId)
{
	bool bIsOk=true;
	m_mapBidToImId.clear();
	if(vtBid.size() == 0)
	{
		return true;
	}

	Connection* conn = getSqlConnFactory()->getConnection();
	Query query = conn->query();

	stringstream ss;
	for(uint32_t i = 0; i < vtBid.size(); ++i)
	{
		ss << vtBid[i] << ",";
	}
	std::string tmp = ss.str();
	tmp = tmp.substr(0, tmp.size() -1);
	query << "select uid, imid from imid where uid in ( "<< tmp<< ")";
	query.parse();	
	Result res = query.store();
	if(res )
	{
		for(Result::iterator it = res.begin(); it != res.end(); ++it)
		{
			uint32_t bid = (*it).at(0);
			uint32_t imid = (*it).at(1);
			m_mapBidToImId.insert(make_pair(bid, imid));
		}
	}
	else
	{
		bIsOk = false;
		log(Error, "[imDataOpHelper_wuji::get_user_im_imid_set] sql error: %s, %s", query.preview().data(), query.error().data());
	}

	return bIsOk;
}

/*
		+---------------+------------------+------+-----+----------+----------------+
		| Field         | Type             | Null | Key | Default  | Extra          |
		+---------------+------------------+------+-----+----------+----------------+
		| id            | bigint(20)       | NO   | PRI | NULL     | auto_increment | 
		| account       | varchar(255)     | NO   | MUL | NULL     |                | 
		| passwd        | varchar(255)     | YES  |     |          |                | 
		| nick          | varchar(255)     | YES  |     |          |                | 
		| sex           | tinyint(4)       | YES  |     | 0        |                | 
		| birthday      | int(11)          | YES  |     | 19800101 |                | 
		| area          | int(11)          | YES  |     | -1       |                | 
		| province      | int(11)          | YES  |     | -1       |                | 
		| city          | int(11)          | YES  |     | -1       |                | 
		| validate      | tinyint(4)       | YES  |     | 0        |                | 
		| resume        | text             | YES  |     | NULL     |                | 
		| last_time     | datetime         | YES  | MUL | NULL     |                | 
		| last_ip       | int(11) unsigned | YES  |     | NULL     |                | 
		| version       | int(11)          | YES  |     | 0        |                | 
		| register_time | datetime         | YES  |     | NULL     |                | 
		| jifen         | int(11)          | YES  |     | 0        |                | 
		| sign          | varchar(255)     | YES  |     |          |                | 
		| passport      | varchar(255)     | YES  | MUL |          |                | 
		| intro         | varchar(255)     | YES  |     |          |                | 
+---------------+------------------+------+-----+----------+----------------+

  select id,account,passwd,nick,sex,birthday,area,province,city,validate,resume,last_time,last_ip,version,register_time
  jifen,sign,passport,intro from uinfo

  select nick, jifen, sign, sex, version,birthday,area,province,city,validate,resume,register_time,intro

  mysql> desc imid;
		  +-------------+----------+------+-----+---------------------+----------------+
		  | Field       | Type     | Null | Key | Default             | Extra          |
		  +-------------+----------+------+-----+---------------------+----------------+
		  | id          | int(11)  | NO   | PRI | NULL                | auto_increment | 
		  | imid        | int(11)  | NO   | UNI | NULL                |                | 
		  | uid         | int(11)  | NO   | UNI | NULL                |                | 
		  | create_time | datetime | NO   |     | 0000-00-00 00:00:00 |                | 
		  +-------------+----------+------+-----+---------------------+----------------+
		  4 rows in set (0.00 sec)
		  
			mysql> desc tbl_im_uinfo;
			+-------------+---------------+------+-----+---------+----------------+
			| Field       | Type          | Null | Key | Default | Extra          |
			+-------------+---------------+------+-----+---------+----------------+
			| nid         | bigint(20)    | NO   | PRI | NULL    | auto_increment | 
			| uid         | decimal(20,0) | NO   | UNI | NULL    |                | 
			| nimageindex | smallint(6)   | NO   |     | -1      |                | 
			| ncelue      | smallint(6)   | NO   |     | 1       |                | 
			+-------------+---------------+------+-----+---------+----------------+
	4 rows in set (0.00 sec)

*/

/*
bool imDataOpHelper_wuji::get_userinfo_db(uint32_t uId, protocol::im::SUserinfo1& info)
{
	bool bIsOk=true;
	log(Debug, "[imDataOpHelper_wuji::get_userinfo_db] uId: %ld start", uId);
//wuji start

	CObjUserInfo objUinfo;
	bIsOk=m_pobjUinfo->GetUinfoAll(uId,objUinfo);
	if(bIsOk)
	{
		info.m_uId=objUinfo.m_uId;
		info.m_strNickname=objUinfo.m_strNickname;
		info.m_sex=objUinfo.m_sex;
		info.m_uBirthDay=objUinfo.m_uBirthDay;
		info.m_uArea=objUinfo.m_uArea;
		info.m_uProvince=objUinfo.m_uProvince;
		info.m_uCity=objUinfo.m_uCity;

		info.m_strResume=objUinfo.m_strResume;
		info.m_uVersion=objUinfo.m_uVersion;
		info.m_uJiFen=objUinfo.m_uJiFen;
		

		info.m_strSign=objUinfo.m_strSign;
		info.m_strIntro=objUinfo.m_strIntro;

		
		Connection* conn = getSqlConnFactory()->getConnection();
		Query query = conn->query();
		query.reset();
		query << "select imid from imid where uid = %0";
		query.parse();
		Result res = query.store(uId);
		if (res && !res.empty())
		{
			info.m_uImId=res.at(0).at(0);
		}


//wuji imapp 分离后不能取头像
// 		query.reset();
// 		query << "select nimageindex from tbl_im_uinfo where uid = %0";
// 		query.parse();
// 		res = query.store(uId);
// 		if (res && !res.empty())
// 		{
// 			info.m_uHeadPhoto=res.at(0).at(0);
// 		}

		log(Debug, "[imDataOpHelper_wuji::get_userinfo_db] uId: %ld ok", uId);
	}
	else 
	{
		log(Info, "[imDataOpHelper_wuji::get_userinfo_db] uId: %ld failed", uId);
	}

	//mysql old code
// 	Connection* conn = getSqlConnFactory()->getConnectionByName(UINFO_SLAVE_DB);
// 	Query query = conn->query();
// 	query << "select nick,sex,birthday,area,province,city,resume,version,jifen, sign, intro from uinfo where id = %0";
// 	query.parse();
// 	Result res = query.store(uId);
// 	if(res && (!res.empty()))
// 	{
// 			info.m_uId=uId;
// 			info.m_strNickname=(string)res.at(0).at(0);
// 			info.m_sex=(int)(res.at(0).at(1)) ? male : female;//数据库里��?代表男，0代表��?
//			info.m_uBirthDay=res.at(0).at(2);
// 			info.m_uArea=res.at(0).at(3);
// 			info.m_uProvince=res.at(0).at(4);
// 			info.m_uCity=res.at(0).at(5);
// 
// 			info.m_strResume=(string)res.at(0).at(6);
// 			info.m_uVersion=res.at(0).at(7);
// 			info.m_uJiFen=res.at(0).at(8);
// 			info.m_uJiFen=info.m_uJiFen/60;
// 
// 			info.m_strSign=(string)res.at(0).at(9);
// 			info.m_strIntro=(string)res.at(0).at(10);
// 
// 			conn = getSqlConnFactory()->getConnection();
// 			query = conn->query();
// 			query.reset();
// 			query << "select imid from imid where uid = %0";
// 			query.parse();
// 			res = query.store(uId);
// 			if (res && !res.empty())
// 			{
// 				info.m_uImId=res.at(0).at(0);
// 			}
// 
// 			query.reset();
// 			query << "select nimageindex from tbl_im_uinfo where uid = %0";
// 			query.parse();
// 			res = query.store(uId);
// 			if (res && !res.empty())
// 			{
// 				info.m_uHeadPhoto=res.at(0).at(0);
// 			}
// 	}
// 	else 
// 	{
// 		log(Debug, "[imDataOpHelper_wuji::get_userinfo_db] uId: %ld failed", uId);
// 		bIsOk=false;
// 		if(!res)
// 			log(Error, "[imDataOpHelper_wuji::get_userinfo_db]sql error: %s, %s", query.preview(uId).data(), query.error().data());
// 	}

//wuji end


	return bIsOk;
}
*/

bool imDataOpHelper_wuji::get_user_im_extra_data(uint32_t uId,string &strData)
{
	bool bIsOk=true;
	//log(Debug, "[imDataOpHelper_wuji::get_user_im_extra_data] uId: %ld start", uId);
		
#ifdef WUJI_USE_MEMCACHED
	PMC_IMByString objMemInfo;
	Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);
	
	string key;
	_get_user_im_extra_data_key(uId, key);
	
	bIsOk=mc->mcGet(key, objMemInfo);
	if (bIsOk)
	{
		strData=objMemInfo.m_strData;
	}
#else

	{
		Connection* conn = getSqlConnFactory()->getConnection();
		Query query = conn->query();
		//query << "select v_data from tbl_mem_extradata where uid = %0";
		query << "select v_data from tbl_mem_chanel where uid = %0";
		query.parse();
		Result res = query.store(uId);
		if(res && (!res.empty()))
		{
			strData=(string)res.at(0).at(0);
		}
		else 
		{
			bIsOk=false;
			if(!res)
				log(Error, "[imDataOpHelper_wuji::get_user_im_extra_data]sql error: %s, %s", query.preview(uId).data(), query.error().data());
			else if (res.empty())
			{
				//log(Debug, "[imDataOpHelper_wuji::get_user_im_extra_data] uId: %ld not found", uId);	
			}

		}
		
	
	}


	
		


	


#endif
/*
if (bIsOk)
	{
		log(Debug, "[imDataOpHelper_wuji::get_user_im_extra_data] uId: %ld ok", uId);	
	}
	else
	{
	log(Debug, "[imDataOpHelper_wuji::get_user_im_extra_data] uId: %ld failed", uId);
	}
	
*/	
	return bIsOk;
	
}

// bool imDataOpHelper_wuji::set_user_im_extra_data(uint32_t uId,string &strData)
// {
// 	bool bIsOk=true;
// 	log(Debug, "[imDataOpHelper_wuji::set_user_im_extra_data] uId: %ld ok", uId);	
// 
// #ifdef WUJI_USE_MEMCACHED
// 	PMC_IMByString objMemInfo;
// 	objMemInfo.m_strData=strData;
// 	Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);
// 	
// 	string key;
// 	_get_user_im_extra_data_key(uId, key);
// 	bIsOk=mc->mcSet(key, objMemInfo);
// #else
// 
// 	{
// 		Connection* conn = getSqlConnFactory()->getConnection();
// 		Query query = conn->query();
// 		query << "insert into tbl_mem_extradata(uid,v_data) values(%0,%1q)  on duplicate key update v_data=%2q";
// 		query.parse();
// 		ResNSel res= query.execute(uId,strData,strData);
// 		if(!res)
// 		{
// 			bIsOk=false;
// 			log(Error, "[imDataOpHelper_wuji::set_user_im_extra_data]sql error: %s, %s", query.preview(uId,strData).data(), query.error().data());
// 		}
// 		
// 	}
// 
// #endif
// 	if (bIsOk)
// 		log(Debug, "[imDataOpHelper_wuji::set_user_im_extra_data] uId: %ld ok", uId);	
// 	else
// 		log(Debug, "[imDataOpHelper_wuji::set_user_im_extra_data] uId: %ld failed", uId);	
// 
// 	return bIsOk;
// 	
// }

// bool imDataOpHelper_wuji::del_user_im_extra_data(uint32_t uId)
// {
// 	bool bIsOk=true;
// 	log(Debug, "[imDataOpHelper_wuji::del_user_im_extra_data] uId: %ld ok", uId);	
// 
// #ifdef WUJI_USE_MEMCACHED
// string key;
// 	Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);
// 	
// 	_get_user_im_extra_data_key(uId, key);
// 	bIsOk=mc->mcDel(key);
// #else
// 	{
// 		Connection* conn = getSqlConnFactory()->getConnection();
// 		Query query = conn->query();
// 		query << "delete from tbl_mem_extradata where uid=%0";
// 		query.parse();
// 		ResNSel res= query.execute(uId);
// 		if(!res)
// 		{
// 			bIsOk=false;
// 			log(Error, "[imDataOpHelper_wuji::del_user_im_extra_data]sql error: %s, %s", query.preview(uId).data(), query.error().data());
// 		}
// 		
// 	}
// 
// 	
// #endif
// 	if (bIsOk)
// 		log(Debug, "[imDataOpHelper_wuji::del_user_im_extra_data] uId: %ld ok", uId);	
// 	else
// 		log(Debug, "[imDataOpHelper_wuji::del_user_im_extra_data] uId: %ld failed", uId);	
// 
// 	return bIsOk;
// }
// 

void imDataOpHelper_wuji::get_user_im_chanel_headphoto_game(uint32_t uId,uint32_t &uHeadPhoto,uint32_t& uChanelId,uint32_t &uGameId,string& strGameData)
{

	

	if(!get_user_im_at_chanel(uId,uChanelId))
	{
		uChanelId=uint32_t(-1);
	}
	
	if (!get_user_im_at_game(uId,uGameId,strGameData))
	{
		uGameId=uint32_t(-1);
	}

	uHeadPhoto=uint32_t(-1);
	
	/* wuji imapp 分离后不能使用
	Connection* conn = getSqlConnFactory()->getConnection();
	Query query = conn->query();
	query.reset();
	query << "select nimageindex from tbl_im_uinfo where uid=%0";
	query.parse();
	Result res = query.store(uId);
	if(!res || res.empty())
	{
		uHeadPhoto=uint32_t(-1);
	}
	else
	{
		uHeadPhoto=res.at(0).at(0);
	}
	*/



}



bool imDataOpHelper_wuji::get_buddylist_im_at_game(uint32_t uid, vector<uint32_t>& vecBid,vector<uint32_t>& vecGameId,vector<string>& vecStrData)
{
	bool bIsOk=true;
	log(Debug, "[imDataOpHelper_wuji::get_buddylist_im_at_game] uId: %ld start", uid);


	//select a.bid,ifnull(b.n_game,-1),ifnull(b.v_data,'')  from buddylist a left join tbl_mem_game b  on a.bid=b.uid where a.uid=18326613 ;
	vecBid.clear();
	vecGameId.clear();
	vecStrData.clear();

	Connection* conn = getSqlConnFactory()->getConnection();
	Query query = conn->query();
	query << "select a.bid,ifnull(b.n_game,-1),ifnull(b.v_data,'')  from buddylist a left join tbl_mem_game b  on a.bid=b.uid where a.uid=%0";
	query.parse();
	Result res = query.store(uid);
	if(res && (!res.empty()))
	{
		for(Result::iterator it = res.begin(); it != res.end(); ++it)
		{
			vecBid.push_back((*it).at(0));
			vecGameId.push_back((*it).at(1));
			vecStrData.push_back((string)(*it).at(2));

		}
	}
	else 
	{
		bIsOk=false;
		if(!res)
			log(Error, "[imDataOpHelper_wuji::get_buddylist_im_at_game]sql error: %s, %s", query.preview(uid).data(), query.error().data());
		else if (res.empty())
		{
			log(Debug, "[imDataOpHelper_wuji::get_buddylist_im_at_game] uId: %ld not found", uid);	
		}
	}


	if (bIsOk)
	{
		log(Debug, "[imDataOpHelper_wuji::get_buddylist_im_at_game] uId: %ld ok", uid);	
	}
	else
	{
		log(Debug, "[imDataOpHelper_wuji::get_buddylist_im_at_game] uId: %ld failed", uid);
	}

	return bIsOk;

}

bool imDataOpHelper_wuji::get_buddylist_im_at_chanel(uint32_t uid, vector<uint32_t>& vecBid,vector<uint32_t>& vecGameId,vector<string>& vecStrData)
{
	bool bIsOk=true;
	log(Debug, "[imDataOpHelper_wuji::get_buddylist_im_at_chanel] uId: %ld start", uid);

	//select a.bid,ifnull(b.n_game,-1),ifnull(b.v_data,'')  from buddylist a left join tbl_mem_game b  on a.bid=b.uid where a.uid=18326613 ;
	vecBid.clear();
	vecGameId.clear();
	vecStrData.clear();

	Connection* conn = getSqlConnFactory()->getConnection();
	Query query = conn->query();
	query << "select a.bid,ifnull(b.n_chanel,-1),ifnull(b.v_data,'') from buddylist a left join tbl_mem_chanel b  on a.bid=b.uid where a.uid=%0";
	query.parse();
	Result res = query.store(uid);
	if(res && (!res.empty()))
	{
		for(Result::iterator it = res.begin(); it != res.end(); ++it)
		{
			vecBid.push_back((*it).at(0));
			vecGameId.push_back((*it).at(1));
			vecStrData.push_back((string)(*it).at(2));
		}
	}
	else 
	{
		bIsOk=false;
		if(!res)
			log(Error, "[imDataOpHelper_wuji::get_buddylist_im_at_chanel]sql error: %s, %s", query.preview(uid).data(), query.error().data());
		else if (res.empty())
		{
			log(Debug, "[imDataOpHelper_wuji::get_buddylist_im_at_chanel] uId: %ld not found", uid);	
		}
	}


	if (bIsOk)
	{
		log(Debug, "[imDataOpHelper_wuji::get_buddylist_im_at_chanel] uId: %ld ok", uid);	
	}
	else
	{
		log(Debug, "[imDataOpHelper_wuji::get_buddylist_im_at_chanel] uId: %ld failed", uid);
	}

	return bIsOk;

}


bool imDataOpHelper_wuji::get_buddylist_im_at_extradata(uint32_t uid, vector<uint32_t>& vecBid,vector<string>& vecStrData)
{
	bool bIsOk=true;
	log(Debug, "[imDataOpHelper_wuji::get_buddylist_im_at_extradata] uId: %ld start", uid);

	//select a.bid,ifnull(b.n_game,-1),ifnull(b.v_data,'')  from buddylist a left join tbl_mem_game b  on a.bid=b.uid where a.uid=18326613 ;
	vecBid.clear();
	vecStrData.clear();

	Connection* conn = getSqlConnFactory()->getConnection();
	Query query = conn->query();
	query << "select a.bid,ifnull(b.v_data,'') from buddylist a left join tbl_mem_chanel b  on a.bid=b.uid where a.uid=%0";
	query.parse();
	Result res = query.store(uid);
	if(res && (!res.empty()))
	{
		for(Result::iterator it = res.begin(); it != res.end(); ++it)
		{
			vecBid.push_back((*it).at(0));
			vecStrData.push_back((string)(*it).at(1));
		}
	}
	else 
	{
		bIsOk=false;
		if(!res)
			log(Error, "[imDataOpHelper_wuji::get_buddylist_im_at_extradata]sql error: %s, %s", query.preview(uid).data(), query.error().data());
		else if (res.empty())
		{
			log(Debug, "[imDataOpHelper_wuji::get_buddylist_im_at_extradata] uId: %ld not found", uid);	
		}
	}


	if (bIsOk)
	{
		log(Debug, "[imDataOpHelper_wuji::get_buddylist_im_at_extradata] uId: %ld ok", uid);	
	}
	else
	{
		log(Debug, "[imDataOpHelper_wuji::get_buddylist_im_at_extradata] uId: %ld failed", uid);
	}

	return bIsOk;

}


bool imDataOpHelper_wuji::get_user_im_at_game(uint32_t uid, uint32_t& uGameId,string& strData)
{
	bool bIsOk=true;
	//log(Debug, "[imDataOpHelper_wuji::get_user_im_at_game] uId: %ld start", uid);

#ifdef WUJI_USE_MEMCACHED

	PMC_IMByUINT32 objMemInfo;
	Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);

	string key;
	_get_user_im_at_game_key(uid, key);
	
	bIsOk=mc->mcGet(key, objMemInfo);
	if (bIsOk)
	{
		uGameId=objMemInfo.m_nId;
	}
#else

	{
		Connection* conn = getSqlConnFactory()->getConnection();
		Query query = conn->query();
		query << "select n_game,v_data from tbl_mem_game where uid = %0";
		query.parse();
		Result res = query.store(uid);
		if(res && (!res.empty()))
		{
			uGameId=res.at(0).at(0);
			strData=(string)res.at(0).at(1);
		}
		else 
		{
			bIsOk=false;
			if(!res)
				log(Error, "[imDataOpHelper_wuji::get_user_im_at_game]sql error: %s, %s", query.preview(uid).data(), query.error().data());
			else if (res.empty())
			{
				log(Debug, "[imDataOpHelper_wuji::get_user_im_at_game] uId: %ld not found", uid);	
			}
		}
		
		
	}
	
#endif

/*
if (bIsOk)
	{
		log(Debug, "[imDataOpHelper_wuji::get_user_im_at_game] uId: %ld ok", uid);	
	}
	else
	{
		log(Debug, "[imDataOpHelper_wuji::get_user_im_at_game] uId: %ld failed", uid);
	}
*/

	return bIsOk;
	
}

bool imDataOpHelper_wuji::get_user_im_at_game(const vector<uint32_t> &vecBid,vector<protocol::im::CObjList1> & m_vGame)
{
	bool bIsOk=true;

	log (Debug,"[imDataOpHelper_wuji::get_user_im_at_game] start select buddy gameId");
	Connection*conn = getSqlConnFactory()->getConnection();
	Query query = conn ->query();

	stringstream sr;
	string str = "";
	uint32_t i;
	protocol::im::CObjList1 objItem;
	set<uint32_t> setIdxUids;
	
	for (i=0;i<vecBid.size();i++)
	{
		sr<<vecBid[i]<<",";
	}
	str = sr.str();
	if(str.size() == 0)
	{
		return true;
	}
	str = str.substr(0, str.size()-1);//把stream流里的类加载到字符串str中,把vecBid里的id号转换为in查询里的字符串，类似 in(1,2,3,4)


	query<<"select uid,n_game,v_data from tbl_mem_game where uid in ("+str+")";
	query.parse();
	log(Debug, "[imDataOpHelper_wuji::get_user_im_at_game]: sql=%s", query.preview().data());
	Result res = query.store();  //执行查询
	Result::iterator iter=res.begin();

	if (res)
	{
		for(iter=res.begin();iter!=res.end();iter++)
		{
			objItem.m_uId = (*iter).at(0);
			objItem.m_uValueId = (*iter).at(1);
			objItem.m_strData = (string)(*iter).at(2);
			m_vGame.push_back(objItem);

			setIdxUids.insert(objItem.m_uId);

		}

		if (setIdxUids.size() < vecBid.size())
		{
			for(vector<uint32_t>::const_iterator iterUid = vecBid.begin();iterUid!=vecBid.end();
				iterUid++)
			{
				if (setIdxUids.count(*iterUid) !=1)
				{
					objItem.m_uId = *iterUid;
					objItem.m_uValueId = -1;
					objItem.m_strData = "";
					m_vGame.push_back(objItem);	
				}
			}
		}

		log(Debug, "[imDataOpHelper_wuji::get_user_im_at_game]  finally ok");
	}
	else 
	{
		bIsOk = false;
		if(!res)
		  log(Error, "[imDataOpHelper_wuji::get_user_im_at_game]sql error: %s, %s", query.preview().data(), query.error().data());
	}
		
	return bIsOk;


}


bool imDataOpHelper_wuji::del_user_im_at_game(uint32_t uid)
{
	bool bIsOk=true;
	log(Debug, "[imDataOpHelper_wuji::del_user_im_at_game] uid: %ld start", uid);	
#ifdef WUJI_USE_MEMCACHED

	string key;
	Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);

	_get_user_im_at_game_key(uid, key);
	bIsOk=mc->mcDel(key);
#else
	{
		Connection* conn = getSqlConnFactory()->getConnection();
		Query query = conn->query();
		query << "delete from tbl_mem_game where uid=%0";
		query.parse();
		ResNSel res= query.execute(uid);
		if(!res)
		{
			bIsOk=false;
			log(Error, "[imDataOpHelper_wuji::del_user_im_at_game]sql error: %s, %s", query.preview(uid).data(), query.error().data());
		}
		
	}

#endif

	if (bIsOk)
		log(Debug, "[imDataOpHelper_wuji::del_user_im_at_game] uid: %ld ok", uid);	
	else
		log(Debug, "[imDataOpHelper_wuji::del_user_im_at_game] uid: %ld failed", uid);	

	return bIsOk;
}

bool imDataOpHelper_wuji::set_user_im_at_game(uint32_t uid,uint32_t uGameId,const string& strData)
{
	bool bIsOk=true;
		//log(Debug, "[imDataOpHelper_wuji::set_user_im_at_game] uid: %ld ok", uid);	
#ifdef WUJI_USE_MEMCACHED

	PMC_IMByUINT32 objMemInfo;
	objMemInfo.m_nId=uGameId;
	Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);

	string key;
	_get_user_im_at_game_key(uid, key);
	bIsOk=mc->mcSet(key, objMemInfo);
#else
	{
		Connection* conn = getSqlConnFactory()->getConnection();
		Query query = conn->query();
		query << "insert into tbl_mem_game(uid,n_game,v_data,tm_addtime) values(%0,%1,%2q,now())  on duplicate key update n_game=%3,v_data=%4q,tm_addtime=now()";
		query.parse();
		ResNSel res= query.execute(uid,uGameId,strData,uGameId,strData);
		if(!res)
		{
			bIsOk=false;
			log(Error, "[imDataOpHelper_wuji::set_user_im_at_game]sql error: %s, %s", query.preview(uid,uGameId,strData,uGameId,strData).data(), query.error().data());
		}
		
	}

#endif

/*
if (bIsOk)
		log(Debug, "[imDataOpHelper_wuji::set_user_im_at_game] uid: %ld ok", uid);	
	else
		log(Debug, "[imDataOpHelper_wuji::set_user_im_at_game] uid: %ld failed", uid);	
*/



	return bIsOk;
	
}

bool imDataOpHelper_wuji::update_user_im_at_game(uint32_t uid,uint32_t uGameId)
{
	bool bIsOk=true;
	//log(Debug, "[imDataOpHelper_wuji::update_user_im_at_game] uid: %ld ok", uid);	
	{
		Connection* conn = getSqlConnFactory()->getConnection();
		Query query = conn->query();
//		query << "insert into tbl_mem_game(uid,n_game,v_data,tm_addtime) values(%0,%1,%2q,now())  on duplicate key update n_game=%3,v_data=%4q,tm_addtime=now()";
		query << "update tbl_mem_game set n_game=%0 where uid=%1";
		query.parse();
		ResNSel res= query.execute(uGameId,uid);
		if(!res)
		{
			bIsOk=false;
			log(Error, "[imDataOpHelper_wuji::update_user_im_at_game]sql error: %s, %s", query.preview(uGameId,uid).data(), query.error().data());
		}

	}



/*
if (bIsOk)
		log(Debug, "[imDataOpHelper_wuji::update_user_im_at_game] uid: %ld ok", uid);	
	else
		log(Debug, "[imDataOpHelper_wuji::update_user_im_at_game] uid: %ld failed", uid);	
*/



	return bIsOk;

}

bool imDataOpHelper_wuji::set_user_im_at_chanel(uint32_t uid, uint32_t  uChanelId,const string &strData)
{
	bool bIsOk=true;

	//log(Debug, "[imDataOpHelper_wuji::set_user_im_at_chanel] uid: %ld ok", uid);

	
	

#ifdef WUJI_USE_MEMCACHED
	PMC_IMByUINT32 objMemInfo;
	objMemInfo.m_nId=uChanelId;
	Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);

	string key;
	_get_user_im_at_chanel_key(uid, key);
	bIsOk=mc->mcSet(key, objMemInfo);
#else
	{
		if (uChanelId==((uint32_t)-1))
		{
			bIsOk=del_user_im_at_chanel(uid);
		}
		else
		{
			Connection* conn = getSqlConnFactory()->getConnection();
			Query query = conn->query();
			query << "insert into tbl_mem_chanel(uid,n_chanel,v_data) values(%0,%1,%2q)  on duplicate key update n_chanel=%3,v_data=%4q";
			query.parse();
			ResNSel res= query.execute(uid,uChanelId,strData,uChanelId,strData);
			if(!res)
			{
				bIsOk=false;
				log(Error, "[imDataOpHelper_wuji::set_user_im_at_chanel]sql error: %s, %s", query.preview(uid,uChanelId,uChanelId,strData,strData).data(), query.error().data());
			}
		}
		
	}

#endif

/*
if (bIsOk)
		log(Debug, "[imDataOpHelper_wuji::set_user_im_at_chanel] uid: %ld ok", uid);	
	else
		log(Debug, "[imDataOpHelper_wuji::set_user_im_at_chanel] uid: %ld failed", uid);	
*/

	return bIsOk;
	
}

//�����û�����Ƶ��
bool imDataOpHelper_wuji::get_user_im_at_chanel(uint32_t uid, uint32_t &uChanelId)
{
	bool bIsOk=true;
	//log(Debug, "[imDataOpHelper_wuji::get_user_im_at_chanel] uid: %ld start", uid);

#ifdef WUJI_USE_MEMCACHED
	PMC_IMByUINT32 objMemInfo;
	
	Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);
	string key;
	_get_user_im_at_chanel_key(uid, key);
	
	bIsOk=mc->mcGet(key, objMemInfo);
	if (bIsOk)
	{
		uChanelId=objMemInfo.m_nId;
	}
#else
	{
		Connection* conn = getSqlConnFactory()->getConnection();
		Query query = conn->query();
		query << "select n_chanel from tbl_mem_chanel where uid = %0";
		query.parse();
		Result res = query.store(uid);
		if(res && (!res.empty()))
		{
			uChanelId=res.at(0).at(0);
		}
		else 
		{
			bIsOk=false;
			if(!res)
				log(Error, "[imDataOpHelper_wuji::get_user_im_at_chanel]sql error: %s, %s", query.preview(uid).data(), query.error().data());
			else if (res.empty())
			{
				log(Debug, "[imDataOpHelper_wuji::get_user_im_at_chanel] uid: %ld not found", uid);	
			}
		}
		
		
	}
	
#endif

/*
if (bIsOk)
	{
		log(Debug, "[imDataOpHelper_wuji::get_user_im_at_chanel] uid: %ld ok", uid);	
	}
	else
	{
		log(Debug, "[imDataOpHelper_wuji::get_user_im_at_chanel] uid: %ld failed", uid);
	}
*/



	return bIsOk;
	
}

bool imDataOpHelper_wuji::get_user_im_at_chanel(uint32_t uid, uint32_t &uChanelId,string &strData)
{
	bool bIsOk=true;
	//log(Debug, "[imDataOpHelper_wuji::get_user_im_at_chanel] uid: %ld start", uid);

	{
		Connection* conn = getSqlConnFactory()->getConnection();
		Query query = conn->query();
		query << "select n_chanel,v_data from tbl_mem_chanel where uid = %0";
		query.parse();
		Result res = query.store(uid);
		if(res && (!res.empty()))
		{
			uChanelId=res.at(0).at(0);
			strData=(string)(res.at(0).at(1));
		}
		else 
		{
			bIsOk=false;
			if(!res)
				log(Error, "[imDataOpHelper_wuji::get_user_im_at_chanel]sql error: %s, %s", query.preview(uid).data(), query.error().data());
			else if (res.empty())
			{
				log(Debug, "[imDataOpHelper_wuji::get_user_im_at_chanel] uid: %ld not found", uid);	
			}
		}


	}

/*

	if (bIsOk)
	{
		log(Debug, "[imDataOpHelper_wuji::get_user_im_at_chanel] uid: %ld ok", uid);	
	}
	else
	{
		log(Debug, "[imDataOpHelper_wuji::get_user_im_at_chanel] uid: %ld failed", uid);
	}

*/

	return bIsOk;

}



bool imDataOpHelper_wuji::get_user_im_at_chanel(const vector<uint32_t> &vecBid,vector<protocol::im::CObjList1> & m_vChanel)
{
	log (Debug,"[imDataOpHelper_wuji::get_user_im_at_channel] start select buddy ChanelId");
	bool bIsOk = true;

	Connection*conn = getSqlConnFactory()->getConnection();
	Query query = conn ->query();
	
	stringstream sr;
	string str;
	uint32_t i;

	for (i=0;i<vecBid.size()-1;i++)
	{
		sr<<vecBid[i]<<",";
	}
	sr >>str;          //把stream流里的类加载到字符串str中,把vecBid里的id号转换为in查询里的字符串，类似 in(1,2,3,4)
	if(str.size() == 0)
	{
		return true;
	}
	str = str.substr(0, str.size() - 1);


	query<<"select uid,n_chanel,v_data from tbl_mem_chanel where uid in ("+str+")";
	query.parse();
	log(Debug, "[imDataOpHelper_wuji::get_user_im_at_chanel]: sql=%s", query.preview().data());
	Result res = query.store();  //执行查询
	Result::iterator iter=res.begin();
	protocol::im::CObjList1 objItem;
	set<uint32_t> setIdxUids;


	if (res)         //查询正确!
	{
		for(iter=res.begin();iter!=res.end();iter++)
		{
			objItem.m_uId = (*iter).at(0);
			objItem.m_uValueId = (*iter).at(1);
			objItem.m_strData =(string)(*iter).at(2);
			m_vChanel.push_back(objItem);

			setIdxUids.insert(objItem.m_uId);         //待查找使用
		}


		if (setIdxUids.size() < vecBid.size())
		{
			for(vector<uint32_t>::const_iterator iterUid = vecBid.begin();iterUid!=vecBid.end();
				iterUid++)
			{
				if (setIdxUids.count(*iterUid) !=1)
				{
					objItem.m_uId = *iterUid;
					objItem.m_uValueId = -1;
					objItem.m_strData = "";
					m_vChanel.push_back(objItem);	
				}
			}
		}
		
		log(Debug, "[imDataOpHelper_wuji::get_user_im_at_chanel] finally ok");	

	}
	else 
	{
		bIsOk = false;
		if(!res)
			log(Error, "[imDataOpHelper_wuji::get_user_im_at_chanel]sql error: %s, %s", query.preview().data(), query.error().data());
	}


	return bIsOk;
}



bool imDataOpHelper_wuji::del_user_im_at_chanel(uint32_t uid)
{
	
	bool bIsOk=true;
	//log(Debug, "[imDataOpHelper_wuji::del_user_im_at_chanel] uid: %ld start", uid);	
#ifdef WUJI_USE_MEMCACHED

	string key;
	Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);

	_get_user_im_at_chanel_key(uid, key);
	bIsOk=mc->mcDel(key);
#else
	{

		Connection* conn = getSqlConnFactory()->getConnection();
		Query query = conn->query();
		query << "delete from tbl_mem_chanel where uid=%0";
		query.parse();
		ResNSel res= query.execute(uid);
		if(!res)
		{
			bIsOk=false;
			log(Error, "[imDataOpHelper_wuji::del_user_im_at_chanel]sql error: %s, %s", query.preview(uid).data(), query.error().data());
		}
		
	}

#endif

/*
if (bIsOk)
		log(Debug, "[imDataOpHelper_wuji::del_user_im_at_chanel] uid: %ld ok", uid);	
	else
		log(Debug, "[imDataOpHelper_wuji::del_user_im_at_chanel] uid: %ld failed", uid);	
*/

	return bIsOk;
}


// bool imDataOpHelper_wuji::get_imuser_checkcode(uint32_t uid, CObjUserSessionCheckCode& objInfo)
// {
// 	bool bIsOk=true;
// 	log(Debug, "[imDataOpHelper_wuji::get_imuser_checkcode] uid: %ld start", uid);
// #ifdef WUJI_USE_MEMCACHED
// 	PMC_IMUserCheckCode objMemInfo;
// 	Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);
// 
// 	string key;
// 	_get_user_imcheckcode_key(uid, key);
// 	
// 	bIsOk=mc->mcGet(key, objMemInfo);
// 	if (bIsOk)
// 	{
// 		objInfo.m_strCode=objMemInfo.m_strCode;
// 		objInfo.m_uCreateTime=objMemInfo.m_uCreateTime;
// 	}
// #else
// 	{
// 		Connection* conn = getSqlConnFactory()->getConnection();
// 		Query query = conn->query();
// 		query << "select v_checkcode,n_createtime from tbl_mem_checkcode where uid = %0";
// 		query.parse();
// 		Result res = query.store(uid);
// 		if(res && (!res.empty()))
// 		{
// 			objInfo.m_strCode=(string)res.at(0).at(0);
// 			objInfo.m_uCreateTime=res.at(0).at(1);
// 			
// 		}
// 		else 
// 		{
// 			bIsOk=false;
// 			if(!res)
// 				log(Error, "[imDataOpHelper_wuji::get_imuser_checkcode]sql error: %s, %s", query.preview(uid).data(), query.error().data());
// 			else if (res.empty())
// 			{
// 				log(Debug, "[imDataOpHelper_wuji::get_imuser_checkcode] uid: %ld not found", uid);	
// 			}
// 		}
// 		
// 		
// 	}
// 
// #endif
// 
// 	if (bIsOk)
// 	{
// 		log(Debug, "[imDataOpHelper_wuji::get_imuser_checkcode] uid: %ld ok", uid);	
// 	}
// 	else
// 	{
// 		log(Debug, "[imDataOpHelper_wuji::get_imuser_checkcode] uid: %ld failed", uid);
// 	}
// 
// 
// 
// 	return bIsOk;
// 	
// }

// bool imDataOpHelper_wuji::set_imuser_checkcode(uint32_t uid, const CObjUserSessionCheckCode& objInfo)
// {
// 	bool bIsOk=true;
// log(Debug, "[imDataOpHelper_wuji::set_imuser_checkcode] uid: %ld ok", uid);	
// #ifdef WUJI_USE_MEMCACHED
// 
// 	PMC_IMUserCheckCode objMemInfo;
// 	objMemInfo.m_strCode=objInfo.m_strCode;
// 	objMemInfo.m_uCreateTime=objInfo.m_uCreateTime;
// 	Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);
// 
// 	string key;
// 	_get_user_imcheckcode_key(uid, key);
// 	bIsOk=mc->mcSet(key, objMemInfo);
// #else
// 	{
// 		Connection* conn = getSqlConnFactory()->getConnection();
// 		Query query = conn->query();
// 		query << "insert into tbl_mem_checkcode(uid,v_checkcode,n_createtime) values(%0,%1q,%2)  on duplicate key update v_checkcode=%3q,n_createtime=%4";
// 		query.parse();
// 		ResNSel res= query.execute(uid,objInfo.m_strCode,objInfo.m_uCreateTime,objInfo.m_strCode,objInfo.m_uCreateTime);
// 		if(!res)
// 		{
// 			bIsOk=false;
// 			log(Error, "[imDataOpHelper_wuji::set_imuser_checkcode]sql error: %s, %s", query.preview(uid,objInfo.m_strCode,objInfo.m_uCreateTime,objInfo.m_strCode,objInfo.m_uCreateTime).data(), query.error().data());
// 		}
// 		
// 	}
// 
// #endif
// 
// 	if (bIsOk)
// 		log(Debug, "[imDataOpHelper_wuji::set_imuser_checkcode] uid: %ld ok", uid);	
// 	else
// 		log(Debug, "[imDataOpHelper_wuji::set_imuser_checkcode] uid: %ld failed", uid);	
// 
// 	
// 	return bIsOk;
// 	
// }
// bool imDataOpHelper_wuji::del_imuser_checkcode(uint32_t uid)
// {
// 	
// 	bool bIsOk=true;
// 	log(Debug, "[imDataOpHelper_wuji::del_imuser_checkcode] uid: %ld ok", uid);	
// #ifdef WUJI_USE_MEMCACHED
// 
// 	string key;
// 	Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);
// 
// 	_get_user_imcheckcode_key(uid, key);
// 	bIsOk=mc->mcDel(key);
// #else
// 	{
// 		Connection* conn = getSqlConnFactory()->getConnection();
// 		Query query = conn->query();
// 		query << "delete from tbl_mem_checkcode where uid=%0";
// 		query.parse();
// 		ResNSel res= query.execute(uid);
// 		if(!res)
// 		{
// 			bIsOk=false;
// 			log(Error, "[imDataOpHelper_wuji::del_imuser_checkcode]sql error: %s, %s", query.preview(uid).data(), query.error().data());
// 		}
// 		
// 	}
// #endif
// 	if (bIsOk)
// 		log(Debug, "[imDataOpHelper_wuji::del_imuser_checkcode] uid: %ld ok", uid);	
// 	else
// 		log(Debug, "[imDataOpHelper_wuji::del_imuser_checkcode] uid: %ld failed", uid);	
// 	
// 
// 
// 	return bIsOk;
// }


bool imDataOpHelper_wuji::get_imuser_maxjifen(uint32_t uid, uint32_t &uJiFenMax)
{
	
		bool bIsOk=true;
		PMC_IMUserMaxJiFen objMemInfo;
//����Ҫ�ŵ����棬��Ϊû�б�Ҫ
// 		string key;
// 		_get_user_immaxjifen_key(uid, key);
// 		
// 		bIsOk=mcHelper::mcGet(key, objMemInfo, mc);
// 		if (bIsOk)
// 		{
// 			log(Debug, "mc_get_user_maxjifen: memcache hit! uid: %d", uid);
// 		}
// 		else
		{
			log(Debug, "mc_get_user_maxjifen: memcache NOT hit! uid: %d", uid);
			Connection* conn = getSqlConnFactory()->getConnection();
			Query query = conn->query();
			query << "select nmaxjifen from tbl_maxjifen where uid = %0";
			query.parse();
			Result res = query.store(uid);	
			if(res && (res.num_rows() != 0) )
			{
				
				objMemInfo.m_uMaxJiFen=res.at(0).at(0);
				//add into memcache
//				bIsOk=mcHelper::mcSet(key, objMemInfo, mc);
				
			}
			else
			{
				log(Error, "sql error: %s, %s", query.preview(uid).data(), query.error().data());
				bIsOk=false;
				goto bad1;
			}
			
		}
		uJiFenMax=objMemInfo.m_uMaxJiFen;
		
		
bad1:
		return bIsOk;
		

}

bool imDataOpHelper_wuji::del_imuser_maxjifen(uint32_t uid)
{
		
		bool bIsOk=true;
#ifdef WUJI_USE_MEMCACHED
	string key;
		Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);
		
		_get_user_immaxjifen_key(uid, key);
		bIsOk=mc->mcDel(key);
#else

#endif
		return bIsOk;

}
bool imDataOpHelper_wuji::del_user_iminfo(uint32_t uid)
{
	
	bool bIsOk=true;
#ifdef WUJI_USE_MEMCACHED

	string key;
	Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);
	_get_user_iminfo_key(uid, key);
	bIsOk=mc->mcDel(key);
#else

#endif
	return bIsOk;
}

bool imDataOpHelper_wuji::get_user_iminfo(uint32_t uid, CObjUserInfoAboutIm& objInfo)
{

	bool bIsOk=true;
 	PMC_IMUserInfo  objMemInfo;

// 	��ʱ�����Ƿ��뵽���棬��Ϊû�б�Ҫ
// 	string key;
// 	_get_user_iminfo_key(uid, key);
// 	
// 	bIsOk=mcHelper::mcGet(key, objMemInfo, mc);
// 	if (bIsOk)
// 	{
// 		log(Debug, "mc_get_user_iminfo: memcache hit! uid: %d", uid);
// 	}
// 	else
	{
		log(Debug, "mc_get_user_iminfo: memcache NOT hit! uid: %d", uid);
		Connection* conn = getSqlConnFactory()->getConnection();
		Query query = conn->query();
		query << "select ncelue from tbl_im_uinfo where uid = %0";
		query.parse();
		Result res = query.store(uid);	
		if(res && (res.num_rows() != 0) )
		{

			objMemInfo.m_uCeLie=res.at(0).at(0);
			//add into memcache
//			bIsOk=mcHelper::mcSet(key, objMemInfo, mc);
			
		}
		else
		{
			if (!res)
			{
				log(Error, "sql error: %s, %s", query.preview(uid).data(), query.error().data());
			}
			
			bIsOk=false;
			goto bad1;
		}
		
	}
	objInfo.m_uCeLie=objMemInfo.m_uCeLie;


bad1:
	return bIsOk;

}

// bool imDataOpHelper_wuji::get_user_sessioninfo(uint32_t uid, CObjUserSessionInfo& objInfo)
// {
// 	bool bIsOk=true;
// 	log(Debug, "[imDataOpHelper_wuji::get_user_sessioninfo] uid: %ld start", uid);
// #ifdef WUJI_USE_MEMCACHED
// 
// 	PMC_LoginInfo objMemInfo;
// 	Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);
// 	
// 	string key;
// 	_get_user_sessioninfo_key(uid, key);
// 
// 	bIsOk=mc->mcGet(key, objMemInfo);
// 	if (bIsOk)
// 	{
// 		objInfo.m_uSearchCount=objMemInfo.m_uSearchCount;
// 	}
// #else
// 	{
// 		Connection* conn = getSqlConnFactory()->getConnection();
// 		Query query = conn->query();
// 		query << "select n_searchcount from tbl_mem_searchcount where uid = %0";
// 		query.parse();
// 		Result res = query.store(uid);
// 		if(res && (!res.empty()))
// 		{
// 			objInfo.m_uSearchCount=res.at(0).at(0);
// 		}
// 		else 
// 		{
// 			bIsOk=false;
// 			if(!res)
// 				log(Error, "[imDataOpHelper_wuji::get_user_sessioninfo]sql error: %s, %s", query.preview(uid).data(), query.error().data());
// 			else if (res.empty())
// 			{
// 				log(Debug, "[imDataOpHelper_wuji::get_user_sessioninfo] uid: %ld not found", uid);	
// 			}
// 
// 		}
// 		
// 		
// 	}
// 
// #endif
// 	if (bIsOk)
// 	{
// 		log(Debug, "[imDataOpHelper_wuji::get_user_sessioninfo] uid: %ld ok", uid);	
// 	}
// 	else
// 	{
// 		log(Debug, "[imDataOpHelper_wuji::get_user_sessioninfo] uid: %ld failed", uid);
// 	}
// 
// 
// 	
// 	return bIsOk;
// 
// }

// bool imDataOpHelper_wuji::set_user_sessioninfo(uint32_t uid, const CObjUserSessionInfo& objInfo)
// {
// 		bool bIsOk=true;
// 		log(Debug, "[imDataOpHelper_wuji::set_user_sessioninfo] uid: %ld ok", uid);	
// #ifdef WUJI_USE_MEMCACHED
// 
// 		PMC_LoginInfo objMemInfo;
// 		objMemInfo.m_uSearchCount=objInfo.m_uSearchCount;
// 		IMemcachedFactory *pFactory=getMemcachedFactory();
// 		Memcached* mc = pFactory->getMemcached(IMDB_MEMCACHED);
// 	
// 		
// 		string key;
// 		_get_user_sessioninfo_key(uid, key);
// 		bIsOk=mc->mcSet(key, objMemInfo);
// #else
// 	{
// 			Connection* conn = getSqlConnFactory()->getConnection();
// 			Query query = conn->query();
// 			query << "insert into tbl_mem_searchcount(uid,n_searchcount) values(%0,%1)  on duplicate key update n_searchcount=%2";
// 			query.parse();
// 			ResNSel res= query.execute(uid,objInfo.m_uSearchCount,objInfo.m_uSearchCount);
// 			if(!res)
// 			{
// 				bIsOk=false;
// 				log(Error, "[imDataOpHelper_wuji::set_user_sessioninfo]sql error: %s, %s", query.preview(uid,objInfo.m_uSearchCount,objInfo.m_uSearchCount).data(), query.error().data());
// 			}
// 			
// 	}
// 
// #endif
// 
// 	if (bIsOk)
// 		log(Debug, "[imDataOpHelper_wuji::set_user_sessioninfo] uid: %ld ok", uid);	
// 	else
// 		log(Debug, "[imDataOpHelper_wuji::set_user_sessioninfo] uid: %ld failed", uid);	
// 	
// 
// 		return bIsOk;
// 
// }
// bool imDataOpHelper_wuji::del_user_sessioninfo(uint32_t uid)
// {
// 	
// 	bool bIsOk=true;
// 	log(Debug, "[imDataOpHelper_wuji::del_user_sessioninfo] uid: %ld start", uid);	
// #ifdef WUJI_USE_MEMCACHED
// 
// 	string key;
// 	Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);
// 	
// 	_get_user_sessioninfo_key(uid, key);
// 	bIsOk=mc->mcDel(key);
// #else
// 	{
// 		Connection* conn = getSqlConnFactory()->getConnection();
// 		Query query = conn->query();
// 		query << "delete from tbl_mem_searchcount where uid=%0";
// 		query.parse();
// 		ResNSel res= query.execute(uid);
// 		if(!res)
// 		{
// 			bIsOk=false;
// 			log(Error, "[imDataOpHelper_wuji::del_user_sessioninfo]sql error: %s, %s", query.preview(uid).data(), query.error().data());
// 		}
// 		
// 	}
// 
// #endif
// 	if (bIsOk)
// 		log(Debug, "[imDataOpHelper_wuji::del_user_sessioninfo] uid: %ld ok", uid);	
// 	else
// 		log(Debug, "[imDataOpHelper_wuji::del_user_sessioninfo] uid: %ld failed", uid);	
// 	
// 
// 
// 	return bIsOk;
// }










bool imDataOpHelper_wuji::del_blacklist(uint32_t uid)
{
	
	bool bIsOk=true;
#ifdef WUJI_USE_MEMCACHED
	string key;
	Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);
	
	_get_blacklist_key(uid, key);
	bIsOk=mc->mcDel(key);
#else
#endif
	return bIsOk;
}

bool imDataOpHelper_wuji::del_blocklist(uint32_t uid)
{

	bool bIsOk=true;
#ifdef WUJI_USE_MEMCACHED
	string key;
	Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);
	
	_get_blocklist_key(uid, key);
	bIsOk=mc->mcDel(key);
#else
#endif
	return bIsOk;
}

			
bool imDataOpHelper_wuji::IsInBlackList(uint32_t uid,uint32_t bid/*bid�Ƿ���uid��������*/,bool& bIsExist/*true������*/)
{
	bool bIsOk=true;
#ifdef WUJI_USE_MEMCACHED
	vector<uint32_t> buddylist;
	
	bIsOk=get_blacklist(uid, buddylist);
	if (bIsOk)
	{
		if (binary_search(buddylist.begin(), buddylist.end(), bid))
		{
			bIsExist=true;
			
			//��
		}
		else 
		{
			
			bIsExist=false;
			//����
		}
	}
#else
	{
		Connection* conn = getSqlConnFactory()->getConnection();
		Query query = conn->query();
		query << "select nblackuid from tbl_blacklist where uid = %0 and nblackuid=%1";
		query.parse();
		Result res = query.store(uid,bid);	
		if(res && !res.empty())
		{
			bIsExist=true;
		}
		else
		{
			if (!res)
			{
				log(Error, "[imDataOpHelper_wuji::IsInBlackList]sql error: %s, %s", query.preview(uid,bid).data(), query.error().data());
			}
			bIsOk=false;
			bIsExist=false;
			goto bad1;
		}
	}
	
#endif
bad1:
	return bIsOk;
	
	
}

bool imDataOpHelper_wuji::IsInBlockList(uint32_t uid,uint32_t bid/*bid�Ƿ���uid��������*/,bool& bIsExist/*true������*/)
{
	bool bIsOk=true;
#ifdef WUJI_USE_MEMCACHED
	vector<uint32_t> buddylist;
	bIsOk=get_blocklist(uid, buddylist);
	if (bIsOk)
	{
		if (binary_search(buddylist.begin(), buddylist.end(), bid))
		{
			bIsExist=true;
			
			//��
		}
		else 
		{
			
			bIsExist=false;
			//����
		}
	}
#else
	{
		Connection* conn = getSqlConnFactory()->getConnection();
		Query query = conn->query();
		query << "select nblockuid from tbl_blocklist where uid = %0 and nblockuid=%1";
		query.parse();
		Result res = query.store(uid,bid);	
		if(res && !res.empty())
		{
			bIsExist=true;
		}
		else
		{
			if (!res)
			{
				log(Error, "[imDataOpHelper_wuji::IsInBlockList]sql error: %s, %s", query.preview(uid,bid).data(), query.error().data());
			}
			bIsOk=false;
			bIsExist=false;
			goto bad1;
		}
		
		
	}
#endif
	
bad1:
	return bIsOk;
	
}

bool imDataOpHelper_wuji::IsInBuddyList(uint32_t uid,uint32_t bid/*bid�Ƿ���uid��������*/,bool& bIsExist/*true������*/)
{
	bool bIsOk=true;
	
#ifdef WUJI_USE_MEMCACHED
	vector<uint32_t> buddylist;
	get_buddylist(uid, buddylist);
	if (buddylist.size()>0)
	{
		if (binary_search(buddylist.begin(), buddylist.end(), bid))
		{
			bIsExist=true;
			
			//��
		}
		else 
		{
			bIsExist=false;
			//����
		}
	}
	else
		bIsOk=false;
#else
	{
		Connection* conn = GetMetaFactory()->GetConnection("buddylist", uid);
		Query query = conn->query();
		query << "select bid from buddylist where uid = %0 and bid=%1 and n_isdel=0";
		query.parse();
		Result res = query.store(uid,bid);	
		if(res && !res.empty())
		{
			bIsExist=true;
		}
		else
			bIsExist=false;
		
	}
#endif

	return bIsOk;
	
}

bool imDataOpHelper_wuji::set_blocklist(uint32_t uid, vector<uint32_t>& blist)
{
	bool bIsOk=true;
#ifdef WUJI_USE_MEMCACHED

	PMC_uid_Buddylist list;
	list.sim_buddylist = blist;
	Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);
		
	string key;
	_get_blocklist_key(uid, key);
	
	bIsOk=mc->mcSet(key, list);
#else

#endif
	return bIsOk;
}

bool  imDataOpHelper_wuji::get_blocklist(uint32_t uid, vector<uint32_t>& buddylist)
{
	bool bIsOk=true;

	buddylist.clear();
	PMC_uid_Buddylist list;
#ifdef WUJI_USE_MEMCACHED
	
	Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);
	
	string key;
	_get_blocklist_key(uid, key);
//wuji debugs	
	if (mc->mcGet(key, list))
	{
		log(Debug, "mc_get_uid_blocklist: memcache hit! uid: %d", uid);
	}
	else
	{
		log(Debug, "mc_get_uid_blocklist: memcache NOT hit! uid: %d", uid);
		Connection* conn = getSqlConnFactory()->getConnection();
		Query query = conn->query();
		query << "select nblockuid from tbl_blocklist where uid = %0 order by nblockuid";
		query.parse();
		Result res = query.store(uid);	
		if(res)
		{
			for(Result::iterator it = res.begin(); it != res.end(); ++it)
				list.sim_buddylist.push_back((*it).at(0));
			
			//add into memcache
			bIsOk=mc->mcSet(key, list);

		}
		else
		{
			log(Error, "sql error: %s, %s", query.preview(uid).data(), query.error().data());
			bIsOk=false;
			goto bad1;
		}
		
	}
#else
	{
		Connection* conn = getSqlConnFactory()->getConnection();
		Query query = conn->query();
		query << "select nblockuid from tbl_blocklist where uid = %0";
		query.parse();
		Result res = query.store(uid);	
		if(res)
		{
			for(Result::iterator it = res.begin(); it != res.end(); ++it)
				list.sim_buddylist.push_back((*it).at(0));
			
		}
		else
		{
			log(Error, "sql error: %s, %s", query.preview(uid).data(), query.error().data());
			bIsOk=false;
			goto bad1;
		}
		
	}

#endif
	buddylist = list.sim_buddylist;
bad1:

	return bIsOk;
}

bool imDataOpHelper_wuji::set_blacklist(uint32_t uid, vector<uint32_t>& blist)
{
	bool bIsOk=true;
#ifdef WUJI_USE_MEMCACHED
	PMC_uid_Buddylist list;
	list.sim_buddylist = blist;
	Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);
	
	string key;
	_get_blacklist_key(uid, key);
	
	bIsOk=mc->mcSet(key, list);
#else

#endif
	return bIsOk;
}

bool imDataOpHelper_wuji::get_blacklist(uint32_t uid, vector<uint32_t>& buddylist)
{
	bool bIsOk=true;

	buddylist.clear();
	PMC_uid_Buddylist list;
#ifdef WUJI_USE_MEMCACHED
	Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);
	
	string key;
	_get_blacklist_key(uid, key);
	//wuji debug
	if (mc->mcGet(key, list))
	{
		log(Debug, "mc_get_uid_blacklist: memcache hit! uid: %d", uid);
	}
	else
	{
		log(Debug, "mc_get_uid_blacklist: memcache NOT hit! uid: %d", uid);
		Connection* conn = getSqlConnFactory()->getConnection();
		Query query = conn->query();
		query << "select nblackuid from tbl_blacklist where uid = %0 order by nblackuid";
		query.parse();
		Result res = query.store(uid);	
		if(res)
		{
			for(Result::iterator it = res.begin(); it != res.end(); ++it)
				list.sim_buddylist.push_back((*it).at(0));
			
			//add into memcache
			bIsOk=mc->mcSet(key, list);
		}
		else
		{
			log(Error, "sql error: %s, %s", query.preview(uid).data(), query.error().data());
			bIsOk=false;
			goto bad1;
		}
	}
#else
	{
		Connection* conn = getSqlConnFactory()->getConnection();
		Query query = conn->query();
		query << "select nblackuid from tbl_blacklist where uid = %0";
		query.parse();
		Result res = query.store(uid);	
		if(res)
		{
			for(Result::iterator it = res.begin(); it != res.end(); ++it)
				list.sim_buddylist.push_back((*it).at(0));
			
		}
		else
		{
			log(Error, "sql error: %s, %s", query.preview(uid).data(), query.error().data());
			bIsOk=false;
			goto bad1;
		}
	}

#endif
	buddylist = list.sim_buddylist;
bad1:
	return bIsOk;
}

//wuji end



//wuji start
#define BLACKLIST_KEY_SUFFIX "BLACKLIST"
#define BLOCKLIST_KEY_SUFFIX "BLOCKLIST"
#define USER_SESSIONINFO_KEY_SUFFIX "IM_USER_SESSIONINFO"
#define USER_IMINFO_KEY_SUFFIX "IM_USER_IMINFO"
#define USER_IMMAXJIFEN_KEY_SUFFIX "IM_USER_IMMAXJIFEN"
#define USER_IMCHECKCODE_KEY_SUFFIX "IM_USER_IMMAXJIFEN"
#define USER_IMATGAME_KEY_SUFFIX  "IM_USER_IM_AT_GAME"
#define USER_IMATCHANEL_KEY_SUFFIX  "IM_USER_IM_AT_CHANEL"
#define USER_IM_EXTRADATA_KEY_SUFFIX  "IM_USER_IM_EXTRA_DATA"

//wuji end

//wuji start
void _get_user_immaxjifen_key(uint32_t uid, std::string& key)
{
	char buf[50];
	sprintf(buf, "%d:%s", uid, USER_IMMAXJIFEN_KEY_SUFFIX);
	key = buf;
}

void _get_user_iminfo_key(uint32_t uid, std::string& key)
{
	char buf[50];
	sprintf(buf, "%d:%s", uid, USER_IMINFO_KEY_SUFFIX);
	key = buf;
}

void _get_blacklist_key(uint32_t uid, string& key)
{
	char buf[50];
	sprintf(buf, "%d:%s", uid, BLACKLIST_KEY_SUFFIX);
	key = buf;
}

void _get_blocklist_key(uint32_t uid, string& key)
{
	char buf[50];
	sprintf(buf, "%d:%s", uid, BLOCKLIST_KEY_SUFFIX);
	key = buf;
}

void _get_user_im_at_game_key(uint32_t uid, std::string& key)
{
	char buf[50];
	sprintf(buf, "%d:%s", uid, USER_IMATGAME_KEY_SUFFIX);
	key = buf;
}

void _get_user_im_at_chanel_key(uint32_t uid, std::string& key)
{
	char buf[50];
	sprintf(buf, "%d:%s", uid, USER_IMATCHANEL_KEY_SUFFIX);
	key = buf;
}

void _get_user_im_extra_data_key(uint32_t uid, std::string& key)
{
	char buf[50];
	sprintf(buf, "%d:%s", uid, USER_IM_EXTRADATA_KEY_SUFFIX);
	key = buf;
}


void _get_user_imcheckcode_key(uint32_t uid, std::string& key)
{
	char buf[50];
	sprintf(buf, "%d:%s", uid, USER_IMCHECKCODE_KEY_SUFFIX);
	key = buf;
}
void _get_user_sessioninfo_key(uint32_t uid, string& key)
{
	char buf[50];
	sprintf(buf, "%d:%s", uid, USER_SESSIONINFO_KEY_SUFFIX);
	key = buf;
}
//wuji end

//取自定义头像
bool imDataOpHelper_wuji::get_custom_head_photo(const uint32_t uid, std::string & custom_head_photo)
{
	Connection* conn = getSqlConnFactory()->getConnection();
	if (conn == NULL)
		return false;

	Query query = conn->query();
	query << "select path from tbl_custom_logo where uid = %0";
	query.parse();

	//log(Debug, "[imDataOpHelper_wuji::get_custom_head_photo] sql : %s", query.preview(uid).data());

	Result res2 = query.store(uid);
	if (res2.empty())
	{
		//log(Info, "[imDataOpHelper_wuji::get_custom_head_photo] sql error: %s, %s", query.preview(uid).data(), query.error().data());
		log(Info, "[imDataOpHelper_wuji::get_custom_head_photo] get CustomLogoPath failed uid:%u",uid);
		return false;
	}

	custom_head_photo = (string)res2.at(0).at(0);
	//log(Debug, "[imDataOpHelper_wuji::get_custom_head_photo] get CustomLogoPath success.  m_strCustomHeadPhotoPath=%s", custom_head_photo.c_str());

	return true;
}



