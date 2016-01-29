#include "source_util.h"
#include <sstream>
#include "server_common/seda/common/any_cast.h"
#include <time.h>
#include <stdexcept>
#include <memory>
#include <vector>
#include <iterator>
#include <cctype>

using namespace std;

int parse_colon_string2map(std::string& ss,std::map<std::string,std::string>& mp)
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

void strip_ch(std::string& str,char ch)
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


void g_cutter_space(std::string& in,std::vector<std::string>& slice)
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