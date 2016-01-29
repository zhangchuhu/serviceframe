#ifndef _PXCONF_H_20111027
#define _PXCONF_H_20111027
#include <map>
#include <vector>
#include <string>
#include "server_common/seda/stage/stage_base.h"
#include "global_typedef.h"

namespace server
{
	namespace dao_proxy
	{

		class PxConf
		{
		public:
			PxConf();
			~PxConf();

			// -1 fail and exit, 0 success
			int32_t init(std::string conf_path);
		public:
			std::string server_name;
			int groupid;
			int port;
			std::string myip;
		};

	}
}

#endif	// _PXCONF_H_20111027

