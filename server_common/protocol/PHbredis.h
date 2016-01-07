#pragma once

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"
#include <vector>
#include <string>
#include <map>
#include <pthread.h>
#include <semaphore.h>
namespace server{
	namespace plug{

		struct PHbRedisReq:public sox::Marshallable
		{
			   enum {uri = 1 << 8 | protocol::UDB_SVID};
            
			   uint32_t uid;
         std::string content;
			
			   virtual void marshal(sox::Pack &pk) const
			   {
				     pk << uid << content;
			   }
			   virtual void unmarshal(const sox::Unpack &up)
			   {
				     up >> uid >> content;
			   }
		};

		struct PHbRedisRes:public sox::Marshallable
		{
		    enum {uri = 2 << 8 | protocol::UDB_SVID};

        //std::string result;
        std::string content;
	      uint32_t uid;

			
			  virtual void marshal(sox::Pack &pk) const
			  {
				    pk << content << uid ; 
			  }
			  virtual void unmarshal(const sox::Unpack &up)
			  {
				    up  >> content >> uid ;
			  }
		};		

	}
}
