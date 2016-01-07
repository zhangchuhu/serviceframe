#ifndef PMSG_H_
#define PMSG_H_

#include "common/packet.h"
#include "common/misc.h"
#ifdef WIN32
#include "macros.h"
#endif

namespace protocol {
namespace session {
struct TextChatFormat : public sox::Marshallable {
public:
	
#ifdef WIN32

	TextChatFormat(const LOGFONT& lf) {
		clr = 0;

		setLogfont(lf);
	}
#endif

	TextChatFormat(void) {
		clr = 0;
	}
	
public:
	
#ifdef WIN32	
	void setLogfont(LOGFONT lf) {
		height = (uint32_t)lf.lfHeight;
		ft.name = lf.lfFaceName;
		if (lf.lfWeight == FW_BOLD)
			ft.effects |= protocol::FE_BOLD;
		if (lf.lfUnderline)
			ft.effects |= protocol::FE_UNDERLINE;
		if (lf.lfItalic)
			ft.effects |= protocol::FE_ITALIC;
		if (lf.lfStrikeOut)
			ft.effects |= protocol::FE_STRIKETHROUGH;
	}
#endif


	virtual void marshal(sox::Pack &pak) const {
		pak<<ft.effects<<ft.name<<clr<<height;
	}
	virtual void unmarshal(const sox::Unpack &pak) {
		pak>>ft.effects>>ft.name>>clr>>height;
	}

#ifdef WIN32

	LOGFONT toLogfont(void) const {
		LOGFONT lf;
		ZeroMemory(&lf, sizeof(LOGFONT));
		::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
		::wcsncpy(lf.lfFaceName, ft.name.c_str(),31);
		lf.lfFaceName[31]=0;
		lf.lfWeight = include(ft.effects, protocol::FE_BOLD)?FW_BOLD:FW_NORMAL;
		lf.lfItalic = include(ft.effects, protocol::FE_ITALIC);
		lf.lfUnderline = include(ft.effects, protocol::FE_UNDERLINE);
		lf.lfStrikeOut = include(ft.effects, protocol::FE_STRIKETHROUGH);
		lf.lfHeight = (LONG)height;
		return lf;
	}
#endif


	COLOR getColor(void) const
	{
		return clr;
	}

	void setColor(COLOR c)
	{
		clr = c;
	}
public:
	FONT ft;

	COLOR clr;

	uint32_t height;
};

struct TextChat : public TextChatFormat {
	UniString msg;
	uint32_t sd;

public:
	TextChat& operator=(const TextChatFormat& tf)
	{
		ft = tf.ft;
		clr= tf.clr;
		height = tf.height;
		return *this;
	}

	virtual void marshal(sox::Pack &pak) const
	{
		TextChatFormat::marshal(pak);
		pak << msg << sd;
	}
	virtual void unmarshal(const sox::Unpack &pak)
	{
		TextChatFormat::unmarshal(pak);
		pak >> msg >> sd;
	};

	std::string toString() const {
		sox::PackBuffer bf;
		sox::Pack pk(bf);
		this->marshal(pk);
		return std::string(pk.data(), pk.size());
	}

	static TextChat fromString(const std::string &str) {
		sox::Unpack up(str.data(), str.size());
		TextChat tc;
		tc.unmarshal(up);
		return tc;
	}

};

struct VoiceChat: public sox::Marshallable
{
public:
	VoiceChat(void)
	{
		fec_id = 0;
		type= 0;
	}

	virtual void marshal(sox::Pack &pak) const
	{
		pak << fec_id << type << payload;
	}
	virtual void unmarshal(const sox::Unpack &pak)
	{
		pak >> fec_id >> type >> payload;
	};

	std::string toString() const {
		sox::PackBuffer bf;
		sox::Pack pk(bf);
		this->marshal(pk);
		return std::string(pk.data(), pk.size());
	}

	static VoiceChat fromString(const std::string &str) {
		VoiceChat tc;
		if(!str.empty())
		{
			sox::Unpack up(str.data(), str.size());
			tc.unmarshal(up);
		}	
		return tc;
	}
public:
	uint16_t fec_id;
	uint16_t type;
	std::string payload;
};
}
}
#endif /*PMSG_H_*/
