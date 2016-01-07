#ifndef _CONSISTENT_HASHING_H_
#define _CONSISTENT_HASHING_H_

#include <map>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/time.h>
#include <time.h>

#include "openssl/md5.h"
#include "int_types.h"

#define DEFAULTNUMBEROFREPLICAS 200

class HashFunction{
public:
	uint32_t hash(std::string& str){
		const char *ptr= str.c_str();
		unsigned char md[16];
		memset(md, sizeof(md), 0);
		MD5((const unsigned char *)ptr, str.length(), md);
		uint32_t hash = ((uint32_t) (md[3] & 0xFF) << 24)
                        | ((uint32_t) (md[2] & 0xFF) << 16)
                        | ((uint32_t) (md[1] & 0xFF) << 8)
                        | (md[0] & 0xFF);
		return hash;
	}
};



class UserConsistentHash{
public:
	UserConsistentHash(uint32_t numberOfReplicas = DEFAULTNUMBEROFREPLICAS){
		this->numberOfReplicas = numberOfReplicas;
	}


	void AddServer(uint32_t serverid) {
		for (uint32_t i = 0; i < numberOfReplicas; i++) {
			std::string sstr= GeneServerStr(serverid, i);
			server_circle[hashFunction.hash(sstr)] = serverid;
		}
	}


	void RemoveServer(uint32_t serverid) {
		for (uint32_t i = 0; i < numberOfReplicas; i++) {
			std::string sstr= GeneServerStr(serverid, i);
			server_circle.erase(hashFunction.hash(sstr));
		}
	}

	uint32_t GetServer(uint32_t uid) {
		uint32_t sid;
		//timeval tim;
		//gettimeofday(&tim, NULL);
		//double t1=tim.tv_sec+(tim.tv_usec/1000000.0);

		if (server_circle.empty()) {
			//log(Warn, "[UserConsistentHash::GetServer]: server_circle is empty!!!!!!!");
			//printf("server_circle is empty!!!!!!!\n");
			return -1;
		}

		std::string ustr = GetUserStr(uid);
		uint32_t hash = hashFunction.hash(ustr);

		T_Key_Sid::iterator it = server_circle.lower_bound(hash);
		if (it == server_circle.end()){ // not found, select the begin one.
			sid = (server_circle.begin())->second;
		}else{ // found
			sid = server_circle[it->first];
		}
/*
		T_Key_Sid::iterator it = server_circle.find(hash);
		if (it == server_circle.end()){
			sid = -1;
			for (it = server_circle.begin(); it != server_circle.end(); ++it){
				if (it->first > hash){
					sid = server_circle[it->first];
					break;
				}
			}

			if (static_cast<const uint32_t>(-1) == sid){
				sid = (server_circle.begin())->second;
			}
		}else
			sid = server_circle[hash];
*/

		//log(Info, "[UserConsistentHash::GetServer]: uid = %d, imlinkd serverid = %d", uid, sid);
		//printf("uid = %d, sid = %d", uid, sid);

		//gettimeofday(&tim, NULL);
		//double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
		//printf("get server spend: %.6lf seconds\n", t2-t1);

		return sid;
	}

private:
	 uint32_t numberOfReplicas;
	 HashFunction hashFunction;
	 typedef std::map<uint32_t, uint32_t> T_Key_Sid;
	 T_Key_Sid server_circle;

private:
	std::string GeneServerStr(uint32_t serverid, uint32_t index){
		std::ostringstream serverStr;
		serverStr << "IMLINKD_SERVERID: " << serverid << "; INDEX: " << index;
		return serverStr.str();
	}


	std::string GetUserStr(uint32_t uid){
		std::ostringstream userStr;
		userStr << "USER_ID: " << uid << "RANDOM_STRING";
		return userStr.str();
	}
};

#endif

