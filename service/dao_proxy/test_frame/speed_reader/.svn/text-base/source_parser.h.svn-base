#ifndef _PROXYSRC_PARSE_H_20120530_
#define _PROXYSRC_PARSE_H_20120530_
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <stdint.h>

using namespace std;

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

struct an_bracket
{
	string keystr;
	string valuestr;
	int parse(std::string& line);
};

struct PrcNode
{
	string ip;
	int port;
	////////
	string ip_port;
	//return 0 if success
	int parse(std::string& line);
	void to_stream(ostream& os);
};

struct Packey
{
	Packey();
	~Packey();
	Packey(const Packey &o);
	Packey& operator = (const Packey &o);
	bool operator < (const Packey &o) const;

	void clear();
	int parse(string& fstr,string& lstr);
	void to_stream(ostream& os);

	string pack_id;
	uint64_t times;
};
struct StatItem
{
	StatItem();
	~StatItem();
	StatItem(const StatItem& o);
	StatItem& operator = (const StatItem& o);
	map<string,string> key;
	///
	uint64_t send;
	uint64_t recv;
	uint64_t drop;
	double resp;
	void to_stream(ostream& os);
	int parse(string& fstr,string& lstr,std::set<std::string>* dao_filter
		,bool needsingle,bool needpair);
};

struct ErrorItem
{
	map<int,int> error_mp;
	void clear();
	void to_stream(ostream& os);
	int parse(string& fstr,string& lstr);
};

/////////
struct LogicStatBasicPack
{
	Packey k;
	PrcNode node;
	vector<StatItem> stats;
	ErrorItem error;
	void clear_body();
	void to_stream(ostream& os);
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
	int n_parse_stat;
	//0:wait []node 1:wait key 2: wait err 3: wait #### 
	//4: omiting_time

	bool bippfilter;
	std::set<std::string> ist_ippset;
	bool btimefilter;
	uint64_t mint;
	uint64_t maxt;
	bool outofmaxt;

	bool bodyfilter;
	std::set<std::string> ist_daoset;

	bool bneedsingle;
	bool bneedpair;
public:
	Reader_fsm_parse();
	~Reader_fsm_parse();
public:
	int do_parse(std::string& line);
	void regist_call_back(IParse_Callback* cb);
private:
	LogicStatBasicPack forming_pack;
	vector<IParse_Callback* > m_cbs;

	void check_head(std::string& line);

};



#endif	// _PROXYSRC_PARSE_H_20120530_

