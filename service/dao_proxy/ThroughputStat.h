#ifndef _THROUGHPUT_STAT_H_20111027
#define _THROUGHPUT_STAT_H_20111027
#include <map>
#include <vector>
#include <string>
#include "x_timer.hpp"
#include "global_typedef.h"

namespace server
{
	namespace dao_proxy
	{
		class ThroughputStatTimer : public x_timer::TimerEventHandler
		{
		public:
			ThroughputStatTimer();
			~ThroughputStatTimer();

			virtual void handle_timeout(void* param);
			void start_timer(int seconds);
		private:
			uint64_t timer_id;
		};

		class ThroughputStat
		{
		public:
			ThroughputStat();
			~ThroughputStat();

			int init();
			ThroughputStatTimer start_timer;
		public:
			int init_load_conf();

			//return value 0:ok get one ticket 1:ticket sold out 2:no licence
			int check_and_get_tickets(std::string* app_name,std::string* dao_name);

			void set_temp_block(std::string* app_name,std::string* dao_name,int sperten,int chargetimes);

		private:
			std::map<std::string,int> m_app_tickets;
			std::map<std::string,int> m_dao_tickets;
			std::map<std::pair<std::string,std::string> ,int> m_appdao_tickets;
			bool m_bGlobalTicket;
			int m_global_ticket;
			///////////////////
			std::map<std::string,int> m_app_licence;
			std::map<std::string,int> m_dao_licence;
			std::map<std::pair<std::string,std::string> ,int> m_appdao_licence;
			bool m_bGlobalLicence;
			int m_global_licence;
			bool inner_check_licence(std::string* app_name,std::string* dao_name);
			///////////////////
		private:
			struct TBlock
			{
				TBlock(){cur=max=recharge=0;}
				~TBlock(){}
				TBlock(const TBlock& o)
				{cur=o.cur;max=o.max;recharge=o.recharge;}
				TBlock& operator =(const TBlock& o)
				{cur=o.cur;max=o.max;recharge=o.recharge;return *this;}
				int cur;
				int max;     //// max send per tensecond
				int recharge;//// how many tensecond
			};
			std::map<std::string,TBlock> m_app_block;
			std::map<std::string,TBlock> m_dao_block;
			std::map<std::pair<std::string,std::string> ,TBlock> m_appdao_block;
			bool m_bGlobalBlock;
			TBlock m_global_block;
			//return true when get succeed
			bool check_and_get_block(std::string* app_name,std::string* dao_name);
		public:
			void timer_through_block();
			///////////////////

		public:
			void debug_show(std::stringstream& ss);
		};

	}
}

#endif	// _THROUGHPUT_STAT_H_20111027

