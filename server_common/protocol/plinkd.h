#include "packet.h"
#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"

namespace server{
  namespace session{

    enum LINKD_URI
    {
      PSyncLinkdUserListURI    = ( 400 << 8 | protocol::LINKD_SVID ),
      PSyncLinkdUserListResURI = ( 401 << 8 | protocol::LINKD_SVID ),
      PPushLinkdUserURI        = ( 402 << 8 | protocol::LINKD_SVID ),
      PLinkdPingURI            = ( 403 << 8 | protocol::LINKD_SVID ),
      PPushLinkdUserRelayURI   = ( 404 << 8 | protocol::LINKD_SVID ),
      PLinkdPingRelayURI       = ( 405 << 8 | protocol::LINKD_SVID ),
      PPushToUserListURI       = ( 406 << 8 | protocol::LINKD_SVID ),
      PPushToUserRangeURI      = ( 407 << 8 | protocol::LINKD_SVID ),
      PPushToUserListRelayURI  = ( 408 << 8 | protocol::LINKD_SVID ),
      PPushToUserRangeRelayURI = ( 409 << 8 | protocol::LINKD_SVID ),
      PPushToUserListResURI    = ( 410 << 8 | protocol::LINKD_SVID ),
      PPushToUserRangeResURI   = ( 411 << 8 | protocol::LINKD_SVID ),
      PLocateOnlineUserURI     = ( 412 << 8 | protocol::LINKD_SVID ),
      PLocateOnlineUserResURI  = ( 413 << 8 | protocol::LINKD_SVID ),
      POnlinePingURI           = ( 414 << 8 | protocol::LINKD_SVID ),
      POnlinePingResURI        = ( 415 << 8 | protocol::LINKD_SVID ),
      PLocateOnlineUser2URI    = ( 416 << 8 | protocol::LINKD_SVID ),
      PLocateOnlineUserRes2URI = ( 417 << 8 | protocol::LINKD_SVID ),
      PSubscribeOnlineServiceURI = ( 418 << 8 | protocol::LINKD_SVID ),
      PSubscribeOnlineServiceResURI = ( 419 << 8 | protocol::LINKD_SVID ),
      PNotifyOnlineServiceURI = ( 420 << 8 | protocol::LINKD_SVID ),
      
      PSyncMobileProxyUserListURI    = ( 28001 << 8 | protocol::LINKD_SVID ),
      PSyncMobileProxyUserListResURI = ( 28002 << 8 | protocol::LINKD_SVID ),
      PPushMobileProxyUserURI        = ( 28003 << 8 | protocol::LINKD_SVID ),
      PMobileProxyOnlinePingURI            = ( 28004 << 8 | protocol::LINKD_SVID ),
      PMobileOnlineRelayURI          = ( 28005 << 8 | protocol::LINKD_SVID ),
    };

    struct PSyncLinkdUserList: public sox::Marshallable
    {
      enum { uri = PSyncLinkdUserListURI };
      uint32_t linkd_id;

      virtual void marshal(sox::Pack &p) const
      {
        p << linkd_id;
      }
      virtual void unmarshal(const sox::Unpack &p)
      {
        p >> linkd_id;
      }
    };

    struct UserInfoOnline: public sox::Marshallable
    {
      UserInfoOnline():ip(0){}
      UserInfoOnline(uint32_t _ip, const std::string & _type):ip(_ip),type(_type){}

      uint32_t    ip;
      std::string type;

      virtual void marshal(sox::Pack &p) const
      {
        p << ip << type;
      }
      virtual void unmarshal(const sox::Unpack &p)
      {
        p >> ip >> type;
      }
    };

    struct PSyncLinkdUserListRes: public sox::Marshallable
    {
      enum { uri = PSyncLinkdUserListResURI };

      uint32_t linkd_id;
      uint32_t uversion;
      std::map<uint32_t, UserInfoOnline> uinf;

      virtual void marshal(sox::Pack &p) const 
      {
        p << linkd_id << uversion;
        sox::marshal_container(p, uinf);
      }
      virtual void unmarshal(const sox::Unpack &p)
      {
        p >> linkd_id >> uversion;
        sox::unmarshal_container(p, std::inserter(uinf,uinf.begin()));
      }
    };

    struct PPushLinkdUser: public sox::Marshallable
    {
      enum { uri = PPushLinkdUserURI };
      uint32_t linkd_id;
      uint32_t uversion;
      std::map<uint32_t, UserInfoOnline> toAdd;
      std::set<uint32_t> toRemove;

      virtual void marshal(sox::Pack &p) const 
      {
        p << linkd_id << uversion;
        sox::marshal_container(p, toAdd);
        sox::marshal_container(p, toRemove);
      }
      virtual void unmarshal(const sox::Unpack &p)
      {
        p >> linkd_id >> uversion;
        sox::unmarshal_container(p, std::inserter(toAdd,toAdd.begin()));
        sox::unmarshal_container(p, std::inserter(toRemove, toRemove.begin()));
      }
    };

    struct PLinkdPing: public sox::Marshallable
    {
      enum { uri = PLinkdPingURI };
      uint32_t linkd_id;
      uint32_t uversion;

      virtual void marshal(sox::Pack &p) const 
      {
        p << linkd_id << uversion;
      }
      virtual void unmarshal(const sox::Unpack &p)
      {
        p >> linkd_id >> uversion;
      }
    };

    struct PPushLinkdUserRelay: public PPushLinkdUser
    {
      enum { uri = PPushLinkdUserRelayURI };
    };

    struct PLinkdPingRelay: public PLinkdPing
    {
      enum { uri = PLinkdPingRelayURI };
    };

    enum LinkdPushQosLevel
    {
      ONE_WAY     = 5,     // 单路发给linkd
      TWO_WAY     = 10,    // 双路发给linkd
      TWO_WAY_ACK = 15,    // 双路发给linkd，并且客户端发送确认消息
    };

    struct PPushToUserList: public sox::Marshallable
    {
      enum { uri = PPushToUserListURI };
      
      PPushToUserList():fromServerId(0),qos(0),seq(0),fromUid(0){}

      uint32_t fromServerId;
      std::set<uint32_t> users;
      uint16_t qos;
      uint32_t seq;
      std::string load;
      uint32_t fromUid;

      virtual void marshal(sox::Pack &p) const 
      {
        p << fromServerId << qos << seq;
        sox::marshal_container(p, users);
        p.push_varstr32(load.data(), load.length());
        p << fromUid;
      }
      virtual void unmarshal(const sox::Unpack &p)
      {
        p >> fromServerId >> qos >> seq;
        sox::unmarshal_container(p, std::inserter(users, users.end()));
        load = p.pop_varstr32();
        if(!p.empty())
        {
          p >> fromUid;
        }
      }
    };

    struct PPushToUserListRelay: public PPushToUserList
    {
      enum { uri = PPushToUserListRelayURI };
    };

    struct PPushToUserRange: public sox::Marshallable
    {
      enum { uri = PPushToUserRangeURI };

      uint32_t fromServerId;
      uint32_t uid_min;
      uint32_t uid_max;
      uint16_t qos;
      uint32_t seq;
      std::string load;

      virtual void marshal(sox::Pack &p) const 
      {
        p << fromServerId << uid_min << uid_max << qos << seq;
        p.push_varstr32(load.data(), load.length());
      }
      virtual void unmarshal(const sox::Unpack &p)
      {
        p >> fromServerId >> uid_min >> uid_max >> qos >> seq;
        load = p.pop_varstr32();
      }
    };

    struct PPushToUserRangeRelay: public PPushToUserRange
    {
      enum { uri = PPushToUserRangeRelayURI };
    };

    struct PPushToUserListRes: public sox::Marshallable
    {
      enum { uri = PPushToUserListResURI };

      std::set<uint32_t> users_ok;
      std::set<uint32_t> users_fail;
      uint32_t seq;

      virtual void marshal(sox::Pack &p) const 
      {
        sox::marshal_container(p, users_ok);
        sox::marshal_container(p, users_fail);
        p << seq;
      }
      virtual void unmarshal(const sox::Unpack &p)
      {
        sox::unmarshal_container(p, std::inserter(users_ok, users_ok.end()));
        sox::unmarshal_container(p, std::inserter(users_fail, users_fail.end()));
        p >> seq;
      }
    };

    struct PPushToUserRangeRes: public sox::Marshallable
    {
      enum { uri = PPushToUserRangeResURI };

      uint32_t users;
      uint32_t seq;

      virtual void marshal(sox::Pack &p) const 
      {
        p << users << seq;
      }
      virtual void unmarshal(const sox::Unpack &p)
      {
        p >> users >> seq;
      }
    };

    struct PLocateOnlineUser: public sox::Marshallable
    {
      enum { uri = PLocateOnlineUserURI };

      uint32_t fromServerId;
      std::set<uint32_t> users;
      uint32_t seq;

      virtual void marshal(sox::Pack &p) const 
      {
        p << fromServerId;
        sox::marshal_container(p, users);
        p << seq;
      }
      virtual void unmarshal(const sox::Unpack &p)
      {
        p >> fromServerId;
        sox::unmarshal_container(p, std::inserter(users, users.begin()));
        p >> seq;
      }
    };

    struct UserAccessInfo: public sox::Marshallable
    {
      uint32_t    ip;
      std::string type;

      UserAccessInfo() : ip(0) {}

      virtual void marshal(sox::Pack &p) const
      {
        p << ip << type;
      }
      virtual void unmarshal(const sox::Unpack &p)
      {
        p >> ip >> type;
      }
    };

    struct PLocateOnlineUserRes: public sox::Marshallable
    {
      enum { uri = PLocateOnlineUserResURI };

      std::map<uint32_t, UserAccessInfo> uid2info;
      std::set<uint32_t> usersOffline;
      uint32_t seq;

      virtual void marshal(sox::Pack &p) const 
      {
        sox::marshal_container(p, uid2info);
        sox::marshal_container(p, usersOffline);
        p << seq;
      }
      virtual void unmarshal(const sox::Unpack &p)
      {
        sox::unmarshal_container(p, std::inserter(uid2info, uid2info.end()));
        sox::unmarshal_container(p, std::inserter(usersOffline, usersOffline.begin()));
        p >> seq;
      }
    };

    struct POnlinePing: public sox::Marshallable
    {
      enum { uri = POnlinePingURI };

      uint32_t fromServerId;
      uint32_t reserver;

      POnlinePing() : reserver(0) {}

      virtual void marshal(sox::Pack &p) const 
      {
        p << fromServerId << reserver;
      }
      virtual void unmarshal(const sox::Unpack &p)
      {
        p >> fromServerId >> reserver;
      }

    };

    struct POnlinePingRes: public sox::Marshallable
    {
      enum { uri = POnlinePingResURI };

      uint32_t linkdNum;
      uint32_t onlineUserNum;
      uint32_t reserver;

      POnlinePingRes() : reserver(0) {}

      virtual void marshal(sox::Pack &p) const 
      {
        p << linkdNum << onlineUserNum << reserver;
      }
      virtual void unmarshal(const sox::Unpack &p)
      {
        p >> linkdNum >> onlineUserNum >> reserver;
      }
    };
    
    struct PLocateOnlineUser2: public PLocateOnlineUser
    {
      enum { uri = PLocateOnlineUser2URI };
      virtual void marshal(sox::Pack &p) const 
      {
        PLocateOnlineUser::marshal(p);
      }
      virtual void unmarshal(const sox::Unpack &p)
      {
        PLocateOnlineUser::unmarshal(p);
      }
    };

    struct UserAccessInfoVector: public sox::Marshallable
    {
      std::vector<UserAccessInfo> infos;

      UserAccessInfoVector() {}

      virtual void marshal(sox::Pack &p) const
      {
        sox::marshal_container(p, infos);
      }
      virtual void unmarshal(const sox::Unpack &p)
      {
        sox::unmarshal_container(p, std::back_inserter(infos));
      }
    };

    struct PLocateOnlineUserRes2: public sox::Marshallable
    {
      enum { uri = PLocateOnlineUserRes2URI };

      std::map<uint32_t, UserAccessInfoVector> uid2info;
      std::set<uint32_t> usersOffline;
      uint32_t seq;

      virtual void marshal(sox::Pack &p) const 
      {
        sox::marshal_container(p, uid2info);
        sox::marshal_container(p, usersOffline);
        p << seq;
      }
      virtual void unmarshal(const sox::Unpack &p)
      {
        sox::unmarshal_container(p, std::inserter(uid2info, uid2info.end()));
        sox::unmarshal_container(p, std::inserter(usersOffline, usersOffline.begin()));
        p >> seq;
      }
    };

    struct PSubscribeOnlineService : public sox::Marshallable
    {
      enum { uri = PSubscribeOnlineServiceURI };

      enum 
      {
        SUBSCRIBE = 0,
        UNSUBSCRIBE = 1,
      };
      
      std::string context;
      uint32_t fromServerId;
      uint32_t subsribeType;

      virtual void marshal(sox::Pack &p) const 
      {
        p << context << fromServerId << subsribeType;
      }
      virtual void unmarshal(const sox::Unpack &p)
      {
        p >> context >> fromServerId >> subsribeType;
      }
    };

    struct PSubscribeOnlineServiceRes : public sox::Marshallable
    {
      enum { uri = PSubscribeOnlineServiceResURI };

      std::string context;
      uint32_t rescode;

      virtual void marshal(sox::Pack &p) const 
      {
        p << context << rescode;
      }
      virtual void unmarshal(const sox::Unpack &p)
      {
        p >> context >> rescode;
      }
    };

    struct PNotifyOnlineService: public sox::Marshallable
    {
      enum { uri = PNotifyOnlineServiceURI};

      std::set<uint32_t> notifyAdd;
      std::set<uint32_t> notifyRemove;
      uint32_t timestamp;

      virtual void marshal(sox::Pack &p) const 
      {
        sox::marshal_container(p, notifyAdd);
        sox::marshal_container(p, notifyRemove);
        p << timestamp;
      }
      virtual void unmarshal(const sox::Unpack &p)
      {
        sox::unmarshal_container(p, std::inserter(notifyAdd, notifyAdd.end()));
        sox::unmarshal_container(p, std::inserter(notifyRemove, notifyRemove.begin()));
        p >> timestamp;
      }
    };


    // for mobile online

    struct PSyncMobileProxyUserList: public sox::Marshallable
    {
      enum { uri = PSyncMobileProxyUserListURI };
      uint32_t mobileProxyId;

      virtual void marshal(sox::Pack &p) const
      {
        p << mobileProxyId;
      }
      virtual void unmarshal(const sox::Unpack &p)
      {
        p >> mobileProxyId;
      }
    };

    struct MobileProxyUserOnlineInfo: public sox::Marshallable2
    {
      MobileProxyUserOnlineInfo() : clientIp(0),linkdIp(0),signalIp(0),serviceIp(0)
        , topsid(0), subsid(0){}
      uint32_t clientIp;
      uint32_t linkdIp;
      uint32_t signalIp;
      uint32_t serviceIp;
      uint32_t topsid;
      uint32_t subsid;

      virtual void marshal2(sox::Pack &p) const
      {
        p << clientIp << linkdIp << signalIp << serviceIp;
        p << topsid << subsid;
      }
      virtual void unmarshal2(const sox::Unpack &p)
      {
        p >> clientIp >> linkdIp >> signalIp >> serviceIp;
        if(!p.empty())
        {
          p >> topsid >> subsid;
        }
      }
    };

    // key: uid
    typedef std::map<uint32_t, MobileProxyUserOnlineInfo> mobile_uinfo_t;

    struct PSyncMobileProxyUserListRes: public sox::Marshallable
    {
      enum { uri = PSyncMobileProxyUserListResURI };

      uint32_t mobileProxyId;
      uint32_t uversion;
      mobile_uinfo_t uid2Info;

      virtual void marshal(sox::Pack &p) const 
      {
        p << mobileProxyId << uversion;
        sox::marshal_container(p, uid2Info);
      }
      virtual void unmarshal(const sox::Unpack &p)
      {
        p >> mobileProxyId >> uversion;
        sox::unmarshal_container(p, std::inserter(uid2Info,uid2Info.begin()));
      }
    };

    struct PPushMobileProxyUser: public sox::Marshallable
    {
      enum { uri = PPushMobileProxyUserURI };
      uint32_t mobileProxyId;
      uint32_t uversion;
      mobile_uinfo_t toAdd;
      std::set<uint32_t> toRemove;

      virtual void marshal(sox::Pack &p) const 
      {
        p << mobileProxyId << uversion;
        sox::marshal_container(p, toAdd);
        sox::marshal_container(p, toRemove);
      }
      virtual void unmarshal(const sox::Unpack &p)
      {
        p >> mobileProxyId >> uversion;
        sox::unmarshal_container(p, std::inserter(toAdd,toAdd.begin()));
        sox::unmarshal_container(p, std::inserter(toRemove, toRemove.begin()));
      }
    };

    struct PMobileProxyOnlinePing: public sox::Marshallable
    {
      enum { uri = PMobileProxyOnlinePingURI };
      uint32_t mobileProxyId;
      uint32_t uversion;

      virtual void marshal(sox::Pack &p) const 
      {
        p << mobileProxyId << uversion;
      }
      virtual void unmarshal(const sox::Unpack &p)
      {
        p >> mobileProxyId >> uversion;
      }
    };
    
    struct PMobileOnlineRelay: public sox::Marshallable
    {
      enum { uri = PMobileOnlineRelayURI };
      std::string data;

      virtual void marshal(sox::Pack &p) const 
      {
        p.push_varstr32(data.data(), data.size());
      }
      virtual void unmarshal(const sox::Unpack &p)
      {
        data = p.pop_varstr32();
      }
    };
 
    
  }
}

