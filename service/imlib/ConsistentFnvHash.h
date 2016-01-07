#ifndef _CONSISTENT_FNV_HASH_H_
#define _CONSISTENT_FNV_HASH_H_

#include <map>
#include <string>
#include <time.h>
#include <sstream>
#include <iostream>
#include <sys/time.h>
#include "int_types.h"

#define DEFAULTNUMBEROFREPLICAS 200

class FnvHashFunction
{
public:
    uint32_t hash(const char *pKey, size_t ulen)
    {
        register uint32_t uMagic = 16777619;
        register uint32_t uHash = 0x811C9DC5;//2166136261L;

        while(ulen--)
        {
            uHash = (uHash ^ (*(unsigned char *)pKey)) * uMagic;
            pKey++;
        }

        uHash += uHash << 13;
        uHash ^= uHash >> 7;
        uHash += uHash << 3;
        uHash ^= uHash >> 17;
        uHash += uHash << 5;

        return uHash;    
     }

    inline uint32_t hash(uint32_t i32)
    {
        return hash((const char *) &i32, sizeof(i32));
    }
};


class UserConsistentFnvHash
{
public:
    UserConsistentFnvHash(uint32_t numberOfReplicas = DEFAULTNUMBEROFREPLICAS)
    {
        this->numberOfReplicas = numberOfReplicas;
    }

    void AddServer(uint32_t serverid)
    {
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

    void RemoveServer(uint32_t serverid)
    {
        char hashbuf[sizeof(uint32_t) * 2];
        memcpy(hashbuf, &serverid, sizeof(serverid));

        for (uint32_t i = 0; i < numberOfReplicas; i++)
        {
            memcpy(hashbuf + sizeof(serverid), &i, sizeof(i));
            uint32_t uValue = hashFunction.hash(hashbuf, sizeof(hashbuf));
            server_circle.erase(uValue);
        }
    }

    uint32_t GetServer(uint32_t uid)
    {
        uint32_t sid;

        if (server_circle.empty())
        {
            return -1;
        }

        uint32_t hash = hashFunction.hash(uid);

        T_Key_Sid::iterator it = server_circle.lower_bound(hash);
        if (it == server_circle.end())
        {
            sid = server_circle.begin()->second;
        }
        else
        {
            sid = it->second;
        }

        return sid;
    }

private:
     uint32_t numberOfReplicas;
     FnvHashFunction hashFunction;
     typedef std::map<uint32_t, uint32_t> T_Key_Sid;
     T_Key_Sid server_circle;
};

#endif
