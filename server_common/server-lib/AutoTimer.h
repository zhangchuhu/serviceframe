#pragma once
#include "core/sox/snox.h"

struct ICallBack
{
    virtual void operator()() = 0;
};

template<typename TClass>
struct TCallBack0: public ICallBack
{
    typedef void (TClass::*TMethod)();
    TClass &_class;
    TMethod _method;

    TCallBack0(TClass &clz, TMethod &mf)
        : _class(clz), _method(mf)
    {
    }

    virtual void operator()()
    {
        (_class.*_method)();
    }
};


template<typename TClass, typename TArg1>
struct TCallBack1: public ICallBack
{
    typedef void (TClass::*TMethod)(const TArg1 &);
    TClass &_class;
    TMethod _method;
    TArg1 _arg1;

    TCallBack1(TClass &clz, TMethod &mf, const TArg1 &arg1)
        : _class(clz), _method(mf), _arg1(arg1)
    {
    }

    virtual void operator()()
    {
        (_class.*_method)(_arg1);
    }
};

class CAutoTimer: public sox::Handler
{
    bool m_bStop;
    int m_nTimeout; //毫秒
    ICallBack *m_pCallBack;

public:
    CAutoTimer()
        : m_bStop(true), m_pCallBack(NULL)
    {
    }

    ~CAutoTimer()
    {
        if (m_pCallBack != NULL)
            delete m_pCallBack;
    }

    void Start(int nTimeout)
    {
        if (m_pCallBack != NULL)
        {
            m_bStop = false;
            m_nTimeout = nTimeout;
            select_timeout(m_nTimeout);
        }
    }

    virtual void handle(int e)
    {
        if((!m_bStop) && (e == sox::SEL_TIMEOUT) && (m_pCallBack != NULL))
        {
            (*m_pCallBack)();
            
            select_timeout(m_nTimeout);
        }
    }

    void Stop()
    {
        m_bStop = true;
    }


    template<typename TClass>
    void Init(TClass *clz, void (TClass::*mf)())
    {
        if(m_pCallBack)
        {
            delete []m_pCallBack;
            m_pCallBack = NULL;
        }
        m_pCallBack = new TCallBack0<TClass>(*clz, mf);
    }

    template<typename TClass, typename TArg1>
    void Init(TClass *clz, void (TClass::*mf)(const TArg1 &), const TArg1 &arg1)
    {     
        if(m_pCallBack)
        {
            delete []m_pCallBack;
            m_pCallBack = NULL;
        }
        m_pCallBack = new TCallBack1<TClass, TArg1>(*clz, mf, arg1);
    }
};

