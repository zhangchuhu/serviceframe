#include "calc_class.h"
#include <vector>
#include <string>
#include <map>
#include <set>
#include <fstream>
#include "time_util.h"


using namespace std;


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




