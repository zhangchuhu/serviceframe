#pragma once

#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/const.h"
#include<string>

using namespace std;

struct AntiVerifyResultInfo : public sox::Marshallable
{
  enum {uri = (110 << 8) | protocol::ANTIJUNK_SVID};

  AntiVerifyResultInfo() : uid(0),uip(0),serverId(0),clientType(255){}

  string   biz_name;
  uint64_t uid;
  uint32_t uip;
  string   passport;
  string   result_data;
  uint32_t serverId;
  string   clientVerStr;
  uint32_t clientType;
  string   clientFrom;

  virtual void marshal(sox::Pack &pk) const
  {
    pk << biz_name << uid << uip << passport << result_data;
    pk << serverId << clientVerStr << clientType << clientFrom;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> biz_name >> uid >> uip >> passport >> result_data;
    if(!up.empty())
    {
      up >> serverId >> clientVerStr;
    }
    if(!up.empty())
    {
      up >> clientType >> clientFrom;
    }
  }
}; 

