
#include "hook_call.hpp"
#include "server_common/seda/syncall/net.hpp"

namespace seda
{
hook_call::hook_call()
{
	_port=0;
	_max_connection=0;
	_current_conns_num=0;
}
hook_call::~hook_call()
{
	for(;;)
	{
		_mtx.lock();
		_max_connection=0;
		if(_current_conns_num==0)
		{
			_mtx.unlock();
			break;
		}
		_mtx.unlock();
		usleep(100000);
	}
}
void hook_call::setup(const char* ip,int port,uint64_t max_connection)
{
	_mtx.lock();
	_max_connection=max_connection;
	std::string tem_ip;
	tem_ip=ip;
	if(_ip==tem_ip && _port==port)
	{
		_mtx.unlock();
		return;
	}
	_ip=tem_ip;
	_port=port;
	////
	CONNDAT* cndesc=0;
	for(uint64_t i=0;i<_pool.size();++i)
	{
		_pool.raw_find(i,cndesc);
		cndesc->_flag=1;
	}
	_mtx.unlock();
}
int hook_call::acquire_one_conn(CONNDAT*& conn,uint32_t& id,std::string& g_ip,int& g_port,int& gflag)
{
	_mtx.lock();
	if(_current_conns_num>=_max_connection)
	{
		_mtx.unlock();
		return -1;
	}
	++_current_conns_num;
	int retaa=_pool.new_id(id, conn);
	assert(retaa==0);
	g_ip=_ip;
	g_port=_port;
	gflag=conn->_flag;
	if(conn->_flag!=0)
	{
		conn->_flag=0;
	}
	_mtx.unlock();
	return 0;
}
void hook_call::release_one_conn(uint32_t id)
{
	CONNDAT* wuliao=0;
	_mtx.lock();
	--_current_conns_num;
	int retaa=_pool.delete_id(id, wuliao);
	assert(retaa==0);
	_mtx.unlock();
}

int hook_call::call(std::vector<char>& send,ISeqFraming* frame,int time_out_mill_sec)
{
	std::vector<char> recv;
	CONNDAT* connd=0;
	uint32_t cnnid=0;
	int original_time_mill=time_out_mill_sec;
	int remain_time_mill=time_out_mill_sec;
	std::string tem_ip;
	int tem_port;
	int tem_flag;
	while(acquire_one_conn(connd,cnnid,tem_ip,tem_port,tem_flag)<0)
	{
		if(_max_connection==0)
		{
			return -1;
		}
		if(remain_time_mill<=100)
		{
			return -1;
		}
		else
		{
			usleep(100000);
			remain_time_mill-=100;
		}
	}
	tcp_connector& tmp_tcp_connector=connd->_conn;
	if(tem_flag!=0)
	{
		tmp_tcp_connector.close();
	}
	long long prev_time=0;
	long long new_time=0;
	int ret=0;
	///
	for(int gh=0;gh<2;++gh)
	{
	/////////////check open
	if(!tmp_tcp_connector.is_open())
	{
		tmp_tcp_connector.open();
		tmp_tcp_connector.configure_blocking(false);
	}
	/////////
	if(!tmp_tcp_connector.is_connected())
	{
		prev_time = Net::current_time_millis();
		if(!tmp_tcp_connector.connect(tem_ip.c_str(),tem_port,remain_time_mill))
		{
			tmp_tcp_connector.close();
			release_one_conn(cnnid);
			return -2;
		}
		new_time = Net::current_time_millis();
		remain_time_mill-= (new_time-prev_time);
		if(remain_time_mill<1)
		{
			//too little time remain
			tmp_tcp_connector.close();
			release_one_conn(cnnid);
			return -2;
		}
		tmp_tcp_connector.tcp_nodelay(true);
	}
	///////////sendding 
	char ggc[4];
	int test_close_read_ret=tmp_tcp_connector.read(ggc, 3);
	if(test_close_read_ret!=0)
	{
		////undefined data meet
		tmp_tcp_connector.close();
		continue;
	}

	prev_time = Net::current_time_millis();
	ret=tmp_tcp_connector.sync_write(&send[0],send.size(),remain_time_mill);
	if(ret==-2)
	{
		tmp_tcp_connector.close();
		continue;
	}
	break;
	}
	if(ret!=(int)send.size())
	{
		tmp_tcp_connector.close();
		release_one_conn(cnnid);
		return -2;
	}
	new_time = Net::current_time_millis();
	remain_time_mill-= (new_time-prev_time);

	//////////////receive
	if(frame==NULL)
	{
		///no receive
		release_one_conn(cnnid);
		return 0;
	}
	recv.clear();
	for(;;)
	{
		int morbe=0;
		int req_len=frame->frame(&recv[0], recv.size(),morbe);
		if(req_len<0)
		{
			tmp_tcp_connector.close();
			release_one_conn(cnnid);
			return -3;
		}
		if(req_len<=(int)recv.size())
		{
			if(morbe==0)
			{
				break;
			}
			else
			{
				uint64_t new_sz=recv.size()-req_len;
				memmove(&recv[0],&recv[0]+req_len,new_sz);
				recv.resize(new_sz);
				remain_time_mill=original_time_mill;
				continue;
			}
		}
		int need_more_bytes=req_len-(int)recv.size();
		int curr_offset=recv.size();
		recv.resize(req_len);
		if(remain_time_mill<1)
		{
			//no time left
			tmp_tcp_connector.close();
			release_one_conn(cnnid);
			return -2;
		}
		prev_time = Net::current_time_millis();
		int recv_len=tmp_tcp_connector.sync_read((&recv[0])+curr_offset,need_more_bytes, remain_time_mill );
		if(recv_len!=need_more_bytes)
		{
			///time_out
			tmp_tcp_connector.close();
			release_one_conn(cnnid);
			return -2;
		}
		new_time = Net::current_time_millis();
		remain_time_mill-= (new_time-prev_time);
	}
	release_one_conn(cnnid);
	return 0;
}


}

