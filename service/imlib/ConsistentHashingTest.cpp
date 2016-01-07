//#include "stdafx.h"
#include "ConsistentHashing.h"
#include <sys/time.h>
#include <time.h>
#include <vector>
#include <map>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


int main()
{
	std::vector<uint32_t> servers;
	std::map<uint32_t, std::vector<uint32_t> > sid_uids;
	UserConsistentHash consHash(200);
	srand ( time(NULL) );




	//start time count
	timeval tim;
	gettimeofday(&tim, NULL);
	double t1=tim.tv_sec+(tim.tv_usec/1000000.0);





	//step1: add servers
	int i;
	for (i = 20; i != 100; ++i){
		//uint32_t serverid = rand() % 100;
		uint32_t serverid = i;
		//printf("add serverid: %d\n", serverid);
		servers.push_back(serverid);
		consHash.AddServer(serverid);
	}

	gettimeofday(&tim, NULL);
	double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
	printf("step1: %.6lf seconds elapsed\n", t2-t1);





	//step2: add users
	srand ( time(NULL) );
	for (i = 0; i != 5000; ++i){
		uint32_t uid = rand() % 1000000 + 100000;
		//printf("uid: %d\n", uid);
		uint32_t sid = consHash.GetServer(uid);
		assert(sid != -1);
		sid_uids[sid].push_back(uid);
		//printf("uid = %d, sid = %d\n", uid, sid);
	}

	gettimeofday(&tim, NULL);
	double t3=tim.tv_sec+(tim.tv_usec/1000000.0);
	printf("step2: %.6lf seconds elapsed\n", t3-t2);

	for (std::map<uint32_t, std::vector<uint32_t> >::iterator it2 = sid_uids.begin(); it2 != sid_uids.end(); ++it2){
		printf("on nomal condition: sid_uids: sid = %d, usernum = %d\n", (*it2).first, (*it2).second.size());
	}





	//step3: add more servers (no count time)
	for (i = 100; i != 101; ++i){
		//uint32_t serverid = rand() % 100;
		uint32_t serverid = i;
		//printf("add serverid: %d\n", serverid);
		servers.push_back(serverid);
		consHash.AddServer(serverid);
	}





	//step4: rellocate users
	std::map<uint32_t, std::vector<uint32_t> > changes_sid_uids;
	std::map<uint32_t, std::vector<uint32_t> > new_sid_uids;
	for (std::map<uint32_t, std::vector<uint32_t> >::iterator it2 = sid_uids.begin(); it2 != sid_uids.end(); ++it2){
		std::vector<uint32_t>::iterator it3 = it2->second.begin();
		for (; it3 != it2->second.end();){
			uint32_t sid = consHash.GetServer(*it3);
			if (sid != it2->first){
				changes_sid_uids[it2->first].push_back(*it3);
				new_sid_uids[sid].push_back(*it3);				
				sid_uids[sid].push_back(*it3);
				it3 = sid_uids[it2->first].erase(it3);
			}else
				it3 = ++it3;
		}
	}

	for (std::map<uint32_t, std::vector<uint32_t> >::iterator it2 = changes_sid_uids.begin(); it2 != changes_sid_uids.end(); ++it2){
		printf("on add more servers: changes_sid_uids: sid = %d, usernum = %d\n", (*it2).first, (*it2).second.size());
	}

	for (std::map<uint32_t, std::vector<uint32_t> >::iterator it2 = new_sid_uids.begin(); it2 != new_sid_uids.end(); ++it2){
		printf("on add more servers: new_sid_uids: sid = %d, usernum = %d\n", (*it2).first, (*it2).second.size());
	}
	for (std::map<uint32_t, std::vector<uint32_t> >::iterator it2 = sid_uids.begin(); it2 != sid_uids.end(); ++it2){
		printf("on add more servers: sid_uids: sid = %d, usernum = %d\n", (*it2).first, (*it2).second.size());
	}

	gettimeofday(&tim, NULL);
	double txxxx=tim.tv_sec+(tim.tv_usec/1000000.0);
	printf("step4: %.6lf seconds elapsed\n", txxxx-t3);







	//step5: remove a server (no count time)
	std::vector<uint32_t> removed_users;
	std::vector<uint32_t>::iterator it_one = servers.begin();
	uint32_t serverid = *it_one;
	servers.push_back(serverid);
	removed_users = sid_uids[serverid];
	sid_uids.erase(serverid);
	consHash.RemoveServer(serverid);







	//step4: rellocate users
	changes_sid_uids.clear();
	new_sid_uids.clear();
	for (std::map<uint32_t, std::vector<uint32_t> >::iterator it2 = sid_uids.begin(); it2 != sid_uids.end(); ++it2){
		std::vector<uint32_t>::iterator it3 = it2->second.begin();
		for (; it3 != it2->second.end();){
			uint32_t sid = consHash.GetServer(*it3);
			if (sid != it2->first){
				changes_sid_uids[it2->first].push_back(*it3);
				new_sid_uids[sid].push_back(*it3);				
				sid_uids[sid].push_back(*it3);
				it3 = sid_uids[it2->first].erase(it3);
			}else
				it3 = ++it3;
		}
	}

	for (std::map<uint32_t, std::vector<uint32_t> >::iterator it2 = changes_sid_uids.begin(); it2 != changes_sid_uids.end(); ++it2){
		printf("on remove a server: changes_sid_uids: sid = %d, usernum = %d\n", (*it2).first, (*it2).second.size());
	}

	for (std::map<uint32_t, std::vector<uint32_t> >::iterator it2 = new_sid_uids.begin(); it2 != new_sid_uids.end(); ++it2){
		printf("on remove a server: new_sid_uids: sid = %d, usernum = %d\n", (*it2).first, (*it2).second.size());
	}
	for (std::map<uint32_t, std::vector<uint32_t> >::iterator it2 = sid_uids.begin(); it2 != sid_uids.end(); ++it2){
		printf("on remove a server: sid_uids: sid = %d, usernum = %d\n", (*it2).first, (*it2).second.size());
	}

	gettimeofday(&tim, NULL);
	double t_remove_server = tim.tv_sec+(tim.tv_usec/1000000.0);
	printf("step6: %.6lf seconds elapsed\n", t_remove_server-t3);






	
	//readd removed users
	for(std::vector<uint32_t>::iterator it = removed_users.begin(); it != removed_users.end(); ++it){
		uint32_t sid = consHash.GetServer(*it);
		sid_uids[sid].push_back(*it);
		//printf("uid = %d, sid = %d\n", uid, sid);
	}

	for (std::map<uint32_t, std::vector<uint32_t> >::iterator it2 = sid_uids.begin(); it2 != sid_uids.end(); ++it2){
		printf("on readd removed users: sid_uids: sid = %d, usernum = %d\n", (*it2).first, (*it2).second.size());
	}












	//step7: remove all servers, and rellocate all users
	for (std::vector<uint32_t>::iterator it = servers.begin(); it != servers.end(); ++it){
		//printf("del serverid: %d\n", *it);
		consHash.RemoveServer(*it);
	}
	
	gettimeofday(&tim, NULL);
	double t4=tim.tv_sec+(tim.tv_usec/1000000.0);
	printf("step7: %.6lf seconds elapsed\n", t4-t3);







	//step8: relocate all users
	srand ( time(NULL) );
	for (i = 0; i != 10000; ++i){
		uint32_t uid = rand() % 100 + 200000;
		//printf("uid: %d\n", uid);
		uint32_t sid = consHash.GetServer(uid);
		assert(sid == -1);
		//printf("uid = %d, sid = %d\n", uid, sid);
	}

	gettimeofday(&tim, NULL);
	double t5=tim.tv_sec+(tim.tv_usec/1000000.0);
	printf("step8: %.6lf seconds elapsed\n", t5-t4);





	//total time elapsed
	printf("total: %.6lf seconds elapsed\n", t5-t1);
	return 0;
}
