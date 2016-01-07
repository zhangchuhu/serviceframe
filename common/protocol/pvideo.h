#pragma once

#include "packet.h"
#include "common/core/base_svid.h"

struct PLinkdToVideoSdk: public sox::Marshallable
{
  enum{uri = (10405 << 8 | protocol::VIDEO_SVID)};
  uint32_t topSid; 
  std::string msg;

  virtual void marshal(sox::Pack &pk) const
  {
    pk << topSid;
    pk.push_varstr32(msg.data(), msg.length());
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> topSid;
    msg = up.pop_varstr32();
  }
};

