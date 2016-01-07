#pragma once 

#include "common/int_types.h"
#include "common/openssl/md5.h"

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <set>

#define DEFAULTNUMBEROFREPLICAS 200

//·þÎñÆ÷¹þÏ£Ëã·¨
enum EHashFuntcionType
{
	ENUM_FUNC_MD5 = 0,
	ENUM_FUNC_NEWHASH,
	ENUM_FUNC_FNVHASH
};

class CConsistentHash
{
	std::map<uint32_t, uint32_t> &m_mapServerCircle;
	EHashFuntcionType m_emFuncType;
	uint32_t m_numberOfReplicas;

	typedef std::map<uint32_t, uint32_t> MAP_CIRCLE;

public:
	CConsistentHash(MAP_CIRCLE &mapServerCircle, EHashFuntcionType emFuncType = ENUM_FUNC_MD5, uint32_t numberOfReplicas = DEFAULTNUMBEROFREPLICAS):
	  m_mapServerCircle(mapServerCircle), m_emFuncType(emFuncType), m_numberOfReplicas(numberOfReplicas) {};

	void AddServer(uint32_t uServerValue)
	{
		if (m_emFuncType == ENUM_FUNC_MD5)
		{
			for (uint32_t i = 0; i < m_numberOfReplicas; i ++) 
			{
				std::string strServer = GeneServerStr(uServerValue, i);
				m_mapServerCircle[ md5hash(strServer) ] = uServerValue;
			}
		}
		else
		{
            uint32_t uValue = 0;
            char hashbuf[sizeof(uint32_t) * 2];
            memcpy(hashbuf, &uServerValue, sizeof(uServerValue));

			for (uint32_t i = 0; i < m_numberOfReplicas; i ++) 
			{
                memcpy(hashbuf + sizeof(uServerValue), &i, sizeof(i));
                if(ENUM_FUNC_NEWHASH == m_emFuncType)
                {
                    uValue = newhash(hashbuf, sizeof(hashbuf));
                }
                else
                {
                    uValue = fnvhash(hashbuf, sizeof(hashbuf));
                }

                std::map<uint32_t, uint32_t>::iterator iter = m_mapServerCircle.find(uValue);
                if (iter != m_mapServerCircle.end())
                {
                    log(Warn, "server circle duplicate! serverid:%u index:%u", uServerValue, i);
                }
				m_mapServerCircle[uValue] = uServerValue;
			}
		}
	}


	void RemoveServer(uint32_t uServerValue)
	{
		if (m_emFuncType == ENUM_FUNC_MD5)
		{
			for (uint32_t i = 0; i < m_numberOfReplicas; i++) 
			{
				std::string strServer = GeneServerStr(uServerValue, i);
				m_mapServerCircle.erase( md5hash(strServer) );
			}
		}
		else
		{
            char hashbuf[sizeof(uint32_t) * 2];
            memcpy(hashbuf, &uServerValue, sizeof(uServerValue));

			for (uint32_t i = 0; i < m_numberOfReplicas; i ++) 
			{
                memcpy(hashbuf + sizeof(uServerValue), &i, sizeof(i));
                if(ENUM_FUNC_NEWHASH == m_emFuncType)
                {
				    m_mapServerCircle.erase( newhash(hashbuf, sizeof(hashbuf)) );
                }
                else
                {
                    m_mapServerCircle.erase( fnvhash(hashbuf, sizeof(hashbuf)) );
                }
			}
		}
	}

	uint32_t GetValueByKey(uint32_t uKey)
	{
		if (m_mapServerCircle.empty()) 
			return -1;

		uint32_t uSValue = -1;
		uint32_t uKeyHash = -1;

		if (m_emFuncType == ENUM_FUNC_MD5)
		{
			std::string strKey = GetKeyStr(uKey);
			uKeyHash = md5hash(strKey);
		}
		else if(ENUM_FUNC_NEWHASH == m_emFuncType)
		{
			uKeyHash = newhash(uKey);
		}
        else
        {
            uKeyHash = fnvhash(uKey);
        }

		MAP_CIRCLE::iterator it = m_mapServerCircle.lower_bound(uKeyHash);
		if (it != m_mapServerCircle.end())
		{
			uSValue = it->second;
		}
		else
		{
			uSValue = (m_mapServerCircle.begin())->second;
		}

		return uSValue;
	}

	bool GetMultiValueByKey(uint32_t uKey, uint32_t uValueCount, std::set<uint32_t>& setValues)
	{
		if (m_mapServerCircle.empty())
		{
			return false;
		}

		uint32_t uGetCount = uValueCount;
		if (m_mapServerCircle.size() < (uValueCount * m_numberOfReplicas))
		{
			uGetCount = m_mapServerCircle.size()/m_numberOfReplicas;
		}

		uint32_t uKeyHash = -1;

		if (m_emFuncType == ENUM_FUNC_MD5)
		{
			std::string strKey = GetKeyStr(uKey);
			uKeyHash = md5hash(strKey);
		}
		else if(ENUM_FUNC_NEWHASH == m_emFuncType)
		{
			uKeyHash = newhash(uKey);
		}
		//for fnv hash
        else
        {
            uKeyHash = fnvhash(uKey);
        }

		MAP_CIRCLE::iterator it = m_mapServerCircle.lower_bound(uKeyHash);
		if (it != m_mapServerCircle.end())
		{
			setValues.insert(it->second);
		}
		else
		{
			setValues.insert((m_mapServerCircle.begin())->second);
		}

		while (setValues.size() < uGetCount)
		{
			it++;
			if (it == m_mapServerCircle.end())
			{
				it = m_mapServerCircle.begin();
			}

			if (setValues.find(it->second) == setValues.end())
			{
				setValues.insert(it->second);
			}
		}

		return true;
	}

private:
	static std::string GeneServerStr(uint32_t uServerValue, uint32_t uIndex)
	{
		std::ostringstream ssServer;
		ssServer << "IMLINKD_SERVERID: " << uServerValue << "; INDEX: " << uIndex;
		return ssServer.str();
	}
	static std::string GetKeyStr(uint32_t uKey)
	{
		std::ostringstream ssKey;
		ssKey << "USER_ID: " << uKey << "RANDOM_STRING";
		return ssKey.str();
	}

public:

	static uint32_t md5hash(std::string &str)
	{
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

	static uint32_t newhash(const char *ptr, size_t length)
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

	static uint32_t newhash(uint32_t i32)
	{
		return newhash((const char *) &i32, sizeof(i32));
	}

    static uint32_t fnvhash(const char *pKey, size_t ulen)
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
    
     static uint32_t fnvhash(uint32_t i32)
     {
         return fnvhash((const char *) &i32, sizeof(i32));
     }
    
};
