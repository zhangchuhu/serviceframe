#ifndef _STANDARD_PACK_CONVERT_H_20120530_
#define _STANDARD_PACK_CONVERT_H_20120530_
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <stdint.h>
#include "daemon_parse.h"
#include "source_parser.h"
#include "calc_class.h"
#include "stdpack_def.h"

extern std::string gl_dao_proxy_name;
extern std::string gl_dao_server_name;
extern DaemonInfoParse* gl_pdaemon_infoparse;


struct APPkuohaoIp
{
	int parse(std::string& s);
	std::string app;
	std::string mip;
};



struct ExtendInfoStatParse
{
	ExtendInfoStatParse();
	~ExtendInfoStatParse();
	void sum_single_p(SingleStatPack& ssp);
	void sum_pair_p(PairStatPack& ssp);
	string extend_key;
	map<uint64_t,map<string,StatIndex> > m_result;
	int orderby;//0:default 1:send 2:resp 3:drop
	void debug_to_stream(ostream& out_srm);
};
class DistributQuery : public IParsePair_Callback
{
public:
	virtual int on_parse(const PairStatPack& pack);
	map<string,string> match;
	map<uint64_t,SndDistribute> m_dis_result;
	void cnv_match();
};

class SingleQuery : public IParseSingle_Callback
{
public:
	SingleQuery();
	~SingleQuery();
public:
	virtual int on_parse(const SingleStatPack& pack);

	map<string,string> match;
	////////////
	map<uint64_t,StatIndex> m_result;
	void set_match(string& match_key);

	void endcal_andshow();
	void get_final_result_dif(std::map<uint64_t,StatIndexEx>& m_resultex);
	DistributQuery s_ppd;
	ExtendInfoStatParse* eif;
};

class PairQuery : public IParsePair_Callback
{
public:
	PairQuery();
	~PairQuery();
public:
	virtual int on_parse(const PairStatPack& pack);

	std::map<string,string> match;
	///////////
	std::map<uint64_t,StatIndex> m_result;
	ExtendInfoStatParse* eif;
};


class GMatcherLearn
{
	struct TMatche
	{
		std::map<std::string,std::string> matches;
		int type;
		void* cb;
		//type==0:class IParseSingle_Callback
		//type==1;class IParsePair_Callback
	};
	struct divnode
	{
		divnode()
		{
			type=0;
			other=0;
		}
		~divnode(){}
		int type;//0:leaf 1:branch
		/////branch node
		string matchkey;
		std::map<std::string,divnode*> nextb;
		divnode* other;
		/////leaf node
		std::vector<TMatche* > nodes;
		std::set<std::string> key_list;
	};
	divnode dn;

public:
	GMatcherLearn();
	~GMatcherLearn();
	void set_keyllist(std::set<std::string>& kl);
	void add_single(std::map<std::string,std::string>& mth ,IParseSingle_Callback* cb);
	void add_pair(std::map<std::string,std::string>& mth ,IParsePair_Callback* cb);
	void final_fix();
	int single_hit(SingleStatPack& ssp);
	int pair_hit(PairStatPack& psp);

	uint32_t reg_size();
private:
	void learn(divnode& no);
	void filter_key(divnode& no);
};

class SPParse : public IParse_Callback
{
public:
	SPParse();
	~SPParse();
public:
	//first regist call_backs
	GMatcherLearn singlelearn;
	GMatcherLearn pairlearn;
	
	ofstream* m_pofilterfile;

	//dobefore parse
	void prepare();
	virtual int on_parse(const LogicStatBasicPack& pack);
};

class CMB_TrobuleIndexSort
{
public:
	///function
	////batch generate file
	///// generate sort result with par
	void init(map<string,string>& initmp,string& srt_file_path,uint64_t bigger_recent_time,uint64_t max_cnt
		,double trouble_level,SPParse& spp);
	void final_do();
	std::map<string, string > m_initmp;
	std::map<string, SingleQuery* > m_mapKey2Par;
	string m_srt_file_path;
	uint64_t m_bigger_recent_time;
	uint64_t m_max_cnt;
	double m_trouble_level;
};


#endif	// _STANDARD_PACK_CONVERT_H_20120530_

