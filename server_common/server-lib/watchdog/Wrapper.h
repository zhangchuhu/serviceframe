#pragma once
#include "packet.h"
#

struct IRequestWrapper
{
    virtual void operator()(uint32_t uSvrID) = 0;
};

template <typename TClass>
struct TRequestWrapper: public IRequestWrapper
{
    typedef void (TClass::*TMethod)(uint32_t);
    TClass &clz_;   //class type
    TMethod mf_;   //member function type
    
    TRequestWrapper(TClass &clz, TMethod &mf)
        : clz_(clz), mf_(mf)
    {
    }

    //invoke
    virtual void operator()(uint32_t uSvrID)
    {
        (clz_.*mf_)(uSvrID);
    }
};

struct IResponseWrapper
{
    virtual void operator()(const sox::Unpack &p) = 0;
};

template <typename TMarshallable, typename TClass>
struct TResponseWrapper: public IResponseWrapper
{
    typedef void (TClass::*TMethod)(TMarshallable *);
    TClass &clz_;   //class type
    TMethod mf_;   //member function type

public:
    TResponseWrapper(TClass &clz, TMethod &mf)
        : clz_(clz), mf_(mf)
    {
    }

    //invoke
    virtual void operator()(const sox::Unpack &p)
    {
        TMarshallable *arg = new TMarshallable();
        arg->unmarshal(p);
        (clz_.*mf_)(arg);
        delete arg;
    }
};

