#include "Notifier.h"
#include "core/sox/snox.h"
#include "core/sox/logger.h"
#include "common/core/sender.h"
#include "packet.h"

static sox::Sockethelper s_socket;


struct PCS_NofityMessage: public sox::Marshallable
{
    uint32_t m_uUid;
    std::string m_strMsg;

    virtual void marshal(sox::Pack &p) const
    {
        p << m_uUid << m_strMsg;
    }

    virtual void unmarshal(const sox::Unpack &p)
    {
    }
};

void NotifyAll(const char *szFmt, ...)
{
    va_list va;
    char buf[1024];
    int len;

    va_start(va, szFmt);
    len = vsnprintf(buf, sizeof(buf), szFmt, va);
    va_end(va);

    std::string strMsg(buf, len);
    NotifyOne(0, strMsg);
}
            
void NotifyAll(const std::string &strMsg)
{
    NotifyOne(0, strMsg);
}
                
void NotifyOne(uint32_t uUid, const char *szFmt, ...)
{
    va_list va;
    char buf[1024];
    int len;

    va_start(va, szFmt);
    len = vsnprintf(buf, sizeof(buf), szFmt, va);
    va_end(va);

    std::string strMsg(buf, len);
    NotifyOne(uUid, strMsg);
}
                    
void NotifyOne(uint32_t uUid, const std::string &strMsg)
{
    static std::string s_strDefHost = "127.0.0.1";
    static int s_nDefPort = 9394;
    log(Info, "[OnNotify]: %s", strMsg.c_str());

    try
    {
    if (!s_socket.isValid() && !s_socket.isConnected())
    {
        log(Error, "[NotifyOne]: socket not connected");
        s_socket.socket();
        s_socket.setblocking(true);
        if (!s_socket.connect(s_strDefHost, s_nDefPort))
        {
            return;
        }
    }

    PCS_NofityMessage obj;
    obj.m_uUid = uUid;
    obj.m_strMsg = strMsg;
    core::Sender sd(9394, obj);
    sd.endPack();

    const char *szBuf = sd.header();
    int nSent = sd.headerSize() + sd.bodySize();
    while (nSent > 0)
    {
        int nRet = s_socket.send(szBuf, nSent);
        if (nRet < 0)
        {
            log(Error, "[NotifyOne]: write error!");
            s_socket.shutdown();
        }
        else
        {
            nSent -= nRet;
        }
    }
    //log(Info, "[NotifyOne]: message sent");
    }
    catch (std::exception e)
    {
        log(Error, "[NotifyOne]: exception: %s", e.what());
    }
}



