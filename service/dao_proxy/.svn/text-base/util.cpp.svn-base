#include "util.h"
#include <sstream>
#include "server_common/seda/common/any_cast.h"
#include <time.h>
#include <stdexcept>
#include <memory>
#include <vector>
#include <iterator>
#include <cctype>

using namespace std;

void parseStr2VecStr(const std::string& str, std::vector<std::string>& vec_str,char div)
{
	vec_str.clear();
	size_t pos=0;
	for(;;)
	{
		size_t r=str.find_first_of(div,pos);
		if(r==std::string::npos)
		{
			if(pos<str.size())
			{
				vec_str.push_back(str.substr(pos));
			}
			break;
		}
		if(pos<r)
		{
			vec_str.push_back(str.substr(pos,r-pos));
		}
		pos=r+1;
	}
}
void parseVecStr2Str(const std::vector<std::string>& vec_str, std::string& str,char div)
{
	str.clear();
	for(size_t i=0;i<vec_str.size();++i)
	{
		if(!str.empty())
		{
			str.push_back(div);
		}
		str+=vec_str[i];
	}
}


void parseStr2SetUint(const std::string& str, std::set<uint32_t>& st_uint)
{
	st_uint.clear();
	std::vector<std::string> vec_str;
	parseStr2VecStr(str,vec_str);
	for(size_t i=0;i<vec_str.size();++i)
	{
		uint32_t tem_i=0;
		JLexical::Cast(vec_str[i],tem_i);
		st_uint.insert(tem_i);
	}
}

void parseSetUint2Str(const std::set<uint32_t>& st_uint, std::string& str)
{
	str.clear();
	for(std::set<uint32_t>::const_iterator i=st_uint.begin();i!=st_uint.end();++i)
	{
		if(!str.empty())
		{
			str+=",";
		}
		std::string tems="0";
		JLexical::Cast(*i,tems);
		str+=tems;
	}
}

void parseStr2SetInt(const std::string& str, std::set<int>& st_int)
{
	st_int.clear();
	std::vector<std::string> vec_str;
	parseStr2VecStr(str,vec_str,'_');
	for(size_t i=0;i<vec_str.size();++i)
	{
		int tem_i=0;
		JLexical::Cast(vec_str[i],tem_i);
		st_int.insert(tem_i);
	}
}
void parseSetInt2Str(const std::set<int>& st_int, std::string& str)
{
	str.clear();
	for(std::set<int>::const_iterator i=st_int.begin();i!=st_int.end();++i)
	{
		if(!str.empty())
		{
			str+="_";
		}
		std::string tems="0";
		JLexical::Cast(*i,tems);
		str+=tems;
	}
}

std::string parseMapIS2String(std::map<int,std::string>& original)
{
	ostringstream oss;
	if (original.empty())
		return string();

	for (map<int,string>::const_iterator it=original.begin(); it!=original.end(); ++it)
	{   
		string enStr = cgicc::form_urlencode(it->second); //call encode interface
		oss << it->first << "=" << enStr << ",";
	}   
	string res = oss.str();
	res = res.substr(0, res.size()-1);
	return res;
}

void parseString2MapIS(std::string& str, std::map<int,std::string>& value)
{
	std::vector<std::string> slice;
	parseStr2VecStr(str,slice);
	value.clear();

	for (vector<string>::iterator it=slice.begin(); it!=slice.end(); ++it)
	{   
		size_t pos = it->find_first_of("=");
		if (pos != string::npos)
		{   
			int key = atoi(it->data());
			string suffStr = it->substr(pos+1);
			string val = cgicc::form_urldecode(suffStr); 
			value[key] = val;
		}   
	}
}
std::string parseMapII2String(std::map<int,int>& original)
{
	ostringstream oss;
	if (original.empty())
		return string();

	for (map<int,int>::const_iterator it=original.begin(); it!=original.end(); ++it)
	{   
		oss << it->first << "=" << it->second << ",";
	}   
	string res = oss.str();
	res = res.substr(0, res.size()-1);
	return res;
}
void parseString2MapII(std::string& str, std::map<int,int>& value)
{
	std::vector<std::string> slice;
	parseStr2VecStr(str,slice);
	value.clear();

	for (vector<string>::iterator it=slice.begin(); it!=slice.end(); ++it)
	{   
		size_t pos = it->find_first_of("=");
		if (pos != string::npos)
		{   
			int key = atoi(it->data());
			string suffStr = it->substr(pos+1);
			int val = atoi(suffStr.data());
			value[key] = val;
		}   
	}
}

std::string parseMapSS2String(std::map<std::string,std::string>& original)
{
	ostringstream oss;
	if (original.empty())
		return string();

	for (map<string,string>::const_iterator it=original.begin(); it!=original.end(); ++it)
	{   
		string enStr = cgicc::form_urlencode(it->second); //call encode interface
		string enKey = cgicc::form_urlencode(it->first); //call encode interface
		oss << enKey << "=" << enStr << ",";
	}   
	string res = oss.str();
	res = res.substr(0, res.size()-1);
	return res;
}
void parseString2MapSS(std::string& str, std::map<std::string,std::string>& value)
{
	std::vector<std::string> slice;
	parseStr2VecStr(str,slice);
	value.clear();

	for (vector<string>::iterator it=slice.begin(); it!=slice.end(); ++it)
	{   
		size_t pos = it->find_first_of("=");
		if (pos != string::npos)
		{   
			string subk=it->substr(0,pos);
			string key = cgicc::form_urldecode(subk); 
			string suffStr = it->substr(pos+1);
			string val = cgicc::form_urldecode(suffStr); 
			value[key] = val;
		}   
	}
}

int diffDays(const time_t tBegin, const time_t tEnd)
{
	time_t _dayBegin = tBegin;
	time_t _dayEnd = tEnd;

	struct tm tmTmp;
	localtime_r(&_dayBegin, &tmTmp);
	tmTmp.tm_hour = tmTmp.tm_min = tmTmp.tm_sec = 0;
	_dayBegin = mktime(&tmTmp);
	
	if (_dayEnd == 0)
	{
		_dayEnd = time(NULL);
	}
	localtime_r(&_dayEnd, &tmTmp);
	tmTmp.tm_hour = tmTmp.tm_min = tmTmp.tm_sec = 0;
	_dayEnd = mktime(&tmTmp);
	int days = (_dayEnd - _dayBegin)/(24*60*60);
	return (days>0)?days:0;
}

void parseStr2VecIPP(const std::string& str, std::vector<std::pair<std::string,int> >& vec_ipp)
{
	vec_ipp.clear();
	if(str=="empty")
	{
		return ;
	}
	std::vector<std::string> slice;
	size_t pos=0;
	for(;;)
	{
		size_t r=str.find_first_of('|',pos);
		if(r==std::string::npos)
		{
			if(pos<str.size())
			{
				slice.push_back(str.substr(pos));
			}
			break;
		}
		if(pos<r)
		{
			slice.push_back(str.substr(pos,r-pos));
		}
		pos=r+1;
	}
	for(size_t i=0;i<slice.size();++i)
	{
		size_t r=slice[i].find_first_of(':',0);
		if(r!=std::string::npos&&r!=0&&(r!=slice[i].size()-1))
		{
			std::string get_ip=slice[i].substr(0,r);
			std::string get_port=slice[i].substr(r+1);
			int real_prt=0;
			JLexical::Cast(get_port,real_prt);
			vec_ipp.push_back( std::pair<std::string,int>(get_ip,real_prt) );
		}
	}
}
void parseVecIPP2Str(const std::vector<std::pair<std::string,int> >& vec_ipp, std::string& str)
{
	string ipspss;
	for(size_t i=0;i<vec_ipp.size();++i)
	{
		string tem;
		JLexical::Cast(vec_ipp[i].second,tem);
		if(i!=0)
		{
			ipspss+="|";
		}
		ipspss+=(vec_ipp[i].first+":"+tem);
	}
	if(ipspss.empty())
	{
		ipspss="empty";
	}
	str.swap(ipspss);
}




bool cgicc::stringsAreEqual(const std::string& s1, 
		       const std::string& s2)
{
  std::string::const_iterator p1 = s1.begin();
  std::string::const_iterator p2 = s2.begin();
  std::string::const_iterator l1 = s1.end();
  std::string::const_iterator l2 = s2.end();

  while(p1 != l1 && p2 != l2) {
    if(std::toupper(*(p1++)) != std::toupper(*(p2++)))
      return false;
  }

  return (s2.size() == s1.size()) ? true : false;
}


bool cgicc::stringsAreEqual(const std::string& s1, 
		       const std::string& s2,
		       size_t n)
{
  std::string::const_iterator p1 = s1.begin();
  std::string::const_iterator p2 = s2.begin();
  bool good = (n <= s1.length() && n <= s2.length());
  std::string::const_iterator l1 = good ? (s1.begin() + n) : s1.end();
  std::string::const_iterator l2 = good ? (s2.begin() + n) : s2.end();

  while(p1 != l1 && p2 != l2) {
    if(std::toupper(*(p1++)) != std::toupper(*(p2++)))
      return false;
  }
  return good;
}

std::string cgicc::charToHex(char c)
{
  std::string result;
  char first, second;

  first = (c & 0xF0) / 16;
  first += first > 9 ? 'A' - 10 : '0';
  second = c & 0x0F;
  second += second > 9 ? 'A' - 10 : '0';

  result.append(1, first);
  result.append(1, second);
  
  return result;
}

char cgicc::hexToChar(char first,
		 char second)
{
  int digit;
  
  digit = (first >= 'A' ? ((first & 0xDF) - 'A') + 10 : (first - '0'));
  digit *= 16;
  digit += (second >= 'A' ? ((second & 0xDF) - 'A') + 10 : (second - '0'));
  return static_cast<char>(digit);
}

 
//Note RFC 1738 is obsoleted by RFC 2396.  Basically it says to
//escape out the reserved characters in the standard %xx format.  It
//also says this about the query string:
//
//query         = *uric
//uric          = reserved | unreserved | escaped
//reserved      = ";" | "/" | "?" | ":" | "@" | "&" | "=" | "+" |
//"$" | ","
//unreserved    = alphanum | mark
//mark          = "-" | "_" | "." | "!" | "~" | "*" | "'" |
//"(" | ")"
//escaped = "%" hex hex 
 
std::string cgicc::form_urlencode(const std::string& src)
{
  std::string result;
  std::string::const_iterator iter;

  for(iter = src.begin(); iter != src.end(); ++iter) {
    switch(*iter) {
    case ' ':
      result.append(1, '+');
      break;
      // alnum
    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
    case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
    case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
    case 'V': case 'W': case 'X': case 'Y': case 'Z':
    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
    case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
    case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
    case 'v': case 'w': case 'x': case 'y': case 'z':
    case '0': case '1': case '2': case '3': case '4': case '5': case '6':
    case '7': case '8': case '9':
      // mark
    case '-': case '_': case '.': case '!': case '~': case '*': case '\'': 
    case '(': case ')':
      result.append(1, *iter);
      break;
      // escape
    default:
      result.append(1, '%');
      result.append(charToHex(*iter));
      break;
    }
  }
  
  return result;
}

std::string cgicc::form_urldecode(const std::string& src)
{
  std::string result;
  std::string::const_iterator iter;
  char c;

  for(iter = src.begin(); iter != src.end(); ++iter) {
    switch(*iter) {
    case '+':
      result.append(1, ' ');
      break;
    case '%':
      // Don't assume well-formed input
      if(std::distance(iter, src.end()) >= 2
	 && std::isxdigit(*(iter + 1)) && std::isxdigit(*(iter + 2))) {
	c = *++iter;
	result.append(1, hexToChar(c, *++iter));
      }
      // Just pass the % through untouched
      else {
	result.append(1, '%');
      }
      break;
    
    default:
      result.append(1, *iter);
      break;
    }
  }
  return result;
}


std::string cgicc::extractBetween(const std::string& data, 
		      const std::string& separator1, 
		      const std::string& separator2)
{
  std::string result;
  std::string::size_type start, limit;
  
  start = data.find(separator1, 0);
  if(std::string::npos != start) {
    start += separator1.length();
    limit = data.find(separator2, start);
    if(std::string::npos != limit)
      result = data.substr(start, limit - start);
  }
  
  return result;
}


void cgicc::writeString(std::ostream& out, const std::string& s)
{ 
  out << s.length() << ' ';
  out.write(s.data(), s.length()); 
}

void cgicc::writeLong(std::ostream& out, 
		 unsigned long l)
{ 
  out << l << ' '; 
}

std::string cgicc::readString(std::istream& in)
{
  std::string::size_type dataSize = 0;
  
  in >> dataSize;
  in.get(); // skip ' '
  
  // Avoid allocation of a zero-length vector
  if(0 == dataSize) {
    return std::string();
  }

  // Don't use auto_ptr, but vector instead
  // Bug reported by bostjan@optonline.net / fix by alexoss@verizon.net
  std::vector<char> temp(dataSize);

  in.read(&temp[0], dataSize);
  if(static_cast<std::string::size_type>(in.gcount()) != dataSize) {
    throw std::runtime_error("I/O error");
  }
  return std::string(&temp[0], dataSize);
}

unsigned long cgicc::readLong(std::istream& in)
{
  unsigned long l;
  in >> l;
  in.get(); // skip ' '
  return l;
}

