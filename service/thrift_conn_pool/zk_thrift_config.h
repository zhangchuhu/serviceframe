#ifndef ZK_THRIFT_CONFIG_H
#define ZK_THRIFT_CONFIG_H

#include <string>
#include <map>
#include "../../common/int_types.h"
#include "libzkclient.h"

namespace server { namespace config {
	
struct ZkThriftConfig{

	std::string host;
	uint16_t port;
	uint32_t groupid;
	uint32_t time_out;
	bool operator== (const ZkThriftConfig &right){
		return host == right.host && port == right.port && groupid == right.groupid;
	}
};
class ThriftConfigLoader: public ZkClientAware
{
public:
	ThriftConfigLoader(const std::string& proc_name);
	~ThriftConfigLoader();
	int loadThriftConfig();
	std::string getParentPath();
	uint32_t getThriftPort();
	std::string getThriftIp();

private:
	bool loaded;
	std::map<std::string, std::vector<ZkThriftConfig> > thrifts;
};
}}

#endif
