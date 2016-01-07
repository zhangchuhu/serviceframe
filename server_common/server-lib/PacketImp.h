#ifndef PACKETIMP_H_
#define PACKETIMP_H_

#include "../protocol/prouter.h"
#include "common/core/iserver.h"
#include "IPacket.h"
#include <list>

namespace server{
	namespace router{
		struct PRouter;
		struct PServerIdRouter;
		struct PUidsRouter;
		struct PAnswer;
		struct PToServersRoute;
	}
}
namespace core{
	class PacketImp:public IPacketProto,
					public IPacketGc
	{
		std::list<server::router::PRouter *> rrs;
		std::list<server::router::PServerIdRouter *> rss;
		std::list<server::router::PUidsRouter *> rus;
		std::list<server::router::PAnswer *> ras;
		std::list<server::router::PToServersRoute *> rtss;   // add by Grj
		std::list<server::router::PRoundRobinRouter *> m_listRoundRobinRouter;

        //add by long
        std::list<server::router::PSS_SyncRouter *> m_listSyncRouter;
        std::list<server::router::PSS_AckRouter *> m_listAckRouter;
        std::list<server::router::PProxyRouter *> m_listProxyRouter;

	public:

		// add by Grj
		virtual IPacketBase *GetRouteToSrvsPacket(const std::string& from, const std::string& to, uint32_t uri, const sox::Marshallable& obj);
		virtual void gcRouteToSrvsPacket(IPacketBase *);

		virtual IPacketBase *GetRoundRobinPacket(const std::string& from, const std::string& to, uint32_t uUri, const sox::Marshallable& obj);
		virtual void GcRoundRobinPacket(IPacketBase *);
		// end

		virtual IPacketBase *getAnswerPacket(uint32_t uri, const sox::Marshallable &obj);
		virtual IPacketBase *getAnswerPacket(uint32_t uri, uint16_t resCode, const sox::Marshallable &obj);
		virtual IPacketBase *getRoutePacket(const std::string &from, const std::string &to, uint32_t uri, uint16_t resCode, const sox::Marshallable &obj);
		virtual IPacketBase *getRoutePacket(const std::string &from, const std::string &to, uint32_t uri, const sox::Marshallable &obj);
		virtual IPacketBase *getRoutePacket(const std::string &from, const std::string &to, uint32_t uri, uint16_t err);
		virtual IPacketBase *getSPacket(const std::string &from, uint32_t s, uint32_t uri, const sox::Marshallable &obj);
		virtual IPacketBase *getUPacket(const std::string from, const std::vector<uint32_t> &uids, uint32_t uri, const sox::Marshallable &);
		//add by kdjie, 2009.12.16
		virtual IPacketBase *getSPacket(const std::string &from, uint32_t s, uint32_t uri, const std::string &str);
		//add end
		
		virtual void gcPackets(std::vector<IPacketBase *> &gcs);
		virtual void gcAnswerPacket(IPacketBase *);
		virtual void gcRoutePacket(IPacketBase *);
		virtual void gcSPacket(IPacketBase *);
		virtual void gcUPacket(IPacketBase *);

        //add by long
        virtual IPacketBase *GetSyncPacket(uint32_t uSvrId, uint32_t uUri, const sox::Marshallable &objMsg);
        virtual IPacketBase *GetAckPacket();
        virtual IPacketBase *GetProxyPacket();
        virtual void GcSyncPacket(IPacketBase *);
        virtual void GcAckPacket(IPacketBase *);
        virtual void GcProxyPacket(IPacketBase *);

		//add by lbc
		virtual IPacketBase *GetMultiSyncPacket(uint32_t uSvrId, uint32_t uUri, const sox::Marshallable &objMsg);
		//end
	};

}
#endif

