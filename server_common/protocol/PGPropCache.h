#pragma once 

#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"
#include "protocol/common/protocol/GPropsCommonType.h"
#include "protocol/common/protocol/PGProperty.h"
#include "server_common/protocol/PGPropSrv.h" // for PSS_NewGroupProps and PSS_NewFolderProps

#include <iostream>
#include <string>

using namespace protocol::gprops;


namespace server
{
	namespace gpropcache
	{
		/****************************************************
        *  UI向GpropCache获取缓存中的群属性.
        ****************************************************/
		struct PCS_SyncGProp2 : public sox::Marshallable
		{
			enum {uri = (5 << 8 | protocol::GPROPCACHE_SVID)};

			PCS_SyncGProp2(){}
			PCS_SyncGProp2(const std::vector<uint32_t>& vecGid)
				: m_vecGroupId(vecGid){}

				std::vector<uint32_t> m_vecGroupId;

				virtual void marshal(sox::Pack &p) const
				{
					marshal_container(p, m_vecGroupId);
				}

				virtual void unmarshal(const sox::Unpack &p)
				{
					unmarshal_container(p, back_inserter(m_vecGroupId));
				}
		};

		struct PCS_SyncGProp : public sox::Marshallable
		{
			enum {uri = (35 << 8 | protocol::GPROPCACHE_SVID)};

			PCS_SyncGProp(){}
			PCS_SyncGProp(const std::vector<uint32_t>& vecGid)
				: m_vecGroupId(vecGid){}

			std::vector<uint32_t> m_vecGroupId;

			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_vecGroupId);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				unmarshal_container(p, back_inserter(m_vecGroupId));
			}
		};

		/****************************************************
        * GpropCache缓存中没有群属性，向GProps请求.
        ****************************************************/
		struct PSS_GFillGroupProp2 : public sox::Marshallable
		{
			enum {uri = (2 << 8) | protocol::GPROPCACHE_SVID};

			std::vector<uint32_t> m_vecGroupId;

			PSS_GFillGroupProp2() {}
			PSS_GFillGroupProp2(std::vector<uint32_t>& vecGid)
				: m_vecGroupId(vecGid){}

			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_vecGroupId);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				unmarshal_container(p, back_inserter(m_vecGroupId));
			}

		};

		struct PSS_GFillGroupProp : public sox::Marshallable
		{
			enum {uri = (32 << 8) | protocol::GPROPCACHE_SVID};

			std::vector<uint32_t> m_vecGroupId;

			PSS_GFillGroupProp() {}
			PSS_GFillGroupProp(std::vector<uint32_t>& vecGid)
				: m_vecGroupId(vecGid){}

			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_vecGroupId);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				unmarshal_container(p, back_inserter(m_vecGroupId));
			}

		};

		struct PSS_GFillGroupSimpleProp : public sox::Marshallable
		{
			enum {uri = (45 << 8) | protocol::GPROPCACHE_SVID};

			std::vector<uint32_t> m_vecGroupId;

			PSS_GFillGroupSimpleProp() {}
			PSS_GFillGroupSimpleProp(std::vector<uint32_t>& vecGid)
				: m_vecGroupId(vecGid){}

			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_vecGroupId);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				unmarshal_container(p, back_inserter(m_vecGroupId));
			}

		};

		/****************************************************
        * GProp向GpropCache返回从DB中获取到的群属性.
        ****************************************************/
		struct PSS_GFillGroupPropRes2 : public sox::Marshallable
		{
			enum {uri = (3 << 8) | protocol::GPROPCACHE_SVID};

			std::vector<CServerGroupProps2> m_vecGroupProp; //回应的群属性列表
			std::vector<uint32_t> m_vecGroupId; //请求的群Id

			PSS_GFillGroupPropRes2() {}
			PSS_GFillGroupPropRes2(std::vector<uint32_t>& vecGid)
				:m_vecGroupId(vecGid){}
			void SetProp(std::vector<CServerGroupProps2>& vecProps)
			{
				m_vecGroupProp.assign(vecProps.begin(), vecProps.end());
			}

			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_vecGroupId);
				marshal_container(p, m_vecGroupProp);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				unmarshal_container(p, back_inserter(m_vecGroupId));
				unmarshal_container(p, back_inserter(m_vecGroupProp));
			}

		};

		struct PSS_GFillGroupPropRes : public sox::Marshallable
		{
			enum {uri = (33 << 8) | protocol::GPROPCACHE_SVID};

			std::vector<CServerGroupProps> m_vecGroupProp; //回应的群属性列表
			std::vector<uint32_t> m_vecGroupId; //请求的群Id

			PSS_GFillGroupPropRes() {}
			PSS_GFillGroupPropRes(std::vector<uint32_t>& vecGid)
				:m_vecGroupId(vecGid){}

			void SetProp(std::vector<CServerGroupProps>& vecProps)
			{
				m_vecGroupProp.assign(vecProps.begin(), vecProps.end());
			}

			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_vecGroupId);
				marshal_container(p, m_vecGroupProp);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				unmarshal_container(p, back_inserter(m_vecGroupId));
				unmarshal_container(p, back_inserter(m_vecGroupProp));
			}

		};

		struct PSS_GFillGroupSimplePropRes : public sox::Marshallable
		{
			enum {uri = (46 << 8) | protocol::GPROPCACHE_SVID};

			std::vector<CServerGroupProps> m_vecGroupProp; //回应的群属性列表
			std::vector<uint32_t> m_vecGroupId; //请求的群Id

			PSS_GFillGroupSimplePropRes() {}
			PSS_GFillGroupSimplePropRes(std::vector<uint32_t>& vecGid)
				:m_vecGroupId(vecGid){}

			void SetProp(std::vector<CServerGroupProps>& vecProps)
			{
				m_vecGroupProp.assign(vecProps.begin(), vecProps.end());
			}

			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_vecGroupId);
				marshal_container(p, m_vecGroupProp);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				unmarshal_container(p, back_inserter(m_vecGroupId));
				unmarshal_container(p, back_inserter(m_vecGroupProp));
			}

		};

		/****************************************************
        *  GProp向GpropCache更新群属性(添加/更新).
        ****************************************************/
		struct PSS_GGroupPropUpdate2 : public sox::Marshallable
		{
			enum {uri = (1 << 8) | protocol::GPROPCACHE_SVID};

			CServerGroupProps2 m_props;

			PSS_GGroupPropUpdate2() {}
			PSS_GGroupPropUpdate2(const PSS_NewGroupProps2& obj)
			{
				m_props.m_strGroupName = obj.m_strName;
				m_props.m_strGroupDesc = obj.m_strDesc;
				m_props.m_eAuthMod     = obj.m_eAuthMod;
				m_props.m_uCategory    = obj.m_uCategory;
				m_props.m_uSubCategory = obj.m_uSubCategory;
				m_props.m_uGroupId     = obj.m_uGroupId;
				m_props.m_uAliasId	   = obj.m_uAliasId;
			}
			
			PSS_GGroupPropUpdate2(const CServerGroupProps2& obj)
			{
				m_props = obj;
			}

			virtual void marshal(sox::Pack &p) const
			{
				m_props.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				m_props.unmarshal(p);
			}
		};

		struct PSS_GGroupPropUpdate : public sox::Marshallable
		{
			enum {uri = (31 << 8) | protocol::GPROPCACHE_SVID};

			CServerGroupProps m_props;

			PSS_GGroupPropUpdate() {}
			PSS_GGroupPropUpdate(PSS_NewGroupProps& obj)
			{
				m_props.m_strGroupName = obj.m_mapProps.GetName();
				m_props.m_strGroupDesc = obj.m_mapProps.GetDesc();
				m_props.m_eAuthMod     = obj.m_mapProps.GetAuthMode();
				m_props.m_uCategory    = obj.m_mapProps.GetCatgoty();
				m_props.m_uSubCategory = obj.m_mapProps.GetSubCatgoty();
				m_props.m_uGroupId     = obj.m_uGroupId;
				m_props.m_uAliasId	   = obj.m_uAliasId;
				obj.m_mapProps.GetChannelInfo(m_props.m_uMapChanAuthInfo);
			}

			PSS_GGroupPropUpdate(const CServerGroupProps& obj)
			{
				m_props = obj;
			}

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
        *  GProp通知GpropCache删除缓存中的群属性.
        ****************************************************/
		struct PSS_GGroupPropDel : public sox::Marshallable
		{
			enum {uri = (4 << 8) | protocol::GPROPCACHE_SVID};

			uint32_t m_uGroupId; //请求的群Id
			std::set <uint32_t> m_uSetFid;

			PSS_GGroupPropDel() {}
			PSS_GGroupPropDel(uint32_t Gid, std::set <uint32_t>& setFid)
				:m_uGroupId(Gid), m_uSetFid(setFid){}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId;
				marshal_container(p, m_uSetFid);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGroupId;
				unmarshal_container(p, inserter(m_uSetFid, m_uSetFid.begin()));
			}

		};


		/****************************************************
        *  UI向GpropCache获取缓存中的组属性.
        ****************************************************/
		struct PCS_SyncFProp2 : public sox::Marshallable
		{
			enum {uri = (7 << 8 | protocol::GPROPCACHE_SVID)};

			PCS_SyncFProp2(){}
			PCS_SyncFProp2(uint32_t uGid, const std::vector<uint32_t>& vecFid)
				: m_uGroupId(uGid), m_vecFolderId(vecFid){}

			uint32_t m_uGroupId;
			std::vector<uint32_t> m_vecFolderId;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId;
				marshal_container(p, m_vecFolderId);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId;
				unmarshal_container(p, back_inserter(m_vecFolderId));
			}
		};

		struct PCS_SyncFProp : public sox::Marshallable
		{
			enum {uri = (37 << 8 | protocol::GPROPCACHE_SVID)};

			PCS_SyncFProp(){}
			PCS_SyncFProp(uint32_t uGid, const std::vector<uint32_t>& vecFid)
				: m_uGroupId(uGid), m_vecFolderId(vecFid){}

				uint32_t m_uGroupId;
				std::vector<uint32_t> m_vecFolderId;

				virtual void marshal(sox::Pack &p) const
				{
					p << m_uGroupId;
					marshal_container(p, m_vecFolderId);
				}

				virtual void unmarshal(const sox::Unpack &p)
				{
					p >> m_uGroupId;
					unmarshal_container(p, back_inserter(m_vecFolderId));
				}
		};

		/****************************************************
        * GpropCache缓存中没有组属性，向GProps请求.
        ****************************************************/
		struct PSS_GFillFolderProp2 : public sox::Marshallable
		{
			enum {uri = (9 << 8) | protocol::GPROPCACHE_SVID};

			uint32_t m_uGroupId; 
			std::vector<uint32_t> m_vecFolderId;

			PSS_GFillFolderProp2() {}
			PSS_GFillFolderProp2(uint32_t Gid, std::vector<uint32_t>& vecFid)
				: m_uGroupId(Gid),m_vecFolderId(vecFid){}

				virtual void marshal(sox::Pack &p) const
				{
					p << m_uGroupId;
					marshal_container(p, m_vecFolderId);
				}

				virtual void unmarshal(const sox::Unpack &p) 
				{
					p >> m_uGroupId;
					unmarshal_container(p, back_inserter(m_vecFolderId));
				}

		};

		struct PSS_GFillFolderProp : public sox::Marshallable
		{
			enum {uri = (39 << 8) | protocol::GPROPCACHE_SVID};

			uint32_t m_uGroupId; 
			std::vector<uint32_t> m_vecFolderId;

			PSS_GFillFolderProp() {}
			PSS_GFillFolderProp(uint32_t Gid, std::vector<uint32_t>& vecFid)
				: m_uGroupId(Gid),m_vecFolderId(vecFid){}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId;
				marshal_container(p, m_vecFolderId);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGroupId;
				unmarshal_container(p, back_inserter(m_vecFolderId));
			}

		};

		struct PSS_GFillFolderSimpleProp : public sox::Marshallable
		{
			enum {uri = (150 << 8) | protocol::GPROPCACHE_SVID};

			uint32_t m_uGroupId; 
			std::vector<uint32_t> m_vecFolderId;

			PSS_GFillFolderSimpleProp() {}
			PSS_GFillFolderSimpleProp(uint32_t Gid, std::vector<uint32_t>& vecFid)
				: m_uGroupId(Gid),m_vecFolderId(vecFid){}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId;
				marshal_container(p, m_vecFolderId);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGroupId;
				unmarshal_container(p, back_inserter(m_vecFolderId));
			}

		};

		struct PSS_GFillFolderSimpleProp2 : public sox::Marshallable//old
		{
			enum {uri = (50 << 8) | protocol::GPROPCACHE_SVID};

			uint32_t m_uGroupId; 
			std::vector<uint32_t> m_vecFolderId;

			PSS_GFillFolderSimpleProp2() {}
			PSS_GFillFolderSimpleProp2(uint32_t Gid, std::vector<uint32_t>& vecFid)
				: m_uGroupId(Gid),m_vecFolderId(vecFid){}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId;
				marshal_container(p, m_vecFolderId);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGroupId;
				unmarshal_container(p, back_inserter(m_vecFolderId));
			}

		};

		/****************************************************
        * GProp向GpropCache返回从DB中获取到的组属性.
        ****************************************************/
		struct PSS_GFillFolderPropRes2 : public sox::Marshallable
		{
			enum {uri = (10 << 8) | protocol::GPROPCACHE_SVID};

			uint32_t m_uGroupId;
			std::vector<CServerFolderProps2> m_vecFolderProp; //回应的组属性列表
			std::vector<uint32_t> m_vecFolderId; //请求的组Id

			PSS_GFillFolderPropRes2() {}
			PSS_GFillFolderPropRes2(uint32_t Gid, std::vector<uint32_t>& vecFid)
				:m_uGroupId(Gid),m_vecFolderId(vecFid){}
				void SetProp(std::vector<CServerFolderProps2>& vecProps)
				{
					m_vecFolderProp.assign(vecProps.begin(), vecProps.end());
				}

				virtual void marshal(sox::Pack &p) const
				{
					p << m_uGroupId;
					marshal_container(p, m_vecFolderId);
					marshal_container(p, m_vecFolderProp);
				}

				virtual void unmarshal(const sox::Unpack &p) 
				{
					p >> m_uGroupId;
					unmarshal_container(p, back_inserter(m_vecFolderId));
					unmarshal_container(p, back_inserter(m_vecFolderProp));
				}

		};

		struct PSS_GFillFolderPropRes : public sox::Marshallable
		{
			enum {uri = (40 << 8) | protocol::GPROPCACHE_SVID};
			
			uint32_t m_uGroupId;
			std::vector<CServerFolderProps> m_vecFolderProp; //回应的组属性列表
			std::vector<uint32_t> m_vecFolderId; //请求的组Id

			PSS_GFillFolderPropRes() {}
			PSS_GFillFolderPropRes(uint32_t Gid, std::vector<uint32_t>& vecFid)
				:m_uGroupId(Gid),m_vecFolderId(vecFid){}
			void SetProp(std::vector<CServerFolderProps>& vecProps)
			{
				m_vecFolderProp.assign(vecProps.begin(), vecProps.end());
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId;
				marshal_container(p, m_vecFolderId);
				marshal_container(p, m_vecFolderProp);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
			 	p >> m_uGroupId;
				unmarshal_container(p, back_inserter(m_vecFolderId));
				unmarshal_container(p, back_inserter(m_vecFolderProp));
			}

		};

		struct PSS_GFillFolderSimplePropRes : public sox::Marshallable
		{
			enum {uri = (151 << 8) | protocol::GPROPCACHE_SVID};

			uint32_t m_uGroupId;
			std::vector<CServerFolderProps> m_vecFolderProp; //回应的组属性列表
			std::vector<uint32_t> m_vecFolderId; //请求的组Id

			PSS_GFillFolderSimplePropRes() {}
			PSS_GFillFolderSimplePropRes(uint32_t Gid, std::vector<uint32_t>& vecFid)
				:m_uGroupId(Gid),m_vecFolderId(vecFid){}

			void SetProp(std::vector<CServerFolderProps>& vecProps)
			{
				m_vecFolderProp.assign(vecProps.begin(), vecProps.end());
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId;
				marshal_container(p, m_vecFolderId);
				marshal_container(p, m_vecFolderProp);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGroupId;
				unmarshal_container(p, back_inserter(m_vecFolderId));
				unmarshal_container(p, back_inserter(m_vecFolderProp));
			}
		};

		struct PSS_GFillFolderSimplePropRes2 : public sox::Marshallable//old
		{
			enum {uri = (51 << 8) | protocol::GPROPCACHE_SVID};

			uint32_t m_uGroupId;
			std::vector<CServerFolderProps> m_vecFolderProp; //回应的组属性列表
			std::vector<uint32_t> m_vecFolderId; //请求的组Id

			PSS_GFillFolderSimplePropRes2() {}
			PSS_GFillFolderSimplePropRes2(uint32_t Gid, std::vector<uint32_t>& vecFid)
				:m_uGroupId(Gid),m_vecFolderId(vecFid){}

			void SetProp(std::vector<CServerFolderProps>& vecProps)
			{
				m_vecFolderProp.assign(vecProps.begin(), vecProps.end());
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId;
				marshal_container(p, m_vecFolderId);
				marshal_container(p, m_vecFolderProp);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGroupId;
				unmarshal_container(p, back_inserter(m_vecFolderId));
				unmarshal_container(p, back_inserter(m_vecFolderProp));
			}
		};

		/****************************************************
        *  GProp向GpropCache更新组属性(添加/更新).
        ****************************************************/
		struct PSS_GFolderPropUpdate2 : public sox::Marshallable
		{
			enum {uri = (12 << 8) | protocol::GPROPCACHE_SVID};

			uint32_t  m_uGroupId;
			CServerFolderProps2 m_props;

			PSS_GFolderPropUpdate2() {}
			PSS_GFolderPropUpdate2(const PSS_NewFolderProps2& obj)
			{
				m_props.m_uFolderId = obj.m_uFolderId;
				m_props.m_strFolderName = obj.m_strFolderName;
				m_props.m_strFolderDesc   = obj.m_strFolderDesc;
				m_props.m_strFolderBulletin = obj.m_strFolderBulletin;
			}

			PSS_GFolderPropUpdate2(const CServerFolderProps2& obj)
			{
				m_props = obj;
			}

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


		struct PSS_GFolderPropUpdate : public sox::Marshallable
		{
			enum {uri = (42 << 8) | protocol::GPROPCACHE_SVID};

			uint32_t  m_uGroupId;
			CServerFolderProps m_props;

			PSS_GFolderPropUpdate(){}

			PSS_GFolderPropUpdate(PSS_NewFolderProps& obj)
			{
				m_props.m_uFolderId         = obj.m_uFolderId;
				m_props.m_strFolderName     = obj.m_mapProps.GetName();
				m_props.m_strFolderDesc     = obj.m_mapProps.GetDesc();
				m_props.m_strFolderBulletin = obj.m_mapProps.GetBulletin();
				m_props.m_eAuthMod          = obj.m_mapProps.GetAuthMode();
				obj.m_mapProps.GetChannelInfo(m_props.m_uMapChanAuthInfo);
			}

			PSS_GFolderPropUpdate(const CServerFolderProps& obj)
			{
				m_props = obj;
			}

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
        *  GProp通知GpropCache删除缓存中的组属性.
        ****************************************************/
		struct PSS_GFolderPropDel : public sox::Marshallable
		{
			enum {uri = (11 << 8) | protocol::GPROPCACHE_SVID};

			uint32_t m_uGroupId; 
			uint32_t m_uFolderId; //请求的组Id

			PSS_GFolderPropDel() {};
			PSS_GFolderPropDel(uint32_t uGid, uint32_t uFid)
				:m_uGroupId(uGid), m_uFolderId(uFid){}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uFolderId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGroupId >> m_uFolderId;
			}

		};

	}
}
