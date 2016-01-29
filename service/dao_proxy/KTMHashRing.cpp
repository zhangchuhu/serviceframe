#include "KTMHashRing.h"
#include <sstream>
#include "server_common/seda/encryption/md5.hpp"


using namespace std;
using namespace server::dao_proxy;
using namespace encryption;


KTMHashRing::KTMHashRing()
{
}
KTMHashRing::~KTMHashRing()
{
}

int KTMHashRing::init(std::map<int,int>& groupid_weight)
{
	if(groupid_weight.empty())
	{
		return -1;
	}
	stringstream ss;
	std::map<uint64_t ,int>().swap( hash_groupid );
	std::set<int>().swap( groupids );
	for(std::map<int,int>::iterator v=groupid_weight.begin();v!=groupid_weight.end();++v)
	{
		int wei=v->second;
		if(wei<=0)
		{
			wei=1;
		}
		else if(wei>1000)
		{
			wei=1000;
		}
		groupids.insert(v->first);
		wei*=10;
		for(int i=0;i<wei;++i)
		{
			ss.clear();
			ss.str("");
			ss <<"G" <<hex <<v->first <<"_" <<hex <<i;
			std::string str=ss.str();
			md5 mf;
			uint64_t hash_value=mf.hash64(str.c_str(),str.size());
			hash_groupid[hash_value]=v->first;
		}
	}
	uint64_t last_hash=(uint64_t)-1;
	if(hash_groupid.find(last_hash)==hash_groupid.end())
	{
		std::map<uint64_t ,int>::iterator bp=hash_groupid.begin();
		int grp=bp->second;
		hash_groupid[last_hash]=grp;
	}
	////////////////////////////
	///optimize speed
	std::vector<uint64_t> can_erase_hash;
	for(std::map<uint64_t ,int>::iterator p=hash_groupid.begin();p!=hash_groupid.end();++p)
	{
		std::map<uint64_t ,int>::iterator np=p;
		np++;
		if(np!=hash_groupid.end())
		{
			if(p->second==np->second)
			{
				can_erase_hash.push_back(p->first);
			}
		}
	}
	for(size_t b=0;b<can_erase_hash.size();++b)
	{
		hash_groupid.erase(can_erase_hash[b]);
	}
	return 0;
}
int KTMHashRing::get_groupid(const std::string& hash_str,std::set<int>* p_groupid_mask,int& out_groupid)
{
	md5 mf;
	uint64_t hash_value=mf.hash64(hash_str.c_str(),hash_str.size());
	return get_groupid(hash_value,p_groupid_mask,out_groupid);
}
//return 0:ok 1:can't find -1:hashring error
int KTMHashRing::get_groupid(uint64_t hash,std::set<int>* p_groupid_mask,int& out_groupid)
{
	std::map<uint64_t ,int>::iterator i=hash_groupid.lower_bound(hash);
	if(i==hash_groupid.end())
	{
		return -1;
	}
	if(p_groupid_mask!=0)
	{
		std::set<int>::iterator vmp=p_groupid_mask->begin();
		for(;vmp!=p_groupid_mask->end();)
		{
			if(groupids.find(*vmp)==groupids.end())
			{
				std::set<int>::iterator old=vmp;
				++vmp;
				p_groupid_mask->erase(old);
			}
			else
			{
				++vmp;
			}
		}
		if(p_groupid_mask->empty())
		{
			return 1;
		}
		std::map<uint64_t ,int>::iterator nn=i;
		for(;;)
		{
			if(p_groupid_mask->find(nn->second)!=p_groupid_mask->end())
			{
				out_groupid=nn->second;
				return 0;
			}
			nn++;
			if(nn==hash_groupid.end())
			{
				nn=hash_groupid.begin();
			}
			if(nn==i)
			{
				return 1;
			}
		}
	}
	else
	{
		out_groupid=i->second;
	}
	return 0;
}



