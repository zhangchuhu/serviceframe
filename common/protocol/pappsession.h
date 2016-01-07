#ifndef PAPPSESSION_H_
#define PAPPSESSION_H_
#include <vector>
#include <list>
#include <iterator>
namespace protocol {
namespace session {
struct PAppServerRegister2AppManager: public sox::Marshallable {
  enum {
    uri = (1 << 8 | protocol::APPSESSION_SVID)
  };
  uint32_t appTypeId;
  uint32_t appServerVersion;
  std::string passwd;
  virtual void marshal(sox::Pack & p) const {
    p << appTypeId << appServerVersion << passwd;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> appTypeId >> appServerVersion >> passwd;
  }
};
enum RegisterStatus {
  REGISTER_SUCCESS, REGISTER_FALSE, REGISTER_REFUSE
}; //
struct PAppServerRegister2AppManagerRes: public sox::Marshallable {
  enum {
    uri = (2 << 8 | protocol::APPSESSION_SVID)
  };
  RegisterStatus status;
  virtual void marshal(sox::Pack & p) const {
    p.push_uint32(status);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    status = (RegisterStatus) p.pop_uint32();
  }
};

struct PSessionQueryAppInfo: public sox::Marshallable {
  enum {
    uri = (3 << 8 | protocol::APPSESSION_SVID)
  };
  uint32_t topChannelId;
  uint32_t sessionIp;
  uint16_t sessionPort;
  virtual void marshal(sox::Pack & p) const {
    p << topChannelId << sessionIp << sessionPort;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> topChannelId >> sessionIp >> sessionPort;
  }
};

struct PAppServerInfoRes: public sox::Marshallable {
  enum {
    uri = (4 << 8 | protocol::APPSESSION_SVID)
  };
  uint32_t topChannelId;
  uint32_t appTypeId;
  uint32_t appServerVersion;
  uint32_t appPluginVersion;
  uint32_t forceUpdateVersion;
  std::string appPluginMD5;
  std::string appName;
  std::string appPluginURL;
  std::string passwd;
  std::set<uint32_t> appSessionIp;
  virtual void marshal(sox::Pack & p) const {
    p << topChannelId << appTypeId << appServerVersion << appPluginVersion << appPluginMD5 << appName << appPluginURL
        << forceUpdateVersion << passwd;
    sox::marshal_container(p, appSessionIp);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> topChannelId >> appTypeId >> appServerVersion >> appPluginVersion >> appPluginMD5 >> appName >> appPluginURL
        >> forceUpdateVersion >> passwd;
    sox::unmarshal_container(p, std::inserter(appSessionIp, appSessionIp.begin()));
  }
};

struct PCreateAppChannel: public sox::Marshallable {
  enum {
    uri = (5 << 8 | protocol::APPSESSION_SVID)
  };
  uint32_t topChannelId;
  uint32_t sessionIp;
  uint16_t sessionPort;
  virtual void marshal(sox::Pack & p) const {
    p << topChannelId << sessionIp << sessionPort;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> topChannelId >> sessionIp >> sessionPort;
  }
};

struct PCreateAppChannelRes: public sox::Marshallable {
  enum {
    uri = (6 << 8 | protocol::APPSESSION_SVID)
  };
  uint32_t topChannelId;
  RegisterStatus status;
  virtual void marshal(sox::Pack & p) const {
    p << topChannelId;
    p.push_uint32(status);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> topChannelId;
    status = (RegisterStatus) p.pop_uint32();
  }
};

struct PAppSessionLogin2Session: public sox::Marshallable {
  enum {
    uri = (7 << 8 | protocol::APPSESSION_SVID)
  };
  uint32_t topChannelId;
  uint32_t appTypeId;
  uint32_t appServerVersion;
  std::string passwd;
  virtual void marshal(sox::Pack & p) const {
    p << topChannelId << appTypeId << appServerVersion << passwd;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> topChannelId >> appTypeId >> appServerVersion >> passwd;
  }
};
enum LoginStatus {
  LOGIN_SUCCESS, LOGIN_FALSE, LOGIN_NOTEXIST
};
struct PAppSessionLogin2SessionRes: public sox::Marshallable {
  enum {
    uri = (8 << 8 | protocol::APPSESSION_SVID)
  };
  uint32_t topChannelId;
  LoginStatus status;
  virtual void marshal(sox::Pack & p) const {
    p << topChannelId;
    p.push_uint32(status);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> topChannelId;
    status = (LoginStatus) p.pop_uint32();
  }
};

// send from appManager to session
struct PAppInfoRes: public sox::Marshallable {
  enum {
    uri = (9 << 8 | protocol::APPSESSION_SVID)
  };
  uint32_t topChannelId;
  uint32_t appTypeId;
  uint32_t appPluginVersion;
  uint32_t forceUpdateVersion;
  std::string appPluginMD5;
  std::string appName;
  std::string appPluginURL;
  virtual void marshal(sox::Pack & p) const {
    p << topChannelId << appTypeId << appPluginVersion << appPluginMD5 << appName << appPluginURL << forceUpdateVersion;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> topChannelId >> appTypeId >> appPluginVersion >> appPluginMD5 >> appName >> appPluginURL >> forceUpdateVersion;
  }
};

enum AppSessionStatus {
  APPSESSION_WAIT, APPSESSION_NORMAL, APPSESSION_DISCONNECTED, APPSESSION_RECONNECTED, APPSESSION_HANDOVER
};
struct PAppSessionStatus: public sox::Marshallable {
  enum {
    uri = (10 << 8 | protocol::APPSESSION_SVID)
  };
  uint32_t appTypeId;
  AppSessionStatus appSessionStatus;
  virtual void marshal(sox::Pack & p) const {
    p << appTypeId;
    p.push_uint32(appSessionStatus);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> appTypeId;
    appSessionStatus = (AppSessionStatus) p.pop_uint32();
  }
};

// This packet will use in: client->session and session->AppSession
struct PPluginReady: public sox::Marshallable {
  enum {
    uri = (11 << 8 | protocol::APPSESSION_SVID)
  };
  uint32_t topChannelId;
  uint32_t userId;
  uint32_t pluginVersion;
  virtual void marshal(sox::Pack & p) const {
    p << topChannelId << userId << pluginVersion;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> topChannelId >> userId >> pluginVersion;
  }
};

//enum ChannelStyle{ FreeStyle, ChairStyle, MicrophoneStyle };
enum TalkingMode {
  FREE_MODE, CHAIRMAN_MODE, MAIXU_MODE
};
struct ChannelInfoUpdate: public sox::Marshallable {
  uint32_t channelId;
  uint32_t parentId; // parentId=channelId means top channel
  uint32_t ownerId; //
  std::string nick;
  uint32_t jifen;
  uint32_t type; //
  bool isLimit; //
  bool isPublic;
  TalkingMode talkingMode;
  virtual void marshal(sox::Pack &p) const {
    p << channelId << parentId << ownerId << nick << jifen << type << isLimit << isPublic;
    p.push_uint32(talkingMode);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> channelId >> parentId >> ownerId >> nick >> jifen >> type >> isLimit >> isPublic;
    talkingMode = (TalkingMode)p.pop_uint32();
  }
};

struct MaixuInfoUpdate: public sox::Marshallable {
  uint32_t sid;
  std::list<uint32_t> uids;
  virtual void marshal(sox::Pack &p) const {
    p << sid;
    sox::marshal_container(p, uids);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> sid;
    sox::unmarshal_container(p, std::back_inserter(uids));
  }
};

struct UserRoleUpdate: public sox::Marshallable {
  uint32_t userId;
  uint32_t channelId;
  uint32_t roleId;
  virtual void marshal(sox::Pack & p) const {
    p << userId << channelId << roleId;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> userId >> channelId >> roleId;
  }
};

struct UserInfoUpdate: public sox::Marshallable {
  uint32_t userId;
  uint32_t currentChannelId;
  uint32_t proxyId;
  virtual void marshal(sox::Pack & p) const {
    p << userId << currentChannelId << proxyId;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> userId >> currentChannelId >> proxyId;
  }
};
struct PAppSessionProxyUpdate: public sox::Marshallable {
  enum {
      uri = (12 << 8 | protocol::APPSESSION_SVID)
    };
  uint32_t proxyId;
  uint32_t ip;
  bool isAdd;
  std::set<uint16_t> tcpPort;
  std::set<uint16_t> udpPort;
  virtual void marshal(sox::Pack & p) const {
    p << proxyId << ip << isAdd;
    sox::marshal_container(p, tcpPort);
    sox::marshal_container(p, udpPort);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> proxyId >> ip >> isAdd;
    sox::unmarshal_container(p, std::inserter(tcpPort, tcpPort.begin()));
    sox::unmarshal_container(p, std::inserter(udpPort, udpPort.begin()));
  }
};

struct PChannelInfoUpdate: public sox::Marshallable {
  enum {
    uri = (13 << 8 | protocol::APPSESSION_SVID)
  };
  uint32_t topChannelId;
  bool isSyncAll;
  std::vector<ChannelInfoUpdate> channelUpdate;
  std::vector<MaixuInfoUpdate> maixuUidsUpdate;
  std::vector<UserInfoUpdate> userUpdate;
  std::vector<uint32_t> removedChannel;
  std::vector<uint32_t> removedUser;
  std::vector<uint32_t> settles;
  std::vector<uint32_t> deSettles;
  std::vector<UserRoleUpdate> roleListUpdate;
  std::vector<UserRoleUpdate> removedRoleList;
  virtual void marshal(sox::Pack &p) const {
    p << topChannelId << isSyncAll;
    sox::marshal_container(p, channelUpdate);
    sox::marshal_container(p, maixuUidsUpdate);
    sox::marshal_container(p, userUpdate);
    sox::marshal_container(p, removedChannel);
    sox::marshal_container(p, removedUser);
    sox::marshal_container(p, settles);
    sox::marshal_container(p, deSettles);
    sox::marshal_container(p, roleListUpdate);
    sox::marshal_container(p, removedRoleList);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> topChannelId >> isSyncAll;
    sox::unmarshal_container(p, std::back_inserter(channelUpdate));
    sox::unmarshal_container(p, std::back_inserter(maixuUidsUpdate));
    sox::unmarshal_container(p, std::back_inserter(userUpdate));
    sox::unmarshal_container(p, std::back_inserter(removedChannel));
    sox::unmarshal_container(p, std::back_inserter(removedUser));
    sox::unmarshal_container(p, std::back_inserter(settles));
    sox::unmarshal_container(p, std::back_inserter(deSettles));
    sox::unmarshal_container(p, std::back_inserter(roleListUpdate));
    sox::unmarshal_container(p, std::back_inserter(removedRoleList));
  }
};

struct PAppSessionReqAllChannelInfo: public sox::Marshallable {
  enum {
    uri = (14 << 8 | protocol::APPSESSION_SVID)
  };
  uint32_t topChannelId;
  virtual void marshal(sox::Pack & p) const {
    p << topChannelId;
  }

  virtual void unmarshal(const sox::Unpack &p) {
    p >> topChannelId;
  }
};

struct PAppSessionReqUserInfo: public sox::Marshallable {
  enum {
    uri = (15 << 8 | protocol::APPSESSION_SVID)
  };
  uint32_t topChannelId;
  std::set<uint32_t> userId;
  virtual void marshal(sox::Pack & p) const {
    p << topChannelId;
    sox::marshal_container(p, userId);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> topChannelId;
    sox::unmarshal_container(p, std::inserter(userId, userId.begin()));
  }
};
struct PUserInfo: public sox::Marshallable {
  enum {
    uri = (16 << 8 | protocol::APPSESSION_SVID)
  };
  uint32_t topChannelId;
  uint32_t userId;
  uint32_t userJifen;
  uint32_t memberJifen;
  uint32_t sex; // {female=0,male=1;}
  std::string nick;
  std::string sign;
  bool settle;
  uint32_t game_type;
  std::string pcInfo;
  virtual void marshal(sox::Pack & p) const {
    p << topChannelId << userId << userJifen << memberJifen << sex << nick << sign << settle << game_type << pcInfo;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> topChannelId >> userId >> userJifen >> memberJifen >> sex >> nick >> sign >> settle >> game_type >> pcInfo;
  }
};
struct PAppReqUserInfoRes: public sox::Marshallable {
  enum {
    uri = (17 << 8 | protocol::APPSESSION_SVID)
  };
  std::vector<PUserInfo> usersInfo;
  virtual void marshal(sox::Pack &p) const {
    sox::marshal_container(p, usersInfo);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    sox::unmarshal_container(p, std::back_inserter(usersInfo));
  }
};
/* The follow packet has replaced by PAppBroadcast2ClientReq
 *
struct PAppBroadcastReq: public sox::Marshallable {
  enum {
    uri = (18 << 8 | protocol::APPSESSION_SVID)
  };
  uint32_t topChannelId;
  uint32_t channelId;
  std::set<uint32_t> exceptUids;
  bool recursiveBroadcast;
  std::string data;
  virtual void marshal(sox::Pack & p) const {
    p << topChannelId << channelId;
    marshal_container(p, exceptUids);
    p << recursiveBroadcast << data;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> topChannelId >> channelId;
    unmarshal_container(p, std::inserter(exceptUids, exceptUids.begin()));
    p >> recursiveBroadcast >> data;
  }
};

struct PAppMulticastReq: public sox::Marshallable {
  enum {
    uri = (19 << 8 | protocol::APPSESSION_SVID)
  };
  uint32_t topChannelId;
  std::set<uint32_t> userIds;
  std::string data;
  virtual void marshal(sox::Pack & p) const {
    p << topChannelId << data;
    sox::marshal_container(p, userIds);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> topChannelId >> data;
    sox::unmarshal_container(p, std::inserter(userIds, userIds.begin()));
  }
};
*/

struct PPluginData2AppSessionReq: public sox::Marshallable {
  enum {
    uri = (20 << 8 | protocol::APPSESSION_SVID)
  };
  uint32_t topChannelId;
  uint32_t userId;
  uint32_t ip;
  std::string data;
  virtual void marshal(sox::Pack & p) const {
    p << topChannelId << userId << ip << data;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> topChannelId >> userId >> ip >> data;
  }
};

struct PStopUpdateUserList2Client: public sox::Marshallable {
  enum {
    uri = (21 << 8 | protocol::APPSESSION_SVID)
  };
  uint32_t topChannelId;
  bool enableUpdateUserList2Client;
  virtual void marshal(sox::Pack & p) const {
    p << topChannelId << enableUpdateUserList2Client;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> topChannelId >> enableUpdateUserList2Client;
  }
};

struct PStopUpdateUserListRes: public sox::Marshallable {
  enum {
    uri = (22 << 8 | protocol::APPSESSION_SVID)
  };
  uint32_t topChannelId;
  RegisterStatus status;
  virtual void marshal(sox::Pack & p) const {
    p << topChannelId;
    p.push_uint32(status);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> topChannelId;
    status = (RegisterStatus) p.pop_uint32();
  }
};

struct PSessionKillAppChannel: public sox::Marshallable {
  enum {
    uri = (23 << 8 | protocol::APPSESSION_SVID)
  };
  uint32_t topChannelId;
  virtual void marshal(sox::Pack &p) const {
    p << topChannelId;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> topChannelId;
  }
};

struct PAppSessionPing: public sox::Marshallable {
  enum {
    uri = (24 << 8 | protocol::APPSESSION_SVID)
  };
  uint32_t topChannelId;
  uint32_t stampc;

  virtual void marshal(sox::Pack &pak) const {
    pak << topChannelId << stampc;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    pak >> topChannelId >> stampc;
  }
};

struct PAppSessionPingRes: public sox::Marshallable {
  enum {
    uri = (25 << 8 | protocol::APPSESSION_SVID)
  };
  uint32_t topChannelId;
  uint32_t stampc;

  virtual void marshal(sox::Pack &pak) const {
    pak << topChannelId << stampc;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    pak >> topChannelId >> stampc;
  }
};

struct PPluginUploadReq: public sox::Marshallable {
  enum {
    uri = (26 << 8) | protocol::APPSESSION_SVID
  };
  std::string data;

  virtual void marshal(sox::Pack &pak) const {
    pak << data;
  }
  virtual void unmarshal(const sox::Unpack &unpak) {
    unpak >> data;
  }
};

struct PAppSessionDownloadData: public sox::Marshallable {
  enum {
    uri = (27 << 8) | protocol::APPSESSION_SVID
  };
  std::string data;
  virtual void marshal(sox::Pack &pak) const {
    pak << data;
  }
  virtual void unmarshal(const sox::Unpack &unpak) {
    unpak >> data;
  }
};

struct PAppSessionLogin2Proxy: public sox::Marshallable {
  enum {
    uri = (28 << 8) | protocol::APPSESSION_SVID
  };
  uint32_t appTypeId;
  uint32_t channelId;
  std::string passwd;
  std::set<uint32_t> registerUris;
  virtual void marshal(sox::Pack &pak) const {
    pak << appTypeId << channelId << passwd;
    sox::marshal_container(pak, registerUris);
  }
  virtual void unmarshal(const sox::Unpack &unpak) {
    unpak >> appTypeId >> channelId >> passwd;
    sox::unmarshal_container(unpak, std::inserter(registerUris, registerUris.begin()));
  }
};

struct PAppSessionLogin2ProxyRes: public sox::Marshallable {
  enum {
    uri = (29 << 8) | protocol::APPSESSION_SVID
  };
  LoginStatus status;
  virtual void marshal(sox::Pack &pak) const {
    pak.push_uint32(status);
  }
  virtual void unmarshal(const sox::Unpack &unpak) {
    status = (LoginStatus) unpak.pop_uint32();
  }
};

struct PClientPack2AppSessionReq: public sox::Marshallable {
  enum {
    uri = (30 << 8 | protocol::APPSESSION_SVID)
  };
  uint32_t topChannelId;
  uint32_t currentChannelId;
  uint32_t userId;
  uint32_t ip;
  std::string data;
  virtual void marshal(sox::Pack & p) const {
    p << topChannelId << currentChannelId << userId << ip << data;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> topChannelId >> currentChannelId >> userId >> ip >> data;
  }
};

struct PAppBroadcast2ClientReq: public sox::Marshallable {
  enum {
    uri = (32 << 8 | protocol::APPSESSION_SVID)
  };
  uint32_t topChannelId;
  std::set<uint32_t> sids;
  std::vector<uint32_t> exceptUids;
  std::set<uint32_t> uids;
  std::string data;
  virtual void marshal(sox::Pack & p) const {
    p << topChannelId;
    marshal_container(p, sids);
    marshal_container(p, exceptUids);
    marshal_container(p, uids);
    p << data;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> topChannelId;
    unmarshal_container(p, std::inserter(sids, sids.begin()));
    unmarshal_container(p, std::back_inserter(exceptUids));
    unmarshal_container(p, std::inserter(uids, uids.begin()));
    p >> data;
  }
};

struct PAppAuthInfo: public sox::Marshallable {
  uint32_t appTypeId;
  uint32_t channelId;
  std::string passwd;
  uint32_t checkIp;
  std::set<uint32_t> AppSessionAllowedIp;
  virtual void marshal(sox::Pack &pak) const {
    pak << appTypeId << channelId << passwd << checkIp;
    sox::marshal_container(pak, AppSessionAllowedIp);
  }
  virtual void unmarshal(const sox::Unpack &unpak) {
    unpak >> appTypeId >> channelId >> passwd >> checkIp;
    sox::unmarshal_container(unpak, std::inserter(AppSessionAllowedIp, AppSessionAllowedIp.begin()));
  }
};

struct PMapAppAuthInfo: public sox::Marshallable {
  enum {
    uri = (33 << 8 | protocol::APPSESSION_SVID)
  };
  std::map<uint32_t, PAppAuthInfo> map_AppAuth;
  virtual void marshal(sox::Pack & p) const {
    sox::marshal_container(p, map_AppAuth);
  }

  virtual void unmarshal(const sox::Unpack &p) {
    sox::unmarshal_container(p, std::inserter(map_AppAuth, map_AppAuth.begin()));
  }
};



}
}

#endif /*PCOMMON_APPSESSION_H_*/

