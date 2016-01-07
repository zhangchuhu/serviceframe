//以下是为数据平台组提供查询频道在线用户列表、查询用户所在频道的协议
//注 SERVICE_SVID= 88
//topSid 是顶级频道id
//sid是子频道id
//isWorking == 1时工作正常
#include "common/core/base_svid.h"
#include "common/protocol/const.h"
#include <vector>
#include <iterator>
#include <map> 
#include <set>
#include <utility>
#include <string>
#ifndef __USER_CHANNEL_SEARCH_PROTOCOL
#define  __USER_CHANNEL_SEARCH_PROTOCOL
using namespace std;
namespace protocol
{
	namespace svcCommon
	{

		struct PSS_QueryChannelUserList: public sox::Marshallable
		{
			enum{uri= ( 157 << 8 |protocol::SERVICE_SVID )};
			uint32_t topSid;
			uint32_t maxUidCount;
			string context;
			PSS_QueryChannelUserList()
				:topSid(0)
				 ,maxUidCount(100000000)
			{}
			virtual void marshal(sox::Pack &p) const
			{
				p<<topSid<<maxUidCount<<context;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p>> topSid>>maxUidCount>>context;
			}
		};
		struct PSS_QueryChannelUserListRes: public sox::Marshallable
		{
			enum{uri= ( 158 << 8 |protocol::SERVICE_SVID )};
			uint32_t topSid;
			std::set<uint32_t> uids;
			uint32_t isWorking;
			string context;
			PSS_QueryChannelUserListRes()
				:topSid(0)
				 ,uids()
				 ,isWorking(1)
			{}
			virtual void marshal(sox::Pack &p) const
			{
				p<<topSid;
				sox::marshal_container(p,uids);
				p<<isWorking;
				p<< context;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p>> topSid;
				sox::unmarshal_container(p,std::inserter(uids,uids.begin()));
				p>>isWorking;
				p>> context;
			}
		};
		struct PSS_QueryChannelIdByUid: public sox::Marshallable
		{
			enum{uri= ( 159 << 8 |protocol::SERVICE_SVID )};
			uint32_t uid;
			string context;	
			PSS_QueryChannelIdByUid()
				:uid(0)
			{}
			virtual void marshal(sox::Pack &p) const
			{
				p<<uid;
				p<<context;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p>>uid;
				p>>context;
			}
		};
		struct PSS_QueryChannelIdByUidRes: public sox::Marshallable
		{
			enum{uri= ( 160 << 8 |protocol::SERVICE_SVID )};
			uint32_t uid;
			uint32_t topSid;
			uint32_t sid;
			uint32_t isWorking;
			string context;	
			PSS_QueryChannelIdByUidRes()
				:uid(0)
				 ,topSid(0)
				 ,sid(0)
				 ,isWorking(1)
			{}
			virtual void marshal(sox::Pack &p) const
			{
				p<<uid<<topSid<<sid<<isWorking;
				p<< context;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p>>uid>>topSid>>sid>>isWorking;
				p>>context;
			}
		};

		///////////////////////
		struct DBI_QueryUidbyPassport: public sox::Marshallable
		{
			enum{uri= ( 3670 << 8 |protocol::SERVICE_SVID )};
			string passport;
			string context;
			DBI_QueryUidbyPassport()
			{}
			virtual void marshal(sox::Pack &p) const
			{
				p<<passport<<context;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p>>passport>>context;
			}
		};
		struct DBI_QueryUidbyPassportRes: public sox::Marshallable
		{
			enum{uri= ( 3671 << 8 |protocol::SERVICE_SVID )};
			uint32_t uid;
			uint32_t rescode;
			string context;
			DBI_QueryUidbyPassportRes()
				:uid(0)
				,rescode(0)
			{}
			virtual void marshal(sox::Pack &p) const
			{
				p<<uid;
				p<<rescode;
				p<<context;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p>>uid;
				p>>rescode;
				p>>context;
			}
		};
		struct DBI_GSettleGuildByUid: public sox::Marshallable
		{
			enum{uri= ( 3672 << 8 |protocol::SERVICE_SVID )};
			uint32_t uid;
			string context;	
			DBI_GSettleGuildByUid()
				:uid(0)
			{}
			virtual void marshal(sox::Pack &p) const
			{
				p<<uid;
				p<<context;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p>>uid;
				p>>context;
			}
		};
		struct DBI_GSettleGuildByUidRes: public sox::Marshallable
		{
			enum{uri= ( 3673 << 8 |protocol::SERVICE_SVID )};
			uint32_t uid;
			uint32_t sgid;
			uint32_t rescode;
			string context;	
			DBI_GSettleGuildByUidRes()
				:uid(0)
				,sgid(0)
				,rescode(0)
			{}
			virtual void marshal(sox::Pack &p) const
			{
				p<<uid<<sgid<<rescode;
				p<<context;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p>>uid>>sgid>>rescode;
				p>>context;
			}
		};
		struct DBI_GRolerByUid: public sox::Marshallable
		{
			enum{uri= ( 3674 << 8 |protocol::SERVICE_SVID )};
			vector<std::pair<uint32_t,uint32_t> > uid_sid;
			string context;
			DBI_GRolerByUid()
			{}
			virtual void marshal(sox::Pack &p) const
			{
				uint32_t szz=uid_sid.size();
				p <<szz;
				for(uint32_t i=0;i<szz;++i)
				{
					p<<(uid_sid[i]).first;
					p<<(uid_sid[i]).second;
				}
				p<<context;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				uint32_t szz=0;
				p>>szz;
				for(uint32_t i=0;i<szz;++i)
				{
					uid_sid.resize(i+1);
					p>>uid_sid[i].first;
					p>>uid_sid[i].second;
				}
				p>>context;
			}
		};
		struct DBI_GRolerByUidRes: public sox::Marshallable
		{
			enum{uri= ( 3675 << 8 |protocol::SERVICE_SVID )};
			vector<uint32_t> rolers;///order same as request
			uint32_t rescode;
			string context;	
			DBI_GRolerByUidRes()
				:rescode(0)
			{}
			virtual void marshal(sox::Pack &p) const
			{
				uint32_t szz=rolers.size();
				p <<szz;
				for(uint32_t i=0;i<szz;++i)
				{
					p<<rolers[i];
				}
				//////////
				p<<rescode;
				p<<context;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				uint32_t szz=0;
				p>>szz;
				for(uint32_t i=0;i<szz;++i)
				{
					rolers.resize(i+1);
					p>>rolers[i];
				}
				///////////
				p>>rescode;
				p>>context;
			}
		};
	}

}
#endif
