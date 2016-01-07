#include <vector>
#include <string>
#include "protocol/const.h"
#include "core/base_svid.h"

namespace protocol 
{
	namespace ygauthd 
	{

		struct PGAuthTokenReq: public sox::Marshallable 
		{
			enum 
			{
				uri = (1 << 8) | protocol::YGAUTHD_SVID
			};

			std::string requestText;

			virtual void marshal(sox::Pack &p) const 
			{
				p << requestText;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> requestText;
			}
		};

		struct PGAuthTokenRes: public sox::Marshallable 
		{
			enum 
			{
				uri = (2 << 8) | protocol::YGAUTHD_SVID
			};

			std::string autToken;

			virtual void marshal(sox::Pack &p) const 
			{
				p << autToken;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> autToken;
			}
		};
	}
}

