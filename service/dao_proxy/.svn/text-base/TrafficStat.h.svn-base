#ifndef _TRAFFIC_STAT_H_20120229
#define _TRAFFIC_STAT_H_20120229
#include <map>
#include <vector>
#include <string>
#include <list>
#include "x_timer.hpp"
#include "global_typedef.h"

namespace server
{
	namespace dao_proxy
	{

		class TrafficStatTimer : public x_timer::TimerEventHandler
		{
		public:
			TrafficStatTimer();
			~TrafficStatTimer();

			virtual void handle_timeout(void* param);
			void start_timer(int seconds);
		private:
			uint64_t timer_id;
		};

		class TrafficStat
		{
		public:
			TrafficStat();
			~TrafficStat();

			int init();
			TrafficStatTimer start_timer;
		public:
			int init_load_conf();

			void stat_send_one(int from_gid,int to_gid,std::string& daoserver,std::string* app_name,std::string* dao_name,int send_cnt);
			void stat_recv_one(int from_gid,int to_gid,std::string& daoserver,std::string* app_name,std::string* dao_name,int recv_cnt,float resp_time);

			void swap_stat();

			int get_dao_last_sendcnt(std::string& dao_name,uint32_t& last_send);
			int get_dao_server_last_sendcnt(std::string& dao_name,std::string& sname,uint32_t& last_send);

		public:
			void stat_error_one(int error_code);

		public:
			void get_some_histroy(std::vector<std::string>& his_str,int start,int leng);

		private:
			struct EachSTAT
			{
				EachSTAT();
				~EachSTAT();
				EachSTAT(const EachSTAT& o);
				EachSTAT& operator = (const EachSTAT& o);
				////////////
				void recv_one_resp(float rtime,int cnt);
				void debug_show(std::stringstream& ss);
				////////////
				int send_num;
				int recv_num;
				int drop_num;
				float ave_resp_time;
			};
			std::map<int,EachSTAT> m_togroup_last_stat;
			std::map<std::string,EachSTAT> m_app_last_stat;
			std::map<std::string,EachSTAT> m_dao_last_stat;
			std::map<std::string,std::map<std::string,EachSTAT> > m_server_dao_last_stat;
			std::map<std::string,std::map<std::string,EachSTAT> > m_app_dao_last_stat;
			EachSTAT m_last_stat;
			////////////
			std::map<int,EachSTAT> m_togroup_stat;
			std::map<std::string,EachSTAT> m_app_stat;
			std::map<std::string,EachSTAT> m_dao_stat;
			std::map<std::string,std::map<std::string,EachSTAT> > m_server_dao_stat;
			std::map<std::string,std::map<std::string,EachSTAT> > m_app_dao_stat;
			EachSTAT m_stat;
			///////////
			std::list<std::string> log_histroy;
			int log_his_size;
			///////////
			std::map<int,int> m_error_count;
			std::map<int,int> m_last_error_count;
		public:
			void debug_show_drop_resp(std::stringstream& ss,uint64_t seq_num);
			/////////////////////
			uint64_t m_incseq;
		};

	}
}

#endif	// _TRAFFIC_STAT_H_20120229


