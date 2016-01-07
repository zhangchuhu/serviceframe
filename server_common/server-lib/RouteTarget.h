#ifndef ROUTE_TARGET_H_
#define ROUTE_TARGET_H_
#include "common/core/ibase.h"
#include "server_common/protocol/prouter.h"
#include "server_common/protocol/WatchDog.h"
#include <vector>

namespace core{
	class IRouteTarget: public IFormTarget{
	public:
		void route(const std::string &from, const std::string &to, uint32_t u, const sox::Marshallable &obj);

		void routeErr(const std::string &to, uint32_t u, uint16_t resCode);

		void route(const std::string &to, uint32_t u, const sox::Marshallable &obj);

		void routeU(uint32_t uid, uint32_t imlinkd, uint32_t uri, const sox::Marshallable &obj);
		void routeU2(uint32_t uid, uint32_t imlinkd, uint32_t uri, const sox::Marshallable &obj, uint32_t uRouter = 0);

		//void routeU(const std::map<uint32_t, uint32_t> &uid_imlinkd, uint32_t uri, const sox::Marshallable &obj);

		//void routeU(uint32_t uid, uint32_t uri, const sox::Marshallable &obj);

		//void routeU(const std::vector<uint32_t> &uids, uint32_t uri, const sox::Marshallable&);
		//void routeUErr(uint32_t uid, uint32_t uri, uint16_t err);

		void routeS(uint32_t serverId, uint32_t uri, const sox::Marshallable &obj);

		void routeS(const std::string &from, uint32_t serverId, uint32_t uri, const sox::Marshallable &obj);

		// add by Grj
		std::string GetFrom();
                void SetFrom(const std::string &from);

		void RouteToServers(const std::string& to, uint32_t uri, const sox::Marshallable& obj);

		// 轮询方式发包
		virtual void RouteRoundRobin(const std::string& to, uint32_t uri, const sox::Marshallable& obj);
		virtual void RouteRoundRobin(const std::string& from, const std::string& to, uint32_t uri, const sox::Marshallable& obj);
		// end

		//add by long
        server::router::CProxyInfo *GetProxyInfo();
        //将proxy转发到下一层
        void RouteWithProxyInfo(uint32_t uServerId, uint32_t uUri, const sox::Marshallable& objMsg);
        void RouteWithProxyInfo(const std::string &strTo, uint32_t uUri, const sox::Marshallable& objMsg);

        //answer上一层的server
        void AnswerWithProxyInfo(uint32_t uUri, const sox::Marshallable& objMsg);
        void AnswerWithProxyInfo(uint32_t uUri, uint16_t uResCode, const sox::Marshallable& objMsg);

        void RouteReliable(uint32_t uServerId, uint32_t uUri, const sox::Marshallable &objMsg);
        void AnswerProxyReliable(uint32_t uUri, const sox::Marshallable& objMsg);
        void AnswerProxyReliable(uint32_t uUri, uint16_t uResCode, const sox::Marshallable& objMsg);

        void RouteLog(int uLevel, const char *szFmt, ...);
        void RouteLog(int uLevel, const std::string &strMsg);

		//end

		//void uRoute(uint32_t uid, const std::string &to, uint32_t u, const sox::Marshallable &obj);

		//add by kdjie, 2009.12.16
		//用于发送序列化后的二进制数据
		void routeS(const std::string &from, uint32_t serverId, uint32_t uri, const std::string &strPacket);
		//add end

		// add by lbc
		void routeM(uint32_t uServerId, uint32_t uUri, const sox::Marshallable &objMsg);
		// add end

		//使用Wrap头发送方式
		virtual void RouteWithWrapHead(uint32_t uServerId, uint32_t uUri, const sox::Marshallable& objMsg);
		virtual void RouteWithWrapHead(const std::string &strTo, uint32_t uUri, const sox::Marshallable& objMsg, bool bRoundRobin = false);
		virtual void AnswerWithWrapHead(uint32_t uUri, const sox::Marshallable& objMsg);
		virtual void RouteWithWrapHeadTrans(uint32_t uServerId, uint32_t uUri, const sox::Marshallable& objMsg); //透明方式
		virtual void RouteWithWrapHeadTrans(const std::string &strTo, uint32_t uUri, const sox::Marshallable& objMsg); //透明方式
	};
}
#endif
