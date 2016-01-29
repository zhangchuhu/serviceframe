#ifndef _HOOKER_CALL_HPP_
#define _HOOKER_CALL_HPP_

#include <vector>
#include <string>
#include "server_common/seda/syncall/tcp_conn.hpp"
#include "server_common/seda/syncall/id_map.h"
#include "server_common/seda/syncall/lin_mutex.hpp"

namespace seda
{
	class ISeqFraming
	{
	public:
		virtual ~ISeqFraming(){};
		///more==0 means no more, more==1 means is more
		virtual int frame(const char* dat,int len,int& more)=0;
	};
	class hook_call
	{
	public:
		hook_call();
		~hook_call();
	public:
		///frame==NULL means no receive
		void setup(const char* ip,int port,uint64_t max_connection);

		//return 
		// 0: succeed
		//-1: time_out of full
		//-2: time_out of net_work
		//-3: bad package
		int call(std::vector<char>& send,ISeqFraming* frame,int time_out_mill_sec);

	private:
		struct CONNDAT
		{
			CONNDAT()
			{
				_flag=0;
			}
			tcp_connector _conn;
			int _flag;
		};
		id_map::IDMap<CONNDAT,id_map::IDType_INT32_2_2> _pool;
		uint64_t _current_conns_num;
		linux_mutex_wrapper _mtx;

		volatile uint64_t _max_connection;
		std::string _ip;
		int _port;
	private:
		int acquire_one_conn(CONNDAT*& conn,uint32_t& id,std::string& g_ip,int& g_port,int& gflag);
		void release_one_conn(uint32_t id);

	};
	
}


#endif //_HOOKER_CALL_HPP_



