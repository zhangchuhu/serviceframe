#ifndef PIM_MSGX_H_
#define PIM_MSGX_H_

#ifdef WIN32
#include "common/misc.h"
#include "macros.h"
#endif

#include "common/packet.h"

namespace protocol {
namespace im {

struct ImTextChatX: public sox::Marshallable {

	uint32_t effects;
	std::string fontName;
	uint32_t color;
	uint32_t height;

	std::string m_strMsg;
	//std::wstring  m_strName;
	uint32_t sd;
	
	uint32_t imid; //增加yy号

	virtual void marshal(sox::Pack &pak) const {
		//pak << m_strOthers;
		pak << effects;
		pak.push_uint32(fontName.size());
		pak.push(fontName.data(), fontName.size());

		pak << color << height;

		pak.push_uint32(m_strMsg.size());
		pak.push(m_strMsg.data(), m_strMsg.size());

		pak << sd;

		pak << imid; //压yy号
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		//pak >> m_strOthers;
		pak >> effects;
		uint32_t size = pak.pop_uint32();
		fontName = pak.pop_fetch(size);

		pak >> color >> height;

		size = pak.pop_uint32();
		m_strMsg = pak.pop_fetch(size);
		pak >> sd;

		//尝试解yy号，解不到置为0
		try
		{
			imid = 0;
			if (pak.size() > 0)
			{
				pak >> imid;
			}
		}
		catch (...) {}
	}
	;

	std::string toString() const {
		sox::PackBuffer bf;
		sox::Pack pk(bf);
		this->marshal(pk);
		return std::string(pk.data(), pk.size());
	}

	static ImTextChatX fromString(const std::string &str) {
		sox::Unpack up(str.data(), str.size());
		ImTextChatX tc;
		tc.unmarshal(up);
		return tc;
	}

	std::string toFontString() const {
		sox::PackBuffer bf;
		sox::Pack pk(bf);
		pk << fontName << effects <<  color << height;
		return std::string(pk.data(), pk.size());
	}

	void fromFontString(const std::string &str) {
		sox::Unpack up(str.data(), str.size());
		up >> fontName >> effects >>  color >> height;
	}
};

}
}
#endif /*PIM_MSG_H_*/
