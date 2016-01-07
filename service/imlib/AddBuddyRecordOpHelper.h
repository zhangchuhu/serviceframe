#ifndef __addBuddyRecordOpHelper_h__
#define __addBuddyRecordOpHelper_h__

#include <string>
#include "mysql++/mysql++.h"

namespace server
{
	namespace imlist
	{
		struct AddBuddyRecordDataSet
		{
			uint32_t add_id;
			uint32_t uid;
			uint32_t bid;
			std::string time;
		};

		static const size_t ADD_BUDDY_MAX_COUNT_PER_DAY = 100;																//�趨ÿ�������Ӻ�����

		//��Ӻ��Ѽ�¼����
		class AddBuddyRecordOpHelper
		{
		public:
			AddBuddyRecordOpHelper(mysqlpp::Connection* conn):m_conn(conn)
			{

			}

			//�����¼
			void insertRecord(AddBuddyRecordDataSet &data);

			//��ȡ������Ӻ��Ѵ���
			uint32_t getCountToday(const uint32_t uid);

		private:
			mysqlpp::Connection* m_conn;
		};
	}
}


#endif
