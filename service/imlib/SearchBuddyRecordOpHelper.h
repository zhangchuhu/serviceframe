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

		static const time_t SEARCH_BUDDY_MAX_COUNT = 10;																						//�趨ʱ�������Ӻ��ѵĴ���
		static const time_t SEARCH_BUDDY_RECORD_TIME_INTERVAL = 180;																//��Ӻ���ͳ���������ʱ��
		static const time_t DELETE_SEARCH_BUDDY_RECORD_TIME = 4*SEARCH_BUDDY_RECORD_TIME_INTERVAL;		//��¼ɾ�����ʱ��
		//��Ӻ��Ѽ�¼����
		class SearchBuddyRecordOpHelper
		{
		public:
			SearchBuddyRecordOpHelper(mysqlpp::Connection* conn):m_conn(conn)
			{

			}

			//�����¼
			void insertRecord(SearchBuddyRecordDataSet &data);

			//ɾ����¼
			void deleteRecord(const time_t t = DELETE_SEARCH_BUDDY_RECORD_TIME);

			//��ȡĳ���û���ĳ��ʱ�������Ӻ��ѵĴ���
			uint32_t getCount(const uint32_t uid, const time_t t = SEARCH_BUDDY_RECORD_TIME_INTERVAL);

		private:
			mysqlpp::Connection* m_conn;
		};
	}
}


#endif
