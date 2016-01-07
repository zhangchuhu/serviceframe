#ifndef PLOGIN_H_
#define PLOGIN_H_
#include <string>
#include <string.h>
#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/const.h"
#include "protocol/common/protocol/pudb.h"
#include "common/protocol/plogin.h"

namespace server {
namespace uinfo {
	struct PQueryPass : public sox::Marshallable {
		enum {uri = (129 << 8 | protocol::UINFO_SVID)};
		uint32_t uid;
		std::string sha1Pass;
		uint32_t ip;
		PQueryPass(uint32_t u, const std::string &ps) :
		uid(u), sha1Pass(ps) {
		}
		PQueryPass() {
		}
		virtual void marshal(sox::Pack &pk) const {
			pk << uid << sha1Pass << ip;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> uid >> sha1Pass >> ip;
		}
	};

	struct PQueryPassRes : public sox::Marshallable {
		enum {uri = (129 << 8 | protocol::UINFO_SVID)};
		uint32_t uid;
		uint16_t res;
		std::string passport;
		virtual void marshal(sox::Pack &pk) const {
			pk << uid << res << passport;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> uid >> res >> passport;
		}
	};


	// Check if the user is legal.
	struct PDynamicCookieCheck : public protocol::login::PUpdateCookie
	{
		enum { uri = (400 << 8) | protocol::UINFO_SVID};

		PDynamicCookieCheck() : PUpdateCookie() {}
		PDynamicCookieCheck(const protocol::login::PUpdateCookie& o) : PUpdateCookie(o) {}
	};

	// Reponse when the user is legal, else no reponse.
	struct PDynamicCookieCheckRes : public protocol::login::PUpdateCookie
	{
		enum { uri = (401 << 8) | protocol::UINFO_SVID};
	};



struct PMini : public sox::Marshallable {
	uint32_t uid;
	std::string nick;
	protocol::uinfo::SEX gender;
	virtual void marshal(sox::Pack &pk) const {
		pk << uid << nick;
		pk.push_uint8(gender);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> nick;
		gender = (protocol::uinfo::SEX)up.pop_uint8();
	}
};
struct PCheckUserExist: public sox::Marshallable{
	enum { uri = (134 << 8 | protocol::UINFO_SVID) };
	uint32_t uid;              
	uint32_t chkId;
	virtual void marshal(sox::Pack &pk) const {
		pk << uid << chkId; 
	}   
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> chkId; 
	}
	virtual std::ostream & trace(std::ostream & os) const
	{ return os << "class: PCheckUserExist" <<std::endl
			<<"uid: " <<uid <<std::endl <<"chkId: " <<chkId <<std::endl; }
};

struct PCheckUserRes: public PCheckUserExist{
	std::string nick;
	uint32_t resCode;
	protocol::uinfo::SEX gender;
	virtual void marshal(sox::Pack &pk) const {
		PCheckUserExist::marshal(pk);
		pk << nick << resCode;
		pk.push_uint16(gender);
	}   
	virtual void unmarshal(const sox::Unpack &up) {
		PCheckUserExist::unmarshal(up);
		up >> nick >> resCode;
		gender = (protocol::uinfo::SEX)up.pop_uint16();
	}
	virtual std::ostream & trace(std::ostream & os) const
	{ return os << "class: PCheckUserRes" <<std::endl
			<<"uid: " <<uid <<std::endl <<"chkId: " <<chkId <<std::endl
			<<"nick: "<<nick <<std::endl <<"resCode: "<<resCode <<std::endl
			<<"gender: " <<((int)gender) <<std::endl; }
};

}

namespace udb {
        struct PUdbGetAndCheckUInfo: public sox::Marshallable
       {   
               enum {uri = (11 << 8 | protocol::UDB_SVID)};
               uint32_t ver;
               uint32_t uid;
               std::string passport;
               std::string password;
               std::string m_contex;

               PUdbGetAndCheckUInfo(): ver(0), uid(0){}

               virtual void marshal(sox::Pack &pk) const {
                       pk << ver << uid << passport << password << m_contex;
               }   
               virtual void unmarshal(const sox::Unpack &up){
                       up >> ver >> uid >> passport >> password >> m_contex;
               }   
       };

	struct PUdbPassword: public uinfo::PQueryPass{
		enum {uri = (1 << 8 | protocol::UDB_SVID)};
		std::string passport;
		std::string sha1Pass;
		uint32_t uid;
		uint32_t ip;
		std::string udb;
		bool bEmail;
		virtual void marshal(sox::Pack &pk) const {
			pk << passport << sha1Pass << uid << ip << udb << bEmail;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> passport >> sha1Pass >> uid >> ip >> udb >> bEmail;
		}
	};

	struct PUdbPasswordCtx: public uinfo::PQueryPass{
		enum {uri = (2 << 8 | protocol::UDB_SVID)};
		std::string passport;
		std::string sha1Pass;
		uint32_t uid;
		uint32_t taskId;

		std::string udb;
		bool bEmail;
		uint32_t ip;

		PUdbPasswordCtx() : ip(0) {}
		virtual void marshal(sox::Pack &pk) const {
			pk << passport << sha1Pass << uid << taskId << udb << bEmail << ip;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> passport >> sha1Pass >> uid >> taskId >> udb >> bEmail;
			if (up.size() >= 4)
				up >> ip;
		}
	};

	struct PRegisterDuowanUdbIp : public protocol::udb::PRegisterDuowanUdb
	{
		enum {uri = (6 << 8) | protocol::UDB_SVID};

		uint32_t ip;

		PRegisterDuowanUdbIp() {}

		explicit PRegisterDuowanUdbIp(const PRegisterDuowanUdb& pk)
		{
			user_name = pk.user_name;
			passwd = pk.passwd;
			email = pk.email;
			refer = pk.refer;
			client_mac = pk.client_mac;
		}

		virtual void marshal(sox::Pack &pk) const {
			protocol::udb::PRegisterDuowanUdb::marshal(pk);
			pk << ip;
		}

		virtual void unmarshal(const sox::Unpack &up) {
			protocol::udb::PRegisterDuowanUdb::unmarshal(up);
			up >> ip;
		}
	};

	////////////////////////////////////
	struct PCreateUserIdByPassportRequest: public sox::Marshallable
	{
		enum {uri = (7 << 8 | protocol::UDB_SVID)};
		std::string m_contex;
		std::string passport;
		virtual void marshal(sox::Pack &pk) const {
			pk << m_contex << passport;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> m_contex >> passport;
		}
	};
	struct PCreateUserIdByPassportResp: public sox::Marshallable
	{
		enum {uri = (8 << 8 | protocol::UDB_SVID)};
		std::string m_contex;
		uint32_t ret_value;
		uint64_t user_id;
		virtual void marshal(sox::Pack &pk) const {
			pk.push_varstr32(m_contex.c_str(),m_contex.size());
			pk << ret_value;
			pk << user_id ;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			m_contex=up.pop_varstr32();
			up >> ret_value;
			up >> user_id;
		}
	};
	/////////////////////////////////////////////
	struct PCreateSubSessionIDRequest: public sox::Marshallable
	{
		enum {uri = (9 << 8 | protocol::UDB_SVID)};
		std::string m_contex;
		uint32_t which;
		std::string m_info;
		virtual void marshal(sox::Pack &pk) const {
			pk.push_varstr32(m_contex.c_str(),m_contex.size());
			pk << which;
			pk << m_info;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			m_contex=up.pop_varstr32();
			up >> which;
			up >> m_info;
		}
	};
	struct PCreateSubSessionIDResp: public sox::Marshallable
	{
		enum {uri = (10 << 8 | protocol::UDB_SVID)};
		std::string m_contex;
		uint32_t ret_value;
		uint64_t subses_id;
		virtual void marshal(sox::Pack &pk) const {
			pk.push_varstr32(m_contex.c_str(),m_contex.size());
			pk << ret_value;
			pk << subses_id ;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			m_contex=up.pop_varstr32();
			up >> ret_value;
			up >> subses_id;
		}
	};
	struct ActivateNewUserReq : public sox::Marshallable
	{
		enum {uri = (20 << 8) | protocol::UDB_SVID};
		std::string email;
		std::string passport;
		std::string sha1Passwd;
		uint32_t ip;
		uint32_t contextId;
		
		ActivateNewUserReq() : ip(0), contextId(0) {}
		virtual void marshal(sox::Pack& pk)	const {
			pk << email << passport << sha1Passwd << ip << contextId;
		}
		virtual void unmarshal(const sox::Unpack& up) {
			up >> email >> passport >> sha1Passwd >> ip >> contextId;
		}
	};

	struct ActivateNewUserRes : public sox::Marshallable
	{
		enum {uri = (21 << 8) | protocol::UDB_SVID};
		uint32_t userId;
		uint16_t resCode;	// RES_EUNKNOWN, RES_SUCCESS
		uint32_t contextId;
		
		ActivateNewUserRes() : userId(0), resCode(RES_SUCCESS), contextId(0) {}
		virtual void marshal(sox::Pack& pk)	const {
			pk << userId << resCode << contextId;
		}
		virtual void unmarshal(const sox::Unpack& up) {   
			up >> userId >> resCode >> contextId;
        }
	};


	struct ActivateYyUserReq : public sox::Marshallable
	{
		enum {uri = (22 << 8) | protocol::UDB_SVID};

		uint32_t uid;
		uint16_t type;

		ActivateYyUserReq() : uid(0), type(0) {}
		virtual void marshal(sox::Pack& pk)	const {
			pk << uid << type ;
		}
		virtual void unmarshal(const sox::Unpack& up) {
			up >> uid >> type;
		}
	};

}

namespace linkd {
struct PInnerKickTry : public sox::Marshallable {
	enum {uri = (130 << 8 | protocol::LINKD_SVID)};
	uint32_t uid;
	virtual void marshal(sox::Pack &pk) const {
		pk << uid;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid;
	}
};

struct PUserWillLogin : public sox::Marshallable {
	enum {uri = (131 << 8 | protocol::LINKD_SVID)};
	PUserWillLogin(){}
	PUserWillLogin(uint32_t u) : uid(u) {}

	uint32_t uid;

	virtual void marshal(sox::Pack &pk) const {
		pk << uid;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid;
	}
};

struct PAllocBuck : public sox::Marshallable {
	enum {uri = (132 << 8 | protocol::LINKD_SVID)};
	PAllocBuck(){}
	PAllocBuck(uint32_t buck) : bucket(buck) {}

	uint32_t bucket;

	virtual void marshal(sox::Pack &pk) const {
		pk << bucket;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> bucket;
	}
};

// TODO(lzd): to be deleleted
struct PKickAfterOnlineDbCheck : public sox::Marshallable {
	enum {uri = (133 << 8 | protocol::LINKD_SVID)};
	uint32_t uid;
	virtual void marshal(sox::Pack &pk) const {
		pk << uid;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid;
	}
};

// PKickAfterOnlineDbCheck基础上加上被封、解封时间和被封原因
struct PKickAfterOnlineDbCheck2 : public sox::Marshallable {
	enum {uri = (137 << 8 | protocol::LINKD_SVID)};
	uint32_t uid;

	std::string banDate;	// 被封时间
	std::string unbanDate;	// 解封时间
	std::string note;		// 备注

	virtual void marshal(sox::Pack &pk) const {
		pk << uid << banDate << unbanDate << note;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> banDate >> unbanDate >> note;
	}
};

struct PKickAfterFreeze : public sox::Marshallable {
	enum {uri = (135 << 8 | protocol::LINKD_SVID)};
	uint32_t uid;
	uint32_t freeTime;
	KickType kType;
	virtual void marshal(sox::Pack &pk) const {
		pk << uid << freeTime;
		pk.push_uint8(kType);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> freeTime;
		kType = up.pop_uint8() == KICKOUT_CHANNEL ? KICKOUT_CHANNEL : LOGIN_FREEZED;
	}
};

  struct PCongestedRegister: public sox::Marshallable{
		enum {uri = (150 << 8 | protocol::LINKD_SVID)};
   	uint32_t taskId;
		uint32_t serverId; //LinkD serverId
		uint32_t msgURI;
		uint32_t interval;
		uint32_t limit;
		bool enable; // if msgURI == 0, enable/disable all the messages congestion control		
    
		virtual void marshal(sox::Pack &pk) const {
			pk << taskId << serverId << msgURI << interval << limit << enable;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> taskId >> serverId >> msgURI >> interval >> limit >> enable;
		}
	};

  struct PCongestedRegisterRes: public sox::Marshallable{
		enum {uri = (151 << 8 | protocol::LINKD_SVID)};
   	uint32_t taskId;
		uint32_t serverId; //LinkD serverId
		uint32_t msgURI;
		uint32_t interval;
	
		virtual void marshal(sox::Pack &pk) const {
			pk  << taskId << serverId << msgURI << interval;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> taskId >> serverId >> msgURI >> interval;
		}
	};	

  struct PCongestedUnregister: public sox::Marshallable{
		enum {uri = (152 << 8 | protocol::LINKD_SVID)};
   	uint32_t taskId;
		uint32_t serverId; //LinkD serverId		
		uint32_t msgURI;
		uint32_t interval;
	
		virtual void marshal(sox::Pack &pk) const {
			pk  << taskId << serverId << msgURI << interval;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> taskId >> serverId >> msgURI >> interval;
		}
	};

  struct PCongestedUnregisterRes: public sox::Marshallable{
		enum {uri = (153 << 8 | protocol::LINKD_SVID)};
   	uint32_t taskId;
		uint32_t serverId; //LinkD serverId		
		uint32_t msgURI;
		uint32_t interval;
	
		virtual void marshal(sox::Pack &pk) const {
			pk << taskId << serverId << msgURI << interval;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> taskId >> serverId  >> msgURI >> interval;
		}
	};		

    struct PLocalSmgrGroupAdd: public sox::Marshallable{
		enum {uri = (154 << 8 | protocol::LINKD_SVID)};
   	uint32_t taskId;
		uint32_t serverId; //LinkD serverId
		uint32_t localSmgrGroup;
		uint32_t reserved;
    
		virtual void marshal(sox::Pack &pk) const {
			pk << taskId << serverId << localSmgrGroup << reserved;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> taskId >> serverId >> localSmgrGroup>> reserved;
		}
	};

  struct PLocalSmgrGroupAddRes: public sox::Marshallable{
		enum {uri = (155 << 8 | protocol::LINKD_SVID)};
   	uint32_t taskId;
		uint32_t serverId; //LinkD serverId
		uint32_t localSmgrGroup;
		uint32_t reserved;
	
		virtual void marshal(sox::Pack &pk) const {
			pk  << taskId << serverId << localSmgrGroup << reserved;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> taskId >> serverId >> localSmgrGroup >> reserved;
		}
	};	

   struct PLocalSmgrGroupRemove: public sox::Marshallable{
		enum {uri = (156 << 8 | protocol::LINKD_SVID)};
   	uint32_t taskId;
		uint32_t serverId; //LinkD serverId
		uint32_t localSmgrGroup;
		uint32_t reserved;
    
		virtual void marshal(sox::Pack &pk) const {
			pk << taskId << serverId << localSmgrGroup << reserved;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> taskId >> serverId >> localSmgrGroup>> reserved;
		}
	};

  struct PLocalSmgrGroupRemoveRes: public sox::Marshallable{
		enum {uri = (157 << 8 | protocol::LINKD_SVID)};
   	uint32_t taskId;
		uint32_t serverId; //LinkD serverId
		uint32_t localSmgrGroup;
		uint32_t reserved;
	
		virtual void marshal(sox::Pack &pk) const {
			pk  << taskId << serverId << localSmgrGroup << reserved;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> taskId >> serverId >> localSmgrGroup >> reserved;
		}
	};	

  struct PLocalSmgrPing : public sox::Marshallable {
    enum {uri = (158 << 8 | protocol::LINKD_SVID)};
	  uint32_t pingId;
    uint32_t targetServerId;
    PLocalSmgrPing()
      : pingId(0)
		  , targetServerId(0)
    {}

    virtual void marshal(sox::Pack &p) const {
      p << pingId << targetServerId;
    }
    virtual void unmarshal(const sox::Unpack &p) {
      p >> pingId >> targetServerId;
    }
  };

  struct PLocalSmgrPong : public sox::Marshallable {
    enum {uri = (159 << 8 | protocol::LINKD_SVID)};
	  uint32_t pingId;
    uint32_t targetServerId;
    uint32_t serverId;
    bool isValid;
    PLocalSmgrPong()
      : pingId(0)
		  , targetServerId(0)
      , serverId(0)
      , isValid(true)
    {}

    virtual void marshal(sox::Pack &p) const {
      p << pingId << targetServerId << serverId << isValid;
    }
    virtual void unmarshal(const sox::Unpack &p) {
      p >> pingId >> targetServerId >> serverId >> isValid;
    }
  };

  struct PLinkdConfigWrapper : public sox::Marshallable {
    enum {uri = (160 << 8 | protocol::LINKD_SVID)};
    std::string context;
    uint32_t configServerId;
    uint32_t seq;
    uint32_t msgUri; // uri of marshallable in msgPack
    std::string msgPack; // no HEADER
    
    PLinkdConfigWrapper()
		  : configServerId(0), seq(0), msgUri(0)
    {}

    virtual void packLoad(uint32_t uri, const sox::Marshallable & msg) {
      msgUri = uri;
      sox::PacketToString(msg, msgPack);
    }

    virtual bool packUnload(uint32_t & uri, sox::Marshallable & msg) const{
      uri = msgUri;
      return sox::StringToPacket(msgPack, msg);
    }

    virtual void marshal(sox::Pack &p) const {
      p << context << configServerId << seq << msgUri;
      p.push_varstr32(msgPack.data(), msgPack.size());
    }
    virtual void unmarshal(const sox::Unpack &p) {
      p >> context >> configServerId >> seq >> msgUri;
      msgPack = p.pop_varstr32();
    }
    virtual std::ostream & trace(std::ostream & os) const
    {
      os << "ctx#=" << context.size() << ",";
      os << "csid=" << configServerId << ",";
      os << "seq=" << seq << ",";
      os << "msg#=" << msgUri << "," << msgPack.size() << ";";   
      return os;  
    }      
  };

  struct PLinkdConfigWrapperRes : public sox::Marshallable {
    enum {uri = (161 << 8 | protocol::LINKD_SVID)};
    std::string context;
    uint32_t linkdServerId;
    uint32_t seq;
    uint32_t res;
    
    PLinkdConfigWrapperRes()
		  : linkdServerId(0)
      , seq(0)
      , res(0)
    {}

    virtual void marshal(sox::Pack &p) const {
      p << context << linkdServerId << seq << res;
    }
    virtual void unmarshal(const sox::Unpack &p) {
      p >> context >> linkdServerId >> seq >> res;
    }
    virtual std::ostream & trace(std::ostream & os) const
    {
      os << "ctx#=" << context.size() << ",";
      os << "lsid=" << linkdServerId << ",";
      os << "seq=" << seq << ",";
      os << "res=" << res << ";";   
      return os;  
    } 

  };

  struct PLinkdConfigSetProperty : public sox::Marshallable {
    enum {uri = (162 << 8 | protocol::LINKD_SVID)};
	  typedef std::map<std::string, std::string> PROPERTY_TYPE;
	  PROPERTY_TYPE property;
    
    virtual void marshal(sox::Pack &p) const {
      sox::marshal_container(p, property);
    }
    virtual void unmarshal(const sox::Unpack &p) {
      sox::unmarshal_container(p, std::inserter(property, property.begin()));
    }
    virtual std::ostream & trace(std::ostream & os) const
    {
      os << "p#=" << property.size() << "{";
      for(PROPERTY_TYPE::const_iterator it = property.begin(); 
        it != property.end(); ++it)
      {
        os << it->first << "=" << it->second << ",";
      }
      os << "};";
      return os;  
    }      
  };

  struct PLinkdConfigRequestProperty : public sox::Marshallable {
    enum {uri = (163 << 8 | protocol::LINKD_SVID)};
    uint32_t linkdServerId;
    
    PLinkdConfigRequestProperty()
		  : linkdServerId(0)
    {}

    virtual void marshal(sox::Pack &p) const {
      p << linkdServerId;
    }
    virtual void unmarshal(const sox::Unpack &p) {
      p >> linkdServerId;
    }
    virtual std::ostream & trace(std::ostream & os) const
    {
      os << "lsid=" << linkdServerId << ",";
      return os;  
    } 

  };

  struct PInnerKickTry2 : public sox::Marshallable {
    enum {uri = (168 << 8 | protocol::LINKD_SVID)};
    uint32_t uid;
    std::string client_from;
    virtual void marshal(sox::Pack &pk) const {
      pk << uid << client_from;
    }
    virtual void unmarshal(const sox::Unpack &up) {
      up >> uid >> client_from;
    }
  };

  struct PNotifyMasterLinkdInfo: public sox::Marshallable
  {
    enum {uri = (302 << 8) | protocol::LINKD_SVID};
    PNotifyMasterLinkdInfo (){}

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

  struct PRequestUserAuthInfo: public sox::Marshallable
  {
    enum {uri = (305 << 8) | protocol::LINKD_SVID};
    PRequestUserAuthInfo (){}

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

  struct PRequestUserAuthInfoRes: public sox::Marshallable
  {
    enum {uri = (306 << 8) | protocol::LINKD_SVID};
    PRequestUserAuthInfoRes (){}

    uint32_t uid;
    uint32_t ip;
    uint32_t serverId;
    bool res;

    virtual void marshal(sox::Pack &pk) const {
      pk << uid << ip << serverId << res;
    }
    virtual void unmarshal(const sox::Unpack &up) {
      up >> uid >> ip >> serverId >> res;
    }
  };

  struct PCheckOnlineUser: public sox::Marshallable
  {
    enum {uri = (309 << 8) | protocol::LINKD_SVID};
    PCheckOnlineUser (){}

    uint32_t ip;
    uint32_t serverId;
    std::vector<uint32_t> uids;

    virtual void marshal(sox::Pack &pk) const {
      pk << ip << serverId;
      marshal_container(pk, uids);
    }
    virtual void unmarshal(const sox::Unpack &up) {
      up >> ip >> serverId;
      unmarshal_container(up, std::back_inserter(uids));
    }
  };

  struct UserAuthRes: public sox::Marshallable
  {
    UserAuthRes() {}
    UserAuthRes(uint32_t u, bool r): uid(u), res(r) {}
    uint32_t uid;
    bool res;
    virtual void marshal(sox::Pack &pk) const {
      pk << uid << res;
    }
    virtual void unmarshal(const sox::Unpack &up) {
      up >> uid >> res;
    }
  };

  struct PCheckOnlineUserRes: public sox::Marshallable
  {
    enum {uri = (310 << 8) | protocol::LINKD_SVID};
    PCheckOnlineUserRes (){}

    uint32_t ip;
    uint32_t serverId;
    std::vector<UserAuthRes> authRes;

    virtual void marshal(sox::Pack &pk) const {
      pk << ip << serverId;
      marshal_container(pk, authRes);
    }
    virtual void unmarshal(const sox::Unpack &up) {
      up >> ip >> serverId;
      unmarshal_container(up, std::back_inserter(authRes));
    }
  };

  struct UserOnlineInfo : public sox::Marshallable
  {
    // info["uid"] = "123456";
    // info["userIp"] = "2346123951";
    // info["clientFrom"] = "yy";
    // info["macAddr"] = "140FAC348910";
    std::map <std::string, std::string> info;
    virtual void marshal(sox::Pack &pk) const {
      marshal_container(pk, info);
    }
    virtual void unmarshal(const sox::Unpack &up) {
      unmarshal_container(up, std::inserter(info, info.begin()));
    }    
  };

  struct PGetAllUserOnlineInfo: public sox::Marshallable
  {
    enum {uri = (500 << 8) | protocol::LINKD_SVID};

    std::string context;

    virtual void marshal(sox::Pack &pk) const {
      pk << context;
    }
    virtual void unmarshal(const sox::Unpack &up) {
      up >> context;
    }
  };

  struct PGetAllUserOnlineInfoRes: public sox::Marshallable
  {
    enum {uri = (501 << 8) | protocol::LINKD_SVID};

    PGetAllUserOnlineInfoRes():time(0){}

    std::string context;
    std::vector<UserOnlineInfo> userOnlineInfos;
    uint32_t time;

    virtual void marshal(sox::Pack &pk) const {
      pk << context;
      marshal_container(pk, userOnlineInfos);
      pk << time;
    }
    virtual void unmarshal(const sox::Unpack &up) {
      up >> context;
      unmarshal_container(up, std::back_inserter(userOnlineInfos));
      if(!up.empty())
      {
        up >> time;
      }
    }
  };

  struct PGetUserOnlineInfo: public sox::Marshallable
  {
    enum {uri = (502 << 8) | protocol::LINKD_SVID};
    
    std::string context;
    uint32_t time;

    virtual void marshal(sox::Pack &pk) const {
      pk << context << time;
    }
    virtual void unmarshal(const sox::Unpack &up) {
      up >> context >> time;
    }
  };

  struct PGetUserOnlineInfoRes: public sox::Marshallable
  {
    enum {uri = (503 << 8) | protocol::LINKD_SVID};
    
    std::string context;
    uint32_t time; // unix timestamp
    std::vector<UserOnlineInfo> adds;
    std::vector<UserOnlineInfo> removes;
    uint32_t period; // 统计周期(单位: 分钟) ( 本次返回的数据统计间隔:[time, time+period*60) )
    uint32_t resCode; // RES_SUCCESS - 成功；RES_ENONEXIST_REAL - 数据已清除

    virtual void marshal(sox::Pack &pk) const {
      pk << context << time;
      marshal_container(pk, adds);
      marshal_container(pk, removes);
      pk << period << resCode;
    }
    virtual void unmarshal(const sox::Unpack &up) {
      up >> context >> time;
      unmarshal_container(up, std::back_inserter(adds));
      unmarshal_container(up, std::back_inserter(removes));
      up >> period >> resCode;
    }
  };

  typedef ZipMarshal<PGetAllUserOnlineInfoRes, 504 << 8 | protocol::LINKD_SVID> PGetAllUserOnlineInfoZipRes;
  typedef ZipMarshal<PGetUserOnlineInfoRes, 505 << 8 | protocol::LINKD_SVID> PGetUserOnlineInfoZipRes;

  struct ComplementaryLinkds :public sox::Marshallable2
  {
    ComplementaryLinkds() : masterLinkdIp(0) {}
    ComplementaryLinkds(uint32_t u, uint32_t i, uint32_t ip1, uint32_t ip2, const std::string& p, bool r, uint32_t mip)
      : uid(u), clientIp(i), linkdIp1(ip1), linkdIp2(ip2), passport(p), res(r), masterLinkdIp(mip) {}
    uint32_t uid;
    uint32_t clientIp;
    uint32_t linkdIp1;
    uint32_t linkdIp2;
    std::string passport;
    bool res;
    uint32_t masterLinkdIp;

    virtual void marshal2(sox::Pack &pk) const {
      pk << uid << clientIp << linkdIp1 << linkdIp2 << passport << res << masterLinkdIp;
    }
    virtual void unmarshal2(const sox::Unpack &up) {
      up >> uid >> clientIp >> linkdIp1 >> linkdIp2 >> passport >> res;
      if (!up.empty())
      {
        up >> masterLinkdIp;
      }
    }
  };

  struct PRequestLinkdRecord:public sox::Marshallable{
    enum {uri = (600 << 8) | protocol::LINKD_SVID};
    uint32_t time;
    uint32_t ip;
    std::vector<ComplementaryLinkds> respRecord;

    virtual void marshal(sox::Pack &pk) const
    {
      pk << time << ip;
      sox::marshal_container(pk, respRecord);
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
      up >> time >> ip;
      sox::unmarshal_container(up, std::back_inserter(respRecord));
    }
  };
}

namespace lbs{

	struct PGetLoginCache: public sox::Marshallable{
		enum {uri = (9 << 8 | protocol::LBS_SVID)};
		uint32_t taskId;			//上下文id, lbs需要
		std::string userUnqSign;	//用户标识，email 或者 通行证
		bool bEmail;				// true: email; false: 通行证
		virtual void marshal(sox::Pack &pk) const {
			pk << taskId << userUnqSign;
			pk.push_uint8(bEmail);
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> taskId >> userUnqSign;
			bEmail = up.pop_uint8() != 0;
		}	
	};

	struct PGetLoginCacheRes: public sox::Marshallable{
		enum {uri = (9 << 8 | protocol::LBS_SVID)};
		uint32_t	taskId; //PGetLoginCache 中传入的原样返回。
		//PSetLoginCache 中 payload
		uint32_t	uid;
		std::string passwd;
		
		uint16_t resCode;		//resCode == 200 命中缓存， 404 结果不存在， 其它错误 500
		virtual void marshal(sox::Pack &pk) const {
			pk << taskId << uid << passwd << resCode;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> taskId >> uid >> passwd >> resCode;
		}	
	};

	struct PSetLoginCache: public sox::Marshallable{
		enum {uri = (10 << 8 | protocol::LBS_SVID)};
		//key
		std::string userUnqSign;
		bool bEmail;

		//payload
		uint32_t	uid;
		std::string passwd;

		virtual void marshal(sox::Pack &pk) const {
			pk << userUnqSign << uid << passwd;
			pk.push_uint8(bEmail);
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> userUnqSign >> uid >> passwd;
			bEmail = up.pop_uint8() != 0;
		}	
	};

	struct PProcLogin_Res3: public sox::Marshallable{
		enum {uri = (200 << 8) | protocol::LBS_SVID};

		PProcLogin_Res3(){}
		PProcLogin_Res3(uint32_t res, const std::string &pass, uint32_t id, uint32_t tid, uint32_t fail_times, uint32_t ip_fail_times, uint32_t err): 
		resCode(res), passwd(pass), userId(id), taskId(tid), account_try_times(fail_times), ip_try_times(ip_fail_times), error_type(err) {}
		uint32_t resCode;
		std::string passwd;
		uint32_t userId;
		uint32_t taskId;
		uint32_t account_try_times;
		uint32_t ip_try_times;
		uint32_t error_type;	// 0-无异常，1-需要显示验证码，2-需要冻结1小时



		virtual void marshal(sox::Pack &pk) const {
			pk << resCode <<  passwd << userId << taskId << account_try_times << ip_try_times << error_type;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> resCode >>  passwd >> userId >> taskId >> account_try_times >> ip_try_times >> error_type;
		}
	};
	struct PUserCreatedCtx3: public sox::Marshallable{
		enum {uri = (201 << 8) | protocol::LBS_SVID};
		std::string udb;
		std::string sha1Passwd;
		uint32_t ip;
		bool bEmail;
		uint32_t taskId;

		uint32_t userId;
		uint16_t resCode;
		uint32_t account_try_times;
		uint32_t ip_try_times;
		uint32_t error_type;
		
		PUserCreatedCtx3() : ip(0), bEmail(false), taskId(0) {}
		
		virtual void marshal(sox::Pack &pk) const {
//			PNewUserCtx::marshal(pk);
			pk << udb << sha1Passwd << ip << bEmail << taskId << userId << resCode << account_try_times << ip_try_times << error_type;
		}
		virtual void unmarshal(const sox::Unpack &up) {
//			PNewUserCtx::unmarshal(up);
			up >> udb >> sha1Passwd >> ip >> bEmail >> taskId >> userId >> resCode >> account_try_times >> ip_try_times >> error_type;
		}
	};
	
	struct PQueryIpIspReq: public sox::Marshallable{
		enum {uri = (120 << 8 | protocol::LBS_SVID)};
		uint32_t ip;
	
		virtual void marshal(sox::Pack &pk) const {
			pk << ip;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> ip;
		}
	};

	struct PQueryIpIspRes: public sox::Marshallable{
		enum {uri = (121 << 8 | protocol::LBS_SVID)};
		uint16_t ispType;
	
		virtual void marshal(sox::Pack &pk) const {
			pk << ispType;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> ispType;
		}
	};

	struct PQueryIpIspRes2: public sox::Marshallable{
		enum {uri = (122 << 8 | protocol::LBS_SVID)};
		uint16_t ispType;
		uint16_t areaType;
	
		virtual void marshal(sox::Pack &pk) const {
			pk << ispType << areaType;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> ispType >> areaType;
		}
	};

  
}
}
#endif /*PLOGIN_H_*/
