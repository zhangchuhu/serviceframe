#ifndef _P_HISTORY_PASSWORD_H_
#define _P_HISTORY_PASSWORD_H_

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"

namespace protocol
{
	static const unsigned int HISPWD_SVID = 106;
};

namespace server{
	namespace hispwd
	{
		struct PIsHistoryPassword :  public sox::Marshallable{
			enum {uri = (1 << 8) | protocol::HISPWD_SVID};

			std::string strSeq; // asynchronous sequence number.
			std::string strUserName;
			std::string strPasswordSha1;
			unsigned int uIp;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << strUserName << strPasswordSha1 << uIp;		
			}

			virtual void unmarshal(const sox::Unpack &p){
				p >> strSeq >> strUserName >> strPasswordSha1 >> uIp;				
			}
		};		

		struct PIsHistoryPasswordRes :  public sox::Marshallable{
			enum {uri = (2 << 8) | protocol::HISPWD_SVID};

			std::string strSeq; // asynchronous sequence number.
			unsigned int uResult; // 0 - history password  .  1 - not a history password  . 2 system error.

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << uResult;
			}

			virtual void unmarshal(const sox::Unpack &p){
				p >> strSeq >> uResult;				
			}
		};	

		struct PInsertHistoryPassword :  public sox::Marshallable{
			enum {uri = (3 << 8) | protocol::HISPWD_SVID};

			std::string strUserName;
			std::string strPassword; // maybe enctyped.
			std::string strClientIp; // a host address in dot form.

			virtual void marshal(sox::Pack &p) const {
				p << strUserName << strPassword << strClientIp;		
			}

			virtual void unmarshal(const sox::Unpack &p){
				p >> strUserName >> strPassword >> strClientIp;				
			}
		};				
	}
}


#endif
