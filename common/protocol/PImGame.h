#pragma once

#include <vector>
#include <string>
#include "iproperty.h"
#include "protocol/const.h"
#include "core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/PGameInfoType.h"

namespace protocol
{
	namespace imgame
	{
		struct PCS_GetGamePluginInfo : public sox::Marshallable 
		{
			enum {uri = (1 << 8) | protocol::IMGAME_SVID};

			uint32_t	m_version;
			EGameName	m_GameName;    // 游戏名称

			PCS_GetGamePluginInfo() {}
			PCS_GetGamePluginInfo(uint32_t version, EGameName name)
				: m_version(version), m_GameName(name)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p.push_uint8(m_GameName);
				p << m_version;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				m_GameName = (EGameName)p.pop_uint8();	
				p >> m_version;		
			}
		};

		struct PCS_GetGamePluginInfoRes : public sox::Marshallable 
		{
			enum {uri = (2 << 8) | protocol::IMGAME_SVID};

			EGameName	m_GameName;		// 游戏名
			std::string m_strVer;		// 插件Version	
			std::string	m_strWebUrl;	// 下载地址
			std::string m_strMD5;		// MD5

			EGameBuddyConfig m_eWowConfig;  // Wow游戏好友插件设置状态

			PCS_GetGamePluginInfoRes() {}
			PCS_GetGamePluginInfoRes(EGameName name, const std::string& version, std::string& url, 
												std::string& md5)
				: m_GameName(name), m_strVer(version), m_strWebUrl(url), m_strMD5(md5)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p.push_uint8(m_GameName);
				p << m_strVer << m_strWebUrl << m_strMD5;
				p.push_uint8(m_eWowConfig);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				m_GameName = (EGameName)p.pop_uint8();	
				p >> m_strVer >> m_strWebUrl >> m_strMD5;	

				if (p.size() != 0){
					try{
						m_eWowConfig = (EGameBuddyConfig)p.pop_uint8();	
					}catch(...){
						m_eWowConfig = ENUM_WAIT;
					}
				}else{
					m_eWowConfig = ENUM_WAIT;
				}
			}
		};

		struct PCS_ReportGameInfo : public sox::Marshallable 
		{
			enum {uri = (3 << 8) | protocol::IMGAME_SVID};

			uint32_t m_version;
			EGameName	m_GameName;    // 游戏种类
			std::string	m_strGameInfo;  // 游戏信息

			PCS_ReportGameInfo() {}
			PCS_ReportGameInfo(EGameName name, std::string &info)
				: m_version(0), m_GameName(name), m_strGameInfo(info)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p.push_uint8(m_GameName);
				p << m_version << m_strGameInfo;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				m_GameName = (EGameName)p.pop_uint8();	
				p >> m_version >> m_strGameInfo;		
			}
		};

	
		struct PCS_GetBottomInfo : public sox::Marshallable 
		{
			enum {uri = (4 << 8) | protocol::IMGAME_SVID};

			uint32_t	m_version;
			
			EBottomInfoType m_infoType; // 获取的底部信息的类型

			PCS_GetBottomInfo()
				: m_version(0), m_infoType(ENUM_UNKNOWN_TYPE)
			{}

			PCS_GetBottomInfo(EBottomInfoType infoType)
				: m_version(0), m_infoType(infoType)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_version;
				p.push_uint8(m_infoType);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_version;		
				m_infoType = (EBottomInfoType)p.pop_uint8();	
			}
		};

		struct PCS_GetBottomInfoRes : public sox::Marshallable 
		{
			enum {uri = (5 << 8) | protocol::IMGAME_SVID};

			//key	:	“底部信息”的类型
			//value	:	XML data
			std::map<protocol::imgame::EBottomInfoType, std::string> m_mapButtomInfo;

			PCS_GetBottomInfoRes() {}
			PCS_GetBottomInfoRes(const std::map<protocol::imgame::EBottomInfoType, std::string>& dataInfo)
				: m_mapButtomInfo(dataInfo)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p.push_uint32(m_mapButtomInfo.size());
				for (std::map<protocol::imgame::EBottomInfoType, std::string>::const_iterator iter = m_mapButtomInfo.begin(); 
					iter != m_mapButtomInfo.end(); ++iter)
				{
					p.push_uint8(iter->first);
					p << iter->second;
				}
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				uint32_t size = p.pop_uint32();
				for (uint32_t i = 0; i < size; ++i)
				{
					EBottomInfoType infoType = static_cast<protocol::imgame::EBottomInfoType>(p.pop_uint8());
					std::string xmlData;
					p >> xmlData;
					m_mapButtomInfo.insert(std::make_pair(infoType, xmlData));
				}
			}
		};

		struct PCS_SetWowBuddyConfig : public sox::Marshallable 
		{
			enum {uri = (6 << 8) | protocol::IMGAME_SVID};

			uint32_t			m_version;
			EGameBuddyConfig	m_eConfig;

			PCS_SetWowBuddyConfig() {}
			PCS_SetWowBuddyConfig(EGameBuddyConfig config)
				: m_version(0), m_eConfig(config)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p.push_uint8(m_eConfig);
				p << m_version;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				m_eConfig = (EGameBuddyConfig)p.pop_uint8();	
				p >> m_version;		
			}
		};

		struct PCS_SetWowBuddyConfigRes : public sox::Marshallable 
		{
			enum {uri = (7 << 8) | protocol::IMGAME_SVID};

			EGameBuddyConfig	m_eConfig;

			PCS_SetWowBuddyConfigRes() {}
			PCS_SetWowBuddyConfigRes(EGameBuddyConfig config)
				: m_eConfig(config)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p.push_uint8(m_eConfig);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				m_eConfig = (EGameBuddyConfig)p.pop_uint8();	
			}
		};
	}
}

