#ifndef PSConfigSvr_h__
#define PSConfigSvr_h__

#include "common/packet.h" //sox::Marshallable
#include "common/core/base_svid.h"
#include "common/res_code.h"

#include "server_im/imlib/ISConfigSvr.h"
#include "protocol/common/protocol/zip_tpl.h"

namespace protocol
{
namespace sconfigsvr
{
	//ΩË”√IMROUTERµƒSVID£¨±‹√‚¿À∑—
	
	struct PSS_RequestConfigInfo : public sox::Marshallable
	{
		enum {uri = (91 << 8 | protocol::IMROUTER_SVID)};

		std::string m_strFrom;
		std::string m_strConfMD5;
		bool m_bAsyncRequest;
		
		PSS_RequestConfigInfo():m_bAsyncRequest(true){}
				
		virtual void marshal(sox::Pack &p) const
		{
			p << m_bAsyncRequest << m_strFrom << m_strConfMD5;
		}

		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> m_bAsyncRequest >> m_strFrom >> m_strConfMD5;
		}	
	};
	
	struct PSS_ConfigInfoRes : public sox::Marshallable
	{
		enum {uri = (92 << 8 | protocol::IMROUTER_SVID)};
		
		std::string m_strConfMd5;
		std::string m_strConfXml;
		std::string m_strChangeXml;
		std::string m_strFrom;
		uint32_t m_uIP;

		virtual void marshal(sox::Pack &p) const
		{
			p << m_strConfMd5 << m_strFrom << m_uIP;
            p.push_varstr32(m_strConfXml.data(), m_strConfXml.length());
            p.push_varstr32(m_strChangeXml.data(), m_strChangeXml.length());			
		}

		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> m_strConfMd5 >> m_strFrom >> m_uIP;
			m_strConfXml = p.pop_varstr32();
			m_strChangeXml = p.pop_varstr32();
		}	
	};
	
	typedef ZipMarshal<PSS_ConfigInfoRes, (93<< 8) | protocol::IMROUTER_SVID> PSS_ConfigInfoZippedRes;

	struct PSS_ConfigInfoReset : public sox::Marshallable
	{
		enum {uri = (95 << 8 | protocol::IMROUTER_SVID)};
		
		std::string m_strConfMd5;

		virtual void marshal(sox::Pack &p) const
		{
			p << m_strConfMd5;
		}

		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> m_strConfMd5;
		}	
		
	};
	
}
}
#endif // PSConfigSvr_h__