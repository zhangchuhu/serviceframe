#ifndef SERVER_COMMON___COMMON___COMMON_H
#define SERVER_COMMON___COMMON___COMMON_H

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include <string.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>

namespace server{ namespace xlinux{


/*
 * lock file
 * @return 0 success, other fail
 */
inline int xlockf(const char* pFileName)
{
    int fd;
    int lock_result;
    fd = open(pFileName, O_CREAT|O_RDWR);

    if(fd<0)
    {
        printf("Open file failed.\n");
        return -1;
    }
    lock_result = lockf(fd,F_TEST,0);  //参数使用F_LOCK，则如果已经加锁，则阻塞到前一个进程释放锁为止，参数0表示对整个文件加锁
    //返回0表示未加锁或者被当前进程加锁；返回-1表示被其他进程加锁
    if(lock_result<0)
    {
        perror("Exec lockf function failed.\n");
        return -2;
    }

    lock_result = lockf(fd,F_LOCK,0);  //参数使用F_LOCK，则如果已经加锁，则阻塞到前一个进程释放锁为止，参数0表示对整个文件加锁
    if(lock_result<0)
    {
        perror("Exec lockf function failed.\n");
        return -3;
    }

    return 0;
}


inline uint64_t get_current_ms()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return  (uint64_t)tv.tv_sec * 1000 + (uint64_t)tv.tv_usec/1000;
}

inline uint64_t get_current_us()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return  (uint64_t)tv.tv_sec * 1000000 + (uint64_t)tv.tv_usec;
}

inline void tokenize_str(const std::string& str, std::vector<std::string>& tokens,
        const std::string& delims = ", \t")
{
    using namespace std;
    // Skip delims at beginning, find start of first token
    string::size_type lastPos = str.find_first_not_of(delims, 0);
    // Find next delimiter @ end of token
    string::size_type pos = str.find_first_of(delims, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delims.  Note the "not_of". this is beginning of token
        lastPos = str.find_first_not_of(delims, pos);
        // Find next delimiter at end of token.
        pos = str.find_first_of(delims, lastPos);
    }
}

inline std::string u64_to_string(uint64_t u64)
{
	char arr[256];
	memset(arr, 0, sizeof(char) * 256);

	sprintf(arr, "%"PRIu64, u64);
	return arr;
}

inline std::string i64_to_string(int64_t i64)
{
	char arr[256];
	memset(arr, 0, sizeof(char) * 256);

	sprintf(arr, "%"PRIi64, i64);
	return arr;
}

}}
#endif
