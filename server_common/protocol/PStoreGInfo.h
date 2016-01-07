#ifndef PSTOREGINFO_H
#define PSTOREGINFO_H

#include <string>
#include "common/packet.h"
#include "common/core/base_svid.h"

namespace server {
namespace store {
struct PSS_CreateGroupReq: public sox::Marshallable {
	enum {
		uri = (40 << 8) | protocol::ACCESS_SVID
	};
	uint32_t uGroupId; 		// group id
	uint32_t uGroupNum;		// group number for users to remember
	uint32_t uOwnerId; 		// user id of creator
	uint32_t uClass; 		// group class
	uint32_t uSubclass; 	// group subclass
	uint32_t uAuthMode; 	// authentication mode to join group
	uint32_t uChannelId;	// authentication mode with session id 
	uint16_t uChannelRoleMask; // authentication mode mask
	uint32_t uCreateTime;	// group create time
	std::string strName;	// group name
	std::string strDesc;	// group description

	virtual void marshal(sox::Pack &p) const
	{
		p << uGroupId << uGroupNum << uOwnerId << uClass << uSubclass << uAuthMode << uChannelId  
			<< uChannelRoleMask << uCreateTime << strName << strDesc;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> uGroupId >> uGroupNum >>uOwnerId >> uClass >> uSubclass >> uAuthMode >> uChannelId 
			>> uChannelRoleMask >> uCreateTime >> strName >> strDesc;
	}

};

struct PSS_UpdateGroupReq: public sox::Marshallable {
	enum {
		uri = (41 << 8) | protocol::ACCESS_SVID
	};
	uint32_t uGroupId; 		// group id will never be updated
	uint32_t uGroupNum;		// don't overwrite if its value is 0
	uint32_t uOwnerId; 		// don't overwrite if its value is 0xffffffff
	uint32_t uClass; 		// don't overwrite if its value is 0xffffffff
	uint32_t uSubclass; 	// don't overwrite if its value is 0xffffffff
	uint32_t uAuthMode; 	// don't overwrite if its value is 0xffffffff
	uint32_t uChannelId;	// don't overwrite if its value is 0xffffffff
	uint16_t uChannelRoleMask; // don't overwrite if its value is 0xffff
	uint32_t uCreateTime;	// don't overwrite if its value is 0
	std::string strName;	// don't overwrite if its value is ""
	std::string strDesc;	// don't overwrite if its value is ""

	PSS_UpdateGroupReq()
	{
		Reset();
	}

	//For ginfo
	PSS_UpdateGroupReq(uint32_t uOwnerId)
	{
		Reset();
		uOwnerId = uOwnerId;
	}

	void Reset()
	{
		uGroupId = 0;
		uGroupNum = 0;
		uOwnerId = 0xffffffff;
		uClass = 0xffffffff;
		uSubclass = 0xffffffff;
		uAuthMode = 0xffffffff;
		uChannelRoleMask = 0xffff;
		uChannelId = 0xffffffff;
		uCreateTime = 0;
		strName = "";
		strDesc = "";
	}

	virtual void marshal(sox::Pack &p) const
	{
		p << uGroupId << uGroupNum << uOwnerId << uClass << uSubclass << uAuthMode
			<< uChannelId << uChannelRoleMask << uCreateTime << strName << strDesc;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> uGroupId >> uGroupNum >> uOwnerId >> uClass >> uSubclass >> uAuthMode
			>> uChannelId >> uChannelRoleMask >> uCreateTime >> strName >> strDesc;
	}

};

struct PSS_DeleteGroupReq: public sox::Marshallable {
	enum {
		uri = (42 << 8) | protocol::ACCESS_SVID
	};
	uint32_t uGroupId; 	// group id

	virtual void marshal(sox::Pack &p) const
	{
		p << uGroupId;
	}

	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> uGroupId;
	}
};

}
}

#endif
