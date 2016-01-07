#pragma once
#include <time.h>
#include "core/ibase.h"
#include "core/sox/snox.h"
#include "server_common/server-lib/iclient.h"
#include "core/iserver.h"
#include "server_common/server-lib/BRouteAppContext.h"
#include <vector>
#include <map>
#include "Wrapper.h"

using namespace core;

struct CServerData: public sdaemon::FavorStrSV
{
    time_t m_uLastRecv;
};


class CWatchDog:
    public core::BRouteAppContext,
    public core::IDaemonServerAwareW,
    public core::IDaemonClientAwareW,
    public sox::Handler
{
public:
    CWatchDog();

    //daemon server event
    virtual void onServerRegistered();

    //daemon client event
    virtual void onRefreshSV();

    virtual void onRecoverRefreshSV();

    virtual void onAddSV(const sdaemon::FavorStrSV & fs);

    virtual void onServerRemovedSV(const sdaemon::FavorStrSV & fs);

    virtual void onServerStatusChangeSV(const sdaemon::FavorStrSV & fs);

    //time to check serivce
    virtual void handle(int nEvent);

    //check if server response
    virtual IWriter *requestDispatch(Request &request, IConn *conn);

    //wrapper util
    template<typename TClass>
    void AddServiceWrapper(const char *szName, TClass &clz, void (TClass::*mf)(uint32_t))
    {
        clz.setWriter(writer);
        std::string strName = szName;
        IRequestWrapper *p = new TRequestWrapper<TClass>(clz, mf);
        m_mapRequest[szName] = p;
    }

    template<typename TMarshallable, typename TClass>
    void AddResponseWrapper(uint32_t uUri, TClass &clz, void (TClass::*mf)(TMarshallable *))
    {
        IResponseWrapper *p = new TResponseWrapper<TMarshallable, TClass>(clz, mf);
        m_mapResponse[uUri] = p;
    }

private:
    uint32_t m_uRandom;
    
    //serverid => server info
    std::map<uint32_t, CServerData> m_mapServerData;

    //suffix => request wrapper
    std::map<std::string, IRequestWrapper *> m_mapRequest;
    
    //uri => response wrapper
    std::map<uint32_t, IResponseWrapper *> m_mapResponse;
};


#define ADD_SERVICE_WRAPPER(NAME, CLASS, FUNCTION)\
    AddServiceWrapper(NAME, CLASS, FUNCTION);\
    log(Info, "[CWatchDog::AddServiceWrapper]: suffix[" #NAME "] => " #FUNCTION);

#define ADD_RESPONSE_WRAPPER(PCS_XXX, CLASS, FUNCTION)\
    AddResponseWrapper<PCS_XXX>((uint32_t) PCS_XXX::uri, CLASS, FUNCTION);\
    log(Info, "[CWatchDog::AddResponseWrapper]: uri[" #PCS_XXX "] => " #FUNCTION);


