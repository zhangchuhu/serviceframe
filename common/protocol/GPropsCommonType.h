#ifndef PGPROPERTY_COMMONTYPE_H_INCLUDED
#define PGPROPERTY_COMMONTYPE_H_INCLUDED

#include <vector>
#include <string>
#include "common/packet.h"
#include "pkg_tag.h"
#include "common/protocol/PGInfoCommonTypes.h"
#include "common/protocol/PGPropertyType.h"
#include "common/protocol/IGProperty.h"

namespace protocol
{
	namespace gprops
	{
		const static uint32_t SINVALID_ID = 0;

		//服务器端简单群属性结构
		struct CServerGroupSimpleProps : public sox::Marshallable
		{
			uint32_t     m_uGroupId;	 //内部群ID
			std::string  m_strGroupName; //群名字
			uint32_t     m_uLogoIdex;	 //当前群的logo索引值，从1开始计算

			CServerGroupSimpleProps()
			{
				m_uGroupId = SINVALID_ID;
				m_uLogoIdex = (uint32_t)ENUM_DEFULT;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_strGroupName << m_uLogoIdex;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId >> m_strGroupName >> m_uLogoIdex;
			}
		};

		//服务器端完整群属性结构
		struct CServerGroupProps : public sox::Marshallable
		{
			uint32_t     m_uGroupId; //内部群ID
			uint32_t     m_uAliasId; //外部群ID

			uint32_t     m_createTime; //群创建时间，从1970年1月1日开始计算的秒值
			uint32_t     m_uLogoIdex;  //当前群的logo索引值，从1开始计算
			std::string  m_strLogoUrl; //该logo的URL

			std::string  m_strGroupName; //群名字
			std::string  m_strGroupDesc; //群描述
			std::string  m_strGroupBulletin; //群公告

			uint16_t     m_uCategory;  //群大类，从0开始计算
			uint16_t     m_uSubCategory; //群小类，从0开始计算
			bool         m_bAllowAdhocChat;  //是否允许群内临时会话，默认值为true(允许)
			bool         m_bIsPrivate;       //是否允许非分组成员查看，默认值为true(不允许)

			protocol::ginfo::EAuthMode    m_eAuthMod; //群验证模式
			protocol::gprops::ETopicMode m_eTopicMod; //主题模式
			MapChannelInfo	m_uMapChanAuthInfo; //该map存放着频道号和该频道对应的马甲掩码
												//只在验证模式为ENUM_BY_SESSION时有效

			CServerGroupProps()
				:m_uMapChanAuthInfo()
			{
				m_uGroupId			= SINVALID_ID;
				m_uAliasId			= SINVALID_ID;
				m_uLogoIdex			= (uint32_t)ENUM_DEFULT;
				m_createTime		= 0;
				m_bAllowAdhocChat	= true;     // allow
				m_bIsPrivate		= false;	// allow
				m_eAuthMod			= protocol::ginfo::ENUM_NO_AUTH; // no auth
				m_eTopicMod			= protocol::gprops::ENUM_NO_AUTH; // no auth
				m_uCategory			= 0;
				m_uSubCategory		= 0;
			}

			bool IsInvalid() const
			{
				return (m_uGroupId == SINVALID_ID);
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uAliasId << m_createTime
					<< m_strGroupName << m_strGroupDesc	<< m_strGroupBulletin
					<< m_uLogoIdex << m_strLogoUrl
					<< m_uCategory << m_uSubCategory;
				p.push_uint8((uint8_t)m_bAllowAdhocChat);
				p.push_uint8((uint8_t)m_bIsPrivate);
				p.push_uint8((uint8_t)m_eAuthMod);
				p.push_uint8((uint8_t)m_eTopicMod);
				m_uMapChanAuthInfo.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId >> m_uAliasId >> m_createTime
					>> m_strGroupName >> m_strGroupDesc >> m_strGroupBulletin
					>> m_uLogoIdex >> m_strLogoUrl
					>> m_uCategory >> m_uSubCategory;
				m_bAllowAdhocChat = p.pop_uint8() != 0;
				m_bIsPrivate = p.pop_uint8() != 0;
				m_eAuthMod = (protocol::ginfo::EAuthMode)p.pop_uint8();
				m_eTopicMod = (protocol::gprops::ETopicMode)p.pop_uint8();
				m_uMapChanAuthInfo.unmarshal(p);
			}
		};


		//服务器端群属性结构2
		struct CServerGroupProps2 : public sox::Marshallable
		{
			uint32_t     m_uGroupId; // Internal ID
			uint32_t     m_uAliasId; // External ID

			uint32_t     m_createTime;
			uint32_t     m_uLogoIdex;
			std::string  m_strLogoUrl;

			std::string  m_strGroupName;
			std::string  m_strGroupDesc;
			std::string  m_strGroupBulletin;

			uint16_t     m_uCategory;
			uint16_t     m_uSubCategory;
			bool         m_bAllowAdhocChat;  // true:  allow
			bool         m_bIsPrivate;       // true:  not allow

			protocol::ginfo::EAuthMode    m_eAuthMod;
			protocol::gprops::ETopicMode m_eTopicMod;

			CServerGroupProps2()
			{
				m_uGroupId			= SINVALID_ID;
				m_uAliasId			= SINVALID_ID;
				m_uLogoIdex			= (uint32_t)ENUM_DEFULT;
				m_createTime		= 0;
				m_bAllowAdhocChat	= true;     // allow
				m_bIsPrivate		= false;	// allow
				m_eAuthMod			= protocol::ginfo::ENUM_NO_AUTH; // no auth
				m_eTopicMod			= protocol::gprops::ENUM_NO_AUTH; // no auth
				m_uCategory			= 0;
				m_uSubCategory		= 0;
			}

			bool IsInvalid() const
			{
				return (m_uGroupId == SINVALID_ID);
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uAliasId << m_createTime
					<< m_strGroupName << m_strGroupDesc	<< m_strGroupBulletin
					<< m_uLogoIdex << m_strLogoUrl
					<< m_uCategory << m_uSubCategory;
				p.push_uint8((uint8_t)m_bAllowAdhocChat);
				p.push_uint8((uint8_t)m_bIsPrivate);
				p.push_uint8((uint8_t)m_eAuthMod);
				p.push_uint8((uint8_t)m_eTopicMod);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId >> m_uAliasId >> m_createTime
					>> m_strGroupName >> m_strGroupDesc >> m_strGroupBulletin
					>> m_uLogoIdex >> m_strLogoUrl
					>> m_uCategory >> m_uSubCategory;
				m_bAllowAdhocChat = p.pop_uint8() != 0;
				m_bIsPrivate = p.pop_uint8() != 0;
				m_eAuthMod = (protocol::ginfo::EAuthMode)p.pop_uint8();
				m_eTopicMod = (protocol::gprops::ETopicMode)p.pop_uint8();
			}
		};

		//服务器端简单组属性结构
		struct CServerFolderSimpleProps : public sox::Marshallable
		{
			uint32_t     m_uFolderId;	    //组ID
			std::string  m_strFolderName;   //组名字
			protocol::ginfo::EAuthMode m_eAuthMod;//分组的验证信息

			CServerFolderSimpleProps()
			{
				m_uFolderId = SINVALID_ID;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uFolderId << m_strFolderName << (uint32_t)m_eAuthMod;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
                uint32_t uAuthMod;
                
				p >> m_uFolderId >> m_strFolderName >> uAuthMod;
                m_eAuthMod = (protocol::ginfo::EAuthMode)uAuthMod;
			}
		};

		struct CServerFolderSimpleProps2 : public sox::Marshallable//old
		{
			uint32_t     m_uFolderId;	 //组ID
			std::string  m_strFolderName; //组名字

			CServerFolderSimpleProps2()
			{
				m_uFolderId = SINVALID_ID;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uFolderId << m_strFolderName;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uFolderId >> m_strFolderName;
			}
		};

		//服务器端完整组属性结构
		struct CServerFolderProps : public sox::Marshallable
		{
			uint32_t     m_uFolderId;	//组ID
			uint32_t     m_createTime;  //组创建时间

			std::string  m_strFolderName; //组名字
			std::string  m_strFolderDesc; //组描述
			std::string  m_strFolderBulletin;	//组公告


			protocol::gprops::ETopicMode m_eTopicMod;	//组主题模式
			protocol::ginfo::EAuthMode    m_eAuthMod;	//组验证模式
			MapChannelInfo	m_uMapChanAuthInfo;	//该map存放着频道号和该频道对应的马甲掩码
												//只在验证模式为ENUM_BY_SESSION时有效
			CServerFolderProps()
				:m_uFolderId(SINVALID_ID), 
				 m_createTime(0), 
				 m_eTopicMod(protocol::gprops::ENUM_NO_AUTH), 
				 m_eAuthMod(protocol::ginfo::ENUM_NO_AUTH),
				 m_uMapChanAuthInfo()

			{

			}

			bool IsInvalid() const
			{
				return m_uFolderId == SINVALID_ID;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uFolderId << m_createTime
					<< m_strFolderName << m_strFolderDesc << m_strFolderBulletin;
				p.push_uint8((uint8_t)m_eTopicMod);
				p.push_uint8((uint8_t)m_eAuthMod);
				m_uMapChanAuthInfo.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uFolderId >> m_createTime
					>> m_strFolderName >> m_strFolderDesc >> m_strFolderBulletin;
				m_eTopicMod = (protocol::gprops::ETopicMode)p.pop_uint8();
				m_eAuthMod = (protocol::ginfo::EAuthMode)p.pop_uint8();
				m_uMapChanAuthInfo.unmarshal(p);
			}
		};


		//服务器端组属性结构2
		struct CServerFolderProps2 : public sox::Marshallable
		{
			uint32_t     m_uFolderId;
			uint32_t     m_createTime;

			std::string  m_strFolderName;
			std::string  m_strFolderDesc;
			std::string  m_strFolderBulletin;
			protocol::gprops::ETopicMode m_eTopicMod;

			CServerFolderProps2()
				:m_uFolderId(SINVALID_ID),
				m_createTime(0),
				m_eTopicMod(protocol::gprops::ENUM_NO_AUTH)
			{

			}

			bool IsInvalid() const
			{
				return m_uFolderId == SINVALID_ID;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uFolderId << m_createTime
					<< m_strFolderName << m_strFolderDesc << m_strFolderBulletin;
				p.push_uint8((uint8_t)m_eTopicMod);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uFolderId >> m_createTime
					>> m_strFolderName >> m_strFolderDesc >> m_strFolderBulletin;
				m_eTopicMod = (protocol::gprops::ETopicMode)p.pop_uint8();
			}
		};

		struct CMapPropsInfoBase : public sox::Properties
		{
			void SetName(const std::string& name)               { props[GIT_NAME] = name;			}
			void SetDesc(const std::string& desc)               { props[GIT_DESC] = desc;			}
			void SetBulletin(const std::string& bulletin)       { props[GIT_BULLETIN] = bulletin;	}
			void SetAuthMode(protocol::ginfo::EAuthMode mode)   { Uint2String((uint16_t)mode, props[GIT_AUTH]);				}
			void SetChannelInfo(const MapChannelInfo& chanInfo)	{ sox::PacketToString(chanInfo, props[GIT_CHAN_INFO]);		}
			void SetShortChannelInfo(const MapChannelInfo& chanInfo)	{ sox::PacketToString(chanInfo, props[GIT_SHORT_CHAN_INFO]);}
			void SetTopicMode(protocol::gprops::ETopicMode topicMode){Uint2String((uint16_t)topicMode, props[GIT_TOPIC]);	}

			std::string GetName()                         
			{ 
				//return props[GIT_NAME];
				sox::Properties::iterator it = props.find(GIT_NAME);
				if (it != props.end())
				{
					return it->second;
				}
				else return "";
			}

			std::string GetDesc()                         
			{ 
				//return props[GIT_DESC];		
				sox::Properties::iterator it = props.find(GIT_DESC);
				if (it != props.end())
				{
					return it->second;
				}
				else return "";
			}

			std::string GetBulletin()                     
			{ 
				//return props[GIT_BULLETIN];	
				sox::Properties::iterator it = props.find(GIT_BULLETIN);
				if (it != props.end())
				{
					return it->second;
				}	
				else return "";
			}

			protocol::ginfo::EAuthMode GetAuthMode()      
			{
				//return (protocol::ginfo::EAuthMode)String2Uint16_t(props[GIT_AUTH]);	
				sox::Properties::iterator it = props.find(GIT_AUTH);
				if (it != props.end()) 
					return (protocol::ginfo::EAuthMode)String2Uint16_t(it->second);
				else
					return protocol::ginfo::ENUM_UNKNOWN_MODE;
				
			}

			void GetChannelInfo(MapChannelInfo& chanInfo) 
			{ 
				//return (void)sox::StringToPacket(props[GIT_CHAN_INFO], chanInfo);		
				sox::Properties::iterator it = props.find(GIT_CHAN_INFO);
				if (it != props.end())
				{
					return (void)sox::StringToPacket(it->second, chanInfo);
				}
			}

			void GetShortChannelInfo(MapChannelInfo& chanInfo) 
			{ 
				//return (void)sox::StringToPacket(props[GIT_SHORT_CHAN_INFO], chanInfo);
				sox::Properties::iterator it = props.find(GIT_SHORT_CHAN_INFO);
				if (it != props.end())
				{
					return (void)sox::StringToPacket(it->second, chanInfo);
				}
			}

			protocol::gprops::ETopicMode GetTopicMode()   
			{ 
				//return (protocol::gprops::ETopicMode)String2Uint16_t(props[GIT_TOPIC]); 
				sox::Properties::iterator it = props.find(GIT_TOPIC);
				if (it != props.end()) 
					return (protocol::gprops::ETopicMode)String2Uint16_t(it->second);
				else
					return protocol::gprops::ENUM_UNKNOWN_MODE;
			}

			void ReplaceChIDs (const std::map<uint32_t, uint32_t>& mapChIDs, std::vector<uint32_t>& vecNonExist, bool isUpdate) 
			{
				MapChannelInfo chanInfo, chanInfoNew;
				GetChannelInfo(chanInfo);
				vecNonExist.clear();
				
				MapChannelInfo::ChIDRoleType& mapListOld = chanInfo.m_mapChannelList;
				MapChannelInfo::ChIDRoleType& mapListNew = chanInfoNew.m_mapChannelList;

				MapChannelInfo::ChIDRoleType::iterator first1 = mapListOld.begin();
				std::map<uint32_t, uint32_t>::const_iterator first2 = mapChIDs.begin();

				//log(Info, "[%s] key=%d, val=%d", __FUNCTION__, first1->first, first1->second);
				//ChIDRoleType的结构为ChannelID -> Role, mapChIDs为ChannelID -> NewChannelID
				//现在要把ChIDRoleType的ChannelID替换成mapChIDs中对应的NewChannelID
				while (first1!=mapListOld.end() && first2!=mapChIDs.end())
				{
					if (first1->first < first2->first ) ++first1;
					else if (first2->first < first1->first) ++first2;
					else 
					{ 
						uint32_t uNewChannelID = first2->second;

						if (0 == uNewChannelID)
						{
							vecNonExist.push_back(first2->first);
							if (isUpdate)
								mapListNew.insert(std::make_pair(uNewChannelID, (first1++)->second)); 
						}
						else
							mapListNew.insert(std::make_pair(uNewChannelID, (first1++)->second));
						++first2;
					}
				}

				SetChannelInfo(chanInfoNew);
			}

			virtual void marshal(sox::Pack &p) const
			{
				sox::Properties::marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::Properties::unmarshal(p);
			}
		
		};

		//群属性map映射表
		struct CMapGroupPropsInfo : public CMapPropsInfoBase
		{
			void SetCatgoty(uint16_t catgory)                   { Uint2String(catgory, props[GIT_CAT1]); }
			void SetSubCatgoty(uint16_t catgory)                { Uint2String(catgory, props[GIT_CAT2]); }
			void SetLogoIdex(uint32_t idex)                     { Uint2String(idex, props[GIT_LOGO_IDX]);}
			void SetLogoUrl(const std::string& url)             { props[GIT_LOGO_URL] = url; }
			void SetIsAlowAdhocChat(bool flag)                  { Uint2String((uint8_t)flag, props[GIT_ADHOC]); }
			void SetIsPrivate(bool flag)                        { Uint2String((uint8_t)flag, props[GIT_PRIV]);	}

			uint16_t GetCatgoty()                         
			{ 
				//return String2Uint16_t(props[GIT_CAT1]);		
				sox::Properties::iterator it = props.find(GIT_CAT1);
				if (it != props.end())
				{
					return String2Uint16_t(it->second);
				}
				else return 0;
			}

			uint16_t GetSubCatgoty()                      
			{ 
				//return String2Uint16_t(props[GIT_CAT2]);		
				sox::Properties::iterator it = props.find(GIT_CAT2);
				if (it != props.end())
				{
					return String2Uint16_t(it->second);
				}
				else return 0;
			}

			uint32_t GetLogoIdex()                        
			{ 
				//return String2Uint32_t(props[GIT_LOGO_IDX]);	
				sox::Properties::iterator it = props.find(GIT_LOGO_IDX);
				if (it != props.end())
				{
					return String2Uint32_t(it->second);
				}
				else return 0;
			}

			std::string GetLogoUrl()                      
			{ 
				//return props[GIT_LOGO_URL]; 
				sox::Properties::iterator it = props.find(GIT_LOGO_URL);
				if (it != props.end())
				{
					return it->second;
				}
				else return "";
			}

			bool GetIsAlowAdhocChat()                     
			{ 
				//return String2Uint8_t(props[GIT_ADHOC]) > 0;	
				sox::Properties::iterator it = props.find(GIT_ADHOC);
				if (it != props.end())
				{
					return String2Uint8_t(it->second) > 0;
				}
				else return true;
			}

			bool GetIsPrivate()                           
			{ 
				//return String2Uint8_t(props[GIT_PRIV]) > 0;	
				sox::Properties::iterator it = props.find(GIT_PRIV);
				if (it != props.end())
				{
					return String2Uint8_t(it->second) > 0;
				}
				else return false;
			}	
		};

		//组属性map映射表
		struct CMapFolderPropsInfo : public CMapPropsInfoBase
		{
		};

	}
}

#endif

