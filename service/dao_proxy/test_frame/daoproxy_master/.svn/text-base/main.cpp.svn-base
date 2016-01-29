#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <stdint.h>
#include <signal.h>
#include "server_common/seda/syncall/remote_call.hpp"
#include "server_common/seda/common/any_cast.h"
#include "server_common/seda/stage/stage_base.h"
#include "PxGlobalConf.h"
#include "server_common/protocol/pdbnode.h"
#include "common/core/sender.h"
#include "common/core/request.h"
#include "util.h"
#include "server_common/seda/encryption/swiftzlib.h"


using namespace std;
using namespace seda;
using namespace server::dao_proxy;
using namespace server::db_node;
using core::Sender;
using core::Request;

class general_frame : public IFraming
{
public:
	int frame(const char* dat,int len)
	{
		if(len<10)
		{
			return 10;
		}
		uint32_t pack_len=*(uint32_t*)dat ;
		if(pack_len>(2*1024*1024))
		{
			return -1;
		}
		return pack_len;
	}
	static general_frame* instance()
	{
		static general_frame obj;
		return &obj;
	}
};

std::map<IPPORTYPE,remote_call* > gl_rc_mp;

remote_call* gl_get_rc(IPPORTYPE& ipp)
{
	std::map<IPPORTYPE,remote_call* >::iterator it
		=gl_rc_mp.find(ipp);
	if(it==gl_rc_mp.end())
	{
		remote_call* prc=new remote_call;
		prc->setup(ipp.first.c_str(),ipp.second,1);
		gl_rc_mp[ipp]=prc;
		return prc;
	}
	else
	{
		return it->second;
	}
}

struct IRCall
{
	virtual void do_rc(remote_call* p_remotecall,const std::string& ip,int port) = 0;
};

void gl_call_all(IRCall* p)
{
	std::map<IPPORTYPE,remote_call* >::iterator it
		=gl_rc_mp.begin();
	for(;it!=gl_rc_mp.end();++it)
	{
		p->do_rc(it->second,it->first.first,it->first.second);
	}

}

void gl_new_conf(PxGlobalConf* conf)
{
	std::map<std::string, PxGlobalConf::PrcAdd> outdat;
	conf->get_all_proxy(outdat);
	std::map<std::string, PxGlobalConf::PrcAdd>::iterator it
		=outdat.begin();
	for(;it!=outdat.end();++it)
	{
		gl_get_rc(it->second.ipports[0]);
	}

}



struct RCallImpCheckMD5 : public IRCall
{
	void do_rc(remote_call* p_remotecall,const std::string& ip,int port)
	{
		std::string log_prefix;
		JLexical::Cast(port,log_prefix);
		log_prefix=("["+ip+":"+log_prefix+"] ");

		vector<char> send;
		send.resize(18);

		Sender sdr;
		DBStateReq dbpo;
		dbpo.which=108;

		vector<char> recv;

		sdr.clear();
		sdr.setUri(DBStateReq::uri);
		sdr.setResCode(200);
		sdr.marshall(dbpo);
		sdr.endPack();
		send.assign(sdr.header(),sdr.header()+sdr.bodySize()+sdr.headerSize());
		int ret=p_remotecall->call( send, general_frame::instance(),recv, 5000);
		if(ret<0)
		{
			cout <<log_prefix <<"can't call" <<endl;
			return;
		}
		//////////
		Request reqest(&recv[0],recv.size());
		DBStateResp op_resp;
		try
		{
			reqest.head();
			if(reqest.getUri()!=DBStateResp::uri)
			{
				cout <<log_prefix <<"uri not matched" <<endl;
				return;
			}
			op_resp.unmarshal(reqest.getPackData());
		}
		catch(sox::PacketError pe)
		{
			cout <<log_prefix <<"un marshal error" <<endl;
			return;
		}
		if(op_resp.which!=dbpo.which)
		{
			cout <<log_prefix <<"which not matched" <<endl;
			return;
		}
		cout <<log_prefix <<op_resp.result <<endl;

	}
};


struct RGetRangeStat : public IRCall
{
	bool use_filter;
	std::string m_filter_ip;
	int m_filter_port;
	int m_start_pos;
	int m_length_p;
	void do_rc(remote_call* p_remotecall,const std::string& ip,int port)
	{
		if(use_filter)
		{
			if(ip==m_filter_ip&&port==m_filter_port)
			{
				///pass
			}
			else
			{
				return;
			}
		}
		std::string log_prefix;
		JLexical::Cast(port,log_prefix);
		log_prefix=("["+ip+":"+log_prefix+"] ");

		vector<char> send;
		send.resize(18);

		Sender sdr;
		DBStateReq dbpo;
		dbpo.which=109;

		std::map<std::string,std::string> reqmap;
		JLexical::Cast(m_start_pos,reqmap["start"]);
		JLexical::Cast(m_length_p,reqmap["length"]);
		dbpo.reserved=parseMapSS2String(reqmap);

		vector<char> recv;

		sdr.clear();
		sdr.setUri(DBStateReq::uri);
		sdr.setResCode(200);
		sdr.marshall(dbpo);
		sdr.endPack();
		send.assign(sdr.header(),sdr.header()+sdr.bodySize()+sdr.headerSize());
		int ret=p_remotecall->call( send, general_frame::instance(),recv, 8000);
		if(ret<0)
		{
			cout <<log_prefix <<"can't call" <<endl;
			return;
		}
		//////////
		Request reqest(&recv[0],recv.size());
		DBStateResp op_resp;
		try
		{
			reqest.head();
			if(reqest.getUri()!=DBStateResp::uri)
			{
				cout <<log_prefix <<"uri not matched" <<endl;
				return;
			}
			op_resp.unmarshal(reqest.getPackData());
		}
		catch(sox::PacketError pe)
		{
			cout <<log_prefix <<"un marshal error" <<endl;
			return;
		}
		if(op_resp.which!=dbpo.which)
		{
			cout <<log_prefix <<"which not matched" <<endl;
			return;
		}

		cout <<log_prefix <<" start" <<endl;
		std::string unzip;
		encryption::zlib_decompress(op_resp.result,unzip);
		cout <<unzip <<endl;

	}
};


struct RCallImpUpdateConf : public IRCall
{
	bool use_filter;
	std::string m_filter_ip;
	int m_filter_port;
	std::string conf_file;
	RCallImpUpdateConf()
	{
		use_filter=false;
	}
	~RCallImpUpdateConf()
	{

	}
	void do_rc(remote_call* p_remotecall,const std::string& ip,int port)
	{
		if(use_filter)
		{
			if(ip==m_filter_ip&&port==m_filter_port)
			{
				///pass
			}
			else
			{
				return;
			}
		}
		std::string log_prefix;
		JLexical::Cast(port,log_prefix);
		log_prefix=("["+ip+":"+log_prefix+"] ");

		vector<char> send;
		send.resize(18);

		Sender sdr;
		DBAdminReq dbpo;
		dbpo.req_id=rand();
		dbpo.option=109;
		dbpo.command=conf_file;

		vector<char> recv;

		sdr.clear();
		sdr.setUri(DBAdminReq::uri);
		sdr.setResCode(200);
		sdr.marshall(dbpo);
		sdr.endPack();
		send.assign(sdr.header(),sdr.header()+sdr.bodySize()+sdr.headerSize());
		int ret=p_remotecall->call( send, general_frame::instance(),recv, 5000);
		if(ret<0)
		{
			cout <<log_prefix <<"can't call" <<endl;
			return;
		}
		//////////
		Request reqest(&recv[0],recv.size());
		DBAdminResp op_resp;
		try
		{
			reqest.head();
			if(reqest.getUri()!=DBAdminResp::uri)
			{
				cout <<log_prefix <<"uri not matched" <<endl;
				return;
			}
			op_resp.unmarshal(reqest.getPackData());
		}
		catch(sox::PacketError pe)
		{
			cout <<log_prefix <<"un marshal error" <<endl;
			return;
		}
		if(op_resp.req_id!=dbpo.req_id)
		{
			cout <<log_prefix <<"req_id not matched" <<endl;
			return;
		}
		if(op_resp.result==0)
		{
			cout <<log_prefix <<"ok" <<endl;
		}
		else
		{
			cout <<log_prefix <<"error:" <<op_resp.result <<endl;
		}

	}
};

struct RCallImpTmpBlock : public IRCall
{
	bool use_filter;
	std::string m_filter_ip;
	int m_filter_port;
	std::string mapcmd;
	RCallImpTmpBlock()
	{
		use_filter=false;
	}
	~RCallImpTmpBlock()
	{

	}
	void do_rc(remote_call* p_remotecall,const std::string& ip,int port)
	{
		if(use_filter)
		{
			if(ip==m_filter_ip&&port==m_filter_port)
			{
				///pass
			}
			else
			{
				return;
			}
		}
		std::string log_prefix;
		JLexical::Cast(port,log_prefix);
		log_prefix=("["+ip+":"+log_prefix+"] ");

		vector<char> send;
		send.resize(18);

		Sender sdr;
		DBAdminReq dbpo;
		dbpo.req_id=rand();
		dbpo.option=110;
		dbpo.command=mapcmd;

		vector<char> recv;

		sdr.clear();
		sdr.setUri(DBAdminReq::uri);
		sdr.setResCode(200);
		sdr.marshall(dbpo);
		sdr.endPack();
		send.assign(sdr.header(),sdr.header()+sdr.bodySize()+sdr.headerSize());
		int ret=p_remotecall->call( send, general_frame::instance(),recv, 5000);
		if(ret<0)
		{
			cout <<log_prefix <<"can't call" <<endl;
			return;
		}
		//////////
		Request reqest(&recv[0],recv.size());
		DBAdminResp op_resp;
		try
		{
			reqest.head();
			if(reqest.getUri()!=DBAdminResp::uri)
			{
				cout <<log_prefix <<"uri not matched" <<endl;
				return;
			}
			op_resp.unmarshal(reqest.getPackData());
		}
		catch(sox::PacketError pe)
		{
			cout <<log_prefix <<"un marshal error" <<endl;
			return;
		}
		if(op_resp.req_id!=dbpo.req_id)
		{
			cout <<log_prefix <<"req_id not matched" <<endl;
			return;
		}
		if(op_resp.result==0)
		{
			cout <<log_prefix <<"ok" <<endl;
		}
		else
		{
			cout <<log_prefix <<"error:" <<op_resp.result <<endl;
		}

	}
};


void g_cutter_space(string& in,vector<string>& slice)
{
	slice.clear();
	size_t pos=0;
	for(;;)
	{
		size_t r=in.find_first_of(' ',pos);
		if(r==string::npos)
		{
			if(pos<in.size())
			{
				slice.push_back(in.substr(pos));
			}
			break;
		}
		if(pos<r)
		{
			slice.push_back(in.substr(pos,r-pos));
		}
		pos=r+1;
	}
}

int main(int argc,char* argv[])
{
	if(argc!=2)
	{
		cerr <<"Usage: " <<argv[0] <<" <xml_conf>" <<endl;
		return -1;
	}
	PxGlobalConf pgc;
	if(pgc.init(argv[1])<0)
	{
		cerr <<"conf init error" <<endl;
		return -1;
	}
	gl_new_conf(&pgc);
	
	string this_command_line;
	for(;;)
	{
		cout <<"db > "<<flush;
		if(!getline(cin,this_command_line))
		{
			break;
		}
		vector<string> command_args;
		g_cutter_space(this_command_line,command_args);
		if(command_args.empty())
		{
			cout <<"command not empty, try 'help'" <<endl;
			continue;
		}
		if(command_args[0]==std::string("exit"))///get state
		{
			cout <<"bye" <<endl;
			return 0;
		}
		if(command_args[0]==std::string("help"))
		{
			cout <<"supported command:" <<endl;
			cout <<"help" <<endl;
			cout <<"exit" <<endl;
			cout <<"get_all_md5" <<endl;
			cout <<"sync_globalconf" <<endl;
			cout <<"sync_one <ip> <port>" <<endl;
			cout <<"get_all_stat <start> <length>" <<endl;
			cout <<"get_one_stat <start> <length> <ip> <port>" <<endl;
			cout <<"tmpblock_one <ip> <port> <map>(app=$,dao=$,sendperten=$,charge=$)" <<endl;
		}
		else if(command_args[0]==std::string("get_all_md5"))
		{
			RCallImpCheckMD5 impmd5;
			gl_call_all(&impmd5);
			cout <<"mymd5:" <<pgc.getGlobalConfMd5()<<endl;
		}
		else if(command_args[0]==std::string("sync_globalconf"))
		{
			RCallImpUpdateConf impsync_conf;
			impsync_conf.conf_file=pgc.last_load_from_file;
			gl_call_all(&impsync_conf);
		}
		else if(command_args[0]==std::string("tmpblock_one"))
		{
			if(command_args.size()!=4)
			{
				cout <<"format incorrect, do as follow: " <<endl;
				cout <<"tmpblock_one <ip> <port> <map>(app=$,dao=$,sendperten=$,charge=$)" <<endl;
				continue;
			}
			RCallImpTmpBlock imptmp_block;
			imptmp_block.use_filter=true;
			imptmp_block.m_filter_ip=command_args[1];
			JLexical::Cast(command_args[2],imptmp_block.m_filter_port);
			imptmp_block.mapcmd =command_args[3];
			gl_call_all(&imptmp_block);
		}
		else if(command_args[0]==std::string("sync_one"))
		{
			if(command_args.size()!=3)
			{
				cout <<"format incorrect, do as follow: " <<endl;
				cout <<"sync_one <ip> <port>" <<endl;
				continue;
			}
			RCallImpUpdateConf impsync_conf;
			impsync_conf.use_filter=true;
			impsync_conf.m_filter_ip=command_args[1];
			JLexical::Cast(command_args[2],impsync_conf.m_filter_port);
			impsync_conf.conf_file=pgc.last_load_from_file;
			gl_call_all(&impsync_conf);
		}
		else if(command_args[0]==std::string("get_all_stat"))
		{
			if(command_args.size()!=3)
			{
				cout <<"format incorrect, do as follow: " <<endl;
				cout <<"get_all_stat <start> <length>" <<endl;
				continue;
			}
			RGetRangeStat rangstat;
			rangstat.use_filter=false;
			JLexical::Cast(command_args[1],rangstat.m_start_pos);
			JLexical::Cast(command_args[2],rangstat.m_length_p);
			gl_call_all(&rangstat);
		}
		else if(command_args[0]==std::string("get_one_stat"))
		{
			if(command_args.size()!=5)
			{
				cout <<"format incorrect, do as follow: " <<endl;
				cout <<"get_one_stat <start> <length> <ip> <port>" <<endl;
				continue;
			}
			RGetRangeStat rangstat;
			rangstat.use_filter=true;
			rangstat.m_filter_ip=command_args[3];
			JLexical::Cast(command_args[4],rangstat.m_filter_port);
			JLexical::Cast(command_args[1],rangstat.m_start_pos);
			JLexical::Cast(command_args[2],rangstat.m_length_p);
			gl_call_all(&rangstat);
		}
		else
		{
			cout <<"command unknown, use 'help'" <<endl;
		}
	}

	
	return 0;
}


