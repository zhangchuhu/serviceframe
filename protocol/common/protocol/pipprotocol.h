#ifndef PIPPROTOCOL_H
#define PIPPROTOCOL_H
#include <string>
#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"

struct PipAllocateChannel : public sox::Marshallable {
    enum {uri = (0x01 << 8) | protocol::PIP_SVID};
    std::string game_string;
    std::string client_mac;
    virtual void marshal(sox::Pack &pk) const {
        pk << game_string << client_mac;
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> game_string >> client_mac;
    }
};

struct PipAllocateChannelResp : public sox::Marshallable {
    enum {uri = (0x02 << 8) | protocol::PIP_SVID};
    uint32_t channel_id;
    uint16_t refresh_interval;
    uint16_t error_code;
    virtual void marshal(sox::Pack &pk) const {
        pk << channel_id << refresh_interval << error_code;
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> channel_id >> refresh_interval >> error_code;
    }
};

struct PipRefreshChannel : public sox::Marshallable {
    enum {uri = (0x03 << 8) | protocol::PIP_SVID};
    uint32_t channel_id;
    std::string client_mac;
    virtual void marshal(sox::Pack &pk) const {
        pk << channel_id << client_mac;
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> channel_id >> client_mac;
    }
};

struct PipRefreshChannelResp : public sox::Marshallable {
    enum {uri = (0x04 << 8) | protocol::PIP_SVID};
    uint16_t error_code;
    virtual void marshal(sox::Pack &pk) const {
        pk << error_code;
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> error_code;
    }
};

struct PipReleaseChannelName : public sox::Marshallable {
    enum {uri = (0x05 << 8) | protocol::PIP_SVID};
    std::string game_string;
    uint32_t channel_id;
    std::string client_mac;
    virtual void marshal(sox::Pack &pk) const {
        pk << game_string << channel_id << client_mac;
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> game_string >> channel_id >> client_mac;
    }
};

struct PipReleaseChannelNameResp : public sox::Marshallable {
    enum {uri = (0x06 << 8) | protocol::PIP_SVID};
    uint16_t error_code;
    virtual void marshal(sox::Pack &pk) const {
        pk << error_code;
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> error_code;
    }
};

struct PipDestroyChannel : public sox::Marshallable {
    enum {uri = (0x07 << 8) | protocol::PIP_SVID};
    uint32_t channel_id;
    std::string client_mac;
    virtual void marshal(sox::Pack &pk) const {
        pk << channel_id << client_mac;
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> channel_id >> client_mac;
    }
};

struct PipDestroyChannelResp : public sox::Marshallable {
    enum {uri = (0x08 << 8) | protocol::PIP_SVID};
    uint16_t error_code;
    virtual void marshal(sox::Pack &pk) const {
        pk << error_code;
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> error_code;
    }
};

struct PipLoginLbsRes : public sox::Marshallable {
	enum {uri = (0x09 << 8) | protocol::PIP_SVID};

  PipLoginLbsRes() : resCode(0), tmp_uid(0), ip(0){}

	uint32_t				resCode;
	std::string				user_name;
	uint32_t				tmp_uid;
	std::string				cookie;
	uint32_t				ip;
	std::vector<uint16_t>	ports;
	uint16_t				refresh_interval;
	ISPType					isp;
	virtual void marshal(sox::Pack &pk) const {
		pk << resCode << user_name << tmp_uid << cookie << ip;
		sox::marshal_container(pk, ports);
		pk << refresh_interval;
		pk.push_uint32(isp);
	}

	virtual void unmarshal(const sox::Unpack &up) {
		up >> resCode >> user_name >> tmp_uid >> cookie >> ip;
		sox::unmarshal_container(up, std::back_inserter(ports));
		up >> refresh_interval;
		if (up.size() >= 4){
			isp = (ISPType) up.pop_uint32();
		}else{
			isp = AUTO_DETECT;
		}
	}
};


struct PipAllocateChannel20 : public sox::Marshallable
{
  enum {uri = (0x0a << 8) | protocol::PIP_SVID};

  std::string game_string;
  std::string client_mac;
  std::string strType; // pip, im p2p, ...
  uint32_t version;

  PipAllocateChannel20() : version(0) {};

  virtual void marshal(sox::Pack &pk) const
  {
    pk << game_string << client_mac << strType << version;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> game_string >> client_mac >> strType;
    if (up.size() >= 4)
    {
      up >> version;
    }
    else
    {
      version = 0;
    }
  }
};

struct PipAllocateChannel20Resp : public sox::Marshallable
{
  enum {uri = (0x0b << 8) | protocol::PIP_SVID};

  std::string game_string;
  uint32_t channel_id;
  uint16_t refresh_interval;
  uint16_t error_code;
  uint32_t version;

  PipAllocateChannel20Resp() : version(1) {}

  virtual void marshal(sox::Pack &pk) const
  {
    pk << game_string << channel_id << refresh_interval << error_code << version;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> game_string >> channel_id >> refresh_interval >> error_code;
    if (up.size() >= 4)
    {
      up >> version;
    }
    else
    {
      version = 1;
    }
  }
};



struct PipLoginLbs20Res : public sox::Marshallable 
{
	enum {uri = (0x0c << 8) | protocol::PIP_SVID};

	uint32_t				resCode;
	std::string				user_name;
	uint32_t				tmp_uid;
	std::string				cookie;
	uint32_t				m_nCookieKeyVer;
	uint32_t				ip;
	std::vector<uint16_t>	ports;
	uint16_t				refresh_interval;

	PipLoginLbs20Res() : resCode(RES_SUCCESS), tmp_uid(0), m_nCookieKeyVer(uint32_t(-1)), ip(0), refresh_interval(0) {}
	virtual void marshal(sox::Pack &pk) const 
	{
		pk << resCode << user_name << tmp_uid << cookie << m_nCookieKeyVer << ip;
		sox::marshal_container(pk, ports);
		pk << refresh_interval;
	}

	virtual void unmarshal(const sox::Unpack &up) 
	{
		up >> resCode >> user_name >> tmp_uid >> cookie >> m_nCookieKeyVer >> ip;
		sox::unmarshal_container(up, std::back_inserter(ports));
		up >> refresh_interval;
	}
};

struct PipLogin : public sox::Marshallable {
	enum {uri = (0x21 << 8) | protocol::PIP_SVID};
	uint32_t    cid;
	std::string user_name;
	std::string password;
	std::string client_mac;
	std::string type;

	virtual void marshal(sox::Pack &pk) const {
		pk << cid << user_name << password << client_mac << type;
	}
	virtual void unmarshal(const sox::Unpack &up) {
		up >> cid >> user_name >> password >> client_mac;
		if(!up.empty())
		{
			up >> type;
		}
		else
		{
			type = "default";
		}
	}
};

struct PipLoginResp : public sox::Marshallable {
    enum {uri = (0x22 << 8) | protocol::PIP_SVID};
    uint32_t    cid;
    uint32_t    temp_uid;
    std::string user_name;
    std::string password;
    uint16_t refresh_interval;
    uint16_t error_code;
    virtual void marshal(sox::Pack &pk) const {
        pk << cid << temp_uid << user_name << password << refresh_interval << error_code;
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> cid >> temp_uid >> user_name >> password >> refresh_interval >> error_code;
    }
};

struct PipRefreshUser : public sox::Marshallable {
    enum {uri = (0x23 << 8) | protocol::PIP_SVID};
    uint32_t temp_uid;
    std::string client_mac;
    virtual void marshal(sox::Pack &pk) const {
        pk << temp_uid << client_mac;
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> temp_uid >> client_mac;
    }
};

struct PipRefreshUserResp : public sox::Marshallable {
    enum {uri = (0x24 << 8) | protocol::PIP_SVID};
    uint16_t error_code;
    virtual void marshal(sox::Pack &pk) const {
        pk << error_code;
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> error_code;
    }
};

struct PipDestroyUser : public sox::Marshallable {
    enum {uri = (0x25 << 8) | protocol::PIP_SVID};
    uint32_t temp_uid;
    std::string client_mac;
    virtual void marshal(sox::Pack &pk) const {
        pk << temp_uid << client_mac;
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> temp_uid >> client_mac;
    }
};

struct PipDestroyUserResp : public sox::Marshallable {
    enum {uri = (0x26 << 8) | protocol::PIP_SVID};
    uint16_t error_code;
    virtual void marshal(sox::Pack &pk) const {
        pk << error_code;
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> error_code;
    }
};

// messages between pipproxy and pipserver
struct PipRouterSerial : public sox::Marshallable {
  enum {uri = (0x27 << 8) | protocol::PIP_SVID};
  std::string from;
  uint32_t serial;
  uint32_t ruri;
  std::string load;

  virtual void marshal(sox::Pack &pk) const {
    pk << from << serial << ruri << load;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> from >> serial >> ruri >> load;
  }
};

struct PipSyncAllocateChannel : public sox::Marshallable {
  enum {uri = (0x28 << 8) | protocol::PIP_SVID};
  uint32_t channel_id;
  std::string game_string;
  std::string channel_type;
  virtual void marshal(sox::Pack &pk) const {
	  pk << channel_id << game_string << channel_type;
  }
  virtual void unmarshal(const sox::Unpack &up) {
	  up >> channel_id >> game_string >> channel_type;
  }
};

struct PipSyncAllocateUser : public sox::Marshallable {
  enum {uri = (0x29 << 8) | protocol::PIP_SVID};
  uint32_t temp_uid;
  std::string user_name;
  virtual void marshal(sox::Pack &pk) const {
	  pk << temp_uid << user_name;
  }
  virtual void unmarshal(const sox::Unpack &up) {
	  up >> temp_uid >> user_name;
  }
};

struct PipAllocateChannel20Resp2 : public PipAllocateChannel20Resp 
{
  enum {uri = (0x2a << 8) | protocol::PIP_SVID};

  std::string game_string;
  uint32_t channel_id;
  uint16_t refresh_interval;
  uint16_t error_code;
  std::string channel_type;

  virtual void marshal(sox::Pack &pk) const 
  {
	  pk << game_string << channel_id << refresh_interval << error_code << channel_type;
  }
  virtual void unmarshal(const sox::Unpack &up) 
  {
	  up >> game_string >> channel_id >> refresh_interval >> error_code >> channel_type;
  }
};

struct PipAllocateChannelResp2 : public PipAllocateChannelResp {
    enum {uri = (0x2b << 8) | protocol::PIP_SVID};
    std::string game_string;
    std::string channel_type;
    virtual void marshal(sox::Pack &pk) const {
        PipAllocateChannelResp::marshal(pk);
        pk << game_string << channel_type ;
    }
    virtual void unmarshal(const sox::Unpack &up) {
        PipAllocateChannelResp::unmarshal(up);
        up >> game_string >> channel_type ;
    }
};

struct PipLoginLbsRes2 : public sox::Marshallable {
	enum {uri = (0x30 << 8) | protocol::PIP_SVID};

	uint32_t				resCode;
	std::string				user_name;
	uint32_t				tmp_uid;
	std::string				cookie;
	uint32_t				ip;
	std::vector<uint16_t>	ports;
	uint16_t				refresh_interval;
	ISPType				isp;
	AreaType				area;
	virtual void marshal(sox::Pack &pk) const {
		pk << resCode << user_name << tmp_uid << cookie << ip;
		sox::marshal_container(pk, ports);
		pk << refresh_interval;
		pk.push_uint32(isp);
		pk.push_uint32(area);
	}

	virtual void unmarshal(const sox::Unpack &up) {
		up >> resCode >> user_name >> tmp_uid >> cookie >> ip;
		sox::unmarshal_container(up, std::back_inserter(ports));
		up >> refresh_interval;
		isp = (ISPType) up.pop_uint32();
		area = (AreaType)up.pop_uint32();
	}
};

// messages for static
enum{
    PIP_REQ_SID,
    PIP_REQ_UID,
    PIP_REFRESH_SID,
    PIP_REFRESH_UID,
    PIP_REL_CHANNEL_NAME,
    PIP_DESTROY_SID,
    PIP_DESTROY_UID,
    PIP_TIMEOUT_SID,
    PIP_TIMEOUT_UID,
    PIP_RESPONSE_FAIL,
};

struct PPipRequestStat : public sox::Marshallable
{
    enum { uri = (911 << 8) | protocol::CATALOG_SVID };
    uint32_t fromIp;
    uint32_t reserve;
    std::map<uint32_t, uint32_t> dataStatic;

    PPipRequestStat(): reserve(0) {}
    
    virtual void marshal(sox::Pack& pk) const
    {
        pk << fromIp << reserve;
        marshal_container(pk, dataStatic);
    }

    virtual void unmarshal(const sox::Unpack& up)
    {
        up >> fromIp >> reserve;
        unmarshal_container(up, std::inserter(dataStatic, dataStatic.begin()));
    }
};

#endif // PIPPROTOCOL_H
