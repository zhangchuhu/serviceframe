//#ifndef PROTOCOL_DAEMON_H_
//#define PROTOCOL_DAEMON_H_
#pragma  once
#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/const.h"
#include "server_common/server-lib/uuid.h"
#include <vector>

namespace sdaemon {
	enum ServerStatus
	{
		INITIAL,
		WAITING,
		READY,
		RUNNING,
	};

	struct FavorStr : public sox::Marshallable {
		std::string sstr;
		uint32_t dip;//电信IP
		uint32_t wip;//网通IP
		uint16_t port;
		uint32_t serverId;

		//uint32_t yourType;
		bool operator == (const FavorStr &right) const{
			return (sstr == right.sstr && dip == right.dip 
				&& wip == right.wip && port == right.port 
				&& serverId == right.serverId);
		}
		virtual void marshal(sox::Pack &pk) const {
			pk << sstr;
			pk.push_uint32(dip).push_uint32(wip);
			pk << serverId << port;
			//pk << yourType;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> sstr;
			dip = up.pop_uint32();
			wip = up.pop_uint32();
			up >> serverId >> port;
			//up >> yourType;
		}
	};

	struct FavorStrSV : public sox::Marshallable {
		std::string sstr;
		uint32_t dip;
		uint32_t wip;
		uint16_t port;
		uint32_t serverId;

		uint32_t			groupId;		// 组id
//		core::ServerStatus	serverStatus;	// 进程（服务器）状态
//		uint16_t serverStatus;	// 进程（服务器）状态
		ServerStatus serverStatus;
		//uint32_t yourType;
		bool operator == (const FavorStrSV &right) const{
			return (sstr == right.sstr && dip == right.dip 
				&& wip == right.wip && port == right.port 
				&& serverId == right.serverId && groupId == right.groupId
				&& serverStatus == right.serverStatus);
		}
		virtual void marshal(sox::Pack &pk) const {
			pk << sstr;
			pk.push_uint32(dip).push_uint32(wip);
			pk << serverId << port;
			pk << groupId;
			pk.push_uint32(serverStatus);
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> sstr;
			dip = up.pop_uint32();
			wip = up.pop_uint32();
			up >> serverId >> port;
			up >> groupId;
			serverStatus = (ServerStatus)up.pop_uint32();
		}
	};


struct PRegisterServerRes : public sox::Marshallable {
	enum {uri = 1 << 8 | protocol::DAEMON_SVID};
	uint32_t serverId;
	std::string dip;
	std::string wip;
	uint32_t yourType;
	core::MUUID uuid;
	virtual void marshal(sox::Pack &pk) const {
		pk << serverId << dip << wip << uuid;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> serverId >> dip >> wip >> uuid;
	}
};

struct PServerStart : public sox::Marshallable {
	enum {uri = 2 << 8 | protocol::DAEMON_SVID};
	uint32_t serverId;
	uint16_t port;
	virtual void marshal(sox::Pack &pk) const {
		pk << serverId << port;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> serverId >> port;
	}
};	


struct PServerRemove : public sox::Marshallable {
	enum {uri = 4 << 8 | protocol::DAEMON_SVID};

	uint16_t serverId;

	virtual void marshal(sox::Pack &pk) const {
		pk << serverId;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> serverId;
	}
};

struct PServerData : public sox::Marshallable{
	uint32_t ip;
	int port;

	virtual void marshal(sox::Pack &pk) const {
		pk.push_uint32(ip);
		pk.push_uint32(port);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		ip = (uint32_t)up.pop_uint32();
		port = (int)up.pop_uint32();
	}
};

struct PGetServerDataRes : public sox::Marshallable{
	enum {uri = 5<<8 | protocol::DAEMON_SVID};
	std::vector<PServerData> serverdata;

	virtual void marshal(sox::Pack &pk) const {
		sox::marshal_container(pk, serverdata);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		sox::unmarshal_container(up, std::back_inserter(serverdata));
	}
};


struct PRegisterFavour: public sox::Marshallable{
	enum {uri = 6 << 8 | protocol::DAEMON_SVID};
	std::vector<uint16_t> favours;

	virtual void marshal(sox::Pack &pk) const {
		sox::marshal_container(pk, favours);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		sox::unmarshal_container(up, std::back_inserter(favours));
	}
};


struct PServerHangUp: public sox::Marshallable{
	enum {uri = 7<<8 | protocol::DAEMON_SVID};
	uint32_t serverid;
	virtual void marshal(sox::Pack &pk) const {
		pk << serverid;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> serverid;
	}
};

struct PServerWakeUp: public sox::Marshallable{
	enum {uri = 8<<8 | protocol::DAEMON_SVID};
	uint32_t serverid;
	virtual void marshal(sox::Pack &pk) const {
		pk << serverid;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> serverid;
	}

};

struct PRegisterStr : public sox::Marshallable {
	enum {uri = 10 << 8 | protocol::DAEMON_SVID};
	std::string sstr;
	std::vector<uint16_t> ports;
	virtual void marshal(sox::Pack &pk) const {
		pk << sstr;
		sox::marshal_container(pk, ports);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> sstr;
		sox::unmarshal_container(up, std::back_inserter(ports));
	}
};

struct PRegisterFavourStr: public sox::Voidmable{
	enum {uri = 11 << 8 | protocol::DAEMON_SVID};
};

struct PRegisterFavourStrRes: public sox::Marshallable{
	enum {uri = 11 << 8 | protocol::DAEMON_SVID};
	std::string dip;
	std::string wip;
	//uint16_t type;
	std::vector<FavorStr> favours; 	
	virtual void marshal(sox::Pack &pk) const {
		pk << dip << wip;
		sox::marshal_container(pk, favours);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> dip >> wip;
		sox::unmarshal_container(up, std::back_inserter(favours));
	}
};


struct PServerAddStr : public FavorStr {
	enum {uri = 12 << 8 | protocol::DAEMON_SVID};

};

struct PQueryPortRes: public sox::Marshallable{
	enum {uri = 13 << 8 | protocol::DAEMON_SVID};
	uint32_t serverId;
	std::vector<uint16_t> ports;
	virtual void marshal(sox::Pack &pk) const {
		pk << serverId;
		sox::marshal_container(pk, ports);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> serverId;
		sox::unmarshal_container(up, std::back_inserter(ports));
	}
};

struct PServerPing: public sox::Marshallable{
	enum {uri = 14 << 8 | protocol::DAEMON_SVID};
	uint32_t serverId;
	virtual void marshal(sox::Pack &pk) const {
		pk << serverId;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> serverId;
	}
};

struct PServerRecover: public sox::Marshallable{
	enum {uri = 15 << 8 | protocol::DAEMON_SVID};
	uint32_t serverId;
	uint16_t port;
	std::string name;
	core::MUUID uuid;
	virtual void marshal(sox::Pack &pk) const {
		pk << serverId << port << name << uuid;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> serverId >> port >> name >> uuid;
	}

};

struct PLoadReport: public sox::Marshallable{
	enum {uri = 16 << 8 | protocol::DAEMON_SVID};
	uint32_t serverId;
	uint32_t loadValue;
	uint64_t total;

	virtual void marshal(sox::Pack &pk) const {
		pk << serverId << loadValue;
		pk.push_uint64(total);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> serverId >> loadValue;
		total = up.pop_uint64();
	}

	void setLoad(uint32_t sid, uint32_t l){
		serverId = sid;
		loadValue = l;
	}
};

struct PLbsDns: public sox::Marshallable{
	enum {uri = 17 << 8 | protocol::DAEMON_SVID};
	uint32_t  serverId;
	std::vector<std::pair<std::string, uint32_t> > names;

	virtual void marshal(sox::Pack &pk) const {
		pk << serverId;
		sox::marshal_container(pk, names);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> serverId;
		sox::unmarshal_container(up, std::back_inserter(names));
	}
};

struct PLbsDnsRes: public sox::Marshallable{
	enum {uri = 18 << 8 | protocol::DAEMON_SVID};
	uint32_t serverId;
	std::string name;
	virtual void marshal(sox::Pack &pk) const {
		pk << serverId << name;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> serverId >> name;
	}
};

struct PRegisterStr2 : public sox::Marshallable {
	enum {uri = 19 << 8 | protocol::DAEMON_SVID};
	std::string sstr;
	virtual void marshal(sox::Pack &pk) const {
		pk << sstr;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> sstr;
	}
};

struct PRegisterServerRes2: public PRegisterServerRes{
	enum {uri = 19 << 8 | protocol::DAEMON_SVID};
	std::vector<FavorStr> favours; 	
	virtual void marshal(sox::Pack &pk) const {
		PRegisterServerRes::marshal(pk);
		sox::marshal_container(pk, favours);
		pk << yourType;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		PRegisterServerRes::unmarshal(up);
		sox::unmarshal_container(up, std::back_inserter(favours));
		up >> yourType;
	}
};

struct PAllowIps: public sox::Marshallable{
	enum {uri = 20 << 8 | protocol::DAEMON_SVID};
	std::vector<uint32_t> allowIps;
	virtual void marshal(sox::Pack &pk) const {
		sox::marshal_container(pk, allowIps);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		sox::unmarshal_container(up, std::back_inserter(allowIps));
	}
};


struct PImLinkdPorts: public sox::Marshallable{
	enum {uri = 21 << 8 | protocol::IMLBS_SVID};
	uint32_t serverId;
	std::vector<uint16_t> ports;
	virtual void marshal(sox::Pack &pk) const {
		pk << serverId;
		sox::marshal_container(pk, ports);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> serverId;
		sox::unmarshal_container(up, std::back_inserter(ports));
	}
};

struct PRegisterStr2SV : public sox::Marshallable {
	enum {uri = 21 << 8 | protocol::DAEMON_SVID};
	std::string sstr;
	uint32_t regGroupId;
	uint16_t port;
	virtual void marshal(sox::Pack &pk) const {
		pk << sstr;
		pk << regGroupId << port;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> sstr;
		up >> regGroupId >> port;
	}
};

struct PRegisterServerResSV : public sox::Marshallable {
	enum {uri = 22 << 8 | protocol::DAEMON_SVID};
	uint32_t serverId;
	std::string dip;
	std::string wip;
	uint32_t yourType;
	core::MUUID uuid;
	uint32_t			groupId;		// 组id		
	virtual void marshal(sox::Pack &pk) const {
		pk << serverId << dip << wip << uuid;
		pk << groupId;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> serverId >> dip >> wip >> uuid;
		up >> groupId;		
	}
};

struct PRegisterServerRes2SV: public PRegisterServerResSV{
	enum {uri = 23 << 8 | protocol::DAEMON_SVID};
	std::vector<FavorStrSV> favours; 	//含全局信息, ori: only RUNNING
	virtual void marshal(sox::Pack &pk) const {
		PRegisterServerResSV::marshal(pk);
		sox::marshal_container(pk, favours);
		pk << yourType;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		PRegisterServerResSV::unmarshal(up);
		sox::unmarshal_container(up, std::back_inserter(favours));
		up >> yourType;
	}
};

struct PServerStartSV : public sox::Marshallable {
	enum {uri = 24 << 8 | protocol::DAEMON_SVID};
	uint32_t serverId;
	uint16_t port;
	ServerStatus srvRegStatus;		/*server注册时提供的注册状态*/
	
	virtual void marshal(sox::Pack &pk) const {
		pk << serverId << port;
		pk.push_uint32(srvRegStatus);	
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> serverId >> port;
		srvRegStatus = (ServerStatus)up.pop_uint32();
	}
};	

struct PServerAddStrSV : public FavorStrSV{
	enum {uri = 25 << 8 | protocol::DAEMON_SVID};

};

struct PServerRecoverSV: public sox::Marshallable{
	enum {uri = 26 << 8 | protocol::DAEMON_SVID};
	uint32_t serverId;
	uint16_t port;
	std::string name;
	core::MUUID uuid;
	uint32_t			groupId;		// 组id	
	ServerStatus serverStatus;
	uint32_t seq;						//恢复序号，防止重复恢复
	virtual void marshal(sox::Pack &pk) const {
		pk << serverId << port << name << uuid;
		pk.push_uint32(serverStatus);		
		pk << groupId << seq;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> serverId >> port >> name >> uuid;
		serverStatus = (ServerStatus)up.pop_uint32();
		up >> groupId >> seq;
	}

};

struct PRegisterFavourStrResSV: public sox::Marshallable{/*the same as PRegFavRecStrResSV, for register*/
	enum {uri = 27 << 8 | protocol::DAEMON_SVID};
	std::string dip;
	std::string wip;
	//uint16_t type;
	std::vector<FavorStrSV> favours; 	
	virtual void marshal(sox::Pack &pk) const {
		pk << dip << wip;
		sox::marshal_container(pk, favours);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> dip >> wip;
		sox::unmarshal_container(up, std::back_inserter(favours));
	}
};


struct PChangeMyServerStatusSV: public sox::Marshallable{
	enum {uri = 28 << 8 | protocol::DAEMON_SVID};
	uint32_t serverId;
	ServerStatus serverStatus;	
	virtual void marshal(sox::Pack &pk) const {
		pk << serverId;
		pk.push_uint32(serverStatus);		
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> serverId;
		serverStatus = (ServerStatus)up.pop_uint32();
	}

};

struct PServerRemoveSV : public sox::Marshallable {
	enum {uri = 29 << 8 | protocol::DAEMON_SVID};

	uint16_t serverId;

	virtual void marshal(sox::Pack &pk) const {
		pk << serverId;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> serverId;
	}
};

struct PServerRemove2 : public sox::Marshallable {
	enum {uri = 33 << 8 | protocol::DAEMON_SVID};

	uint32_t serverId;

	virtual void marshal(sox::Pack &pk) const {
		pk << serverId;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> serverId;
	}
};

struct PRegFavRecStrResSV: public sox::Marshallable{/*the same as PRegisterFavourStrResSV, for recover*/
	enum {uri = 30 << 8 | protocol::DAEMON_SVID};
	std::string dip;
	std::string wip;
	//uint16_t type;
	std::vector<FavorStrSV> favours; 	
	virtual void marshal(sox::Pack &pk) const {
		pk << dip << wip;
		sox::marshal_container(pk, favours);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> dip >> wip;
		sox::unmarshal_container(up, std::back_inserter(favours));
	}
};

struct PServersStatusChangeSV: public sox::Marshallable{
	enum {uri = 31 << 8 | protocol::DAEMON_SVID};
	uint32_t serverId;
	std::string name;
	ServerStatus serverStatus;	
	virtual void marshal(sox::Pack &pk) const {
		pk << serverId << name;
		pk.push_uint32(serverStatus);		
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> serverId >> name;
		serverStatus = (ServerStatus)up.pop_uint32();
	}

};

struct PServerPing2: public sox::Marshallable{
	enum {uri = 32 << 8 | protocol::DAEMON_SVID};
	uint32_t serverId;
	virtual void marshal(sox::Pack &pk) const {
		pk << serverId;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> serverId;
	}
};

struct PServerPing2Res: public sox::Marshallable{
	enum {uri = 32 << 8 | protocol::DAEMON_SVID};
	uint32_t serverId;
	bool isRecord;

	virtual void marshal(sox::Pack &pk) const {
		pk << serverId;
		pk.push_uint8(isRecord);
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> serverId;
		isRecord = (up.pop_uint8() != 0);
	}
};

struct PQueryPort: public sox::Marshallable
{
  enum {uri = 34 << 8 | protocol::DAEMON_SVID};
  uint32_t toServerId;
  uint32_t fromServerId;

  virtual void marshal(sox::Pack &pk) const 
  {
    pk << toServerId << fromServerId;
  }
  virtual void unmarshal(const sox::Unpack &up) 
  {
    up >> toServerId >> fromServerId;
  }
};

struct PChecker: public sox::Marshallable{
        enum {uri = 33 << 8 | protocol::DAEMON_SVID};
        uint32_t taskId;
        std::vector<uint32_t> ips;
        uint16_t port;

        virtual void marshal(sox::Pack &pk) const {
                pk << taskId << port;
                sox::marshal_container(pk, ips);
        }
        virtual void unmarshal(const sox::Unpack &up) {
                up >> taskId >> port;
                sox::unmarshal_container(up, std::back_inserter(ips));
        }
};

enum CheckResult{
        CONNECTED = 0,
        NOTCONNECTED = 1,
        TIMEOUT = 2,
        CHECKING = 3,
};

struct PCheckerResult: public sox::Marshallable{
        enum {uri = 34 << 8 | protocol::DAEMON_SVID};
        uint32_t taskId;
        CheckResult res;
        virtual void marshal(sox::Pack &pk) const {
                pk << taskId;
                pk.push_uint8(res);
        }
        virtual void unmarshal(const sox::Unpack &up) {
                up >> taskId;
                res = (CheckResult)up.pop_uint8();
        }
};

struct PServerRecover3: public sox::Marshallable{
        enum {uri = 35 << 8 | protocol::DAEMON_SVID};
        uint32_t                serverId;
        uint16_t                port;
        std::string             name;
        core::MUUID             uuid;
        uint32_t                groupId;                // 组id 
        uint32_t                seq;                    //恢复序号，防止重复恢复
        std::map<std::string, std::string> props;

        virtual void marshal(sox::Pack &pk) const {
                pk << serverId << port << name << uuid << groupId << seq;
                sox::marshal_container(pk, props);
        }
        virtual void unmarshal(const sox::Unpack &up) {
                up >> serverId >> port >> name >> uuid >> groupId >> seq;
                sox::unmarshal_container(up, std::inserter(props, props.begin()));
        }

};



struct PRegister3 : public sox::Marshallable {
        enum {uri = 36 << 8 | protocol::DAEMON_SVID};
        std::string sstr;
        uint32_t regGroupId;
        uint16_t port;
        std::map<std::string, std::string> props;
        virtual void marshal(sox::Pack &pk) const {
                pk << sstr << regGroupId << port;
                sox::marshal_container(pk, props);
        }
        virtual void unmarshal(const sox::Unpack &up) {
                up >> sstr >> regGroupId >> port;
                sox::unmarshal_container(up, std::inserter(props, props.begin()));
        }
};

struct IpTypeMar: public sox::Marshallable
{
	uint32_t ip;
	uint32_t types;	
        IpTypeMar(){ip = 0; types = 0;}
        IpTypeMar(uint32_t i, uint32_t t)
	{
                ip = i;
                types = t;
        }

        virtual void marshal(sox::Pack &pk) const {
                pk << ip << types;
        }
        virtual void unmarshal(const sox::Unpack &up) {
                up >> ip >> types;
        }
};
struct FavorStr3 : public sox::Marshallable {
        std::string name;
        uint16_t port;
        uint32_t serverId;
        uint32_t                        groupId;                // 组id
        std::vector<IpTypeMar> ips;
        std::map<std::string, std::string> props;
        bool operator == (const FavorStr3 &right) const{
                return (name == right.name && port == right.port
                        && serverId == right.serverId && groupId == right.groupId && ips.size() == right.ips.size());
        }

        virtual void marshal(sox::Pack &pk) const {
                pk << name << serverId << port << groupId;
                sox::marshal_container(pk, ips);
                sox::marshal_container(pk, props);
        }
        virtual void unmarshal(const sox::Unpack &up) {
                up >> name >> serverId >> port >> groupId;
                sox::unmarshal_container(up, std::back_inserter(ips));
                sox::unmarshal_container(up, std::inserter(props, props.begin()));
        }
};

struct PRegisterServerRes3 : public sox::Marshallable {
        enum {uri = 37 << 8 | protocol::DAEMON_SVID};
        uint32_t serverId;
        std::vector<IpTypeMar> yourIps;
        core::MUUID uuid;
        std::vector<FavorStr3> favors;
        virtual void marshal(sox::Pack &pk) const {
                pk << serverId << uuid;
                sox::marshal_container(pk, yourIps);
                sox::marshal_container(pk, favors);
        }
        virtual void unmarshal(const sox::Unpack &up) {
                up >> serverId >> uuid;
                sox::unmarshal_container(up, std::back_inserter(yourIps));
                sox::unmarshal_container(up, std::inserter(favors, favors.begin()));
        }
};

struct PServerAdd3 : public FavorStr3{
        enum {uri = 38 << 8 | protocol::DAEMON_SVID};
};

struct PRecoverRes3: public sox::Marshallable{
        enum {uri = 39 << 8 | protocol::DAEMON_SVID};
        std::vector<FavorStr3> favours;
        virtual void marshal(sox::Pack &pk) const {
                sox::marshal_container(pk, favours);
        }
        virtual void unmarshal(const sox::Unpack &up) {
                sox::unmarshal_container(up, std::back_inserter(favours));
        }
};
struct PRegister2dot5 : public sox::Marshallable {
        enum {uri = 40 << 8 | protocol::DAEMON_SVID};
        std::string sstr;
        uint32_t regGroupId;
        uint16_t port;
        std::map<std::string, std::string> props;
        virtual void marshal(sox::Pack &pk) const {
                pk << sstr << regGroupId << port;
                sox::marshal_container(pk, props);
        }
        virtual void unmarshal(const sox::Unpack &up) {
                up >> sstr >> regGroupId >> port;
                sox::unmarshal_container(up, std::inserter(props, props.begin()));
        }
};

struct PServerRecover2dot5: public sox::Marshallable{
        enum {uri = 41 << 8 | protocol::DAEMON_SVID};
        uint32_t                serverId;
        uint16_t                port;
        std::string             name;
        core::MUUID             uuid;
        uint32_t                groupId;                // 组id 
        uint32_t                seq;                    //恢复序号，防止重复恢复
        std::map<std::string, std::string> props;

        virtual void marshal(sox::Pack &pk) const {
                pk << serverId << port << name << uuid << groupId << seq;
                sox::marshal_container(pk, props);
        }
        virtual void unmarshal(const sox::Unpack &up) {
                up >> serverId >> port >> name >> uuid >> groupId >> seq;
                sox::unmarshal_container(up, std::inserter(props, props.begin()));
        }

};

struct PRegisterFavor3: public sox::Voidmable{
        enum {uri = 42 << 8 | protocol::DAEMON_SVID};
};

struct PRegisterFavor3Res: public sox::Marshallable{/*the same as PRegFavRecStrResSV, for register*/
        enum {uri = 43 << 8 | protocol::DAEMON_SVID};
        std::vector<FavorStr3> favours;
        virtual void marshal(sox::Pack &pk) const {
                sox::marshal_container(pk, favours);
        }
        virtual void unmarshal(const sox::Unpack &up) {
                sox::unmarshal_container(up, std::back_inserter(favours));
        }
};
}
//#endif /*DAEMON_H_*/
