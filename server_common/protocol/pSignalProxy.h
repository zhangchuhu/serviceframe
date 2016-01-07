#pragma once

#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/const.h"
#include "common/protocol/zip_tpl.h"
#include "common/helper/PacketString.h"
#include <vector>
#include <iterator>
#include <string>
#include "common/res_code.h"
#include "core/sox/sockethelper.h"


namespace protocol
{
namespace signal
{
//===========================================================================// 
// common enum
enum UserOnlineInfoKey
{
  USER_ONLINE_INFO_UID32 = 0,
  USER_ONLINE_INFO_UID64 = 1,
  USER_ONLINE_INFO_TYPE = 2,
  USER_ONLINE_INFO_INST = 3,
  USER_ONLINE_INFO_IP = 4,
  USER_ONLINE_INFO_TIMESTAMP = 5,
  USER_ONLINE_INFO_SPLIST = 6,
};

//===========================================================================// 
// common structs
struct UserOnlineInfo2: public sox::Marshallable2
{
  UserOnlineInfo2():ip(0),timeStamp(0){}  
  UserOnlineInfo2(uint32_t i, const std::string & t, const std::string & in, uint32_t ts)
    :ip(i),type(t),inst(in),timeStamp(ts){}
  
  uint32_t    ip;
  std::string type;
  std::string inst;
  uint32_t    timeStamp;

  virtual void marshal2(sox::Pack &p) const
  {
    p << ip << type << inst << timeStamp;
  }
  virtual void unmarshal2(const sox::Unpack &p)
  {
    p >> ip >> type >> inst;
    if(!p.empty())
    {
      p >> timeStamp;
    }
  }
};

struct UserOnlineInfo3: public sox::Marshallable2
{
  UserOnlineInfo3():ip(0),timeStamp(0){}

  uint32_t    ip;
  std::string type;
  std::string inst;
  uint32_t timeStamp;
  std::vector<uint64_t> proxyList;

  virtual void marshal2(sox::Pack &p) const
  {
    p << ip << type << inst << timeStamp;
    sox::marshal_container(p, proxyList);
  }
  virtual void unmarshal2(const sox::Unpack &p)
  {
    p >> ip >> type >> inst >> timeStamp;
    sox::unmarshal_container(p, std::back_inserter(proxyList)); 
  }  
  virtual std::ostream & trace(std::ostream & os) const
  {
    os << " ip=" << sox::addr_ntoa(ip);
    os << " time=" << timeStamp << " sp#=" << proxyList.size();
    os << " type=" << type << " inst=" << inst;
    return os;
  }
};

struct UserOnlineInfo4: public sox::Marshallable2
{
  UserOnlineInfo4(){}
  UserOnlineInfo4(uint32_t uid32, const std::string & type, uint32_t time)
  {
    addPropertyByKey(userInfoProp, (uint32_t)USER_ONLINE_INFO_UID32, uid32);
    addPropertyByKey(userInfoProp, (uint32_t)USER_ONLINE_INFO_TYPE, type);
    addPropertyByKey(userInfoProp, (uint32_t)USER_ONLINE_INFO_TIMESTAMP, time);
  }
  UserOnlineInfo4(uint32_t uid32, uint32_t userIp, const std::string & type, uint32_t time)
  {
    addPropertyByKey(userInfoProp, (uint32_t)USER_ONLINE_INFO_UID32, uid32);
    addPropertyByKey(userInfoProp, (uint32_t)USER_ONLINE_INFO_TYPE, type);
    addPropertyByKey(userInfoProp, (uint32_t)USER_ONLINE_INFO_IP, userIp);
    addPropertyByKey(userInfoProp, (uint32_t)USER_ONLINE_INFO_TIMESTAMP, time);
  }
  UserOnlineInfo4(uint32_t uid32, const UserOnlineInfo2 & uinfo2)
  {
    addPropertyByKey(userInfoProp, (uint32_t)USER_ONLINE_INFO_UID32, uid32);
    addPropertyByKey(userInfoProp, (uint32_t)USER_ONLINE_INFO_TYPE, uinfo2.type);
    addPropertyByKey(userInfoProp, (uint32_t)USER_ONLINE_INFO_INST, uinfo2.inst);
    addPropertyByKey(userInfoProp, (uint32_t)USER_ONLINE_INFO_IP, uinfo2.ip);
    addPropertyByKey(userInfoProp, (uint32_t)USER_ONLINE_INFO_TIMESTAMP, uinfo2.timeStamp);
  }
  UserOnlineInfo4(uint32_t uid32, const UserOnlineInfo3 & uinfo3)
  {
    addPropertyByKey(userInfoProp, (uint32_t)USER_ONLINE_INFO_UID32, uid32);
    addPropertyByKey(userInfoProp, (uint32_t)USER_ONLINE_INFO_IP, uinfo3.ip);
    addPropertyByKey(userInfoProp, (uint32_t)USER_ONLINE_INFO_TYPE, uinfo3.type);
    addPropertyByKey(userInfoProp, (uint32_t)USER_ONLINE_INFO_INST, uinfo3.inst);
    addPropertyByKey(userInfoProp, (uint32_t)USER_ONLINE_INFO_TIMESTAMP, uinfo3.timeStamp);
    addPropertyByKey(userInfoProp, (uint32_t)USER_ONLINE_INFO_SPLIST, uinfo3.proxyList);
  }
  UserOnlineInfo4(uint64_t uid64, const std::string & type, uint32_t time)
  {
    addPropertyByKey(userInfoProp, (uint32_t)USER_ONLINE_INFO_UID64, uid64);
    addPropertyByKey(userInfoProp, (uint32_t)USER_ONLINE_INFO_TYPE, type);
    addPropertyByKey(userInfoProp, (uint32_t)USER_ONLINE_INFO_TIMESTAMP, time);
  }
  UserOnlineInfo4(uint64_t uid64, uint32_t userIp, const std::string & type, uint32_t time)
  {
    addPropertyByKey(userInfoProp, (uint32_t)USER_ONLINE_INFO_UID64, uid64);
    addPropertyByKey(userInfoProp, (uint32_t)USER_ONLINE_INFO_TYPE, type);
    addPropertyByKey(userInfoProp, (uint32_t)USER_ONLINE_INFO_IP, userIp);
    addPropertyByKey(userInfoProp, (uint32_t)USER_ONLINE_INFO_TIMESTAMP, time);
  }
  UserOnlineInfo4(const std::vector<uint64_t> & spList)
  {
    addPropertyByKey(userInfoProp, (uint32_t)USER_ONLINE_INFO_SPLIST, spList);
  }
  
  std::map<uint32_t, std::string> userInfoProp;

  virtual void marshal2(sox::Pack &p) const
  {
    sox::marshal_container(p, userInfoProp);
  }
  virtual void unmarshal2(const sox::Unpack &p)
  {
    sox::unmarshal_container(p, std::inserter(userInfoProp, userInfoProp.begin())); 
  }
};

// uri top 24bit : [40000, 41000) [42000, 43000) ... for server to server
//===========================================================================// 
// proxy <---sync---> online 
struct PSyncSignalProxyUserList: public sox::Marshallable
{
  enum { uri = ( 40001 << 8 ) | protocol::LINKD_SVID };

  PSyncSignalProxyUserList():signalProxyId(0){}
  
  uint64_t signalProxyId;

  virtual void marshal(sox::Pack &p) const
  {
    p << signalProxyId;
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> signalProxyId;
  }
};

struct PSyncSignalProxyUserListRes: public sox::Marshallable
{
  enum { uri = ( 40002 << 8 ) | protocol::LINKD_SVID };
  
  PSyncSignalProxyUserListRes():signalProxyId(0),uversion(0){}

  uint64_t signalProxyId;
  uint32_t uversion;
  std::map<uint64_t, UserOnlineInfo2> uid2Info;

  virtual void marshal(sox::Pack &p) const 
  {
    p << signalProxyId << uversion;
    sox::marshal_container(p, uid2Info);
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> signalProxyId >> uversion;
    sox::unmarshal_container(p, std::inserter(uid2Info, uid2Info.begin()));
  }
};
// todo typedef ZipMarshal<PSyncSignalProxyUserListRes, ( 40002 << 8 ) | protocol::LINKD_SVID> PSyncSignalProxyUserListRes;

struct PPushSignalProxyUser: public sox::Marshallable
{
  enum { uri = ( 40003 << 8 ) | protocol::LINKD_SVID };

  PPushSignalProxyUser():signalProxyId(0),uversion(0){}
  
  uint64_t signalProxyId;
  uint32_t uversion;
  std::map<uint64_t, UserOnlineInfo2> toAdd;
  std::set<uint64_t> toRemove;
  std::set<uint64_t> toLogout;

  virtual void marshal(sox::Pack &p) const 
  {
    p << signalProxyId << uversion;
    sox::marshal_container(p, toAdd);
    sox::marshal_container(p, toRemove);
    sox::marshal_container(p, toLogout);
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> signalProxyId >> uversion;
    sox::unmarshal_container(p, std::inserter(toAdd,toAdd.begin()));
    sox::unmarshal_container(p, std::inserter(toRemove, toRemove.begin()));
    sox::unmarshal_container(p, std::inserter(toLogout, toLogout.begin()));
  }
};

struct PSignalProxyPing: public sox::Marshallable
{
  enum { uri = ( 40004 << 8 ) | protocol::LINKD_SVID };

  PSignalProxyPing():signalProxyId(0),uversion(0){}
  
  uint64_t signalProxyId;
  uint32_t uversion;

  virtual void marshal(sox::Pack &p) const 
  {
    p << signalProxyId << uversion;
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> signalProxyId >> uversion;
  }
};

// proxy <---transmit---> services
struct PSignalProxyUserUpload: public sox::Marshallable
{
  enum { uri = ( 40005 << 8 ) | protocol::LINKD_SVID };

  PSignalProxyUserUpload():signalProxyId(0),signalUid(0),serviceType(0){}
  
  std::string context;
  uint64_t signalProxyId;
  uint64_t signalUid;
  uint32_t serviceType;
  std::map<uint32_t, std::string> userInfoProps;
  
  std::string msg;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context << signalProxyId << signalUid << serviceType;
    sox::marshal_container(p, userInfoProps);
    p.push_varstr32(msg.data(), msg.size());
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context >> signalProxyId >> signalUid >> serviceType;
    sox::unmarshal_container(p, std::inserter(userInfoProps, userInfoProps.begin()));
    msg = p.pop_varstr32();
  }
};

struct PSignalProxyUserUploadRes: public sox::Marshallable
{
  enum { uri = ( 40006 << 8 ) | protocol::LINKD_SVID };

  PSignalProxyUserUploadRes():serverId(0),signalUid(0),serviceType(0){}
  
  std::string context;
  uint64_t serverId;
  uint64_t signalUid;
  uint32_t serviceType;
  std::map<uint32_t, std::string> extentProps;
  
  std::string msg;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context << serverId << signalUid << serviceType;
    sox::marshal_container(p, extentProps);
    p.push_varstr32(msg.data(), msg.size());
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context >> serverId >> signalUid >> serviceType;
    sox::unmarshal_container(p, std::inserter(extentProps, extentProps.begin()));
    msg = p.pop_varstr32();
  }

};

// proxy <---user data---> online auth
struct PSignalProxyOnlineAuth: public sox::Marshallable
{
  enum { uri = ( 40007 << 8 ) | protocol::LINKD_SVID };

  PSignalProxyOnlineAuth():signalProxyId(0),uid(0){}
  
  std::string context;
  
  uint64_t signalProxyId;
  uint32_t uid;
  std::string type;
  std::string inst;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context;
    p << signalProxyId << uid;
    p << type << inst;
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context;
    p >> signalProxyId >> uid;
    p >> type >> inst;
  }
};

struct PSignalProxyOnlineAuthRes: public sox::Marshallable
{
  enum { uri = ( 40008 << 8 ) | protocol::LINKD_SVID };

  PSignalProxyOnlineAuthRes():rescode(RES_NOCHANGED),serverId(0),signalUid(0){}
  
  std::string context;

  uint32_t rescode;  
  uint64_t serverId;
  uint64_t signalUid;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context << rescode << serverId << signalUid;
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context >> rescode >> serverId >> signalUid;
  }
};

// auth <---userOnlineInfo---> online
struct PQueryUserOnlineInfo: public sox::Marshallable
{
  enum { uri = ( 40009 << 8 ) | protocol::LINKD_SVID };

  PQueryUserOnlineInfo():uid(0){}
  
  std::string context;
  
  uint32_t uid;
  std::string type; // empty for all types
  std::string inst; // empty for all instances

  virtual void marshal(sox::Pack &p) const 
  {
    p << context;
    p << uid;
    p << type << inst;
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context;
    p >> uid;
    p >> type >> inst;
  }
};

struct PQueryUserOnlineInfoRes: public sox::Marshallable
{
  enum { uri = ( 40010 << 8 ) | protocol::LINKD_SVID };

  PQueryUserOnlineInfoRes():rescode(RES_NOCHANGED){}
  
  std::string context;

  uint32_t rescode;
  std::map<uint64_t, UserOnlineInfo3> uid2Info;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context << rescode;
    sox::marshal_container(p, uid2Info);
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context >> rescode;
    sox::unmarshal_container(p, std::inserter(uid2Info, uid2Info.begin()));
  }
};

struct PQueryUserOnlineInfo2: public sox::Marshallable
{
  enum { uri = ( 40011 << 8 ) | protocol::LINKD_SVID };

  PQueryUserOnlineInfo2():uid(0){}
  
  std::string context;
  
  uint32_t uid;
  std::string type; // empty for all types
  std::string inst; // empty for all instances
  std::set<uint32_t> queryKeys; // empty for all keys

  virtual void marshal(sox::Pack &p) const 
  {
    p << context;
    p << uid;
    p << type << inst;
    sox::marshal_container(p, queryKeys);
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context;
    p >> uid;
    p >> type >> inst;
    sox::unmarshal_container(p, std::inserter(queryKeys, queryKeys.begin()));
  }
};

struct PQueryUserOnlineInfo2Res: public sox::Marshallable
{
  enum { uri = ( 40012 << 8 ) | protocol::LINKD_SVID };

  PQueryUserOnlineInfo2Res():rescode(RES_NOCHANGED){}
  
  std::string context;

  uint32_t rescode;
  std::map<uint64_t, UserOnlineInfo4> uid2Info;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context << rescode;
    sox::marshal_container(p, uid2Info);
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context >> rescode;
    sox::unmarshal_container(p, std::inserter(uid2Info, uid2Info.begin()));
  }
};

// online auth <---kick user---> proxy
struct PSignalProxyOnlineKick: public sox::Marshallable
{
  enum { uri = ( 40013 << 8 ) | protocol::LINKD_SVID };

  PSignalProxyOnlineKick():serverId(0),signalUid(0){}
  
  std::string context;
  
  uint64_t serverId;
  uint64_t signalUid;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context;
    p << serverId << signalUid;
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context;
    p >> serverId >> signalUid;
  }
};

struct PSignalProxyOnlineKickRes: public sox::Marshallable
{
  enum { uri = ( 40014 << 8 ) | protocol::LINKD_SVID };

  PSignalProxyOnlineKickRes():rescode(RES_NOCHANGED),signalProxyId(0),signalUid(0){}
  
  std::string context;

  uint32_t rescode;  
  uint64_t signalProxyId;
  uint64_t signalUid;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context << rescode << signalProxyId << signalUid;
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context >> rescode >> signalProxyId >> signalUid;
  }
};

struct PPublishUserOnlineInfo: public sox::Marshallable
{
  PPublishUserOnlineInfo():serverId(0),version(0),timeStamp(0){}

  uint64_t serverId;
  uint32_t version;
  uint32_t timeStamp;
  // key: uid64; value: uid32 userip type login/logouttime
  std::map<uint64_t, UserOnlineInfo4> adds;
  std::map<uint64_t, UserOnlineInfo4> removes;

  virtual void marshal(sox::Pack &p) const 
  {
    p << serverId << version << timeStamp;
    sox::marshal_container(p, adds);
    sox::marshal_container(p, removes);
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> serverId >> version >> timeStamp;
    sox::unmarshal_container(p, std::inserter(adds, adds.begin()));
    sox::unmarshal_container(p, std::inserter(removes, removes.begin()));
  }
};
typedef ZipMarshal<PPublishUserOnlineInfo, (40015<<8)|protocol::LINKD_SVID> PPublishUserOnlineInfoZip;

struct PPublishUserOnlineInfo2: public sox::Marshallable
{
  PPublishUserOnlineInfo2():serverId(0),version(0),timeStamp(0){}

  uint64_t serverId;
  uint32_t version;
  uint32_t timeStamp;
  // key: uid32; value: uid64 userip type login/logouttime
  std::multimap<uint32_t, UserOnlineInfo4> adds;
  std::multimap<uint32_t, UserOnlineInfo4> removes;

  virtual void marshal(sox::Pack &p) const 
  {
    p << serverId << version << timeStamp;
    sox::marshal_container(p, adds);
    sox::marshal_container(p, removes);
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> serverId >> version >> timeStamp;
    sox::unmarshal_container(p, std::inserter(adds, adds.begin()));
    sox::unmarshal_container(p, std::inserter(removes, removes.begin()));
  }
};
typedef ZipMarshal<PPublishUserOnlineInfo2, (40016<<8)|protocol::LINKD_SVID> PPublishUserOnlineInfo2Zip;

struct PQueryUserOnlineInfoBatch: public sox::Marshallable
{
  enum { uri = ( 40017 << 8 ) | protocol::LINKD_SVID };

  PQueryUserOnlineInfoBatch(){}

  std::string context;
  std::set<uint64_t> uids;
  std::set<uint32_t> propKeys;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context;
    sox::marshal_container(p, uids);
    sox::marshal_container(p, propKeys);
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context;
    sox::unmarshal_container(p, std::inserter(uids, uids.begin()));
    sox::unmarshal_container(p, std::inserter(propKeys, propKeys.begin()));
  }  
  virtual std::ostream & trace(std::ostream & os) const
  {
    os << " uids#=" << uids.size();
    os << " k#=" << propKeys.size();
    return os;
  }
  std::string dump() const
  {
    std::ostringstream os;
    os << *this;
    return os.str();
  }
};

struct PQueryUserOnlineInfoBatchRes: public sox::Marshallable
{
  PQueryUserOnlineInfoBatchRes():rescode(RES_NOCHANGED){}
  
  std::string context;

  uint32_t rescode;
  std::map<uint64_t, UserOnlineInfo4> uid2Info;
  std::set<uint64_t> offlineUids;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context << rescode;
    sox::marshal_container(p, uid2Info);
    sox::marshal_container(p, offlineUids);
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context >> rescode;
    sox::unmarshal_container(p, std::inserter(uid2Info, uid2Info.begin()));
    sox::unmarshal_container(p, std::inserter(offlineUids, offlineUids.begin()));
  }
};
typedef ZipMarshal<PQueryUserOnlineInfoBatchRes, (40018<<8)|protocol::LINKD_SVID> PQueryUserOnlineInfoBatchResZip;

struct PQueryOnlineUserList: public sox::Marshallable
{
  enum { uri = ( 40019 << 8 ) | protocol::LINKD_SVID };

  PQueryOnlineUserList(){}

  std::string context;
  std::string continueContext; // from PQueryOnlineUserListRes for 拆包续传

  virtual void marshal(sox::Pack &p) const 
  {
    p << context << continueContext;
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context >> continueContext;
  }
  std::string dump() const
  {
    std::ostringstream os;
    os << *this;
    return os.str();
  }
};

struct PQueryOnlineUserListRes: public sox::Marshallable
{
  PQueryOnlineUserListRes():rescode(RES_NOCHANGED){}
  
  std::string context;
  uint32_t rescode;
  std::string continueContext; // for 拆包续传 继续请求PQueryOnlineUserList
  std::set<uint64_t> uids;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context << rescode << continueContext;
    sox::marshal_container(p, uids);
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context >> rescode >> continueContext;
    sox::unmarshal_container(p, std::inserter(uids, uids.begin()));
  }
};
typedef ZipMarshal<PQueryOnlineUserListRes, (40020<<8)|protocol::LINKD_SVID> PQueryOnlineUserListResZip;

struct PPushMsgToUser: public sox::Marshallable
{
  enum { uri = ( 40021 << 8 ) | protocol::LINKD_SVID };

  PPushMsgToUser():serverId(0),signalUid(0),qos(0),seqNum(0),timeStamp(0){}
  
  std::string context;
  uint64_t serverId;
  uint64_t signalUid;
  uint32_t qos;
  uint32_t seqNum;
  uint32_t timeStamp;
  
  std::string msg;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context << serverId << signalUid << qos << seqNum << timeStamp;
    p.push_varstr32(msg.data(), msg.size());
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context >> serverId >> signalUid >> qos >> seqNum >> timeStamp;
    msg = p.pop_varstr32();
  }
  virtual std::ostream & trace(std::ostream & os) const
  {
    os << " svid=" << std::hex << serverId << " uid=" << signalUid << std::dec;
    os << " qos=" << qos << " seq=" << seqNum << " time=" << timeStamp;
    os << " msg#=" << msg.size();
    return os;
  }
  std::string dump() const
  {
    std::ostringstream os;
    os << *this;
    return os.str();
  }  
};

struct PPushMsgToUserRes: public sox::Marshallable
{
  enum { uri = ( 40022 << 8 ) | protocol::LINKD_SVID };

  PPushMsgToUserRes():serverId(0),rescode(RES_NOCHANGED){}
  
  std::string context;
  uint64_t serverId;
  uint32_t rescode;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context << serverId << rescode;
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context >> serverId >> rescode;
  }

};

struct PPublishUserProxyInfo: public sox::Marshallable
{
  PPublishUserProxyInfo():serverId(0),version(0),timeStamp(0){}

  uint64_t serverId;
  uint32_t version;
  uint32_t timeStamp;
  // key: uid64; value: uinfo - proxyIds
  std::map<uint64_t, UserOnlineInfo4> adds;
  std::map<uint64_t, UserOnlineInfo4> removes; // no use, todo

  virtual void marshal(sox::Pack &p) const 
  {
    p << serverId << version << timeStamp;
    sox::marshal_container(p, adds);
    //sox::marshal_container(p, removes);
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> serverId >> version >> timeStamp;
    sox::unmarshal_container(p, std::inserter(adds, adds.begin()));
    //sox::unmarshal_container(p, std::inserter(removes, removes.begin()));
  }

};
typedef ZipMarshal<PPublishUserProxyInfo, (40023<<8)|protocol::LINKD_SVID> PPublishUserProxyInfoZip;

struct PPublishUserProxyInfo2: public sox::Marshallable
{
  PPublishUserProxyInfo2():serverId(0),version(0),timeStamp(0){}

  uint64_t serverId;
  uint32_t version;
  uint32_t timeStamp;
  // key: uid64; value: proxyId
  std::set<std::pair<uint64_t, uint64_t> > adds;
  std::set<std::pair<uint64_t, uint64_t> > removes;

  virtual void marshal(sox::Pack &p) const 
  {
    p << serverId << version << timeStamp;
    sox::marshal_container(p, adds);
    sox::marshal_container(p, removes);
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> serverId >> version >> timeStamp;
    sox::unmarshal_container(p, std::inserter(adds, adds.begin()));
    sox::unmarshal_container(p, std::inserter(removes, removes.begin()));
  }
};
typedef ZipMarshal<PPublishUserProxyInfo2, (40024<<8)|protocol::LINKD_SVID> PPublishUserProxyInfo2Zip;

struct PPushMsgToUserList: public sox::Marshallable
{
  enum { uri = ( 40025 << 8 ) | protocol::LINKD_SVID };

  PPushMsgToUserList():serverId(0),qos(0),seqNum(0),timeStamp(0){}
  
  std::string context;
  uint64_t serverId;
  std::set<uint64_t> signalUids;
  uint32_t qos;
  uint32_t seqNum;
  uint32_t timeStamp;
  
  std::string msg;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context << serverId;
    sox::marshal_container(p, signalUids);
    p << qos << seqNum << timeStamp;
    p.push_varstr32(msg.data(), msg.size());
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context >> serverId;
    sox::unmarshal_container(p, std::inserter(signalUids, signalUids.begin()));
    p >> qos >> seqNum >> timeStamp;
    msg = p.pop_varstr32();
  }
  virtual std::ostream & trace(std::ostream & os) const
  {
    os << " svid=" << std::hex << serverId << " uid#=" << std::dec << signalUids.size();
    os << " qos=" << qos << " seq=" << seqNum << " time=" << timeStamp;
    os << " msg#=" << msg.size();
    return os;
  }
  std::string dump() const
  {
    std::ostringstream os;
    os << *this;
    return os.str();
  }    
};

struct PPushMsgToUserListRes: public sox::Marshallable
{
  enum { uri = ( 40026 << 8 ) | protocol::LINKD_SVID };

  PPushMsgToUserListRes():serverId(0),rescode(RES_NOCHANGED){}
  
  std::string context;
  uint64_t serverId;
  uint32_t rescode;
  std::set<uint32_t> usersOk;
  std::set<uint32_t> usersFail;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context << serverId << rescode;
    sox::marshal_container(p, usersOk);
    sox::marshal_container(p, usersFail);
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context >> serverId >> rescode;
    sox::unmarshal_container(p, std::inserter(usersOk, usersOk.end()));
    sox::unmarshal_container(p, std::inserter(usersFail, usersFail.end()));
  }

};

struct PPushMsgToUserRange: public sox::Marshallable
{
  enum { uri = ( 40027 << 8 ) | protocol::LINKD_SVID };

  PPushMsgToUserRange():serverId(0),signalUidMin(0),signalUidMax(0),qos(0),seqNum(0),timeStamp(0){}
  
  std::string context;
  uint64_t serverId;
  uint64_t signalUidMin;
  uint64_t signalUidMax;
  uint32_t qos;
  uint32_t seqNum;
  uint32_t timeStamp;
  
  std::string msg;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context << serverId << signalUidMin << signalUidMax;
    p << qos << seqNum << timeStamp;
    p.push_varstr32(msg.data(), msg.size());
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context >> serverId >> signalUidMin >> signalUidMax;
    p >> qos >> seqNum >> timeStamp;
    msg = p.pop_varstr32();
  }
};

struct PPushMsgToUserRangeRes: public sox::Marshallable
{
  enum { uri = ( 40028 << 8 ) | protocol::LINKD_SVID };

  PPushMsgToUserRangeRes():serverId(0),rescode(RES_NOCHANGED){}
  
  std::string context;
  uint64_t serverId;
  uint32_t rescode;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context << serverId << rescode;
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context >> serverId >> rescode;
  }

};

struct PCheckUsersOnline: public sox::Marshallable
{
  enum { uri = ( 40029 << 8 ) | protocol::LINKD_SVID };

  PCheckUsersOnline(){}
  
  std::string context;
  std::set<uint64_t> signalUids;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context;
    sox::marshal_container(p, signalUids);
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context;
    sox::unmarshal_container(p, std::inserter(signalUids, signalUids.begin()));
  }
  virtual std::ostream & trace(std::ostream & os) const
  {
    os << " uids#=" << signalUids.size();
    return os;
  }
  std::string dump() const
  {
    std::ostringstream os;
    os << *this;
    return os.str();
  }   
};

struct PCheckUsersOnlineRes: public sox::Marshallable
{
  enum { uri = ( 40030 << 8 ) | protocol::LINKD_SVID };

  PCheckUsersOnlineRes():rescode(RES_NOCHANGED){}
  
  std::string context;
  uint32_t rescode;
  std::set<uint32_t> usersOnline;
  std::set<uint32_t> usersOffline;
  std::set<uint32_t> usersUndefine;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context << rescode;
    sox::marshal_container(p, usersOnline);
    sox::marshal_container(p, usersOffline);
    sox::marshal_container(p, usersUndefine);
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context >> rescode;
    sox::unmarshal_container(p, std::inserter(usersOnline, usersOnline.end()));
    sox::unmarshal_container(p, std::inserter(usersOffline, usersOffline.end()));
    sox::unmarshal_container(p, std::inserter(usersUndefine, usersUndefine.end()));
  }

};

struct PQueryUserOnlineInfo3: public sox::Marshallable
{
  enum { uri = ( 40031 << 8 ) | protocol::LINKD_SVID };

  PQueryUserOnlineInfo3():signalUid(0){}
  
  std::string context;
  
  uint64_t signalUid;
  std::set<uint32_t> queryKeys;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context;
    p << signalUid;
    sox::marshal_container(p, queryKeys);
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context;
    p >> signalUid;
    sox::unmarshal_container(p, std::inserter(queryKeys, queryKeys.begin()));
  } 
  virtual std::ostream & trace(std::ostream & os) const
  {
    os << " uid=" << std::hex << signalUid << "(" << std::dec << signalUid << ")";
    os << " k#=" << queryKeys.size();
    return os;
  }
  std::string dump() const
  {
    std::ostringstream os;
    os << *this;
    return os.str();
  }
};

struct PQueryUserOnlineInfo3Res: public sox::Marshallable
{
  enum { uri = ( 40032 << 8 ) | protocol::LINKD_SVID };

  PQueryUserOnlineInfo3Res():rescode(RES_NOCHANGED){}
  
  std::string context;

  uint32_t rescode;
  UserOnlineInfo4 uinfo;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context << rescode << uinfo;
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context >> rescode >> uinfo;
  }
};

struct PQueryAllUserOnlineInfo: public sox::Marshallable
{
  enum { uri = ( 40033 << 8 ) | protocol::LINKD_SVID };

  PQueryAllUserOnlineInfo(){}

  std::string context;
  std::string continueContext; // from PQueryAllUserOnlineInfoRes for 拆包续传

  virtual void marshal(sox::Pack &p) const 
  {
    p << context << continueContext;
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context >> continueContext;
  }
  std::string dump() const
  {
    std::ostringstream os;
    os << *this;
    return os.str();
  }
};

struct PQueryAllUserOnlineInfoRes: public sox::Marshallable
{
  PQueryAllUserOnlineInfoRes():rescode(RES_NOCHANGED){}
  
  std::string context;
  uint32_t rescode;
  std::string continueContext; // for 拆包续传 继续请求PQueryOnlineUserList
  std::map<uint64_t, UserOnlineInfo4> uid2Info;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context << rescode << continueContext;
    sox::marshal_container(p, uid2Info);
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context >> rescode >> continueContext;
    sox::unmarshal_container(p, std::inserter(uid2Info, uid2Info.begin()));
  }
};
typedef ZipMarshal<PQueryAllUserOnlineInfoRes, (40034<<8)|protocol::LINKD_SVID> PQueryAllUserOnlineInfoResZip;

struct PQueryUserOnlineInfoBatch2: public sox::Marshallable
{
  enum { uri = ( 40035 << 8 ) | protocol::LINKD_SVID };

  PQueryUserOnlineInfoBatch2(){}

  std::string context;
  std::set<uint32_t> uids;
  std::set<uint32_t> propKeys;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context;
    sox::marshal_container(p, uids);
    sox::marshal_container(p, propKeys);
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context;
    sox::unmarshal_container(p, std::inserter(uids, uids.begin()));
    sox::unmarshal_container(p, std::inserter(propKeys, propKeys.begin()));
  }
};

struct PQueryUserOnlineInfoBatch2Res: public sox::Marshallable
{
  PQueryUserOnlineInfoBatch2Res():rescode(RES_NOCHANGED){}
  
  std::string context;
  uint32_t rescode;
  std::multimap<uint32_t, UserOnlineInfo4> uid2Info;
  std::set<uint32_t> offlineUids;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context << rescode;
    sox::marshal_container(p, uid2Info);
    sox::marshal_container(p, offlineUids);
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context >> rescode;
    sox::unmarshal_container(p, std::inserter(uid2Info, uid2Info.begin()));
    sox::unmarshal_container(p, std::inserter(offlineUids, offlineUids.begin()));
  }
};
typedef ZipMarshal<PQueryUserOnlineInfoBatch2Res, (40036<<8)|protocol::LINKD_SVID> PQueryUserOnlineInfoBatch2ResZip;

struct dummy
{

};

//===========================================================================//
}
}

