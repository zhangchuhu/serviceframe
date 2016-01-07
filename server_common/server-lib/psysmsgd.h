#include "packet.h"
#include "core/base_svid.h"

namespace server{
	namespace sysmsg{
		//用于监听用户登陆
		struct PNotifyUserLogin : public sox::Marshallable
		{
			enum {uri = (1 << 8) | protocol::SYSMSGD_SVID};
			PNotifyUserLogin(){}

			uint32_t linkd_id;
			uint32_t u_id;
			
			virtual void marshal(sox::Pack &p) const
			{
				p << u_id << linkd_id;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> u_id >> linkd_id;
			}

			virtual ~PNotifyUserLogin()
			{

			}
		};

		//批量发送系统消息
		struct PSS_BatchSysMsg: public sox::Marshallable
		{
			enum {uri = (2 << 8) | protocol::SYSMSGD_SVID};
			std::string title;
			std::string message;
			std::vector<uint32_t> uids;

			virtual void marshal(sox::Pack &p) const
			{
				p << title << message;
                sox::marshal_container(p, uids);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> title >> message;
                sox::unmarshal_container(p, std::back_inserter(uids));
			}
		};
	}
}

