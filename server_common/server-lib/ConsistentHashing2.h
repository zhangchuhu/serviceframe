#ifndef _CONSISTENT2_HASHING_H_
#define _CONSISTENT2_HASHING_H_

#include <map>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/time.h>
#include <time.h>
#include "int_types.h"

#define DEFAULTNUMBEROFREPLICAS 200


class HashFunction2
{
public:
    uint32_t hash(const char *ptr, size_t length)
    {
        const char *end = ptr + length;
        uint32_t hash;

        for (hash = 0; ptr < end; ptr++)
        {
            hash *= 16777619;
            hash ^= (uint32_t) *((unsigned char *) ptr);
        }
        return hash;
    }

    inline uint32_t hash(uint32_t i32)
    {
        return hash((const char *) &i32, sizeof(i32));
    }
};



class UserConsistentHash2
{
public:
	UserConsistentHash2(uint32_t numberOfReplicas = DEFAULTNUMBEROFREPLICAS){
		this->numberOfReplicas = numberOfReplicas;
	}

	void AddServer(uint32_t serverid) {
        char hashbuf[sizeof(uint32_t) * 2];
        memcpy(hashbuf, &serverid, sizeof(serverid));

		for (uint32_t i = 0; i < numberOfReplicas; i++) 
        {
            memcpy(hashbuf + sizeof(serverid), &i, sizeof(i));
            uint32_t uValue = hashFunction.hash(hashbuf, sizeof(hashbuf));

            std::map<uint32_t, uint32_t>::iterator iter = server_circle.find(uValue);
            if (iter != server_circle.end())
            {
                log(Warn, "server circle duplicate! serverid:%u index:%u", serverid, i);
            }
            server_circle[uValue] = serverid;
		}
	}


	void RemoveServer(uint32_t serverid) {
        char hashbuf[sizeof(uint32_t) * 2];
        memcpy(hashbuf, &serverid, sizeof(serverid));

		for (uint32_t i = 0; i < numberOfReplicas; i++) {
            memcpy(hashbuf + sizeof(serverid), &i, sizeof(i));
            uint32_t uValue = hashFunction.hash(hashbuf, sizeof(hashbuf));
			server_circle.erase(uValue);
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

		uint32_t hash = hashFunction.hash(uid);

		T_Key_Sid::iterator it = server_circle.lower_bound(hash);
		if (it == server_circle.end()){ // not found, select the begin one.
				sid = server_circle.begin()->second;
		}else{ // found
				sid = it->second;
		}

		//log(Info, "[UserConsistentHash::GetServer]: uid = %d, imlinkd serverid = %d", uid, sid);
		//printf("uid = %d, sid = %d", uid, sid);

		//gettimeofday(&tim, NULL);
		//double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
		//printf("get server spend: %.6lf seconds\n", t2-t1);

		return sid;
	}

	size_t GetServerNum() const {
		return server_circle.size();
	}

	bool GetAllServers(std::set<uint32_t>& setServers){
		if(server_circle.empty())
		{
			return false;
		}

		for(T_Key_Sid::iterator it = server_circle.begin(); it != server_circle.end(); ++it)
		{
			setServers.insert(it->second);
		}
		return true;
	}

private:
	 uint32_t numberOfReplicas;
	 HashFunction2 hashFunction;
	 typedef std::map<uint32_t, uint32_t> T_Key_Sid;
	 T_Key_Sid server_circle;
};

#endif

