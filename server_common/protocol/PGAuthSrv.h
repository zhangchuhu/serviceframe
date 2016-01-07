#pragma once

#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"

#include "protocol/PGPropertyType.h"
#include "protocol/pgtopicbase.h"

#include "protocol/common/protocol/PGTransfer.h" 

#include <iostream>
#include <string>

namespace server
{
	namespace gauth
	{
	// ----- Interact with GInfo ------
		struct PSS_GAuthGrpFldAdminsReq : public sox::Marshallable
		{
			enum {uri = (1 << 8 | protocol::GAUTH_SVID)};

			uint32_t m_uGrpID;  // 群ID
			uint32_t m_uOriUri; //protocol 原始uri
			std::string m_strOriPacket; // protocol 原始包
			std::vector<uint32_t> m_vecReq; //请求权限的Group/Folder

			PSS_GAuthGrpFldAdminsReq() {}

			PSS_GAuthGrpFldAdminsReq(uint32_t uURI, const std::string &strOriPacket)
			{
				m_uOriUri = uURI;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}

			PSS_GAuthGrpFldAdminsReq(uint32_t uURI, const sox::Marshallable &objMarshal)
			{
				m_uOriUri = uURI;
				protocol::gtransfer::Packet2String(objMarshal, m_strOriPacket);
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID << m_uOriUri << m_strOriPacket;
				sox::marshal_container(p, m_vecReq);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID >> m_uOriUri >> m_strOriPacket;
				sox::unmarshal_container(p, std::back_inserter(m_vecReq));
			}

		};

		struct PSS_GAuthGrpFldAdminsRes : public sox::Marshallable
		{
			enum {uri = (2 << 8 | protocol::GAUTH_SVID)};

			uint32_t m_uGrpID;  //群ID
			uint32_t m_uOriUri; //protocol 原始uri
			std::string m_strOriPacket; // protocol 原始包

			// vector保存请求的群和组下面的管理员的集合
			// map< fid, set <uids> >
			std::map< uint32_t, std::set<uint32_t> > m_mapAdminList;

			PSS_GAuthGrpFldAdminsRes() {}

			PSS_GAuthGrpFldAdminsRes(uint32_t uURI, const std::string &strOriPacket)
			{
				m_uOriUri = uURI;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}

			PSS_GAuthGrpFldAdminsRes(uint32_t uURI, const sox::Marshallable &objMarshal)
			{
				m_uOriUri = uURI;
				protocol::gtransfer::Packet2String(objMarshal, m_strOriPacket);
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID << m_uOriUri << m_strOriPacket;
				sox::marshal_container(p, m_mapAdminList);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID >> m_uOriUri >> m_strOriPacket;
				sox::unmarshal_container(p, std::inserter(m_mapAdminList,
					m_mapAdminList.begin()));
			}
		};


	// ----- Interact with GProps------
		struct PSS_GAuthGrpFldTopicModeReq : public sox::Marshallable
		{
			enum {uri = (3 << 8 | protocol::GAUTH_SVID)};

			uint32_t m_uGrpID;	//群ID
			uint32_t m_uFldID;	//组ID
			uint32_t m_uOriUri; //protocol 原始uri
			std::string m_strOriPacket; // protocol 原始包

			PSS_GAuthGrpFldTopicModeReq() {}

			PSS_GAuthGrpFldTopicModeReq(uint32_t uURI, const std::string &strOriPacket)
			{
				m_uOriUri = uURI;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}

			PSS_GAuthGrpFldTopicModeReq(uint32_t uURI, const sox::Marshallable &objMarshal)
			{
				m_uOriUri = uURI;
				protocol::gtransfer::Packet2String(objMarshal, m_strOriPacket);
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID << m_uFldID << m_uOriUri << m_strOriPacket;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID >> m_uFldID >> m_uOriUri >> m_strOriPacket;
			}

		};

		struct PSS_GAuthGrpFldTopicModeRes : public sox::Marshallable
		{
			enum {uri = (4 << 8 | protocol::GAUTH_SVID)};

			uint32_t m_uGrpID;   //群ID
			uint32_t m_uFldID;   //组ID
			uint32_t m_uOriUri;  //protocol 原始uri
			std::string m_strOriPacket; // protocol 原始包

			// 某个群/组下的发帖模式
			protocol::gprops::ETopicMode m_eTopicMod;

			PSS_GAuthGrpFldTopicModeRes() {}

			PSS_GAuthGrpFldTopicModeRes(uint32_t uURI, const std::string &strOriPacket)
			{
				m_uOriUri = uURI;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}

			PSS_GAuthGrpFldTopicModeRes(uint32_t uURI, const sox::Marshallable &objMarshal)
			{
				m_uOriUri = uURI;
				protocol::gtransfer::Packet2String(objMarshal, m_strOriPacket);
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID << m_uFldID << m_uOriUri << m_strOriPacket;
				p.push_uint8(m_eTopicMod);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID >> m_uFldID >> m_uOriUri >> m_strOriPacket;
				m_eTopicMod = (protocol::gprops::ETopicMode)p.pop_uint8();
			}
		};


	// ----- Interact with GTopic------
		struct PSS_GAuthTopicPermissionReq : public sox::Marshallable
		{
			enum {uri = (5 << 8 | protocol::GAUTH_SVID)};

			uint32_t m_uGrpID;   // 群ID
			uint32_t m_uFldID;   // 组ID
			uint32_t m_uOriUri;  //protocol 原始uri
			std::string m_strOriPacket; // protocol 原始包

			PSS_GAuthTopicPermissionReq() {}

			PSS_GAuthTopicPermissionReq(uint32_t uURI, const std::string &strOriPacket)
			{
				m_uOriUri = uURI;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}

			PSS_GAuthTopicPermissionReq(uint32_t uURI, const sox::Marshallable &objMarshal)
			{
				m_uOriUri = uURI;
				protocol::gtransfer::Packet2String(objMarshal, m_strOriPacket);
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID << m_uFldID << m_uOriUri << m_strOriPacket;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID >> m_uFldID >> m_uOriUri >> m_strOriPacket;
			}

		};

		struct PSS_GAuthTopicPermissionRes : public sox::Marshallable
		{
			enum {uri = (6 << 8 | protocol::GAUTH_SVID)};

			uint32_t m_uGrpID;  //群ID
			uint32_t m_uFldID;  //组ID
			uint32_t m_uOriUri; //protocol 原始uri
			std::string m_strOriPacket; // protocol 原始包

			// 某个群/组下的所有有限制的主题
			// map< tid, GTopicPermission >
			std::map< uint32_t, uint32_t/*protocol::gtopic::GTopicPermission*/ > m_mapTopicPermssion;

			PSS_GAuthTopicPermissionRes() {}

			PSS_GAuthTopicPermissionRes(uint32_t uURI, const std::string &strOriPacket)
			{
				m_uOriUri = uURI;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}

			PSS_GAuthTopicPermissionRes(uint32_t uURI, const sox::Marshallable &objMarshal)
			{
				m_uOriUri = uURI;
				protocol::gtransfer::Packet2String(objMarshal, m_strOriPacket);
			}


			PSS_GAuthTopicPermissionRes(PSS_GAuthTopicPermissionReq& msg)
			{
				m_uGrpID=msg.m_uGrpID;
				m_uFldID=msg.m_uFldID;
				m_uOriUri=msg.m_uOriUri;
				m_strOriPacket=msg.m_strOriPacket;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID << m_uFldID << m_uOriUri << m_strOriPacket;
				sox::marshal_container(p, m_mapTopicPermssion);
			}


			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID >> m_uFldID >> m_uOriUri >> m_strOriPacket;
				sox::unmarshal_container(p, std::inserter(m_mapTopicPermssion,
					m_mapTopicPermssion.begin()));
			}
		};

		enum E_GTOPIC_UPDATE_OPER_PERMISSION
		{
			ADD_TOPIC_PERMISSION = 1,
			DEL_TOPIC_PERMISSION = 2,
			DEL_TOPIC_FOR_TID_CACHE = 4
		};

		struct PSS_GAuthTopicPermissionUpdate : public sox::Marshallable
		{
			enum {uri = (7 << 8 | protocol::GAUTH_SVID)};

			uint32_t m_uOper;
			uint32_t m_uGrpID;
			uint32_t m_uFldID;
			uint32_t m_uTopicID;
			uint32_t m_uPermission;

			PSS_GAuthTopicPermissionUpdate() 
			{
				m_uOper = 0;
				m_uGrpID = 0;
				m_uFldID = 0;
				m_uTopicID = 0;
				m_uPermission = 0;
			}

			PSS_GAuthTopicPermissionUpdate(uint32_t oper, uint32_t gid, uint32_t fid, uint32_t tid=0, uint32_t permission=0)
			{
				m_uOper = oper;
				m_uGrpID = gid;
				m_uFldID = fid;
				m_uTopicID = tid;
				m_uPermission = permission;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uOper << m_uGrpID << m_uFldID << m_uTopicID << m_uPermission;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uOper >> m_uGrpID >> m_uFldID >> m_uTopicID >> m_uPermission;
			}
		};

		// GAuth -> GActivity
		// request group/folder activity(latest reply time) list
		struct PSS_GetGrpFldActivityList : public sox::Marshallable 
		{
			enum {uri = (9 << 8) | protocol::GAUTH_SVID};
			
			uint32_t 	m_uid;
			uint32_t	m_imlinkdId;
			uint32_t	m_groupId;     
			uint32_t	m_folderId;     

			PSS_GetGrpFldActivityList() : m_uid(-1), m_imlinkdId(-1), m_groupId(-1), m_folderId(-1){}

			PSS_GetGrpFldActivityList(uint32_t uid, uint32_t imlinkdId, uint32_t groupId, uint32_t folderId)
				: m_uid(uid), m_imlinkdId(imlinkdId), m_groupId(groupId), m_folderId(folderId)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uid << m_imlinkdId << m_groupId << m_folderId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uid >> m_imlinkdId >> m_groupId >> m_folderId;
			}
		};

		// GActivity -> GAuth
		// response of group/folder activity(latest reply time) list
		struct PSS_GetGrpFldActivityListRes : public sox::Marshallable 
		{
			enum {uri = (10 << 8) | protocol::GAUTH_SVID};
			
			uint32_t 	m_uid;
			uint32_t	m_imlinkdId;			
			uint32_t    m_groupId;      
			uint32_t    m_folderId;     

			// key(uid) => latest reply time
			std::map<uint32_t, uint32_t> m_mapUid2LastReplyTime;

			PSS_GetGrpFldActivityListRes() : m_uid(-1), m_imlinkdId(-1), m_groupId(-1), m_folderId(-1){}

			PSS_GetGrpFldActivityListRes(const PSS_GetGrpFldActivityList& objReq, 
										 const std::map<uint32_t, uint32_t>& mapUid2LastReplyTime)
				: m_uid(objReq.m_uid), m_imlinkdId(objReq.m_imlinkdId), m_groupId(objReq.m_groupId),
				  m_folderId(objReq.m_folderId), m_mapUid2LastReplyTime(mapUid2LastReplyTime)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uid << m_imlinkdId << m_groupId << m_folderId;
				sox::marshal_container(p, m_mapUid2LastReplyTime);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uid >> m_imlinkdId >> m_groupId >> m_folderId;
				sox::unmarshal_container(p, std::inserter(m_mapUid2LastReplyTime, m_mapUid2LastReplyTime.begin()));
			}
		};

		// GAuth -> GActivity
		struct PSS_UpdateUserActivity : public sox::Marshallable 
		{
			enum {uri = (11 << 8) | protocol::GAUTH_SVID};
			
			uint32_t 	m_uid;			
			uint32_t    m_groupId;      
			uint32_t    m_folderId;  
			uint32_t 	m_timestamp; 

			PSS_UpdateUserActivity() : m_uid(-1), m_groupId(-1), m_folderId(-1), m_timestamp(-1){}

			PSS_UpdateUserActivity(uint32_t uid, uint32_t groupId, uint32_t folderId, uint32_t timestamp)
				: m_uid(uid), m_groupId(groupId), m_folderId(folderId), m_timestamp(timestamp)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uid << m_groupId << m_folderId << m_timestamp;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uid >> m_groupId >> m_folderId >> m_timestamp;
			}
		};

		enum EUpdateType
		{
			ENUM_DEL_GROUP = 0,	//  删除群
			ENUM_DEL_FOLDER, 	//  删除组
			ENUM_DEL_USER,		//  删除用户
		};

		struct PSS_UpdateGActivityCache : public sox::Marshallable
		{
			enum {uri = (12 << 8) | protocol::GAUTH_SVID};
			
			EUpdateType m_OperType;	//操作类型		
			uint32_t m_uGrpID; 		//群id
		
			//组id-uid列表,map为空删除群，uid列表为空则删除组
			std::map<uint32_t, std::set<uint32_t> > m_mapFldUID; 

			PSS_UpdateGActivityCache(){}
			PSS_UpdateGActivityCache(EUpdateType type, uint32_t uGrpID, std::map<uint32_t, std::set<uint32_t> >& mapData)
			{
				m_OperType = type;
				m_uGrpID = uGrpID;
				m_mapFldUID = mapData;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID;
				p.push_uint8(m_OperType);
				sox::marshal_container(p, m_mapFldUID);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID;
				m_OperType = (EUpdateType)p.pop_uint8();	
				sox::unmarshal_container(p, std::inserter(m_mapFldUID, m_mapFldUID.begin()));
			}
		};
    }
}
