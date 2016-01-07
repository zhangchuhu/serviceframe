#ifndef ZKMUTEX_H
#define ZKMUTEX_H
#include "server_3rd/zookeeper-3.4.5/src/c/include/zookeeper.h"
#include <assert.h>
#include <iostream>
#include <string.h>
#include <uuid/uuid.h>

/*
	@note: zookeeper.c, log_env() --> getlogin() will affect Linux alarm. Just for log, del it!
	sample:
		zhandle_t* m_pZK = zookeeper_init(strIpPorts.c_str(), NULL, timeout_ms, NULL, NULL, 0);
		assert(m_pZK != NULL);
		...


		ZKMutex zkMutex(m_pZK, "/lock/asid_10");
		while (zkMutex.lock())
		{
			...
			zkMutex.unlock();
			break;
		}

		...
		assert(ZOK == zookeeper_close(m_pZK));
*/

class ZKMutex
{
public:	
	ZKMutex(zhandle_t* pZK, const std::string& strPath) : m_pZK(pZK), m_strPath(strPath)
	{
		uuid_t uuid;
		const int buffer_size = 40;
		char buf[buffer_size];
		memset(buf, 0, sizeof(char) * buffer_size);
		uuid_generate_random(uuid);
		uuid_unparse(uuid, buf);
		m_strUUID = buf;
	}

	bool lock() {
		int iCreateRes = zoo_create(m_pZK, m_strPath.c_str(), m_strUUID.c_str(),
				m_strUUID.size(), &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL, NULL, 0);

		if (ZOK == iCreateRes) {
			return true;
		} else if (ZNODEEXISTS == iCreateRes) {
			struct Stat stat;
			int buffer_len = 40;
			char buffer[buffer_len];
			memset(buffer, 0, sizeof(char) * buffer_len);

			int iGetRes = zoo_get(m_pZK, m_strPath.c_str(), 0, buffer, &buffer_len, &stat);
			if (ZOK == iGetRes && m_strUUID == std::string(buffer, buffer_len)) {
				return true;
			}
		}

		return false;
	}

	// @note: zoo_delete失败次数超过100次，直接assert触发进程core dump
	void unlock() {
		// TODO: 版本匹配才删
		int iDelTimes = 0;
		int iDelRes = ZOK;
		do {
			++iDelTimes;
			iDelRes = zoo_delete(m_pZK, m_strPath.c_str(), -1);

			if (iDelTimes > 1000) {
				assert(false);
				exit(-1);
			}
		} while (! (ZOK == iDelRes || ZNONODE == iDelRes));
	}

	zhandle_t* m_pZK;
	std::string m_strPath;
	std::string m_strUUID;
};

#endif
