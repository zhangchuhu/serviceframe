#ifndef UNIXDAEMON_H_
#define UNIXDAEMON_H_

#include <string.h>
#include <string>


struct string_util
{
		enum{
				max_line_size = 1024
		};
		static int getline(std::string& stream, std::string& line, uint32_t npos = 0)
		{
				int nsize = 0; int i = 0;
				for(; (i<max_line_size) && (npos+i<stream.size()); ++ i)
				{
						if(stream[i+npos] == '\n') {
								nsize = i + 1;
								break;
						}
						if(stream[i+npos] == '\r'){
								if(((i+npos + 1) < stream.size()) && stream[i+npos+1] == '\n') {
										nsize = i + 2; break;
								}else{
										nsize = i + 1; break;
								}
						}
				}
				line = stream.substr(npos, i);
				return nsize;
		}
};


#ifndef WIN32
#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <io.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <core/sox/logger.h>
#include "./version.h"




class file_system{
public:
		static void mkdirs(const char* path)
		{
				std::string dir = path;
				size_t pos = std::string::npos;
				while(true){
						pos = dir.find("/", pos+1);
						if(pos == std::string::npos)  return;
						if(pos)
								mkdir(dir.substr(0, pos).c_str(), 0777);
				}
		};

		static bool create_file(const char* pathname)
		{
				return creat(pathname, O_CREAT|O_TRUNC) >= 0;
		};
		static bool remove_file(const char* /*pathname*/)
		{
				return false;
		}
};

static void set_core_limit(int cur, int max)
{
		rlimit rl;
		rl.rlim_cur = cur;
		rl.rlim_max = max;
		setrlimit(RLIMIT_CORE, &rl);
}


static  void set_file_limit(int cur, int max)
{
		rlimit rl;
		rl.rlim_cur = cur;
		rl.rlim_max = max;
		setrlimit(RLIMIT_NOFILE, &rl);
		rlimit limit;
		getrlimit(RLIMIT_NOFILE, &limit);
}




class init_daemon {
public:
	init_daemon(int sz, char *args[]){
		if(sz == 2 && strcmp(args[1], "-v") == 0){
			printf("%s version:%d\n", args[0], SERVER_VERSION);
			exit(0);
		}
		/*if(sz < 2){
			printf("need log file name\n");
			exit(-1);
		}*/
		
		/*int pid;
		newFile = NULL;
		if ((pid=fork()) != 0)
			exit(0);//是父进程，结束父进程
		else if (pid< 0)
			exit(1);//fork失败，退出
		//是第一子进程，后台继续执行

		setsid();//第一子进程成为新的会话组长和进程组长
		//并与控制终端分离
		if ((pid=fork()) != 0)
			exit(0);//是第一子进程，结束第一子进程
		else if (pid< 0)
			exit(1);//fork失败，退出
		//是第二子进程，继续
		//第二子进程不再是会话组长

		//signal(SIGPIPE, SIG_IGN);
			
		for (int i=2; i< NOFILE; ++i)
			//关闭打开的文件描述符
			close(i);
		*/
		
		/*chdir("/data/yy");//改变工作目录到/tmp
		mkdir("/data/yy/log", 0777);
		chdir("/data/yy/log");
		mkdir(args[0], 0777);
		chdir(args[0]);
		umask(0);//重设文件创建掩模
        */
		/*char *fn = args[1];

		if(!sox::setlog(fn,100*1024)){
				exit(1);
		}*/
		initLog();
		set_file_limit(65535, 1000000);
        set_core_limit(1024 * 1024 * 1024,  1024 * 1024 * 1024);
	}

	~init_daemon() {
	}
};
#else

class init_daemon {
public:
	init_daemon(int sz, char *args[]){}
};

class file_system
{
public:
		static void mkdirs(const char* path)
		{
		};

		static bool create_file(const char* pathname)
		{
				return false;
		};
		static bool remove_file(const char* pathname)
		{
				return false;
		}
};

#endif
#endif /*UNIXDAEMON_H_*/

