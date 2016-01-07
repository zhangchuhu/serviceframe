#pragma once

#include "common/protocol/UserGroupIdType.h"

struct PUserGroupInfoSync : public sox::Marshallable                                
{                                                                              
  enum {uri = 2076 << 8 | protocol::SERVICE_SVID};// todo server::service::PUserGroupInfoSync_uri};                        
  uint64_t serverId;                                                           
  bool isFullSync;                                                             
  std::set<UserGroupIdType> addedGroup;                                        
  std::set<UserGroupIdType> removedGroup;                                      
                                                                               
  PUserGroupInfoSync()                                                         
    : isFullSync(false)                                                        
  {                                                                            
  }                                                                            
                                                                               
  virtual void marshal(sox::Pack &pk) const                                         
  {                                                                            
    pk << serverId << isFullSync;                                              
    marshal_container(pk, addedGroup);                                         
    marshal_container(pk, removedGroup);                                       
  }                                                                            
                                                                               
  virtual void unmarshal(const sox::Unpack &up)                                     
  {                                                                            
    up >> serverId >> isFullSync;                                              
    unmarshal_container(up, std::inserter(addedGroup, addedGroup.begin()));    
    unmarshal_container(up, std::inserter(removedGroup, removedGroup.begin()));
  }                                                                            
};                                                                             
                                                                               
struct PBroadcastUserGroupMsg : public sox::Marshallable                            
{                                                                              
  enum {uri = 2077 << 8 | protocol::SERVICE_SVID};// todo server::service::PBroadcastUserGroupMsg_uri};                    

  enum OptionalInfoType
  {
    TOPSID
  };

  enum PriorityType
  {
    EMERGENCY = 0,
    IMPORTANT,
    NORMAL
  };

  PBroadcastUserGroupMsg()
    : priority(NORMAL)
  {
  }

  UserGroupIdType userGroupId;
  uint32_t serviceType;
  uint64_t serviceId;
  uint64_t seqNum;
  uint64_t uid;
  uint32_t serviceStartTime;
  uint8_t priority;
  // <type, value>
  // for now, only in subsid bro case, type can be TOPSID
  std::map<uint32_t, uint64_t> optionalInfo;
  std::string serviceMsgData;

  virtual void marshal(sox::Pack& pk) const
  {
    pk << userGroupId;
    pk << serviceType << serviceId;
    pk << seqNum << uid << serviceStartTime << priority;
    marshal_container(pk, optionalInfo);
    pk.push_varstr32(serviceMsgData.c_str(),serviceMsgData.length());
  }

  virtual void unmarshal(const sox::Unpack& up)
  {
    up >> userGroupId;
    up >> serviceType >> serviceId;
    up >> seqNum >> uid >> serviceStartTime >> priority;
    unmarshal_container(up, std::inserter(optionalInfo, optionalInfo.begin()));
    serviceMsgData = up.pop_varstr32();
  }

  virtual void unmarshalWithoutData(const sox::Unpack& up)
  {
    up >> userGroupId;
    up >> serviceType >> serviceId;
    up >> seqNum >> uid >> serviceStartTime >> priority;
    unmarshal_container(up, std::inserter(optionalInfo, optionalInfo.begin()));
  }
                                                                             
  virtual std::ostream & trace(std::ostream& os)const                           
  {                                                                            
    os << "ugid=" << userGroupId << " svct=" << serviceType;
    os << std::hex << " svid=" << serviceId << " uid=" << uid;
    os << std::dec << " seq=" << seqNum << " ts=" << serviceStartTime << " pr=" << priority;
    os << " oi#=" << optionalInfo.size() << " msg#=" << serviceMsgData.size();
    return os;
  }
  
  std::string dump()const
  {
    std::ostringstream os;
    os << *this;
    return os.str();
  }  
};

struct PBroadcastUserGroupSeqMsg: public PBroadcastUserGroupMsg
{
  enum {uri = 2078 << 8 | protocol::SERVICE_SVID};
  uint32_t userDefKey;

  virtual void marshal(sox::Pack& pk) const
  {
    pk << userDefKey;
    PBroadcastUserGroupMsg::marshal(pk);
  }

  virtual void unmarshal(const sox::Unpack& up)
  {
    up >> userDefKey;
    PBroadcastUserGroupMsg::unmarshal(up);
  }

  virtual void unmarshalWithoutData(const sox::Unpack& up)
  {
    up >> userDefKey;
    PBroadcastUserGroupMsg::unmarshalWithoutData(up);
  }
};

//SvcProxy Reg to BcAgent
struct PSvcProxyReg2BcAgent : public sox::Marshallable
{
    enum {uri = 2401 << 8 | protocol::SERVICE_SVID};
    uint16_t groupId;
    uint64_t svcProxyId;
    virtual void marshal(sox::Pack &pk) const {
       pk << groupId << svcProxyId ;

    }
    virtual void unmarshal(const sox::Unpack &up) {
       up >> groupId >> svcProxyId;
    }
};

struct PSvcProxyReg2BcAgentRes : public sox::Marshallable
{
    enum {uri = 2402 << 8 | protocol::SERVICE_SVID};
    uint16_t groupId;
    uint64_t bcAgentId;
    virtual void marshal(sox::Pack &pk) const {
       pk << groupId << bcAgentId ;

    }
    virtual void unmarshal(const sox::Unpack &up) {
       up >> groupId >> bcAgentId;
    }
};

// SvcProxy ping to BcAgent
struct PSvcProxy2BcAgentPing : public sox::Marshallable
{
    enum {uri = 2404 << 8 | protocol::SERVICE_SVID};
    uint32_t timestamp;
    uint64_t svcProxyId;

    virtual void marshal(sox::Pack &pk) const {
        pk << timestamp << svcProxyId;
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> timestamp >> svcProxyId;
    }
};

struct PSvcProxy2BcAgentPingRes : public sox::Marshallable
{
    enum {uri = 2405 << 8 | protocol::SERVICE_SVID};
    uint64_t bcAgentId;

    virtual void marshal(sox::Pack &pk) const {
        pk << bcAgentId;
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> bcAgentId;
    }
};


