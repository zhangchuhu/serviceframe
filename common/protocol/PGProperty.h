#ifndef PGPROPERTY_H_INCLUDED
#define PGPROPERTY_H_INCLUDED

#include <vector>
#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/PGInfoCommonTypes.h"
#include "common/protocol/IGInfoDataTypes.h"
#include "GPropsCommonType.h"
#include "PGCheck.h"
#include "pimcommon.h"


namespace protocol
{
	namespace gprops
	{
		/****************************************************
        *  消息基类.
        ****************************************************/
		struct IChannelIDBase: virtual public sox::Marshallable
		{
			virtual void ReplaceChIDs (const std::map<uint32_t, uint32_t>& mapChIDs, std::vector<uint32_t>& vecNonExist, bool IsUpdate = false) = 0;
		};

		struct GPropsBase:public sox::Marshallable
		{
			GPropsBase(){}
			
			GPropsBase(uint32_t sender)
				:uSender(sender)
			{
			}
	
			uint32_t uSender;
			
			virtual void marshal(sox::Pack &p) const
			{
				p << uSender;
			}
			
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uSender;
			}
		};


		/****************************************************
        *  获取完整组属性请求.
        ****************************************************/
		struct PCS_GetFolderProps2 : public sox::Marshallable
		{
			enum {uri = (1 << 8 | protocol::GPROPERTY_SVID)};

			uint32_t              m_uGroupId;
			std::vector<uint32_t> m_vecFolderIds;

			PCS_GetFolderProps2(){}
			PCS_GetFolderProps2(uint32_t uGid, const std::vector<uint32_t>& vecFid)
				: m_uGroupId(uGid), m_vecFolderIds(vecFid)
			{

			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId;
				sox::marshal_container(p, m_vecFolderIds);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId;
				sox::unmarshal_container(p, back_inserter(m_vecFolderIds));
			}
		};

		struct PCS_GetFolderProps 
			: public protocol::gprops::IChannelIDBase
		{
			enum {uri = (31 << 8 | protocol::GPROPERTY_SVID)};

			uint32_t              m_uGroupId;
			std::vector<uint32_t> m_vecFolderIds;

			PCS_GetFolderProps(){}
			PCS_GetFolderProps(uint32_t uGid, const std::vector<uint32_t>& vecFid)
				: m_uGroupId(uGid), m_vecFolderIds(vecFid)
			{

			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId;
				sox::marshal_container(p, m_vecFolderIds);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId;
				sox::unmarshal_container(p, back_inserter(m_vecFolderIds));
			}

			virtual void ReplaceChIDs(const std::map<uint32_t, uint32_t>& mapChIDs, std::vector<uint32_t>& vecNonExist, bool IsUpdate = false){} 
		};

		/****************************************************
        *  获取简单组属性请求.
        ****************************************************/
		struct PCS_GetFolderSimpleProps : public sox::Marshallable
		{
			enum {uri = (141 << 8 | protocol::GPROPERTY_SVID)};

			uint32_t               m_uGroupId;
			std::vector<uint32_t>  m_vecFolderIds;

			PCS_GetFolderSimpleProps(){}
			PCS_GetFolderSimpleProps(uint32_t uGid, const std::vector<uint32_t>& vecFid)
				: m_uGroupId(uGid), m_vecFolderIds(vecFid)
			{

			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId;
				sox::marshal_container(p, m_vecFolderIds);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId;
				sox::unmarshal_container(p, back_inserter(m_vecFolderIds));
			}
		};

		struct PCS_GetFolderSimpleProps2 : public sox::Marshallable//old
		{
			enum {uri = (41 << 8 | protocol::GPROPERTY_SVID)};

			uint32_t              m_uGroupId;
			std::vector<uint32_t> m_vecFolderIds;

			PCS_GetFolderSimpleProps2(){}
			PCS_GetFolderSimpleProps2(uint32_t uGid, const std::vector<uint32_t>& vecFid)
				: m_uGroupId(uGid), m_vecFolderIds(vecFid)
			{

			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId;
				sox::marshal_container(p, m_vecFolderIds);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId;
				sox::unmarshal_container(p, back_inserter(m_vecFolderIds));
			}
		};

		/****************************************************
        *  获取完整组属性返回.
        ****************************************************/
		struct PCS_GetFolderPropsRes2 : public sox::Marshallable
		{
			enum {uri = (2 << 8 | protocol::GPROPERTY_SVID)};
			
			uint32_t   m_uGroupId;
			std::vector<CServerFolderProps2>  m_vecFolderProps;
			
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId;
				marshal_container(p, m_vecFolderProps);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId;
				unmarshal_containerEx(p, m_vecFolderProps);
			}
		};

		struct PCS_GetFolderPropsRes : public protocol::gprops::IChannelIDBase
		{
			enum {uri = (32 << 8 | protocol::GPROPERTY_SVID)};

			uint32_t   m_uGroupId;
			//uint32_t m_uResCode;
			std::vector<CServerFolderProps>  m_vecFolderProps;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId; // << m_uResCode; 
				marshal_container(p, m_vecFolderProps);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId; // >> m_uResCode;
				unmarshal_containerEx(p, m_vecFolderProps);
			}

			virtual void ReplaceChIDs(const std::map<uint32_t, uint32_t>& mapChIDs, std::vector<uint32_t>& vecNonExist, bool IsUpdate = false){} 	
		};

		/****************************************************
        *  获取简单组属性返回.
        ****************************************************/
		struct PCS_GetFolderPropsSimpleRes : public sox::Marshallable
		{
			enum {uri = (142 << 8 | protocol::GPROPERTY_SVID)};

			uint32_t   m_uGroupId;
			std::vector<CServerFolderSimpleProps>  m_vecFolderProps;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId;
				marshal_container(p, m_vecFolderProps);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId;
				unmarshal_containerEx(p, m_vecFolderProps);
			}
		};

		struct PCS_GetFolderPropsSimpleRes2 : public sox::Marshallable//old
		{
			enum {uri = (42 << 8 | protocol::GPROPERTY_SVID)};

			uint32_t   m_uGroupId;
			std::vector<CServerFolderSimpleProps2>  m_vecFolderProps;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId;
				marshal_container(p, m_vecFolderProps);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId;
				unmarshal_containerEx(p, m_vecFolderProps);
			}
		};

		/****************************************************
        *  获取完整群属性请求.
        ****************************************************/
		struct PCS_GetGroupProps2 : public sox::Marshallable
		{
			enum {uri = (3 << 8 | protocol::GPROPERTY_SVID)};

			PCS_GetGroupProps2(){}
			PCS_GetGroupProps2(const std::vector<uint32_t>& vecGid)
				: m_vecGroupIds(vecGid)
			{

			}

			std::vector<uint32_t> m_vecGroupIds;

			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_vecGroupIds);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				unmarshal_container(p, back_inserter(m_vecGroupIds));
			}
		};

		struct PCS_GetGroupProps : public protocol::gprops::IChannelIDBase
		{
			enum {uri = (33 << 8 | protocol::GPROPERTY_SVID)};

			PCS_GetGroupProps(){}
			PCS_GetGroupProps(const std::vector<uint32_t>& vecGid)
				: m_vecGroupIds(vecGid)
			{

			}

			std::vector<uint32_t> m_vecGroupIds;

			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_vecGroupIds);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				unmarshal_container(p, back_inserter(m_vecGroupIds));
			}

			virtual void ReplaceChIDs(const std::map<uint32_t, uint32_t>& mapChIDs, 
				std::vector<uint32_t>& vecNonExist, bool IsUpdate = false){} 
		};

		/****************************************************
        *  获取简单群属性请求.
        ****************************************************/
		struct PCS_GetGroupSimpleProps : public sox::Marshallable
		{
			enum {uri = (43 << 8 | protocol::GPROPERTY_SVID)};

			PCS_GetGroupSimpleProps(){}
			PCS_GetGroupSimpleProps(const std::vector<uint32_t>& vecGid)
				: m_vecGroupIds(vecGid)
			{

			}

			std::vector<uint32_t> m_vecGroupIds;

			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_vecGroupIds);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				unmarshal_container(p, back_inserter(m_vecGroupIds));
			}
		};


		/****************************************************
        *  获取完整群属性返回.
        ****************************************************/
		struct PCS_GetGroupPropsRes2 : public sox::Marshallable
		{
			enum {uri = (4 << 8 | protocol::GPROPERTY_SVID)};

			std::vector<CServerGroupProps2>  m_vecGroupProp;

			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_vecGroupProp);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				unmarshal_containerEx(p, m_vecGroupProp);
			}
		};

		struct PCS_GetGroupPropsRes : public protocol::gprops::IChannelIDBase
		{
			enum {uri = (34 << 8 | protocol::GPROPERTY_SVID)};

			std::vector<CServerGroupProps>  m_vecGroupProp;

			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_vecGroupProp);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				unmarshal_containerEx(p, m_vecGroupProp);
			}

			virtual void ReplaceChIDs(const std::map<uint32_t, uint32_t>& mapChIDs, std::vector<uint32_t>& vecNonExist, bool IsUpdate = false){} 
			
		};

		/****************************************************
        *  获取简单群属性返回.
        ****************************************************/
		struct PCS_GetGroupPropsSimpleRes : public sox::Marshallable
		{
			enum {uri = (44 << 8 | protocol::GPROPERTY_SVID)};

			std::vector<CServerGroupSimpleProps>  m_vecGroupProp;

			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_vecGroupProp);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				unmarshal_containerEx(p, m_vecGroupProp);
			}
		};

		/****************************************************
        *  新建群属性返回.
        ****************************************************/
		struct PCS_NewGroupPropsRes2 : public sox::Marshallable
		{
			enum {uri = (6 << 8 | protocol::GPROPERTY_SVID)};

			CServerGroupProps2 m_props;

			PCS_NewGroupPropsRes2(){}

			virtual void marshal(sox::Pack &p) const
			{
				m_props.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				m_props.unmarshal(p);
			}
		};

		struct PCS_NewGroupPropsRes : public sox::Marshallable
		{
			enum {uri = (36 << 8 | protocol::GPROPERTY_SVID)};

			CServerGroupProps m_props;

			PCS_NewGroupPropsRes(){}

			virtual void marshal(sox::Pack &p) const
			{
				m_props.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				m_props.unmarshal(p);
			}
		};


		/****************************************************
        *  新建组属性返回.
        ****************************************************/
		struct PCS_NewFolderPropsRes2 : public sox::Marshallable
		{
			enum {uri = (8 << 8 | protocol::GPROPERTY_SVID)};

			uint32_t           m_uGroupId;
			CServerFolderProps2 m_props;

			PCS_NewFolderPropsRes2(){}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId;
				m_props.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId;
				m_props.unmarshal(p);
			}
		};

		struct PCS_NewFolderPropsRes : public sox::Marshallable
		{
			enum {uri = (38 << 8 | protocol::GPROPERTY_SVID)};

			uint32_t           m_uGroupId;
			CServerFolderProps m_props;

			PCS_NewFolderPropsRes(){}
			
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId;
				m_props.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId;
				m_props.unmarshal(p);
			}
		};


		/****************************************************
        *  更新群属性请求.
        ****************************************************/
		struct PCS_UpdateGroupProps 
			:public protocol::gcheck::PCS_GroupCommon,
			 public protocol::gprops::IChannelIDBase
		{
			enum {uri = (9 << 8 | protocol::GPROPERTY_SVID)};

			uint32_t  m_uGroupId;
			CMapGroupPropsInfo m_mapProps;
			GGroupPropsMask mask;

			PCS_UpdateGroupProps(){}
			PCS_UpdateGroupProps(uint32_t uGid, const uint32_t uSender, const GGroupPropsMask umask)
				:PCS_GroupCommon(uSender, uGid), 
				 m_uGroupId(uGid), 
				 mask(umask)
			{

			}

			virtual void marshal(sox::Pack &p) const
			{
				protocol::gcheck::PCS_GroupCommon::marshal(p);
				p << m_uGroupId;
				m_mapProps.marshal(p);
				p << mask.m_uMask;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				protocol::gcheck::PCS_GroupCommon::unmarshal(p);
				p >> m_uGroupId;
				m_mapProps.unmarshal(p);
				p >> mask.m_uMask;
			}

			virtual void ReplaceChIDs(const std::map<uint32_t, uint32_t>& mapChIDs, std::vector<uint32_t>& vecNonExist, bool IsUpdate = false) 
			{
				m_mapProps.ReplaceChIDs(mapChIDs, vecNonExist, IsUpdate);
			}
		};

		/****************************************************
        *  更新群属性返回.
        ****************************************************/
		struct PCS_UpdateGroupPropsRes : public protocol::im::PCS_CommonHead 
		{
			enum {uri = (10 << 8 | protocol::GPROPERTY_SVID)};
			
			uint32_t  m_uGroupId;
			CMapGroupPropsInfo m_mapProps;

			PCS_UpdateGroupPropsRes(){}
			PCS_UpdateGroupPropsRes(const PCS_UpdateGroupProps& obj)
			{
				m_uGroupId = obj.m_uGroupId;
				m_mapProps = obj.m_mapProps;
			}

			virtual void marshal(sox::Pack &p) const
			{
				protocol::im::PCS_CommonHead::marshal(p);
				p << m_uGroupId;
				m_mapProps.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				protocol::im::PCS_CommonHead::unmarshal(p);
				p >> m_uGroupId;
				m_mapProps.unmarshal(p);
			}
		};

		/****************************************************
        *  更新组属性请求.
        ****************************************************/
		struct PCS_UpdateFolderProps 
			:public protocol::gcheck::PCS_GroupCommon,
			 public protocol::gprops::IChannelIDBase
		{
			enum {uri = (11 << 8 | protocol::GPROPERTY_SVID)};

			uint32_t   m_uGroupId;
			uint32_t   m_uFolderId;
			CMapFolderPropsInfo m_mapProps;
			GFolderPropsMask mask;

			PCS_UpdateFolderProps() {}
			PCS_UpdateFolderProps(uint32_t uGid, const uint32_t uFid, const uint32_t uSender, const GFolderPropsMask umask)
				: PCS_GroupCommon(uSender, uGid, uFid), 
				  m_uGroupId(uGid), 
				  m_uFolderId(uFid),
				  mask(umask)
			{

			}

			virtual void marshal(sox::Pack &p) const
			{
				protocol::gcheck::PCS_GroupCommon::marshal(p);
				p << m_uGroupId << m_uFolderId;
				m_mapProps.marshal(p);
				p << mask.m_uMask;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				protocol::gcheck::PCS_GroupCommon::unmarshal(p);
				p >> m_uGroupId >> m_uFolderId;
				m_mapProps.unmarshal(p);
				p >> mask.m_uMask;
			}

			virtual void ReplaceChIDs(const std::map<uint32_t, uint32_t>& mapChIDs, std::vector<uint32_t>& vecNonExist, bool IsUpdate = false) 
			{
				m_mapProps.ReplaceChIDs(mapChIDs, vecNonExist, IsUpdate);
			}
		};

		/****************************************************
        *  更新组属性返回.
        ****************************************************/
		struct PCS_UpdateFolderPropsRes : public protocol::im::PCS_CommonHead 
		{
			enum {uri = (12 << 8 | protocol::GPROPERTY_SVID)};

			uint32_t   m_uGroupId;
			uint32_t   m_uFolderId;
			CMapFolderPropsInfo m_mapProps;

			PCS_UpdateFolderPropsRes() {}
			PCS_UpdateFolderPropsRes(const PCS_UpdateFolderProps& obj)
			{
				m_uGroupId = obj.m_uGroupId;
				m_uFolderId = obj.m_uFolderId;
				m_mapProps = obj.m_mapProps;
			}

			virtual void marshal(sox::Pack &p) const
			{
				protocol::im::PCS_CommonHead::marshal(p);
				p << m_uGroupId << m_uFolderId;
				m_mapProps.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				protocol::im::PCS_CommonHead::unmarshal(p);
				p >> m_uGroupId >> m_uFolderId;
				m_mapProps.unmarshal(p);
			}
		};


		/****************************************************
        *  更新群属性广播消息.
        ****************************************************/
		struct PCS_NotifyGroupPropsChange : public GPropsBase
		{
			enum {uri = (15 << 8 | protocol::GPROPERTY_SVID)};

			uint32_t           m_uGroupId;
			CMapGroupPropsInfo m_mapProps;

			PCS_NotifyGroupPropsChange(){}
			PCS_NotifyGroupPropsChange(uint32_t uSender, uint32_t uGroupId, const CMapGroupPropsInfo& obj)
				: GPropsBase(uSender), 
				  m_uGroupId (uGroupId)
			{
				m_mapProps = obj;
			}

			virtual void marshal(sox::Pack &p) const
			{
				GPropsBase::marshal(p);
				p << m_uGroupId;
				m_mapProps.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				GPropsBase::unmarshal(p);
				p >> m_uGroupId;
				m_mapProps.unmarshal(p);
			}
		};

		/****************************************************
        *  更新组属性广播消息.
        ****************************************************/
		struct PCS_NotifyFolderPropsChange : public GPropsBase
		{
			enum {uri = (16 << 8 | protocol::GPROPERTY_SVID)};

			uint32_t      m_uGroupId;
			uint32_t      m_uFolderId;
			CMapFolderPropsInfo  m_mapProps;

			PCS_NotifyFolderPropsChange(){}
			PCS_NotifyFolderPropsChange(uint32_t uSender, uint32_t uGroupId, uint32_t uFolderId, const CMapFolderPropsInfo& obj)
				: GPropsBase(uSender),
				  m_uGroupId(uGroupId), 
				  m_uFolderId(uFolderId)
			{
				m_mapProps = obj;
			}


			virtual void marshal(sox::Pack &p) const
			{
				GPropsBase::marshal(p);
				p << m_uGroupId << m_uFolderId;
				m_mapProps.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				GPropsBase::unmarshal(p);
				p >> m_uGroupId >> m_uFolderId;
				m_mapProps.unmarshal(p);
			}

		};

		/****************************************************
        *  新建组属性广播消息.
        ****************************************************/
		struct PCS_NotifyNewFolder2 :  public sox::Marshallable
		{
			enum {uri = (17 << 8 | protocol::GPROPERTY_SVID)};

			uint32_t  m_uGroupId;
			CServerFolderProps2  m_props;

			PCS_NotifyNewFolder2() {}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId;
				m_props.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId;
				m_props.unmarshal(p);
			}
		};

		struct PCS_NotifyNewFolder :  public sox::Marshallable
		{
			enum {uri = (47 << 8 | protocol::GPROPERTY_SVID)};

			uint32_t  m_uGroupId;
			CServerFolderProps  m_props;

			PCS_NotifyNewFolder() {}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId;
				m_props.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId;
				m_props.unmarshal(p);
			}
		};
	}
}

#endif

