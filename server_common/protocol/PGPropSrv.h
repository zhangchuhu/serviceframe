#pragma once

#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"
#include "protocol/common/protocol/GPropsCommonType.h"
#include "protocol/common/protocol/PGInfo.h"
#include "common/protocol/PGInfoCommonTypes.h"
#include "server_common/protocol/PGTransferSrv.h"

#include <iostream>
#include <string>

namespace protocol
{
	namespace gprops
	{
		//新建群属性消息新协议
		struct PSS_NewGroupProps : public sox::Marshallable
		{
			enum {uri = (50 << 8) | protocol::GPROPERTY_SVID};

			uint32_t  m_uGroupId;//Internal GID
			uint32_t  m_uAliasId;//External GID
			CMapGroupPropsInfo m_mapProps;

			PSS_NewGroupProps()
				:m_uGroupId(0),
				 m_uAliasId(0),
				 m_mapProps()
			{

			}

			PSS_NewGroupProps(uint32_t uGid, uint32_t uAid)
				 :m_uGroupId(uGid),
		          m_uAliasId(uAid)
			{

			}

			PSS_NewGroupProps(protocol::ginfo::PCS_NewGroupRes& obj)
				:m_uGroupId(obj.m_uGrpID),
				 m_uAliasId(obj.m_uAliasID),
				 m_mapProps(obj.m_mapProps)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uAliasId;
				m_mapProps.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId >> m_uAliasId;
				m_mapProps.unmarshal(p);
			}

		};

		//新建群属性消息旧协议
		struct PSS_NewGroupProps2 : public sox::Marshallable
		{
			enum {uri = (20 << 8) | protocol::GPROPERTY_SVID};

			uint32_t     m_uGroupId; //Internal GID
			uint32_t     m_uAliasId;	//External GID
			std::string  m_strName;
			std::string  m_strDesc;
			uint16_t     m_uCategory;
			uint16_t     m_uSubCategory;
			protocol::ginfo::EAuthMode	m_eAuthMod;

			PSS_NewGroupProps2() : m_uGroupId(0), m_uAliasId(0), m_uCategory(0),
				m_uSubCategory(0), m_eAuthMod(protocol::ginfo::ENUM_UNKNOWN_MODE){}

			PSS_NewGroupProps2(uint32_t uGid, uint32_t aliasId, const std::string& name, const std::string& desc,
				uint16_t category, uint16_t subCat, protocol::ginfo::EAuthMode mode)
				: m_uGroupId(uGid), m_uAliasId(aliasId), m_strName(name), m_strDesc(desc),
				m_uCategory(category),m_uSubCategory(subCat), m_eAuthMod(mode)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uAliasId << m_strName << m_strDesc << m_uCategory
					<< m_uSubCategory;
				p.push_uint16(m_eAuthMod);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId >> m_uAliasId >> m_strName >> m_strDesc >> m_uCategory
					>> m_uSubCategory;
				m_eAuthMod = (protocol::ginfo::EAuthMode)p.pop_uint16();
			}

		};

		
		//新建组属性消息新协议
		struct PSS_NewFolderProps : public sox::Marshallable
		{
			enum {uri = (51 << 8) | protocol::GPROPERTY_SVID};

			uint32_t  m_uGroupId;
			uint32_t  m_uFolderId;
			CMapFolderPropsInfo m_mapProps;

			PSS_NewFolderProps()
				: m_uGroupId(0),
				  m_uFolderId(0),
				  m_mapProps()
			{

			}

			PSS_NewFolderProps(uint32_t uGid, uint32_t uFid)
				 :m_uGroupId(uGid),
				  m_uFolderId(uFid)
			{

			}
			PSS_NewFolderProps(protocol::ginfo::PCS_NewGroupFolderRes& obj)
				:m_uGroupId(obj.m_uGrpID),
				 m_uFolderId(obj.m_uFldID),
				 m_mapProps(obj.m_mapProps)
			{
			}
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uFolderId;
				m_mapProps.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId >> m_uFolderId;
				m_mapProps.unmarshal(p);
			}


		};

		//新建组属性消息旧协议
		struct PSS_NewFolderProps2 : public sox::Marshallable
		{
			enum {uri = (21 << 8 | protocol::GPROPERTY_SVID)};

			uint32_t     m_uGroupId;
			uint32_t     m_uFolderId;
			std::string  m_strFolderName;
			std::string  m_strFolderDesc;
			std::string  m_strFolderBulletin;

			PSS_NewFolderProps2():m_uGroupId(0), m_uFolderId(0){}

			PSS_NewFolderProps2(uint32_t uGid, uint32_t uFid, const std::string& name,
				const std::string& desc, const std::string& bulletin)
				: m_uGroupId(uGid), m_uFolderId(uFid), m_strFolderName(name), m_strFolderDesc(desc),
				m_strFolderBulletin(bulletin)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uFolderId << m_strFolderName << m_strFolderDesc << m_strFolderBulletin;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId >> m_uFolderId >> m_strFolderName >> m_strFolderDesc >> m_strFolderBulletin;
			}
		};

		//更新主题模式消息
		struct PSS_UpdateTopicMode : public sox::Marshallable
		{
			enum {uri = (22 << 8) | protocol::GPROPERTY_SVID};

			uint32_t	m_uGroupId;
			uint32_t	m_uFolderId;
			protocol::gprops::ETopicMode	m_eTopicMod;

			PSS_UpdateTopicMode() : m_uGroupId(0), m_uFolderId(0) {}
			PSS_UpdateTopicMode(uint32_t uGid, uint32_t uFid, protocol::gprops::ETopicMode mode)
				: m_uGroupId(uGid), m_uFolderId(uFid), m_eTopicMod(mode)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uFolderId;
				p.push_uint8(m_eTopicMod);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId >> m_uFolderId;
				m_eTopicMod = (protocol::gprops::ETopicMode)p.pop_uint8();
			}
		};

		struct PSS_BaseContext : virtual public sox::Marshallable
		{
			uint32_t m_uUID;
			uint32_t m_uSvrID;
			uint32_t m_uAppURI;			// protocol 原始uri
			std::string m_strAppCtx;	// protocol 原始包

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uSvrID << m_uAppURI << m_strAppCtx << m_uUID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uSvrID >> m_uAppURI >> m_strAppCtx >> m_uUID;
			}
		};

		struct PSS_GetGrpAuthMode : public PSS_BaseContext
		{
			enum {uri = (81 << 8 | protocol::GPROPERTY_SVID)};

			uint32_t m_uGrpID;

			PSS_GetGrpAuthMode ()
			{
				m_uGrpID = 0;
				m_uUID = 0;
				m_uSvrID = 0;
				m_uAppURI = 0;
			}

			PSS_GetGrpAuthMode (uint32_t uGrpID, uint32_t uUID, uint32_t uSvrID,
				uint32_t uAppURI, const sox::Marshallable &objMarshal)
			{
				m_uGrpID = uGrpID;
				m_uUID = uUID;
				m_uSvrID = uSvrID;
				m_uAppURI = uAppURI;
				protocol::gtransfer::Packet2String(objMarshal, m_strAppCtx);
			}

			virtual void marshal(sox::Pack &p) const
			{
				PSS_BaseContext::marshal(p);
				p << m_uGrpID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PSS_BaseContext::unmarshal(p);
				p >> m_uGrpID;
			}
		};

		struct PSS_GetGrpAuthModeRes : public PSS_GetGrpAuthMode
		{
			enum {uri = (82 << 8 | protocol::GPROPERTY_SVID)};

			protocol::ginfo::EAuthMode m_enAuthMode;
			MapChannelInfo m_mapChanInfo;

			PSS_GetGrpAuthModeRes()
				:m_enAuthMode(protocol::ginfo::ENUM_UNKNOWN_MODE){}

			PSS_GetGrpAuthModeRes(PSS_GetGrpAuthMode& objOri)
				:PSS_GetGrpAuthMode(objOri),
				 m_enAuthMode(protocol::ginfo::ENUM_UNKNOWN_MODE),
				 m_mapChanInfo()
			{

			}

			virtual void marshal(sox::Pack &p) const
			{
				PSS_GetGrpAuthMode::marshal(p);
				m_mapChanInfo.marshal(p);
				p.push_uint8(m_enAuthMode);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PSS_GetGrpAuthMode::unmarshal(p);
				m_mapChanInfo.unmarshal(p);
				m_enAuthMode = (protocol::ginfo::EAuthMode)p.pop_uint8();
			}
		};

		struct PSS_GetFldAuthMode : public PSS_GetGrpAuthMode
		{
			enum {uri = (83 << 8 | protocol::GPROPERTY_SVID)};

			uint32_t m_uFldID;

			PSS_GetFldAuthMode ()
				:PSS_GetGrpAuthMode()
			{
				m_uFldID = 0;
			}

			PSS_GetFldAuthMode (uint32_t uGrpID, uint32_t uFldID, uint32_t uUID, uint32_t uSvrID,
				uint32_t uAppURI, const sox::Marshallable &objMarshal) :
				PSS_GetGrpAuthMode(uGrpID, uUID, uSvrID, uAppURI, objMarshal)
			{
				m_uFldID = uFldID;
			}

			virtual void marshal(sox::Pack &p) const
			{
				PSS_GetGrpAuthMode::marshal(p);
				p << m_uFldID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PSS_GetGrpAuthMode::unmarshal(p);
				p >> m_uFldID;
			}
		};

		struct PSS_GetFldAuthModeRes : public PSS_GetFldAuthMode
		{
			enum {uri = (84 << 8 | protocol::GPROPERTY_SVID)};

			protocol::ginfo::EAuthMode m_enAuthMode;
			MapChannelInfo m_mapChanInfo;

			PSS_GetFldAuthModeRes()
				:m_enAuthMode(protocol::ginfo::ENUM_UNKNOWN_MODE){}

			PSS_GetFldAuthModeRes(PSS_GetFldAuthMode& objOri)
				:PSS_GetFldAuthMode(objOri),
				 m_enAuthMode(protocol::ginfo::ENUM_UNKNOWN_MODE),
				 m_mapChanInfo()
			{

			}

			virtual void marshal(sox::Pack &p) const
			{
				PSS_GetFldAuthMode::marshal(p);
				p.push_uint8(m_enAuthMode);
				m_mapChanInfo.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PSS_GetFldAuthMode::unmarshal(p);
				m_enAuthMode = (protocol::ginfo::EAuthMode)p.pop_uint8();
				m_mapChanInfo.unmarshal(p);
			}
		};
	}
}
