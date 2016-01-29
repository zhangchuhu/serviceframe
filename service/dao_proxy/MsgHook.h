#ifndef _MESSAGE_HOOK_H_H_20120309
#define _MESSAGE_HOOK_H_H_20120309

#include <map>
#include <vector>
#include <string>
#include "x_timer.hpp"
#include "global_typedef.h"

namespace server
{
	namespace dao_proxy
	{
		class MsgHook
		{
		public:
			MsgHook();
			~MsgHook();

			void set_hook(uint64_t req_id,uint64_t connectid,std::string* app_name,std::string* dao_name,int sperten,int chargetimes);
			bool is_hooked(std::string* app_name,std::string* dao_name);
			void clear_hook();
			//return 0: normal  1: just finish timer
			int timer_pool();
			///////////
		public:
			bool b_active;
			std::string m_app_name;
			std::string m_dao_name;
			int m_cur;
			int m_max;
			int m_cnt;
			uint64_t m_cid;

			uint64_t m_req_id;
		};

	}
}

#endif	// _MESSAGE_HOOK_H_H_20120309

