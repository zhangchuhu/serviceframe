#include "./AddBuddyRecordOpHelper.h"
#include "core/sox/logger.h"

using namespace server::imlist;
using namespace mysqlpp;

//插入记录
void AddBuddyRecordOpHelper::insertRecord(AddBuddyRecordDataSet &data)
{
	Query query = m_conn->query();
	query << "insert into add_buddy_record(uid, bid, time)  values(%0, %1, now())";

	query.parse();

	log(Info, "%s", query.preview(data.uid,data.bid).data());

	ResNSel res = query.execute(data.uid, data.bid);

	if (!res.success)
	{
		log(Error,"insert into add_buddy_record error!");
		throw mysqlpp::BadQuery("SQL operation error!");
	}
}


//获取当天添加好友次数
uint32_t AddBuddyRecordOpHelper::getCountToday(const uint32_t uid)
{
	FunLog funlog("AddBuddyRecordOpHelper::getCountToday", true);
	Query query = m_conn->query();
	query << "select count(*) from add_buddy_record where uid = %0 and curdate() = date(time)";
	query.parse();
	funlog.logWithFunName(Info, "%s", query.preview(uid).data());
	Result res = query.store(uid);
	uint32_t result = ADD_BUDDY_MAX_COUNT_PER_DAY;
	if (res)
	{
		result = (uint32_t)(res.at(0).at(0));
	}
	funlog.logWithFunName(Info, "uid(%d) add buddy %d times today!", uid, result);
	return result;
}

