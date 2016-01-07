#ifndef PGTOPIC_MSG_H_
#define PGTOPIC_MSG_H_

#include <string>
#include "common/packet.h"

namespace protocol {
	namespace gmsgcache {
		struct  GTopicTextChat : public sox::Marshallable  {
			std::string msgtext;
			std::string nickname;
			std::string fontname;
			
			uint32_t effect;
			uint32_t charset;
			uint32_t color;
			uint32_t height;

		public:
			GTopicTextChat& operator=(const GTopicTextChat& input)
			{
				msgtext = input.msgtext;
				nickname = input.nickname;
				fontname = input.fontname;
				
				effect = input.effect;
				charset = input.charset;
				color = input.color;
				height = input.height;

				return *this;
			}
			virtual void marshal(sox::Pack &pak) const
			{
				pak << msgtext << nickname << fontname << effect << charset << color << height;
			}
			virtual void unmarshal(const sox::Unpack &pak)
			{
				pak >> msgtext >> nickname >> fontname >> effect >> charset >> color >> height;
			};
			
			std::string toFontString() const {
				sox::PackBuffer bf;
				sox::Pack pk(bf);
				pk << fontname << effect << charset << color << height;
				return std::string(pk.data(), pk.size());
			}

			void fromFontString(const std::string &str) {
				try
				{
					sox::Unpack up(str.data(), str.size());
					up >> fontname >> effect >> charset >> color >> height;
				}
				catch(std::exception e)
				{
					fontname = "Arial";
					effect = 0 ;
					charset = 134;
					color = 0;
					height=-12;
				}
			}
		};
	}
}

#endif

