#pragma once

#include "common/core/base_svid.h"
#include "packet.h"
#include <string>

namespace server
{
  namespace sedispatcher
  {
    struct PDispatcherConfigSetProperty : public sox::Marshallable
    {
      enum {uri = (6 << 8 | protocol::SE_DISPATCHER_SVID)};
      std::string context;
      uint32_t targetServerid;
      uint32_t seq;
      typedef std::map<std::string, std::string> PROPERTY_TYPE;
      PROPERTY_TYPE property;

      PDispatcherConfigSetProperty()
        : targetServerid(0)
        , seq(0)
      {}

      virtual void marshal(sox::Pack &p) const
      {
        p << context << targetServerid << seq;
        sox::marshal_container(p, property);
      }

      virtual void unmarshal(const sox::Unpack &p)
      {
        p >> context >> targetServerid >> seq;
        sox::unmarshal_container(p, std::inserter(property, property.begin()));
      }

      virtual std::ostream & trace(std::ostream & os) const
      {
        os << "ctx#=" << context.size() << ",";
        os << "tsid=" << targetServerid << ",";
        os << "seq=" << seq << ",";
        os << "p#=" << property.size() << "{";
        for(PROPERTY_TYPE::const_iterator it = property.begin();
          it != property.end(); ++it)
        {
          os << it->first << "=" << it->second << ",";
        }
        os << "};";
        return os;
      }
    };

    struct PDispatcherConfigSetPropertyRes : public sox::Marshallable
    {
      enum {uri = (7 << 8 | protocol::SE_DISPATCHER_SVID)};
      std::string context;
      uint32_t targetServerid;
      uint32_t serverId;
      uint32_t seq;

      PDispatcherConfigSetPropertyRes()
        : targetServerid(0)
        , serverId(0)
        , seq(0)
      {}

      virtual void marshal(sox::Pack &p) const
      {
        p << context << targetServerid << serverId << seq;
      }

      virtual void unmarshal(const sox::Unpack &p)
      {
        p >> context >> targetServerid >> serverId >> seq;
      }

      virtual std::ostream & trace(std::ostream & os) const
      {
        os << "ctx#=" << context.size() << ",";
        os << "tsid=" << targetServerid << ",";
        os << "rsid=" << serverId << ",";
        os << "seq=" << seq << ",";
        return os;
      }
    };

    struct PDispatcherConfigRequestProperty : public sox::Marshallable
    {
      enum {uri = (8 << 8 | protocol::SE_DISPATCHER_SVID)};
      uint32_t serverId;
      std::string name;

      PDispatcherConfigRequestProperty()
        : serverId(0)
      {}

      virtual void marshal(sox::Pack &p) const
      {
        p << serverId;
        p << name;
      }
      virtual void unmarshal(const sox::Unpack &p)
      {
        p >> serverId;
        if (!p.empty())
        {
          p >> name;
        }
      }
      virtual std::ostream & trace(std::ostream & os) const
      {    
        os << "dispatcherServerId=" << serverId << ",";
        os << "name=" << name << ".";
        return os;
      }
    };
  }
}

