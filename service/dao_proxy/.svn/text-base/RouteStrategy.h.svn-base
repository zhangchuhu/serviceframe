#ifndef _ROUTESTRATEGYES_H_20111108
#define _ROUTESTRATEGYES_H_20111108
#include <map>
#include <vector>
#include <string>

#include "global_typedef.h"

namespace server
{
	namespace dao_proxy
	{

		class RouteStrategy
		{
		public:
			RouteStrategy();
			~RouteStrategy();

		public:
			int init_load_conf();

			//return value 0:ok -1:fail
			int get_route_str(std::string* app_name,std::string* dao_name,ROUTESTRATEGY_TYPE& filter);

		private:
			std::map<std::string,ROUTESTRATEGY_TYPE> m_app_route;
			std::map<std::string,ROUTESTRATEGY_TYPE> m_dao_route;
			std::map<std::pair<std::string,std::string> ,ROUTESTRATEGY_TYPE> m_appdao_route;
			ROUTESTRATEGY_TYPE m_default_strategy;
		public:
			void debug_show(std::stringstream& ss);
		};

	}
}

#endif	// _ROUTESTRATEGYES_H_20111108

