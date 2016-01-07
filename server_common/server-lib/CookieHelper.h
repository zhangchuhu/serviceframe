#ifndef COOKIE_HELPER_H_
#define COOKIE_HELPER_H_
#include <vector>
#include <string>
#include "common/int_types.h"
#include "openssl/blowfish.h"

namespace server{
class CookieHelper{
	time_t time;
	int version;
	uint32_t uid;
	std::string passport;
	std::string from;
	uint32_t ip;
	std::string sha_passwd;
	std::string extension;
	uint64_t termId;
public:
	static BF_KEY m_key;	// @note: use a common BF_KEY
	static bool m_bInitBFKey;

	CookieHelper(uint32_t u, const std::string &p, uint32_t ip, const std::string &passwd, const char *from, uint64_t termId);

	CookieHelper(const CookieHelper& ck);

	static void InitBFKey(void);

	static CookieHelper parse(const std::string &str);

	std::string toCookie() const;

	bool isValid(uint32_t uid) const;

	static CookieHelper parse20(const std::string &str);

	std::string toCookie20() const;

	bool isValid20(std::string account, const std::string& sha_passwd) const;

	bool isValid20NoTime(uint32_t uid, std::string account, const std::string& sha_passwd) const;

	static CookieHelper parse30(const std::string &str);

	std::string toCookie30() const;

	bool isValid30(uint32_t uid, const std::string & account, const std::string & sha_passwd, const std::string & extension) const;

	static CookieHelper parse40(const std::string &str);

	std::string toCookie40() const;

	const std::string &getPassport() const;
	
	const std::string &getExtension() const;

	void operator=(const CookieHelper& ck);

	void dump() const;

    uint32_t getUid() const{
        return uid;
    }
	uint32_t getTime() const{
		return time;
	}

	int getVersion() const{
		return version;
	}

	uint32_t getIp() const{
		return ip;
	}

	std::string getFrom() const{
		return from;
	}

	void setTime(uint32_t t){
		time = t;
	}

	void setVersion(int v){
		version = v;
	}

	const uint64_t &getTermId() const
	{
		return termId;
	}

	const uint32_t getAppKey() const
	{
		return termId >> 32;
	}

	const uint32_t getTerminalType() const
	{
		return termId & 0x00000000FFFFFFFFul;
	}

};
}
#endif

