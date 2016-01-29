#include "source_parser.h"
#include <vector>
#include <string>
#include <map>
#include <set>
#include <fstream>
#include <assert.h>
#include "source_util.h"
#include "util.h"
#include "server_common/seda/common/any_cast.h"

using namespace std;

int an_bracket::parse(std::string& line)
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

int PrcNode::parse(std::string& line)
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
	ip_port=line.substr(p,l-p);
	size_t cm=ip_port.find_first_of(':');
	if(cm==ip_port.npos)
	{return 4;}
	ip=ip_port.substr(0,cm);
	string postr=ip_port.substr(cm+1);
	port=0;
	JLexical::Cast(postr,port);
	return 0;
}

void PrcNode::to_stream(ostream& os)
{
	os<<"[" <<ip <<":" <<port <<"]" <<endl;
}


Packey::Packey()
{
}
Packey::~Packey()
{
}
Packey::Packey(const Packey &o)
{
	pack_id=o.pack_id;
	times=o.times;
}
Packey& Packey::operator = (const Packey &o)
{
	pack_id=o.pack_id;
	times=o.times;
	return *this;
}

bool Packey::operator < (const Packey &o) const
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

void Packey::clear()
{
	pack_id="";
	times=0;
}
int Packey::parse(string& fstr,string& lstr)
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

void Packey::to_stream(ostream& os)
{
	os <<"<start: " <<pack_id <<"> times:" <<times <<endl;
}

StatItem::StatItem()
{
}
StatItem::~StatItem()
{
}
StatItem::StatItem(const StatItem& o)
{
	key=o.key;
	send=o.send;
	recv=o.recv;
	drop=o.drop;
	resp=o.resp;
}
StatItem& StatItem::operator = (const StatItem& o)
{
	key=o.key;
	send=o.send;
	recv=o.recv;
	drop=o.drop;
	resp=o.resp;
	return *this;
}

void StatItem::to_stream(ostream& os)
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
int StatItem::parse(string& fstr,string& lstr,std::set<std::string>* dao_filter
	,bool needsingle,bool needpair)
{
	map<string,string> fm;
	parse_colon_string2map(fstr,fm);
	size_t heds=fm.size();
	if(!
		(
		(needsingle&&heds==1)
		||
		(needpair&&heds==2) 
		)
	)
	{
		return 1;
	}
	std::string dnn="dao";
	map<string,string>::iterator finddi
		=fm.find(dnn);
	if(finddi==fm.end())
	{
		return 1;
	}
	if(dao_filter!=0)
	{
		if(dao_filter->find(finddi->second)==dao_filter->end())
		{
			return 1;
		}
	}

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


void ErrorItem::clear()
{
	error_mp.clear();
}
void ErrorItem::to_stream(ostream& os)
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
int ErrorItem::parse(string& fstr,string& lstr)
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

void LogicStatBasicPack::clear_body()
{
	k.clear();
	stats.clear();
	error.clear();
}
void LogicStatBasicPack::to_stream(ostream& os)
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

Reader_fsm_parse::Reader_fsm_parse()
{
	n_parse_stat=0;
	//0:wait []node 1:wait key 2: wait err 3: wait #### 
	//4: omiting_time

	bippfilter=false;
	btimefilter=false;
	mint=0;
	maxt=2000000000;
	outofmaxt=false;

	bodyfilter=false;

	bneedsingle=true;;
	bneedpair=true;;
}
Reader_fsm_parse::~Reader_fsm_parse()
{
}
void Reader_fsm_parse::check_head(std::string& line)
{
	if(forming_pack.node.parse(line)==0)
	{
		forming_pack.clear_body();
		if(bippfilter)
		{
			if(ist_ippset.find(forming_pack.node.ip_port)
				!=ist_ippset.end())
			{
				n_parse_stat=1;
			}
			else
			{
				n_parse_stat=0;
			}
		}
		else
		{
			n_parse_stat=1;
		}
	}
}
int Reader_fsm_parse::do_parse(std::string& line)
{
	switch(n_parse_stat)
	{
		case 0:
		{
			//wait []
			check_head(line);
		}
		break;
		case 1:
		{
			//wait key

			//maybe encounter []
			check_head(line);
			an_bracket abb;
			if(abb.parse(line)==0)
			{
				if(forming_pack.k.parse(abb.keystr,abb.valuestr)==0)
				{
					if(btimefilter&&(forming_pack.k.times<mint
							||forming_pack.k.times>maxt))
					{
						///go to time wait
						n_parse_stat=4;
					}
					else
					{
						//ok
						n_parse_stat=2;
					}
				}
			}
		}
		break;
		case 2:
		{
			//wait err
			//maybe encounter []
			check_head(line);
			an_bracket abb;
			if(abb.parse(line)==0)
			{
				StatItem si;
				if(forming_pack.error.parse(abb.keystr,abb.valuestr)==0)
				{
					//ok next
					n_parse_stat=3;
				}
				else 
				{
					if(bodyfilter)
					{
						if(si.parse(abb.keystr,abb.valuestr,&ist_daoset
							,bneedsingle,bneedpair)==0)
						{
							forming_pack.stats.push_back(si);
						}
					}
					else
					{
						if(si.parse(abb.keystr,abb.valuestr,0
							,bneedsingle,bneedpair)==0)
						{
							forming_pack.stats.push_back(si);
						}
					}
				}
			}
		}
		break;
		case 3:
		{
			//wait #### 
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
				n_parse_stat=1;
			}
			else
			{
				check_head(line);
			}
		}
		break;
		case 4:
		{
			//omiting_time wait imbound time
			an_bracket abb;
			if(abb.parse(line)==0)
			{
				Packey ky;
				if(ky.parse(abb.keystr,abb.valuestr)==0)
				{
					if(ky.times>=mint&&ky.times<=maxt)
					{
						n_parse_stat=0;
					}
					else if(ky.times>maxt)
					{
						outofmaxt=true;
					}
				}
			}
		}
		break;
		default:
			assert(false);
	}
	return 0;

}


void Reader_fsm_parse::regist_call_back(IParse_Callback* cb)
{
	m_cbs.push_back(cb);
}

