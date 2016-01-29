#ifndef _THE_DAO_CONNECTION_MANAGER_H_
#define _THE_DAO_CONNECTION_MANAGER_H_
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <set>
#include "server_common/server-lib3/interface.h"
#include "global_typedef.h"
#include "common/core/ilink.h"
#include "x_timer.hpp"

namespace server
{
namespace dao_proxy
{
	class DaoConnectionTimer : public x_timer::TimerEventHandler
	{
	public:
		DaoConnectionTimer();
		~DaoConnectionTimer();

		virtual void handle_timeout(void* param);
		void start_timer(int seconds);
	private:
		uint64_t timer_id;
	};

	class DaoConnectionManager
	{
	public:
		struct IPINFO
		{
			uint64_t connectid;
			/////
			float resptime;////us 1000 * 1000
			int bad_level;///0 1 2 3 4 5 6
			/////
			int ping_counter;
			uint64_t last_ping_time;
			int m_state;//0:ok 1:waiting_resp
		};
	public:
		DaoConnectionManager();
		~DaoConnectionManager();

		DaoConnectionTimer connect_checker;

		int init(core::IClientConnFactory* iccf,core::ILinkHandler* ilh);
		int init_from_conf();
		int rc_calltest();

	public:
		///return 0:succeed  -1:can't create connect
		int get_connection(const IPPORTYPE& ipport,uint64_t& connect_id);

		///return 0:ok  -1:arg error 1:out of bound
		int select_best_connection(std::vector<IPPORTYPE>& ipports,IPPORTYPE& ipp,IPINFO* presult_info=0);

		///return 0:ok 01:fail
		int ping_resp(IPPORTYPE& ipp,uint64_t start_time_us,uint64_t recv_time_us);

		core::ILinkHandler* get_linkhandler();
		core::IClientConnFactory* get_connectionfactory();

		int predict_add_bad_level(std::vector<IPPORTYPE>& ipports);
		int predict_tiny_add_bad_level(std::vector<IPPORTYPE>& ipports);
	public:
		core::IClientConnFactory* m_client_connfactory;
		core::ILinkHandler* m_linkhandler;

		std::map<IPPORTYPE,IPINFO> m_ipinfo;
	public:
		void debug_show(std::stringstream& ss);
	};

}
}

#endif	// _THE_DAO_CONNECTION_MANAGER_H_

