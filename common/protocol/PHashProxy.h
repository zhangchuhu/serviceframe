#pragma once

#include "core/base_svid.h"
#include "packet.h"
#include <string>
#include <vector>

namespace protocol
{
namespace hashproxy
{
        struct PCS_HashRequest: public sox::Marshallable
        {
                enum { uri = (1 << 8) | HASHPROXY_SVID};
                std::vector<uint32_t> m_vecKey;
                std::vector<uint32_t> m_vecSvid; //¿±êrverµÄuffix, Ï·¢¸ø¸öfix£»È¹û£¬·¢¸ø¸ö              uint32_t m_uUri

                uint32_t m_uUri;
                std::string m_strPacket;

                virtual void marshal(sox::Pack &p) const 
                {
                        marshal_container(p, m_vecKey);
                        marshal_container(p, m_vecSvid);
                        p << m_uUri << m_strPacket;
                }

                virtual void unmarshal(const sox::Unpack &p) 
                {
                        unmarshal_container(p, std::back_inserter(m_vecKey));
                        unmarshal_container(p, std::back_inserter(m_vecSvid));
                        p >> m_uUri >> m_strPacket;
                }
        };
}

}
