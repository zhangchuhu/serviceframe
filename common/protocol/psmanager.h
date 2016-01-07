#ifndef PSMANAGER_H_
#define PSMANAGER_H_
#include "common/core/base_svid.h"
#include "common/protocol/psessionbase.h"
#include "common/protocol/pslist.h"
#include "common/protocol/const.h"
#include <vector>
#include <iterator>

namespace protocol {
namespace session {
	enum SMANAGER_URI {	// (1 ~ 100)
		PSessionLbsURI      = (1 << 8 | protocol::SMANAGER_SVID),
		PSessionLbsMovedURI = (3 << 8 | protocol::SMANAGER_SVID),
		PSessionLbsRes2URI  = (10 << 8 | protocol::SMANAGER_SVID),
		PSessionLbs2URI     = (11 << 8 | protocol::SMANAGER_SVID),
		PSessionLbsRes3URI  = (12 << 8 | protocol::SMANAGER_SVID),
		PSessionLbsResChannelFrozenURI  = (13 << 8 | protocol::SMANAGER_SVID),
		PSManagerQueryURI		= ( 14 << 8 | protocol::SMANAGER_SVID ),
		PSManagerReplyURI		= ( 15 << 8 | protocol::SMANAGER_SVID ),
		PSessionLbsRes4URI	= ( 16 << 8 | protocol::SMANAGER_SVID ),
    PSessionLbsUdpURI = ( 20 << 8 | protocol::SMANAGER_SVID) ,
	};

//uri 0 - 31

struct PCreateChanel : public sox::Marshallable {
	enum {uri = 1 << 8 | protocol::SDB_SVID};

	PCreateChanel() {
	}
	PCreateChanel(const std::string &n, bool l, bool p, const std::string &h,
		ISPType sp = CTL, const std::string &pwd = "", uint32_t parent = 0, VoiceQC q = quality) :
	name(n), bLimit(l), bPub(p), hello(h), pid(parent), password(pwd), isp(sp), qc(q) {
	}

	std::string name;
	bool bLimit;
	bool bPub;
	std::string hello;
	uint32_t pid; // parent channel id
	uint32_t tid; // topmost channel id
	std::string password;
	ISPType isp;
  VoiceQC qc;

	virtual void marshal(sox::Pack &p) const {
		p << name << bLimit << bPub << hello << pid << tid << password;
		p.push_uint16(qc);
		p.push_uint16(isp);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p>> name >> bLimit >> bPub >> hello >> pid >> tid >> password;
		qc = (VoiceQC)p.pop_uint16();
		isp = (ISPType)p.pop_uint16();
	}

};

// 在PCreateChanel基础上增加
/*struct PCreateChanel2 : public sox::Marshallable {
	enum {uri = 2 << 8 | protocol::SDB_SVID};

	PCreateChanel2() {
	}
	PCreateChanel2(const std::string &n, bool l, bool p, const std::string &h
			, ISPType sp, const std::string &pwd, uint32_t parent, VoiceQC q
			, int nType, const std::string& sTypeStr) 
		: name(n), bLimit(l), bPub(p), hello(h), pid(parent), password(pwd)
		, qc(q), isp(sp), type(nType), typeStr(sTypeStr)
	{}

	std::string name;
	bool		bLimit;
	bool		bPub;
	std::string hello;
	uint32_t	pid; // parent channel id
	uint32_t	tid; // topmost channel id
	std::string password;
	VoiceQC		qc;
	ISPType		isp;
	uint32_t	type;
	std::string typeStr;

	virtual void marshal(sox::Pack &p) const {
		p << name << bLimit << bPub << hello << pid << tid << password;
		p.push_uint16(qc);
		p.push_uint16(isp);
		p << type << typeStr;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p>> name >> bLimit >> bPub >> hello >> pid >> tid >> password;
		qc = (VoiceQC)p.pop_uint16();
		isp = (ISPType)p.pop_uint16();
		p >> type >> typeStr;
	}

};*/


struct PCreateChanelRes : public protocol::slist::PAddSessRes {
	enum {uri = 1 << 8 | protocol::SDB_SVID};
};


struct PCreateChanelOverFlow : public sox::Marshallable {
	enum {uri = (1 << 8 | protocol::SDB_SVID)};
	uint32_t max;
	virtual void marshal(sox::Pack &p) const {
		p << max;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> max;
	}
};
//res is response slist
/*
 struct PCreateChanelRes:  public sox::Marshallable {
 enum {uri = 1 << 8 | protocol::SDB_SVID};
 uint32_t sid;

 virtual void marshal(sox::Pack &p) const {
 p << sid;
 }
 virtual void unmarshal(const sox::Unpack &p) {
 p>> sid;
 }
 };*/

struct PDismissChanel : public PSessionSid {
	enum {uri = (4 << 8 | protocol::SDB_SVID)};
};

struct PDismissChanelRes : public sox::Voidmable {
	enum {uri = (4 << 8 | protocol::SDB_SVID)};
};

struct PSessionLbs : public PSessionSid {
  enum {uri = PSessionLbsURI };
  ISPType ispType;
  uint32_t protoVer;
  bool    isZip;
  bool    isSpSeperate;
  virtual void marshal(sox::Pack &p) const {
    p << sid;
    p.push_uint16(ispType);
    p << protoVer;
    p << isZip;
    p << isSpSeperate;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> sid;
    ispType = (ISPType)up.pop_uint16();
    if(!up.empty())
    {
      up >> protoVer;
    }
    else
    {
      protoVer = 0;
    }
    if(!up.empty())
    {
      up >> isZip >> isSpSeperate;
    }
    else
    {
      isZip = false;
      isSpSeperate = false;
    }
  }
};

struct PSessionLbsRes: public sox::Marshallable{
	enum { uri = (1 << 8) | protocol::SESSION_SVID };
	uint32_t sid;
	uint32_t asid;
	std::string ip;
	std::vector<uint16_t> ports;
	void marshal(sox::Pack &pk) const{
		pk << sid << asid << ip;
		sox::marshal_container(pk, ports);
	}

	void unmarshal(const sox::Unpack &up){
		up >> sid >> asid >> ip;
		sox::unmarshal_container(up, std::back_inserter(ports));
	}
};

struct PSessionLbsMoved: public sox::Marshallable{
	// enum {uri = (3 << 8 | protocol::SMANAGER_SVID)};
	enum {uri = PSessionLbsMovedURI};
	uint32_t aSid;
	uint32_t realSid;
	std::string name;
	virtual void marshal(sox::Pack &p) const {
		p << aSid << realSid << name;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> aSid >> realSid >> name;
	}
};


struct PJoinSession1Res: public sox::Marshallable{
	enum {uri = (1<< 8 | protocol::SESSION_SVID)};
	uint32_t realSid;
	uint32_t asid;
	std::string ip;
	std::vector<uint16_t> ports;
	virtual void marshal(sox::Pack &p) const {
		p << realSid << asid << ip;
		sox::marshal_container(p, ports);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p  >> realSid >> asid >> ip;
		sox::unmarshal_container(p, std::back_inserter(ports));
	}	
};

// enum {uri = (4 << 8 | protocol::SMANAGER_SVID)}; 曾经被分配给指令 PGetNews 和 POnGetNews

/*
struct PSessionSyncInfoReq : public PSessionSyncInfo {
//	enum {uri = (2 << 8 | protocol::SMANAGER_SVID)};
};

struct PSessionSyncInfoRes : public protocol::session::PSessionSyncInfo {
//	enum {uri = (3 << 8 | protocol::SMANAGER_SVID)};
};
*/

enum FindType {
	ById,
	ByName,
	ByLocate,
	ByType,
};

struct PFindChannel : public sox::Marshallable {
	enum {uri = (6 << 8 | protocol::SDB_SVID)};
	uint32_t sid;
	uint16_t ctx_id;
	std::string name;
	uint32_t area, province, city;
	FindType type;

	virtual void marshal(sox::Pack &p) const {
		p << sid << ctx_id << name << area << province << city;
		p.push_uint8(type);
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> sid >> ctx_id >> name >> area >> province >> city;
		type = (FindType)p.pop_uint8();
	}
};

struct FindItem : public sox::Marshallable {
	uint32_t sid;
	std::string name;
	uint32_t owner;
	/*uint32_t city;
	 uint32_t area;
	 uint32_t province;*/
	bool bPub;
	bool bLimit;

	virtual void marshal(sox::Pack &p) const {
		p << sid << name << owner /*<< area << province << city*/<< bPub
				<< bLimit;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> sid >> name >> owner /*>> area >> province >> city */>> bPub
				>> bLimit;
	}
};

struct PFindResult : public sox::Marshallable {
	enum {uri = (6 << 8 | protocol::SDB_SVID)};
	uint16_t findCtxId;
	uint16_t resCode;
	std::vector<FindItem> items;
	virtual void marshal(sox::Pack &p) const {
		p << findCtxId << resCode;
		sox::marshal_container(p, items);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> findCtxId >> resCode;
		sox::unmarshal_container(p, std::back_inserter(items));
	}

};

struct PSessDataInfo : public sox::Marshallable {
	enum {uri = (7 << 8 | protocol::SDB_SVID)};
	uint32_t sid;
	std::string dip,wip;
	uint16_t port;
	virtual void marshal(sox::Pack &p) const {
		p << sid << dip << wip << port;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> sid >> dip >> wip >> port;
	}
};

struct PSessionLbsRes2 : public sox::Marshallable
{
	// enum {uri = (10 << 8 | protocol::SMANAGER_SVID)};
	enum {uri = PSessionLbsRes2URI };
	uint32_t sid;
	uint32_t asid;
	std::string sessionResPack;
	virtual void marshal(sox::Pack &p) const {
		p << sid << asid << sessionResPack;

	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> sid >> asid >> sessionResPack;
	}

};

struct PSessionLbsRes3 : public PSessionLbsRes2 {
	// enum {uri = (12 << 8 | protocol::SMANAGER_SVID)};
	enum {uri = PSessionLbsRes3URI};
	uint32_t resCode;
	virtual void marshal(sox::Pack &p) const {
		PSessionLbsRes2::marshal(p);
		p << resCode;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		PSessionLbsRes2::unmarshal(p);
		p >> resCode;
	}
};

struct PSessionLbsRes4 : public sox::Marshallable {
	enum { uri = PSessionLbsRes4URI };
	uint32_t	code;
	uint32_t	sid;
	uint32_t	asid;
	std::map<std::string, std::string>	response;
	PSessionLbsRes4()
		: code(0)
		, sid(0)
		, asid(0)
	{}
	virtual void marshal(sox::Pack &p) const {
		p << code << sid << asid;
		marshal_container( p, response );
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> code >> sid >> asid;
		unmarshal_container( p, std::inserter(response, response.end()) );
	}
};

struct PSessionLbsUdp : public sox::Marshallable {
  enum {uri = PSessionLbsUdpURI };
  uint32_t sid;
  ISPType type;
  uint32_t protoVer;
  uint32_t uid;
  bool     isSpSeperate;
  virtual void marshal(sox::Pack &p) const {
    p << sid;
    p.push_uint16(type);
    p << protoVer;
    p << uid;
    p << isSpSeperate;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> sid;
    type = (ISPType)up.pop_uint16();
    if(!up.empty())
    {
      up >> protoVer;
    }
    else
    {
      protoVer = 0;
    }
    if(!up.empty())
    {
      up >> uid;
    }
    if(!up.empty())
    {
      up >> isSpSeperate;
    }
    else
    {
      isSpSeperate = false;
    }
  }
};


struct PSessionLbsResChannelFrozen : public sox::Marshallable
{
	// enum {uri = (13 << 8 | protocol::SMANAGER_SVID)};
	enum {uri = PSessionLbsResChannelFrozenURI};
	
    PSessionLbsResChannelFrozen()
			: sid(0)
			, asid(0)
			, unfrozenTime(0)
    {}
	
	uint32_t sid;
	uint32_t asid;
	uint32_t unfrozenTime;  //todo@: use string here, or other type?
	virtual void marshal(sox::Pack &p) const {
		p << sid << asid << unfrozenTime;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> sid >> asid >> unfrozenTime;
	}
};

struct PSessionLbs2 : public PSessionSid {
	// enum {uri = (11 << 8 | protocol::SMANAGER_SVID)};
	enum {uri = PSessionLbs2URI };
};

struct PSManagerQuery : public sox::Marshallable {
	enum { uri = PSManagerQueryURI };
	uint32_t		tid;
	std::string key;
	PSManagerQuery()
		: tid(0)
		, key()
	{}

	virtual void marshal(sox::Pack &p) const {
		p << tid << key;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> tid >> key;
	}
};

struct PSManagerReply : public sox::Marshallable {
	enum { uri = PSManagerReplyURI };
	uint32_t tid;
	uint32_t code;
	std::string response;
	PSManagerReply()
		: tid(0)
		, code(0)
		, response()
	{}

	virtual void marshal(sox::Pack &p) const {
		p << tid << code << response;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> tid >> code >> response;
	}	
};



}
}
#endif /*PSMANAGER_H_*/
