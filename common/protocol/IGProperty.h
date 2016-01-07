#ifndef PROTOCOL_IGPROPS_WATCH_H
#define PROTOCOL_IGPROPS_WATCH_H

#include <vector>
#include <map>
#include <string>
#include "common/packet.h"
#include "common/unistr.h"
#include "common/protocol/PGPropertyType.h"
#include "common/protocol/PGInfoCommonTypes.h"

namespace protocol
{
	namespace gprops
	{
		const static uint32_t INVALID_ID = 0;

		//群的频道模式验证信息
		struct  MapChannelInfo : public sox::Marshallable
		{
			//<频道号，马甲掩码>
			typedef std::map<uint32_t, uint16_t> ChIDRoleType;

			ChIDRoleType m_mapChannelList;

			MapChannelInfo()
			{
				m_mapChannelList.clear();
			}

			MapChannelInfo& operator = (const MapChannelInfo& obj)
			{
				if (this == &obj)
				{
					return *this;
				}

				m_mapChannelList = obj.m_mapChannelList;
				return *this;
			}

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_mapChannelList);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::inserter(m_mapChannelList,
					m_mapChannelList.begin()));
			}
		};

		//群简单属性
		struct CClientGroupSimpleProps
		{
			uint32_t     m_uGroupId;
			uint32_t     m_uLogoIdex;
			UniString    m_strGroupName;
//			bool		 bIsSimple;

			CClientGroupSimpleProps()
			{
				m_uGroupId  = INVALID_ID;
				m_uLogoIdex = (uint32_t)ENUM_DEFULT;
//				bIsSimple   = true;
			}
		};

		//群组属性公用信息
		struct CClientBaseInfo
		{
			protocol::gprops::ETopicMode  m_eTopicMod;  //default: all allow
			protocol::ginfo::EAuthMode    m_eAuthMod;	//default: all allow

			uint32_t		m_createTime;
			MapChannelInfo	m_uChanMap;

			CClientBaseInfo()
				:m_uChanMap()
			{
				m_eTopicMod = protocol::gprops::ENUM_NO_AUTH;
				m_eAuthMod  = protocol::ginfo::ENUM_NO_AUTH;
				m_createTime = 0;
			}

			CClientBaseInfo& operator = (const CClientBaseInfo& obj)
			{
				if (this == &obj)
				{
					return *this;
				}

				m_eTopicMod = obj.m_eTopicMod;  //default: all allow
				m_eAuthMod = obj.m_eAuthMod;	//default: all allow
				m_createTime = obj.m_createTime;
				m_uChanMap = obj.m_uChanMap;

				return *this;
			}

			CClientBaseInfo(const CClientBaseInfo& msg)
			{
				m_eTopicMod  = msg.m_eTopicMod;
				m_eAuthMod	 = msg.m_eAuthMod;
				m_createTime = msg.m_createTime;
				m_uChanMap   = msg.m_uChanMap;
			}
		};


		//群属性客户端本地数据
		struct CClientGroupProps :
			public CClientGroupSimpleProps,
			public CClientBaseInfo
		{
			uint32_t     m_uAliasId;

			bool		 m_bAllowAdhocChat;  // true:  allow
			bool		 m_bIsPrivate;       // true:  not allow

			uint16_t     m_uCategory;
			uint16_t     m_uSubCategory;

			UniString    m_strGroupDesc;
			UniString    m_strGroupBulletin;
			UniString    m_strLogoUrl;

			CClientGroupProps():
				CClientBaseInfo(),
				m_uAliasId(INVALID_ID),
				m_bAllowAdhocChat(true),
				m_bIsPrivate(false),
				m_uCategory(0),
				m_uSubCategory(0)
			{
			}

			CClientGroupProps& operator = (const CClientGroupProps& obj)
			{
				if (this == &obj)
				{
					return *this;
				}

				m_eTopicMod = obj.m_eTopicMod;  //default: all allow
				m_eAuthMod = obj.m_eAuthMod;	//default: all allow
				m_createTime = obj.m_createTime;
				m_uChanMap = obj.m_uChanMap;

				m_uGroupId = obj.m_uGroupId;
				m_uAliasId = obj.m_uAliasId;
				m_uLogoIdex = obj.m_uLogoIdex;
				m_bAllowAdhocChat = obj.m_bAllowAdhocChat;  // true:  allow
				m_bIsPrivate = obj.m_bIsPrivate;			// true:  not allow
				m_uCategory = obj.m_uCategory;
				m_uSubCategory = obj.m_uSubCategory;
				m_strGroupName = obj.m_strGroupName;
				m_strGroupDesc = obj.m_strGroupDesc;
				m_strGroupBulletin = obj.m_strGroupBulletin;
				m_strLogoUrl = obj.m_strLogoUrl;

				return *this;
			}

			bool IsInvalid() const
			{
				return (m_uGroupId == INVALID_ID);
			}
		};


		//组简单客户端本地数据
		struct CClientFolderSimpleProps
		{
			uint32_t     m_uFolderId;
			UniString    m_strFolderName;
            protocol::ginfo::EAuthMode m_eFidAuthMod;//This Auth is the same with the one in basic.	
//			bool		 bIsSimple;

			CClientFolderSimpleProps()
			{
				m_uFolderId    = INVALID_ID;
                m_eFidAuthMod  = protocol::ginfo::ENUM_NO_AUTH;
//				bIsSimple = true;
			}
		};

		//组属性客户端本地数据
		struct CClientFolderProps :
			public CClientBaseInfo,
			public CClientFolderSimpleProps
		{
			UniString    m_strFolderDesc;
			UniString    m_strFolderBulletin;

			CClientFolderProps():
				CClientBaseInfo()
			{
			}

			bool IsInvalid() const
			{
				return m_uFolderId == INVALID_ID;
			}
		};

		// group props map
		typedef std::map<uint32_t, CClientGroupProps *> MapGroupProps_T;

		typedef std::map<uint32_t, CClientGroupSimpleProps *> MapGroupPropsSimple_T;

		// folder group map
		typedef std::map<uint32_t, CClientFolderProps *> MapFolderProps_T;

		typedef std::map<uint32_t, CClientFolderSimpleProps *> MapFolderPropsSimple_T;

		//////////////////////////////////////////////////////////////////////////

		// 客户端回调接口
		class IGPropertyWather
		{
		public:

			// 群属性更新返回
			//uRes可以为：
			//1.RES_SUCCESS：更新成功
			//2.RES_EDBERROR：数据库操作失败
			//3.RES_EACCESS：发送请求者身份验证失败
			//4.RES_GINFO_INVALID_CHANNEL：更新频道号时，指定频道ID不存在
			virtual void onUpdateGroupPropsRes(const uint32_t& uRes, const uint32_t& uGroupId) = 0;

			// 组属性更新返回
			//uRes可以为：
			//1.RES_SUCCESS：更新成功
			//2.RES_EDBERROR：数据库操作失败
			//3.RES_EACCESS：发送请求者身份验证失败
			//4.RES_GINFO_INVALID_CHANNEL：更新频道号时，指定频道ID不存在
			virtual void onUpdateFolderPropsRes(const uint32_t& uRes, const uint32_t& uGroupId, const uint32_t& uFolderId) = 0;

			// 群完整属性改变（其他用户update，UI自己请求群属性返回）
			// 注意： 并不是返回所有群
			virtual void OnGroupPropsChange(const MapGroupProps_T& mapGroupProps) = 0;

			// 群简单属性改变（其他用户update，UI自己请求群属性返回）
			// 注意： 并不是返回所有群
			virtual void OnGroupPropsSimpleChange(const MapGroupPropsSimple_T& mapGroupProps) = 0;

			// 组完整属性改变（其他用户update，UI自己请求群属性返回）
			// 注意： 并不是返回所有组
			virtual void OnFolderPropsChange(const uint32_t& uGroupId, const MapFolderProps_T& mapFolderProps) = 0;

			// 组简单属性改变（其他用户update，UI自己请求群属性返回）
			// 注意： 并不是返回所有组
			virtual void OnFolderPropsSimpleChange(const uint32_t& uGroupId, const MapFolderPropsSimple_T& mapFolderProps) = 0;

			// 新建组返回
			virtual void OnNewFolderPropsRes(const uint32_t& uRes, const uint32_t& uGroupId, const CClientFolderProps& folderProps) = 0;
			// 下面消息，由IGroupList.h 响应UI群的创建，此处UI不关心
			// 新建群返回
			virtual void OnNewGroupPropsRes(const uint32_t& uRes, const uint32_t& uGroupId) = 0;
		};

		class IOpGProperty
		{
		public:
			virtual void watch(IGPropertyWather *w) = 0;

			virtual void revoke(IGPropertyWather *w) = 0;
		};

		//客户端调用接口
		class IGProperty :
			public virtual IOpGProperty
		{
		public:
			virtual ~IGProperty() {}

			// 获取群完整属性
			virtual void GetGroupProps(const std::vector<uint32_t>& vecGroupId) = 0;

			// 获取群简单属性
			virtual void GetGroupSimpleProps(const std::vector<uint32_t>& vecGroupId) = 0;

			// 获取组完整属性
			virtual void GetFolderProps(uint32_t uGroupId, const std::vector<uint32_t>& vecFolderId) = 0;

			// 获取组简单属性
			virtual void GetFolderSimpleProps(uint32_t uGroupId, const std::vector<uint32_t>& vecFolderId) = 0;

			// 更新群属性
			// mask 说明：属性更改，将相应的位置1
			virtual void UpdateGroupProps(const CClientGroupProps& groupProps, const GGroupPropsMask& mask) = 0;

			// 更新组属性
			// mask 说明：属性更改，将相应的位置1
			virtual void UpdateFolderProps(uint32_t uGroupId, const CClientFolderProps& folderProps, const GFolderPropsMask& mask) = 0;
		};
	}
}

#endif

