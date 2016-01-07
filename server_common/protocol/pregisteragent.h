#ifndef _PREGISTER_AGENT_DEF_H_
#define _PREGISTER_AGENT_DEF_H_

#include "packet.h"
#include <vector>
#include <map>
#include <string>
#include "common/core/base_svid.h"
#include <stdint.h>

namespace server{
namespace registeragent{
	struct AgentRequest : public sox::Marshallable
	{
		enum {uri = (55 << 8) | ::protocol::ROUTE_SVID};
		std::string source_server_name;
		std::string dest_server_name;
		uint32_t groupid;
		uint32_t agent_option;
		uint32_t payload_uri;
		std::string payload;
		std::string reserved;
		virtual void marshal(sox::Pack &p) const{
			p.push_varstr32(source_server_name.data(), source_server_name.size());
			p.push_varstr32(dest_server_name.data(), dest_server_name.size());
			p << groupid;
			p << agent_option;
			p << payload_uri;
			p.push_varstr32(payload.data(), payload.size());
			p.push_varstr32(reserved.data(), reserved.size());
		}
		virtual void unmarshal(const sox::Unpack &p) {
			source_server_name=p.pop_varstr32();
			dest_server_name=p.pop_varstr32();
			p >> groupid;
			p >> agent_option;
			p >> payload_uri;
			payload=p.pop_varstr32();
			reserved=p.pop_varstr32();
		}
	};
	struct ARegisterRequest : public sox::Marshallable
	{
		enum {uri = (56 << 8) | ::protocol::ROUTE_SVID};
		std::string m_contex;
		uint32_t payload_uri;
		std::string payload;
		virtual void marshal(sox::Pack &p) const{
			p.push_varstr32(m_contex.data(), m_contex.size());
			p << payload_uri;
			p.push_varstr32(payload.data(), payload.size());
		}
		virtual void unmarshal(const sox::Unpack &p) {
			m_contex=p.pop_varstr32();
			p >> payload_uri;
			payload=p.pop_varstr32();
		}
	};
	struct AgentReply : public sox::Marshallable
	{
		enum {uri = (57 << 8) | ::protocol::ROUTE_SVID};
		std::string m_contex;
		uint32_t payload_uri;
		std::string payload;
		virtual void marshal(sox::Pack &p) const{
			p.push_varstr32(m_contex.data(), m_contex.size());
			p << payload_uri;
			p.push_varstr32(payload.data(), payload.size());
		}
		virtual void unmarshal(const sox::Unpack &p) {
			m_contex=p.pop_varstr32();
			p >> payload_uri;
			payload=p.pop_varstr32();
		}
	};
	struct RawMarshallb : public sox::Marshallable
	{
		enum {uri = 0};
		RawMarshallb(std::string& dt):dat(dt){};
		~RawMarshallb(){}
		std::string& dat;
		virtual void marshal(sox::Pack &p) const{
			p.push(dat.data(), dat.size());
		}
		virtual void unmarshal(const sox::Unpack &p) {
		}
	};
	
}
}

#endif

