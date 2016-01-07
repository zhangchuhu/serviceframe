#include "imDataOpHelper.h"
#include "core/sox/logger.h"
#include "server_common/server-lib/PublicFun.h"
#include "server_common/helper/server_const.h"

#include <mysql++/mysql++.h>

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


/*need on memcache
#define BUDDYLIST_KEY_SUFFIX "BUDDYLIST"
#define SIMPLE_BUDDYLIST_KEY_SUFFIX "SIMPLE_BUDDYLIST"
#define ONLINE_ST_KEY_SUFFIX "ONLINE_ST"
#define SIMPLE_UINFO_KEY_SUFFIX "SIMPLE_UINFO"


void _get_buddylist_key(uint32_t uid, string& key){
	char buf[50];
	sprintf(buf, "%d:%s", uid, BUDDYLIST_KEY_SUFFIX);
	key = buf;
}

void _get_simple_buddylist_key(uint32_t uid, string& key){
	char buf[50];
	sprintf(buf, "%d:%s", uid, SIMPLE_BUDDYLIST_KEY_SUFFIX);
	key = buf;
}

void _get_status_key(uint32_t uid, string& key){
	char buf[50];
	sprintf(buf, "%d:%s", uid, ONLINE_ST_KEY_SUFFIX);
	key = buf;
}

void _get_simple_uinfo_key(uint32_t uid, string& key){
	char buf[50];
	sprintf(buf, "%d:%s", uid, SIMPLE_UINFO_KEY_SUFFIX);
	key = buf;
}



typedef std::pair<std::set<uint32_t> *, std::map<uint32_t, protocol::im::SUserinfo> *> blist_infomap_t;
typedef std::pair<std::set<uint32_t> *,  std::vector<uint32_t> *> online_blist_t;
typedef std::pair<std::set<uint32_t> *,  std::vector<protocol::im::Budid_St> *> buddy_pair_t;

typedef memcached_return (*mcm_callback)(memcached_st *ptr, memcached_result_st *result, void *context);
typedef void (*key_method)(uint32_t uid, std::string& key);

memcached_return _get_not_offline_buddylist_cb(memcached_st *ptr, memcached_result_st *result, void *context);
memcached_return _buddylist_get_uinfo_cb(memcached_st *ptr, memcached_result_st *result, void *context);
memcached_return _bdlist_cb(memcached_st *ptr, memcached_result_st *result, void *context);*/



//set or get buddy list
/*
bool imDataOpHelper::set_buddylist(uint32_t uid, vector<uint32_t>& blist)
{
	log(Debug, "set_buddylist() uid : %d", uid);
	Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);
	PMC_uid_Buddylist list;
	list.sim_buddylist = blist;

	string key;
	_get_buddylist_key(uid, key);

	return mc->mcSet(key, list);
}*/

bool imDataOpHelper::get_buddylist(uint32_t uid, vector<uint32_t>& buddylist)
{
	log(Debug, "[imDataOpHelper::get_buddylist]: uid = %d", uid);

	//PMC_uid_Buddylist list;

	/*Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);
	buddylist.clear();

	string key;
	_get_buddylist_key(uid, key);

	if (mc->mcGet(key, list))
	{
		log(Debug, "mc_get_uid_buddylist: memcache hit! uid: %d", uid);
	}
	else
	{
		log(Debug, "mc_get_uid_buddylist: memcache NOT hit! uid: %d", uid);*/
		Connection* conn = GetMetaFactory()->GetConnection("buddylist", uid);
		if (conn == NULL)
			return false;

		Query query = conn->query();
		query << "select bid from buddylist where uid = %0 and n_isdel = 0";
		query.parse();
		Result res = query.store(uid);
		if (res)
		{
			for(Result::iterator it = res.begin(); it != res.end(); ++it)
			{
				buddylist.push_back( (*it).at(0) );
			}
			return true;
			//list.sim_buddylist.push_back((*it).at(0));

			//add into memcache
			//bIsOk=mc->mcSet(key, list);
			//bIsOk = true;
		}
//	}

	//buddylist = list.sim_buddylist;
	//return bIsOk;
	return false;
}

bool imDataOpHelper::GetBuddyList4Brocast(uint32_t uId, vector<uint32_t>& vecData)
{
	bool bIsOk=true;
	log(Debug, "[imDataOpHelper::GetBuddyList4Brocast] uid:%u start", uId);
	vecData.clear();
	Connection* conn = GetMetaFactory()->GetConnection("buddylist", uId);
	Query query = conn->query();
	query << "select bid from buddylist where uid = %0";//除了双方好友，也包括单方好友
	query.parse();
	Result res = query.store(uId);	
	if(res)
	{
		for(Result::iterator it = res.begin(); it != res.end(); ++it)
			vecData.push_back((*it).at(0));
		log(Debug, "[imDataOpHelper::GetBuddyList4Brocast] uid:%u ok", uId);
	}
	else 
	{
		if (!res)
			log(Debug, "[imDataOpHelper::GetBuddyList4Brocast] uid:%u failed", uId);
		else
			log(Debug, "[imDataOpHelper::GetBuddyList4Brocast] uid:%u ok", uId);
		bIsOk=false;
	}
	
	return bIsOk;
}

bool imDataOpHelper::GetNoRemarkBuddylist(uint32_t uId, vector<protocol::im::CBuddyListNoRemark>& vecData)
{
	bool bIsOk=true;
	Connection* conn;
	
	if ((conn = GetMetaFactory()->GetConnection("buddylist", uId)) == NULL)
		return false;

	Query query = conn->query();
	query << "select bid, pid from buddylist where uid = %0 and n_isdel=0";//除了双方好友
	query.parse();
	Result res = query.store(uId);	
	if(res)
	{
		for(Result::iterator it = res.begin(); it != res.end(); ++it)
		{
			vecData.push_back(CBuddyListNoRemark( (*it).at(0), (*it).at(1) ));
		}
		log(Debug, "[imDataOpHelper::GetNoRemarkBuddylist] uid(%u)-size(%u) ok", uId, vecData.size());
	}
	else 
	{
		if (!res)
			log(Debug, "[imDataOpHelper::GetNoRemarkBuddylist] uid:%u failed", uId);
		else
			log(Debug, "[imDataOpHelper::GetNoRemarkBuddylist] uid:%u ok", uId);
		bIsOk=false;
	}

	return bIsOk;
}

//get or set simple buddy list (simple buddy info just include : buddyid, pid, remark)
/*
bool imDataOpHelper::set_simple_buddylist(uint32_t uid, vector<SimpleBInfo>& blist)
{
	log(Debug, "set_simple_buddylist() uid: %d", uid);
	Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);
	PMC_Simple_Buddylist list;
	list.buddylist = blist;

	string key;
	_get_simple_buddylist_key(uid, key);

	return mc->mcSet(key, list);
}*/

bool imDataOpHelper::get_simple_buddylist(uint32_t uid, vector<SimpleBInfo>& buddylist)
{
	bool bIsOk=true;
	log(Debug, "[imDataOpHelper::get_simple_buddylist]: uid = %d", uid);
	PMC_Simple_Buddylist list;	

	/*Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);
	buddylist.clear();
	string key;
	_get_simple_buddylist_key(uid, key);

 	if (mc->mcGet(key, list)){
 		log(Debug, "mc_get_simple_buddylist: memcache hit! uid: %d", uid);
 	}
 	else
	{
		log(Debug, "mc_get_simple_buddylist: memcache NOT hit! uid: %d", uid);*/
		Connection* conn = GetMetaFactory()->GetConnection("buddylist", uid);
		Query query = conn->query();
		query << "select bid, pid, remark from buddylist where uid = %0 and n_isdel=0";
		query.parse();
		Result res = query.store(uid);	
		if(res)
		{
			for(Result::iterator it = res.begin(); it != res.end(); ++it){
				SimpleBInfo info;
				info.buddyid = (*it).at(0);
				info.pid = (*it).at(1);
				info.remark = (const char*)(*it).at(2) ? (const char*)(*it).at(2) : "";

				list.buddylist.push_back(info);
			}

			//add into memcache
			//bIsOk=mc->mcSet(key, list);
		}
	//}

	buddylist = list.buddylist;
	return bIsOk;
}

bool imDataOpHelper::get_simple_buddylist(uint32_t uid, vector<CSimpleBInfo1>& buddylist)
{
	bool bIsOk=true;
	log(Debug, "[imDataOpHelper::get_simple_buddylist]: uid = %d", uid);
	
	buddylist.clear();
	Connection* conn;
	if ((conn = GetMetaFactory()->GetConnection("buddylist", uid)) == NULL)
		return false;

	Query query = conn->query();
	query << "select bid,pid,remark from buddylist where uid=%0 and n_isdel=0";
	query.parse();
	Result res = query.store(uid);	
	if(res)
	{
		std::vector<uint32_t> bids;
		for(Result::iterator it = res.begin(); it != res.end(); ++it)
		{
			CSimpleBInfo1 info;
			info.buddyid = (*it).at(0);
			bids.push_back(info.buddyid);
			info.pid = (*it).at(1);
			info.remark = (const char*)(*it).at(2) ? (const char*)(*it).at(2) : "";
			buddylist.push_back(info);
		}
		
		if (bids.size() > 0)
		{
			Connection *conn2 = getSqlConnFactory()->getConnection();
			Query query2 = conn2->query();
			query2 << "select uid,imid from imid where uid in (";
			
			for (std::vector<uint32_t>::size_type i = 0, j = bids.size(); i < j; i++)
			{
				query2 << bids[i];
				if (i + 1 < j)
					query2 << ",";
			}
			query2 << ")";
			query2.parse();
			Result res2 = query2.store();
			if (res2)
			{
				for(Result::iterator it2 = res2.begin(); it2 != res2.end(); ++it2)
				{
					uint32_t _uid = (uint32_t) (*it2).at(0);
					uint32_t _imid = (uint32_t) (*it2).at(1);
					for(vector<CSimpleBInfo1>::iterator it3 = buddylist.begin(); it3 != buddylist.end(); ++it3)
					{
						if (_uid == (*it3).buddyid)
						{
							(*it3).m_uImid = _imid;
							break;
						}
					}
				}
			}
		}
	}
	else
		bIsOk=false;
	return bIsOk;
}


//set or get status
/*
void imDataOpHelper::set_status(uint32_t uid, E_IMSTATUS st, uint32_t linkd_id){
	Memcached* mc = getMemcachedFactory()->getMemcached(IMONLINE_MEMCACHED);
	PMC_OnlineStat onlinest;
	onlinest.st = st;
	onlinest.linkd_id = linkd_id;

	string key;
	_get_status_key(uid, key);

	mc->mcSet(key, onlinest);
}*/

void imDataOpHelper::get_status(uint32_t uid, E_IMSTATUS& st, uint32_t& linkd_id){
	PMC_OnlineStat onlinest;	

	/*Memcached* mc = getMemcachedFactory()->getMemcached(IMONLINE_MEMCACHED);
	string key;
	_get_status_key(uid, key);
	if (mc->mcGet(key, onlinest))
	{
		log(Debug, "mc_get_onlinest: memcache hit! uid: %d, imlinkd: %d, status: %d", uid, onlinest.linkd_id, onlinest.st);
	}
	else
	{*/
		Connection* conn = getSqlConnFactory()->getConnection();
		Query query = conn->query();
		query << "select imlinkd, status from imonline where uid = %0";
		query.parse();
		Result res = query.store(uid);
		if (res.empty()){
			onlinest.st = IMOFFLINE;
			onlinest.linkd_id = NONE_IMLINKD;
		}else{
            onlinest.linkd_id = (E_IMSTATUS)(int)(res.at(0).at(0));
			onlinest.st = (E_IMSTATUS)(int)(res.at(0).at(1));
		}

		log(Debug, "[imDataOpHelper::get_status]: uid: %d, from DB, imlinkd: %d, status: %d", uid, onlinest.linkd_id, onlinest.st);

		//add into memcache
		//mc->mcSet(key, onlinest);
	//}

	st = onlinest.st;
	linkd_id = onlinest.linkd_id;
}


//wuji start
//old code
//get not offline buddylist
// bool imDataOpHelper::get_not_offline_buddylist(uint32_t uid, vector<uint32_t>& onlinebuddies)
// {
// 	bool bIsOk=true;
// 	
// 	log(Debug, "[imDataOpHelper::get_not_offline_buddylist]: uid = %d", uid);
// 	onlinebuddies.clear();
// 	vector<uint32_t> buddylist;
// 
// 	get_buddylist(uid, buddylist);
// 
// 	//need on memcache
// 	//set<uint32_t> needdb_ids;
// 	//Memcached* mc = getMemcachedFactory()->getMemcached(IMONLINE_MEMCACHED);
// 	//online_blist_t pair = make_pair(&needdb_ids, &onlinebuddies);
// 	//copy(buddylist.begin(), buddylist.end(), inserter(needdb_ids, needdb_ids.begin()));
// 	//_multi_get(mc, buddylist, _get_status_key, _get_not_offline_buddylist_cb, (void*)&pair);
// 
// 	//if (needdb_ids.size()){
// 	if (buddylist.size())
// 	{
// 		//string ids = idcat(needdb_ids);
// 		string ids = idcat(buddylist);
// 		Connection* conn = getSqlConnFactory()->getConnection();
// 		Query query = conn->query();
// 		query << "select uid, imlinkd, status from imonline where uid in (" << ids << ")";
// 		log(Debug, "%s", query.preview().data());
// 		Result res = query.store();
// 		if (res){
// 			for(Result::iterator it = res.begin(); it != res.end(); ++it){
// 				uint32_t bid = (*it).at(0);
// 				onlinebuddies.push_back(bid);
// 
// 				//add into memcache
// 				//set_status(bid, (protocol::E_IMSTATUS)(int)(*it).at(2), (*it).at(1));
// 				//needdb_ids.erase(bid);
// 			}
// 		}
// 
// 		//??��??????user_online?��?
// 		/*for (set<uint32_t>::iterator it = needdb_ids.begin(); it != needdb_ids.end(); ++it){
// 			//add into memcache, NONE_IMLINKD stand for none linkd
// 			set_status(*it, IMOFFLINE, NONE_IMLINKD);
// 			needdb_ids.erase(*it);
// 		}*/
// 	}
// 	return bIsOk;
// 
// }
//wuji end

bool imDataOpHelper::get_not_offline_buddylist(uint32_t uid, vector<uint32_t>& onlinebuddies)
{
	bool bIsOk=true;
	log(Debug, "[imDataOpHelper::get_not_offline_buddylist]: uid = %d", uid);
	onlinebuddies.clear();
		Connection* conn = getSqlConnFactory()->getConnection();
		Query query = conn->query();//挑选那些在线好友
		//query << "select uid, imlinkd, status from imonline where uid in (" << ids << ")";
		//query << "select a.uid, a.imlinkd, a.status from imonline as a,buddylist as b where a.uid=b.bid and a.uid=%0";
		query << "select a.uid from imonline a,buddylist b where a.uid=b.bid and b.uid=%0";
		query.parse();
		Result res = query.store(uid);
		if (res)
		{
			for(Result::iterator it = res.begin(); it != res.end(); ++it)
			{
				uint32_t bid = (*it).at(0);
				onlinebuddies.push_back(bid);
			}
		}
		else
		{
			bIsOk=false;
		}

	
	return bIsOk;

}

bool imDataOpHelper::get_not_offline_buddylist_and_imlinkid(uint32_t uid, vector<uint32_t>& onlinebuddies, vector<uint32_t>& imlinkids)
{
	bool bIsOk=true;
	log(Debug, "[imDataOpHelper::get_not_offline_buddylist_and_imlinkid]: uid = %d", uid);
	onlinebuddies.clear();
	Connection* conn = getSqlConnFactory()->getConnection();
	Query query = conn->query();//挑选那些在线好友
	//query << "select uid, imlinkd, status from imonline where uid in (" << ids << ")";
	//query << "select a.uid, a.imlinkd, a.status from imonline as a,buddylist as b where a.uid=b.bid and a.uid=%0";
	query << "select a.uid, a.imlinkd from imonline a,buddylist b where a.uid=b.bid and b.uid=%0";
	query.parse();
	Result res = query.store(uid);
	if (res)
	{
		for(Result::iterator it = res.begin(); it != res.end(); ++it)
		{
			uint32_t bid = (*it).at(0);
			uint32_t imlinkid =  (*it).at(1);
			onlinebuddies.push_back(bid);
			imlinkids.push_back(imlinkid);
		}
	}
	else
	{
		bIsOk=false;
	}
	
	return bIsOk;
}

//get 'uid''s all buddies status
void imDataOpHelper::get_buddy_status(uint32_t uid, vector<Budid_St>& bid_st_vec){
	log(Debug, "[imDataOpHelper::get_buddy_status]: uid = %d", uid);
	bid_st_vec.clear();
	vector<uint32_t> buddylist;

	get_buddylist(uid, buddylist);

	buddylist_get_status(buddylist, bid_st_vec);
}


//whether uid is offline
bool imDataOpHelper::isoffline(uint32_t uid)
{
	log(Debug, "[imDataOpHelper::isoffline]: uid = %d", uid);
	uint32_t linkd_id;
	E_IMSTATUS st;

	get_status(uid, st, linkd_id);

	return (st == IMOFFLINE) ? true : false;
}

bool imDataOpHelper::getImLinkd(uint32_t uid, uint32_t& imlinkd)
{
	log(Debug, "[imDataOpHelper::getImLinkd]: uid = %d", uid);
	uint32_t linkd_id;
	E_IMSTATUS st;

	get_status(uid, st, linkd_id);
	imlinkd = linkd_id;
	
	return (st == IMOFFLINE) ? false : true;
}


//void imDataOpHelper::buddylist_get_uinfo(vector<uint32_t>& buddylist, map<uint32_t, SUserinfo>& uinfo_map)
//{
//	uinfo_map.clear();
//
//	bool bIsOk=true;
//	//need on memcache
//	/*set<uint32_t> needdb_id;
//	Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);
//	copy(buddylist.begin(), buddylist.end(), inserter(needdb_id, needdb_id.begin()));
//	blist_infomap_t pair = make_pair(&needdb_id, &uinfo_map);
//	_multi_get(mc, buddylist, _get_simple_uinfo_key, _buddylist_get_uinfo_cb, (void*)&pair);*/
//	//if (needdb_id.size())
//
////wuji start
//	vector<CObjUserInfo> vecObjUinfo;
//	if (buddylist.size())
//	{
////		bool CUInfoDaoImp::GetBuddyListUinfo(const vector<uint32_t>& vecBuddyList,vector<CObjUserInfo>& vecObjUinfo)
//		bIsOk=m_pobjUinfo->GetBuddyListUinfo(buddylist,vecObjUinfo);
//		if (bIsOk)
//		{
//			for (unsigned int i=0;i< vecObjUinfo.size();i++)
//			{
//				SUserinfo info;
//				info.bid = vecObjUinfo[i].m_uId;
//				info.nickname = vecObjUinfo[i].m_strNickname;
//				info.rank = vecObjUinfo[i].m_uJiFen;
//				info.sign = vecObjUinfo[i].m_strSign;
//				info.sex = vecObjUinfo[i].m_sex;
//				info.version = vecObjUinfo[i].m_uVersion;
//				uinfo_map[info.bid] = info;
//
//			}
//				//add into memcache
//				/*PMC_Simple_Uinfo mcinfo;
//				mcinfo.nickname = info.nickname;
//				mcinfo.rank = info.rank;
//				mcinfo.sign = info.sign;
//				mcinfo.sex = info.sex;
//				mcinfo.version = info.version;
//				set_simple_uinfo(info.bid, mcinfo);*/
//		}
//	}
//
//
//	//wuji start 2009.10.28
//	//旧版本代码
////status
//// 	vector<Budid_St> bid_st_vec;
//// 	buddylist_get_status(buddylist, bid_st_vec);//旧版本代码
//// 	//composite info and status
//// 	for (vector<Budid_St>::iterator it = bid_st_vec.begin(); it != bid_st_vec.end(); ++it)
//// 	{
//// 		uinfo_map[(*it).bid].stat = (*it).st;
//// 	}
//	//wuji end
//
//}



//wuji start
//为优化登录流程做了改动，取某个用户的所有好友的状态
void imDataOpHelper::buddylist_get_status(uint32_t uId, vector<Budid_St>& bid_st_vec)
{
		bid_st_vec.clear();
		Connection* conn = getSqlConnFactory()->getConnection();
		Query query = conn->query();
		query << "select a.uid, a.imlinkd, a.status from imonline a , buddylist b where b.uid=%0 and b.bid=a.uid";
		query.parse();
		//log(Debug, "%s", query.preview().data());
		Result res = query.store(uId);
		if (res)
		{
			for(Result::iterator it = res.begin(); it != res.end(); ++it)
			{
				uint32_t uid = (*it).at(0);
				Budid_St bid_st;
				bid_st.bid = uid;
				bid_st.st = (protocol::E_IMSTATUS)(int)(*it).at(2);
				bid_st_vec.push_back(bid_st);
				log(Debug, "[imDataOpHelper::buddylist_get_status]: from imonline tables uid:%d, status:%d", uid, bid_st.st);
			}
		}

}

 void imDataOpHelper::buddylist_get_status(vector<uint32_t>& buddylist, vector<Budid_St>& bid_st_vec)
 {
 	bid_st_vec.clear();
 
 	/*Memcached* mc = getMemcachedFactory()->getMemcached(IMONLINE_MEMCACHED);
 	set<uint32_t> needdb_id;
 	buddy_pair_t pair = make_pair(&needdb_id, &bid_st_vec);	
 	copy(buddylist.begin(), buddylist.end(), inserter(needdb_id, needdb_id.begin()));
 	_multi_get(mc, buddylist, _get_status_key, _bdlist_cb, (void*)&pair);
 	if (needdb_id.size()){*/
 
 	if (buddylist.size())
 	{
 		//string ids = idcat(needdb_id);
 		string ids = idcat(buddylist);
 		Connection* conn = getSqlConnFactory()->getConnection();
 		Query query = conn->query();
 		query << "select uid, imlinkd, status from imonline where uid in (" << ids << ")";
 		log(Debug, "%s", query.preview().data());
 		Result res = query.store();
 		if (res){
 			for(Result::iterator it = res.begin(); it != res.end(); ++it)
 			{
 				uint32_t uid = (*it).at(0);
 				Budid_St bid_st;
 				bid_st.bid = uid;
 				bid_st.st = (protocol::E_IMSTATUS)(int)(*it).at(2);
 				bid_st_vec.push_back(bid_st);
 
 				//add into memcache
 				//set_status(uid, (protocol::E_IMSTATUS)(int)(*it).at(2), (*it).at(1));
 				//needdb_id.erase(uid);
 
 				log(Debug, "[imDataOpHelper::buddylist_get_status]: from imonline tables uid:%d, status:%d", uid, bid_st.st);
 			}
 		}
 
 		//??��??????user_online?��?
 		/*for (set<uint32_t>::iterator it = needdb_id.begin(); it != needdb_id.end(); ++it){
 		uint32_t uid = (*it);
 		Budid_St bid_st;
 		bid_st.bid = uid;
 		bid_st.st = IMOFFLINE;
 		bid_st_vec.push_back(bid_st);
 
 		//add into memcache
 		set_status(uid, IMOFFLINE, NONE_IMLINKD);
 		}*/
 	}
 }

//void imDataOpHelper::get_buddy_uinfo(uint32_t uid, map<uint32_t, SUserinfo>& uinfo_map){
//	log(Debug, "[imDataOpHelper::get_buddy_uinfo]: uid = %d", uid);
//	uinfo_map.clear();
//	vector<uint32_t> buddylist;
//
//	get_buddylist(uid, buddylist);
//	buddylist_get_uinfo(buddylist, uinfo_map);
//}


//bool imDataOpHelper::get_userinfo(uint32_t uid, SUserinfo& info)
//{
//	log(Debug, "[imDataOpHelper::get_userinfo]: uid = %d", uid);
//	info.bid = uid;
//	//online status
//	//wuji 使用memcache取状态后，这部分不需要了。
//	//get_status(uid, st, linkd_id);
//	//info.stat = st;
//
//	//simple uinfo
//	PMC_Simple_Uinfo mcuinfo;
//	get_simple_uinfo(uid, mcuinfo);
//	info.nickname = mcuinfo.nickname;
//	info.rank = mcuinfo.rank;
//	info.sign = mcuinfo.sign;
//	info.version = mcuinfo.version;
//	info.sex = mcuinfo.sex;
//	return true;
//}


//void imDataOpHelper::get_budnick(uint32_t bid, string& nickname)
//{
//	log(Debug, "[imDataOpHelper::get_budnick]: uid = %d", bid);
//	SUserinfo info;
//	get_userinfo(bid, info);
//	nickname = info.nickname;
//}



/* need on memcache
void imDataOpHelper::set_simple_uinfo(uint32_t uid, PMC_Simple_Uinfo& info){
	log(Debug, "set_simple_uinfo() uid: %d", uid);
	Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);
	string key;
	_get_simple_uinfo_key(uid, key);
	mc->mcSet(key, info);
}*/

// delete by Grj
//void imDataOpHelper::get_simple_uinfo(uint32_t uid, PMC_Simple_Uinfo& mcinfo)
//{
//	log(Debug, "[imDataOpHelper::get_simple_uinfo]: uid = %d", uid);
//	/*Memcached* mc = getMemcachedFactory()->getMemcached(IMDB_MEMCACHED);
//	string key;
//	_get_simple_uinfo_key(uid, key);
//
//	if (mc->mcGet(key, mcinfo))
//	{
//		log(Debug, "get_simple_uinfo: memcache hit! uid: %d", uid);
//	}
//	else
//	{
//		log(Debug, "get_simple_uinfo: memcache NOT hit! uid: %d", uid);*/
//
//	//wuji start
//
//	CObjUserInfo objUinfo;
//	bool bIsOk=m_pobjUinfo->GetUinfoAll(uid,objUinfo);
//	if(bIsOk)
//	{
//		mcinfo.nickname = objUinfo.m_strNickname;
//		mcinfo.rank = objUinfo.m_uJiFen;
//		mcinfo.sign = objUinfo.m_strSign;
//		mcinfo.sex = objUinfo.m_sex;//0����Ů��1������
//		mcinfo.version = objUinfo.m_uVersion;
//		
//		//add to memcache
//		//set_simple_uinfo(uid, mcinfo);
//	}
//
//
//	/*
//		//mysql old code
//		Connection* conn = getSqlConnFactory()->getConnectionByName(UINFO_SLAVE_DB);
//		Query query = conn->query();
//		query << "select nick, jifen, sign, sex, version from uinfo where id = %0";
//		query.parse();
//		Result res = query.store(uid);
//		if(res && (res.num_rows() != 0) )
//		{
//			mcinfo.nickname = (string)res.at(0).at(0);
//			mcinfo.rank = res.at(0).at(1);
//			mcinfo.sign = (string)res.at(0).at(2);
//			mcinfo.sex = (int)(res.at(0).at(3)) ? male : female;//res.at(0).at(3)=0����Ů��1������
//			mcinfo.version = res.at(0).at(4);
//
//			//add to memcache
//			//set_simple_uinfo(uid, mcinfo);
//		}
//	*/
//	//wuji end
//	
//}


/*
bool imDataOpHelper::_multi_get(Memcached *m,  const vector<uint32_t> &ids, key_method km, mcm_callback cb, void *context){
	char **keys = new char *[ids.size()];
	size_t *klen  = new size_t[ids.size()];

	vector<string> kv;
	for(size_t i = 0; i < ids.size(); ++i){
		string key;
		km(ids[i], key);
		kv.push_back(key);
		keys[i] = const_cast<char*>(kv.back().data());
		klen[i] = kv.back().size();
	}

	memcached_return rc= memcached_mget(m->memcache(), keys, klen, ids.size());

	mcm_callback ptr[1];
	ptr[0]= cb;

	bool ret = true;
	if(rc == MEMCACHED_SUCCESS){
		memcached_fetch_execute(m->memcache(), ptr, context, 1);
	}else{
		ret = false;
	}

	delete[] keys;
	delete[] klen;

	return ret;
}



void unmarshal(Marshallable& target, void* value, uint32_t size){
	Unpack p(value, size);
	target.unmarshal(p);
}


//callbacks
memcached_return _get_not_offline_buddylist_cb(memcached_st *ptr, memcached_result_st *result, void *context){
	char *return_value = memcached_result_value(result);
	online_blist_t *ctx = (online_blist_t *)context;
	int id = atoi(memcached_result_key_value(result));
	if (return_value){
		PMC_OnlineStat onlinest;
		unmarshal(onlinest, return_value, memcached_result_length(result));
		if(onlinest.st != IMOFFLINE){
			ctx->second->push_back(id);
		}
		ctx->first->erase(id);
		log(Debug, "get_not_offline_buddylist: memcache hit! uid: %d, status: %d", id, onlinest.st);
	}
	return MEMCACHED_SUCCESS;
}

memcached_return _buddylist_get_uinfo_cb(memcached_st *ptr, memcached_result_st *result, void *context){
	char *return_value = memcached_result_value(result);
	blist_infomap_t *ctx = (blist_infomap_t *)context;
	int id = atoi(memcached_result_key_value(result));
	if (return_value){
		PMC_Simple_Uinfo info;
		unmarshal(info, return_value, memcached_result_length(result));

		SUserinfo uinfo;
		uinfo.bid = id;
		uinfo.nickname = info.nickname;
		uinfo.rank = info.rank;
		uinfo.sign = info.sign;
		uinfo.version = info.version;
		uinfo.sex = info.sex;

		(*(ctx->second))[id] = uinfo;
		ctx->first->erase(id);
		log(Debug, "buddylist_get_uinfo: memcache hit! uid: %d", id);
	}
	return MEMCACHED_SUCCESS;
}


memcached_return _bdlist_cb(memcached_st *ptr, memcached_result_st *result, void *context){
	char *return_value = memcached_result_value(result);
	buddy_pair_t *ctx = (buddy_pair_t *)context;
	int id = atoi(memcached_result_key_value(result));
	if (return_value){
		PMC_OnlineStat onlinest;
		unmarshal(onlinest, return_value, memcached_result_length(result));

		Budid_St bid_st;
		bid_st.bid = id;
		bid_st.st = onlinest.st;
		ctx->second->push_back(bid_st);
		ctx->first->erase(id);
		log(Debug, "from memcache online uid:%d, status%d", bid_st.bid, bid_st.st);
	}
	return MEMCACHED_SUCCESS;
}
*/
