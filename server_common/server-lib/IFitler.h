#ifndef SERVER_LIB_IFILTER_FXW_H
#define SERVER_LIB_IFILTER_FXW_H

namespace core{
	class Request;
}

namespace server{
	namespace filter{
		//appacontext process完后， 发送之前
		struct IAfterProcessFilter{
			virtual ~IAfterProcessFilter(){}
			virtual bool process(core::Request &request) = 0;
		};

		struct IAfterProcessFilterAware{
		private:
			IAfterProcessFilter *afterProcessFiter;
		public:
			IAfterProcessFilterAware():afterProcessFiter(0){}

			void setAfterProcessFilter(IAfterProcessFilter *a){afterProcessFiter = a;}
			IAfterProcessFilter *getAfterProcessFilter(){return afterProcessFiter;}
		};
	}
}

#endif

