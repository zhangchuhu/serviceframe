#ifndef PIM_MSG_H_
#define PIM_MSG_H_

#include "common/misc.h"
#include "common/packet.h"
#ifdef WIN32
	#include "macros.h"
#endif
#include "pmsg.h"
#include "core/sox/logger.h"

namespace protocol {
	namespace im {
		struct ImTextChatFormat : public sox::Marshallable {
		public:
#ifdef WIN32	
			ImTextChatFormat(const LOGFONT& lf) {
				clr = 0;

				setLogfont(lf);
			}
			;
#endif

			ImTextChatFormat(void) {
				clr = 0;
			}
			;
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
			;
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
			};

			COLOR getColor(void) const
			{
				return clr;
			}

			void setColor(COLOR c)
			{
				clr = c;
			}
#endif
		public:
			FONT ft;

			COLOR clr;

			uint32_t height;
		};

		struct ImTextChat : public ImTextChatFormat {
			UniString msg;
			UniString name;
			uint32_t sd;
			uint32_t imid; //增加yy号

		public:
			ImTextChat& operator=(const ImTextChatFormat& tf)
			{
				ft = tf.ft;
				clr= tf.clr;
				height = tf.height;
				return *this;
			}

			virtual void marshal(sox::Pack &pak) const
			{
				pak << msg << name << sd;
				ImTextChatFormat::marshal(pak);
				pak << imid; //压yy号
			}
			virtual void unmarshal(const sox::Unpack &pak)
			{
				pak >> msg >> name >> sd;
				ImTextChatFormat::unmarshal(pak);
				//尝试解yy号，解不到置为0
				try
				{
					imid = 0;
					if (pak.size() > 0)
					{
						pak >> imid;
					}
				}
				catch (...)
				{
					log(Info, "ImTextChat::unmarshal can't get imid");
				}
			};

			std::string toString() const {
				sox::PackBuffer bf;
				sox::Pack pk(bf);
				this->marshal(pk);
				return std::string(pk.data(), pk.size());
			}

			static ImTextChat fromString(const std::string &str) {
				sox::Unpack up(str.data(), str.size());
				ImTextChat tc;
				tc.unmarshal(up);
				return tc;
			}
		};

		//多余的，暂时注释，稍后删除
		//struct ImTextChatForTrunk : public ImTextChatFormat {
		//	UniString msg;
		//	uint32_t sd;

		//public:
		//	ImTextChatForTrunk& operator=(const ImTextChatFormat& tf)
		//	{
		//		ft = tf.ft;
		//		clr= tf.clr;
		//		height = tf.height;
		//		return *this;
		//	}

		//	virtual void marshal(sox::Pack &pak) const
		//	{
		//		ImTextChatFormat::marshal(pak);
		//		pak << msg << sd;
		//	}
		//	virtual void unmarshal(const sox::Unpack &pak)
		//	{
		//		ImTextChatFormat::unmarshal(pak);
		//		pak >> msg >> sd;
		//	};

		//	std::string toString() const {
		//		sox::PackBuffer bf;
		//		sox::Pack pk(bf);
		//		this->marshal(pk);
		//		return std::string(pk.data(), pk.size());
		//	}

		//	static ImTextChatForTrunk fromString(const std::string &str) {
		//		sox::Unpack up(str.data(), str.size());
		//		ImTextChatForTrunk tc;
		//		tc.unmarshal(up);
		//		return tc;
		//	}
		//};

		//为了在消息做中传递yy号，IM消息新的消息结构
		struct TextChat4Im : public ImTextChatFormat {
			UniString msg;
			uint32_t sd;
			uint32_t imid; //增加yy号

		public:
			TextChat4Im& operator=(const ImTextChatFormat& tf)
			{
				ft = tf.ft;
				clr= tf.clr;
				height = tf.height;
				return *this;
			}

			virtual void marshal(sox::Pack &pak) const
			{
				ImTextChatFormat::marshal(pak);
				pak << msg << sd;
				pak << imid; //压yy号
			}
			virtual void unmarshal(const sox::Unpack &pak)
			{
				ImTextChatFormat::unmarshal(pak);
				pak >> msg >> sd;
				//尝试解yy号，解不到置为0
				try
				{
					imid = 0;
					if (pak.size() > 0)
					{
						pak >> imid;
					}
				}
				catch (...)
				{
					log(Info, "TextChat4Im::unmarshal can't get imid");
				}
			};

			std::string toString() const {
				sox::PackBuffer bf;
				sox::Pack pk(bf);
				this->marshal(pk);
				return std::string(pk.data(), pk.size());
			}

			static TextChat4Im fromString(const std::string &str) {
				sox::Unpack up(str.data(), str.size());
				TextChat4Im tc;
				tc.unmarshal(up);
				return tc;
			}
		};

		inline TextChat4Im ConvertTo(const protocol::session::TextChat &text, uint32_t uImid)
		{
			TextChat4Im text4im;

			text4im.ft.effects = text.ft.effects;
			text4im.ft.name = text.ft.name;
			text4im.clr = text.clr;
			text4im.height = text.height;
			text4im.msg = text.msg;
			text4im.sd = text.sd;
			text4im.imid = uImid;

			return text4im;
		}
		inline protocol::session::TextChat ConvertTo(const TextChat4Im &text4im)
		{
			protocol::session::TextChat text;

			text.ft.effects = text4im.ft.effects;
			text.ft.name = text4im.ft.name;
			text.clr = text4im.clr;
			text.height = text4im.height;
			text.msg = text4im.msg;
			text.sd = text4im.sd;

			return text;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//下面这部分是给服务器解消息格式使用的

		struct	FONTForServer{
			uint32_t		effects ;
			std::string		name ;
		};

		struct TextChatFormatForServer : public sox::Marshallable {
		public:
			virtual void marshal(sox::Pack &pak) const {
				pak<<ft.effects;
				pak.push_varstr32(ft.name.data(), ft.name.size());
				pak<<clr<<height;
			}
			virtual void unmarshal(const sox::Unpack &pak) {
				pak>>ft.effects;
				ft.name = pak.pop_varstr32();
				pak>>clr>>height;
			}

		public:
			FONTForServer ft;
			COLOR clr;
			uint32_t height;
		};

		//群会话PCS_GChatMsg ImTextChat兼容消息，目前还应用于临时会话离线消息存储
		struct ImTextChatForServer : public TextChatFormatForServer {
			std::wstring msg;
			std::wstring name;
			uint32_t sd;

			virtual void marshal(sox::Pack &pak) const
			{
				pak << msg << name << sd;
				TextChatFormatForServer::marshal(pak);
			}
			virtual void unmarshal(const sox::Unpack &pak)
			{
				pak >> msg >> name >> sd;
				TextChatFormatForServer::unmarshal(pak);
			};

			std::string toString() const {
				sox::PackBuffer bf;
				sox::Pack pk(bf);
				this->marshal(pk);
				return std::string(pk.data(), pk.size());
			}

			static ImTextChatForServer fromString(const std::string &str) {
				sox::Unpack up(str.data(), str.size());
				ImTextChatForServer tc;
				tc.unmarshal(up);
				return tc;
			}
		};

		//群临时会话消息 SayToGrpPeer(protocol::im::TextChat4Grp&)
		struct TextChat4GrpForServer : public TextChatFormatForServer
		{
			uint32_t gid;
			std::string msg;
			uint32_t sd;
			uint32_t imid; //增加yy号

			virtual void marshal(sox::Pack &pak) const 
			{
				TextChatFormatForServer::marshal(pak);
				pak << gid;
				pak.push_varstr32(msg.data(), msg.size());
				pak << sd;
				pak << imid; //压yy号
			}

			virtual void unmarshal(const sox::Unpack &pak) 
			{ 
				TextChatFormatForServer::unmarshal(pak); 
				pak >> gid;
				msg = pak.pop_varstr32();
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

			std::string toString() const 
			{ 
				sox::PackBuffer bf; 
				sox::Pack pk(bf); 
				this->marshal(pk); 
				return std::string(pk.data(), pk.size()); 
			}

			static TextChat4GrpForServer fromString(const std::string &str) 
			{
				sox::Unpack up(str.data(), str.size());
				TextChat4GrpForServer tcg;
				tcg.unmarshal(up);
				return tcg;
			}
		};

		//IM好友消息 Ansy_saytopeer(protocol::session::TextChat&)
		struct TextChatForServer : public TextChatFormatForServer {
			std::string msg;
			uint32_t sd;
			uint32_t imid; //增加yy号

			virtual void marshal(sox::Pack &pak) const
			{
				TextChatFormatForServer::marshal(pak);
				pak.push_varstr32(msg.data(), msg.size());
				pak << sd;
				pak << imid; //压yy号
			}
			virtual void unmarshal(const sox::Unpack &pak)
			{
				TextChatFormatForServer::unmarshal(pak);
				msg = pak.pop_varstr32();
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
			};

			std::string toString() const {
				sox::PackBuffer bf;
				sox::Pack pk(bf);
				this->marshal(pk);
				return std::string(pk.data(), pk.size());
			}

			static TextChatForServer fromString(const std::string &str) {
				sox::Unpack up(str.data(), str.size());
				TextChatForServer tc;
				tc.unmarshal(up);
				return tc;
			}
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
}
#endif /*PIM_MSG_H_*/
