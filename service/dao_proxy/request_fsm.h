#ifndef _THEDAO_REQUEST_FSM_H_
#define _THEDAO_REQUEST_FSM_H_
#include <string>
#include <vector>
#include <map>

#include "x_timer.hpp"
#include "route_types.h"

struct memcached_st;

namespace server
{
namespace dao_proxy
{

	class ReqFSM : public x_timer::TimerEventHandler
	{
	public:
		~ReqFSM();
	public:
		// static method
		static uint32_t gl_m_fsm_num;
		static ReqFSM* createFSM();
		static ReqFSM* findFSM(uint64_t u64FSMId);
		static void destroyFSM(uint64_t u64FSMId);

		void start(int32_t thhedid,uint64_t cid,std::string& return_function,RouteReq* proute_header,std::string& fname,const char* dat,uint32_t len);
		void onResponse(uint32_t result_code,std::string& trace_debug,uint32_t coding,std::string& upper_layer,uint32_t free);
		void end();

		virtual void handle_timeout(void* param);
	private:
		ReqFSM();
		void start_timer(int seconds);
		uint64_t m_u64TimerId;	// illegal value: 0
		uint64_t m_u64FsmId;
		/////////
		std::string m_app_name;
		int to_groupid;
		uint64_t req_connection_id;
		std::string m_fname;
		int32_t thrift_headid;
		std::string m_select_server_name;
		std::string return_function_name;
		///////
		uint64_t abs_send_time_us;
		////////////
		RouteResp m_routeresp_pk;
	public:
		int32_t headofthrift_headid;
	protected:
		void send_resp_thrift_packet(std::string& thriftpack,uint64_t connectid,std::string* ret_addition_debug=0);
		std::string trace_debug_mytag();
	};
}
}

#endif	// _THEDAO_REQUEST_FSM_H_
