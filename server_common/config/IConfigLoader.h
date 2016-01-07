#ifndef ICONFIG_LOADER_H_FXW
#define ICONFIG_LOADER_H_FXW
#include <stdio.h>

namespace server{
	namespace config{
		struct IConfigLoader{
			virtual ~IConfigLoader(){};
			virtual void load(FILE *fp) = 0;
		};

	}
}
#endif

