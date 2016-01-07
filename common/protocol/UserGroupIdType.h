#pragma once

#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/const.h"
#include <sstream>
                                                                               
struct UserGroupIdType : public sox::Marshallable                                   
{                                                                              
  uint64_t userGroupType;                                                      
  uint64_t userGroupId;                                                        
                                                                               
  UserGroupIdType()                                                            
    : userGroupType(0)                                                         
    , userGroupId(0)                                                           
  {                                                                            
  }                                                                            
                                                                               
  UserGroupIdType(uint64_t type, uint64_t id)                                  
  {                                                                            
    userGroupType = type;                                                      
    userGroupId = id;                                                          
  }                                                                            
                                                                               
  virtual void marshal(sox::Pack &pk) const                                         
  {                                                                            
    pk << userGroupType << userGroupId;                                        
  }                                                                            
                                                                               
  virtual void unmarshal(const sox::Unpack &up)                                     
  {                                                                            
    up >> userGroupType >> userGroupId;                                        
  }                                                                            
                                                                               
  bool operator<(const UserGroupIdType& right) const                           
  {                                                                            
    return (userGroupType < right.userGroupType) ||                            
	 	  ((userGroupType == right.userGroupType) && (userGroupId < right.userGroupId));
  }                                                                            
                                                                               
  virtual std::ostream & trace(std::ostream& os)const                           
  {                                                                            
    os << "" << userGroupType << "," << userGroupId;
    return os;
  }
  
  std::string dump()const
  {
    std::ostringstream os;
    os << *this;
    return os.str();
  }                                                                        
};                                                                             
