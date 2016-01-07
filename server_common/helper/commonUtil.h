#ifndef __COMMON_UTIL_H__
#define __COMMON_UTIL_H__

#include "server_common/server-lib/PublicFun.h"
#include <algorithm>
#include <sstream>
//#include <occi.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

namespace server{
namespace helper{
	struct common_util
	{
			static std::string trim(const std::string& s)
			{
			  std::string::size_type pos = s.find_first_not_of(' ');
			  if (pos == std::string::npos)
			  {
			    return std::string();
			  }

			  std::string::size_type n = s.find_last_not_of(' ') - pos + 1;
			  return s.substr(pos, n);
			}
			static std::string& TOUPPPER(std::string& s)
			{
				std::transform(s.begin(), s.end(), s.begin(), (int (*)(int))toupper); 
				return s;
			}
			
			static std::string& TOLOWER(std::string& s)
			{
				std::transform(s.begin(), s.end(), s.begin(), (int (*)(int))tolower); 
				return s;
			}

			static uint32_t getNumLength(uint32_t pNum){
		              char   s[20];
		              uint32_t numLength;
		              memset(s, 0x00, sizeof(s));
		              numLength = sprintf(s,"%d", pNum);
		              return numLength;
			}

/*
			//time_t to string
			static void time_t2String(const time_t *tIn, std::string &strOut){		
				char buf[30];
				memset(buf, 0x00, sizeof(buf));	
				tm* ltmp = localtime(tIn);
				sprintf(buf, "'%d-%d-%d %d/%d/%d'", ltmp->tm_year+1900, ltmp->tm_mon+1, ltmp->tm_mday,ltmp->tm_hour, ltmp->tm_min, ltmp->tm_sec);
				strOut = buf;
			}
*/
			static std::string genStringByTime_t(const time_t *tIn){
				std::ostringstream ostr;				
/*				char buf[30];
				memset(buf, 0x00, sizeof(buf));	
				tm* ltmp = localtime(tIn);
				sprintf(buf, "'%d-%d-%d %d/%d/%d'", ltmp->tm_year+1900, ltmp->tm_mon+1, ltmp->tm_mday,ltmp->tm_hour, ltmp->tm_min, ltmp->tm_sec);
				ostr << buf;
*/				
				return ostr.str();

			}
			
			  
		};
 }
}
#endif
