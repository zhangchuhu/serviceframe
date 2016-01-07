#ifndef _P_KEY_AUDITOR_H_
#define _P_KEY_AUDITOR_H_

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"

namespace protocol
{
	static const unsigned int KEY_AUDITOR_SVID = 212;
};

namespace server{
	namespace key_auditor
	{
		struct PTicketVerifyDetail: public sox::Marshallable
		{
			enum {uri = (1 << 8) | protocol::KEY_AUDITOR_SVID};			
		
			uint32_t uYyuid;
			std::string strAppid;
			std::string strServerType;
			uint32_t uStStartTime;
			uint32_t uStEndTime;
			uint32_t uAuthSecond;
			uint32_t uAuthMilli;
			uint32_t uClientIp;
			std::string strUsername;
			uint32_t uKeyVersion;
			uint32_t uEncryptType;	
			uint64_t uServiceId;		

			virtual void marshal(sox::Pack &pk) const
			{
				pk << uYyuid << strAppid << strServerType << uStStartTime << uStEndTime << uAuthSecond << uAuthMilli \
					<< uClientIp << strUsername << uKeyVersion << uEncryptType << uServiceId;		
			}
			virtual void unmarshal( const sox::Unpack &up)
			{
				up >> uYyuid >> strAppid >> strServerType >> uStStartTime >> uStEndTime >> uAuthSecond >> uAuthMilli \
					>> uClientIp >> strUsername >> uKeyVersion >> uEncryptType;

				uServiceId = 0;
				if ( false == up.empty() ){
					try{
						up >> uServiceId;			  
					}
					catch(...){			  
					}
				}			
			}
		};
		
		struct PChangePswRecord: public sox::Marshallable
		{
			enum {uri = (1 << 8) | 221};			
		
			uint64_t yyuid;
			uint32_t ChangePswTime;

			virtual void marshal(sox::Pack &pk) const
	        {
    	        pk << yyuid << ChangePswTime;
		
	        }
    	    virtual void unmarshal( const sox::Unpack &up)
        	{
            	up >> yyuid >> ChangePswTime;				
	        }
		};
	}
}


#endif
