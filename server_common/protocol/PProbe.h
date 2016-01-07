#pragma once

#include "common/core/base_svid.h"
#include "common/packet.h"
#include <string>
#include <sstream>
using namespace std;

namespace server {
namespace probe {

#define RTT_DEFAULT_WEIGHT 5
#define LOSS__DEFAULT_WEIGHT 95
#define RTT_HEAVY_WEIGHT 25
#define LOSS_HEAVY_WEIGHT 500

  enum ProbeDataUri
  {
    PProbeDataMgrRegReqURI = (1 << 8 | protocol::PROBE_DATA_SVID),
    PProbeDataMgrRegResURI = (2 << 8 | protocol::PROBE_DATA_SVID),
    PProbeDataReqURI = (3 << 8 | protocol::PROBE_DATA_SVID),
    PProbeDataResURI = (4 << 8 | protocol::PROBE_DATA_SVID),
    PProbeDataLbsReqURI = (5 << 8 | protocol::PROBE_DATA_SVID),
    PProbeDataLbsResURI = (6 << 8 | protocol::PROBE_DATA_SVID),
    ProbeLinkdMgrRegReqURI = (10 << 8 | protocol::PROBE_DATA_SVID),
    ProbeLinkdMgrRegResURI = (11 << 8 | protocol::PROBE_DATA_SVID)
  };

  enum RegResult
  {
    RegisterSucessful,
    InstanceAlreadyExist,
    InstanceAllocationFailure
  };

  enum RegExtInfo
  {
    NormalReg,
    DuplicateReg
  };

  struct PProbeDataMgrRegReq : public sox::Marshallable
  {
    enum {uri = PProbeDataMgrRegReqURI};

    uint32_t procInstanceNum;
    uint32_t serverId;
    RegExtInfo extInfo;

    virtual void marshal(sox::Pack &p) const
    {
      p << procInstanceNum;
      p << serverId;
      p.push_uint8(extInfo);
    }

    virtual void unmarshal(const sox::Unpack & up)
    {
      up >> procInstanceNum;
      up >> serverId;
      extInfo = NormalReg;
      if (!up.empty())
      {
        extInfo = static_cast<RegExtInfo>(up.pop_uint8());
      }
    }
  };

  struct PProbeDataMgrRegRes : public sox::Marshallable
  {
    enum {uri = PProbeDataMgrRegResURI};

    uint32_t procInstanceNum;
    uint32_t serverId;
    RegResult result;
    RegExtInfo extInfo;

    virtual void marshal(sox::Pack &p) const
    {
      p << procInstanceNum;
      p << serverId;
      p.push_uint8(result);
      p.push_uint8(extInfo);
    }

    virtual void unmarshal(const sox::Unpack & up)
    {
      up >> procInstanceNum;
      up >> serverId;
      result = static_cast<RegResult>(up.pop_uint8());
      extInfo = NormalReg;
      if (!up.empty())
      {
        extInfo = static_cast<RegExtInfo>(up.pop_uint8());
      }
    }
  };

  struct PProbeServerStatItem : public sox::Marshallable
  {
    uint32_t clientIp;
    uint32_t groupId;
    uint32_t rtt;
    uint8_t pkgSent;
    uint8_t pkgRecv;

    PProbeServerStatItem():clientIp(0xffffffff), groupId(0xffffffff), rtt(0xffffffff), pkgSent(0xff), pkgRecv(0xff){}

    double score(const PProbeServerStatItem& value) const
    {
        // The lower the score, the better
        // The weight for rtt is 5 and pkg loss rate is 95

//        uint32_t rttWeight = 5;
//        uint32_t lossWeight = 95;
//
//        double loss = (static_cast<double>(value.pkgSent - value.pkgRecv) / value.pkgSent) * 100;
//        if(value.rtt >= 95)
//          rttWeight = 25;
//        if(loss >= 5)
//          lossWeight = 500;
//
//        return (rttWeight*value.rtt + lossWeight*loss);

      double loss = (static_cast<double>(value.pkgSent - value.pkgRecv) / value.pkgSent) * 100;

      return ((value.rtt > 95 ? RTT_HEAVY_WEIGHT : RTT_DEFAULT_WEIGHT) * value.rtt
        + (loss > 5 ? LOSS_HEAVY_WEIGHT : LOSS__DEFAULT_WEIGHT) * loss);
    }

    bool operator< (const PProbeServerStatItem& right) const
    {
      return score(*this) < score(right);
    }

    virtual void marshal(sox::Pack &p) const
    {
      p << clientIp;
      p << groupId;
      p << rtt << pkgSent << pkgRecv;
    }

    virtual void unmarshal(const sox::Unpack & up)
    {
      up >> clientIp;
      up >> groupId;
      up >> rtt >> pkgSent >> pkgRecv;
    }
  };

  struct PProbeServerStat : public sox::Marshallable
  {
    enum {uri = ((1 << 16) | (3 << 8) |protocol::LINKD_TRANSMIT_SVID)/*PProbeServerStatURI*/};
    // <clientIp, std::vector<PProbeServerStatItem> >
    std::map<uint32_t, std::vector<PProbeServerStatItem> > probeResults;
    uint32_t uid;

    PProbeServerStat():uid(0){}

    virtual void marshal(sox::Pack &p) const
    {
      p << static_cast<uint32_t>(probeResults.size());
      std::map<uint32_t, std::vector<PProbeServerStatItem> >::const_iterator mcit;
      for(mcit = probeResults.begin(); mcit != probeResults.end(); ++mcit)
      {
        p << mcit->first;
        sox::marshal_container(p, mcit->second);
      }
      p << uid;
    }

    virtual void unmarshal(const sox::Unpack & up)
    {
      uint32_t mapSize;
      up >> mapSize;
      for(uint32_t loop = 0; loop < mapSize; ++loop)
      {
        uint32_t clientIp;
        up >> clientIp;
        std::vector<PProbeServerStatItem>& tmpVec = probeResults[clientIp];
        sox::unmarshal_container(up, std::back_inserter(tmpVec));
      }
      if (!up.empty())
      {
        up >> uid;
      }
    }
  };

  struct PProbeDataReq : public sox::Marshallable
  {
    enum {uri = PProbeDataReqURI};

    uint32_t clientIp;
    uint32_t serverId;
    uint32_t seqNum;

    virtual void marshal(sox::Pack& p) const
    {
      p << clientIp << serverId;
      p << seqNum;
    }

    virtual void unmarshal(const sox::Unpack& up)
    {
      up >> clientIp >> serverId;
      up >> seqNum;
    }
  };

  struct PProbeDataLbsReq : public PProbeDataReq
  {
    enum {uri = PProbeDataLbsReqURI};

    uint32_t connId;
    int port;

    virtual void marshal(sox::Pack& p) const
    {
      PProbeDataReq::marshal(p);
      p << connId;
      p << port;
    }

    virtual void unmarshal(const sox::Unpack& up)
    {
      PProbeDataReq::unmarshal(up);
      up >> connId;
      up >> port;
    }
  };

  struct ProbeDataItem : public sox::Marshallable
  {
    uint32_t clientIpRange;
    uint32_t groupId;
    uint32_t rtt;
    double pkgLossRate;

    ProbeDataItem()
      : clientIpRange(0)
      , groupId(0)
      , rtt(0)
      , pkgLossRate(0)
    {
    }

    ProbeDataItem(uint32_t _clientIpRange, uint32_t _groupId, uint32_t _rtt, double _pkgLossRate)
      : clientIpRange(_clientIpRange)
      , groupId(_groupId)
      , rtt(_rtt)
      , pkgLossRate(_pkgLossRate)
    {
    }

    double score(const ProbeDataItem& value) const
    {
        // The lower the score, the better
        // The weight for rtt is 5 and pkg loss rate is 95

//    uint32_t rttWeight = 5;
//    uint32_t lossWeight = 95;
//
//    if(value.rtt >= 95)
//      rttWeight = 25;
//    if(value.pkgLossRate >= 5)
//      lossWeight = 500;
//
//    return (rttWeight*value.rtt + lossWeight*value.pkgLossRate);

      // Well this code looks ugly, but it actually boost up some performance, :)
      return ((value.rtt > 95 ? RTT_HEAVY_WEIGHT : RTT_DEFAULT_WEIGHT) * value.rtt
        + (value.pkgLossRate > 5 ? LOSS_HEAVY_WEIGHT : LOSS__DEFAULT_WEIGHT) * value.pkgLossRate);
    }

    bool operator< (const ProbeDataItem& right) const
    {
      return score(*this) < score(right);
    }

    virtual void marshal(sox::Pack& p) const
    {
      p << clientIpRange;
      p << groupId;
      p << rtt;
      uint32_t tmp = static_cast<uint32_t>(pkgLossRate + 0.01);
      p << tmp;
    }

    virtual void unmarshal(const sox::Unpack& up)
    {
      up >> clientIpRange;
      up >> groupId;
      up >> rtt;
      uint32_t tmp;
      up >> tmp;
      pkgLossRate = static_cast<double>(tmp);
    }
  };

  struct PProbeDataRes : public sox::Marshallable
  {
    enum {uri = PProbeDataResURI};

    bool result;
    std::multiset<ProbeDataItem> probeData;
    uint32_t seqNum;

    virtual void marshal(sox::Pack& p) const
    {
      p << result;
      marshal_container(p, probeData);
      p << seqNum;
    }

    virtual void unmarshal(const sox::Unpack& up)
    {
      up >> result;
      unmarshal_container(up, std::inserter(probeData, probeData.begin()));
      up >> seqNum;
    }
  };

  struct PProbeDataLbsRes : public PProbeDataRes
  {
    enum {uri = PProbeDataLbsResURI};

    uint32_t connId;
    uint32_t ip;
    int port;

    virtual void marshal(sox::Pack& p) const
    {
      PProbeDataRes::marshal(p);
      p << connId;
      p << ip;
      p << port;
    }

    virtual void unmarshal(const sox::Unpack& up)
    {
      PProbeDataRes::unmarshal(up);
      up >> connId;
      up >> ip;
      up >> port;
    }
  };


  struct ProbePingCheckInfo : public sox::Marshallable
  {
    //进程所在机器的信息
    uint32_t ip;
    std::vector<uint16_t> ports;
    uint64_t serverId;
    uint32_t groupId;

    ProbePingCheckInfo() :ip(0), serverId(0), groupId(0) {}

    bool operator < (const ProbePingCheckInfo& pp) const
    {
      return serverId < pp.serverId;
    }

    string dump()
    {
      std::ostringstream os;
      os << "ip:" << ip << ";";
      os << "serverId:" << serverId << ";";
      os << "groupId:" << groupId << ";";
      os << "ports:[";
      for (std::vector<uint16_t>::iterator it = ports.begin(); it != ports.end(); ++it)
      {
        os << (*it) << ",";
      }
      os << "]";
      return os.str();
    }

    virtual void marshal(sox::Pack &pk) const
    {
      pk << ip;
      sox::marshal_container(pk, ports);
      pk << serverId;
      pk << groupId;
    }

    virtual void unmarshal(const sox::Unpack &up)
    {
      up >> ip;
      sox::unmarshal_container(up, std::back_inserter(ports));
      up >> serverId;
      up >> groupId;
    }
  };

  struct ProbeLinkdMgrRegReq : public sox::Marshallable
  {
    enum { uri = ProbeLinkdMgrRegReqURI };

    ProbePingCheckInfo info;

    virtual void marshal(sox::Pack &pk) const
    {
      pk << info;
    }

    virtual void unmarshal(const sox::Unpack &up)
    {
      up >> info;
    }
  };

  struct ProbeLinkdMgrRegRes : public sox::Marshallable
  {
    enum { uri = ProbeLinkdMgrRegResURI }; 

    uint32_t resCode;
    uint32_t serverId;//lbsLinkdMgr的serverId

    virtual void marshal(sox::Pack &pk) const
    {
      pk << resCode;
      pk << serverId;
    }

    virtual void unmarshal(const sox::Unpack &up)
    {
      up >> resCode;
      up >> serverId;
    }
  };

}
}

