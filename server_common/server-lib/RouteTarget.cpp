#include "RouteTarget.h"
#include "BRouteWriter.h"
#include "UrlHelper.h"
#include "server-lib/id_def.h"
#include <stdarg.h>
#include <stdio.h>

using namespace core;
using namespace server::router;
//add by long

CProxyInfo* IRouteTarget::GetProxyInfo()
{
	ABRouteWriter *bw = (ABRouteWriter *) writer;
	return bw->GetProxyInfo();
}

void IRouteTarget::RouteWithProxyInfo(uint32_t uServerId, uint32_t uUri, const sox::Marshallable& objMsg)
{
    ABRouteWriter *bw = (ABRouteWriter *) writer;
    bw->RouteWithProxyInfo(uServerId, uUri, objMsg); 
}

void IRouteTarget::RouteWithProxyInfo(const std::string &strTo, uint32_t uUri, const sox::Marshallable& objMsg)
{
    ABRouteWriter *bw = (ABRouteWriter *) writer;
    bw->RouteWithProxyInfo(strTo, uUri, objMsg);
}

void IRouteTarget::AnswerWithProxyInfo(uint32_t uUri, const sox::Marshallable& objMsg)
{
    ABRouteWriter *bw = (ABRouteWriter *) writer;
    bw->AnswerWithProxyInfo(uUri, objMsg);
}

void IRouteTarget::AnswerWithProxyInfo(uint32_t uUri, uint16_t uResCode, const sox::Marshallable& objMsg)
{
    ABRouteWriter *bw = (ABRouteWriter *) writer;
    bw->AnswerWithProxyInfo(uUri, uResCode, objMsg);
}

void IRouteTarget::AnswerProxyReliable(uint32_t uUri, const sox::Marshallable& objMsg)
{
    ABRouteWriter *bw = (ABRouteWriter *) writer;
    bw->AnswerProxyReliable(uUri, objMsg);
}

void IRouteTarget::AnswerProxyReliable(uint32_t uUri, uint16_t uResCode, const sox::Marshallable& objMsg)
{
    ABRouteWriter *bw = (ABRouteWriter *) writer;
    bw->AnswerProxyReliable(uUri, uResCode, objMsg);
}

void IRouteTarget::RouteReliable(uint32_t uServerId, uint32_t uUri, const sox::Marshallable &objMsg)
{
    ABRouteWriter *bw = (ABRouteWriter *) writer;
    bw->RouteSync(uServerId, uUri, objMsg);
}

//end

// add by Grj
void IRouteTarget::RouteToServers(const std::string& to, uint32_t uri, const sox::Marshallable& obj)
{
	ABRouteWriter *bw = (ABRouteWriter *)writer;
	bw->RouteToServers(to, uri, obj);
}

void IRouteTarget::RouteRoundRobin(const std::string& to, uint32_t uri, const sox::Marshallable& obj)
{
	ABRouteWriter *bw = (ABRouteWriter *)writer;
	bw->RouteRoundRobin(to, uri, obj);
}

void IRouteTarget::RouteRoundRobin(const std::string& from, const std::string& to, uint32_t uri, const sox::Marshallable& obj)
{
	ABRouteWriter *bw = (ABRouteWriter *)writer;
	bw->RouteRoundRobin(from, to, uri, obj);
}

std::string IRouteTarget::GetFrom()
{
	ABRouteWriter *bw = (ABRouteWriter *)writer;
	return bw->GetFrom();
}

void IRouteTarget::SetFrom(const std::string &from)
{
	ABRouteWriter *bw = (ABRouteWriter *)writer;
	return bw->setFrom(from);
}
// end

void IRouteTarget::route(const std::string &from, const std::string &to, uint32_t u, const sox::Marshallable &obj){
	ABRouteWriter *bw = (ABRouteWriter *)writer;
	bw->route(from, to, u, obj);
}

void IRouteTarget::route(const std::string &to, uint32_t u, const sox::Marshallable &obj){
	ABRouteWriter *bw = (ABRouteWriter *)writer;
	bw->route(to, u, obj);
}

void IRouteTarget::routeErr(const std::string &to, uint32_t u, uint16_t resCode){
	ABRouteWriter *bw = (ABRouteWriter *)writer;
	bw->routeErr(to, u, resCode);
}

void IRouteTarget::routeU(uint32_t uid, uint32_t imlinkd, uint32_t uri, const sox::Marshallable &obj){
	ABRouteWriter *bw = (ABRouteWriter *)writer;
	bw->routeU(uid, imlinkd, uri, obj);
}
void IRouteTarget::routeU2(uint32_t uid, uint32_t imlinkd, uint32_t uri, const sox::Marshallable &obj, uint32_t uRouter)
{
	ABRouteWriter *bw = (ABRouteWriter *)writer;
	bw->routeU2(uid, imlinkd, uri, obj, uRouter);
}

/*
void IRouteTarget::routeU(const std::map<uint32_t, uint32_t> &uid_imlinkd, uint32_t uri, const sox::Marshallable &obj){
	ABRouteWriter *bw = (ABRouteWriter *)writer;
	bw->routeU(uid_imlinkd, uri, obj);
}*/

/*
void IRouteTarget::routeUErr(uint32_t uid, uint32_t uri, uint16_t resCode){
	ABRouteWriter *bw = (ABRouteWriter *)writer;
	bw->route(uid2Url(uid), u, resCode);
}*/

void IRouteTarget::routeS(uint32_t serverId, uint32_t uri, const sox::Marshallable &obj){
	ABRouteWriter *bw = (ABRouteWriter *)writer;
	bw->routeS(serverId, uri, obj);
}

void IRouteTarget::routeS(const std::string &from, uint32_t serverId, uint32_t uri, const sox::Marshallable &obj){
	ABRouteWriter *bw = (ABRouteWriter *)writer;
	bw->routeS(from, serverId, uri, obj);
}

//add by kdjie, 2009.12.16
//用于发送序列化后的二进制数据

void IRouteTarget::routeS(const std::string &from, uint32_t serverId, uint32_t uri, const std::string &strPacket)
{
	ABRouteWriter *bw = (ABRouteWriter *)writer;
	bw->routeS(from, serverId, uri, strPacket);
}
//add end
//


void IRouteTarget::RouteLog(int uLevel, const char *szFmt, ...)
{
    char buf[1024];
    va_list va;
    va_start(va, szFmt);
    size_t n = vsnprintf(buf, sizeof(buf), szFmt, va);
    va_end(va);

    if (n < 0)
    {
        return;
    }

    server::PSS_ServerLog obj;
    obj.m_uLevel = uLevel;
    if (n > sizeof(buf))
    {
        obj.m_strMsg.assign(buf, sizeof(buf));
    }
    else
    {
        obj.m_strMsg.assign(buf, n);
    }
    route(SUFFIX_WATCHDOG, obj.uri, obj);
}

void IRouteTarget::RouteLog(int uLevel, const std::string &strMsg)
{
    server::PSS_ServerLog obj;
    obj.m_uLevel = uLevel;
    obj.m_strMsg = strMsg;
    route(SUFFIX_WATCHDOG, obj.uri, obj);
}

void IRouteTarget::routeM(uint32_t uServerId, uint32_t uUri, const sox::Marshallable &objMsg)
{
	ABRouteWriter *bw = (ABRouteWriter *) writer;
	bw->RouteMultiSync(uServerId, uUri, objMsg);
}

//使用Wrap头发送方式
void IRouteTarget::RouteWithWrapHead(uint32_t uServerId, uint32_t uUri, const sox::Marshallable& objMsg)
{
	ABRouteWriter *bw = (ABRouteWriter *) writer;
	bw->RouteWithWrapHead(uServerId, uUri, objMsg); 
}
void IRouteTarget::RouteWithWrapHead(const std::string &strTo, uint32_t uUri, const sox::Marshallable& objMsg, bool bRoundRobin)
{
	ABRouteWriter *bw = (ABRouteWriter *) writer;
	bw->RouteWithWrapHead(strTo, uUri, objMsg, bRoundRobin); 
}
void IRouteTarget::AnswerWithWrapHead(uint32_t uUri, const sox::Marshallable& objMsg)
{
	ABRouteWriter *bw = (ABRouteWriter *) writer;
	bw->AnswerWithWrapHead(uUri, objMsg); 
}
void IRouteTarget::RouteWithWrapHeadTrans(uint32_t uServerId, uint32_t uUri, const sox::Marshallable& objMsg) //透明方式
{
	ABRouteWriter *bw = (ABRouteWriter *) writer;
	bw->RouteWithWrapHeadTrans(uServerId, uUri, objMsg); 
}
void IRouteTarget::RouteWithWrapHeadTrans(const std::string &strTo, uint32_t uUri, const sox::Marshallable& objMsg) //透明方式
{
	ABRouteWriter *bw = (ABRouteWriter *) writer;
	bw->RouteWithWrapHeadTrans(strTo, uUri, objMsg); 
}
