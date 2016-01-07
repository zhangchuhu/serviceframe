#ifndef BACK_ROUTER_WRITER_H
#define BACK_ROUTER_WRITER_H

#include "common/core/iserver.h"
#include "common/protocol/prouter.h"
#include "server_common/protocol/prouter.h"
#include "ProtoWriter.h"

#include "server_common/protocol/PWrapRequest.h"

namespace core{

	class ABRouteWriter:public ProtoWriter, 
						public core::IDaemonServerAware
	{

		std::string tmpFrom;
        server::router::CProxyInfo *m_pProxyInfo;
		protocol::imlinkd::PSS_WrapRequest *m_pWrapHead;

	public:
		ABRouteWriter();
		
		virtual void answer(uint32_t uri, uint16_t ResCode, const sox::Marshallable &obj);
		virtual void answer(uint32_t uri, const sox::Marshallable &obj);
		virtual void answerErr(uint32_t uri, uint16_t ResCode);

		virtual void route(const std::string &from, const std::string &to, uint32_t uri, const sox::Marshallable &);
		virtual void route(const std::string &to, uint32_t uri, const sox::Marshallable &);
		virtual void routeS(uint32_t s, uint32_t u, const sox::Marshallable &);
		virtual void routeS(const std::string &from, uint32_t s, uint32_t u, const sox::Marshallable &);
		//add by kdjie, 2009.12.15
		//用于发送序列化后的二进制数据
		virtual void routeS(const std::string &from, uint32_t s, uint32_t u, const std::string &str);
		//add end
		//virtual void routeU(const std::vector<uint32_t> &uids, uint32_t uri, const sox::Marshallable &);
		virtual void routeU(uint32_t uid, uint32_t imlinkd, uint32_t uri, const sox::Marshallable &m);
		virtual void routeU2(uint32_t uid, uint32_t imlinkd, uint32_t uri, const sox::Marshallable &m, uint32_t uRouter = 0);
		//virtual void routeU(const std::map<uint32_t, uint32_t> &uid_imlinkd, uint32_t uri, const sox::Marshallable &m);
		virtual void routeErr(const std::string &to, uint32_t uri, uint16_t resCode);
		/*virtual void icmp(const common::router::PICMP &err);*/

		// add by Grj 向一类服务器广播
		virtual void RouteToServers(const std::string& to, uint32_t uri, const sox::Marshallable& obj);

		// 轮询方式发包
		virtual void RouteRoundRobin(const std::string& to, uint32_t uri, const sox::Marshallable& obj);
		virtual void RouteRoundRobin(const std::string& from, const std::string& to, uint32_t uri, const sox::Marshallable& obj);
		
		virtual std::string GetFrom();
		const std::string &getFrom() const;
		// add by long
		virtual server::router::CProxyInfo *GetProxyInfo();
		virtual void SetProxyInfo(server::router::CProxyInfo *);
        
        //将proxy转发到下一层
        virtual void RouteWithProxyInfo(uint32_t uServerId, uint32_t uUri, const sox::Marshallable& objMsg);
		virtual void RouteWithProxyInfo(const std::string &strTo, uint32_t uUri, const sox::Marshallable& objMsg);

        //answer上一层的server
        virtual void AnswerWithProxyInfo(uint32_t uUri, const sox::Marshallable& objMsg);
        virtual void AnswerWithProxyInfo(uint32_t uUri, uint16_t uResCode, const sox::Marshallable& objMsg);

        //route reliable!
        virtual void RouteSync(uint32_t uSvrId, uint32_t uUri, const sox::Marshallable& objMsg);
        virtual void RouteAck(server::router::PSS_SyncRouter *p, uint32_t uExpectId, uint32_t uImbridgeId);
        //answer reliable!
        virtual void AnswerProxyReliable(uint32_t uUri, const sox::Marshallable& objMsg);
        virtual void AnswerProxyReliable(uint32_t uUri, uint16_t uResCode, const sox::Marshallable& objMsg);
		//end

		// add by lbc
		// route reliable if multiple paths are available
		virtual void RouteMultiSync(uint32_t uSvrId, uint32_t uUri, const sox::Marshallable& objMsg);
		// end

		virtual void setFrom(const std::string &f);

		//add by kdjie
		virtual protocol::imlinkd::PSS_WrapRequest *GetWrapHead();
		virtual void SetWrapHead(protocol::imlinkd::PSS_WrapRequest *pWrapHead);

		//使用Wrap头发送方式
		virtual void RouteWithWrapHead(uint32_t uServerId, uint32_t uUri, const sox::Marshallable& objMsg);
		virtual void RouteWithWrapHead(const std::string &strTo, uint32_t uUri, const sox::Marshallable& objMsg, bool bRoundRobin = false);
		virtual void AnswerWithWrapHead(uint32_t uUri, const sox::Marshallable& objMsg);
		virtual void RouteWithWrapHeadTrans(uint32_t uServerId, uint32_t uUri, const sox::Marshallable& objMsg); //透明方式
		virtual void RouteWithWrapHeadTrans(const std::string &strTo, uint32_t uUri, const sox::Marshallable& objMsg); //透明方式
		//add end
	protected:
		std::string getName();
	};

	class RouteWriter: public ABRouteWriter{

	};
}
#endif

