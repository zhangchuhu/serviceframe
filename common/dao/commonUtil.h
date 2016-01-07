#ifndef __COMMON_UTIL_H__
#define __COMMON_UTIL_H__

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
};

#endif
