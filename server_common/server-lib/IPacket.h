#ifndef SERVER_IPACKET_H_
#define SERVER_IPACKET_H_

#include "common/packet.h"
#include <vector>
#include "core/corelib/RouterBase.h"

namespace core{

struct IPacketGc{
	virtual ~IPacketGc(){}
	virtual void gcAnswerPacket(IPacketBase *) = 0;
	virtual void gcRoutePacket(IPacketBase *) = 0;
	virtual void gcSPacket(IPacketBase *) = 0;
	virtual void gcUPacket(IPacketBase *) = 0;
    //add by long
    virtual void GcSyncPacket(IPacketBase *) = 0;
    virtual void GcAckPacket(IPacketBase *) = 0;
    virtual void GcProxyPacket(IPacketBase *) = 0;

	// add by Grj
	virtual void gcRouteToSrvsPacket(IPacketBase *) = 0;
	virtual void GcRoundRobinPacket(IPacketBase *) = 0;
	// end;
};

struct IPacketProto{
	virtual ~IPacketProto(){}
	// add by Grj
	virtual IPacketBase *GetRouteToSrvsPacket(const std::string& from, const std::string& to, uint32_t uri, const sox::Marshallable& obj) = 0;
	virtual IPacketBase *GetRoundRobinPacket(const std::string& from, const std::string& to, uint32_t uUri, const sox::Marshallable& obj) = 0;
	// end
	virtual IPacketBase *getAnswerPacket(uint32_t uri, const sox::Marshallable &obj) = 0;
	virtual IPacketBase *getAnswerPacket(uint32_t uri, uint16_t resCode, const sox::Marshallable &obj) = 0;
	virtual IPacketBase *getRoutePacket(const std::string &from, const std::string &to, uint32_t uri, uint16_t resCode, const sox::Marshallable &obj) = 0;
	virtual IPacketBase *getRoutePacket(const std::string &from, const std::string &to, uint32_t uri, const sox::Marshallable &obj) = 0;
	virtual IPacketBase *getRoutePacket(const std::string &from, const std::string &to, uint32_t uri, uint16_t err) = 0;
	virtual IPacketBase *getSPacket(const std::string &from, uint32_t s, uint32_t uri, const sox::Marshallable &obj) = 0;
	virtual IPacketBase *getUPacket(const std::string from, const std::vector<uint32_t> &uids, uint32_t uri, const sox::Marshallable &) = 0;
	//add by kdjie, 2009.12.16
	virtual IPacketBase *getSPacket(const std::string &from, uint32_t s, uint32_t uri, const std::string &str) = 0;
	//add end

    //add by long
    virtual IPacketBase *GetSyncPacket(uint32_t uSvrId, uint32_t uUri, const sox::Marshallable &objMsg) = 0;
    virtual IPacketBase *GetAckPacket() = 0;
    virtual IPacketBase *GetProxyPacket() = 0;
    //end

	//add by lbc
	virtual IPacketBase *GetMultiSyncPacket(uint32_t uSvrId, uint32_t uUri, const sox::Marshallable &objMsg) = 0;
	//end

	virtual void gcPackets(std::vector<IPacketBase *> &gcs) = 0;
};

struct IPacketProtoAware{
protected:
	IPacketProto *pp;
public:
	IPacketProtoAware() : pp(NULL) {}

	virtual ~IPacketProtoAware(){}
	virtual void setPacketProto(IPacketProto *p){
		pp = p;
	}
};

}
#endif

