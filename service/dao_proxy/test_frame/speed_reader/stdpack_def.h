#ifndef _STANDARD_PACK_DEFINE_H_20120530_
#define _STANDARD_PACK_DEFINE_H_20120530_
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <stdint.h>
#include "daemon_parse.h"
#include "source_parser.h"

struct StatIndex
{
	StatIndex();
	~StatIndex();
	uint32_t send;
	uint32_t recv;
	uint32_t drop;
	double resp;
	map<int,int> error_code;
	void to_stream(ostream& os);
	StatIndex(const StatIndex& o);
	StatIndex& operator = (const StatIndex& o);
	StatIndex& operator += (const StatIndex& o);
};

struct SingleStatPack
{
public:
	SingleStatPack();
	~SingleStatPack();
	SingleStatPack(const SingleStatPack& o);
	SingleStatPack& operator = (const SingleStatPack& o);
	uint64_t timestamp;
	string srv_name;
	string srv_ipport;
	int srv_groupid;
	string fun_type;
	StatIndex si;
	string get_by_string(string& kys);
	bool match(std::map<string,string>& mk);
};


struct PairStatPack
{
public:
	PairStatPack();
	~PairStatPack();
	PairStatPack(const PairStatPack& o);
	PairStatPack& operator =(const PairStatPack& o);
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
	string get_by_string(string& kys);
	bool match(std::map<string,string>& mk);
};

class IParseSingle_Callback
{
public:
	virtual ~IParseSingle_Callback(){};
	////return 0:ok  1:end next callback
	virtual int on_parse(const SingleStatPack& pack)=0;
};
class IParsePair_Callback
{
public:
	virtual ~IParsePair_Callback(){};
	////return 0:ok  1:end next callback
	virtual int on_parse(const PairStatPack& pack)=0;
};

struct StatIndexEx
{
	StatIndexEx();
	~StatIndexEx();
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
	void cal_trouble();

	void to_stream(ostream& os);
	StatIndexEx(const StatIndexEx& o);
	StatIndexEx& operator = (const StatIndexEx& o);
};

struct SndDistribute
{
	SndDistribute();
	~SndDistribute();
	SndDistribute(const SndDistribute& o);
	SndDistribute& operator = (const SndDistribute& o);
	map<string,uint64_t > distri;
	void clear();
	void add(string& ip,uint64_t cnt);
	//return [0--100] score
	static int diff_rate(SndDistribute& lhs,SndDistribute& rhs);
};


#endif	// _STANDARD_PACK_DEFINE_H_20120530_

