#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
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
#include "time_util.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <cstring>

using namespace std;
using namespace seda;
using namespace server::dao_proxy;
using namespace server::db_node;

struct GLConf
{
	PxGlobalConf* gl_pglobal_conf;
	void init()
	{
		gl_pglobal_conf->get_all_dao(outdao);
		gl_pglobal_conf->get_all_proxy(outproxy);
		///////////
		std::map<std::string, PxGlobalConf::PrcAdd>::iterator it;
		for(it=outdao.begin();it!=outdao.end();++it)
		{
			string full_ippss;
			parseVecIPP2Str(it->second.ipports,full_ippss);
			string ky;
			for(size_t yu=0;yu<it->second.ipports.size();++yu)
			{
				string tem;
				JLexical::Cast(it->second.ipports[yu].second,tem);
				ky=it->second.ipports[yu].first+":"+tem;
				m_ipp_sn[ky]=it->first;
				m_ipp_multi_ipps[ky]=full_ippss;
			}
		}
		for(it=outproxy.begin();it!=outproxy.end();++it)
		{
			string full_ippss;
			parseVecIPP2Str(it->second.ipports,full_ippss);
			string ky;
			for(size_t yu=0;yu<it->second.ipports.size();++yu)
			{
				string tem;
				JLexical::Cast(it->second.ipports[yu].second,tem);
				ky=it->second.ipports[yu].first+":"+tem;
				m_ipp_sn[ky]=it->first;
				m_ipp_multi_ipps[ky]=full_ippss;
			}
		}
	}
	bool get_proxy_by_oneip(string& ipp,int& group_id,string& ipps)
	{
		std::map<string,string>::iterator itr=m_ipp_sn.find(ipp);
		if(itr==m_ipp_sn.end())
		{
			return false;
		}
		std::map<std::string, PxGlobalConf::PrcAdd>::iterator iv=
			outproxy.find(itr->second);
		if(iv==outproxy.end())
		{
			return false;
		}
		group_id=iv->second.groupid;
		parseVecIPP2Str(iv->second.ipports,ipps);
		return true;
	}
	bool getdao_by_sname(string& sname,string& ipp,int& group_id)
	{
		std::map<std::string, PxGlobalConf::PrcAdd>::iterator iv=
			outdao.find(sname);
		if(iv==outdao.end())
		{
			return false;
		}
		group_id=iv->second.groupid;
		parseVecIPP2Str(iv->second.ipports,ipp);
		return true;
	}
	std::map<std::string, PxGlobalConf::PrcAdd> outdao;
	std::map<std::string, PxGlobalConf::PrcAdd> outproxy;
	std::map<string,string> m_ipp_sn;
	std::map<string,string> m_ipp_multi_ipps;
};

GLConf gl_global_conf_mp;

///Linux dir

bool LinuxGetList(const char* dir,std::vector<std::string>& _dat)
{
	_dat.clear();
	DIR * pdir=opendir(dir);
	if(pdir==0)
	{
		return false;
	}
	dirent* dir_item;
	while((dir_item=readdir(pdir))!=0)
	{
		if( strcmp(dir_item->d_name,".")==0||
			strcmp(dir_item->d_name,"..")==0)
			continue;
		std::string get=dir_item->d_name;
		if(!get.empty()&&get[0]!='.')
		{
			_dat.push_back(get);
		}
	}
	////////
	closedir(pdir);
	return true;
}


//parse example
//
//[121.14.241.43:19090]  start
//<start: 121.14.241.43_30811_1> times:1331619159 T:2012-3-13 14:12:39
//<global> send:59860 recv:59998 drop:0 resp:14615.6 drop_rate:0%
//<groupid:97> send:59860 recv:59998 drop:0 resp:14615.6 drop_rate:0%
//<app:we> send:59860 recv:59998 drop:0 resp:14615.6 drop_rate:0%
//<dao:dao_malfunction> send:59860 recv:59998 drop:0 resp:14615.6 drop_rate:0%
//<sname:0 dao:dao_malfunction> send:30028 recv:30095 drop:0 resp:14620.3 drop_rate:0% predict_resp:53576.9
//<sname:1 dao:dao_malfunction> send:29832 recv:29903 drop:0 resp:14610.9 drop_rate:0% predict_resp:54588.4
//<app:we dao:dao_malfunction> send:59860 recv:59998 drop:0 resp:14615.6 drop_rate:0%
//<error_code_count: > 
//####
int parse_colon_string2map(std::string& ss,map<string,string>& mp)
{
	size_t pos=0;
	mp.clear();
	int pr_state=0;//pr_state 0:init 1:key 2:value
	string last_key;
	for(;;)
	{
		if(pos>=ss.size()||pos==ss.npos)
		{
			//finish
			break;
		}
		bool colon_trigger=false;
		string token;
		size_t cloc=ss.find_first_of(':',pos);
		size_t sloc=ss.find_first_of(' ',pos);
		////
		if(cloc==ss.npos)
		{
			if(sloc==ss.npos)
			{
				////////
				token=ss.substr(pos);
				pos=ss.size();
			}
			else
			{
				token=ss.substr(pos,sloc-pos);
				pos=sloc+1;
			}
		}
		else
		{
			if(sloc==ss.npos)
			{
				token=ss.substr(pos,cloc-pos);
				pos=cloc+1;
				colon_trigger=true;
			}
			else
			{
				if(cloc<sloc)
				{
					token=ss.substr(pos,cloc-pos);
					pos=cloc+1;
					colon_trigger=true;
				}
				else
				{
					token=ss.substr(pos,sloc-pos);
					pos=sloc+1;
				}
			}
		}
		
		/////////////////
		int round=0;
		if(!token.empty())
		{
			round++;
		}
		if(colon_trigger)
		{
			round++;
		}
		for(int q=0;q<round;++q)
		{
		if(pr_state==0)
		{
			if(!token.empty())
			{
				last_key=token;
				pr_state=1;
			}
		}
		else if(pr_state==1)
		{
			if(colon_trigger)
			{
				pr_state=2;
			}
		}
		else if(pr_state==2)
		{
			if(!token.empty())
			{
				mp[last_key]=token;
				pr_state=0;
			}
		}
		}		
	}
	/////////
	return 0;
}
struct an_bracket
{
	string keystr;
	string valuestr;
	int parse(std::string& line)
	{
		size_t p=line.find_first_of('<');
		if(p==line.npos)
		{return 1;}
		size_t l=line.find_first_of('>');
		if(l==line.npos)
		{return 2;}
		p++;//omit [
		if(!(l>=1+p))
		{return 3;}
		/////////////
		keystr=line.substr(p,l-p);
		valuestr=line.substr(l+1);
		return 0;
	}
};
struct PrcNode
{
	string ip;
	int port;
	//return 0 if success
	int parse(std::string& line)
	{
		size_t p=line.find_first_of('[');
		if(p==line.npos)
		{return 1;}
		size_t l=line.find_first_of(']');
		if(l==line.npos)
		{return 2;}
		p++;//omit [
		if(!(l>=3+p))
		{return 3;}
		/////////////
		std::string ipps=line.substr(p,l-p);
		size_t cm=ipps.find_first_of(':');
		if(cm==ipps.npos)
		{return 4;}
		ip=ipps.substr(0,cm);
		string postr=ipps.substr(cm+1);
		port=0;
		JLexical::Cast(postr,port);
		return 0;
	}
	void to_stream(ostream& os)
	{
		os<<"[" <<ip <<":" <<port <<"]" <<endl;
	}
};
struct Packey
{
	Packey(){}
	~Packey(){}
	Packey(const Packey &o)
	{
		pack_id=o.pack_id;
		times=o.times;
	}
	Packey& operator = (const Packey &o)
	{
		pack_id=o.pack_id;
		times=o.times;
		return *this;
	}
	bool operator < (const Packey &o) const
	{
		if(times<o.times)
		{
			return true;
		}
		else if(times > o.times)
		{
			return false;
		}
		return pack_id<o.pack_id;
	}
	string pack_id;
	uint64_t times;

	void clear()
	{
		pack_id="";
		times=0;
	}
	int parse(string& fstr,string& lstr)
	{
		map<string,string> fm;
		parse_colon_string2map(fstr,fm);
		string findk="start";
		map<string,string>::iterator mpi=fm.find(findk);
		if(mpi==fm.end())
		{
			return 1;
		}
		pack_id=mpi->second;
		/////////////////
		parse_colon_string2map(lstr,fm);
		findk="times";
		mpi=fm.find(findk);
		if(mpi==fm.end())
		{
			return 2;
		}
		times=0;
		JLexical::Cast(mpi->second,times);
		return 0;
	}
	void to_stream(ostream& os)
	{
		os <<"<start: " <<pack_id <<"> times:" <<times <<endl;
	}
};
struct StatItem
{
	StatItem(){};
	~StatItem(){};
	StatItem(const StatItem& o)
	{
		key=o.key;
		send=o.send;
		recv=o.recv;
		drop=o.drop;
		resp=o.resp;
	}
	StatItem& operator = (const StatItem& o)
	{
		key=o.key;
		send=o.send;
		recv=o.recv;
		drop=o.drop;
		resp=o.resp;
		return *this;
	}
	map<string,string> key;
	///
	uint64_t send;
	uint64_t recv;
	uint64_t drop;
	double resp;
	void to_stream(ostream& os)
	{
		if(key.empty())
		{
			os<<"<global> " ;
		}
		else
		{
			os<<"<";
			for(map<string,string>::iterator i=key.begin();i!=key.end();++i)
			{
				if(i!=key.begin())
				{
					os <<" ";
				}
				os<<i->first <<":" <<i->second ;
			}
			os <<">";
		}
		os<<" send:" <<send
			<<" recv:" <<recv
			<<" drop:" <<drop
			<<" resp:" <<resp <<endl;
	}
	int parse(string& fstr,string& lstr)
	{
		map<string,string> fm;
		parse_colon_string2map(fstr,fm);
		map<string,string> fv;
		parse_colon_string2map(lstr,fv);

		map<string,string>::iterator mps=fv.find(string("send"));
		map<string,string>::iterator mpr=fv.find(string("recv"));
		map<string,string>::iterator mpd=fv.find(string("drop"));
		map<string,string>::iterator mpe=fv.find(string("resp"));
		if(mps==fv.end()
			||mpr==fv.end()
			||mpd==fv.end()
			||mpe==fv.end()
			)
		{
			return 1;
		}
		key=fm;
		send=0;
		recv=0;
		drop=0;
		resp=0;
		JLexical::Cast(mps->second,send);
		JLexical::Cast(mpr->second,recv);
		JLexical::Cast(mpd->second,drop);
		JLexical::Cast(mpe->second,resp);
		return 0;
	}
};

void strip_ch(string& str,char ch)
{
        size_t fp=str.find_first_not_of(ch);
        if(fp==str.npos)
        {
                str.clear();
                return;
        }
        size_t lp=str.find_last_not_of(ch);
        str=str.substr(fp,lp-fp+1);
}

struct ErrorItem
{
	map<int,int> error_mp;
	void clear()
	{
		error_mp.clear();
	}
	void to_stream(ostream& os)
	{
		os <<"<error_code_count: > ";
		for(map<int,int>::iterator i=error_mp.begin();i!=error_mp.end();++i)
		{
			if(i!=error_mp.begin())
			{
				os <<",";
			}
			os <<i->first <<"=" <<i->second;
		}
		os <<endl;
	}
	int parse(string& fstr,string& lstr)
	{
		string matchc="error_code_count:";
		if(fstr.find(matchc)==fstr.npos)
		{
			return 1;
		}
		string lss=lstr;
		strip_ch(lss,' ');
		std::map<std::string,std::string> ecc;
		parseString2MapSS(lss,ecc);
		std::map<std::string,std::string>::iterator gh=ecc.begin();
		for(;gh!=ecc.end();++gh)
		{
			int temk=0;
			int temv=0;
			JLexical::Cast(gh->first,temk);
			JLexical::Cast(gh->second,temv);
			if(temk==0||temv==0)
			{
				continue;
			}
			error_mp[temk]=temv;
		}
		return 0;
	}
};

/////////
struct LogicStatBasicPack
{
	Packey k;
	PrcNode node;
	vector<StatItem> stats;
	ErrorItem error;
	void clear_body()
	{
		k.clear();
		stats.clear();
		error.clear();
	}
	void to_stream(ostream& os)
	{
		node.to_stream(os);
		k.to_stream(os);
		for(size_t i=0;i<stats.size();++i)
		{
			stats[i].to_stream(os);
		}
		error.to_stream(os);
		os <<"####" <<endl;
	}
};

class IParse_Callback
{
public:
	virtual ~IParse_Callback(){};
	////return 0:ok  1:end next callback
	virtual int on_parse(const LogicStatBasicPack& pack)=0;
};

class Reader_fsm_parse
{
public:
	Reader_fsm_parse(){}
	~Reader_fsm_parse(){}
public:
	int do_parse(std::string& line)
	{
		//check_pack_end
		if(line.substr(0,4)==std::string("####"))
		{
			//trigger pack_fin
			for(size_t i=0;i<m_cbs.size();++i)
			{
				if(m_cbs[i]->on_parse(forming_pack)==1)
				{
					//end
					break;
				}
			}
			//clear some of forming_pack
			forming_pack.clear_body();
			return 0;
		}
		//check node_start
		if(forming_pack.node.parse(line)==0)
		{
			//new node is start
			//clear some of forming_pack
			forming_pack.clear_body();
			return 0;
		}
		//parse < >
		an_bracket abb;
		if(abb.parse(line)==0)
		{
			StatItem si;
			if(forming_pack.error.parse(abb.keystr,abb.valuestr)==0)
			{
				//ok
			}
			else if(forming_pack.k.parse(abb.keystr,abb.valuestr)==0)
			{
				//ok
			}
			else if(si.parse(abb.keystr,abb.valuestr)==0)
			{
				//ok
				forming_pack.stats.push_back(si);
			}
		}
		return 0;
	}
	void regist_call_back(IParse_Callback* cb)
	{
		m_cbs.push_back(cb);
	}
private:
	LogicStatBasicPack forming_pack;
	//////////////
	vector<IParse_Callback* > m_cbs;
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


////////////////////statstic pack define

struct StatIndex
{
	StatIndex()
	{
		send=0;
		recv=0;
		drop=0;
		resp=0.0;
	}
	~StatIndex(){}
	uint32_t send;
	uint32_t recv;
	uint32_t drop;
	double resp;
	map<int,int> error_code;
	void to_stream(ostream& os)
	{
		os <<"send: " <<send <<" recv:" <<recv <<" resp:" <<resp <<" drop:" <<drop;
		os <<" error_code:";
		if(error_code.empty())
		{
			os <<"empty";
		}
		for(map<int,int>::iterator i=error_code.begin();i!=error_code.end();++i)
		{
			if(i!=error_code.begin())
			{
				os <<",";
			}
			os <<i->first <<"=" <<i->second;
		}
		os <<endl;
	}
	StatIndex(const StatIndex& o)
	{
		send=o.send;
		recv=o.recv;
		drop=o.drop;
		resp=o.resp;
		error_code=o.error_code;
	}
	StatIndex& operator = (const StatIndex& o)
	{
		send=o.send;
		recv=o.recv;
		drop=o.drop;
		resp=o.resp;
		error_code=o.error_code;
		return *this;
	}
	StatIndex& operator += (const StatIndex& o)
	{
		double new_resp=0.0;
		uint32_t sumr=recv+o.recv;
		if(sumr!=0)
		{
			new_resp=resp*((double)recv/(double)sumr)
				+o.resp*((double)o.recv/(double)sumr);
		}
		send+=o.send;
		recv+=o.recv;
		drop+=o.drop;
		resp=new_resp;
		map<int,int>& cast_error_code=const_cast<map<int,int>& >(o.error_code);
		map<int,int>::iterator ci;
		for(ci=cast_error_code.begin();ci!=cast_error_code.end();++ci)
		{
			map<int,int>::iterator b=error_code.find(ci->first);
			if(b==error_code.end())
			{
				error_code[ci->first]=ci->second;
			}
			else
			{
				b->second+=ci->second;
			}
		}
		return *this;
	}
};

struct SingleStatPack
{
public:
	SingleStatPack(){}
	~SingleStatPack(){}
	SingleStatPack(const SingleStatPack& o)
	{
		timestamp=o.timestamp;
		srv_name=o.srv_name;
		srv_ipport=o.srv_ipport;
		srv_groupid=o.srv_groupid;
		fun_type=o.fun_type;
		si=o.si;
	}
	SingleStatPack& operator = (const SingleStatPack& o)
	{
		timestamp=o.timestamp;
		srv_name=o.srv_name;
		srv_ipport=o.srv_ipport;
		srv_groupid=o.srv_groupid;
		fun_type=o.fun_type;
		si=o.si;
		return *this;
	}
	uint64_t timestamp;
	string srv_name;
	string srv_ipport;
	int srv_groupid;
	string fun_type;
	StatIndex si;
	string get_by_string(string& kys)
	{
		if(kys==string("srv_name"))
		{
			return srv_name;
		}
		else if(kys==string("srv_ipport"))
		{
			return srv_ipport;
		}
		else if(kys==string("srv_groupid"))
		{
			string temi;
			JLexical::Cast(srv_groupid,temi);
			return temi;
		}
		else if(kys==string("fun_type"))
		{
			return fun_type;
		}
		return std::string("");
	}
	bool match(std::map<string,string>& mk)
	{
		std::map<string,string>::iterator v;
		for(v=mk.begin();v!=mk.end();++v)
		{
			if(v->first==string("srv_name"))
			{
				if(srv_name!=v->second)
				{
					return false;
				}
			}
			else if(v->first==string("srv_ipport"))
			{
				if(srv_ipport!=v->second)
				{
					return false;
				}
			}
			else if(v->first==string("srv_groupid"))
			{
				string temi;
				JLexical::Cast(srv_groupid,temi);
				if(temi!=v->second)
				{
					return false;
				}
			}
			else if(v->first==string("fun_type"))
			{
				if(fun_type!=v->second)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		return true;
	}
};


struct PairStatPack
{
public:
	PairStatPack(){}
	~PairStatPack(){}
	PairStatPack(const PairStatPack& o)
	{
		timestamp=o.timestamp;
		src_name=o.src_name;
		src_ipport=o.src_ipport;
		src_groupid=o.src_groupid;
		fromreq_type=o.fromreq_type;
		des_name=o.des_name;
		des_ipport=o.des_ipport;
		des_groupid=o.des_groupid;
		toreq_type=o.toreq_type;
		si=o.si;
	}
	PairStatPack& operator =(const PairStatPack& o)
	{
		timestamp=o.timestamp;
		src_name=o.src_name;
		src_ipport=o.src_ipport;
		src_groupid=o.src_groupid;
		fromreq_type=o.fromreq_type;
		des_name=o.des_name;
		des_ipport=o.des_ipport;
		des_groupid=o.des_groupid;
		toreq_type=o.toreq_type;
		si=o.si;
		return *this;
	}
	uint64_t timestamp;
	string src_name;
	string src_ipport;
	int src_groupid;
	string des_name;
	string des_ipport;
	int des_groupid;
	string fromreq_type;
	string toreq_type;
	StatIndex si;
	string get_by_string(string& kys)
	{
		if(kys==string("src_name"))
		{
			return src_name;
		}
		else if(kys==string("src_ipport"))
		{
			return src_ipport;
		}
		else if(kys==string("src_groupid"))
		{
			string temi;
			JLexical::Cast(src_groupid,temi);
			return temi;
		}
		else if(kys==string("fromreq_type"))
		{
			return fromreq_type;
		}
		else if(kys==string("des_name"))
		{
			return des_name;
		}
		else if(kys==string("des_ipport"))
		{
			return des_ipport;
		}
		else if(kys==string("des_groupid"))
		{
			string temi;
			JLexical::Cast(des_groupid,temi);
			return temi;
		}
		else if(kys==string("toreq_type"))
		{
			return toreq_type;
		}
		return std::string("");
	}
	bool match(std::map<string,string>& mk)
	{
		std::map<string,string>::iterator v;
		for(v=mk.begin();v!=mk.end();++v)
		{
			if(v->first==string("src_name"))
			{
				if(src_name!=v->second)
				{
					return false;
				}
			}
			else if(v->first==string("src_ipport"))
			{
				if(src_ipport!=v->second)
				{
					return false;
				}
			}
			else if(v->first==string("src_groupid"))
			{
				string temi;
				JLexical::Cast(src_groupid,temi);
				if(temi!=v->second)
				{
					return false;
				}
			}
			else if(v->first==string("fromreq_type"))
			{
				if(fromreq_type!=v->second)
				{
					return false;
				}
			}
			else if(v->first==string("des_name"))
			{
				if(des_name!=v->second)
				{
					return false;
				}
			}
			else if(v->first==string("des_ipport"))
			{
				if(des_ipport!=v->second)
				{
					return false;
				}
			}
			else if(v->first==string("des_groupid"))
			{
				string temi;
				JLexical::Cast(des_groupid,temi);
				if(temi!=v->second)
				{
					return false;
				}
			}
			else if(v->first==string("toreq_type"))
			{
				if(toreq_type!=v->second)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		return true;
	}
};

////////////////////apps//////////////////
////////////////////apps//////////////////
////////////////////apps//////////////////
////////////////////apps//////////////////
////////////////////apps//////////////////
////////////////////apps//////////////////
////////////////////apps//////////////////
////////////////////apps//////////////////
////////////////////apps//////////////////

class statDebugParse : public IParse_Callback
{
public:
	virtual int on_parse(const LogicStatBasicPack& pack)
	{
		LogicStatBasicPack& npack=const_cast<LogicStatBasicPack&>(pack);
		npack.to_stream(cout);
		return 0;
	}
};

void gl_show_result(map<uint64_t,StatIndex>& accm)
{
	map<uint64_t,StatIndex>::iterator ig
		=accm.begin();
	for(;ig!=accm.end();++ig)
	{
		cout <<"<times:" <<ig->first <<"> ";
		ig->second.to_stream(cout);
	}
}

void gl_sum_to_it(map<uint64_t,StatIndex>& accm,uint64_t tim,StatIndex& si)
{
	map<uint64_t,StatIndex>::iterator ig
		=accm.find(tim);
	if(ig==accm.end())
	{
		accm[tim]=si;
	}
	else
	{
		ig->second+=si;
	}
}

struct SndDistribute
{
	SndDistribute(){}
	~SndDistribute(){}
	SndDistribute(const SndDistribute& o)
	{
		distri=o.distri;
	}
	SndDistribute& operator = (const SndDistribute& o)
	{
		distri=o.distri;
		return *this;
	}
	map<string,uint64_t > distri;
	void clear()
	{
		distri.clear();
	}
	void add(string& ip,uint64_t cnt)
	{
		if(cnt==0)
		{
			return;
		}
		map<string,uint64_t >::iterator it
			=distri.find(ip);
		if(it==distri.end())
		{
			distri[ip]=cnt;
		}
		else
		{
			it->second+=cnt;
		}
	}
	//return [0--100] score
	static int diff_rate(SndDistribute& lhs,SndDistribute& rhs)
	{
		double suml=0.0;
		double sumr=0.0;
		map<string,uint64_t >::iterator li;
		map<string,uint64_t >::iterator ri;
		for(li=lhs.distri.begin();li!=lhs.distri.end();++li)
		{
			suml+=li->second;
		}
		for(ri=rhs.distri.begin();ri!=rhs.distri.end();++ri)
		{
			sumr+=ri->second;
		}
		///let there mod be 50, => make max diff equal 100
		suml+=30;//dist base
		sumr+=30;//dist base
		suml/=50.0;
		sumr/=50.0;
		///
		map<string,double > diff_mp;
		map<string,double >::iterator dmi;
		for(li=lhs.distri.begin();li!=lhs.distri.end();++li)
		{
			diff_mp[li->first]=((double)li->second/suml);
		}
		//////////
		for(ri=rhs.distri.begin();ri!=rhs.distri.end();++ri)
		{
			dmi=diff_mp.find(ri->first);
			if(dmi==diff_mp.end())
			{
				diff_mp[ri->first]=((double)ri->second/sumr);
			}
			else
			{
				double tmd=((double)ri->second/sumr);
				if(dmi->second>=tmd)
				{
					dmi->second-=tmd;
				}
				else
				{
					tmd-=dmi->second;
					dmi->second=tmd;
				}
			}
		}
		/////////
		double sumdiff=0;
		for(dmi=diff_mp.begin();dmi!=diff_mp.end();++dmi)
		{
			sumdiff+=dmi->second;
		}
		int restn=(sumdiff+0.5);
		if(restn<0)
		{
			restn=0;
		}
		if(restn>100)
		{
			restn=100;
		}
		return restn;
	}
};

struct StatIndexEx
{
	StatIndexEx()
	{
		send=0;
		recv=0;
		drop=0;
		resp=0.0;
		///////////////
		resp_diff=0;
		send_diff=0;
		dist_diff=0;
		///////////
		trouble_idx=0;
	}
	~StatIndexEx(){}
	uint32_t send;
	uint32_t recv;
	uint32_t drop;
	double resp;
	map<int,int> error_code;
	////////////
	int resp_diff;//[0,100]
	int send_diff;//[0,100]
	int dist_diff;//[0,100]
	////////////
	double trouble_idx;
	void cal_trouble()
	{
		double rv_sum=drop+recv;
		rv_sum+=1;
		rv_sum/=10000.0;
		double dropixd=((double)drop/(double)rv_sum);
		int dropix=(dropixd+0.5);
		if(dropix<0)
		{
			dropix=0;
		}
		else if(dropix>10000)
		{
			dropix=10000;
		}
		if(drop!=0&&dropix==0)
		{
			dropix=1;
		}
		////
		trouble_idx=dropix+dist_diff+send_diff+resp_diff;
	}

	void to_stream(ostream& os)
	{
		os <<"send:" <<send <<" recv:" <<recv <<" resp:" <<resp <<" drop:" <<drop;
		os <<" resp_diff:" <<resp_diff <<" send_diff:" <<send_diff <<" dist_diff:"<<dist_diff;
		os <<" trouble_index:" <<trouble_idx;
		os <<" error_code:";
		if(error_code.empty())
		{
			os <<"empty";
		}
		for(map<int,int>::iterator i=error_code.begin();i!=error_code.end();++i)
		{
			if(i!=error_code.begin())
			{
				os <<",";
			}
			os <<i->first <<"=" <<i->second;
		}
		os <<endl;
	}
	StatIndexEx(const StatIndexEx& o)
	{
		send=o.send;
		recv=o.recv;
		drop=o.drop;
		resp=o.resp;
		error_code=o.error_code;
		resp_diff=o.resp_diff;
		send_diff=o.send_diff;
		dist_diff=o.dist_diff;
		trouble_idx=o.trouble_idx;
	}
	StatIndexEx& operator = (const StatIndexEx& o)
	{
		send=o.send;
		recv=o.recv;
		drop=o.drop;
		resp=o.resp;
		error_code=o.error_code;
		resp_diff=o.resp_diff;
		send_diff=o.send_diff;
		dist_diff=o.dist_diff;
		trouble_idx=o.trouble_idx;
		return *this;
	}
};
void calc_diffs(map<uint64_t,StatIndex>& m_sresult
	,map<uint64_t,SndDistribute>& m_dis_result
	,map<uint64_t,StatIndexEx>& m_ex);

void gl_show_result_ex(map<uint64_t,StatIndexEx>& accm,ostream& out_srm);

///////////////////////////////////
class UniqueParsefilter : public IParse_Callback
{
public:
	int gl_pack_filter(LogicStatBasicPack& pack)
	{
		pack.k.times/=10;
		pack.k.times*=10;
		std::set<Packey>::iterator ivr=gl_duplicate_rm.find(pack.k);
		if(ivr==gl_duplicate_rm.end())
		{
			gl_duplicate_rm.insert(pack.k);
			return 0;
		}
		return 1;
	}
	virtual int on_parse(const LogicStatBasicPack& pack)
	{
		LogicStatBasicPack& npack=const_cast<LogicStatBasicPack&>(pack);
		npack.k.times/=10;
		npack.k.times*=10;
		if( npack.k.times < mintimes || npack.k.times > maxtimes)
		{
			return 1;
		}
		if(gl_pack_filter(npack)!=0)
		{
			return 1;
		}
		return 0;
	}
	std::set<Packey> gl_duplicate_rm;
	uint64_t mintimes;
	uint64_t maxtimes;
};

class DistributParse : public IParse_Callback
{
public:
	virtual int on_parse(const LogicStatBasicPack& pack)
	{
		LogicStatBasicPack& npack=const_cast<LogicStatBasicPack&>(pack);
		string tepstr;
		JLexical::Cast(npack.node.port,tepstr);
		int npack_gid=0;
		string ippsky=npack.node.ip+":"+tepstr;
		string ipports_full;
		if(!gl_global_conf_mp.get_proxy_by_oneip(ippsky,npack_gid,ipports_full))
		{
			return 0;
		}
		////to pair packs
		for(size_t i=0;i<npack.stats.size();++i)
		{
			if(npack.stats[i].key.size()==2)
			{
				string mthhk="dao";
				map<string,string>::iterator ik
					=npack.stats[i].key.find(mthhk);
				if(ik==npack.stats[i].key.end())
				{
					continue;
				}
				////
				PairStatPack psp;
				////
				mthhk="app";
				map<string,string>::iterator iap
					=npack.stats[i].key.find(mthhk);
				if(iap!=npack.stats[i].key.end())
				{
					////do get one app=>dao stat
					psp.timestamp=npack.k.times;
					////from info
					psp.des_name="dao_proxy";
					psp.des_ipport=ipports_full;
					psp.des_groupid=npack_gid;
					psp.toreq_type=ik->second;
					////to info
					psp.src_name=iap->second;
					psp.src_ipport=psp.des_ipport;
					psp.src_groupid=psp.des_groupid;
					psp.fromreq_type=psp.toreq_type;
					////////////////////////
					psp.si.resp=npack.stats[i].resp;
					psp.si.drop=npack.stats[i].drop;
					psp.si.send=npack.stats[i].send;
					psp.si.recv=npack.stats[i].recv;
					////match
					trigger_single_p(psp);
				}
				else
				{
					mthhk="sname";
					map<string,string>::iterator isr
						=npack.stats[i].key.find(mthhk);
					if(isr!=npack.stats[i].key.end())
					{
						////do get one dao => sname stat
						psp.timestamp=npack.k.times;
						////from info
						psp.des_name="dao_server";
						string ippss;
						int gidddd=0;
						if(!gl_global_conf_mp.getdao_by_sname(isr->second,ippss,gidddd))
						{
							continue;
						}
						psp.des_ipport=ippss;
						psp.des_groupid=gidddd;
						psp.toreq_type=ik->second;
						////to info
						psp.src_name="dao_proxy";
						psp.src_ipport=ipports_full;
						psp.src_groupid=npack_gid;
						psp.fromreq_type=ik->second;
						////////////////////////
						psp.si.resp=npack.stats[i].resp;
						psp.si.drop=npack.stats[i].drop;
						psp.si.send=npack.stats[i].send;
						psp.si.recv=npack.stats[i].recv;
						////match
						trigger_single_p(psp);
					}
					else
					{
						continue;
					}
				}
			}
		}
		return 0;
	}
	void trigger_single_p(PairStatPack& psp)
	{
		////match
		if(psp.match(match))
		{
			//join result
			map<uint64_t,SndDistribute>::iterator irr=m_dis_result.find(psp.timestamp);
			if(irr==m_dis_result.end())
			{
				SndDistribute inis;
				inis.add(psp.des_ipport,psp.si.send);
				m_dis_result[psp.timestamp]=inis;
				return;
			}
			else
			{
				irr->second.add(psp.des_ipport,psp.si.send);
			}
		}
	}
	map<string,string> match;
	map<uint64_t,SndDistribute> m_dis_result;
	void cnv_match()
	{
		map<string,string> res_match;
		map<string,string>::iterator i=match.begin();
		for(;i!=match.end();++i)
		{
			if(i->first==string("srv_name"))
			{
				res_match["src_name"]=i->second;
			}
			else if(i->first==string("srv_ipport"))
			{
				res_match["src_ipport"]=i->second;
			}
			else if(i->first==string("srv_groupid"))
			{
				res_match["src_groupid"]=i->second;
			}
			else if(i->first==string("fun_type"))
			{
				res_match["fromreq_type"]=i->second;
			}
			else
			{
				res_match[i->first]=i->second;
			}
		}
		res_match.swap(match);
	}
};
struct ExtendInfoStatParse
{
	void sum_single_p(SingleStatPack& ssp)
	{
		string extr_k=ssp.get_by_string(extend_key);
		map<uint64_t,map<string,StatIndex> >::iterator itr
			=m_result.find(ssp.timestamp);
		if(itr==m_result.end())
		{
			m_result[ssp.timestamp][extr_k]=ssp.si;
			if(!extr_k.empty())
			{
				string emp="";
				m_result[ssp.timestamp][emp]=ssp.si;
			}
		}
		else
		{
			map<string,StatIndex>::iterator bn=itr->second.find(extr_k);
			if(bn==itr->second.end())
			{
				itr->second[extr_k]=ssp.si;
			}
			else
			{
				bn->second+=ssp.si;
			}
			///
			if(!extr_k.empty())
			{
				string emp="";
				itr->second[emp]+=ssp.si;
			}
		}
	}
	void sum_pair_p(PairStatPack& ssp)
	{
		string extr_k=ssp.get_by_string(extend_key);
		map<uint64_t,map<string,StatIndex> >::iterator itr
			=m_result.find(ssp.timestamp);
		if(itr==m_result.end())
		{
			m_result[ssp.timestamp][extr_k]=ssp.si;
			if(!extr_k.empty())
			{
				string emp="";
				m_result[ssp.timestamp][emp]=ssp.si;
			}
		}
		else
		{
			map<string,StatIndex>::iterator bn=itr->second.find(extr_k);
			if(bn==itr->second.end())
			{
				itr->second[extr_k]=ssp.si;
			}
			else
			{
				bn->second+=ssp.si;
			}
			///
			if(!extr_k.empty())
			{
				string emp="";
				itr->second[emp]+=ssp.si;
			}
		}
	}
	string extend_key;
	map<uint64_t,map<string,StatIndex> > m_result;
	void debug_to_stream(ostream& out_srm)
	{
		map<uint64_t,map<string,StatIndex> >::iterator ig
			=m_result.begin();
		for(;ig!=m_result.end();++ig)
		{
			out_srm <<"<times:" <<ig->first <<"> " ;
			ig->second[string("")].to_stream(out_srm);
			map<string,StatIndex>::iterator mpkv
				=ig->second.begin();
			for(;mpkv!=ig->second.end();++mpkv)
			{
				if(mpkv->first!=string(""))
				{
				out_srm <<"<" <<extend_key <<":" <<mpkv->first <<"> ";
				mpkv->second.to_stream(out_srm);
				}
			}
			out_srm <<"####" <<endl;
		}
	}
};
class SingleParse : public IParse_Callback
{
public:
	SingleParse()
	{
		eif=0;
		m_pofilterfile=0;
	}
	~SingleParse()
	{
	}
public:
	virtual int on_parse(const LogicStatBasicPack& pack)
	{
		LogicStatBasicPack& npack=const_cast<LogicStatBasicPack&>(pack);
		string tepstr;
		JLexical::Cast(npack.node.port,tepstr);
		int npack_gid=0;
		string ippsky=npack.node.ip+":"+tepstr;
		string ipports_full;
		if(!gl_global_conf_mp.get_proxy_by_oneip(ippsky,npack_gid,ipports_full))
		{
			return 0;
		}
		////to error info
		if(!npack.error.error_mp.empty())
		{
			SingleStatPack ssp;
			ssp.timestamp=npack.k.times;
			ssp.srv_name="dao_proxy";
			ssp.srv_ipport=ipports_full;
			ssp.srv_groupid=npack_gid;
			ssp.fun_type="";
			ssp.si.error_code=npack.error.error_mp;
			////match
			if(trigger_single_p(ssp))
			{
				if(m_pofilterfile!=0)
				{
				npack.to_stream(*m_pofilterfile);
				return 0;
				}
			}
		}
		////to single pack(s)
		for(size_t i=0;i<npack.stats.size();++i)
		{
			if(npack.stats[i].key.size()==1)
			{
				map<string,string>::iterator fdi=
					npack.stats[i].key.find(string("dao"));
				if(fdi!=npack.stats[i].key.end())
				{
					////////////////
					SingleStatPack ssp;
					ssp.timestamp=npack.k.times;
					ssp.srv_name="dao_proxy";
					ssp.srv_ipport=ipports_full;
					ssp.srv_groupid=npack_gid;
					ssp.fun_type=fdi->second;
					ssp.si.resp=npack.stats[i].resp;
					ssp.si.drop=npack.stats[i].drop;
					ssp.si.send=npack.stats[i].send;
					ssp.si.recv=npack.stats[i].recv;
					////match
					if(trigger_single_p(ssp))
					{
						if(m_pofilterfile!=0)
						{
						npack.to_stream(*m_pofilterfile);
						return 0;
						}
					}
				}
			}
		}
		//////////////////////////////
		return s_ppd.on_parse(pack);
	}
	bool trigger_single_p(SingleStatPack& ssp)
	{
		if(ssp.match(match))
		{
			if(eif!=0)
			{
				eif->sum_single_p(ssp);
			}
			else
			{
				////join result
				gl_sum_to_it(m_result,ssp.timestamp,ssp.si);
			}
			return true;
		}
		return false;
	}
	////
	ofstream* m_pofilterfile;
	map<string,string> match;
	////////////
	map<uint64_t,StatIndex> m_result;
	void set_match(string& match_key)
	{
		parseString2MapSS(match_key,match);
		s_ppd.match=match;
		s_ppd.cnv_match();
	}
	void endcal_andshow()
	{
		map<uint64_t,StatIndexEx> m_ex;
		calc_diffs(m_result,s_ppd.m_dis_result,m_ex);
		gl_show_result_ex(m_ex,cout);
	}
	void get_final_result_dif(map<uint64_t,StatIndexEx>& m_resultex)
	{
		m_resultex.clear();
		calc_diffs(m_result,s_ppd.m_dis_result,m_resultex);
	}
	DistributParse s_ppd;
	ExtendInfoStatParse* eif;
};
class PairParse : public IParse_Callback
{
public:
	virtual int on_parse(const LogicStatBasicPack& pack)
	{
		LogicStatBasicPack& npack=const_cast<LogicStatBasicPack&>(pack);
		string tepstr;
		JLexical::Cast(npack.node.port,tepstr);
		int npack_gid=0;
		string ippsky=npack.node.ip+":"+tepstr;
		string ipports_full;
		if(!gl_global_conf_mp.get_proxy_by_oneip(ippsky,npack_gid,ipports_full))
		{
			return 0;
		}
		////to pair packs
		for(size_t i=0;i<npack.stats.size();++i)
		{
			if(npack.stats[i].key.size()==2)
			{
				string mthhk="dao";
				map<string,string>::iterator ik
					=npack.stats[i].key.find(mthhk);
				if(ik==npack.stats[i].key.end())
				{
					continue;
				}
				////
				PairStatPack psp;
				////
				mthhk="app";
				map<string,string>::iterator iap
					=npack.stats[i].key.find(mthhk);
				if(iap!=npack.stats[i].key.end())
				{
					////do get one app=>dao stat
					psp.timestamp=npack.k.times;
					////from info
					psp.des_name="dao_proxy";
					psp.des_ipport=ipports_full;
					psp.des_groupid=npack_gid;
					psp.toreq_type=ik->second;
					////to info
					psp.src_name=iap->second;
					psp.src_ipport=psp.des_ipport;
					psp.src_groupid=psp.des_groupid;
					psp.fromreq_type=psp.toreq_type;
					////////////////////////
					psp.si.resp=npack.stats[i].resp;
					psp.si.drop=npack.stats[i].drop;
					psp.si.send=npack.stats[i].send;
					psp.si.recv=npack.stats[i].recv;
					////match
					if(trigger_single_p(psp))
					{
						if(m_pofilterfile!=0)
						{
						npack.to_stream(*m_pofilterfile);
						return 0;
						}
					}
				}
				else
				{
					mthhk="sname";
					map<string,string>::iterator isr
						=npack.stats[i].key.find(mthhk);
					if(isr!=npack.stats[i].key.end())
					{
						////do get one dao => sname stat
						psp.timestamp=npack.k.times;
						////from info
						psp.des_name="dao_server";
						string ippss;
						int gidddd=0;
						if(!gl_global_conf_mp.getdao_by_sname(isr->second,ippss,gidddd))
						{
							continue;
						}
						psp.des_ipport=ippss;
						psp.des_groupid=gidddd;
						psp.toreq_type=ik->second;
						////to info
						psp.src_name="dao_proxy";
						psp.src_ipport=ipports_full;
						psp.src_groupid=npack_gid;
						psp.fromreq_type=ik->second;
						////////////////////////
						psp.si.resp=npack.stats[i].resp;
						psp.si.drop=npack.stats[i].drop;
						psp.si.send=npack.stats[i].send;
						psp.si.recv=npack.stats[i].recv;
						////match
						if(trigger_single_p(psp))
						{
							if(m_pofilterfile!=0)
							{
							npack.to_stream(*m_pofilterfile);
							return 0;
							}
						}
					}
					else
					{
						continue;
					}
				}
			}
		}
		return 0;
	}
	bool trigger_single_p(PairStatPack& psp)
	{
		////match
		if(psp.match(match))
		{
			if(eif!=0)
			{
				eif->sum_pair_p(psp);
			}
			else
			{
				////join result
				gl_sum_to_it(m_result,psp.timestamp,psp.si);
			}
			return true;
		}
		return false;
	}
	ofstream* m_pofilterfile;
	map<string,string> match;
	///////////
	map<uint64_t,StatIndex> m_result;
	ExtendInfoStatParse* eif;
};


void gl_show_result_ex(map<uint64_t,StatIndexEx>& accm,ostream& out_srm)
{
	map<uint64_t,StatIndexEx>::iterator ig
		=accm.begin();
	for(;ig!=accm.end();++ig)
	{
		out_srm <<"<times:" <<ig->first <<"> ";
		ig->second.to_stream(out_srm);
	}
}

void gl_calc_diffcore(double lhs,double rhs,int& diff)
{
	double max;
	double min;
	if(lhs>=rhs)
	{
		max=lhs;
		min=(lhs-rhs);
	}
	else
	{
		max=rhs;
		min=(rhs-lhs);
	}
	max+=30;
	max/=100.0;//maximum 100

	double re=(min/max);
	diff=(re+0.5);
	if(diff<0)
	{
		diff=0;
	}
	else if(diff>100)
	{
		diff=100;
	}
}


void calc_diffs(map<uint64_t,StatIndex>& m_sresult
	,map<uint64_t,SndDistribute>& m_dis_result
	,map<uint64_t,StatIndexEx>& m_ex)
{
	map<uint64_t,StatIndex>::iterator lsat=m_sresult.begin();
	map<uint64_t,StatIndex>::iterator trav=m_sresult.begin();
	for(;;)
	{
		if(trav==m_sresult.end())
		{
			break;
		}
		/////////////////////
		StatIndexEx& sei=m_ex[trav->first];
		sei.send=trav->second.send;
		sei.recv=trav->second.recv;
		sei.drop=trav->second.drop;
		sei.resp=trav->second.resp;
		sei.error_code=trav->second.error_code;
		sei.resp_diff=0;
		sei.send_diff=0;
		sei.dist_diff=0;
		if(trav!=lsat)
		{
			if(trav->second.recv>3&&lsat->second.recv>3)
			{
				gl_calc_diffcore(trav->second.resp,lsat->second.resp,sei.resp_diff);
			}
			else
			{
				sei.resp_diff=0;
			}
			if((trav->second.send+lsat->second.send)>10)
			{
				gl_calc_diffcore((double)trav->second.send
					,(double)lsat->second.send,sei.send_diff);
			}
			else
			{
				sei.send_diff=0;
			}
			map<uint64_t,SndDistribute>::iterator ldi=m_dis_result.find(trav->first);
			map<uint64_t,SndDistribute>::iterator rdi=m_dis_result.find(lsat->first);
			if((trav->second.send+lsat->second.send)>10)
			{
				if(ldi!=m_dis_result.end() && rdi!=m_dis_result.end())
				{
					sei.dist_diff=SndDistribute::diff_rate(ldi->second,rdi->second);
				}
			}
			else
			{
				sei.dist_diff=0;
			}
		}
		sei.cal_trouble();
		/////////////////////
		lsat=trav;
		++trav;
	}
}

class LocalFileReader
{
	enum {BUFSZ=10*1024*1024};
	public:
		LocalFileReader()
		{
			_f=0;
			m_buf.resize(BUFSZ);
			buflim=0;
			bufcur=0;
		}
		~LocalFileReader()
		{
			delete _f;
		}
	public:
		bool open(const char* path)
		{
			delete _f;
			_f=new ifstream;
			_f->open(path,ios_base::binary);
			if((!(*_f))||_f->fail())
			{
				return false;
			}
			setbuf(0,0);
			//////////////////
			buflim=read(&m_buf[0],BUFSZ);
			bufcur=0;
			return true;
		}
		bool read_line(string& line)
		{
			line.clear();
			for(;;)
			{
			if(bufcur<buflim)
			{
				uint64_t tcr=bufcur;
				for(;;)
				{
					if(m_buf[tcr]=='\n')
					{
						char* frst=&m_buf[bufcur];
						char* edst=&m_buf[tcr];
						line.append(frst,edst);
						bufcur=tcr+1;
						return true;
					}
					++tcr;
					if(tcr==buflim)
					{
						char* frst=&m_buf[bufcur];
						char* edst=&m_buf[tcr];
						line.append(frst,edst);
						bufcur=tcr;
						break;
					}
				}
			}
			/////////load buflim
			if(buflim!=BUFSZ)
			{
				if(line.empty())
				{
					return false;
				}
				return true;
			}
			buflim=read(&m_buf[0],BUFSZ);
			bufcur=0;
			}
		}
	private:
		void seek(uint64_t pos)
		{
			_f->seekg(pos);
		}
		uint64_t length()
		{
			uint64_t org=_f->tellg();
			_f->seekg(0,ios_base::end);
			uint64_t len=_f->tellg();
			_f->seekg(org);
			return len;			
		}
		uint64_t read(char* buf,uint64_t len)
		{
			uint64_t ret=_f->tellg();
			_f->read(buf,len);
			if(_f->eof())
			{
				_f->clear();
			}
			ret=(uint64_t)_f->tellg()-ret;
			return ret;
		}
	public:
		void close()
		{
			if(_f!=0)
			{
				_f->close();
			}
		}
	private:
		void setbuf(char* buf,uint64_t len)
		{
			_f->rdbuf()->pubsetbuf(buf,len);
		}
	private:
		ifstream* _f;
		vector<char> m_buf;
		uint64_t buflim;
		uint64_t bufcur;
	private:
		LocalFileReader(const LocalFileReader& o);
		LocalFileReader& operator =(const LocalFileReader& o);
};


struct SortByTroubleIdx
{
	SortByTroubleIdx(std::vector<double>& u):v(u){}
	~SortByTroubleIdx(){}
	bool operator()(unsigned int a,unsigned int b)
	{
		return v[a]>v[b];
	}
	std::vector<double>& v;
};

class CMB_TrobuleIndexSort
{
public:
	///function
	////batch generate file
	///// generate sort result with par
	void init(map<string,string>& initmp,string& srt_file_path,uint64_t bigger_recent_time,uint64_t max_cnt
		,double trouble_level,Reader_fsm_parse& rfp)
	{
		m_initmp=initmp;
		m_srt_file_path=srt_file_path;
		m_bigger_recent_time=bigger_recent_time;
		m_max_cnt=max_cnt;
		m_trouble_level=trouble_level;
		//////
		map<string,string>::iterator imr=m_initmp.begin();
		for(;imr!=m_initmp.end();++imr)
		{
			SingleParse* psp=new SingleParse;
			psp->set_match(const_cast<string&>(imr->first));
			rfp.regist_call_back(psp);
			m_mapKey2Par[imr->first]=psp;
		}
	}
	void final_do()
	{
		vector<double> troubleix;
		vector<string> troublekey;
		vector<StatIndexEx> troubleStat;
		vector<uint64_t > troubleTime;
		vector<unsigned int> troubleidxsort;
		//////////////
		std::map<string, SingleParse* >::iterator irr=m_mapKey2Par.begin();
		for(;irr!=m_mapKey2Par.end();++irr)
		{
			map<uint64_t,StatIndexEx> get_xxx;
			irr->second->get_final_result_dif(get_xxx);

			ofstream out_linefile(m_initmp[irr->first].c_str());
			if(!out_linefile||out_linefile.fail())
			{
				cerr <<"can't create write file:" <<m_initmp[irr->first].c_str() <<endl;
				return;
			}
			gl_show_result_ex(get_xxx,out_linefile);
			out_linefile.close();
			///////////////////////
			map<uint64_t,StatIndexEx>::iterator ire=get_xxx.begin();
			for(;ire!=get_xxx.end();++ire)
			{
				map<uint64_t,StatIndexEx>::iterator irenx;
				irenx=ire;
				++irenx;
				if(irenx==get_xxx.end())
				{
					break;
				}
				if(ire->first>=m_bigger_recent_time)
				{
					if(ire->second.trouble_idx>=m_trouble_level)
					{
						troubleix.push_back(ire->second.trouble_idx);
						troublekey.push_back(irr->first);
						troubleStat.push_back(ire->second);
						troubleTime.push_back(ire->first);
						unsigned int nsz=troubleidxsort.size();
						troubleidxsort.push_back(nsz);
					}
				}
			}			
		}
		/////////////////////
		SortByTroubleIdx srter(troubleix);
		std::sort(troubleidxsort.begin(),troubleidxsort.end(),srter);
		size_t ndsz=troubleidxsort.size()<m_max_cnt?troubleidxsort.size():m_max_cnt;
		ofstream out_srt_fl(m_srt_file_path.c_str());
		if(!out_srt_fl||out_srt_fl.fail())
		{
			cerr <<"can't create file: " <<m_srt_file_path.c_str() <<endl;
			return;
		}
		for(size_t bi=0;bi<ndsz;++bi)
		{
			out_srt_fl <<"<match:" <<troublekey[troubleidxsort[bi] ] ;
			out_srt_fl <<" time:" <<troubleTime[troubleidxsort[bi] ];
			string read_tmm;
			time_util::transfer_time(troubleTime[troubleidxsort[bi] ],read_tmm);
			out_srt_fl <<" readable:" <<read_tmm;
			out_srt_fl <<"> ";
			troubleStat[troubleidxsort[bi] ].to_stream(out_srt_fl);
		}
		out_srt_fl.close();
	}
	std::map<string, string > m_initmp;
	std::map<string, SingleParse* > m_mapKey2Par;
	string m_srt_file_path;
	uint64_t m_bigger_recent_time;
	uint64_t m_max_cnt;
	double m_trouble_level;

};

bool match_key_extend_extrace(string& match_keyy,ExtendInfoStatParse& exti)
{
	map<string,string> match;
	parseString2MapSS(match_keyy,match);
	map<string,string>::iterator itr=match.find(string("extend"));
	if(itr==match.end())
	{
		return false;
	}
	exti.extend_key=itr->second;
	match.erase(itr);
	match_keyy=parseMapSS2String(match);
	return true;
}

int main(int argc,char* argv[])
{
	ofstream* p_ofsm=0;
	std::string saved_file;
	if(argc==8)
	{
		saved_file=argv[7];
		p_ofsm=new ofstream(saved_file.c_str());
		if(p_ofsm->fail())
		{
			cerr <<"can't open file:" <<saved_file <<endl;
			return -1;
		}
	}
	else if(argc!=7)
	{
		cerr <<"Usage: " <<argv[0] <<" <directory> <single|pair|multimatch|debug> <min_times> <max_times> <map>(multimatch: file<map> <file>) <global_conf> <filter_sav_file>" <<endl;
		cerr <<"<map> keys:" <<endl;
		cerr <<"[for single]" <<endl;
		cerr <<"srv_name" <<endl;
		cerr <<"srv_ipport" <<endl;
		cerr <<"srv_groupid" <<endl;
		cerr <<"fun_type" <<endl;
		cerr <<"[for pair]" <<endl;
		cerr <<"src_name" <<endl;
		cerr <<"src_ipport" <<endl;
		cerr <<"src_groupid" <<endl;
		cerr <<"des_name" <<endl;
		cerr <<"des_ipport" <<endl;
		cerr <<"des_groupid" <<endl;
		cerr <<"fromreq_type" <<endl;
		cerr <<"toreq_type" <<endl;
		return -1;
	}
	PxGlobalConf gcnf;
	string confd_path=argv[6];
	if(gcnf.init(confd_path)!=0)
	{
		cerr <<"error load conf: " <<argv[6] <<endl;
		return -1;
	}
	gl_global_conf_mp.gl_pglobal_conf=&gcnf;
	gl_global_conf_mp.init();

	string this_command_line;
	statDebugParse cbbb_f;
	Reader_fsm_parse rfp;

	
	UniqueParsefilter p_upf;
	SingleParse s_pp;
	s_pp.m_pofilterfile=p_ofsm;
	PairParse p_pp;
	p_pp.m_pofilterfile=p_ofsm;
	string sptype=argv[2];
	string match_key=argv[5];
	uint64_t mintimes=0;
	uint64_t maxtimes=0;
	JLexical::Cast(argv[3],mintimes);
	JLexical::Cast(argv[4],maxtimes);

	CMB_TrobuleIndexSort main_ctis;
	ExtendInfoStatParse gl_extend_info;
	if(match_key_extend_extrace(match_key,gl_extend_info))
	{
		s_pp.eif=&gl_extend_info;
		p_pp.eif=&gl_extend_info;
	}
	else
	{
		s_pp.eif=0;
		p_pp.eif=0;
	}

	////omit function init Top
	bool omit_headlines=false;
	uint64_t omit_timestamp=0;
	int omit_stat=-1;//-1: wait first hit 0:wait time 1:wait hit [] 2:normal pass 
	if(mintimes>3600)
	{
		omit_headlines=true;
		omit_timestamp=mintimes-3600;
	}
	////omit function init Bottom

	if(sptype==string("pair"))
	{
		p_upf.mintimes=mintimes;
		p_upf.maxtimes=maxtimes;
		rfp.regist_call_back(&p_upf);
		rfp.regist_call_back(&p_pp);
		parseString2MapSS(match_key,p_pp.match);
	}
	else if(sptype==string("debug"))
	{
		rfp.regist_call_back(&cbbb_f);
	}
	else if(sptype==string("multimatch"))
	{
		ifstream infsm(match_key.c_str());
		if(!infsm||infsm.fail())
		{
			cerr <<"can't open multimatch_file: " <<match_key.c_str() <<endl;
			return -1;
		}
		string cmdline;
		map<string,string> match_files;
		bool first_param_line=true;;
		string srt_file_path;
		uint64_t bigger_recent_time;
		uint64_t max_cnt;
		double trouble_level;
		for(;;)
		{
			if(!getline(infsm,cmdline))
			{
				break;
			}
			if(cmdline.empty())
			{
				break;
			}
			if(first_param_line)
			{
				map<string,string> paramss;
				parse_colon_string2map(cmdline,paramss);
				first_param_line=false;
				/////////
				map<string,string>::iterator itso=paramss.find(string("trouble_sort_out"));
				map<string,string>::iterator itdm=paramss.find(string("time_diff_max"));
				map<string,string>::iterator itl=paramss.find(string("trouble_level"));
				map<string,string>::iterator imr=paramss.find(string("max_row"));
				if(itso==paramss.end()
				||itdm==paramss.end()
				||itl==paramss.end()
				||imr==paramss.end())
				{
					cerr <<"multi_match_file format error "<<endl;
					return -1;
				}
				JLexical::Cast(itso->second,srt_file_path);
				JLexical::Cast(itdm->second,bigger_recent_time);
				JLexical::Cast(itl->second,trouble_level);
				JLexical::Cast(imr->second,max_cnt);
				continue;
			}
			vector<string> cmd_arg;
			gl_cutter_space(cmdline,cmd_arg);
			if(cmd_arg.size()!=2)
			{
				cerr <<"format error:size not 2" <<endl;
				return -1;
			}
			match_files[cmd_arg[0] ]=cmd_arg[1];
		}
		//////////
		if(match_files.empty())
		{
			cerr <<"format error:mmfile empty" <<endl;
			return -1;
		}
		if(first_param_line)
		{
			cerr <<"format error:no first param" <<endl;
			return -1;
		}
		if(maxtimes>bigger_recent_time)
		{
			bigger_recent_time=maxtimes-bigger_recent_time;
		}
		else
		{
			bigger_recent_time=mintimes;
		}
		/////////////filters
		p_upf.mintimes=mintimes;
		p_upf.maxtimes=maxtimes;
		rfp.regist_call_back(&p_upf);
		///////////
		main_ctis.init(match_files,srt_file_path,bigger_recent_time,max_cnt,trouble_level,rfp);
	}
	else
	{
		p_upf.mintimes=mintimes;
		p_upf.maxtimes=maxtimes;
		rfp.regist_call_back(&p_upf);
		rfp.regist_call_back(&s_pp);
		s_pp.set_match(match_key);
	}

	////////////////////////////////////////////////////////
	vector<string> file_lst;
	string dirfl=argv[1];
	if(dirfl.size()<1)
	{
		cerr <<"bad directory: "<<dirfl <<endl;
		return -1;
	}
	if(dirfl[dirfl.size()-1]!='/')
	{
		dirfl.push_back('/');
	}
	LinuxGetList(dirfl.c_str(),file_lst);
	if(file_lst.empty())
	{
		cerr <<"empty directory: "<<dirfl <<endl;
		return -1;
	}
	for(size_t yui=0;yui<file_lst.size();++yui)
	{
		if(omit_headlines)/////omit function
		{omit_stat=-1;}/////////omit function
		string thisfile_p=dirfl+file_lst[yui];
		//ifstream infl(thisfile_p.c_str());
		//if(!infl||infl.fail())
		LocalFileReader infl;
		if(!infl.open(thisfile_p.c_str()))
		{
			cerr <<"can't open file: " <<thisfile_p <<endl;
			return -1;
		}
		for(;;)
		{
			//if(!getline(infl,this_command_line))
			if(!infl.read_line(this_command_line))
			{
				break;
			}
			///////omit function Top
			if(omit_headlines)
			{
				if(omit_stat==-1)
				{
					an_bracket abb;
					string& line=this_command_line;
					if(abb.parse(line)==0)
					{
						Packey ky;
						if(ky.parse(abb.keystr,abb.valuestr)==0)
						{
							if(ky.times>=omit_timestamp)
							{
								omit_stat=2;
							}
							else
							{
								omit_stat=0;
								continue;
							}///////////////////////
						}
					}
				}
				else if(omit_stat==0)
				{
					an_bracket abb;
					string& line=this_command_line;
					if(abb.parse(line)!=0)
					{
						continue;
					}
					Packey ky;
					if(ky.parse(abb.keystr,abb.valuestr)!=0)
					{
						continue;
					}
					if(ky.times>=omit_timestamp)
					{
						omit_stat=1;
					}
					continue;
				}
				else if(omit_stat==1)
				{
					PrcNode ppnode;
					string& line=this_command_line;
					if(ppnode.parse(line)!=0)
					{
						continue;
					}
					omit_stat=2;
				}
			}
			///////omit function Bottom
			rfp.do_parse(this_command_line);
		}
	}
	/////////////////////////////////////////////////////
	if(p_ofsm!=0)
	{
		return 0;
	}
	if(sptype==string("pair"))
	{
		if(p_pp.eif==0)
		{
			gl_show_result(p_pp.m_result);
		}
		else
		{
			p_pp.eif->debug_to_stream(cout);
		}
	}
	else if(sptype==string("debug"))
	{
	}
	else if(sptype==string("multimatch"))
	{
		main_ctis.final_do();
	}
	else
	{
		if(s_pp.eif==0)
		{
			s_pp.endcal_andshow();
		}
		else
		{
			s_pp.eif->debug_to_stream(cout);
		}
	}

	return 0;
}


