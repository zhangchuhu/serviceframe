#ifndef PGINFOSRV_H_INCLUDED
#define PGINFOSRV_H_INCLUDED

#include "common/packet.h" //sox::Marshallable
#include "common/core/base_svid.h"
#include "common/iproperty.h"
#include "common/res_code.h"

#include "common/protocol/PGInfoCommonTypes.h" //Shared definitions for server/protocol/UI
//#include "protocol/common/protocol/pgtransfer.h" //为了能包含EOfflineMsgMode的定义
#include "server_common/protocol/PGTransferSrv.h"  // For E_GMEM_UPDATE_OPER_FLAG
#include "common/protocol/PGInfo.h"
#include "server_common/protocol/psession.h"

//Forward definition for gtransfer

namespace protocol
{
	namespace gtransfer
	{
		//将结构体转换成字符串
		void Packet2String(const sox::Marshallable &objMarshal, std::string &strPacket);
	}
}


namespace protocol{
	namespace ginfo{

		// Request the member lists of the specified folders
		/*
		struct PSS_GetGrpFolderMemberList : public sox::Marshallable
		{
			enum {uri = (81 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uGrpID;

			// Folder IDs. When folder ID == group ID, the list of independent members is
			// returned. If m_vecReq is empty, full member list of the group will be returned
			std::vector<uint32_t> m_vecReq;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID;
				sox::marshal_container(p, m_vecReq);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID;
				sox::unmarshal_container(p, std::back_inserter(m_vecReq));
			}

		};

		struct PSS_GetGrpFolderMemberListRes : public sox::Marshallable
		{
			enum {uri = (82 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uGrpID;

			// Mapping of FID <-> Folder member list
			FldMemberListMapType m_mapMemList;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID;
				sox::marshal_container(p, m_mapMemList);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID;
				sox::unmarshal_container(p, std::inserter(m_mapMemList,
					m_mapMemList.begin()));
			}

		};
		*/

		struct PSS_GetGrpFolderRoleList : public sox::Marshallable
		{
			enum {uri = (83 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uGrpID;

			// Folder IDs. When folder ID == group ID, the list of group admins/owner is
			// returned.
			std::vector<uint32_t> m_vecReq;

			uint32_t m_uOriUri; //原包uri，解析并派发原包时使用
			std::string m_strOriPacket; //原包数据

			PSS_GetGrpFolderRoleList() {}

			PSS_GetGrpFolderRoleList(uint32_t uURI, const std::string &strOriPacket)
			{
				m_uOriUri = uURI;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}

			PSS_GetGrpFolderRoleList(uint32_t uURI, const sox::Marshallable &objMarshal)
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

		struct PSS_GetGrpFolderRoleListRes : public sox::Marshallable
		{
			enum {uri = (84 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uGrpID;

			// Mapping of FID <-> Folder role list
			FldMemberInfoListMapType m_mapMemInfoList;

			uint32_t m_uOriUri; //原包uri，解析并派发原包时使用
			std::string m_strOriPacket; //原包数据

			PSS_GetGrpFolderRoleListRes() {}

			PSS_GetGrpFolderRoleListRes(uint32_t uURI, const std::string &strOriPacket)
			{
				m_uOriUri = uURI;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}

			PSS_GetGrpFolderRoleListRes(uint32_t uURI, const sox::Marshallable &objMarshal)
			{
				m_uOriUri = uURI;
				protocol::gtransfer::Packet2String(objMarshal, m_strOriPacket);
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID << m_uOriUri << m_strOriPacket;
				sox::marshal_container(p, m_mapMemInfoList);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID >> m_uOriUri >> m_strOriPacket;
				sox::unmarshal_container(p, std::inserter(m_mapMemInfoList,
					m_mapMemInfoList.begin()));
			}

		};

		/*
		// Notify gtransfer that the folder member lists are stained
		struct PSS_InvalidateGrpFolderMemberList : public sox::Marshallable
		{
			enum {uri = (81 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uGrpID;

			// Folder IDs. When folder ID == group ID, the list of independent members is
			// stained. If m_vecReq is empty, full member list of the group is stained.
			std::vector<uint32_t> m_vecReq;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID;
				sox::marshal_container(p, m_vecReq);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID;
				sox::unmarshal_container(p, std::back_inserter(m_vecReq));
			}

		};
		*/

		// Notify gtransfer that the folder role lists are stained
		struct PSS_InvalidateGrpFolderRoleList : public sox::Marshallable
		{
			enum {uri = (85 << 8 | protocol::GINFO_SVID)};

			//uint32_t m_uGrpID;

			// Folder IDs. When folder ID == group ID, the list of group admins/owner is
			// stained.
			std::vector<uint32_t> m_vecReq;

			virtual void marshal(sox::Pack &p) const
			{
				//p << m_uGrpID;
				sox::marshal_container(p, m_vecReq);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				//p >> m_uGrpID;
				sox::unmarshal_container(p, std::back_inserter(m_vecReq));
			}

		};

		enum E_ROLE_UPDATE_OPER_FLAG
		{
			ROLE_EMPTY_LIST = 0,
			ROLE_ADD_LIST = 1,
			ROLE_DEL_LIST = 2
		};

		//更新群/组成员角色列表
		struct SGRoleUpdateList : public sox::Marshallable
		{
			uint32_t    m_uGroupId;         // 群ID
			uint32_t    m_uOper;            // 操作符,取值范围见E_ROLE_UPDATE_OPER_FLAG
			uint32_t    m_uCheckSum;        // 校验和 （暂时无用）

			FldMemberInfoListMapType m_mapMemInfoList;

			SGRoleUpdateList(){}
			SGRoleUpdateList(uint32_t gid, uint32_t oper, uint32_t cksum,
				const FldMemberInfoListMapType& mapMemInfoList)
				: m_uGroupId(gid), m_uOper(oper), m_uCheckSum(cksum)
			{
				m_mapMemInfoList = mapMemInfoList;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId <<  m_uOper << m_uCheckSum;
				sox::marshal_container(p, m_mapMemInfoList);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId >>  m_uOper >> m_uCheckSum;
				sox::unmarshal_container(p, std::inserter(m_mapMemInfoList,
					m_mapMemInfoList.begin()));
			}
		};

		//批量通知cache。群组的具体变动在m_vecGmemList中携带
		struct PSS_GRoleUpdate  : public sox::Marshallable
		{
			enum {uri = (9 << 8) | protocol::GCHECK_SVID};

			//具体的变动（增减，清空）
			std::vector<SGRoleUpdateList> m_vecGmemList;

			PSS_GRoleUpdate() {};
			PSS_GRoleUpdate(const std::vector<SGRoleUpdateList> &vec)
			{
				m_vecGmemList.assign(vec.begin(), vec.end());
			}

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_vecGmemList);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_containerEx(p, m_vecGmemList);
			}
		};

		////////////////////////////////////////////////////////////////////////
		struct SGRoleUpdateListForGAuth : public sox::Marshallable
		{
			uint32_t    m_uGroupId;         // 群ID
			uint32_t    m_uOper;            // 操作符,取值范围见E_ROLE_UPDATE_OPER_FLAG

			FldUIDMapType m_mapMemInfoList;

			SGRoleUpdateListForGAuth(){}
			SGRoleUpdateListForGAuth(uint32_t gid, uint32_t oper,
				const FldUIDMapType& mapMemInfoList)
				: m_uGroupId(gid), m_uOper(oper)
			{
				m_mapMemInfoList = mapMemInfoList;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId <<  m_uOper;
				sox::marshal_container(p, m_mapMemInfoList);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId >>  m_uOper;
				sox::unmarshal_container(p, std::inserter(m_mapMemInfoList,
					m_mapMemInfoList.begin()));
			}
		};

		//批量通知cache。群组的具体变动在m_vecGmemList中携带
		struct PSS_GRoleUpdateForGAuth  : public sox::Marshallable
		{
			enum {uri = (9 << 8) | protocol::GCHECK_SVID};

			//具体的变动（增减，清空）
			std::vector<SGRoleUpdateListForGAuth> m_vecGmemList;

			PSS_GRoleUpdateForGAuth() {};
			PSS_GRoleUpdateForGAuth(const std::vector<SGRoleUpdateListForGAuth> &vec)
			{
				m_vecGmemList.assign(vec.begin(), vec.end());
			}

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_vecGmemList);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_containerEx(p, m_vecGmemList);
			}
		};

		////////////////////////////////////////////////////////////////////////

		struct PSS_DelGroupOrFolder : public sox::Marshallable
		{
			enum {uri = (0xe0 << 8 | protocol::GINFO_SVID)}; //224

			uint32_t m_uReqUID; //发起者UID
			uint32_t m_uGrpID; //群ID
			std::set<uint32_t> m_setFldID; //组ID列表
			bool m_bIsGroup; //=true 删除群；=false删除组

			PSS_DelGroupOrFolder():
				m_uReqUID(0), m_uGrpID(0), m_bIsGroup(true) {}

			PSS_DelGroupOrFolder(uint32_t uUID, uint32_t uGrpID, bool bIsGroup = true)
			{
				m_uGrpID = uGrpID;
				m_uReqUID = uUID;
				m_bIsGroup = bIsGroup;
			}

			PSS_DelGroupOrFolder(uint32_t uUID, uint32_t uGrpID, uint32_t uFldID)
			{
				m_uGrpID = uGrpID;
				m_uReqUID = uUID;
				m_bIsGroup = false;
				m_setFldID.insert(uFldID);
			}

			PSS_DelGroupOrFolder(CGInfoBaseResMsg& obj, bool bIsGroup = true)
			{
				m_uGrpID = obj.m_uGrpID;
				m_uReqUID = obj.m_uReqUID;
				m_bIsGroup = bIsGroup;
				m_setFldID.insert(obj.m_uFldID);
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uReqUID << m_uGrpID << m_bIsGroup;
				sox::marshal_container(p, m_setFldID);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uReqUID >> m_uGrpID >> m_bIsGroup;
				sox::unmarshal_container(p, std::inserter(m_setFldID, m_setFldID.begin()));
			}
		};

// Example 1:
//   FID=11:12340020,12340030,12340040,12340050,FID=211:12340020,12340030
//   4 users are forced out of group 11. And 2 of the 4 users are in a subfolder 211 which is under group 11.
// Example 2:
//  FID=11:12341111,FID=111:12341111,FID=211:12341111
//  1 user is forced out of group 11. And the subfolders he/she joined are 111 and 211.
		struct PSS_DelGroupOrFolderMember : public sox::Marshallable
		{
			enum {uri = (0xe1 << 8 | protocol::GINFO_SVID)}; //225

			uint32_t m_uReqUID; //发起者UID
			uint32_t m_uGrpID; //群ID
			std::map<uint32_t, std::set<uint32_t> > m_mapFldUID; //组ID - 成员ID列表
			bool m_bIsGroup; //=true 出群；=false 出组

			PSS_DelGroupOrFolderMember():
				m_uReqUID(0), m_uGrpID(0), m_bIsGroup(true) {}

			PSS_DelGroupOrFolderMember(uint32_t uUID, uint32_t uGrpID, bool bIsGroup = true)
			{
				m_uGrpID = uGrpID;
				m_uReqUID = uUID;
				m_bIsGroup = bIsGroup;
			}

			PSS_DelGroupOrFolderMember(uint32_t uUID, uint32_t uGrpID, uint32_t uFldID, uint32_t uDelUID)
			{
				m_uGrpID = uGrpID;
				m_uReqUID = uUID;
				m_mapFldUID[uFldID].insert (uDelUID);
				m_bIsGroup = false;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uReqUID << m_uGrpID <<  m_bIsGroup;
				sox::marshal_container(p, m_mapFldUID);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uReqUID >> m_uGrpID >> m_bIsGroup;
				sox::unmarshal_container(p, std::inserter(m_mapFldUID, m_mapFldUID.begin()));
			}
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Request to create a new group

		struct PSS_CreateGroupWithAliasID : public PCS_NewGroup
		{
			enum {uri = (0x1e2 << 8 | protocol::GINFO_SVID)}; //482

			uint32_t m_uUID;
			uint32_t m_uAliasID;
			uint32_t m_uLimit;

			PSS_CreateGroupWithAliasID() {};
			PSS_CreateGroupWithAliasID(PCS_NewGroup &obj, uint32_t uUid, uint32_t uAliasId, uint32_t uLimit) :
			PCS_NewGroup(obj), m_uUID(uUid), m_uAliasID(uAliasId), m_uLimit(uLimit) {}

			virtual void marshal(sox::Pack &p) const
			{
				PCS_NewGroup::marshal(p);
				p << m_uUID << m_uAliasID << m_uLimit;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_NewGroup::unmarshal(p);
				p >> m_uUID >> m_uAliasID >> m_uLimit;
			}
		};

		struct PSS_DeleteGroupAliasID : public sox::Marshallable
		{
			enum {uri = (0xe3 << 8 | protocol::GINFO_SVID)}; //227

			uint32_t m_uUid;
			uint32_t m_uAliasID;
			//uint32_t m_uResCode;

			PSS_DeleteGroupAliasID() : m_uUid(0), m_uAliasID(0)/*, m_uResCode(RES_SUCCESS)*/ {}
			PSS_DeleteGroupAliasID(uint32_t uUid, uint32_t uAliasId) :
				m_uUid(uUid), m_uAliasID(uAliasId)/*, m_uResCode(uResCode)*/ {}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uAliasID/* << m_uResCode*/;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_uAliasID/* >> m_uResCode*/;
			}
		};

		struct PSS_CreateGroupWithAliasIDSuper : public PCS_NewGroup
		{
			enum {uri = (300 << 8 | protocol::GMONITOR_SVID)}; 

			uint32_t m_uUID;
			uint32_t m_uAliasID;
			uint32_t m_uLimit;

			PSS_CreateGroupWithAliasIDSuper() {};
			PSS_CreateGroupWithAliasIDSuper(PCS_NewGroup &obj, uint32_t uUid, uint32_t uAliasId, uint32_t uLimit) :
			PCS_NewGroup(obj), m_uUID(uUid), m_uAliasID(uAliasId), m_uLimit(uLimit) {}

			virtual void marshal(sox::Pack &p) const
			{
				PCS_NewGroup::marshal(p);
				p << m_uUID << m_uAliasID << m_uLimit;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_NewGroup::unmarshal(p);
				p >> m_uUID >> m_uAliasID >> m_uLimit;
			}
		};
		////////////////////////////////////////////////////////////////////////
		struct PSS_DelGroupSuper : public CGInfoBaseMsg
		{
			enum {uri = (301 << 8 | protocol::GMONITOR_SVID)};
		};

		struct PSS_DelGroupSuperRes : public CGInfoBaseResMsg
		{
			enum {uri = (302 << 8 | protocol::GMONITOR_SVID)};
		};

		struct PSS_DelGroupFolderSuper : public PCS_DelGroupFolder2
		{
			enum {uri = (303 << 8 | protocol::GMONITOR_SVID)};

//			virtual void marshal(sox::Pack &p) const
//			{
//				PCS_DelGroupFolder::marshal(p);
//			}
//
//			virtual void unmarshal(const sox::Unpack &p)
//			{
//				PCS_DelGroupFolder::unmarshal(p);
//			}

		};

		struct PSS_DelGroupFolderSuperRes : public PCS_DelGroupFolderRes2
		{
			enum {uri = (304 << 8 | protocol::GMONITOR_SVID)};

//			virtual void marshal(sox::Pack &p) const
//			{
//				PCS_DelGroupFolderRes::marshal(p);
//			}
//
//			virtual void unmarshal(const sox::Unpack &p)
//			{

//				PCS_DelGroupFolderRes::unmarshal(p);
//			}

		};

		struct PSS_AddGroupMemberSuper : public CGInfoBaseMsg
		{
			enum {uri = (305 << 8 | protocol::GMONITOR_SVID)};

			// UID of the new member
			std::vector<uint32_t> m_uUIDlist;
			uint32_t m_uAliasID;			
			//bool m_bReject;
			std::map<uint32_t, std::string> m_cardNameList;

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseMsg::marshal(p);
				p << m_uAliasID;
				sox::marshal_container(p,m_uUIDlist);
				sox::marshal_container(p,m_cardNameList);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseMsg::unmarshal(p);
				p >> m_uAliasID;
				sox::unmarshal_container(p, std::inserter(m_uUIDlist, m_uUIDlist.begin()));
				sox::unmarshal_container(p, std::inserter(m_cardNameList, m_cardNameList.begin()));
			}

		};

		struct PSS_SetMemberCardSuper : public CGInfoBaseMsg
		{
			enum {uri = (306 << 8 | protocol::GMONITOR_SVID)};

			std::map<uint32_t, std::string> m_cardNameList;
			//bool m_bReject;

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseMsg::marshal(p);
				sox::marshal_container(p,m_cardNameList);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseMsg::unmarshal(p);
				sox::unmarshal_container(p, std::inserter(m_cardNameList, m_cardNameList.begin()));
			}

		};

        /*GListCache requests ginfo to sync up user's Joined FID List and Admin FID List.*/
        struct PSS_SyncJoinedAdminFldsList : public sox::Marshallable
        {
            enum {uri = (200 << 8) | protocol::GINFO_SVID};

            uint32_t m_uUid;

            std::set<uint32_t> m_setReqGids;//GIDs Requested

            virtual void marshal(sox::Pack &p) const
            {
                p << m_uUid;
                sox::marshal_container(p, m_setReqGids);
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                p >> m_uUid;
                sox::unmarshal_container(p, std::inserter(m_setReqGids, m_setReqGids.begin()));
            }
        };

        /*Msg - Ginfo responds glistcache's sync up request.*/
        struct PSS_SyncJoinedAdminFldsRes : public sox::Marshallable
        {
            enum {uri = (203 << 8) | protocol::GINFO_SVID};

            typedef std::map<uint32_t, std::set<uint32_t> > mapList;
            typedef std::map<uint32_t, std::set<uint32_t> >::const_iterator citList;

            //Members List.
            uint32_t m_uUid;              //My UID
            mapList m_mapJoinedFldsList;//Joined Folders List.
            mapList m_mapAdminFldsList;//Admin Folders List.
            std::set<uint32_t> m_setGhostGids; //Ghost GIDs in the original request list. set<GIDs>

            //Marshal & Unmarshal
            virtual void marshal(sox::Pack &p) const
            {
                p << m_uUid;
                sox::marshal_container(p, m_mapJoinedFldsList);
                sox::marshal_container(p, m_mapAdminFldsList);
                sox::marshal_container(p, m_setGhostGids);
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                p >> m_uUid;
                sox::unmarshal_container(p, std::inserter(m_mapJoinedFldsList, m_mapJoinedFldsList.begin()));
                sox::unmarshal_container(p, std::inserter(m_mapAdminFldsList, m_mapAdminFldsList.begin()));
                sox::unmarshal_container(p, std::inserter(m_setGhostGids, m_setGhostGids.begin()));
            }
        };

		/*Gauth to get group ban list from ginfo.*/
		struct PSS_GetGrpChatBanList : public sox::Marshallable
		{
			enum {uri = (91 << 8) | protocol::GINFO_SVID};

			//members list
			uint32_t 			m_uUid;
			uint32_t 			m_uImlinkdID;
			uint32_t 			m_uGid;
			std::set<uint32_t> 	m_setFids;
			uint32_t 			m_uOriUri; //original uri
			std::string 		m_strOriPacket;//original packet

			//construct && destruct
			PSS_GetGrpChatBanList(void) :
				m_uUid(0),
				m_uImlinkdID(0),
				m_uGid(0),
				m_uOriUri(0xff),
				m_strOriPacket("")
			{
				//m_setFids.clear();
			}

			PSS_GetGrpChatBanList
			(
				const uint32_t uUid, const uint32_t uImlkdID, const uint32_t uGid,
				const std::set<uint32_t> &setFids, const uint32_t uOriUri, const std::string &strOriPacket
			) :
				m_uUid(uUid),
				m_uImlinkdID(uImlkdID),
				m_uGid(uGid),
				m_setFids(setFids),
				m_uOriUri(uOriUri),
				m_strOriPacket(strOriPacket)
			{}

			//marshal && unmarshal
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uImlinkdID << m_uGid;
				sox::marshal_container(p, m_setFids);
				p << m_uOriUri << m_strOriPacket;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_uImlinkdID >> m_uGid;
				sox::unmarshal_container(p, std::inserter(m_setFids, m_setFids.begin()));
				p >> m_uOriUri >> m_strOriPacket;
			}
		};

		struct PSS_GetGrpChatBanListRes : public sox::Marshallable
		{
			enum {uri = (92 << 8) | protocol::GINFO_SVID};

			//members list
			uint32_t 			m_uUid;
			uint32_t 			m_uImlinkdID;
			uint32_t 			m_uGid;
			FldUIDMapType 		m_mapGFidUids;//map<GID/FID, set<UIDs> >
			uint32_t 			m_uOriUri; //original uri
			std::string 		m_strOriPacket;//original packet

			//construct && deconstruct
			PSS_GetGrpChatBanListRes(void) :
				m_uUid(0),
				m_uImlinkdID(0),
				m_uGid(0),
				m_uOriUri(0xff),
				m_strOriPacket("")
			{
				//m_mapGFidUids.clear();
			}

			PSS_GetGrpChatBanListRes
			(
				const uint32_t uUid, const uint32_t uImlkdID, const uint32_t uGid,
				const FldUIDMapType &mapGFidUids, const uint32_t uOriUri, const std::string &strOriPacket
			) :
				m_uUid(uUid),
				m_uImlinkdID(uImlkdID),
				m_uGid(uGid),
				m_mapGFidUids(mapGFidUids),
				m_uOriUri(uOriUri),
				m_strOriPacket(strOriPacket)
			{}

			PSS_GetGrpChatBanListRes
			(
				const uint32_t uUid, const uint32_t uImlkdID, const uint32_t uGid,
				const uint32_t uOriUri, const std::string &strOriPacket
			) :
				m_uUid(uUid),
				m_uImlinkdID(uImlkdID),
				m_uGid(uGid),
				m_uOriUri(uOriUri),
				m_strOriPacket(strOriPacket)
			{}

			//marshal && unmarshal
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uImlinkdID << m_uGid;
				sox::marshal_container(p, m_mapGFidUids);
				p << m_uOriUri << m_strOriPacket;
			}

			virtual	void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_uImlinkdID >> m_uGid;
				sox::unmarshal_container(p, std::inserter(m_mapGFidUids, m_mapGFidUids.begin()));
				p >> m_uOriUri >> m_strOriPacket;
			}
		};

		enum E_GRP_BAN_LIST_OP
		{
			E_BAN_LIST_GRP_EMPTY = 0,
			E_BAN_LIST_FLD_EMPTY,
			E_BAN_LIST_ADD,
			E_BAN_LIST_DEL,

			E_BAN_LIST_INVALID = 0xff
		};
		/*Ginfo notifies gauth update group ban list.*/
		struct SGBanListUpdateForGAuth : public sox::Marshallable
		{

			//members list
			uint32_t 			m_uGid;
			E_GRP_BAN_LIST_OP 	m_eBanListOp; //the specific operation type.
			FldUIDMapType		m_mapGFidUids;//map<GID/FID, set<UIDs> >

			//construct && deconstruct
			SGBanListUpdateForGAuth(void) :
				m_uGid(0),
				m_eBanListOp(E_BAN_LIST_INVALID)
			{
				//m_mapGFidUids.clear()
			}

			SGBanListUpdateForGAuth
			(
				const uint32_t uGid,
				const E_GRP_BAN_LIST_OP eBanListOp,
				const FldUIDMapType &mapGFidUids
			) :
				m_uGid(uGid),
				m_eBanListOp(eBanListOp),
				m_mapGFidUids(mapGFidUids)
			{}

			//marshal && unmarshal
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGid << (uint32_t)m_eBanListOp;
				sox::marshal_container(p, m_mapGFidUids);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				uint32_t uBanListOp;

				p >> m_uGid >> uBanListOp;
				sox::unmarshal_container(p, std::inserter(m_mapGFidUids, m_mapGFidUids.begin()));

				m_eBanListOp = (E_GRP_BAN_LIST_OP)uBanListOp;
			}
		};

		struct PSS_GChatBanListUpdateForGAuth : public sox::Marshallable
		{
			enum {uri = (8 << 8) | protocol::GAUTH_SVID};

			/*The update list: Add, delete, empty.*/
			std::vector<SGBanListUpdateForGAuth> m_VecGrpBanUpdateList;

			//construct && deconstruct
			PSS_GChatBanListUpdateForGAuth(void)
			{
				//m_VecGrpBanUpdateList.clear();
			}

			PSS_GChatBanListUpdateForGAuth(const std::vector<SGBanListUpdateForGAuth> &vecGrpBanUpdateList) :
				m_VecGrpBanUpdateList(vecGrpBanUpdateList)
			{}

			//marshal && unmarshal
			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_VecGrpBanUpdateList);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_containerEx(p, m_VecGrpBanUpdateList);
			}
		};

		struct PSS_GetChannelMembersByRoleResFilter : public server::session::PSS_GetChannelMembersByRoleRes
		{
			enum {uri = (93 << 8 | protocol::GINFO_SVID)};

			PSS_GetChannelMembersByRoleResFilter()
			{
			}

			PSS_GetChannelMembersByRoleResFilter(PSS_GetChannelMembersByRole& objOri):
			PSS_GetChannelMembersByRoleRes(objOri)
			{
			}

		};

		struct PSS_DelGroupOrFolderEx : public PSS_DelGroupOrFolder
		{
			enum {uri = (94 << 8 | protocol::GINFO_SVID)};
			
			std::map<uint32_t, std::set<uint32_t> > m_mapUIDJoinedFlds;
			
			PSS_DelGroupOrFolderEx(const PSS_DelGroupOrFolder& obj):PSS_DelGroupOrFolder(obj)
			{}

			PSS_DelGroupOrFolderEx():PSS_DelGroupOrFolder(0,0,true)
			{}

			PSS_DelGroupOrFolderEx(uint32_t uUID, uint32_t uGrpID, bool bIsGroup = true)
			{
				m_uGrpID = uGrpID;
				m_uReqUID = uUID;
				m_bIsGroup = bIsGroup;
			}

			PSS_DelGroupOrFolderEx(uint32_t uUID, uint32_t uGrpID, uint32_t uFldID)
			{
				m_uGrpID = uGrpID;
				m_uReqUID = uUID;
				m_bIsGroup = false;
			}

			PSS_DelGroupOrFolderEx(CGInfoBaseResMsg& obj, bool bIsGroup = true)
			{
				m_uGrpID = obj.m_uGrpID;
				m_uReqUID = obj.m_uReqUID;
				m_bIsGroup = bIsGroup;
			}

			virtual void marshal(sox::Pack &p) const
			{
				//p << m_uReqUID << m_uGrpID << m_bIsGroup;
				PSS_DelGroupOrFolder::marshal(p);
				sox::marshal_container(p, m_mapUIDJoinedFlds);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				//p >> m_uReqUID >> m_uGrpID >> m_bIsGroup;
				PSS_DelGroupOrFolder::unmarshal(p);
				sox::unmarshal_container(p, std::inserter(m_mapUIDJoinedFlds, m_mapUIDJoinedFlds.begin()));
			}

		};
		
	}
}


#endif // PGINFOSRV_H_INCLUDED
