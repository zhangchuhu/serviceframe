#ifndef SERVER_BRANCHESFILTER_FXW_H
#define SERVER_BRANCHESFILTER_FXW_H
#include "IFitler.h"
#include "BRouteWriter.h"
namespace server{
	namespace filter{
		//appacontext process完后， 发送之前
		class BranchesFilter: public IAfterProcessFilter{
			core::ABRouteWriter *writer;
			std::string target;
		public:
			void setTarget(const std::string &name){
				target = name;
			}
			virtual bool process(core::Request &request);
			void setRouteWriter(core::ABRouteWriter *w){
				writer = w;
			}
		};
	}
}
#endif

