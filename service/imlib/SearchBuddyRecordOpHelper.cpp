#include "./SearchBuddyRecordOpHelper.h"
#include "core/sox/logger.h"

using namespace server::imlist;
using namespace mysqlpp;

//插入记录
void SearchBuddyRecordOpHelper::insertRecord(SearchBuddyRecordDataSet &data)
{
	Query query = m_conn->query();
	query << "insert into search_buddy_record(uid, search_uid, time)  values(%0, %1, now())";

	query.parse();

	log(Info, "%s", query.preview(data.uid,data.search_uid).data());

	ResNSel res = query.execute(data.uid, data.search_uid);

	if (!res.success)
	{
		log(Error,"insert into search_buddy_record error!");
		throw mysqlpp::BadQuery("SQL operation error!");
	}
}

//删除记录
void SearchBuddyRecordOpHelper::deleteRecord(const time_t t)
{
	Query query = m_conn->query();
	query << "delete from search_buddy_record where UNIX_TIMESTAMP(time) < UNIX_TIMESTAMP()-%0";

	query.parse();
	log(Info, "%s", query.preview(static_cast<const uint32_t>(t)).data());
	ResNSel res = query.execute(static_cast<const uint32_t>(t));

	if (!res.success)
	{
		log(Error,"delete search_buddy_record error!");
		throw mysqlpp::BadQuery("SQL operation error!");
	}

}

//获取某个用户在某个时间段内添加好友的次数
uint32_t SearchBuddyRecordOpHelper::getCount(const uint32_t uid, const time_t t)
{
	Query query = m_conn->query();
	query << "select count(*) from search_buddy_record where uid = %0 and UNIX_TIMESTAMP(time) >=UNIX_TIMESTAMP()-%1";
	query.parse();
	log(Info, "%s", query.preview(uid, static_cast<const uint32_t>(t)).data());
	Result res = query.store(uid, static_cast<const uint32_t>(t));
	uint32_t result = SEARCH_BUDDY_MAX_COUNT;
	if (res)
	{
		result = (uint32_t)(res.at(0).at(0));
	}

	return result;
}

