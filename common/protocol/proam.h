#ifndef PROAM_H_
#define PROAM_H_
#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"

namespace protocol {
namespace roaming {


struct PSyncRoamingData: public sox::Marshallable
{
	enum {uri = (1 << 8 | protocol::ROAM_SVID)};
	uint32_t uid;
	uint32_t dataversion;
	PSyncRoamingData(){}
	PSyncRoamingData(uint32_t id, uint32_t ver) : uid(id), dataversion(ver)
	{
	}
	virtual void marshal(sox::Pack &p) const {
		p << uid << dataversion;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> uid >> dataversion;
	}
};

//dataversion=0: no data in database.
//dataversion>0: the latest version in DB. mydata may be empty if version is same as that in request.
struct PSyncRoamingDataRes: public sox::Marshallable
{
	enum {uri = (2 << 8 | protocol::ROAM_SVID)};
	uint32_t uid;
	uint32_t dataversion;
	std::string mydata;
	PSyncRoamingDataRes(){}
	PSyncRoamingDataRes(uint32_t id, uint32_t ver, std::string data) : uid(id), dataversion(ver), mydata(data)
	{
	}
	virtual void marshal(sox::Pack &p) const {
		p << uid << dataversion << mydata;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> uid >> dataversion >> mydata;
	}
};

//to be obsolete
struct PSetRoamingData: public sox::Marshallable
{
	enum {uri = (3 << 8 | protocol::ROAM_SVID)};
	uint32_t uid;
	uint32_t dataversion;
	std::string mydata;
	PSetRoamingData(){}
	PSetRoamingData(uint32_t id, uint32_t ver, std::string data) : uid(id), dataversion(ver), mydata(data)
	{
	}
	virtual void marshal(sox::Pack &p) const {
		p << uid << dataversion << mydata;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> uid >> dataversion >> mydata;
	}
};

//to be obsolete
struct PSetRoamingDataRes: public sox::Marshallable
{
	enum {uri = (4 << 8 | protocol::ROAM_SVID)};
	uint32_t uid;
	uint32_t dataversion;
	bool issetok;
	PSetRoamingDataRes(){}
	PSetRoamingDataRes(uint32_t id, uint32_t ver, bool ok) : uid(id), dataversion(ver), issetok(ok)
	{
	}
	virtual void marshal(sox::Pack &p) const {
		p << uid << dataversion << issetok;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> uid >> dataversion >> issetok;
	}
};

//
// protocolversion: high 16 bits: big version;
//                  low  16 bits: minor version;
// Mask: the reason to upload data to server:
//       High 2Bs: Extremely important events;
//                0x0100: Save when quit yy;
//       Middle 4Bs: Very important events;
//                0x01000000: To save the basic configuration changes;
//                0x00000100: To save the tab configuration changes;
//       Low 2Bs: Important events;
//                0x0200: Save the recent visit channels;
//                0x0100: Save the recent contacts;
//

struct PSetRoamingData2: public sox::Marshallable
{
	enum {uri = (5 << 8 | protocol::ROAM_SVID)};
	uint32_t protocolversion;
	uint32_t uid;
	uint32_t dataversion;
	uint64_t mask;
	std::string mydata;
	PSetRoamingData2(){}
	PSetRoamingData2(uint32_t protocolversion_, uint32_t uid_, uint32_t dataversion_, uint64_t mask_, std::string mydata_) : 
		protocolversion(protocolversion_), uid(uid_), dataversion(dataversion_), mask(mask_), mydata(mydata_)
	{
	}
	virtual void marshal(sox::Pack &p) const {
		p << protocolversion << uid << dataversion << mask << mydata;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> protocolversion >> uid >> dataversion >> mask >> mydata;
	}
};

//
// result: Save data action result.
//         0: Successfully saved.
//         1: Failed due to server busy, retry after 'delay' seconds;
//         2: Failed due to wrong input parameter;
//         3: Server failed, try again;
//         4: Input too long ,try to wait; 
//
struct PSetRoamingDataRes2: public sox::Marshallable
{
	enum {uri = (6 << 8 | protocol::ROAM_SVID)};
	uint32_t protocolversion;
	uint32_t uid;
	uint32_t dataversion;
	uint32_t result;
	uint32_t delay;
	PSetRoamingDataRes2(){}
	PSetRoamingDataRes2(uint32_t protocolversion_, uint32_t uid_, uint32_t dataversion_, uint32_t result_, uint32_t delay_) :
	       protocolversion(protocolversion_), uid(uid_), dataversion(dataversion_), result(result_), delay(delay_)
	{
	}
	virtual void marshal(sox::Pack &p) const {
		p << protocolversion << uid << dataversion << result << delay;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> protocolversion >> uid >> dataversion >> result >> delay;
	}
};

}//namespace roaming
}//namespace protocol


#endif /*PROAM_H_*/


