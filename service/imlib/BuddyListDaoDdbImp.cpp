#include <occi.h>
#include "core/sox/logger.h"
#include "BuddyListDaoDdbImp.h"
#include "server_common/dblib/MysqlCallback.h"


using namespace std;
using namespace server::mysqldb;
using namespace server::mysqlimdb;
using namespace server::imdao;
using namespace protocol::im;

/*
CREATE TABLE `buddylist` (
`uid` int(11) NOT NULL,
`bid` int(11) NOT NULL,
`pid` int(11) NOT NULL,
`remark` varchar(255) default '',
`n_isdel` tinyint(4) NOT NULL default '0',
`tm_addtime` datetime NOT NULL,
PRIMARY KEY  (`uid`,`bid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8
*/

CBuddyListDaoDdbImp::CBuddyListDaoDdbImp(server::mysqldb::MysqlHelperTempalte *poracleHT)
{
	setMysqlHelperTempalte(poracleHT);
}


// bool CBuddyListDaoMysqlImp::Get(uint32_t uId,std::vector<protocol::im::SimpleBInfo>& vecData)//取真正的好友列表
// {
// 	bool bIsOk=true;
// 	struct CTemp : public AbstractCallBack
// 	{
// 	public:
// 		CTemp()
// 		{
// 			m_bIsExist=false;
// 		}
// 		virtual void onResult(oracle::occi::ResultSet *res, void *obj) 
// 		{
// 			vector<SimpleBInfo>& vecData=*(vector<SimpleBInfo>*)obj;
// 			SimpleBInfo objData;
// 			while(res->next())
// 			{
// 				objData.buddyid=res->getUInt(1);
// 				objData.pid=res->getUInt(2);
// 				objData.remark=res->getString(3);
// 				vecData.push_back(objData);
// 				m_bIsExist = true;
// 			}
// 		}
// 		bool m_bIsExist;
// 	} objCb;
// 
// 	vecData.clear();
// 	bIsOk = getMysqlHelperTempalte()->commonSqlExe(&objCb, &vecData, "select n_bid,n_pid,str_remark  from tbl_buddylist where n_uid=:1 "
// 		"and n_isdel=0",uId);
// 
// 	return objCb.m_bIsExist && bIsOk;
// 
// }



bool CBuddyListDaoDdbImp::GetCount(uint32_t uId,uint32_t &nCount)//取好友个数
{
	struct Get1CallBack : public AbstractCallBack
	{
	public:
		virtual void onResult(mysqlpp::Result &result, void *obj) 
		{
			uint32_t &nTemp=*(uint32_t*)obj;
			mysqlpp::Result::iterator iter = result.begin();
			nTemp=(uint32_t) (*iter).at(0);
		}
	} objCb;

	objCb.setPreview(false);
	std::string &strUserDB = GetMetaData()->GetDbName(uId);
	
	return getMysqlHelperTempalte()->commonSqlExe(strUserDB, &objCb, &nCount,
		"select count(bid) from buddylist where uid=:0 and n_isdel=0", uId);
}


bool CBuddyListDaoDdbImp::Delete(uint32_t uUid, uint32_t uBid)
{
// 	query << "update buddylist set n_isdel=1 where uid = %0 and bid = %1";
// 	query << "select uid from buddylist where  uid = %0 and bid = %1 and n_isdel=1";
// 	query << "delete from buddylist where uid = %0 and bid = %1";

	bool bIsOk=true;
	NOPCallBack objCb;
	objCb.setPreview(false);	

	struct Delete1CallBack : public AbstractCallBack
	{
	public:
		Delete1CallBack()
		{
			m_bIsExist=false;
		}
		virtual void onResult(mysqlpp::Result &result, void *obj) 
		{
			if (!result.empty())
			{
				m_bIsExist = true;
			}
		}
		bool m_bIsExist;
	} objCb1;
	
	objCb1.setPreview(false);
	//检查对方是否也删除了自己
    std::string &strUserDB = GetMetaData()->GetDbName(uUid);
    std::string &strBuddyDB = GetMetaData()->GetDbName(uBid);
    bIsOk = getMysqlHelperTempalte()->commonSqlExe(strBuddyDB, &objCb1, NULL,
		"select uid from buddylist where uid=:0 and bid=:1 and n_isdel=1", uBid, uUid);  

	if (!bIsOk)
	{
		return false;
	}
	else if (objCb1.m_bIsExist)
	{
		//双方都已删除，清除双方记录
		bIsOk = getMysqlHelperTempalte()->commonSqlExe(strUserDB, &objCb, NULL,
			"delete from buddylist where uid=:0 and bid=:1", uUid, uBid);

		bIsOk = getMysqlHelperTempalte()->commonSqlExe(strBuddyDB, &objCb, NULL,
			"delete from buddylist where uid=:0 and bid=:1", uBid, uUid);
	}
	else if (!objCb1.m_bIsExist)
	{
		//单方删除，标记自己的记录
		bIsOk = getMysqlHelperTempalte()->commonSqlExe(strUserDB, &objCb, NULL,
			"update buddylist set n_isdel=1 where uid=:0 and bid=:1", uUid, uBid);
	}

	return bIsOk;
}


//修改好友备注
bool CBuddyListDaoDdbImp::Update(uint32_t uUid, uint32_t uBid, const std::string& strRemark)
{
	NOPCallBack objCb;
	objCb.setPreview(false);
	std::string &strUserDB = GetMetaData()->GetDbName(uUid);
// 	return getMysqlHelperTempalte()->commonSqlExe(strUserDB, &objCb, NULL,
// 		"update buddylist set str_remark=:s0 where n_uid=:1 and n_bid=:2", strRemark.c_str(), uUid, uBid);
	return getMysqlHelperTempalte()->commonSqlExe(strUserDB, &objCb, NULL,
		"update buddylist set remark=:s0 where uid=:1 and bid=:2", strRemark.c_str(), uUid, uBid);
}

//移动好友到某个组
bool CBuddyListDaoDdbImp::Update(uint32_t uUid, uint32_t uBid, uint32_t uPid)
{
	NOPCallBack objCb;
	objCb.setPreview(false);
    std::string &strUserDB = GetMetaData()->GetDbName(uUid);
	return  getMysqlHelperTempalte()->commonSqlExe(strUserDB, &objCb, NULL,
		"update buddylist set pid=:0 where uid=:1 and bid=:2", uPid, uUid, uBid);
}

bool CBuddyListDaoDdbImp::Add(const CBuddyListDaoData& objData)
{
	NOPCallBack objCb;
	objCb.setPreview(false);
	std::string &strUserDB = GetMetaData()->GetDbName(objData.m_uId);

	if (objData.m_strRemark.length()>0)
	{
		return getMysqlHelperTempalte()->commonSqlExe(strUserDB, &objCb, NULL,
			"insert into buddylist(uid, bid, pid, n_isdel, tm_addtime,remark) values (:0, :1, :2, 0, now(),:s3)\
			on duplicate key update pid=:4, n_isdel=0, tm_addtime=now(),remark=:s5",
			objData.m_uId, objData.m_uBid, objData.m_uPid,objData.m_strRemark.c_str(), objData.m_uPid,objData.m_strRemark.c_str());
	}
	else
	{
		return getMysqlHelperTempalte()->commonSqlExe(strUserDB, &objCb, NULL,
			"insert into buddylist(uid, bid, pid, n_isdel, tm_addtime) values (:0, :1, :2, 0, now())\
			on duplicate key update pid=:3, n_isdel=0, tm_addtime=now()",
			objData.m_uId, objData.m_uBid, objData.m_uPid, objData.m_uPid);
	}


// 	mysql> insert into buddylist(uid, bid, pid, n_isdel, tm_addtime) 
// 		values (75960332, 75960438, 1, 0, now())          on duplicate key update   pid=1, n_isdel=0, tm_addtime=now();   
}

bool CBuddyListDaoDdbImp::IsExist(uint32_t uUid,uint32_t uBid)
{
	bool bIsOk = true;

	struct IsExistCallBack: public AbstractCallBack
	{
	public:
		IsExistCallBack()
		{
			m_bIsExist=false;
		}
		virtual void onResult(mysqlpp::Result &result, void *obj) 
		{
			if(!result.empty())
			{
				m_bIsExist = true;
			}
		}
		bool m_bIsExist;
	} objCb;

	objCb.setPreview(false);
    std::string &strUserDB = GetMetaData()->GetDbName(uUid);
	bIsOk = getMysqlHelperTempalte()->commonSqlExe(strUserDB, &objCb, NULL,
		"select uid from buddylist where uid=:0 and bid=:1 and n_isdel=0", uUid, uBid);

	return objCb.m_bIsExist;

}

//取真正的好友列表
bool CBuddyListDaoDdbImp::Get(uint32_t uUid,std::vector<server::imdao::CBuddyListDaoData>& vecData)
{
	struct Get1CallBack : public AbstractCallBack
	{
	public:
		virtual void onResult(mysqlpp::Result &result, void *obj) 
		{
			vector<CBuddyListDaoData>& vecData = *(vector<CBuddyListDaoData>*)obj;
			CBuddyListDaoData objData;

			for (mysqlpp::Result::iterator iter = result.begin(); iter != result.end(); iter++)
			{
				objData.m_uId = (uint32_t) (*iter).at(0);
				objData.m_uBid = (uint32_t) (*iter).at(1);
				objData.m_uPid = (uint32_t) (*iter).at(2);
				objData.m_strRemark = (string) (*iter).at(3);
				vecData.push_back(objData);
			}
		}
	} objCb;

	objCb.setPreview(false);
    std::string &strUserDB = GetMetaData()->GetDbName(uUid);
	vecData.clear();
	return getMysqlHelperTempalte()->commonSqlExe(strUserDB, &objCb, &vecData,
		"select uid,bid,pid,remark from buddylist where uid=:0 and n_isdel=0", uUid);
}

//取好友列表（包括真正的和被单方删除的）
bool CBuddyListDaoDdbImp::GetAll(uint32_t uUid,std::vector<server::imdao::CBuddyListDaoData>& vecData)
{
	struct GetAllCallBack : public AbstractCallBack
	{
	public:
		virtual void onResult(mysqlpp::Result &result, void *obj) 
		{
			vector<CBuddyListDaoData>& vecData=*(vector<CBuddyListDaoData>*)obj;
			CBuddyListDaoData objData;
			
			for (mysqlpp::Result::iterator iter = result.begin(); iter != result.end(); iter++)
			{
				objData.m_uId = (uint32_t) (*iter).at(0);
				objData.m_uBid = (uint32_t) (*iter).at(1);
				objData.m_uPid = (uint32_t) (*iter).at(2);
				objData.m_strRemark = (string) (*iter).at(3);
				vecData.push_back(objData);
			}
		}
	} objCb;


	objCb.setPreview(false);
	std::string &strUserDB = GetMetaData()->GetDbName(uUid);
	vecData.clear();
	return getMysqlHelperTempalte()->commonSqlExe(strUserDB, &objCb, &vecData,
		"select uid,bid,pid,remark from buddylist where uid=:0", uUid);
}





// bool CBuddyListDaoMysqlImp::Get(uint32_t uId,std::vector<server::imdao::CBuddyListDaoData>& vecData)
// {
// 	bool bIsOk=true;
// 	struct CTemp : public AbstractCallBack
// 	{
// 	public:
// 		CTemp()
// 		{
// 			m_bIsExist=false;
// 		}
// 		virtual void onResult(oracle::occi::ResultSet *res, void *obj) 
// 		{
// 			vector<CBuddyListDaoData>& vecData=*(vector<CBuddyListDaoData>*)obj;
// 			CBuddyListDaoData objData;
// 			while(res->next())
// 			{
// 				objData.m_uPid=res->getUInt(1);
// 				objData.m_uId=res->getUInt(2);
// 				objData.m_strRemark=res->getString(3);
// 				vecData.push_back(objData);
// 				m_bIsExist = true;
// 			}
// 		}
// 		bool m_bIsExist;
// 	} objCb;
// 
// 	vecData.clear();
// 	bIsOk = getMysqlHelperTempalte()->commonSqlExe(&objCb, &vecData, "select n_fid,n_uid,str_remark  from tbl_buddylist where n_uid=:1",uId);
// 
// 	return objCb.m_bIsExist && bIsOk;
// 
// }


//取真正的好友列表
bool CBuddyListDaoDdbImp::Get(uint32_t uUid, std::vector<uint32_t>& vecData)
{
	struct GetBidCallBack : public AbstractCallBack
	{
	public:
		virtual void onResult(mysqlpp::Result &result, void *obj) 
		{
			vector<uint32_t>& vecData = *(vector<uint32_t>*)obj;
			uint32_t uBid;

			for (mysqlpp::Result::iterator iter = result.begin(); iter != result.end(); iter++)
			{
				uBid = (uint32_t) (*iter).at(0);
				vecData.push_back(uBid);
			}
		}
	} objCb;

	objCb.setPreview(false);
	std::string &strUserDB = GetMetaData()->GetDbName(uUid);
	vecData.clear();
// 	return getMysqlHelperTempalte()->commonSqlExe(strUserDB, &objCb, &vecData,
// 		"select n_bid  from tbl_buddylist where n_uid=:0 and n_isdel=0", uUid);
	return getMysqlHelperTempalte()->commonSqlExe(strUserDB, &objCb, &vecData,
		"select bid  from buddylist where uid=:0 and n_isdel=0", uUid);
}


//取好友列表（包括真正的和被单方删除的）
bool CBuddyListDaoDdbImp::GetAll(uint32_t uUid, std::vector<uint32_t>& vecData)
{
	struct GetUid2CallBack : public AbstractCallBack
	{
	public:
		virtual void onResult(mysqlpp::Result &result, void *obj) 
		{
			vector<uint32_t>& vecData = *(vector<uint32_t>*)obj;
			uint32_t uBid;

			for (mysqlpp::Result::iterator iter = result.begin(); iter != result.end(); iter++)
			{
				uBid =  (uint32_t) (*iter).at(0);
				vecData.push_back(uBid);
			}
		}
	} objCb;

	objCb.setPreview(false);
	std::string &strUserDB = GetMetaData()->GetDbName(uUid);
	vecData.clear();
	return  getMysqlHelperTempalte()->commonSqlExe(strUserDB, &objCb, &vecData,
		"select bid from buddylist where uid=:0", uUid);
}

//取真正的好友列表
bool CBuddyListDaoDdbImp::Get(uint32_t uUid, std::vector<protocol::im::CBuddyListNoRemark>& vecData)
{
	//log(Info, "[CBuddyListDaoDdbImp::Get]: uid:%u start",uUid);
	struct GetNoRemarkCallBack : public AbstractCallBack
	{
	public:
		virtual void onResult(mysqlpp::Result& result, void *obj) 
		{
			vector<CBuddyListNoRemark>& vecData = *(vector<CBuddyListNoRemark>*)obj;
			CBuddyListNoRemark objData;

			//log(Info, "[CBuddyListDaoDdbImp::Get]: onResult start");
			
			for (mysqlpp::Result::iterator iter = result.begin(); iter != result.end(); iter++)
			{
				objData.m_uBid = (uint32_t) (*iter).at(0);
				objData.m_uPid = (uint32_t) (*iter).at(1);
				vecData.push_back(objData);
			}
			//log(Info, "[CBuddyListDaoDdbImp::Get]: onResult ok");
		}
	} objCb;
	objCb.setPreview(false);

	std::string &strUserDB = GetMetaData()->GetDbName(uUid);
	vecData.clear();
	log(Info, "[CBuddyListDaoDdbImp::Get]: uid:%u ok,strDb:%s",uUid,strUserDB.c_str());

	return getMysqlHelperTempalte()->commonSqlExe(strUserDB, &objCb, &vecData,
		"select bid,pid from buddylist where uid=:0 and n_isdel=0", uUid);

}

//取真正的好友列表
bool CBuddyListDaoDdbImp::Get(uint32_t uUid,std::map<uint32_t, std::string>& mapBidToRemark)
{
	struct GetBidToRemarkCallBack : public AbstractCallBack
	{
	public:
		virtual void onResult(mysqlpp::Result &result, void *obj) 
		{
			map<uint32_t, std::string>& mapData = *(map<uint32_t, std::string>*)obj;
			uint32_t uBid;

			for (mysqlpp::Result::iterator iter = result.begin(); iter != result.end(); iter++)
			{
				uBid = (*iter).at(0);
				mapData[uBid] = (string) (*iter).at(1);
			}
		}
	} objCb;

	objCb.setPreview(false);
	mapBidToRemark.clear();
	std::string &strUserDB = GetMetaData()->GetDbName(uUid);
	return getMysqlHelperTempalte()->commonSqlExe(strUserDB, &objCb, &mapBidToRemark,
		"select bid,remark from buddylist where uid=:0 and n_isdel=0", uUid);
}
