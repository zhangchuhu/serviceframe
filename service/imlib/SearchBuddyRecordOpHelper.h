#ifndef __searchBuddyRecordOpHelper_h__
#define __searchBuddyRecordOpHelper_h__

#include <string>
#include "mysql++/mysql++.h"

namespace server
{
	namespace imlist
	{
		struct SearchBuddyRecordDataSet
		{
			uint32_t search_id;
			uint32_t uid;
			uint32_t search_uid;
			std::string time;
		};

		static const time_t SEARCH_BUDDY_MAX_COUNT = 10;																						//设定时间段内添加好友的次数
		static const time_t SEARCH_BUDDY_RECORD_TIME_INTERVAL = 180;																//添加好友统计数量间隔时间
		static const time_t DELETE_SEARCH_BUDDY_RECORD_TIME = 4*SEARCH_BUDDY_RECORD_TIME_INTERVAL;		//记录删除间隔时间
		//添加好友记录操作
		class SearchBuddyRecordOpHelper
		{
		public:
			SearchBuddyRecordOpHelper(mysqlpp::Connection* conn):m_conn(conn)
			{

			}

			//插入记录
			void insertRecord(SearchBuddyRecordDataSet &data);

			//删除记录
			void deleteRecord(const time_t t = DELETE_SEARCH_BUDDY_RECORD_TIME);

			//获取某个用户在某个时间段内添加好友的次数
			uint32_t getCount(const uint32_t uid, const time_t t = SEARCH_BUDDY_RECORD_TIME_INTERVAL);

		private:
			mysqlpp::Connection* m_conn;
		};
	}
}


#endif
