#ifndef __MSSELECTOR_H__
#define __MSSELECTOR_H__

#include "int_types.h"
#include "server_common/server-lib/iclient.h"
#include "server_common/helper/server_const.h"
#include "core/iserver.h"

#include <vector>
#include <string>

namespace server{
	namespace msselector{
		uint32_t MSSelector(std::vector<uint32_t>& servers);
		EMSFlag MSGetMyTypeSV(core::IDaemonClient *client, core::IDaemonServer *server, std::string sstr);
	}
}

#endif
