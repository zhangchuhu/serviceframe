//#include "stdafx.h"
#include "./VerifyCodeHelper.h"
#include <sys/time.h>
#include <time.h>
#include <vector>
#include <map>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <list>
#include <iostream>
#include "server/config/TinyXmlDbConfigImp.h"
#include "config/MemcachedConfigImp.h"
#include "dblib/MemcachedFactory.h"
#include "config/LoaderContainer.h"

using namespace std;
using namespace server::config;
using namespace server::mc;

int main()
{
	std::vector<uint32_t> servers;
	std::map<uint32_t, std::vector<uint32_t> > sid_uids;
	server::verify_code::VerifyCodeHelper helper;

	MemcachedFactory __memcachedFactory;
	MemcachedConfigImp __memcachedConfImp;
	
	FILE *fp = fopen("../bin/server.xml", "r");
	__memcachedConfImp.load(fp);

	__memcachedFactory.setMcConfig(&__memcachedConfImp);

	fclose(fp);
	helper.setMemcachedFactory(&__memcachedFactory);

	srand ( time(NULL) );


	printf("verifycode test start......\n");

	//start time count
	timeval tim;
	gettimeofday(&tim, NULL);
	double t1=tim.tv_sec+(tim.tv_usec/1000000.0);


	//step1: add servers
	int i;
	std::list<std::string> vtKey;
	for (i = 0; i != 100000000; ++i){
		protocol::verify_code::VerifyCodeObject obj;
		obj.strKey = "";
		helper.getVerifyCodeAndKey(obj);
		assert(obj.strKey!="" && obj.pPng != NULL);
		vtKey.push_back(obj.strKey);
	}

	gettimeofday(&tim, NULL);
	double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
	printf("step1: %.6lf seconds elapsed and %d times to getVerifyCodeAndKey()\n", t2-t1, i);

	i = 0;
	for (std::list<std::string>::iterator iter = vtKey.begin(); iter != vtKey.end(); ++iter){
		helper.checkVerifyCode(*iter, "1234");
		++i;
	}


	gettimeofday(&tim, NULL);
	double txxxx=tim.tv_sec+(tim.tv_usec/1000000.0);
	printf("step2: %.6lf seconds elapsed and %d times to checkVerifyCode()\n", txxxx-t2, i);


	return 0;
}
