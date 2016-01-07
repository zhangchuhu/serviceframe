#pragma once

#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/const.h"
#include "common/protocol/zip_tpl.h"
#include <vector>
#include <iterator>
#include <string>
#include "common/res_code.h"
#include "core/sox/sockethelper.h"

#include "common/protocol/UserGroupIdType.h"

namespace protocol
{
namespace signal
{
//===========================================================================// 
// common enum
enum UserInfoKey
{
  USER_INFO_TOP_SID = 1,
  USER_INFO_SUB_SID = 2,
};

// uri top 24bit : [41000, 42000) [43000, 44000) ... for server <-> client
//===========================================================================// 
struct PLoginSignalProxyAuth : public protocol::login::PLoginAuth
{
  enum {uri = (41001 << 8 | protocol::LINKD_SVID)};
};

struct PLoginSignalProxyAuthRes : public protocol::login::PLoginAuthRes
{
  enum {uri = (41002 << 8 | protocol::LINKD_SVID)};

  PLoginSignalProxyAuthRes(){}
  PLoginSignalProxyAuthRes(const PLoginAuthRes & res):PLoginAuthRes(res){}
};

struct PLoginSignalProxyCred : public protocol::login::PLoginLinkdCred
{   
  enum {uri = (41003 << 8 | protocol::LINKD_SVID)};
};

struct PLoginSignalProxyCredRes : public sox::Marshallable
{   
  enum {uri = (41004 << 8 | protocol::LINKD_SVID)};

  PLoginSignalProxyCredRes():res(0),creditRescode(0),signalUid(0),signalProxyId(0){}
  PLoginSignalProxyCredRes(uint32_t r, uint32_t c, uint64_t u, uint64_t p):
    res(r),creditRescode(c),signalUid(u),signalProxyId(p){}
  
  uint32_t res;
  uint32_t creditRescode;
  uint64_t signalUid;
  uint64_t signalProxyId;

  virtual void marshal(sox::Pack &pk) const 
  {
    pk << res << creditRescode << signalUid << signalProxyId;
  }
  virtual void unmarshal(const sox::Unpack &up) 
  {
    up >> res >> creditRescode >> signalUid >> signalProxyId;
  }  
};

struct PReLoginSignalProxyCred : public PLoginSignalProxyCred
{   
  enum {uri = (41005 << 8 | protocol::LINKD_SVID)};
  uint64_t signalUid;
  std::string reserve;
  virtual void marshal(sox::Pack &pk) const 
  {
    pk << signalUid << reserve;
    PLoginSignalProxyCred::marshal(pk);
  }
  virtual void unmarshal(const sox::Unpack &up) 
  {
    up >> signalUid >> reserve;
    PLoginSignalProxyCred::unmarshal(up);
  }
};

struct PReLoginSignalProxyCredRes : public PLoginSignalProxyCredRes
{   
  enum {uri = (41006 << 8 | protocol::LINKD_SVID)};
};

struct PLogoutSignalProxy : public sox::Voidmable
{   
  enum {uri = (41007 << 8 | protocol::LINKD_SVID)};
};

struct PSignalProxyBroadcastMsg: public sox::Marshallable
{
  enum { uri = ( 41008 << 8 ) | protocol::LINKD_SVID };

  uint32_t serviceType;
  UserGroupIdType group;
  
  std::string msg;

  virtual void marshal(sox::Pack &p) const 
  {
    p << serviceType << group;
    //sox::marshal_container(p, userInfoProps);
    p.push_varstr32(msg.data(), msg.size());
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> serviceType >> group;
    //sox::unmarshal_container(p, std::inserter(userInfoProps, userInfoProps.begin()));
    msg = p.pop_varstr32();
  }
};

struct PJoinUserGroupReq: public sox::Marshallable
{
  enum { uri = ( 41009 << 8 ) | protocol::LINKD_SVID };

  uint32_t serviceType;
  UserGroupIdType group;

  virtual void marshal(sox::Pack &p) const 
  {
    p << serviceType << group;
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> serviceType >> group;
  }
};

struct PLeaveUserGroupReq: public sox::Marshallable
{
  enum { uri = ( 41010 << 8 ) | protocol::LINKD_SVID };

  uint32_t serviceType;
  UserGroupIdType group;

  virtual void marshal(sox::Pack &p) const 
  {
    p << serviceType << group;
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> serviceType >> group;
  }
};

struct PJoinUserGroupBatchReq: public sox::Marshallable
{
  enum { uri = ( 41011 << 8 ) | protocol::LINKD_SVID };

  uint32_t serviceType;
  std::set<UserGroupIdType> groups;

  virtual void marshal(sox::Pack &p) const 
  {
    p << serviceType;
    sox::marshal_container(p, groups);
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> serviceType;
    sox::unmarshal_container(p, std::inserter(groups, groups.begin()));
  }
};

struct PLeaveUserGroupBatchReq: public sox::Marshallable
{
  enum { uri = ( 41012 << 8 ) | protocol::LINKD_SVID };

  uint32_t serviceType;
  std::set<UserGroupIdType> groups;

  virtual void marshal(sox::Pack &p) const 
  {
    p << serviceType;
    sox::marshal_container(p, groups);
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> serviceType;
    sox::unmarshal_container(p, std::inserter(groups, groups.begin()));
  }

};

// 
struct PUserMsgUpload: public sox::Marshallable
{
  enum { uri = ( 41013 << 8 ) | protocol::LINKD_SVID };

  PUserMsgUpload():serviceType(0){}
  
  std::string context;
  std::string serverName;
  uint32_t serviceType;
  std::map<uint32_t, std::string> userInfoProps;
  
  std::string msg;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context << serverName << serviceType;
    sox::marshal_container(p, userInfoProps);
    p.push_varstr32(msg.data(), msg.size());
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context >> serverName >> serviceType;
    sox::unmarshal_container(p, std::inserter(userInfoProps, userInfoProps.begin()));
    msg = p.pop_varstr32();
  }
};

struct PUserMsgUploadRes: public sox::Marshallable
{
  enum { uri = ( 41014 << 8 ) | protocol::LINKD_SVID };

  PUserMsgUploadRes():serviceType(0){}
  
  std::string context;
  std::string serverName;
  uint32_t serviceType;
  std::map<uint32_t, std::string> serverProps;
  
  std::string msg;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context << serverName << serviceType;
    sox::marshal_container(p, serverProps);
    p.push_varstr32(msg.data(), msg.size());
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> context >> serverName >> serviceType;
    sox::unmarshal_container(p, std::inserter(serverProps, serverProps.begin()));
    msg = p.pop_varstr32();
  }

};

struct PLoginSignalProxyAnonymous : public protocol::login::PLoginAuth
{
  enum {uri = (41015 << 8 | protocol::LINKD_SVID)};
};

struct PLoginSignalProxyAnonymousRes : public protocol::login::PLoginAuthRes
{
  enum {uri = (41016 << 8 | protocol::LINKD_SVID)};
  PLoginSignalProxyAnonymousRes(){}
  PLoginSignalProxyAnonymousRes(const PLoginAuthRes & res):PLoginAuthRes(res){}
};


//===========================================================================//
}
}

