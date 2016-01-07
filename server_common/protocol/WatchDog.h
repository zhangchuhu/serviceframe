#pragma once
#include "packet.h"
#include <string>

namespace server
{
    struct PSS_ServerPing: public sox::Marshallable
    {
        enum {uri = (1 << 8) | 0};
        uint32_t m_timestamp;
        
        PSS_ServerPing()
        {
            m_timestamp = 0;
        }
        PSS_ServerPing(uint32_t time)
        {
            m_timestamp = time;
        }

        virtual void marshal(sox::Pack &p) const
        {
            p << m_timestamp;
        }

        virtual void unmarshal(const sox::Unpack &p)
        {
            if (p.size() > 0)
            {
                p >> m_timestamp;
            }
            else
            {
				m_timestamp = 0;
            }
        }                   
    };

    struct PSS_ServerPingRes: public sox::Marshallable
    {
        enum {uri = (2 << 8) | 0};
        uint32_t m_timestamp;
        
        PSS_ServerPingRes()
        {
            m_timestamp = 0;
        }
        PSS_ServerPingRes(uint32_t time)
        {
            m_timestamp = time;
        }

        virtual void marshal(sox::Pack &p) const
        {
            p << m_timestamp;
        }

        virtual void unmarshal(const sox::Unpack &p)
        {
            if (p.size() > 0)
            {
                p >> m_timestamp;
            }
            else
            {
				m_timestamp = 0;
            }
        }                   

    };

    struct PSS_ServerLog: public sox::Marshallable
    {
        enum {uri = (3 << 8) | 0};
        std::string m_strMsg;
        uint32_t m_uLevel;

        PSS_ServerLog()
        {
            m_uLevel = Debug;
        }                   
        virtual void marshal(sox::Pack &p) const
        {
            p << m_strMsg << m_uLevel;
        }

        virtual void unmarshal(const sox::Unpack &p)
        {
            p >> m_strMsg;
            if (p.size() > 0)
            {
                p >> m_uLevel;
            }               
        }                   
    };
}

