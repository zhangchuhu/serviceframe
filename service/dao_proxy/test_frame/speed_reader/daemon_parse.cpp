#include "daemon_parse.h"
#include <vector>
#include <string>
#include <map>
#include <set>
#include <fstream>

using namespace std;

DaemonInfoParse::DaemonInfoParse()
{
	tail_fix="_tmpnewdi";
}
DaemonInfoParse::~DaemonInfoParse()
{
}

void gl_cutter_by_char(std::string& in,std::vector<std::string>& slice,char divc)
{
	slice.clear();
	size_t pos=0;
	for(;;)
	{
		size_t r=in.find_first_of(divc,pos);
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

void gl_filter_by_char(std::string& in,std::string& out,char fltc)
{
	out.clear();
	for(size_t i=0;i<in.size();++i)
	{
		if(in[i]!=fltc)
		{
			out.push_back(in[i]);
		}
	}
}

void DaemonInfoParse::init()
{
	m_all_info.clear();
}

bool DaemonInfoParse::parse(std::string file)
{
	//map<uint32_t, map<std::string, std::string> > result_info;
	ifstream inf(file.c_str());
	if(!inf||inf.fail())
	{
		return false;
	}

	std::vector<std::string> result_vss;
	std::string this_command_line;
	for(;;)
	{
		if(!getline(inf,this_command_line))
		{
			break;
		}
		if(this_command_line.empty())
		{
			continue;
		}
		///////
		std::string tmpo;
		gl_filter_by_char(this_command_line,tmpo,' ');
		gl_cutter_by_char(tmpo,result_vss,',');
		std::map<std::string,std::string> maparr;
		for(size_t b=0;b<result_vss.size();++b)
		{
			size_t fp=result_vss[b].find_first_of(':');
			if(fp!=result_vss[b].npos)
			{
				maparr[result_vss[b].substr(0,fp)]
					=result_vss[b].substr(fp+1);
			}
		}
		std::map<std::string,std::string>::iterator ivt
			=maparr.find("serverId");
		if(ivt!=maparr.end())
		{
			m_all_info[ivt->second]=maparr;
		}
	}

	return true;
}

int DaemonInfoParse::save_file(std::string file)
{
	std::string new_fll=file+tail_fix;
	ofstream onf(new_fll.c_str());
	if(!onf||onf.fail())
	{
		return -1;
	}
	/////////
	std::map<std::string, std::map<std::string, std::string> >::iterator
		itrr=m_all_info.begin();
	for(;itrr!=m_all_info.end();++itrr)
	{
		std::map<std::string, std::string>::iterator gi
			=itrr->second.begin();
		for(;gi!=itrr->second.end();++gi)
		{
			if(gi!=itrr->second.begin())
			{
				onf <<",";
			}
			onf <<gi->first <<":" <<gi->second;
		}
		onf <<endl;	
	}
	
	/////////
	onf.close();
	rename(new_fll.c_str(),file.c_str());
	return 0;
}

///[Example]
///name:savelogwan, serverId:273605795, cnc|:58.249.116.180, ctl|:121.14.47.212, port:5751, groupId:96
int DaemonInfoParse::get_info(std::string serverid,std::string& ipport,std::string& name,std::string& group)
{
	std::map<std::string, std::map<std::string, std::string> >::iterator ir
		=m_all_info.find(serverid);
	//////
	if(ir==m_all_info.end())
	{
		return -1;
	}
	std::map<std::string, std::string>::iterator nnr;
	nnr=ir->second.find("groupId");
	if(nnr!=ir->second.end())
	{
		group=nnr->second;
	}
	else
	{
		return -1;
	}
	/////////
	nnr=ir->second.find("name");
	if(nnr!=ir->second.end())
	{
		name=nnr->second;
	}
	else
	{
		return -1;
	}
	/////////
	std::string tmpget_port="0";
	nnr=ir->second.find("port");
	if(nnr!=ir->second.end())
	{
		tmpget_port=nnr->second;
	}
	else
	{
		return -1;
	}
	////////
	std::set<std::string> ipss;
	nnr=ir->second.begin();
	for(;nnr!=ir->second.end();++nnr)
	{
		size_t pstt=nnr->first.find_first_of('|');
		if(pstt!=nnr->first.npos)
		{
			ipss.insert(nnr->second);
		}
	}
	/////////
	std::set<std::string>::iterator tst=ipss.begin();
	ipport.clear();
	for(;tst!=ipss.end();++tst)
	{
		if(tst!=ipss.begin())
		{
			ipport+="|";
		}
		ipport+= (*tst);
		ipport+= ":";
		ipport+=tmpget_port;
	}
	return 0;
}

