#ifndef _SOURCEUTIL_2011_H_
#define _SOURCEUTIL_2011_H_

#include <new>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <stdlib.h>

int parse_colon_string2map(std::string& ss,std::map<std::string,std::string>& mp);
void strip_ch(std::string& str,char ch);
void g_cutter_space(std::string& in,std::vector<std::string>& slice);


#endif //_SOURCEUTIL_2011_H_
