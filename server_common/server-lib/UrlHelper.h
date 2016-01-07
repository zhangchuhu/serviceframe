#ifndef URL_HELPER_H_
#define URL_HELPER_H_
#include <vector>
#include <string>
#include "common/int_types.h"

#define  ERROR_UID 0


uint32_t hashUid(uint32_t uid);
//std::string uid2Url(uint32_t uid);
//std::string uid2Url(uint32_t uid, const char* suffix);
std::string uid2LinkUrl(uint32_t uid);
std::string uidFrom(uint32_t uid, uint32_t serverId);
std::string uidFromUdp(uint32_t uid, uint32_t serverId);
uint32_t url2Uid(const std::string &url);
std::string url2UidStr(const std::string &url);
std::string extractParam(const std::string &url);

class DomainName{
	std::vector<std::string> domains;
	std::string param;
public:
	std::string toDomain();
	std::string toUrl();
	bool isUser();
	std::string getRoot() const;
	std::string getLeaf() const;
	bool isRoot() const;

	const std::string &getParam() const;
	uint32_t getParami() const;

	static DomainName parse(const std::string &query);
	static std::string toDomain(const std::vector<std::string> &s, const std::string &suffix);
	static std::string extactParam(const std::string &url);
	static std::string trimParam(const std::string &url);
	static bool toUid(const std::string &url, uint32_t &out);
	//Õâ¸ösidÊÇserverId
	static bool getSid(const std::string &url, uint32_t &out);
	static bool getSessionId(const std::string &url, uint32_t &out);
};
#endif

