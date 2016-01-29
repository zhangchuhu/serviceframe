#ifndef _PX_ERROR_CODE_H_
#define _PX_ERROR_CODE_H_

#include <string>

namespace server
{
	namespace dao_proxy
	{
		std::string error_code_string(int code)
		{
			switch(code)
			{
				case 0:return std::string("no_error");
				case 1:return std::string("unknown_error");
				case 101:return std::string("network_error");
				case 201:return std::string("daobusy");
				case 100101:return std::string("authorize_fail");
				case 100102:return std::string("no_licence");
				case 100103:return std::string("throttled");
				case 100104:return std::string("nodao");
				default:return std::string("undefined_error");
			}
			return std::string("");
		}
		struct ErrorCodeObj
		{
			ErrorCodeObj(std::string s)
			{
				if(s==std::string("no_error")){			code=0;}
				else if(s==std::string("unknown_error")){code=1;}
				else if(s==std::string("network_error")){code=101;}
				else if(s==std::string("daobusy")){code=201;}
				else if(s==std::string("authorize_fail")){code=100101;}
				else if(s==std::string("no_licence")){code=100102;}
				else if(s==std::string("throttled")){code=100103;}
				else if(s==std::string("nodao")){code=100104;}
				else{code=-1;}
			}
			~ErrorCodeObj(){}
			int code;
		private:
			ErrorCodeObj();
		};
	}

}


#endif //_PX_ERROR_CODE_H_
