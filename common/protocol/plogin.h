#ifndef PLOGIN_PROTOCOL_H_
#define PLOGIN_PROTOCOL_H_
#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/const.h"
#include "common/protocol/zip_tpl.h"
#include "common/protocol/LinkdTransmitSubSvid.h"
#include "protocol/CodeInfo.h"
#include <vector>
#include <iterator>
#include <string>
#include "common/res_code.h"
#include "core/sox/sockethelper.h"

namespace protocol {
namespace login {

enum
{
  AUTH_CHECK_NONE = 0,
  AUTH_CHECK_COOKIE = 1,
  AUTH_CHECK_LINKD_CREDIT = 2,
  AUTH_CHECK_TICKET_EXTENSION = 3,
};

enum
{
  RES_OK = 0,  //成功
  TIME_OUT    = 1,  //超时
  FAILED      = 2,  //校验失败
  NOT_SUPP    = 3,   //不支持UDP linkd版本
  CREDIT_FAILED = 4  //票据验证失败
};

enum _LOGIN_URI
{
    PReqProbeIpsURI = (3 << 16 | (LBSSMGR_LINKDTRANSMIT_SUBSVID << 8) | LINKD_TRANSMIT_SVID),
    PReqProbeIpsResURI = (4 << 16 | (LBSSMGR_LINKDTRANSMIT_SUBSVID << 8) | LINKD_TRANSMIT_SVID),
    PCheckCacheDnsLbs_URI = (5 << 16 | (LBSSMGR_LINKDTRANSMIT_SUBSVID << 8) | LINKD_TRANSMIT_SVID),
    PCheckCacheDnsLbsRes_URI = (6 << 16 | (LBSSMGR_LINKDTRANSMIT_SUBSVID << 8) | LINKD_TRANSMIT_SVID),    
    PProbeSeverReqURI = (206 << 8 | protocol::LINKD_SVID),
    PProbeSeverResURI = (207 << 8 | protocol::LINKD_SVID),
    PLinkdWithProbeReqURI = (31 << 8 | protocol::LBS_SVID),
    PLinkdWithProbeResURI = (32 << 8 | protocol::LBS_SVID)
};

struct PLinkD : public sox::Marshallable {
	enum {uri = protocol::LBS_SVID};
	uint32_t version;
	ISPType type;
	virtual void marshal(sox::Pack &pk) const {
		pk << version;
		pk.push_uint16(type);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		if (up.empty()) {
			version = 0;
			type = CNC;
		} else {
			up >> version;
			type = (ISPType)up.pop_uint16();
		}
	}
};

struct PLinkDRes : public sox::Marshallable {
	enum {uri = protocol::LBS_SVID};
	uint32_t uid;
	std::string ip;
	std::vector<uint16_t> ports;
	PLinkDRes() {
	}
	
	virtual void marshal(sox::Pack &pk) const {
		pk << uid <<ip;
		sox::marshal_container(pk, ports);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >>ip;
		sox::unmarshal_container(up, std::back_inserter(ports));
	}
};

struct Ploginlbs: public sox::Marshallable {
	enum {uri = (2 << 8) | protocol::LBS_SVID};

	Ploginlbs(){}
	Ploginlbs(uint32_t ver, std::string ud, std::string passwd, std::string cook, uint32_t id, ISPType ty, bool bmail)
		: version(ver), udb(ud), md5passwd(passwd), cookie(cook), uid(id), type(ty), bEmail(bmail) {}

	uint32_t version;
	std::string udb;
	std::string md5passwd;
	std::string cookie;
	uint32_t uid;
	ISPType type;
	bool bEmail;

	virtual void marshal(sox::Pack &pk) const {
		pk << version << udb << md5passwd << cookie << uid << bEmail;
		pk.push_uint16(type);
		//.push_uint16(bEmail);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> version >> udb >> md5passwd >> cookie >> uid >> bEmail;
		type = (ISPType)up.pop_uint16();
		//bEmail = up.pop_uint16();
	}
};

struct Ploginlbs20: public sox::Marshallable {
  enum {uri = (17 << 8) | protocol::LBS_SVID};

  Ploginlbs20()
  : authType(0), lcid(0)
  {}

  Ploginlbs20(uint32_t ver, std::string ud, std::string passwd, std::string cook, uint32_t id, ISPType ty, bool bmail
    , std::string& vStr, std::string& mac, uint32_t auType)
    : version(ver), udb(ud), md5passwd(passwd), cookie(cook), uid(id), type(ty), bEmail(bmail), verStr(vStr), macAddr(mac)
    , authType(auType)
  {}

  uint32_t version;
  std::string udb;
  std::string md5passwd;
  std::string cookie;
  uint32_t uid;
  ISPType type;	// 无意义字段
  bool bEmail;

  std::string verStr;
  std::string macAddr;
  uint32_t authType; // 客户端支持的验证类型：0-用户名密码验证；1-手机令牌

  uint32_t lcid;

  std::string antiCodeResult;

  virtual void marshal(sox::Pack &pk) const {
    pk << version << udb << md5passwd << cookie << uid << bEmail;
    pk.push_uint16(type);
    //.push_uint16(bEmail);
    pk << verStr << macAddr << authType << lcid << antiCodeResult;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> version >> udb >> md5passwd >> cookie >> uid >> bEmail;
    type = (ISPType)up.pop_uint16();
    //bEmail = up.pop_uint16();
    if(up.size() > 0)
    {
      up >> verStr >> macAddr;
    }
    if(up.size() > 0)
    {
      up >> authType;
    }
    if(up.size() > 0)
    {
      up >> lcid;
    }
    if(up.size() > 0)
    {
      up >> antiCodeResult;
    }
  }
  virtual std::ostream & trace(std::ostream & os) const
  {
    os << "p=" << udb << ":" << md5passwd.size() << ",";
    os << "uid=" << uid << ",";
    os << "ver=" << version << "+" << lcid << "=" << verStr << ",";
    os << "e=" << bEmail << ",";
    os << "a=" << authType << ",";
    os << "c#=" << cookie.size() << ",";
    os << "m#=" << macAddr.size() << ",";
    return os;  
  }  
};

struct PloginlbsRetry: public sox::Marshallable
{		
	enum {uri = (106 << 8) | protocol::LBS_SVID};

	PloginlbsRetry(){}

	uint32_t uType;
	uint32_t uSameAccountTimes;
	uint32_t uSameIpTimes;
	std::string strReserve;

	virtual void marshal(sox::Pack &pk) const {
		pk << uType << uSameAccountTimes << uSameIpTimes << strReserve;	
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uType >> uSameAccountTimes >> uSameIpTimes >> strReserve;
	}
};

// 登陆指令，附加平台信息
struct Ploginlbs30 : public sox::Marshallable { // <----------hinted by 振达，这个包用于手机YY的，以后才用得着。
  enum {uri = (105 << 8) | protocol::LBS_SVID};

  Ploginlbs30(): version(0),uid(0),clientIp(0),clientPort(0) {}

  Ploginlbs30(uint32_t ver, const std::string& ud, const std::string& passwd
    , const std::string& cook, uint32_t id
    , ISPType ty, bool bmail, const std::string& from)
    : version(ver), udb(ud), md5passwd(passwd), cookie(cook), uid(id), type(ty), bEmail(bmail)
    , m_from(from) , clientIp(0), clientPort(0)
  {}

  uint32_t version;
  std::string udb;
  std::string md5passwd;
  std::string cookie;
  uint32_t uid;
  ISPType type;	// 无意义字段
  bool bEmail;
  std::string m_from;	// 附加的平台信息，字符串长度<=10
  uint32_t clientIp;
  uint16_t clientPort;

  virtual void marshal(sox::Pack &pk) const {
    pk << version << udb << md5passwd << cookie << uid << bEmail;
    pk.push_uint16(type);
    pk << m_from;
    pk << clientIp << clientPort;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> version >> udb >> md5passwd >> cookie >> uid >> bEmail;
    type = (ISPType)up.pop_uint16();
    up >> m_from;
    if(!up.empty()){
      up >> clientIp;
    }
    if(!up.empty()){
      up >> clientPort;
    }
  }
};

struct Ploginlbs20_new: public Ploginlbs20
{
  enum {uri = (125 << 8) | protocol::LBS_SVID};
};

// yycookie key ver
struct Ploginlbs40 : public sox::Marshallable {
	enum {uri = (110 << 8) | protocol::LBS_SVID};

	uint32_t	version;
	std::string m_strUdb;
	std::string md5passwd;
	std::string m_strCookie;
	uint32_t	uid;
	ISPType		type;
	bool		bEmail;
	std::string m_from;	// 附加的平台信息，字符串长度<=10
	uint32_t	m_nCookieKeyVer;

	Ploginlbs40() : version(0), uid(0), type(AUTO_DETECT), m_nCookieKeyVer((uint32_t)-1) {}

	Ploginlbs40(uint32_t ver, const std::string& udb, const std::string& passwd
		, const std::string& cookie, uint32_t id, ISPType ty, bool bmail, const std::string& from, uint32_t nKeyVer)


		: version(ver), m_strUdb(udb), md5passwd(passwd), m_strCookie(cookie)
		, uid(id), type(ty), bEmail(bmail), m_from(from), m_nCookieKeyVer(nKeyVer)
	{}


	virtual void marshal(sox::Pack &pk) const {
		if (m_from.size() > 10)
			assert(false);

		pk << version << m_strUdb << md5passwd << m_strCookie << uid << bEmail;
		pk.push_uint16(type);
		pk << m_from << m_nCookieKeyVer;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> version >> m_strUdb >> md5passwd >> m_strCookie >> uid >> bEmail;
		type = (ISPType)up.pop_uint16();
		up >> m_from >> m_nCookieKeyVer;
    }
};

struct PUpdateCookie : public sox::Marshallable {
	enum {uri = (111 << 8) | protocol::LBS_SVID};

	std::string m_strPassport;
	std::string m_strPasswd;
	std::string m_strCookie;
	uint32_t m_nKeyVer;
	uint32_t m_nUid;
	
	PUpdateCookie() : m_nKeyVer(uint32_t(-1)), m_nUid(uint32_t(-1)) {}

/*
	// for uinfod
	PUpdateCookie(const PUpdateCookie& o) 
		: sox::Marshallable()
		, m_strPassport(o.m_strPassport)
		, m_strPasswd(o.m_strPasswd)
		, m_strCookie(o.m_strCookie)
		, m_nKeyVer(o.m_nKeyVer)
		, m_nUid(o.m_nUid)
	{}
*/

	virtual void marshal(sox::Pack &pk) const {
		pk << m_strPassport << m_strPasswd << m_strCookie << m_nKeyVer << m_nUid;
	}

	virtual void unmarshal(const sox::Unpack &up) {
		up >> m_strPassport >> m_strPasswd >> m_strCookie >> m_nKeyVer >> m_nUid;
	}
};

struct PUpdateCookieRes : public PUpdateCookie {
	enum {uri = (112 << 8) | protocol::LBS_SVID};
};

// 丢弃
struct PloginlbsRes: public sox::Marshallable {
	enum {uri = (2 << 8) | protocol::LBS_SVID};

	PloginlbsRes(){}
	PloginlbsRes(std::vector<uint16_t> ports, uint32_t serverip, std::string db, uint32_t u, std::string cook, uint32_t res, bool bm)
		: sports(ports), sip(serverip), udb(db), uid(u), cookie(cook), resCode(res), bEmail(bm) {}

	std::vector<uint16_t> sports;
	uint32_t sip;
	std::string udb;
	uint32_t uid;
	std::string cookie;
	uint32_t resCode;
	bool bEmail;

	virtual void marshal(sox::Pack &pk) const {
		sox::marshal_container(pk, sports);
		pk << sip << udb << uid << cookie << resCode << bEmail;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		sox::unmarshal_container(up, std::back_inserter(sports));
		up >> sip >> udb >> uid >> cookie >> resCode >> bEmail;
	}
};

struct PloginlbsIPRes: public sox::Marshallable {
	enum {uri = (104 << 8) | protocol::LBS_SVID};

	PloginlbsIPRes(){}
	PloginlbsIPRes(std::vector<uint16_t> ports, uint32_t serverip, std::string db, uint32_t u, std::string cook, uint32_t res, bool bm,ISPType ispt)
		: sports(ports), sip(serverip), udb(db), uid(u), cookie(cook), resCode(res), bEmail(bm), isptype(ispt) {}

	std::vector<uint16_t> sports;
	uint32_t sip;
	std::string udb;
	uint32_t uid;
	std::string cookie;
	uint32_t resCode;
	bool bEmail;
	ISPType isptype;

	virtual void marshal(sox::Pack &pk) const {
		sox::marshal_container(pk, sports);
		pk << sip << udb << uid << cookie << resCode << bEmail ;
		pk.push_uint16(isptype);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		sox::unmarshal_container(up, std::back_inserter(sports));
		up >> sip >> udb >> uid >> cookie >> resCode >> bEmail;
		isptype = (ISPType)up.pop_uint16();
	}
};

struct PloginlbsIPRes2: public sox::Marshallable {
  enum {uri = (114 << 8) | protocol::LBS_SVID};

  PloginlbsIPRes2() : version(0), extension(""), clientIp(0){}
  PloginlbsIPRes2(std::vector<uint16_t> ports, uint32_t serverip, std::string db, uint32_t u, std::string cook, uint32_t res, bool bm,ISPType ispt,AreaType area, uint32_t version, std::string extension)
    : sports(ports), sip(serverip), udb(db), uid(u), cookie(cook), resCode(res), bEmail(bm), isptype(ispt), areatype(area),version(version),extension(extension),clientIp(0) {}

  std::vector<uint16_t> sports;
  uint32_t sip;
  std::string udb;
  uint32_t uid;
  std::string cookie;
  uint32_t resCode;
  bool bEmail;
  ISPType isptype;
  AreaType areatype;
  uint32_t version;      //2表示使用票据
  std::string extension; //透传的身份票据
  uint32_t clientIp;     //客户端ip

  virtual void marshal(sox::Pack &pk) const {
    sox::marshal_container(pk, sports);
    pk << sip << udb << uid << cookie << resCode << bEmail ;
    pk.push_uint16(isptype);
    pk.push_uint16(areatype);
    pk << version << extension;
    pk << clientIp;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    sox::unmarshal_container(up, std::back_inserter(sports));
    up >> sip >> udb >> uid >> cookie >> resCode >> bEmail;
    isptype = (ISPType)up.pop_uint16();
    areatype = (AreaType)up.pop_uint16();
    if (!up.empty())
    {
      up >> version >> extension;
    }
    if (!up.empty())
    {
      up >> clientIp;
    }
  }
};

// 检查手机令牌动态密码
struct PCheckDKey: public sox::Marshallable
{
  enum {uri = (115 << 8) | protocol::LBS_SVID};

  uint32_t taskId;
  std::string udb;
  uint32_t uid;
  uint32_t dkey;

  virtual void marshal(sox::Pack &pk) const 
  {
    pk << taskId << udb << uid << dkey;
  }
  virtual void unmarshal(const sox::Unpack &up) 
  {
    up >> taskId >> udb >> uid >> dkey;
  }
};

struct PRequestDKey: public sox::Marshallable
{
  enum {uri = (116 << 8) | protocol::LBS_SVID};

  uint32_t taskId;
  std::string udb;
  uint32_t uid;
  uint32_t timestamp;

  virtual void marshal(sox::Pack &pk) const 
  {
    pk << taskId << udb << uid << timestamp;
  }
  virtual void unmarshal(const sox::Unpack &up) 
  {
    up >> taskId >> udb >> uid >> timestamp;
  }
};

struct PCheckDKeyFailure: public sox::Marshallable
{
  enum {uri = (117 << 8) | protocol::LBS_SVID};

  uint32_t taskId;
  uint32_t resCode;
  std::string udb;
  uint32_t uid;

  virtual void marshal(sox::Pack &pk) const 
  {
    pk << taskId << resCode << udb << uid;
  }
  virtual void unmarshal(const sox::Unpack &up) 
  {
    up >> taskId >> resCode >> udb >> uid;
  }
};

struct PLinkdInfo: public sox::Marshallable
{
  uint32_t ip;
  std::vector<uint16_t> ports;
  uint32_t reserver1;
  uint32_t reserver2;

  PLinkdInfo(): ip(0), reserver1(0), reserver2(0) {}

  virtual void marshal(sox::Pack &pk) const
  {
    pk << ip << reserver1 << reserver2;
    marshal_container(pk, ports);
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> ip >> reserver1 >> reserver2;
    unmarshal_container(up, std::back_inserter(ports));
  }
};

struct PloginlbsIPRes3: public sox::Marshallable
{
  enum {uri = (118 << 8) | protocol::LBS_SVID};

  PloginlbsIPRes3() : version(0), extension(""), clientIp(0), loginFailErrcode(0){}

  std::vector<PLinkdInfo> linkds;
  std::string udb;
  uint32_t uid;
  std::string cookie;
  uint32_t resCode;
  bool bEmail;
  ISPType isptype;
  AreaType areatype;
  uint32_t version;      //2表示使用票据
  std::string extension; //透传的身份票据
  uint32_t clientIp;     //客户端ip
  uint32_t loginFailErrcode;
  std::string loginFailMsg;
  uint32_t timeoutTryMax;

  virtual void marshal(sox::Pack &pk) const
  {
    sox::marshal_container(pk, linkds);
    pk << udb << uid << cookie << resCode << bEmail ;
    pk.push_uint16(isptype);
    pk.push_uint16(areatype);
    pk << version << extension;
    pk << clientIp << loginFailErrcode << loginFailMsg;
    pk << timeoutTryMax;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    sox::unmarshal_container(up, std::back_inserter(linkds));
    up >> udb >> uid >> cookie >> resCode >> bEmail;
    isptype = (ISPType)up.pop_uint16();
    areatype = (AreaType)up.pop_uint16();
    if (!up.empty())
    {
      up >> version >> extension;
    }
    if (!up.empty())
    {
      up >> clientIp;
    }
    if (!up.empty())
    {
      up >> loginFailErrcode >> loginFailMsg;
    }
    if (!up.empty())
    {
      up >> timeoutTryMax;
    }
  }
};

// yycookie key ver
struct Ploginlbs20Res: public sox::Marshallable {
	enum {uri = (113 << 8) | protocol::LBS_SVID};

	std::vector<uint16_t> sports;
	uint32_t sip;
	std::string udb;
	uint32_t uid;
	std::string cookie;
	uint32_t resCode;
	bool bEmail;
	ISPType isptype;
	uint32_t m_nCookieKeyVer;

	Ploginlbs20Res() : sip(0), uid(0), resCode(RES_SUCCESS), isptype(AUTO_DETECT), m_nCookieKeyVer(uint32_t(-1)) {}
	explicit Ploginlbs20Res(uint32_t nResCode) : resCode(nResCode) {}
	Ploginlbs20Res(std::vector<uint16_t> ports, uint32_t serverip, std::string db, uint32_t u
		, std::string cook, uint32_t res, bool bm,ISPType ispt, uint32_t nKeyVer)

		: sports(ports), sip(serverip), udb(db), uid(u), cookie(cook), resCode(res), bEmail(bm)
		, isptype(ispt), m_nCookieKeyVer(nKeyVer)
	{}

	

	virtual void marshal(sox::Pack &pk) const {
		sox::marshal_container(pk, sports);
		pk << sip << udb << uid << cookie << resCode << bEmail ;
		pk.push_uint16(isptype);
		pk << m_nCookieKeyVer;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		sox::unmarshal_container(up, std::back_inserter(sports));
		up >> sip >> udb >> uid >> cookie >> resCode >> bEmail;
		isptype = (ISPType)up.pop_uint16();
		up >> m_nCookieKeyVer;
	}
};

struct PGetImLinkdInfo: public sox::Voidmable {
	enum {uri = (18 << 8) | protocol::IMLBS_SVID};

	uint32_t uid;
	virtual void marshal(sox::Pack &pk) const {
		pk.push_uint32(uid);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		uid = up.pop_uint32();
	}

};

struct PGetImLinkdInfoRes: public sox::Marshallable {
	enum {uri = (18 << 8) | protocol::IMLBS_SVID};

	std::vector<uint16_t> ports;
	std::string ip;

	virtual void marshal(sox::Pack &pk) const {
		sox::marshal_container(pk, ports);
		pk << ip;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		sox::unmarshal_container(up, std::back_inserter(ports));
		up >> ip;
	}
};

struct PNewUser: public sox::Marshallable{
	enum {uri = (3 << 8) | protocol::UDB_SVID};
	std::string udb;
	std::string sha1Passwd;
	uint32_t ip;
	bool bEmail;
	virtual void marshal(sox::Pack &pk) const {
		pk << udb << sha1Passwd << ip << bEmail;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> udb >> sha1Passwd >> ip >> bEmail;
	}
};


struct PNewUserCtx: public sox::Marshallable{
	enum {uri = (4 << 8) | protocol::UDB_SVID};
	std::string udb;
	std::string sha1Passwd;
	uint32_t ip;
	bool bEmail;
	uint32_t taskId;

	PNewUserCtx() : ip(0), bEmail(false), taskId(0) {}

	virtual void marshal(sox::Pack &pk) const {
		pk << udb << sha1Passwd << ip << bEmail << taskId;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> udb >> sha1Passwd >> ip >> bEmail >> taskId;
	}
};

/*
struct PLbsUdbRes: public sox::Marshallable{
	enum {uri = (4 << 8) | protocol::LBS_SVID};
	std::string udb;
	uint16_t resCode;
	uint32_t serverId;
	virtual void marshal(sox::Pack &pk) const {
		pk << udb << resCode << serverId;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> udb >> resCode >> serverId;
	}
};*/

struct PProcLogin: public sox::Marshallable{
	enum {uri = (5 << 8) | protocol::LBS_SVID};
	PProcLogin(){}
	PProcLogin(std::string u, std::string p, uint32_t pip, bool mail)
		: udb(u), passwd(p), ip(pip), bEmail(mail) {}

	std::string udb;
	std::string passwd;
	uint32_t ip;
	bool bEmail;

	virtual void marshal(sox::Pack &pk) const {
		pk << udb << passwd << ip << bEmail;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> udb >> passwd >> ip >> bEmail;
	}
};

struct PProcLogin_Res: public sox::Marshallable{
	enum {uri = (5 << 8) | protocol::LBS_SVID};
	PProcLogin_Res(){}
	PProcLogin_Res(uint32_t res, std::string db, bool bm)
		: resCode(res), udb(db), bEmail(bm) {}
	PProcLogin_Res(uint32_t res, uint32_t sid, std::string pass, uint32_t id, uint32_t pip, std::string db, bool bm, bool reCal)
		: resCode(res), serverId(sid), passwd(pass), userId(id), ip(pip), udb(db), bEmail(bm), reCalCookie(reCal) {}

	uint32_t resCode;
	uint32_t serverId;
	std::string passwd;
	uint32_t userId;
	uint32_t ip;
	std::string udb;
	bool bEmail;
	bool reCalCookie;

	virtual void marshal(sox::Pack &pk) const {
		pk << resCode << serverId << passwd << userId << ip << udb << bEmail << reCalCookie;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> resCode >> serverId >> passwd >> userId >> ip >> udb >> bEmail >> reCalCookie;
	}
};

struct PProcLogin2: public sox::Marshallable{
	enum {uri = (6 << 8) | protocol::LBS_SVID};
	PProcLogin2(){}
	PProcLogin2(std::string u, std::string p, uint32_t pip, bool mail, uint32_t t)
		: udb(u), passwd(p), ip(pip), bEmail(mail), taskId(t){}

		std::string udb;
		std::string passwd;
		uint32_t ip;
		bool bEmail;
		uint32_t taskId;

		virtual void marshal(sox::Pack &pk) const {
			pk << udb << passwd << ip << bEmail << taskId;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> udb >> passwd >> ip >> bEmail >> taskId;
		}
};

struct PProcLogin_Res2: public sox::Marshallable{
	enum {uri = (6 << 8) | protocol::LBS_SVID};

	PProcLogin_Res2(){}
	PProcLogin_Res2(uint32_t res, const std::string &pass, uint32_t id, uint32_t tid): 
									resCode(res), passwd(pass), userId(id), taskId(tid)  {}
	uint32_t resCode;
	std::string passwd;
	uint32_t userId;
	uint32_t taskId;

	virtual void marshal(sox::Pack &pk) const {
		pk << resCode <<  passwd << userId << taskId;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> resCode >>  passwd >> userId >> taskId;
	}
};


/*
struct PLocateLinkd: public sox::Marshallable{
	enum {uri = (6 << 8) | protocol::ULOCATE_SVID};
	PLocateLinkd(){}
	PLocateLinkd(uint32_t uid, std::string pass, uint32_t pip, std::string db, bool bm, bool reCal)
		: userId(uid), passwd(pass), ip(pip), udb(db), bmail(bm), reCalCookie(reCal) {}

	uint32_t userId;
	std::string passwd;
	uint32_t ip;
	std::string udb;
	bool bmail;
	bool reCalCookie;

	virtual void marshal(sox::Pack &pk) const {
		pk << userId << passwd << ip << udb << bmail << reCalCookie;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> userId >> passwd >> ip >> udb >> bmail >> reCalCookie;
	}
};
*/
/*
struct PLocateLinkdRes: public PLocateLinkd{
	enum {uri = (6 << 8) | protocol::ULOCATE_SVID};
	PLocateLinkd(uint32_t uid, std::string pass, uint32_t pip, std::string db, bool bm, uint32_t sid, uint16_t res)
	uint32_t serverId;
	uint16_t resCode;	

	virtual void marshal(sox::Pack &pk) const {
		PLocateLinkd::marshal(pk);
		pk << serverId << resCode;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		PLocateLinkd::unmarshal(up);
		up >> serverId >> resCode;
	}
};*/

struct PUserCreated: public PNewUser{
	enum {uri = (7 << 8) | protocol::LBS_SVID};
	uint32_t userId;
	uint16_t resCode;
	virtual void marshal(sox::Pack &pk) const {
		PNewUser::marshal(pk);
		pk << userId << resCode;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		PNewUser::unmarshal(up);
		up >> userId >> resCode;
	}
};

struct PUserCreatedCtx: public PNewUserCtx{
	enum {uri = (8 << 8) | protocol::LBS_SVID};
	uint32_t userId;
	uint16_t resCode;
	virtual void marshal(sox::Pack &pk) const {
		PNewUserCtx::marshal(pk);
		pk << userId << resCode;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		PNewUserCtx::unmarshal(up);
		up >> userId >> resCode;
	}
};



struct PExchangeKey : public sox::Marshallable {
	enum {uri = (17 << 8) | protocol::LINKD_SVID};
	std::string publicKey;
	std::string e;
	std::string bizName;
	virtual void marshal(sox::Pack &pk) const {
		pk << publicKey << e << bizName;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> publicKey >> e;
		if(!up.empty())
		{
			up >> bizName;
		}
	}
};

struct PExchangeKey2 : public PExchangeKey {
	enum {uri = (20 << 8) | protocol::LINKD_SVID};
	virtual void marshal(sox::Pack &pk) const {
		PExchangeKey::marshal(pk);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		PExchangeKey::unmarshal(up);
	}
};

struct PExchangeKeyRes : public sox::Marshallable {
	enum {uri = (21 << 8) | protocol::LINKD_SVID};
	std::string encSessionKey;

	virtual void marshal(sox::Pack &pk) const {
		pk << encSessionKey;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> encSessionKey;
	}
};

struct PExchangeKey_LINKRes : public sox::Marshallable {
	enum {uri = (18 << 8) | protocol::LINKD_SVID};
	std::string encSessionKey;
	std::string antiCode;

	virtual void marshal(sox::Pack &pk) const {
		pk << encSessionKey << antiCode;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> encSessionKey;
		if(!up.empty()){
			up >> antiCode;
		}
	}
};

struct PExchangeKey_LBSRes : public sox::Marshallable {
	enum {uri = (8 << 8) | protocol::LBS_SVID};
	std::string encSessionKey;
	std::string antiCode;

	virtual void marshal(sox::Pack &pk) const {
		pk << encSessionKey << antiCode;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> encSessionKey;
		if(!up.empty()){
			up >> antiCode;
		}
	}
};

struct PExchangeKey_IMLINKRes : public sox::Marshallable {
	enum {uri = (3 << 8) | protocol::IMLINKD_SVID};
	std::string encSessionKey;

	virtual void marshal(sox::Pack &pk) const {
		pk << encSessionKey;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> encSessionKey;
	}
};


struct PExchangeKey_SessRes : public sox::Marshallable {
	enum {uri = (8 << 8) | protocol::SESSION_SVID};
	std::string encSessionKey;

	virtual void marshal(sox::Pack &pk) const {
		pk << encSessionKey;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> encSessionKey;
	}
};


struct PLoginlinkd : public sox::Marshallable {
	enum {uri = (9 << 8) | protocol::LINKD_SVID};

	PLoginlinkd() {}
	PLoginlinkd(uint32_t u, const std::string &c) :	uid(u), cookie(c) {
	}

	uint32_t uid;
	std::string cookie;

	virtual void marshal(sox::Pack &pk) const {
		pk << uid << cookie;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> cookie;
	}
};


struct PLoginlinkd20 : public sox::Marshallable {
	enum {uri = (19 << 8) | protocol::LINKD_SVID};

	PLoginlinkd20() {}
	PLoginlinkd20(uint32_t u, const std::string &c, const std::string& passwd, const std::string& acc) :
		uid(u), cookie(c), sha_passwd(passwd), account(acc) {
	}

	uint32_t uid;
	std::string cookie;
	std::string sha_passwd;
	std::string account;

	virtual void marshal(sox::Pack &pk) const {
		pk << uid << cookie << sha_passwd << account;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> cookie >> sha_passwd >> account;
	}
};


struct PLoginlinkdRes : public sox::Marshallable {
  enum {uri = (9 << 8) | protocol::LINKD_SVID};

  PLoginlinkdRes(): udpFlag(0), requestLinkdFlag(0) {}
  explicit PLoginlinkdRes(uint32_t r) : res(r), udpFlag(0), requestLinkdFlag(0) {}

  uint32_t res;
  bool udpFlag;
  uint32_t serverId;
  uint32_t creditRescode;
  bool requestLinkdFlag;

  virtual void marshal(sox::Pack &pk) const {
    pk << res << udpFlag << serverId;
    pk << creditRescode;
    pk << requestLinkdFlag;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> res;
    if (!up.empty())
    {
      up >> udpFlag >> serverId;
    }
    if (!up.empty())
    {
      up >> creditRescode;
    }
    if (!up.empty())
    {
      up >> requestLinkdFlag;
    }
  }
};



struct PKickOff : public sox::Marshallable {
	enum {uri = (5 << 8)|protocol::LINKD_SVID};
	uint32_t who;
	virtual void marshal(sox::Pack &pk) const {
		pk << who;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> who;
	}
};

struct PLogout : public sox::Voidmable {
	enum {uri = (4 << 8) | protocol::LINKD_SVID};
};

struct PCheckVersion: public sox::Marshallable{
	enum {uri = (10 << 8 | protocol::LINKD_SVID)};
	uint32_t version;
	virtual void marshal(sox::Pack &pk) const {
		pk << version;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> version;
	}
};


struct PCheckVersionRes: public sox::Marshallable{
    enum {uri = (10 << 8 | protocol::LINKD_SVID)};
    ISPType isp;

    PCheckVersionRes()
    {
        isp = AUTO_DETECT;
    }

    virtual void marshal(sox::Pack &p) const {
        p.push_uint16(isp);
    }
    virtual void unmarshal(const sox::Unpack &up) {
        if (up.size() >=2)
        {
            isp = (ISPType)up.pop_uint16();
        }
    }
};

struct PServerShutDown: public sox::Marshallable{
	enum{uri = (11 << 8 | protocol::LINKD_SVID)};
	std::string word;
	virtual void marshal(sox::Pack &pk) const {
		pk << word;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> word;
	}
};

struct PPlus: public sox::Voidmable{
	enum{uri = (12 << 8 | protocol::LINKD_SVID)};
	uint32_t stampc;
	uint32_t stamps;
	virtual void marshal(sox::Pack &pk) const {
		pk << stampc << stamps;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> stampc >> stamps;
	}
};

struct PEchoLbsSession: public sox::Voidmable{
	enum{uri = (13 << 8 | protocol::LINKD_SVID)};
};

struct UserLossReportL : public sox::Marshallable {
	enum{uri = (13 << 8 | protocol::LINKD_SVID)};
	std::string uid;
	uint32_t sid,loss,total,sip;
	uint16_t sport;
	virtual void marshal(sox::Pack &pk) const {  
		pk << uid << sid << loss << total << sip << sport;
	}
	virtual void unmarshal(const sox::Unpack &up) { 
		up >> uid >> sid >> loss >> total >> sip >> sport;
	}
};


struct PServerChange : public sox::Marshallable {

		enum{uri = (14 << 8 | protocol::LINKD_SVID)};
		std::string balance_ip;
		virtual void marshal(sox::Pack &pk) const {  
				pk << balance_ip;
		}
		virtual void unmarshal(const sox::Unpack &up) { 
				up >> balance_ip;
		}
};

struct PHttpProxy: public sox::Marshallable{
	enum{uri = (15 << 8 | protocol::LINKD_SVID)};
	std::string query;
	uint32_t proxyId;
	virtual void marshal(sox::Pack &pk) const {  
		pk << query << proxyId;
	}
	virtual void unmarshal(const sox::Unpack &up) { 
		up >> query >> proxyId;
	}
};

struct PHttpProxyRes: public sox::Marshallable{
	enum{uri = (15 << 8 | protocol::LINKD_SVID)};
	std::string httpData;
	uint32_t proxyId;
	virtual void marshal(sox::Pack &pk) const {  
		pk << httpData << proxyId;
	}
	virtual void unmarshal(const sox::Unpack &up) { 
		up >> httpData >> proxyId;
	}
};

struct PHttpProxyEnd: public sox::Voidmable{
	enum{uri = (16 << 8 | protocol::LINKD_SVID)};
};

struct PClientHttpProxyEnd: public sox::Marshallable{
	enum{uri = (16 << 8 | protocol::LINKD_SVID)};
	uint32_t proxyId;
	virtual void marshal(sox::Pack &pk) const {  
		pk << proxyId;
	}
	virtual void unmarshal(const sox::Unpack &up) { 
		up >> proxyId;
	}
};

struct PGlobalBan2Client : public sox::Marshallable {
	enum {uri = (25 << 8)|protocol::LINKD_SVID};

	uint32_t id; // 数据库global_ban表的id字段
	virtual void marshal(sox::Pack &pk) const {  
		pk << id;
	}
	virtual void unmarshal(const sox::Unpack &up) { 
		up >> id;
	}
};

// 登陆时带pc_info，支持全局黑名单
struct PLoginlinkd30 : public sox::Marshallable {
	enum {uri = (27 << 8) | protocol::LINKD_SVID};

	PLoginlinkd30() :
	  uid(0), version(0), lcid(0){}
	PLoginlinkd30(uint32_t u, const std::string &c, 
		const std::string& passwd, const std::string& acc, const std::string& pcInfo) 
		: uid(u), cookie(c), sha_passwd(passwd), account(acc), pc_info(pcInfo) 
	{
	}

	uint32_t uid;
	std::string cookie;
	std::string sha_passwd;
	std::string account;
	std::string pc_info;
	std::string linkdCred;
	std::string macAddr;
	uint32_t version;
	uint32_t lcid;
	std::string verStr;
	std::string inst;

	virtual void marshal(sox::Pack &pk) const {
		pk << uid << cookie << sha_passwd << account << pc_info << linkdCred << macAddr;
		pk << version << lcid << verStr << inst;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> cookie >> sha_passwd >> account >> pc_info;
		if ( !up.empty() )
		{
			up >> linkdCred;
		}
		if ( !up.empty() )
		{
			up >> macAddr;
		}
		if ( !up.empty() )
		{
			up >> version >> lcid >> verStr;
		}
		if ( !up.empty() )
		{
			up >> inst;
		}
	}
};

// 给手机yy代理使用，带上客户端的真实ip
struct PLoginlinkd31 : public PLoginlinkd30 {
  enum {uri = (33 << 8) | protocol::LINKD_SVID};

  PLoginlinkd31() : ip(0) {}
  PLoginlinkd31(uint32_t u, const std::string &c, const std::string& passwd, 
                const std::string& acc, const std::string& pcInfo) 
    : PLoginlinkd30(u, c, passwd, acc, pcInfo)
    , ip(0)
  {
  }

  uint32_t ip;

  virtual void marshal(sox::Pack &pk) const {
    PLoginlinkd30::marshal(pk);
    pk << ip;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    PLoginlinkd30::unmarshal(up);
    up >> ip;
  }
};

struct PU2UTransmit : public sox::Marshallable
{
  enum {uri = (34 << 8) | protocol::LINKD_SVID};

  uint32_t fromUid;
  uint32_t seq;
  std::set<uint32_t> toUids;
  std::string load;

  virtual void marshal(sox::Pack &pk) const {
    pk << fromUid << seq;
    sox::marshal_container(pk, toUids);
    pk.push_varstr32(load.data(), load.length());
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> fromUid >> seq;
    sox::unmarshal_container(up, std::inserter(toUids, toUids.begin()));
    load = up.pop_varstr32();
  }
};

// 预留消息，暂时没用
struct PU2UTransmitAck : public sox::Marshallable
{
  enum {uri = (35 << 8) | protocol::LINKD_SVID};

  uint32_t fromUid;
  uint32_t seq;
  std::set<uint32_t> succ_uids;
  std::set<uint32_t> fail_uids;

  virtual void marshal(sox::Pack &pk) const {
    pk << fromUid << seq;
    sox::marshal_container(pk, succ_uids);
    sox::marshal_container(pk, fail_uids);
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> fromUid >> seq;
    sox::unmarshal_container(up, std::inserter(succ_uids, succ_uids.begin()));
    sox::unmarshal_container(up, std::inserter(fail_uids, fail_uids.begin()));
  }
};

struct PCheckCookieKeyVer : public sox::Marshallable {
	enum {uri = (270 << 8) | protocol::LINKD_SVID};

	uint32_t m_nKeyVer;
	PCheckCookieKeyVer() : m_nKeyVer(-1) {}
	explicit PCheckCookieKeyVer(uint32_t nKeyVer) { m_nKeyVer = nKeyVer; }

	virtual void marshal(sox::Pack &pk) const {
		pk << m_nKeyVer;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> m_nKeyVer;
	}
};

struct PCheckCookieKeyVerRes : public sox::Voidmable {
	enum {uri = (271 << 8) | protocol::LINKD_SVID};
};

struct PLoginlinkd40 : public sox::Marshallable {
	enum {uri = (272 << 8) | protocol::LINKD_SVID};

	uint32_t uid;
	std::string m_strCookie;
	std::string sha_passwd;
	std::string account;
	std::string pc_info;
	uint32_t m_nKeyVer;

	PLoginlinkd40() : m_nKeyVer(uint32_t(-1)) {}
	PLoginlinkd40(uint32_t u, const std::string& cookie, uint32_t nKeyVer, const std::string& passwd
		, const std::string& acc, const std::string& pcInfo) 

		: uid(u), m_strCookie(cookie), sha_passwd(passwd), account(acc), pc_info(pcInfo), m_nKeyVer(nKeyVer)
	{
	}


	virtual void marshal(sox::Pack &pk) const {
		pk << uid << m_strCookie << m_nKeyVer << sha_passwd << account << pc_info;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> m_strCookie >> m_nKeyVer >> sha_passwd >> account >> pc_info;
	}
};

struct PCheckIpList : public sox::Marshallable {
	enum {uri = (101 << 8) | protocol::IPLIST_SVID};
	std::string strMd5;
	virtual void marshal(sox::Pack &pk) const {
		pk << strMd5;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> strMd5;
	}
};

struct PCheckIpListRes : public sox::Marshallable {
	enum {uri = (102 << 8) | protocol::IPLIST_SVID};
	std::string strIpList;
	bool bNeedUpdate;
	virtual void marshal(sox::Pack &pk) const {
		pk << strIpList << bNeedUpdate;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> strIpList >> bNeedUpdate;
	}
};

typedef ZipMarshal<PCheckIpListRes, 146 << 8 | protocol::IPLIST_SVID> PCheckIpListZipRes;

//////////////////////////////////////////////////////////////////////////

struct PGlobalBanKick : public sox::Marshallable {
	enum {uri = (28 << 8)|protocol::LINKD_SVID};

	std::string note;	// 被封原因

	virtual void marshal(sox::Pack &pk) const {
		pk << note;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> note;
	}
};

struct PGlobalBanKick2 : public sox::Marshallable {
	enum {uri = (30 << 8)|protocol::LINKD_SVID};

	std::string banDate;	// 被封时间
	std::string unbanDate;	// 解封时间
	std::string note;		// 原因

	virtual void marshal(sox::Pack &pk) const {
		pk << banDate;
		pk << unbanDate;
		pk << note;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> banDate;
		up >> unbanDate;
		up >> note;
	}
};

struct PGlobalBanKick3 : public sox::Marshallable {
	enum {uri = (36 << 8)|protocol::LINKD_SVID};

	std::string banMsg;

	virtual void marshal(sox::Pack &pk) const {
		pk.push_varstr32(banMsg.data(), banMsg.size());
	}
	virtual void unmarshal(const sox::Unpack &up) {
		banMsg = up.pop_varstr32();
	}
};

struct PFreezeKick: public sox::Marshallable{
	enum {uri = (29 << 8)|protocol::LINKD_SVID};
	uint32_t freeTime;
	KickType kType; //用户被提出频道后true。登陆被冻结用户false

	virtual void marshal(sox::Pack &pk) const {
		pk << freeTime;
		pk.push_uint8(kType);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> freeTime;
		kType = up.pop_uint8() == KICKOUT_CHANNEL ? KICKOUT_CHANNEL : LOGIN_FREEZED;
	}
};

struct PGetWANIp: public sox::Voidmable{
	enum {uri = (31 << 8)|protocol::LINKD_SVID};
};

struct PGetWANIpRes: public sox::Marshallable{
	enum {uri = (32 << 8)|protocol::LINKD_SVID};
	PGetWANIpRes(){};
	uint32_t uid;
	uint32_t wanIp;
	uint32_t port;

	virtual void marshal(sox::Pack &pk) const {
		pk << uid;
		pk << wanIp;
		pk << port;
    }
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid;
		up >> wanIp;
		up >> port;
	}
};

struct PKickUser: public sox::Voidmable{
	enum {uri = (123 << 8)|protocol::LINKD_SVID};

};

struct PSendLocalIp: public sox::Marshallable{
	enum {uri = (124 << 8)|protocol::LINKD_SVID};
	PSendLocalIp(){};
	PSendLocalIp(uint32_t id,uint32_t ip):uid(id), localIp(ip){};
	uint32_t uid;
	uint32_t localIp;

	virtual void marshal(sox::Pack &pk) const {
		pk << uid;
		pk << localIp;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid;
		up >> localIp;
	}

};

struct PKickUsersList: public sox::Marshallable{
	enum {uri = (125 << 8)|protocol::LINKD_SVID};
	std::vector<uint32_t> kUsers;

	virtual void marshal(sox::Pack &pk) const {
		sox::marshal_container(pk, kUsers);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		sox::unmarshal_container(up, std::back_inserter(kUsers));
	}

};

struct PLoginedClients: public sox::Marshallable{
	enum {uri = (127 << 8)|protocol::LINKD_SVID};
	PLoginedClients(){};
	PLoginedClients(uint32_t id,uint32_t lc):uid(id), logined_clients(lc){};
	uint32_t uid;
	uint32_t logined_clients;

	virtual void marshal(sox::Pack &pk) const {
		pk << uid;
		pk << logined_clients;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid;
		up >> logined_clients;
	}
};


struct PCodeVersionInfo: public sox::Voidmable{
	enum {uri = (201 << 8)|protocol::LINKD_SVID};
};

struct PCodeVersionInfoRes: public sox::Marshallable{
	enum {uri = (127 << 8)|protocol::LINKD_SVID};
	std::vector<server::sendcode::CodeInfo> codeInfoSet;

	virtual void marshal(sox::Pack &pk) const {
		sox::marshal_container(pk, codeInfoSet);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		sox::unmarshal_container(up, std::back_inserter(codeInfoSet));
	}
};

struct PGetCode: public sox::Marshallable{
	enum {uri = (128 << 8)|protocol::LINKD_SVID};
	uint32_t uCodeID;

	virtual void marshal(sox::Pack &pk) const {
		pk << uCodeID;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uCodeID;
	}
};

struct PGetCodeRes: public sox::Marshallable{
	enum {uri = (129 << 8)|protocol::LINKD_SVID};
	uint32_t uCodeID;
	std::string data;

	virtual void marshal(sox::Pack &pk) const {
		pk << uCodeID << data;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uCodeID >> data;
	}
};

struct PSendPcinfo: public sox::Marshallable{
	enum {uri = (140 << 8)|protocol::LINKD_SVID};
	PSendPcinfo(){};
	PSendPcinfo(uint32_t id,std::string pi, std::string pinomac):uid(id), strPcinfo(pi), strPcinfoNoMac(pinomac){};
	uint32_t uid;
	std::string strPcinfo;
	std::string strPcinfoNoMac;

	virtual void marshal(sox::Pack &pk) const {
		pk << uid;
		pk << strPcinfo;
		pk << strPcinfoNoMac;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid;
		up >> strPcinfo;
		up >> strPcinfoNoMac;
	}
};

struct PLinkdPushData: public sox::Marshallable{
  enum {uri = (40 << 8)|protocol::LINKD_SVID};
  uint32_t fromServerId;
  uint32_t seq;
  bool     needAck;
  std::string load;

  virtual void marshal(sox::Pack &pk) const 
  {
    pk << fromServerId << seq << needAck;
    pk.push_varstr32(load.data(), load.length());
  }
  virtual void unmarshal(const sox::Unpack &up) 
  {
    up >> fromServerId >> seq >> needAck;
    load = up.pop_varstr32();
  }
};

struct PLinkdPushDataAck: public sox::Marshallable{
  enum {uri = (41 << 8)|protocol::LINKD_SVID};
  uint32_t fromServerId;
  uint32_t seq;
  uint32_t uid;

  virtual void marshal(sox::Pack &pk) const 
  {
    pk << fromServerId << seq << uid;
  }
  virtual void unmarshal(const sox::Unpack &up) 
  {
    up >> fromServerId >> seq >> uid;
  }
};

struct loginUserCtx : public sox::Marshallable
{
    uint32_t        ip;
    uint16_t        userType;
    uint16_t        status;
    uint32_t        time;
    std::string     passport;
    std::string     version;

    virtual void marshal(sox::Pack &p) const 
    {
        p << ip << userType << status << time << passport << version;

    }
    virtual void unmarshal(const sox::Unpack &up) 
    {
        up >> ip >> userType >> status >> time >> passport >> version;
    }
};

struct PUserLoginStatistic : public sox::Marshallable
{
    enum {uri = (450 << 8) | protocol::CATALOG_SVID};

    std::string                        srvIp;
    uint32_t                        serverId;
    std::vector<loginUserCtx>       userCtx;
    uint32_t                        timeStamp;

    virtual void marshal(sox::Pack &p) const 
    {
        p << srvIp << serverId;
        sox::marshal_container(p, userCtx);
        p << timeStamp;
    }
    virtual void unmarshal(const sox::Unpack &up) 
    {
        up >> srvIp >> serverId;
        sox::unmarshal_container(up, std::back_inserter(userCtx));
        up >> timeStamp;
    }
};

typedef ZipMarshal<PUserLoginStatistic, (451 << 8) | protocol::CATALOG_SVID>  PUserLoginStatisticZip;

struct LinkdLoginUserCtx : public sox::Marshallable
{
    enum
    {
      IP = 0,
      UID = 1,
      TYPE = 2,
      STATUS = 3,
      ERROR_CODE = 4,
    };
    
    std::map<uint32_t, uint32_t> loginInfo;
    std::string reserve;

    virtual void marshal(sox::Pack &p) const 
    {
        sox::marshal_container(p, loginInfo);
        p << reserve;

    }
    virtual void unmarshal(const sox::Unpack &up) 
    {
        sox::unmarshal_container(up, std::inserter(loginInfo, loginInfo.begin()));
        up >> reserve;
    }
};

struct PLinkdUserLoginStatistic2 : public sox::Marshallable
{
    enum {uri = (452 << 8) | protocol::CATALOG_SVID};

    std::string                          srvIp;
    uint32_t                             serverId;
    std::vector<LinkdLoginUserCtx>       userCtx;
    uint32_t                             timeStamp;

    virtual void marshal(sox::Pack &p) const 
    {
        p << srvIp << serverId;
        sox::marshal_container(p, userCtx);
        p << timeStamp;
    }
    virtual void unmarshal(const sox::Unpack &up) 
    {
        up >> srvIp >> serverId;
        sox::unmarshal_container(up, std::back_inserter(userCtx));
        up >> timeStamp;
    }
};

typedef ZipMarshal<PLinkdUserLoginStatistic2, (453 << 8) | protocol::CATALOG_SVID>  PLinkdUserLoginStatistic2Zip;

struct PLbsLiteStatistic : public sox::Marshallable
{
    enum {uri = (454 << 8) | protocol::CATALOG_SVID};
    // key: status ; value status count and status total time cost
    typedef std::pair<uint64_t, uint64_t> stat_item_t;
    typedef std::map<uint32_t, stat_item_t> stat_t;

    std::string srvIp;
    uint32_t serverId;
    uint32_t timeStamp;
    stat_t status2Stat;

    virtual void marshal(sox::Pack &p) const 
    {
        p << srvIp << serverId << timeStamp;
        sox::marshal_container(p, status2Stat);
    }
    virtual void unmarshal(const sox::Unpack &up) 
    {
        up >> srvIp >> serverId >> timeStamp;
        sox::unmarshal_container(up, std::inserter(status2Stat, status2Stat.begin()));
    }
};

struct PLbsLinkdStatistic : public sox::Marshallable
{
    enum {uri = (455 << 8) | protocol::CATALOG_SVID};
    
    std::string srvIp;
    uint32_t serverId;
    uint32_t timeStamp;
    // key: area (spec: 0=all) ; value : valid linkd counts
    std::map<uint32_t, uint32_t> area2ValidCnts;
    // key: linkd invalid type; value : invalid linkd counts
    std::map<uint32_t, uint32_t> status2InvalidCnts;

    virtual void marshal(sox::Pack &p) const 
    {
        p << srvIp << serverId << timeStamp;
        sox::marshal_container(p, area2ValidCnts);
        sox::marshal_container(p, status2InvalidCnts);
    }
    virtual void unmarshal(const sox::Unpack &up) 
    {
        up >> srvIp >> serverId >> timeStamp;
        sox::unmarshal_container(up, std::inserter(area2ValidCnts, area2ValidCnts.begin()));
        sox::unmarshal_container(up, std::inserter(status2InvalidCnts, status2InvalidCnts.begin()));
    }
};

struct PLinkdAppFlowItem : public sox::Marshallable
{
  enum FLOW_DIRECTION
  {
    UP_USER_TO_LINKD = 0,
    UP_LINKD_TO_APP,
    DOWN_APP_TO_LINKD,
    DOWN_LINKD_TO_USER,
  };
  
  typedef std::pair<uint32_t, uint32_t> flow_stat_item_t; // counts, packet total size
  typedef std::map<uint32_t, flow_stat_item_t> uri_to_flow_stat_t;
   
  uri_to_flow_stat_t uri2FlowStats;
  
  virtual void marshal(sox::Pack &p) const 
  {
      sox::marshal_container(p, uri2FlowStats);
  }
  virtual void unmarshal(const sox::Unpack &up) 
  {
      sox::unmarshal_container(up, std::inserter(uri2FlowStats, uri2FlowStats.begin()));
  }
};

struct PLinkdAppFlowStatistic : public sox::Marshallable
{
    enum {uri = (456 << 8) | protocol::CATALOG_SVID};
    
    // key : FLOW_DIRECTION ; value : PLinkdAppFlowItem pack
    mutable std::map<uint32_t, std::string> uriFlowStatsPack;
    std::map<uint32_t, PLinkdAppFlowItem> uriFlowStats; // not pack
    
    std::string srvIp;
    uint32_t serverId;
    uint32_t timeStamp;
    uint32_t groupId;

    virtual void marshal(sox::Pack &p) const 
    {
        // pack uriFlowStats to uriFlowStatsPack
        uriFlowStatsPack.clear();
        std::map<uint32_t, PLinkdAppFlowItem>::const_iterator it = uriFlowStats.begin();
        for(; it != uriFlowStats.end(); ++it)
        {
          sox::PacketToString(it->second, uriFlowStatsPack[it->first]);
        }    
        sox::marshal_container(p, uriFlowStatsPack);
        p << srvIp << serverId << timeStamp << groupId;
    }
    virtual void unmarshal(const sox::Unpack &up) 
    {
        sox::unmarshal_container(up, std::inserter(uriFlowStatsPack, uriFlowStatsPack.begin()));
        up >> srvIp >> serverId >> timeStamp >> groupId;
        // unpack uriFlowStatsPack to uriFlowStats
        uriFlowStats.clear();
        std::map<uint32_t, std::string>::iterator it = uriFlowStatsPack.begin();
        for(; it != uriFlowStatsPack.end(); ++it)
        {
          sox::StringToPacket(it->second, uriFlowStats[it->first]);
        }        
    }
};

struct PLogindStatsInfo : public sox::Marshallable
{
  uint32_t send;
  uint32_t recv;
  uint32_t drop;
  uint64_t resp;
  uint32_t lgDip;
  uint32_t lgWip;
  uint32_t lgGroupId;  

  PLogindStatsInfo():send(0),recv(0),drop(0),resp(0),lgDip(0),lgWip(0),lgGroupId(0){}
  
  virtual void marshal(sox::Pack &p) const 
  {
      p << send << recv << drop << resp;
      p << lgDip << lgWip << lgGroupId;
  }
  virtual void unmarshal(const sox::Unpack &up) 
  {
      up >> send >> recv >> drop >> resp;
      up >> lgDip >> lgWip >> lgGroupId;
  }
};

struct PLinkdLogindStatistic : public sox::Marshallable
{
    enum {uri = (457 << 8) | protocol::CATALOG_SVID};
    
    // key : logind serverid ; value : PLogindStatsInfo pack
    mutable std::map<uint32_t, std::string> svid2StatsPack;
    std::map<uint32_t, PLogindStatsInfo> svid2Stats; // not pack
    
    std::string srvIp;
    uint32_t serverId;
    uint32_t timeStamp;
    uint32_t groupId;

    virtual void marshal(sox::Pack &p) const 
    {
        // pack svid2Stats to svid2StatsPack
        svid2StatsPack.clear();
        std::map<uint32_t, PLogindStatsInfo>::const_iterator it = svid2Stats.begin();
        for(; it != svid2Stats.end(); ++it)
        {
          sox::PacketToString(it->second, svid2StatsPack[it->first]);
        }
        //
        sox::marshal_container(p, svid2StatsPack);
        p << srvIp << serverId << timeStamp << groupId;
    }
    virtual void unmarshal(const sox::Unpack &up) 
    {
        sox::unmarshal_container(up, std::inserter(svid2StatsPack, svid2StatsPack.begin()));
        up >> srvIp >> serverId >> timeStamp >> groupId;
        // unpack svid2StatsPack to svid2Stats
        svid2Stats.clear();
        std::map<uint32_t, std::string>::iterator it = svid2StatsPack.begin();
        for(; it != svid2StatsPack.end(); ++it)
        {
          sox::StringToPacket(it->second, svid2Stats[it->first]);
        }        
    }
};

struct PLinkdUserNumStatictis : public sox::Marshallable
{
  enum {uri = (460 << 8) | protocol::CATALOG_SVID};

  uint32_t loginedUsers;
  uint32_t masterLinkdOnlineUsers;
  uint32_t slaveLinkdOnlineUsers;
  uint32_t serverId;
  uint32_t timeStamp;
  std::string srvIp;
  virtual void marshal(sox::Pack &p) const 
  {
    p << loginedUsers << masterLinkdOnlineUsers << slaveLinkdOnlineUsers << serverId << timeStamp << srvIp;
  }
  virtual void unmarshal(const sox::Unpack &up) 
  {
    up >> loginedUsers >> masterLinkdOnlineUsers >> slaveLinkdOnlineUsers >> serverId >> timeStamp >> srvIp;
  }
};

struct PLinkdLoginItem : public sox::Marshallable2
{
  PLinkdLoginItem():count(0),timeCostTotal(0){}

  uint32_t count;
  uint64_t timeCostTotal;

  virtual void marshal2(sox::Pack &p) const 
  {
    p << count << timeCostTotal;
  }
  virtual void unmarshal2(const sox::Unpack &up) 
  {
    up >> count >> timeCostTotal;
  } 
};

struct PLinkdLoginBatchStat : public sox::Marshallable
{
    enum {uri = (458 << 8) | protocol::CATALOG_SVID};
    
    std::string srvIp;
    uint32_t serverId;
    uint32_t timeStamp;
    // key: status; value: pair of counts and time cost
    std::map<uint32_t, PLinkdLoginItem> status2Item;

    virtual void marshal(sox::Pack &p) const 
    {
        p << srvIp << serverId << timeStamp;
        sox::marshal_container(p, status2Item);
    }
    virtual void unmarshal(const sox::Unpack &up) 
    {
        up >> srvIp >> serverId >> timeStamp;
        sox::unmarshal_container(up, std::inserter(status2Item, status2Item.begin()));
    }
};
// reserve
typedef ZipMarshal<PLinkdLoginBatchStat, (459 << 8) | protocol::CATALOG_SVID>  PLinkdLoginBatchStatZip;

struct PLinkdOnlineStatus : public sox::Marshallable2
{
  PLinkdOnlineStatus():type(0),status(0),error(0){}
  PLinkdOnlineStatus(uint32_t t, uint32_t s, uint32_t e):type(t),status(s),error(e){}
  
  uint32_t type;
  uint32_t status;
  uint32_t error;

  virtual void marshal2(sox::Pack &p) const 
  {
    p << type << status << error;
  }
  virtual void unmarshal2(const sox::Unpack &up) 
  {
    up >> type >> status >> error;
  }
};

inline bool operator < (const PLinkdOnlineStatus & a, const PLinkdOnlineStatus & b)
{
  if(a.type == b.type)
    if(a.status == b.status)
      return a.error < b.error;
    else
      return a.status < b.status;
  else
    return a.type < b.type;
}

struct PLinkdOnlineBatchStat : public sox::Marshallable
{
    enum {uri = (461 << 8) | protocol::CATALOG_SVID};
    
    std::string srvIp;
    uint32_t serverId;
    uint32_t timeStamp;
    // key: status/type/error; value: counts
    std::map<PLinkdOnlineStatus, uint32_t> status2Counts;

    virtual void marshal(sox::Pack &p) const 
    {
        p << srvIp << serverId << timeStamp;
        sox::marshal_container(p, status2Counts);
    }
    virtual void unmarshal(const sox::Unpack &up) 
    {
        up >> srvIp >> serverId >> timeStamp;
        sox::unmarshal_container(up, std::inserter(status2Counts, status2Counts.begin()));
    }
};
// reserve
typedef ZipMarshal<PLinkdOnlineBatchStat, (462 << 8) | protocol::CATALOG_SVID>  PLinkdOnlineBatchStatZip;

struct PLoginLbs2 : public sox::Marshallable
{   
  enum {uri = (20 << 8 | protocol::LBS_SVID)};
  std::string context; // 交互上下文，保留
  uint32_t version; // 客户端版本号 201207280
  uint32_t uid; // uid，没获取到时填0
  std::string passport; // 用户名(有可能是中文)
  ISPType isptype;	// 无意义字段 -> 辅助纠错？
  uint32_t lcid; // 客户端版本号扩展 2052
  uint32_t lbsTrySeq; // 客户端尝试连lbs的次数
  uint32_t linkdTrySeq; // 客户端尝试连linkd的次数
  std::vector<uint32_t> goodLinkds;
  std::vector<uint32_t> poorLinkds;

  PLoginLbs2():version(0),uid(0),isptype(ISPType(0)),lcid(0),lbsTrySeq(0),linkdTrySeq(0){}

  virtual void marshal(sox::Pack &pk) const
  {
    pk << context << version << uid << passport;
    pk.push_uint16(isptype);
    pk << lcid << lbsTrySeq << linkdTrySeq;
    sox::marshal_container(pk, goodLinkds);
    sox::marshal_container(pk, poorLinkds);
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> context >> version >> uid >> passport;
    isptype = (ISPType)up.pop_uint16();
    if(!up.empty())
    {
      up >> lcid;
    }
    if(!up.empty())
    {
      up >> lbsTrySeq >> linkdTrySeq;
    }
    if(!up.empty())
    {
      sox::unmarshal_container(up, std::back_inserter(goodLinkds));
      sox::unmarshal_container(up, std::back_inserter(poorLinkds));
    }
  }
  virtual std::ostream & trace(std::ostream & os) const
  {
    os << "ctx#=" << context.size() << ",";
    os << "p=" << passport << ",";
    os << "uid=" << uid << ",";
    os << "ver=" << version << "+" << lcid << ",";
    os << "isp=" << isptype << ",";
    os << "try=" << lbsTrySeq << "+" << linkdTrySeq << ","; 
    os << "lk#=" << goodLinkds.size() << "+" << poorLinkds.size() << ",";    
    return os;  
  }  
};
    

struct PLinkdInfo2: public sox::Marshallable
{
  uint32_t ip;
  std::vector<uint16_t> ports;
  uint32_t serverId;
  std::string reserver2;

  PLinkdInfo2(): ip(0), serverId(0) {}

  virtual void marshal(sox::Pack &pk) const
  {
    pk << ip << serverId << reserver2;
    sox::marshal_container(pk, ports);
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> ip >> serverId >> reserver2;
    sox::unmarshal_container(up, std::back_inserter(ports));
  }

  bool operator < (const PLinkdInfo2 & b) const
  {
    if(ip == b.ip)
      return (serverId < b.serverId);
    else
      return (ip < b.ip);
  }  

  virtual std::ostream & trace(std::ostream & os) const
  {
    os << " " << ip2string(ip) << ":";
    std::copy(ports.begin(),ports.end(),std::ostream_iterator<uint16_t>(os, ":"));
    os << " " << serverId;
    return os;  
  }
};

struct PLoginLbs2Res : public sox::Marshallable
{   
  enum {uri = (21 << 8 | protocol::LBS_SVID)};
  
  PLoginLbs2Res(): resCode(RES_EDAEMON),
    isptype(AUTO_DETECT), areatype(AREA_UNKNOWN), clientIp(0),
    linkdExchkeyMode(0),lbsCharCode(0),reqLinkdProbeDelayTime(0){}
  
  std::string context; // 交互上下文，保留
  uint32_t resCode; // 返回码，RES_SUCCESS:成功，
  std::vector<PLinkdInfo2> linkds; // linkd列表
  std::string resInfo; // 返回信息，可能是失败详细信息/提示信息
  std::string reserve; // 返回扩展，可能是个内嵌消息包，保留
  
  ISPType isptype; // isp类型: 网通/电信 ...
  AreaType areatype; // 分区类型: 电信南区/电信东区...
  uint32_t clientIp;     //客户端ip	    

  uint32_t linkdExchkeyMode;
  uint32_t lbsCharCode;
  uint32_t reqLinkdProbeDelayTime;

  virtual void marshal(sox::Pack &pk) const
  {
    pk << context << resCode;
    sox::marshal_container(pk, linkds);
    pk << resInfo << reserve;
    pk.push_uint16(isptype);
    pk.push_uint16(areatype);
    pk << clientIp;
    pk << linkdExchkeyMode;
    pk << lbsCharCode;
    pk << reqLinkdProbeDelayTime;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> context >> resCode;
    sox::unmarshal_container(up, std::back_inserter(linkds));
    up >> resInfo >> reserve;
    isptype = (ISPType)up.pop_uint16();
    areatype = (AreaType)up.pop_uint16();
    up >> clientIp;
    if(!up.empty())
    {
      up >> linkdExchkeyMode;
    }
    if(!up.empty())
    {
      up >> lbsCharCode;
    }
    if (!up.empty())
    {
      up >> reqLinkdProbeDelayTime;
    }
  }
};

struct PLoginAuth : public sox::Marshallable
{   
  enum {uri = (200 << 8 | protocol::LINKD_SVID)};

  PLoginAuth():version(0),uid(0),authType(1),bEmail(false),lcid(0){}
  
  std::string context; // 交互上下文，保留
  uint32_t version; // 客户端版本号 417060 ?
  uint32_t uid; // uid，没获取到时填0
  
  std::string passport; // 用户名(有可能是中文)
  std::string md5passwd; // 密码
  std::string verStr; // ? 验证要用到
  
  uint32_t authType; // 客户端支持的验证类型：0-用户名密码验证；1-手机令牌
  bool bEmail; // 
  
  std::map<uint32_t, std::string> cookie; // 1:cookie/2:ticket/3:...
  
  std::string pc_info; // 统计，封ip用
  std::string macAddr; // 统计，封mac用 

  std::string antiCodeResult;
  uint32_t lcid; // 客户端版本号lcid
  std::string clientFrom;

  virtual void marshal(sox::Pack &pk) const
  {
    pk << context << version << uid << passport << md5passwd << verStr << authType;
    pk.push_uint8(bEmail);
    sox::marshal_container(pk, cookie);
    pk << pc_info << macAddr << antiCodeResult << lcid << clientFrom;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> context >> version >> uid >> passport >> md5passwd >> verStr >> authType;
    bEmail = up.pop_uint8();
    sox::unmarshal_container(up, std::inserter(cookie, cookie.begin()));
    up >> pc_info >> macAddr;
    if(!up.empty())
    {
      up >> antiCodeResult;
    }
    if(!up.empty())
    {
      up >> lcid;
    }
    if(!up.empty())
    {
      up >> clientFrom;
    }
  }
  virtual std::ostream & trace(std::ostream & os) const
  {
    os << "ctx=" << context << ",";
    os << "p=" << passport << ":" << md5passwd.size() << ",";
    os << "uid=" << uid << ",";
    os << "ver=" << version << "=" << verStr << "," << lcid << ",";
    os << "e=" << bEmail << ",";
    os << "a=" << authType << ",";
    os << "f=" << clientFrom << ",";
    for(std::map<uint32_t, std::string>::const_iterator it = cookie.begin(); it != cookie.end(); ++it)
    {
      os << "c[" << it->first << "]=" << it->second.size() << ",";
    }
    os << "p#=" << pc_info.size() << ",";
    os << "m#=" << macAddr.size() << ",";
    return os;  
  }
};

struct PLoginAuthTest: public PLoginAuth
{
  enum {uri = (204 << 8 | protocol::LINKD_SVID)};
};

struct PLoginAuthRes : public sox::Marshallable
{   
  enum {uri = (201 << 8 | protocol::LINKD_SVID)};
  
  PLoginAuthRes() : resCode(RES_NOCHANGED),uid(0),bEmail(false),cookieType(0),
    loginFailErrcode(0),serverTimeStamp(0),clientIp(0){}
  
  std::string context; // 交互上下文，保留
  uint32_t resCode; // 返回码，0:成功，
  uint32_t uid; // 
  std::string passport; // logind _uname
  std::string email; // logind _email
  bool bEmail; // 是否需要返回？
  
  std::string resInfo; // 返回信息，可能是失败详细信息/提示信息
  std::string reserve; // 返回扩展，可能是个内嵌消息包，保留
  
  uint32_t cookieType;      //2表示使用票据
  std::map<uint32_t, std::string> cookie; // 1:cookie/2:ticket/3:...

  uint32_t loginFailErrcode;
  std::string loginFailMsg;

  uint32_t serverTimeStamp;
  uint32_t clientIp;

  virtual void marshal(sox::Pack &pk) const
  {
    pk << context << resCode << uid << passport;
    pk.push_uint8(bEmail);
    pk << resInfo << reserve;
    pk << cookieType;
    sox::marshal_container(pk, cookie);
    pk << email;
    pk << loginFailErrcode << loginFailMsg;
    pk << serverTimeStamp;
    pk << clientIp;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> context >> resCode >> uid >> passport;
    bEmail = up.pop_uint8();
    up >> resInfo >> reserve;
    up >> cookieType;
    sox::unmarshal_container(up, std::inserter(cookie, cookie.begin()));
    if(!up.empty())
    {
      up >> email;
    }    
    if(!up.empty())
    {
      up >> loginFailErrcode >> loginFailMsg;
    }
    if(!up.empty())
    {
      up >> serverTimeStamp;
    }
    if(!up.empty())
    {
      up >> clientIp;
    }
  }
  
};  

struct PLoginAuthTestRes: public PLoginAuthRes
{
  enum {uri = (205 << 8 | protocol::LINKD_SVID)};
};

struct PMobileLoginAuth : public sox::Marshallable
{   
  enum {uri = (208 << 8 | protocol::LINKD_SVID)};

  PMobileLoginAuth() : uid(0), authType(0), bEmail(false), clientIp(0), clientPort(0){}
  
  std::string context; // 交互上下文，保留
  uint32_t uid; // uid，没获取到时填0
  
  std::string passport; // 用户名(有可能是中文)
  std::string md5passwd; // 密码
  std::string verStr; // ? 验证要用到
  
  uint32_t authType; // 客户端支持的验证类型：0-用户名密码验证；1-手机令牌
  bool bEmail; // 

  uint32_t clientIp;
  uint32_t clientPort;
  std::string clientFrom; // m_from in PLoginLbs30

  virtual void marshal(sox::Pack &pk) const
  {
    pk << context << uid << passport << md5passwd << verStr << authType;
    pk.push_uint8(bEmail);
    pk << clientIp << clientPort << clientFrom;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> context >> uid >> passport >> md5passwd >> verStr >> authType;
    bEmail = up.pop_uint8();
    up >> clientIp >> clientPort >> clientFrom;
  }
  virtual std::ostream & trace(std::ostream & os) const
  {
    os << "ctx=" << context << ",";
    os << "uid=" << uid << ",";
    os << "p=" << passport << ":" << md5passwd.size() << ",";
    os << "e=" << bEmail << ",";
    os << "a=" << authType << ",";
    os << "from=" << verStr << "," << clientFrom << "," << sox::addr_ntoa(clientIp) << ":" << clientPort << ",";
    return os;  
  }
  std::string dump() const
  {
    std::ostringstream os;
    os << *this;
    return os.str();
  }
};

struct PMobileLogin3rdAuth : public sox::Marshallable
{   
  enum {uri = (209 << 8 | protocol::LINKD_SVID)};

  PMobileLogin3rdAuth() : uid(0), clientIp(0), clientPort(0){}
  
  std::string context; // 交互上下文，保留
  uint32_t uid; // uid，没获取到时填0
  
  std::string userName; // 用户名
  std::string accountInfo; // 完整的第三方账号信息
  std::string token; // 相当于密码(cookie用)
  std::string verStr; // 验证要用到

  uint32_t clientIp;
  uint32_t clientPort;
  std::string clientFrom; // m_from in PLoginLbs30

  virtual void marshal(sox::Pack &pk) const
  {
    pk << context << uid << userName << accountInfo << token << verStr ;
    pk << clientIp << clientPort << clientFrom;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> context >> uid >> userName >> accountInfo >> token >> verStr ;
    up >> clientIp >> clientPort >> clientFrom;
  }
  virtual std::ostream & trace(std::ostream & os) const
  {
    os << "ctx=" << context << ",";
    os << "uid=" << uid << ",";
    os << "p=" << userName << ":" << accountInfo.size() <<  ":" << token.size() << ",";
    os << "from=" << verStr << "," << clientFrom << "," << sox::addr_ntoa(clientIp) << ":" << clientPort << ",";
    return os;  
  }
  std::string dump() const
  {
    std::ostringstream os;
    os << *this;
    return os.str();
  }
};


struct PMobileLogin3rdAuthRes : public PLoginAuthRes
{   
  enum {uri = (210 << 8 | protocol::LINKD_SVID)};

};

struct PLogin3rdAuth : public sox::Marshallable
{   
  enum {uri = (211 << 8 | protocol::LINKD_SVID)};

  PLogin3rdAuth() : uid(0), version(0), lcid(0){}
  
  std::string context; // 交互上下文，保留
  uint32_t uid; // uid，没获取到时填0
  
  std::string userName; // 用户名
  std::string accountInfo; // 完整的第三方账号信息
  std::string token; // 相当于密码(cookie用)
  std::string verStr; // 验证要用到

  std::string macAddr;
  uint32_t version;
  uint32_t lcid;

  virtual void marshal(sox::Pack &pk) const
  {
    pk << context << uid << userName << accountInfo << token << verStr ;
    pk << macAddr << version << lcid;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> context >> uid >> userName >> accountInfo >> token >> verStr ;
    up >> macAddr >> version >> lcid;
  }
  virtual std::ostream & trace(std::ostream & os) const
  {
    os << "ctx=" << context << ",";
    os << "uid=" << uid << ",";
    os << "p=" << userName << ":" << accountInfo.size() <<  ":" << token.size() << ",";
    os << "ver=" << verStr << "," << version << "," << lcid << ",";
    return os;  
  }
  std::string dump() const
  {
    std::ostringstream os;
    os << *this;
    return os.str();
  }
};

struct PLogin3rdAuthRes : public PLoginAuthRes
{   
  enum {uri = (212 << 8 | protocol::LINKD_SVID)};

};

struct PQueryLinkdPortRes: public sox::Marshallable{
	enum {uri = (22 << 8 | protocol::LBS_SVID)};
	uint32_t serverId;
	std::vector<uint16_t> ports;
	uint32_t version;
	std::string type;
	virtual void marshal(sox::Pack &pk) const {
		pk << serverId;
		sox::marshal_container(pk, ports);
		pk << version << type;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> serverId;
		sox::unmarshal_container(up, std::back_inserter(ports));
		if(!up.empty())
		{
      up >> version;
		}
		if(!up.empty())
		{
      up >> type;
		}		
	}
};

struct PLinkdTransptQosRes : public sox::Marshallable
{
  enum 
  {
    LINKD_QOS_DELAY = 0,
    LINKD_QOS_BLOCK
  };

  enum { uri = (202 << 8 | protocol::LINKD_SVID)};
  uint32_t qos;
  uint32_t transptUri;
  std::string transptPacket;
  std::string reason;

  virtual void marshal(sox::Pack &pk) const
  {
    pk << qos << transptUri << transptPacket << reason;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> qos >> transptUri >> transptPacket >> reason;
  }
};


// 检查手机令牌动态密码 linkd
struct PCheckDKeyLinkd: public sox::Marshallable
{
  enum {uri = (210 << 8) | protocol::LINKD_SVID};

  uint32_t taskId;
  std::string udb;
  uint32_t uid;
  uint32_t dkey;

  virtual void marshal(sox::Pack &pk) const 
  {
    pk << taskId << udb << uid << dkey;
  }
  virtual void unmarshal(const sox::Unpack &up) 
  {
    up >> taskId >> udb >> uid >> dkey;
  }
};

struct PRequestDKeyLinkd: public sox::Marshallable
{
  enum {uri = (211 << 8) | protocol::LINKD_SVID};

  uint32_t taskId;
  std::string udb;
  uint32_t uid;
  uint32_t timestamp;

  virtual void marshal(sox::Pack &pk) const 
  {
    pk << taskId << udb << uid << timestamp;
  }
  virtual void unmarshal(const sox::Unpack &up) 
  {
    up >> taskId >> udb >> uid >> timestamp;
  }
};

struct PCheckDKeyFailureLinkd: public sox::Marshallable
{
  enum {uri = (212 << 8) | protocol::LINKD_SVID};

  uint32_t taskId;
  uint32_t resCode;
  std::string udb;
  uint32_t uid;

  virtual void marshal(sox::Pack &pk) const 
  {
    pk << taskId << resCode << udb << uid;
  }
  virtual void unmarshal(const sox::Unpack &up) 
  {
    up >> taskId >> resCode >> udb >> uid;
  }
};

//
struct PLoginDynaCheckAsk: public sox::Marshallable
{
  enum{ uri = ( 213 << 8 )|protocol::LINKD_SVID };
  std::string  context;
  std::string  uname;
  uint32_t     uid;
  std::string  cliLoad;  // 客户端子协议消息

  virtual void marshal(sox::Pack &pk) const
  {
     pk << context << uname << uid << cliLoad;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
     up >> context >> uname >> uid >> cliLoad;
  }
};

// 动态验证消息应答
struct PLoginDynaCheckAck: public sox::Marshallable
{
  enum{ uri = ( 214 << 8 )|protocol::LINKD_SVID };
  std::string  context;
  std::string  uname;
  uint32_t     uid;
  std::string  cliLoad;  // 客户端子协议消息

  virtual void marshal(sox::Pack &pk) const
  {
     pk << context << uname << uid << cliLoad;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
     up >> context >> uname >> uid >> cliLoad;
  }
};


struct PLoginLinkdCredRes: public PLoginlinkdRes
{
  enum {uri = (221 << 8) | protocol::LINKD_SVID};
};

struct PLoginLinkdCredTestRes: public PLoginLinkdCredRes
{
  enum {uri = (223 << 8) | protocol::LINKD_SVID};
};

struct PLoginLinkdCred: public PLoginlinkd30
{
  enum {uri = (220 << 8) | protocol::LINKD_SVID};
};

struct PLoginLinkdCredTest: public PLoginLinkdCred
{
  enum {uri = (222 << 8) | protocol::LINKD_SVID};
};

// 第三方登录验证
struct PLoginLbsBy3rd: public sox::Marshallable {
  enum {uri = (23 << 8 | protocol::LBS_SVID)};

  PLoginLbsBy3rd()
  {}

  PLoginLbsBy3rd(uint32_t ver, const std::string& ud, uint32_t id
    , const std::string& account, const std::string& cook, ISPType ty
    , const std::string& vStr, const std::string& mac, uint32_t lc)
    : version(ver), udb(ud), uid(id), account_info(account), cookie(cook), type(ty), verStr(vStr), macAddr(mac), lcid(lc)
  {}

  uint32_t version;
  std::string udb;
  uint32_t uid;
  std::string account_info;
  std::string token;
  std::string cookie;
  ISPType type;

  std::string verStr;
  std::string macAddr;

  uint32_t lcid;

  virtual void marshal(sox::Pack &pk) const {
    pk << version << udb << uid << account_info << token << cookie;
    pk.push_uint16(type);
    pk << verStr << macAddr << lcid;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> version >> udb >> uid >> account_info >> token >> cookie;
    type = (ISPType)up.pop_uint16();
    up >> verStr >> macAddr >> lcid;
  }
  virtual std::ostream & trace(std::ostream & os) const
  {
    os << "p=" << udb << ":" << account_info.size() << "," << token.size() << ",";
    os << "uid=" << uid << ",";
    os << "ver=" << version << "+" << lcid << "=" << verStr << ",";
    os << "c#=" << cookie.size() << ",";
    os << "m#=" << macAddr.size() << ",";
    return os;  
  }
};

// 第三方登录验证(webyy)
struct PLoginLbsBy3rdProxy: public sox::Marshallable {
  enum {uri = (30 << 8 | protocol::LBS_SVID)};

  PLoginLbsBy3rdProxy()
   : version(0),uid(0),lcid(0),clientIp(0),clientPort(0){}

  uint32_t version;
  std::string udb;
  uint32_t uid;
  std::string account_info;
  std::string token;
  std::string cookie;
  ISPType type;

  std::string verStr;
  std::string macAddr;

  uint32_t lcid;

  std::string from;	// 附加的平台信息，字符串长度<=10
  uint32_t clientIp;
  uint16_t clientPort;

  virtual void marshal(sox::Pack &pk) const {
    pk << version << udb << uid << account_info << token << cookie;
    pk.push_uint16(type);
    pk << verStr << macAddr << lcid << from << clientIp << clientPort;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> version >> udb >> uid >> account_info >> token >> cookie;
    type = (ISPType)up.pop_uint16();
    up >> verStr >> macAddr >> lcid >> from >> clientIp;
    if(!up.empty())
    {
      up >> clientPort;
    }
  }
  virtual std::ostream & trace(std::ostream & os) const
  {
    os << "p=" << udb << ":" << account_info.size() << "," << token.size() << ",";
    os << "uid=" << uid << ",";
    os << "ver=" << version << "+" << lcid << "=" << verStr << ",";
    os << "c#=" << cookie.size() << ",";
    os << "m#=" << macAddr.size() << ",";
    return os;  
  }
};

struct PLoginLbsBy3rdRes: public PloginlbsIPRes3
{
  enum {uri = (24 << 8 | protocol::LBS_SVID)};
};

struct PAntiCode: public sox::Marshallable
{
  enum {uri = (25 << 8) | protocol::LBS_SVID};

  std::string antiCode;

  virtual void marshal(sox::Pack &pk) const 
  {
    pk << antiCode;
  }
  virtual void unmarshal(const sox::Unpack &up) 
  {
    up >> antiCode;
  }
};

struct PloginLinkd2Retry: public sox::Marshallable
{		
	enum {uri = (203 << 8) | protocol::LINKD_SVID};

	PloginLinkd2Retry(){}

	uint32_t uType;
	uint32_t uSameAccountTimes;
	uint32_t uSameIpTimes;
	std::string strReserve;

	virtual void marshal(sox::Pack &pk) const {
		pk << uType << uSameAccountTimes << uSameIpTimes << strReserve;	
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uType >> uSameAccountTimes >> uSameIpTimes >> strReserve;
	}
};

struct PNotifySlaveLinkdInfo: public sox::Marshallable
{
  enum {uri = (300 << 8) | protocol::LINKD_SVID};
  PNotifySlaveLinkdInfo (){}

  uint32_t uid;
  uint32_t ip;
  uint32_t serverId;

  virtual void marshal(sox::Pack &pk) const {
    pk << uid << ip << serverId;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> uid >> ip >> serverId;
  }
};

struct PNotifySlaveLinkdInfoRes: public sox::Marshallable
{
  enum {uri = (301 << 8) | protocol::LINKD_SVID};
  PNotifySlaveLinkdInfoRes (){}

  uint32_t res;
  virtual void marshal(sox::Pack &pk) const {
    pk << res;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> res;
  }
};

struct PLoginLinkdUdp: public sox::Marshallable
{
  enum {uri = (303 << 8) | protocol::LINKD_SVID};
  PLoginLinkdUdp ():isRelogin(0){}

  uint32_t uid;
  uint32_t ip;               //master linkd ip
  uint32_t serverId;         //master serverId
  uint32_t seqNum;
  std::string publicKey;
  std::string e;
  std::string ticket;
  bool isRelogin;            //是否重登录

  virtual void marshal(sox::Pack &pk) const {
    pk << uid << ip << serverId << seqNum << publicKey << e << ticket;
    pk << isRelogin;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> uid >> ip >> serverId >> seqNum >> publicKey >> e >> ticket;
    if (!up.empty())
    {
      up >> isRelogin;
    }
  }
};

struct PLoginLinkdUdpRes: public sox::Marshallable
{
  enum {uri = (304 << 8) | protocol::LINKD_SVID};
  PLoginLinkdUdpRes (){}

  uint32_t res;
  uint32_t seqNum;
  std::string key;

  virtual void marshal(sox::Pack &pk) const {
    pk << res << seqNum << key;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> res >> seqNum >>  key;
  }
};

struct PPingSlaveLinkd: public sox::Marshallable
{
  enum {uri = (307 << 8) | protocol::LINKD_SVID};
  PPingSlaveLinkd (){}

  uint32_t uid;
  uint32_t seq;

  virtual void marshal(sox::Pack &pk) const {
    pk << uid << seq;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> uid >> seq;
  }
};

struct PPingSlaveLinkdRes: public sox::Marshallable
{
  enum {uri = (308 << 8) | protocol::LINKD_SVID};
  PPingSlaveLinkdRes (){}
  uint32_t uid;
  uint32_t seq;
  uint32_t res;
  uint32_t serverId;

  virtual void marshal(sox::Pack &pk) const {
    pk << uid << seq << res << serverId;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> uid >> seq >> res >> serverId;
  }
};

struct PStoreLoginInfo : public sox::Marshallable
{
  enum {uri = (12000 << 8) | protocol::LINKD_SVID};
  std::string       passport;
  std::string       email;
  std::string       passwd;
  std::string       token;
  std::string       reserve;
  uint32_t          version;
  uint32_t          uid;

  virtual void marshal(sox::Pack &p) const
  {
    p << passport << email << passwd << token << reserve << version << uid;
  }

  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> passport >> email >> passwd >> token >> reserve >> version >> uid;
  }
  
  virtual std::ostream & trace(std::ostream & os) const
  {
    os << passport << " " << email << " "  << passwd.size() << " " << token.size() << " ";
    os << reserve.size() << " ";
    os << version << " " << uid << " ";
    return os;
  }  
};

struct PGetUserLoginInfo : public sox::Marshallable
{
  enum {uri = (12001 << 8) | protocol::LINKD_SVID};

  uint32_t          taskId;
  std::string       passport;
  std::string       email;
  std::string       passwd;

  PGetUserLoginInfo() : taskId(0){}
  PGetUserLoginInfo(uint32_t id, const std::string & pp, const std::string & em, const std::string & pw) :
    taskId(id), passport(pp), email(em), passwd(pw){}
  
  virtual void marshal(sox::Pack &p) const
  {
    p << taskId << passport << email << passwd;
  }

  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> taskId >> passport >> email >> passwd;
  }
  
  virtual std::ostream & trace(std::ostream & os) const
  {
    os << taskId << " " << passport << " " << email << " "  << passwd.size();
    return os;
  }  
};

struct PGetUserLoginInfoRes : public PStoreLoginInfo
{
  enum {uri = (12002 << 8) | protocol::LINKD_SVID};

  uint32_t taskId;
  uint32_t resCode;
  std::string reserve;

  virtual void marshal(sox::Pack &p) const
  {
    p << taskId << resCode << reserve;
    PStoreLoginInfo::marshal(p);
  }

  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> taskId >> resCode >> reserve;
    PStoreLoginInfo::unmarshal(up);
  }

  virtual std::ostream & trace(std::ostream & os) const
  {
    os << taskId << " " << resCode << " " << reserve.size() << " ";
    PStoreLoginInfo::trace(os);
    return os;
  }

};

struct PGetUserLoginInfoRes2 : public PStoreLoginInfo
{
  enum {uri = (12003 << 8) | protocol::LINKD_SVID};

  uint32_t          taskId;

  virtual void marshal(sox::Pack &p) const
  {
    p << taskId;
    PStoreLoginInfo::marshal(p);
  }

  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> taskId;
    PStoreLoginInfo::unmarshal(up);
  }

  virtual std::ostream & trace(std::ostream & os) const
  {
    os << taskId << " ";
    PStoreLoginInfo::trace(os);
    return os;
  }

};

struct PLbsLinkdStatusPing: public sox::Marshallable
{
  enum {uri = (26 << 8) | protocol::LBS_SVID};
  std::string context;

  virtual void marshal(sox::Pack &pk) const 
  {
    pk << context;
  }
  virtual void unmarshal(const sox::Unpack &up) 
  {
    up >> context;
  }
};

struct PLbsLinkdStatusPingRes: public sox::Marshallable
{
  enum {uri = (27 << 8) | protocol::LBS_SVID};
  std::string context;
  uint32_t linkdServerId;
  uint32_t linkdTimeStamp;
  uint32_t linkdSeq;
  uint32_t userOnlineLoad;
  uint32_t timeoutRate;

  PLbsLinkdStatusPingRes() : linkdServerId(0),linkdTimeStamp(0),
    linkdSeq(0),userOnlineLoad(0),timeoutRate(0){}

  virtual void marshal(sox::Pack &pk) const 
  {
    pk << context << linkdServerId << linkdTimeStamp << linkdSeq;
    pk << userOnlineLoad << timeoutRate;
  }
  virtual void unmarshal(const sox::Unpack &up) 
  {
    up >> context >> linkdServerId >> linkdTimeStamp >> linkdSeq;
    up >> userOnlineLoad >> timeoutRate;
  }
  virtual std::ostream & trace(std::ostream & os) const
  {
    os << " " << context.size();
    os << " " << linkdServerId << " " << linkdTimeStamp << " " << linkdSeq;
    os << " " << userOnlineLoad << " " << timeoutRate;
    return os;
  }  
};

struct PQueryLinkdPortRes2: public PQueryLinkdPortRes{
	enum {uri = (28 << 8 | protocol::LBS_SVID)};
	// no more fields extension
};

struct PLinkdInfo3: public sox::Marshallable2{
  
  uint32_t ip;
  std::vector<uint16_t> ports;
  uint32_t serverId;

  virtual void marshal2(sox::Pack &pk) const {
    pk << ip;
    sox::marshal_container(pk, ports);
    pk << serverId;
  }
  virtual void unmarshal2(const sox::Unpack &up) {
    up >> ip;
    sox::unmarshal_container(up, std::back_inserter(ports));
    up >> serverId;
  }   

    bool operator < (const PLinkdInfo3& b) const
    {
        if(ip == b.ip)
            return (serverId < b.serverId);
        else
            return (ip < b.ip);
  }

  virtual std::ostream & trace(std::ostream & os) const
  {
    os << " " << ip2string(ip) << ":";
    std::copy(ports.begin(),ports.end(),std::ostream_iterator<uint16_t>(os, ":"));
    os << " " << serverId;
    return os;
  }   
};

struct PRedirectLinkd: public sox::Marshallable{
  enum {uri = (230 << 8)|protocol::LINKD_SVID};
  std::vector<PLinkdInfo3> linkds;

  virtual void marshal(sox::Pack &pk) const {
    sox::marshal_container(pk, linkds);
  }
  virtual void unmarshal(const sox::Unpack &up) {
    sox::unmarshal_container(up, std::back_inserter(linkds));
  }
};

struct PRedirectLinkdRes: public sox::Marshallable{
  enum {uri = (231 << 8)|protocol::LINKD_SVID};
  uint32_t res;

  virtual void marshal(sox::Pack &pk) const {
    pk << res;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> res;
  }
};

typedef ZipMarshal<PExchangeKey, 42 << 8 | protocol::LINKD_SVID> PExchangeKeyZip;
typedef ZipMarshal<PExchangeKey_LINKRes, 43 << 8 | protocol::LINKD_SVID> PExchangeKey_LINKZipRes;

struct PRequestLinkd : public sox::Marshallable
{   
  enum {uri = (1<<16) | (protocol::LBSSMGR_LINKDTRANSMIT_SUBSVID << 8) | LINKD_TRANSMIT_SVID};
  uint32_t version; // 客户端版本号 
  uint32_t uid; // uid
  uint32_t ip; //客户端ip
  uint32_t masterLinkdIp; //主linkdip
  std::string passport; // 用户名(有可能是中文)

  PRequestLinkd():version(0),uid(0),ip(0),masterLinkdIp(0){}

  virtual void marshal(sox::Pack &pk) const
  {
    pk << version << uid << ip << masterLinkdIp << passport;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> version >> uid >> ip >> masterLinkdIp >> passport;
  }
};

struct PRequestLinkdRes : public sox::Marshallable
{   
  enum {uri = (2<<16) | (protocol::LBSSMGR_LINKDTRANSMIT_SUBSVID << 8) | LINKD_TRANSMIT_SVID};
  enum REQUEST_LINKD_RES
  {
    REQ_SUCCESS,
    REQ_FAILED
  };
  uint32_t resCode;
  std::vector<PLinkdInfo3> linkds; // linkd列表  

  virtual void marshal(sox::Pack &pk) const
  {
    pk << resCode;
    sox::marshal_container(pk, linkds);
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> resCode;
    sox::unmarshal_container(up, std::back_inserter(linkds));
  }
};

struct PReqProbeIps : public sox::Marshallable
{
    enum {uri = PReqProbeIpsURI};
    uint32_t version;
    uint32_t uid;
    uint32_t ip;
    std::string passport;

    PReqProbeIps():version(0),uid(0),ip(0){}

    virtual void marshal(sox::Pack &pk) const
    {
        pk << version << uid << ip << passport;
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        up >> version >> uid >> ip >> passport;
    }
};

struct PReqProbeIpsRes : public sox::Marshallable
{
    enum {uri = PReqProbeIpsResURI};
    bool probeSwitch;
    bool cacheProbeIpSwitch;
    uint8_t probeReportNum;
    std::map<uint32_t, std::set<PLinkdInfo3> > probeIps;

    virtual void marshal(sox::Pack &pk) const
    {
        pk << probeSwitch << cacheProbeIpSwitch << probeReportNum;
        pk << static_cast<uint32_t>(probeIps.size());
        std::map<uint32_t, std::set<PLinkdInfo3> >::const_iterator cit;
        for(cit = probeIps.begin(); cit != probeIps.end(); ++cit)
        {
            pk << cit->first;
            sox::marshal_container(pk, cit->second);
        }
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        up >> probeSwitch >> cacheProbeIpSwitch >> probeReportNum;
        uint32_t mapSize;
        up >> mapSize;
        for(uint32_t loop = 0; loop < mapSize; ++loop)
        {
            uint32_t groupId;
            up >> groupId;
            std::set<PLinkdInfo3>& tmpSet = probeIps[groupId];
            sox::unmarshal_container(up, std::inserter(tmpSet, tmpSet.begin()));
        }
    }
};

struct PProbeSeverReq : public sox::Marshallable
{
    enum {uri = PProbeSeverReqURI};
    uint32_t uid;
    uint32_t timeStamp;

    virtual void marshal(sox::Pack &pk) const
    {
        pk << uid << timeStamp;
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        up >> uid >> timeStamp;
    }
};

struct PProbeSeverRes : public sox::Marshallable
{
    enum {uri = PProbeSeverResURI};
    uint32_t clientIp;
    uint32_t timeStamp;
    bool stop;

    virtual void marshal(sox::Pack &pk) const
    {
        pk << clientIp << timeStamp << stop;
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        up >> clientIp >> timeStamp >> stop;
    }
};

struct PLinkdWithProbeReq : public sox::Marshallable
{
    enum {uri = PLinkdWithProbeReqURI};
    uint32_t clientIp;
    std::string account;

    virtual void marshal(sox::Pack &pk) const
    {
        pk << clientIp << account;
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        up >> clientIp >> account;
    }
};

struct PLinkdWithProbeRes : public sox::Marshallable
{
    enum {uri = PLinkdWithProbeResURI};
    bool result;
    std::vector<PLinkdInfo2> linkds;

    virtual void marshal(sox::Pack &pk) const
    {
        pk << result;
        sox::marshal_container(pk, linkds);
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        up >> result;
        sox::unmarshal_container(up, std::back_inserter(linkds));
    }
};

struct PCheckCacheDnsLbs : public sox::Marshallable
{
    enum {uri = PCheckCacheDnsLbs_URI};
    std::map<std::string, uint32_t> domain2Size;
    std::string md5sum;

    virtual void marshal(sox::Pack &pk) const
    {
        sox::marshal_container(pk, domain2Size);
        pk << md5sum;
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        sox::unmarshal_container(up, std::inserter(domain2Size, domain2Size.begin()));
        up >> md5sum;
    }
};

struct PCacheDnsLbs : public sox::Marshallable2
{
    std::set<uint32_t> lbsIps;
    virtual void marshal2(sox::Pack &pk) const
    {
        sox::marshal_container(pk, lbsIps);
    }
    virtual void unmarshal2(const sox::Unpack &up)
    {
        sox::unmarshal_container(up, std::inserter(lbsIps, lbsIps.begin()));
    }
};

struct PCheckCacheDnsLbsRes : public sox::Marshallable
{
    enum {uri = PCheckCacheDnsLbsRes_URI};
    uint32_t resCode;
    std::string md5sum;
    uint32_t reserve;
    typedef std::map<std::string, PCacheDnsLbs> DOMAIN_LBS_MAP_T;
    DOMAIN_LBS_MAP_T domain2lbsIps;

    virtual void marshal(sox::Pack &pk) const
    {
        pk << resCode << md5sum << reserve;
        sox::marshal_container(pk, domain2lbsIps);
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        up >> resCode >> md5sum >> reserve;
        sox::unmarshal_container(up, std::inserter(domain2lbsIps, domain2lbsIps.begin()));
    }
};

struct MobileProxyInfo : public sox::Marshallable
{
  uint32_t ip;
  std::vector<uint16_t> ports;
  uint32_t serverId;

  MobileProxyInfo(): ip(0), serverId(0) {}

  virtual void marshal(sox::Pack &pk) const
  {
    pk << ip << serverId;
    sox::marshal_container(pk, ports);
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> ip >> serverId;
    sox::unmarshal_container(up, std::back_inserter(ports));
  }

  bool operator < (const MobileProxyInfo& b) const
  {
    if(ip == b.ip)
      return (serverId < b.serverId);
    else
      return (ip < b.ip);
  }  

  virtual std::ostream& trace(std::ostream & os) const
  {
    os << " " << ip2string(ip) << ":";
    std::copy(ports.begin(), ports.end(), std::ostream_iterator<uint16_t>(os, ":"));
    os << " " << serverId;
    return os;
  }
};

struct PRequestMobleProxy : public sox::Marshallable
{
  enum {uri = 19000 << 8 | protocol::LINKD_SVID};

  PRequestMobleProxy() {}
  PRequestMobleProxy(std::string cont, uint32_t ver, uint32_t u, std::string pass)
    :context(cont), version(ver), uid(u), passport(pass) {}

  std::string context;   // 交互上下文，保留
  uint32_t version;      // sdk版本号
  uint32_t uid;          // uid，没获取到时填0
  std::string passport;  // 用户名(有可能是中文)

  virtual void marshal(sox::Pack &pk) const
  {
    pk << context << version << uid << passport;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> context >> version >> uid >> passport;
  }
};

struct PRequestMobleProxyRes : public sox::Marshallable
{
  enum {uri = 19001 << 8 | protocol::LINKD_SVID};

  std::string context;   // 交互上下文，保留
  uint32_t resCode;     // 返回码
  std::vector<MobileProxyInfo> proxyInfos;
  AreaType areaType;     // 分区类型: 电信南区/电信东区...
  uint32_t clientIp;     // 客户端ip

  virtual void marshal(sox::Pack &pk) const
  {
    pk << context << resCode;
    sox::marshal_container(pk, proxyInfos);
    pk.push_uint16(areaType);
    pk << clientIp;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> context >> resCode;
    sox::unmarshal_container(up, std::back_inserter(proxyInfos));
    areaType = (AreaType)up.pop_uint16();
    up >> clientIp;
  }
};

struct PMpbsMobilePorxyStatusPing: public sox::Marshallable
{
  enum {uri = (19002 << 8) | protocol::LINKD_SVID};
  uint32_t proxyServerId;
  uint32_t proxyTimeStamp;
  uint32_t proxySeq;
  uint32_t userOnlineLoad;

  PMpbsMobilePorxyStatusPing() : proxyServerId(0),proxyTimeStamp(0),
    proxySeq(0),userOnlineLoad(0){}

  virtual void marshal(sox::Pack &pk) const 
  {
    pk << proxyServerId << proxyTimeStamp << proxySeq << userOnlineLoad;
  }
  virtual void unmarshal(const sox::Unpack &up) 
  {
    up >> proxyServerId >> proxyTimeStamp >> proxySeq >> userOnlineLoad;
  }
  virtual std::ostream & trace(std::ostream & os) const
  {
    os << " " << proxyServerId << " " << proxyTimeStamp;
    os << " " << proxySeq << " " << userOnlineLoad;
    return os;
  }
};

struct PMobileLoginProxy : public sox::Marshallable
{
    enum {uri = 20001 << 8 | protocol::LINKD_SVID};
    enum
    {
      AUTO = 0,
      PASSPORT = 1,
      EMAIL = 2,
      THIRD = 3,
      YYID = 4,
      Anonymous = 5,
    };
    
    PMobileLoginProxy(): accountType(AUTO){}

    uint32_t accountType;
    std::string account;
    std::string passwdMd5;
    
    std::string platform;
    std::string version;

    virtual void marshal(sox::Pack &pk) const
    {
        pk << accountType << account << passwdMd5;
        pk << platform << version;
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        up >> accountType >> account >> passwdMd5;
        up >> platform >> version;
    }
};

struct PMobileLoginProxyRes : public sox::Marshallable
{
    enum {uri = 20002 << 8 | protocol::LINKD_SVID};
    PMobileLoginProxyRes(): rescode(RES_NOCHANGED),uid(0){}

    uint32_t rescode;
    std::string passport;
    uint32_t uid;
    std::string cookie;
    std::string extension;
    std::string account;

    virtual void marshal(sox::Pack &pk) const
    {
        pk << rescode << passport << uid << cookie << extension << account;
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        up >> rescode >> passport >> uid >> cookie;
        if(!up.empty())
        {
          up >> extension;
        }
        if(!up.empty())
        {
          up >> account;
        }
    }
};

struct PMobileJoinChannel : public sox::Marshallable
{
    enum {uri = 20003 << 8 | protocol::LINKD_SVID};
    PMobileJoinChannel(): uid(0),sid(0){}
    
    uint32_t uid;
    uint32_t sid;
    uint32_t subsid;

    virtual void marshal(sox::Pack &pk) const
    {
        pk << uid << sid << subsid;
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        up >> uid >> sid;
        if(!up.empty())
        {
          up >> subsid;
        }
    }
};

struct PMobileJoinChannelRes : public sox::Marshallable
{
    enum {uri = 20004 << 8 | protocol::LINKD_SVID};
    PMobileJoinChannelRes(): rescode(RES_NOCHANGED),asid(0),sid(0),subsid(0){}

    uint32_t rescode;
    uint32_t asid;
    uint32_t sid;
    uint32_t subsid;
    std::string configSetting;

    virtual void marshal(sox::Pack &pk) const
    {
        pk << rescode << asid << sid << subsid << configSetting;
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        up >> rescode >> asid >> sid >> subsid >> configSetting;
    }
};

struct PMobileProxyPing : public sox::Marshallable
{
    enum {uri = 20005 << 8 | protocol::LINKD_SVID};
    PMobileProxyPing(): stampc(0){}
    
    uint32_t stampc;

    virtual void marshal(sox::Pack &pk) const
    {
        pk << stampc ;
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        up >> stampc;
    }
};

struct PMobileProxyPingRes : public sox::Marshallable
{
    enum {uri = 20006 << 8 | protocol::LINKD_SVID};
    PMobileProxyPingRes(): stampc(0),stamps(0){}
    
    uint32_t stampc;
    uint32_t stamps;

    virtual void marshal(sox::Pack &pk) const
    {
        pk << stampc << stamps ;
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        up >> stampc >> stamps;
    }
};

struct PMobileLogoutProxy : public sox::Voidmable
{
    enum {uri = 20007 << 8 | protocol::LINKD_SVID};
};

struct PMobileOnlineKickOff : public sox::Marshallable
{
    enum {uri = 20008 << 8 | protocol::LINKD_SVID};
    std::string msg; // todo 原因或者是kickoff消息包，暂不处理

    virtual void marshal(sox::Pack &pk) const
    {
        pk << msg;
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        up >> msg;
    }
};

struct PMobileLeaveChannel : public sox::Voidmable
{
    enum {uri = 20009 << 8 | protocol::LINKD_SVID};
};

struct PMobileChannelKickOff : public sox::Marshallable
{
    enum {uri = 20010 << 8 | protocol::LINKD_SVID};
    std::string msg; // todo 原因或者是kickoff消息包，暂不处理

    virtual void marshal(sox::Pack &pk) const
    {
        pk << msg;
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        up >> msg;
    }
};

struct PMobileTransmitUpLinkData : public sox::Marshallable
{
    enum {uri = 20011 << 8 | protocol::LINKD_SVID};
    enum DATA_TYPE
    {
      AUTO_DETECT = 0,
      LINKD = 1,
      SIGNAL = 2,
      SERVICE = 3,
    };
    uint32_t type;
    uint32_t uid;
    uint32_t sid;
    uint32_t innerUri;
    std::string msg; // limit 64k

    virtual void marshal(sox::Pack &pk) const
    {
        pk << type << uid << sid << innerUri << msg;
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        up >> type >> uid >> sid >> innerUri >> msg;
    }
};

struct PMobileTransmitUpLinkDataRes : public sox::Marshallable
{
    enum {uri = 20012 << 8 | protocol::LINKD_SVID};
    uint32_t rescode;

    virtual void marshal(sox::Pack &pk) const
    {
        pk << rescode;
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        up >> rescode;
    }
};

struct PMobileTransmitDownLinkData : public sox::Marshallable
{
    enum {uri = 20013 << 8 | protocol::LINKD_SVID};
    enum DATA_TYPE
    {
      OTHER = 0,
      LINKD = 1,
      SIGNAL = 2,
      SERVICE = 3,
    };
    uint32_t type;
    uint32_t uid;
    uint32_t sid;
    uint32_t innerUri;
    std::string msg; // limit 64k

    virtual void marshal(sox::Pack &pk) const
    {
        pk << type << uid << sid << innerUri << msg;
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        up >> type >> uid >> sid >> innerUri >> msg;
    }
};

struct PMobileReloginProxy : public sox::Marshallable
{
    enum {uri = 20015 << 8 | protocol::LINKD_SVID};
    PMobileReloginProxy(): uid(0),topsid(0),accountType(0),subsid(0){}
    enum
    {
      AUTO = 0,
      PASSPORT = 1,
      EMAIL = 2,
      THIRD = 3,
      YYID = 4,
      Anonymous = 5,
    };

    uint32_t uid;
    uint32_t topsid;
    uint32_t accountType;
    std::string account;
    std::string passwdMd5;
    std::string platform;
    std::string version;
    std::string cookie;
    uint32_t subsid;

    virtual void marshal(sox::Pack &pk) const
    {
        pk << uid << topsid;
        pk << accountType << account << passwdMd5;
        pk << platform << version << cookie;
        pk << subsid;
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        up >> uid >> topsid;
        up >> accountType >> account >> passwdMd5;
        up >> platform >> version >> cookie;
        if(!up.empty())
        {
          up >> subsid;
        }
    }
};

struct PMobileReloginProxyRes : public sox::Marshallable
{
    enum {uri = 20016 << 8 | protocol::LINKD_SVID};
    PMobileReloginProxyRes(): rescode(RES_NOCHANGED),rescodeChannel(RES_NOCHANGED),uid(0),topsid(0),subsid(0){}

    // no passport and asid yet
    uint32_t rescode;
    uint32_t rescodeChannel;
    uint32_t uid;
    uint32_t topsid;
    uint32_t subsid;
    std::string cookie;

    virtual void marshal(sox::Pack &pk) const
    {
        pk << rescode << rescodeChannel << uid << topsid << subsid;
        pk << cookie;
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        up >> rescode >> rescodeChannel >> uid >> topsid >> subsid;
        up >> cookie;
    }
};

struct PMobileGetChannelUserCountBySubsids : public sox::Marshallable
{
    enum {uri = 20017 << 8 | protocol::LINKD_SVID};

    std::set<uint32_t> subsids;

    virtual void marshal(sox::Pack &pk) const
    {
        sox::marshal_container(pk, subsids);
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        sox::unmarshal_container(up, std::inserter(subsids, subsids.begin()));
    }
};

struct PMobileGetChannelUserCountBySubsidsRes : public sox::Marshallable
{
    enum {uri = 20018 << 8 | protocol::LINKD_SVID};
    PMobileGetChannelUserCountBySubsidsRes() : totalCnts(0){}

    uint32_t totalCnts;
    std::map<uint32_t, uint32_t> subsid2Cnts;

    virtual void marshal(sox::Pack &pk) const
    {
        pk << totalCnts;
        sox::marshal_container(pk, subsid2Cnts);
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        up >> totalCnts;
        sox::unmarshal_container(up, std::inserter(subsid2Cnts, subsid2Cnts.begin()));
    }

};

struct PMobileGetChannelUserCountByTopsid : public sox::Marshallable
{
    enum {uri = 20019 << 8 | protocol::LINKD_SVID};

    uint32_t topsid;

    virtual void marshal(sox::Pack &pk) const
    {
        pk << topsid;
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        up >> topsid;
    }
};

struct PMobileGetChannelUserCountByTopsidRes : public sox::Marshallable
{
    enum {uri = 20020 << 8 | protocol::LINKD_SVID};

    std::map<uint32_t, uint32_t> subsid2Cnts;

    virtual void marshal(sox::Pack &pk) const
    {
        sox::marshal_container(pk, subsid2Cnts);
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        sox::unmarshal_container(up, std::inserter(subsid2Cnts, subsid2Cnts.begin()));
    }

};

struct PMobileTextChatUpLink: public sox::Marshallable
{
	enum{ uri = (20021 << 8 | protocol::LINKD_SVID) };
	uint32_t from;
	uint32_t topsid;
	uint32_t sid;
	std::string chat;
	std::string nick;
	std::string sign;

	virtual void marshal(sox::Pack &p) const 
	{
		p << from << topsid << sid << chat << nick << sign;
	}
	virtual void unmarshal(const sox::Unpack &p) 
	{
		p >> from >> topsid >> sid >> chat >> nick >> sign;
	}
};

struct PMobileTextChatDownLink: public sox::Marshallable
{
	enum{ uri = (20022 << 8 | protocol::LINKD_SVID) };
	uint32_t from;
	uint32_t topsid;
	uint32_t sid;
	std::string chat;
	std::string nick;
	std::string sign;

	virtual void marshal(sox::Pack &p) const 
	{
		p << from << topsid << sid << chat << nick << sign;
	}
	virtual void unmarshal(const sox::Unpack &p) 
	{
		p >> from >> topsid >> sid >> chat >> nick >> sign;
	}
};

struct PMobileServiceUpLink : public sox::Marshallable {
	enum{uri = (20023 << 8 | protocol::LINKD_SVID)};
	uint16_t serviceType;
	uint32_t topsid;
	uint32_t uid;
	std::string msg;
	virtual void marshal(sox::Pack &pak) const {
		pak << serviceType << topsid<< uid;
		pak.push_varstr32(msg.data(), msg.size());
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> serviceType >> topsid >> uid;
		msg = pak.pop_varstr32();
	}
};

struct PMobileServiceDownLink : public sox::Marshallable {
	enum{uri = (20024 << 8 | protocol::LINKD_SVID)};
	uint16_t serviceType;
	uint32_t topsid;
	uint32_t uid;
	std::string msg;
	virtual void marshal(sox::Pack &pak) const {
		pak << serviceType << topsid << uid;
		pak.push_varstr32(msg.data(), msg.size());
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> serviceType >> topsid >> uid;
		msg = pak.pop_varstr32();
	}
};

struct PMobileGetAppBySubsid : public sox::Marshallable {
	enum{uri = (20025 << 8 | protocol::LINKD_SVID)};
	uint32_t topsid;
	uint32_t subsid;
	uint32_t uid;
	virtual void marshal(sox::Pack &pak) const {
		pak << topsid << subsid<< uid;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> topsid >> subsid >> uid;
	}
};

struct PMobileGetAppBySubsidRes : public sox::Marshallable {
	enum{uri = (20026 << 8 | protocol::LINKD_SVID)};
	uint32_t topsid;
	uint32_t subsid;
	uint32_t uid;
	uint32_t rescode;
	std::vector<uint32_t> sidApps;
	virtual void marshal(sox::Pack &pak) const {
		pak << topsid << subsid << uid << rescode;
		sox::marshal_container(pak, sidApps);
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> topsid >> subsid >> uid >> rescode;
		sox::unmarshal_container(pak, std::back_inserter(sidApps));
	}
};

struct PMobileGetAppByTopsid : public sox::Marshallable {
	enum{uri = (20027 << 8 | protocol::LINKD_SVID)};
	uint32_t topsid;
	uint32_t uid;
	virtual void marshal(sox::Pack &pak) const {
		pak << topsid << uid;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> topsid >> uid;
	}
};

struct PMobileGetAppByTopsidRes : public sox::Marshallable {
	enum{uri = (20028 << 8 | protocol::LINKD_SVID)};
	uint32_t topsid;
	uint32_t uid;
	uint32_t rescode;
	std::vector<uint32_t> globalApps;
	virtual void marshal(sox::Pack &pak) const {
		pak << topsid << uid << rescode;
		sox::marshal_container(pak, globalApps);
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak >> topsid >> uid >> rescode;
		sox::unmarshal_container(pak, std::back_inserter(globalApps));
	}
};

struct PMobileAppAddNotify : public sox::Marshallable
{
    enum{uri = (20029 << 8 | protocol::LINKD_SVID)};
    uint32_t topSid;
    std::vector<uint32_t>   appIds;
    virtual void marshal(sox::Pack &pk) const {
        pk << topSid;
        sox::marshal_container(pk, appIds);
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> topSid;
        sox::unmarshal_container(up, std::back_inserter(appIds));
    }
};

struct PMobileAppDelNotify : public sox::Marshallable
{
    enum{uri = (20030 << 8 | protocol::LINKD_SVID)};
    uint32_t topSid;
    std::vector<uint32_t>   appIds;
    virtual void marshal(sox::Pack &pk) const {
        pk << topSid;
        sox::marshal_container(pk, appIds);
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> topSid;
        sox::unmarshal_container(up, std::back_inserter(appIds));
    }
};

struct PMobileSubscribeApp : public sox::Marshallable 
{
    enum{uri = (20031 << 8 | protocol::LINKD_SVID)};
    enum 
    {
        CMD_SUBSCRIBE_BYIDS=1,
        CMD_CANCEL_SUBSCRIBE_BYIDS=3,
        CMD_CANCEL_SUBSCRIBE_ALL=4,   
    };

    uint32_t                cmd;
    std::vector<uint32_t>   appIds;

    virtual void marshal(sox::Pack &pak) const {
        pak << cmd;
        sox::marshal_container(pak, appIds);
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> cmd;
        sox::unmarshal_container(up, std::back_inserter(appIds));
    }
};

struct PMobileSubscribeAppRes : public sox::Marshallable 
{
    enum{uri = (20032 << 8 | protocol::LINKD_SVID)};

    std::vector<uint32_t>   appIds;

    virtual void marshal(sox::Pack &pak) const {
        sox::marshal_container(pak, appIds);
    }
    virtual void unmarshal(const sox::Unpack &up) {
        sox::unmarshal_container(up, std::back_inserter(appIds));
    }
};

struct PMobileSubscribeAppOutCh : public sox::Marshallable 
{
  enum{uri = (20033 << 8 | protocol::LINKD_SVID)};
  enum 
  {
    CMD_SUBSCRIBE_BYIDS=1,
    CMD_CANCEL_SUBSCRIBE_BYIDS=3,
    CMD_CANCEL_SUBSCRIBE_ALL=4,  
  };

  uint32_t                cmd;
  std::vector<uint32_t>   appIds;

  virtual void marshal(sox::Pack &pak) const {
    pak << cmd;
    sox::marshal_container(pak, appIds);
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> cmd;
    sox::unmarshal_container(up, std::back_inserter(appIds));
  }
};

struct PMobileSubscribeAppOutChRes : public sox::Marshallable 
{
  enum{uri = (20034 << 8 | protocol::LINKD_SVID)};

  std::vector<uint32_t>   appIds;

  virtual void marshal(sox::Pack &pak) const {
    sox::marshal_container(pak, appIds);
  }
  virtual void unmarshal(const sox::Unpack &up) {
    sox::unmarshal_container(up, std::back_inserter(appIds));
  }
};

struct PMusicQuery : public sox::Marshallable 
{
    enum{uri = (30001 << 8 | protocol::LINKD_SVID)};

    PMusicQuery() : fromServerId(0){}

    std::string msg;
    std::string appData;
    std::map<std::string, std::string> extraInfo;
    uint32_t fromServerId;

    virtual void marshal(sox::Pack &pak) const {
        pak.push_varstr32(msg.data(), msg.size());
        pak.push_varstr32(appData.data(), appData.size());
        sox::marshal_container(pak, extraInfo);
        pak << fromServerId;
    }
    virtual void unmarshal(const sox::Unpack &up) {
        msg = up.pop_varstr32();
        appData = up.pop_varstr32();
        sox::unmarshal_container(up, std::inserter(extraInfo, extraInfo.begin()));
        up >> fromServerId;
    }
};

struct PMusicQueryRes : public sox::Marshallable 
{
    enum{uri = (30002 << 8 | protocol::LINKD_SVID)};

    std::string msg;
    std::string appData;
    std::map<std::string, std::string> extraInfo;

    virtual void marshal(sox::Pack &pak) const {
        pak.push_varstr32(msg.data(), msg.size());
        pak.push_varstr32(appData.data(), appData.size());
        sox::marshal_container(pak, extraInfo);
    }
    virtual void unmarshal(const sox::Unpack &up) {
        msg = up.pop_varstr32();
        appData = up.pop_varstr32();
        sox::unmarshal_container(up, std::inserter(extraInfo, extraInfo.begin()));
    }
};



}
}
#endif /*PLOGIN_H_*/
	
