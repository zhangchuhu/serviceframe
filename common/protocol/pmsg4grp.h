#ifndef __PMSG4GRP_H__
#define __PMSG4GRP_H__

#include "common/protocol/pmsg.h"
#include "core/sox/logger.h"

//群临时聊天消息
namespace protocol{
	namespace im{
		struct TextChat4Grp : public protocol::session::TextChatFormat
		{
			uint32_t gid;
			UniString msg;
			uint32_t sd;
			uint32_t imid; //增加yy号
			
			virtual void marshal(sox::Pack &pak) const 
			{
				TextChatFormat::marshal(pak);
				pak << 	gid << msg << sd; 
				pak << imid; //压yy号
			}
			
			virtual void unmarshal(const sox::Unpack &pak) 
			{ 
				TextChatFormat::unmarshal(pak); 
				pak >> gid >> msg >> sd; 
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
					//log(Info, "TextChat4Grp::unmarshal can't get imid");
				}
			}
			
			std::string toString() const 
			{ 
				sox::PackBuffer bf; 
				sox::Pack pk(bf); 
				this->marshal(pk); 
				return std::string(pk.data(), pk.size()); 
			}

			static TextChat4Grp fromString(const std::string &str) 
			{
				sox::Unpack up(str.data(), str.size());
				TextChat4Grp tcg;
				tcg.unmarshal(up);
				return tcg;
			}
		};
	}
}

#endif