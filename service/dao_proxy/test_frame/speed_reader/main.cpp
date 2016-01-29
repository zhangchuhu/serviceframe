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

#include "server_common/protocol/pdbnode.h"
#include "common/core/sender.h"
#include "common/core/request.h"
#include "util.h"
#include "source_util.h"
#include "source_parser.h"
#include "stdpack_cnv.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <cstring>
#include "calc_class.h"


#include "config_basic_util.h"
using namespace std;
using namespace seda;
using namespace server::dao_proxy;
using namespace server::db_node;

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

bool match_key_extend_extrace(string& match_keyy,ExtendInfoStatParse& exti)
{
	map<string,string> match;
	parseString2MapSS(match_keyy,match);
	////////////////normal sort by name
	bool findded=false;
	map<string,string>::iterator itr=match.find(string("extend"));
	if(itr!=match.end())
	{
		exti.extend_key=itr->second;
		exti.orderby=0;
		match.erase(itr);
		findded=true;
	}
	/////////////////sort by send
	itr=match.find(string("extends"));
	if(itr!=match.end())
	{
		exti.extend_key=itr->second;
		exti.orderby=1;
		match.erase(itr);
		findded=true;
	}
	/////////////////sort by resp
	itr=match.find(string("extendr"));
	if(itr!=match.end())
	{
		exti.extend_key=itr->second;
		exti.orderby=2;
		match.erase(itr);
		findded=true;
	}
	/////////////////sort by drop
	itr=match.find(string("extendd"));
	if(itr!=match.end())
	{
		exti.extend_key=itr->second;
		exti.orderby=3;
		match.erase(itr);
		findded=true;
	}

	if(!findded)
	{
		return false;
	}
	/////////////////
	match_keyy=parseMapSS2String(match);
	return true;
}

void single_filter_setting(Reader_fsm_parse& rfp,string match_key)
{
	std::map<std::string,std::string> mp_kv;
	parseString2MapSS(match_key,mp_kv);
	////////////
	if(mp_kv.find(std::string("srv_ipport"))!=mp_kv.end())
	{
		if(mp_kv.find(std::string("srv_name"))!=mp_kv.end())
		{
			if(mp_kv[std::string("srv_name")] == gl_dao_proxy_name)
			{
				rfp.bippfilter=true;
				rfp.ist_ippset.insert(mp_kv[std::string("srv_ipport")]);
			}
		}
	}
	else if(mp_kv.find(std::string("srv_groupid"))!=mp_kv.end())
	{
		if(mp_kv.find(std::string("srv_name"))!=mp_kv.end())
		{
			if(mp_kv[std::string("srv_name")] == gl_dao_proxy_name)
			{
				rfp.bippfilter=true;
				int grpp=0;
				JLexical::Cast(mp_kv[std::string("srv_groupid")],grpp);
				std::vector<std::string> rsss;
				if(gl_global_conf_mp.get_proxyips_by_group(grpp,rsss))
				{
					for(size_t bn=0;bn<rsss.size();++bn)
					{
						rfp.ist_ippset.insert(rsss[bn]);
					}
				}
			}
		}
	}

	/////////////////////
	if(mp_kv.find(std::string("fun_type"))!=mp_kv.end())
	{
		rfp.bodyfilter=true;
		rfp.ist_daoset.insert(mp_kv[std::string("fun_type")]);
	}
}

void pair_filter_setting(Reader_fsm_parse& rfp,string match_key)
{
	std::map<std::string,std::string> mp_kv;
	parseString2MapSS(match_key,mp_kv);
	////////////
	if(mp_kv.find(std::string("src_ipport"))!=mp_kv.end())
	{
		if(mp_kv.find(std::string("src_name"))!=mp_kv.end())
		{
			if(mp_kv[std::string("src_name")] == gl_dao_proxy_name)
			{
				rfp.bippfilter=true;
				rfp.ist_ippset.insert(mp_kv[std::string("src_ipport")]);
			}
		}
	}
	else if(mp_kv.find(std::string("src_groupid"))!=mp_kv.end())
	{
		if(mp_kv.find(std::string("src_name"))!=mp_kv.end())
		{
			if(mp_kv[std::string("src_name")] == gl_dao_proxy_name)
			{
				rfp.bippfilter=true;
				int grpp=0;
				JLexical::Cast(mp_kv[std::string("src_groupid")],grpp);
				std::vector<std::string> rsss;
				if(gl_global_conf_mp.get_proxyips_by_group(grpp,rsss))
				{
					for(size_t bn=0;bn<rsss.size();++bn)
					{
						rfp.ist_ippset.insert(rsss[bn]);
					}
				}
			}
		}
	}

	/////////////////////
	if(mp_kv.find(std::string("fromreq_type"))!=mp_kv.end())
	{
		rfp.bodyfilter=true;
		rfp.ist_daoset.insert(mp_kv[std::string("fromreq_type")]);
	}
	///////////////////////////////////////////
	if(mp_kv.find(std::string("des_ipport"))!=mp_kv.end())
	{
		if(mp_kv.find(std::string("des_name"))!=mp_kv.end())
		{
			if(mp_kv[std::string("des_name")] == gl_dao_proxy_name)
			{
				rfp.bippfilter=true;
				rfp.ist_ippset.insert(mp_kv[std::string("des_ipport")]);
			}
		}
	}
	else if(mp_kv.find(std::string("des_groupid"))!=mp_kv.end())
	{
		if(mp_kv.find(std::string("des_name"))!=mp_kv.end())
		{
			if(mp_kv[std::string("des_name")] == gl_dao_proxy_name)
			{
				rfp.bippfilter=true;
				int grpp=0;
				JLexical::Cast(mp_kv[std::string("des_groupid")],grpp);
				std::vector<std::string> rsss;
				if(gl_global_conf_mp.get_proxyips_by_group(grpp,rsss))
				{
					for(size_t bn=0;bn<rsss.size();++bn)
					{
						rfp.ist_ippset.insert(rsss[bn]);
					}
				}
			}
		}
	}

	/////////////////////
	if(mp_kv.find(std::string("toreq_type"))!=mp_kv.end())
	{
		rfp.bodyfilter=true;
		rfp.ist_daoset.insert(mp_kv[std::string("toreq_type")]);
	}

}


int main(int argc,char* argv[])
{
	DaemonInfoParse daemon_info;
	gl_pdaemon_infoparse=&daemon_info;
	daemon_info.init();
	if(!daemon_info.parse("daemon_history_info"))
	{
		cerr <<"not found file: daemon_history_info" <<endl;
		return -1;
	}
	ofstream* p_ofsm=0;
	std::string saved_file;
	gl_dao_proxy_name="dao_proxy";
	gl_dao_server_name="dao_server";
	if(argc==9)
	{
		gl_dao_proxy_name=argv[7];
		gl_dao_server_name=argv[8];
	}
	else if(argc==8)
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
		daemon_info.save_file("daemon_history_info");
		cerr <<"Usage: " <<argv[0] <<" <directory> <single|pair|multimatch|debug> <min_times> <max_times> <map>(multimatch: file<map> <file>) <global_conf> <filter_sav_file>" <<endl;
		cerr <<"another need file: dbd_proxy_conf.xml daemon_history_info" <<endl;
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
	string checktype=argv[2];
	if(   checktype!=std::string("single")
		&&checktype!=std::string("pair")
		&&checktype!=std::string("multimatch")
		&&checktype!=std::string("debug")
		)
	{
		cerr <<"argument 2 format error" <<endl;
		return -1;
	}
	string check_confname=argv[6];
	uint32_t conf_size=(uint32_t)check_confname.size();
	if(conf_size<4)
	{
		cerr <<"config argument too short" <<endl;
		return -1;
	}
	if(check_confname.substr(conf_size-4)!=std::string(".xml"))
	{
		cerr <<"config argument postfix is not .xml" <<endl;
		return -1;
	}
	///////////////////////

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
	SPParse singlepair_parse;
	singlepair_parse.m_pofilterfile=p_ofsm;
	
	UniqueParsefilter p_upf;
	SingleQuery s_pp;
	PairQuery p_pp;
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
	if(mintimes>600)
	{
		rfp.btimefilter=true;
		rfp.mint=mintimes-600;
		rfp.maxt=maxtimes+600;
	}
	////omit function init Bottom

	if(sptype==string("pair"))
	{
		p_upf.mintimes=mintimes;
		p_upf.maxtimes=maxtimes;
		rfp.regist_call_back(&p_upf);
		rfp.regist_call_back(&singlepair_parse);

		parseString2MapSS(match_key,p_pp.match);
		singlepair_parse.pairlearn.add_pair(p_pp.match,&p_pp);
		pair_filter_setting(rfp,match_key);
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
		rfp.regist_call_back(&singlepair_parse);
		///////////
		main_ctis.init(match_files,srt_file_path,bigger_recent_time,max_cnt,trouble_level
			,singlepair_parse);
	}
	else
	{
		p_upf.mintimes=mintimes;
		p_upf.maxtimes=maxtimes;
		rfp.regist_call_back(&p_upf);
		rfp.regist_call_back(&singlepair_parse);
		
		s_pp.set_match(match_key);
		////зЂВс
		singlepair_parse.singlelearn.add_single(s_pp.match,&s_pp);
		singlepair_parse.pairlearn.add_pair(s_pp.s_ppd.match,&s_pp.s_ppd);

		single_filter_setting(rfp,match_key);
	}

	////prepare start load file
	if(singlepair_parse.singlelearn.reg_size()==0)
	{
		rfp.bneedsingle=false;
	}
	if(singlepair_parse.pairlearn.reg_size()==0)
	{
		rfp.bneedpair=false;
	}
	singlepair_parse.prepare();
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
		///reset the parser state IMPORTANT! Top
		rfp.n_parse_stat=0;
		///reset the parser state IMPORTANT! Bottom

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
			rfp.do_parse(this_command_line);

			if(rfp.outofmaxt)
			{
				///o mit whole file
				break;
			}
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


