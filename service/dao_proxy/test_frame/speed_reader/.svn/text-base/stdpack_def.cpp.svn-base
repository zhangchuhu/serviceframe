#include "stdpack_def.h"
#include <vector>
#include <string>
#include <map>
#include <set>
#include <fstream>
#include "config_basic_util.h"
#include "server_common/seda/common/any_cast.h"

using namespace std;


StatIndex::StatIndex()
{
	send=0;
	recv=0;
	drop=0;
	resp=0.0;
}
StatIndex::~StatIndex()
{
}

void StatIndex::to_stream(ostream& os)
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
StatIndex::StatIndex(const StatIndex& o)
{
	send=o.send;
	recv=o.recv;
	drop=o.drop;
	resp=o.resp;
	error_code=o.error_code;
}
StatIndex& StatIndex::operator = (const StatIndex& o)
{
	send=o.send;
	recv=o.recv;
	drop=o.drop;
	resp=o.resp;
	error_code=o.error_code;
	return *this;
}
StatIndex& StatIndex::operator += (const StatIndex& o)
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


SingleStatPack::SingleStatPack()
{
}
SingleStatPack::~SingleStatPack()
{
}
SingleStatPack::SingleStatPack(const SingleStatPack& o)
{
	timestamp=o.timestamp;
	srv_name=o.srv_name;
	srv_ipport=o.srv_ipport;
	srv_groupid=o.srv_groupid;
	fun_type=o.fun_type;
	si=o.si;
}
SingleStatPack& SingleStatPack::operator = (const SingleStatPack& o)
{
	timestamp=o.timestamp;
	srv_name=o.srv_name;
	srv_ipport=o.srv_ipport;
	srv_groupid=o.srv_groupid;
	fun_type=o.fun_type;
	si=o.si;
	return *this;
}

string SingleStatPack::get_by_string(string& kys)
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

bool SingleStatPack::match(std::map<string,string>& mk)
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

PairStatPack::PairStatPack()
{
}
PairStatPack::~PairStatPack()
{
}
PairStatPack::PairStatPack(const PairStatPack& o)
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
PairStatPack& PairStatPack::operator =(const PairStatPack& o)
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

string PairStatPack::get_by_string(string& kys)
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
bool PairStatPack::match(std::map<string,string>& mk)
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




///////////////////
StatIndexEx::StatIndexEx()
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
StatIndexEx::~StatIndexEx()
{
}
void StatIndexEx::cal_trouble()
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

void StatIndexEx::to_stream(ostream& os)
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

StatIndexEx::StatIndexEx(const StatIndexEx& o)
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

StatIndexEx& StatIndexEx::operator = (const StatIndexEx& o)
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


SndDistribute::SndDistribute()
{
}
SndDistribute::~SndDistribute()
{
}
SndDistribute::SndDistribute(const SndDistribute& o)
{
	distri=o.distri;
}
SndDistribute& SndDistribute::operator = (const SndDistribute& o)
{
	distri=o.distri;
	return *this;
}

void SndDistribute::clear()
{
	distri.clear();
}
void SndDistribute::add(string& ip,uint64_t cnt)
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
int SndDistribute::diff_rate(SndDistribute& lhs,SndDistribute& rhs)
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

