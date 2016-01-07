#ifndef _P_HIIDO_STATISTIC_H_
#define _P_HIIDO_STATISTIC_H_

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"

namespace protocol
{
	static const unsigned int HIIDO_STATISTIC_SVID = 211;
};

namespace server{
	namespace hiido_statistic
	{
		/*
		 uid，通行证，yy号，注册时间，注册ip，注册来源，注册耗时，最后修改时间。
		*/
		struct REGISTER_INFO: public sox::Marshallable
		{
			enum {uri = (303 << 8) | protocol::HIIDO_STATISTIC_SVID};
		
			uint32_t uYyuid;
			std::string strUname;
			uint32_t uYyid;
			uint32_t uRegTime;			// unix time in second.
			std::string strRegIp;		// in dot form.
			std::string strRegSource;	
			uint32_t uRegConsume;		// in milli-second.
			uint32_t uUdbuid;

			virtual void marshal(sox::Pack &pk) const
	        {
    	        pk << uYyuid << strUname << uYyid << uRegTime << strRegIp << strRegSource << uRegConsume << uUdbuid;
	        }
    	    virtual void unmarshal( const sox::Unpack &up)
        	{
            	up >> uYyuid >> strUname >> uYyid >> uRegTime >> strRegIp >> strRegSource >> uRegConsume >> uUdbuid;
	        }
		};

		struct MIBAO_INFO: public sox::Marshallable
		{
			enum {uri = (304 << 8) | protocol::HIIDO_STATISTIC_SVID};

			uint32_t uYyuid;
			std::string strEmail;
			std::string strMobile;
			std::string strIden;
			uint32_t uModifyTime;		// unix time in second.
			std::string strMibaoQuestion; // '1' for modification ,'0' for unset ,empty for nothing

			virtual void marshal(sox::Pack &pk) const
	        {
    	        pk << uYyuid << strEmail << strMobile << strIden << uModifyTime << strMibaoQuestion;
	        }
    	    virtual void unmarshal( const sox::Unpack &up)
        	{
            	up >> uYyuid >> strEmail >> strMobile >> strIden >> uModifyTime >> strMibaoQuestion;
	        }			
			
		};	

	}
}


#endif
