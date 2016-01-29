#ifndef _UTIL_2011_H_
#define _UTIL_2011_H_

#include <new>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <stdlib.h>
#include "common/int_types.h"

void parseStr2VecStr(const std::string& str, std::vector<std::string>& st_uint,char div=',');
void parseVecStr2Str(const std::vector<std::string>& st_uint, std::string& str,char div=',');

void parseStr2SetUint(const std::string& str, std::set<uint32_t>& st_uint);
void parseSetUint2Str(const std::set<uint32_t>& st_uint, std::string& str);

void parseStr2SetInt(const std::string& str, std::set<int>& st_int);
void parseSetInt2Str(const std::set<int>& st_int, std::string& str);

std::string parseMapIS2String(std::map<int,std::string>& original);
void parseString2MapIS(std::string& str, std::map<int,std::string>& value);

std::string parseMapSS2String(std::map<std::string,std::string>& original);
void parseString2MapSS(std::string& str, std::map<std::string,std::string>& value);


int diffDays(const time_t tBegin, const time_t tEnd=0);

void parseStr2VecIPP(const std::string& str, std::vector<std::pair<std::string,int> >& vec_ipp);
void parseVecIPP2Str(const std::vector<std::pair<std::string,int> >& vec_ipp, std::string& str);


namespace cgicc {

  bool stringsAreEqual(const std::string& s1, const std::string& s2);
  bool stringsAreEqual(const std::string& ss1, const std::string& ss2, size_t n);

  std::string form_urldecode(const std::string& src);
  std::string form_urlencode(const std::string& src);
  std::string charToHex(char c);

  char hexToChar(char first,char second);

  std::string
  extractBetween(const std::string& data, const std::string& separator1, const std::string& separator2);
  inline std::string
  extractBetween(const std::string& datas, const std::string& separators)
  { return extractBetween(datas, separators, separators); }

  void writeString(std::ostream& out, const std::string& s);
  void writeLong(std::ostream& out, unsigned long l);
  std::string readString(std::istream& in);
  unsigned long  readLong(std::istream& in);
  
} // namespace cgicc



#endif //_UTIL_2011_H_
