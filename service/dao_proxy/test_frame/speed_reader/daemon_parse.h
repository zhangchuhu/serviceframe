#ifndef _DAEMON_PARSE_H_20120530_
#define _DAEMON_PARSE_H_20120530_
#include <string>
#include <map>

using namespace std;

class DaemonInfoParse
{
public:
	DaemonInfoParse();
	~DaemonInfoParse();

	void init();
	bool parse(std::string file);
	int save_file(std::string file);

	int get_info(std::string serverid,std::string& ipport,std::string& name,std::string& group);

public:
	std::map<std::string, std::map<std::string, std::string> > m_all_info;
	string tail_fix;

};


#endif	// _DAEMON_PARSE_H_20120530_

