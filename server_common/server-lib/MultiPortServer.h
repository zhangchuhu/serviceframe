#include "server_common/server-lib/DaemonServer.h"
#include "common/int_types.h"

#include <vector>

class MultiPortServer : public core::SimplePrxServer
{
protected:
	std::vector<uint16_t> curports;
	virtual core::ServerSocketHelper* create(const char* ip, uint16_t p, uint16_t &cur);

public:
	virtual core::ServerSocketHelper *createHelper(const char *ip);
	virtual std::vector<uint16_t> getPorts();
	virtual void start();
};
