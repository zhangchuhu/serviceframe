#ifndef _TIME_UTIL_H_
#define _TIME_UTIL_H_

#include <string>
#include <sstream>
#include <time.h>


struct time_util
{

static void transfer_time(time_t seconds,std::string& time_string)
{
	time_t now_sc=seconds;
	struct tm* ttm;
	ttm=localtime(&now_sc);
	stringstream ss;
	int act_year=1900+ttm->tm_year;
	int act_month=1+ttm->tm_mon;
	ss <<act_year <<"-" <<act_month <<"-" <<ttm->tm_mday <<" " 
		<<ttm->tm_hour <<":" <<ttm->tm_min <<":" <<ttm->tm_sec ;
	time_string=ss.str();
}
static void get_current_time(std::string& time_string)
{
	time_t now_sc=time(0);
	transfer_time(now_sc,time_string);
}
static void get_current_time_add(std::string& time_string,int add_sec)
{
	time_t now_sc=time(0)+add_sec;
	transfer_time(now_sc,time_string);
}

};


#endif

