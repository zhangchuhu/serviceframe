#include "stdpack_cnv.h"
#include <vector>
#include <string>
#include <map>
#include <set>
#include <fstream>
#include "config_basic_util.h"
#include "util.h"
#include "server_common/seda/common/any_cast.h"
#include "time_util.h"

using namespace std;

std::string gl_dao_proxy_name;
std::string gl_dao_server_name;
DaemonInfoParse* gl_pdaemon_infoparse=NULL;


int APPkuohaoIp::parse(std::string& s)
{
	size_t pos=s.find_first_of('(');
	if(pos==s.npos)
	{
		return -1;
	}
	size_t posee=s.find_first_of(')');
	if(posee==s.npos)
	{
		return -1;
	}
	if(posee<=pos)
	{
		return -1;
	}
	//////////////
	std::string subff;
	app=s.substr(0,pos);
	std::string subsee;
	mip=s.substr(pos+1,posee-(pos+1));
	mip+=":0";
	return 0;
}



struct ExtendComparor
{
	bool operator() (const std::string& lhs,const std::string& rhs) const
	{
		if(cmptype==0)
		{
			return lhs<rhs;
		}
		map<string,StatIndex>::iterator lf=
			mpp->find(lhs);
		map<string,StatIndex>::iterator rf=
			mpp->find(rhs);
		///////////
		if(cmptype==1)
		{
			return lf->second.send>rf->second.send;
		}
		else if(cmptype==2)
		{
			return lf->second.resp>rf->second.resp;
		}
		else if(cmptype==3)
		{
			return lf->second.drop>rf->second.drop;
		}
		/////////
		return lhs<rhs;
	}
	map<string,StatIndex>* mpp;
	int cmptype;//0:default 1:send 2:resp 3:drop
};

ExtendInfoStatParse::ExtendInfoStatParse()
{
	orderby=0;
}
ExtendInfoStatParse::~ExtendInfoStatParse()
{
}
void ExtendInfoStatParse::sum_single_p(SingleStatPack& ssp)
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
void ExtendInfoStatParse::sum_pair_p(PairStatPack& ssp)
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

void ExtendInfoStatParse::debug_to_stream(ostream& out_srm)
{
	map<uint64_t,map<string,StatIndex> >::iterator ig
		=m_result.begin();
	for(;ig!=m_result.end();++ig)
	{
		out_srm <<"<times:" <<ig->first <<"> " ;
		ig->second[string("")].to_stream(out_srm);
		if(orderby==0)
		{
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
		}
		else
		{
			std::vector<std::string> ordnames;
			map<string,StatIndex>::iterator mpkv
				=ig->second.begin();
			for(;mpkv!=ig->second.end();++mpkv)
			{
				if(mpkv->first!=string(""))
				{
					ordnames.push_back(mpkv->first);
				}
			}
			ExtendComparor ecmp;
			ecmp.cmptype=orderby;
			ecmp.mpp=&ig->second;
			std::sort(ordnames.begin(),ordnames.end(),ecmp);
			for(size_t g=0;g<ordnames.size();++g)
			{
				out_srm <<"<" <<extend_key <<":" <<ordnames[g] <<"> ";
				ig->second[ordnames[g] ].to_stream(out_srm);
			}
		}
		out_srm <<"####" <<endl;
	}
}

int DistributQuery::on_parse(const PairStatPack& pack)
{
	PairStatPack& psp=const_cast<PairStatPack&>(pack);
	//join result
	map<uint64_t,SndDistribute>::iterator irr=m_dis_result.find(psp.timestamp);
	if(irr==m_dis_result.end())
	{
		SndDistribute inis;
		inis.add(psp.des_ipport,psp.si.send);
		m_dis_result[psp.timestamp]=inis;
		return 0;
	}
	else
	{
		irr->second.add(psp.des_ipport,psp.si.send);
	}
	return 0;
}

void DistributQuery::cnv_match()
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


SingleQuery::SingleQuery()
{
	eif=0;
}
SingleQuery::~SingleQuery()
{
}
int SingleQuery::on_parse(const SingleStatPack& ssp)
{
	SingleStatPack& nssp=const_cast<SingleStatPack&>(ssp);
	if(eif!=0)
	{
		eif->sum_single_p(nssp);
	}
	else
	{
		////join result
		gl_sum_to_it(m_result,ssp.timestamp,nssp.si);
	}
	return 0;
}

void SingleQuery::set_match(string& match_key)
{
	parseString2MapSS(match_key,match);
	s_ppd.match=match;
	s_ppd.cnv_match();
}
void SingleQuery::endcal_andshow()
{
	map<uint64_t,StatIndexEx> m_ex;
	calc_diffs(m_result,s_ppd.m_dis_result,m_ex);
	gl_show_result_ex(m_ex,cout);
}
void SingleQuery::get_final_result_dif(std::map<uint64_t,StatIndexEx>& m_resultex)
{
	m_resultex.clear();
	calc_diffs(m_result,s_ppd.m_dis_result,m_resultex);
}

PairQuery::PairQuery()
{
	eif=0;
}
PairQuery::~PairQuery()
{
}
int PairQuery::on_parse(const PairStatPack& psp)
{
	PairStatPack& npsp=const_cast<PairStatPack&>(psp);
	if(eif!=0)
	{
		eif->sum_pair_p(npsp);
	}
	else
	{
		////join result
		gl_sum_to_it(m_result,psp.timestamp,npsp.si);
	}
	return 0;
}

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


///function
////batch generate file
///// generate sort result with par
void CMB_TrobuleIndexSort::init(map<string,string>& initmp,string& srt_file_path,uint64_t bigger_recent_time,uint64_t max_cnt
	,double trouble_level,SPParse& spp)
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
		SingleQuery* psp=new SingleQuery;
		psp->set_match(const_cast<string&>(imr->first));
		spp.singlelearn.add_single(psp->match,psp);
		spp.pairlearn.add_pair(psp->s_ppd.match,&psp->s_ppd);
		m_mapKey2Par[imr->first]=psp;
	}
}

void CMB_TrobuleIndexSort::final_do()
{
	vector<double> troubleix;
	vector<string> troublekey;
	vector<StatIndexEx> troubleStat;
	vector<uint64_t > troubleTime;
	vector<unsigned int> troubleidxsort;
	//////////////
	std::map<string, SingleQuery* >::iterator irr=m_mapKey2Par.begin();
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

GMatcherLearn::GMatcherLearn()
{
	dn.type=0;//leaf
	dn.other=0;
}
GMatcherLearn::~GMatcherLearn()
{
}
void GMatcherLearn::set_keyllist(std::set<std::string>& kl)
{
	dn.key_list=kl;
}
void GMatcherLearn::add_single(std::map<std::string,std::string>& mth ,IParseSingle_Callback* cb)
{
	TMatche* tm=new TMatche;
	tm->type=0;
	tm->cb=static_cast<void*>(cb);
	tm->matches=mth;
	dn.nodes.push_back(tm);
}
void GMatcherLearn::add_pair(std::map<std::string,std::string>& mth ,IParsePair_Callback* cb)
{
	TMatche* tm=new TMatche;
	tm->type=1;
	tm->cb=static_cast<void*>(cb);
	tm->matches=mth;
	dn.nodes.push_back(tm);
}
void GMatcherLearn::learn(divnode& no)
{
	if(no.type!=0)
	{
		return;
	}
	if(no.nodes.size()==0)
	{
		return;
	}
	filter_key(no);
	if(no.key_list.size()==0)
	{
		return;
	}
	uint32_t minsize_keysize=no.nodes.size();
	std::string minsize_key;
	std::set<std::string>::iterator yki;
	std::string max_match;
	uint32_t max_matchsz=0;
	for(yki=no.key_list.begin();yki!=no.key_list.end();++yki)
	{
		uint32_t key_max_vecsize=0;
		std::map<std::string,uint32_t> vl;
		uint32_t nhit=0;
		for(size_t i=0;i<no.nodes.size();++i)
		{
			std::map<std::string,std::string>::iterator g;
			g=no.nodes[i]->matches.find(*yki);
			if(g==no.nodes[i]->matches.end())
			{
				nhit++;
			}
			else
			{
				if(vl.find(g->second)==vl.end())
				{
					vl[g->second]=1;
				}
				else
				{
					vl[g->second]++;
				}
			}
		}
		////
		key_max_vecsize=nhit;
		std::map<std::string,uint32_t>::iterator gi;
		for(gi=vl.begin();gi!=vl.end();++gi)
		{
			if(gi->second>max_matchsz)
			{
				max_matchsz=gi->second;
				max_match=*yki;
			}
			gi->second+=nhit;
			if(gi->second>key_max_vecsize)
			{
				key_max_vecsize=gi->second;
			}
		}
		////
		if(key_max_vecsize<minsize_keysize)
		{
			minsize_keysize=key_max_vecsize;
			minsize_key=*yki;
		}
	}
	//
	if(minsize_keysize==no.nodes.size())
	{
		minsize_key=max_match;
	}
	//////////////////////
	no.type=1;
	no.matchkey=minsize_key;
	for(size_t i=0;i<no.nodes.size();++i)
	{
		std::map<std::string,std::string>::iterator g;
		g=no.nodes[i]->matches.find(minsize_key);
		if(g==no.nodes[i]->matches.end())
		{
			///
			if(no.other==0)
			{
				no.other=new divnode;
				no.other->key_list=no.key_list;
				no.other->key_list.erase(minsize_key);
			}
			no.other->nodes.push_back(no.nodes[i]);
		}
		else
		{
			///
			std::map<std::string,divnode*>::iterator fbi
				=no.nextb.find(g->second);
			if(fbi==no.nextb.end())
			{
				no.nextb[g->second]=new divnode;
				no.nextb[g->second]->key_list=no.key_list;
				no.nextb[g->second]->key_list.erase(minsize_key);
			}
			no.nextb[g->second]->nodes.push_back(no.nodes[i]);
		}
	}
	/////////////
	std::map<std::string,divnode*>::iterator fbi
		=no.nextb.begin();
	for(;fbi!=no.nextb.end();++fbi)
	{
		if(no.other!=0)
		{
			for(size_t hi=0;hi<no.other->nodes.size();++hi)
			{
				fbi->second->nodes.push_back(no.other->nodes[hi]);
			}
		}
		learn(*fbi->second);
	}
	if(no.other!=0)
	{
		learn(*no.other);
	}

}
void GMatcherLearn::filter_key(divnode& no)
{
	std::set<std::string>::iterator ik
		=no.key_list.begin();
	for(;ik!=no.key_list.end();)
	{
		bool bfound=false;
		for(size_t y=0;y<no.nodes.size();++y)
		{
			if(no.nodes[y]->matches.find(*ik)
				!=no.nodes[y]->matches.end())
			{
				bfound=true;
				break;
			}
		}
		if(!bfound)
		{
			std::set<std::string>::iterator dl=ik;
			++ik;
			no.key_list.erase(dl);
		}
		else
		{
			++ik;
		}
	}
}
void GMatcherLearn::final_fix()
{
	learn(dn);
	//////////

}
int GMatcherLearn::single_hit(SingleStatPack& ssp)
{
	int retv=0;
	divnode* nn=&dn;
	for(;;)
	{
		if(nn->type==0)
		{
			//hit
			for(size_t b=0;b<nn->nodes.size();++b)
			{
				if(nn->nodes[b]->type==0)
				{
				static_cast<IParseSingle_Callback*>(nn->nodes[b]->cb)
					->on_parse(ssp);
				++retv;
				}
			}
			return retv;
		}
		std::string v=ssp.get_by_string(nn->matchkey);
		std::map<std::string,divnode*>::iterator inb
			=nn->nextb.find(v);
		if(inb==nn->nextb.end())
		{
			if(nn->other==0)
			{
				return retv;
			}
			nn=nn->other;
		}
		else
		{
			nn=inb->second;
		}
	}
}
int GMatcherLearn::pair_hit(PairStatPack& psp)
{
	int retv=0;
	divnode* nn=&dn;
	for(;;)
	{
		if(nn->type==0)
		{
			//hit
			for(size_t b=0;b<nn->nodes.size();++b)
			{
				if(nn->nodes[b]->type==1)
				{
				static_cast<IParsePair_Callback*>(nn->nodes[b]->cb)
					->on_parse(psp);
				retv++;
				}
			}
			return retv;
		}
		std::string v=psp.get_by_string(nn->matchkey);
		std::map<std::string,divnode*>::iterator inb
			=nn->nextb.find(v);
		if(inb==nn->nextb.end())
		{
			if(nn->other==0)
			{
				return retv;
			}
			nn=nn->other;
		}
		else
		{
			nn=inb->second;
		}
	}
}
uint32_t GMatcherLearn::reg_size()
{
	return dn.nodes.size();
}

///////////////////////////
SPParse::SPParse()
{
	m_pofilterfile=0;
}
SPParse::~SPParse()
{
}

void SPParse::prepare()
{
	std::set<std::string> sk;
	sk.insert(std::string("srv_name"));
	sk.insert(std::string("srv_ipport"));
	sk.insert(std::string("srv_groupid"));
	sk.insert(std::string("fun_type"));
	singlelearn.set_keyllist(sk);

	std::set<std::string> pk;
	pk.insert(std::string("src_name"));
	pk.insert(std::string("src_ipport"));
	pk.insert(std::string("src_groupid"));
	pk.insert(std::string("des_name"));
	pk.insert(std::string("des_ipport"));
	pk.insert(std::string("des_groupid"));
	pk.insert(std::string("fromreq_type"));
	pk.insert(std::string("toreq_type"));
	pairlearn.set_keyllist(pk);

	singlelearn.final_fix();
	pairlearn.final_fix();
}

int SPParse::on_parse(const LogicStatBasicPack& pack)
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
		ssp.srv_name=gl_dao_proxy_name;
		ssp.srv_ipport=ipports_full;
		ssp.srv_groupid=npack_gid;
		ssp.fun_type="";
		ssp.si.error_code=npack.error.error_mp;
		////match
		if(singlelearn.single_hit(ssp)>0)
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
				ssp.srv_name=gl_dao_proxy_name;
				ssp.srv_ipport=ipports_full;
				ssp.srv_groupid=npack_gid;
				ssp.fun_type=fdi->second;
				ssp.si.resp=npack.stats[i].resp;
				ssp.si.drop=npack.stats[i].drop;
				ssp.si.send=npack.stats[i].send;
				ssp.si.recv=npack.stats[i].recv;
				////match
				if(singlelearn.single_hit(ssp)>0)
				{
					if(m_pofilterfile!=0)
					{
					npack.to_stream(*m_pofilterfile);
					return 0;
					}
				}
			}
		}
		else if(npack.stats[i].key.size()==2)
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
				APPkuohaoIp appkuohip;
				if(appkuohip.parse(iap->second)!=0)
				{
					appkuohip.app=iap->second;
					appkuohip.mip=ipports_full;
				}

				////do get one app=>dao stat
				psp.timestamp=npack.k.times;
				////from info
				psp.des_name=gl_dao_proxy_name;
				psp.des_ipport=ipports_full;
				psp.des_groupid=npack_gid;
				psp.toreq_type=ik->second;
				////to info
				psp.src_name=appkuohip.app;
				psp.src_ipport=appkuohip.mip;
				psp.src_groupid=psp.des_groupid;
				psp.fromreq_type=psp.toreq_type;
				////////////////////////
				psp.si.resp=npack.stats[i].resp;
				psp.si.drop=npack.stats[i].drop;
				psp.si.send=npack.stats[i].send;
				psp.si.recv=npack.stats[i].recv;
				////match
				if(pairlearn.pair_hit(psp)>0)
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
					psp.des_name=gl_dao_server_name;
					string ippss;
					int gidddd=0;
					if(gl_global_conf_mp.getdao_by_sname(isr->second,ippss,gidddd))
					{
						psp.des_ipport=ippss;
						psp.des_groupid=gidddd;
					}
					else
					{
						///check whether daemon info
						///"daemon_${server_id}"
						if(isr->second.substr(0,7)!=std::string("daemon_"))
						{
							continue;
						}
						std::string srvid=isr->second.substr(7);
						std::string temu="0";
						if(gl_pdaemon_infoparse->get_info(srvid,
							psp.des_ipport,psp.des_name,temu)<0)
						{
							continue;
						}
						JLexical::Cast(temu,psp.des_groupid);
					}
					psp.toreq_type=ik->second;
					////to info
					psp.src_name=gl_dao_proxy_name;
					psp.src_ipport=ipports_full;
					psp.src_groupid=npack_gid;
					psp.fromreq_type=ik->second;
					////////////////////////
					psp.si.resp=npack.stats[i].resp;
					psp.si.drop=npack.stats[i].drop;
					psp.si.send=npack.stats[i].send;
					psp.si.recv=npack.stats[i].recv;
					////match
					if(pairlearn.pair_hit(psp)>0)
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






