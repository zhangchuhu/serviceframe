#ifndef _THE_DAO_MANAGER_H_
#define _THE_DAO_MANAGER_H_
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <set>
#include "global_typedef.h"
#include "x_timer.hpp"
#include "PxGlobalConf.h"
#include "server_common/protocol/prouter.h"

namespace server
{
namespace dao_proxy
{
	class DaoStatTimer : public x_timer::TimerEventHandler
	{
	public:
		DaoStatTimer();
		~DaoStatTimer();

		virtual void handle_timeout(void* param);
		void start_timer(int seconds);
	private:
		uint64_t timer_id;
	};

	class DaoStatManager
	{
	public:
		DaoStatTimer stat_poller;
		struct DaoINFO
		{
			int curren_load;
			int tick_remain;
			int dao_type;////1:master db  2:slave db
			std::string dao_extent_info;
			std::string debug_info;
			///control
			int tick_remain_last_poll;
			///dynamic stat
			uint32_t sending_request;
			////resp time learning
			double m_lms_k;
			double m_lms_b;
		};
		struct ServINFO
		{
			//static
			std::vector<IPPORTYPE> ipports;
			int groupid;
			//dynamic
			uint64_t last_respping_time;
			std::map<std::string,DaoINFO> dao_infos;

			//control
			int b_immreq_stat;
		};
	public:
		DaoStatManager();
		~DaoStatManager();

		int init_from_conf();
		int rc_calltest();

		void send_all_empty_dao();

		////filter 'A'local 'B'master 'C'slave
		////return 0:ok -1:dao busy -2:no dao
		int dao_select(std::string& dao_name,int localgroupid,ROUTESTRATEGY_TYPE& roustg,ServINFO*& out_serv,std::string& out_ser_name);

		int update_dao_info(std::string& srvname,std::vector< ::server::router::DAOLOAD>& daoinfos);

		std::map<std::string, ServINFO> serv_infos;
		struct DaoIndex
		{
			std::set<std::string> servers;
		};
		std::map<std::string,DaoIndex> dao_index;

		int get_server_ptr(const std::string& ser_name,ServINFO*& out_serv);
		int update_serverdao_free(const std::string& sername,const std::string& daoname,uint32_t free);
	public:
		///prediction modify
		void predict_send_one(std::string& server_name,ServINFO* pserv,std::string& dao_name);

		void start_send_one(ServINFO* pserv,std::string& dao_name);
		void end_recv_one(std::string& server_name,std::string& dao_name);

		int learning_lms(const std::string& server_name,const std::string& dao_name,double actual_resp,uint32_t actual_send_cnt,double& predict_resp);
		int learning_predict_lms(const std::string& server_name,const std::string& dao_name,double actual_resp,uint32_t actual_send_cnt,double& predict_resp);
		void learning_lms_shrink();
	private:
		///////////////
		int inner_del_server(const std::string& server_name);
		int inner_set_server(const std::string& server_name,ServINFO& info);
		int inner_filter_help_A(std::vector<std::string>& server_list,std::string& dao_name,int groupid);
		int inner_filter_help_B(std::vector<std::string>& server_list,std::string& dao_name);
		int inner_filter_help_C(std::vector<std::string>& server_list,std::string& dao_name);
		int inner_filter_help_E(std::vector<std::string>& server_list,std::string& dao_name,std::string& hashid);
		int inner_filter_help_U(std::vector<std::string>& server_list,std::string& dao_name,int usability_level);
		int inner_filter_help_F(std::vector<std::string>& server_list,std::string& dao_name,std::set<int>& groupids);

		int inner_filter_highresponse_minsending(std::vector<std::string>& server_list,std::string& dao_name,std::string& out_serv);
		int inner_filter_highresponse_lmspredict(std::vector<std::string>& server_list,std::string& dao_name,std::string& out_serv);
		int inner_filter_highresponse_random(std::vector<std::string>& server_list,std::string& dao_name,std::string& out_serv);
		////////////////
		uint32_t random_vector_pos(std::vector<long double>& vec,long double sum);
		/////
		int usability_filter(std::vector<std::string>& server_list,std::string& dao_name,int level);
	public:
		void debug_show(std::stringstream& ss);
	};

}
}

#endif	// SERVER_BACK___SQC_MEMCACHE___MEMCACHECLUSTER_H
