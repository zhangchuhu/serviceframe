//文件描述:
//本文件为ginfo所用消息的协议定义
//
//修改历史:
//2009/12/16 LinYue Initial version
//2010/02/23 LinYue 增加PCS_GetGrpFolderListsWithMyRoles，用于在获取folder list时也获取
//                  群角色列表和自己的角色列表

#ifndef PGINFO_H_INCLUDED
#define PGINFO_H_INCLUDED

#include "common/packet.h" //sox::Marshallable
#include "common/core/base_svid.h"
#include "common/iproperty.h"
#include "common/res_code.h"
#include "./pgverifycodepng.h"

//#include "GPropsCommonType.h"
#include "PGProperty.h"
#include "protocol/PGInfoCommonTypes.h" //Shared definitions for server/protocol/UI

namespace protocol{
	namespace ginfo{

		// The common fields for all GInfo messages
		struct CGInfoBaseMsg : public virtual sox::Marshallable
		{
			uint32_t m_uGrpID; // Group ID on which the operation is applied
			//uint32_t m_uSenderID; // UID of the operation requester
			//uint32_t m_uErrCode;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID; // << m_uErrCode;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID; // >> m_uErrCode;
			}

		};

		// The common fields for all GInfo messages
		struct CGInfoBaseResMsg : public virtual sox::Marshallable
		{
			uint32_t m_uGrpID; // Group ID on which the operation is applied
			uint32_t m_uFldID;
			uint32_t m_uReqUID; // UID of the operation requester
			uint32_t m_uResCode;

			CGInfoBaseResMsg()
			{
				InitBaseResVal();
			}

			CGInfoBaseResMsg(uint32_t uGrpID, uint32_t uFldID, uint32_t uReqUID, uint32_t uRC = RES_SUCCESS) :
				m_uGrpID(uGrpID),m_uFldID(uFldID),m_uReqUID(uReqUID),m_uResCode(uRC){}

			virtual void InitBaseResVal()
			{
				m_uGrpID = 0;
				m_uFldID = 0;
				m_uReqUID = 0;
				m_uResCode = RES_SUCCESS;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID << m_uFldID << m_uReqUID << m_uResCode;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID >> m_uFldID >> m_uReqUID >> m_uResCode;
			}

		};

		struct CGInfoBaseMsgExt : public sox::Marshallable
		{
			uint32_t m_uGrpID; // Group ID on which the operation is applied
			uint8_t m_uExt;

			CGInfoBaseMsgExt() : m_uExt(0) {}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID << m_uExt;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID >> m_uExt;
			}

		};

		struct CGInfoBaseResMsgExt : public CGInfoBaseResMsg
		{
			uint8_t m_uExt;

			CGInfoBaseResMsgExt() : CGInfoBaseResMsg(), m_uExt(0) {}

			CGInfoBaseResMsgExt(uint32_t uGrpID, uint32_t uFldID, uint32_t uReqUID, uint32_t uRC = RES_SUCCESS) :
				CGInfoBaseResMsg(uGrpID, uFldID, uReqUID, uRC){}

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseResMsg::marshal(p);
				p << m_uExt;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseResMsg::unmarshal(p);
				p >> m_uExt;
			}

		};

		// Basic information for a group/folder
		struct CPrimitiveGroup : public virtual sox::Marshallable
		{
			std::string m_strDesc;
			std::string m_strName;
			std::string m_strBulletin;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strDesc << m_strName << m_strBulletin;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_strDesc >> m_strName >> m_strBulletin;
			}
		};

///////////////////////////////////////////////////////////////////////////////

		// Get information of multiple groups
		// Deprecated!
		struct PCS_GetGroupFolderLists : public sox::Marshallable
		{
			enum {uri = (61 << 8 | protocol::GINFO_SVID)};

			std::vector<uint32_t> m_vecReq;

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_vecReq);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::back_inserter(m_vecReq));
			}

		};

		// Deprecated!
		/*
		struct PCS_GetGroupFolderListRes : public sox::Marshallable
		{
			enum {uri = (61 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uResCode;

			// Key - Group ID
			MultiGroupFolderListsMapType m_mapMultiGrpFolderLists;
			// <uGrpID, CGrpFldListRes>
			//          ^---<uFldID, CGrpFolderInfo>

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uResCode;
				sox::marshal_container(p, m_mapMultiGrpFolderLists);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uResCode;
				sox::unmarshal_container(p, std::inserter(m_mapMultiGrpFolderLists,
					m_mapMultiGrpFolderLists.begin()));
			}

		};
		*/

		struct PCS_GetMyJoinedFolders : public sox::Marshallable
		{
			enum {uri = (62 << 8 | protocol::GINFO_SVID)};

			std::vector<uint32_t> m_vecReq;

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_vecReq);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::back_inserter(m_vecReq));
			}
		};

		struct PCS_GetMyJoinedFoldersRes : public sox::Marshallable
		{
			enum {uri = (62 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uResCode;

			// Key - Group ID
			IDListMapType m_mapGrpFld;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uResCode;
				sox::marshal_container(p, m_mapGrpFld);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uResCode;
				sox::unmarshal_container(p, std::inserter(m_mapGrpFld,
					m_mapGrpFld.begin()));
			}
		};

		// 该消息由protocol 发给Gtransfer，用于请求组列表
		struct PCS_GFoldersReq : public sox::Marshallable
		{
			enum {uri = (111 << 8) | protocol::GINFO_SVID};
			uint32_t    m_uGroupId;
			uint32_t    m_uUid;    //Filled by the server
			uint32_t    m_uImlinkdId;    //Filled by the server

			PCS_GFoldersReq(){}
			// 			PCS_GFoldersReq(uint32_t groudId, uint32_t uid, uint32_t imlinkdId)
			// 				: m_uGroupId(groudId), m_uUid(uid), m_uImlinkdId(imlinkdId)
			// 			{}
			PCS_GFoldersReq(uint32_t groudId)
				: m_uGroupId(groudId), m_uUid(0), m_uImlinkdId(0)
			{}
			// 			PCS_GFoldersReq(uint32_t groudId)
			// 				: m_uGroupId(groudId)
			// 			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uUid << m_uImlinkdId;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId >> m_uUid >> m_uImlinkdId;
			}
		};

		// 该消息由gtransfer 发给protocol，用于响应PCS_GFoldersReq
		struct PCS_GFoldersRes : public sox::Marshallable
		{
			enum {uri = (112 << 8) | protocol::GINFO_SVID};
			uint32_t	m_uGroupId;    // 请求者群id
			std::vector<uint32_t>    m_vecFolderList;    // 获得的folder list

			PCS_GFoldersRes(){}
			PCS_GFoldersRes(uint32_t groudId, const std::vector<uint32_t>& vec)
				: m_uGroupId(groudId)
			{
				m_vecFolderList = vec;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId;
				sox::marshal_container(p, m_vecFolderList);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId;
				sox::unmarshal_container(p, std::back_inserter(m_vecFolderList));
			}
		};

		struct PCS_GetMyRoleInFolders : public PCS_GetMyJoinedFolders
		{
			enum {uri = (64 << 8 | protocol::GINFO_SVID)};
		};

		struct PCS_GetMyRoleInFoldersRes : public PCS_GetMyJoinedFoldersRes
		{
			enum {uri = (64 << 8 | protocol::GINFO_SVID)};
		};

		// Also get group admins/owner list and my roles in the group
		// Deprecated!
		struct PCS_GetGrpFolderListsWithMyRoles : public PCS_GetGroupFolderLists
		{
			enum {uri = (69 << 8 | protocol::GINFO_SVID)};
		};

		// Deprecated!
		struct PCS_GetGrpFolderListsWithMyRolesRes : public sox::Marshallable
		{
			enum {uri = (70 << 8 | protocol::GINFO_SVID)};
			uint32_t m_uResCode;

			// Key - Group ID
			MultiGroupFolderListsMapType m_mapMultiGrpFolderLists;
			// <uGrpID, CGrpFldListRes>
			//          ^---<uFldID, CGrpFolderInfo>

			// Key - Group/Folder ID
			//MemberInfoMapType m_mapRoleList;
			FldMemberInfoListMapType m_mapRoleList;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uResCode;
				sox::marshal_container(p, m_mapMultiGrpFolderLists);
				sox::marshal_container(p, m_mapRoleList);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uResCode;
				sox::unmarshal_container(p, std::inserter(m_mapMultiGrpFolderLists,
					m_mapMultiGrpFolderLists.begin()));
				sox::unmarshal_container(p, std::inserter(m_mapRoleList,
					m_mapRoleList.begin()));
			}
		};

		// Request the member lists of the specified folders, and the independent
		//  members and folder/group admins.
		// Protocol uses it to sync its local member lists with the server's lists
		// It is sent when a group is just added at protocol and no member lists
		//  haven't been retrieved yet.
		struct PCS_SyncGrpFolderMemberList : public sox::Marshallable
		{
			enum {uri = (63 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uGrpID;
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


		// Independent members and group admins is always returned
		struct PCS_SyncGrpFolderMemberListRes : public sox::Marshallable
		{
			enum {uri = (63 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uGrpID;

			FldMemberInfoListMapType m_mapMemInfoList;

			FldMemberListMapType m_mapMemList;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID;
				sox::marshal_container(p, m_mapMemInfoList);
				sox::marshal_container(p, m_mapMemList);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID;
				sox::unmarshal_container(p, std::inserter(m_mapMemInfoList,
					m_mapMemInfoList.begin()));
				sox::unmarshal_container(p, std::inserter(m_mapMemList,
					m_mapMemList.begin()));
			}

		};

		struct PCS_UnfoldGrpFolderMemberList : public PCS_SyncGrpFolderMemberList
		{
			enum {uri = (65 << 8 | protocol::GINFO_SVID)};
		};

		// Only members/admins of specified folders are returned
		struct PCS_UnfoldGrpFolderMemberListRes : public PCS_SyncGrpFolderMemberListRes
		{
			enum {uri = (66 << 8 | protocol::GINFO_SVID)};
		};

		// The invitee rejects the invitation by sending this message.
		struct PCS_RejectInvitation : public CGInfoBaseMsg
		{
			enum {uri = (67 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uInviterUID;
			uint32_t m_uInviteeUID; //filled by ginfo
			std::string m_strReason;
			//uint32_t m_uChecksum;

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseMsg::marshal(p);
				p << m_uInviterUID << m_uInviteeUID << m_strReason;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseMsg::unmarshal(p);
				p >> m_uInviterUID >> m_uInviteeUID >> m_strReason;
			}

		};

		// The uReqUID is invited and joined
		struct PCS_RejectInvitationRes : public CGInfoBaseResMsg
		{
			enum {uri = (68 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uInviterUID;

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseResMsg::marshal(p);
				p << m_uInviterUID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseResMsg::unmarshal(p);
				p >> m_uInviterUID;
			}
		};

		//Request the "independent-member list" directly from the dbd. Should be deprecated later.
		struct PCS_GrpFolderMemberList : public PCS_SyncGrpFolderMemberList
		{
			enum {uri = (71 << 8 | protocol::GINFO_SVID)};
		};

/*
		//Now gtrans supports the "independent-member list"
		struct PCS_IndpMemberListViaCached : public PCS_GrpFolderMemberList
		{
			//enum {uri = (71 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uUid;
			uint32_t m_uImlinkdId;

			PCS_IndpMemberListViaCached(){}
			PCS_IndpMemberListViaCached(uint32_t gid, const std::vector<uint32_t>& vec)
			{
				m_uGrpID = gid;
				m_vecReq = vec;
				m_uUid = 0;
				m_uImlinkdId = 0;
			}

			virtual void marshal(sox::Pack &p) const
			{
				PCS_GrpFolderMemberList::marshal(p);
				p <<  m_uUid << m_uImlinkdId;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_GrpFolderMemberList::unmarshal(p);
				p >> m_uUid >> m_uImlinkdId;
			}

		};
*/
//		struct PCS_GrpFolderMemberListRes : public PCS_SyncGrpFolderMemberListRes
//		{
//			enum {uri = (71 << 8 | protocol::GINFO_SVID)};
//		};

		struct PCS_GrpFolderRoleList : public PCS_SyncGrpFolderMemberList
		{
			enum {uri = (72 << 8 | protocol::GINFO_SVID)};
		};

//  		struct PCS_RoleListViaCached : public PCS_SyncGrpFolderMemberList
//  		{
//  			enum {uri = (8 << 8 | protocol::GCHECK_SVID)};
//  		};

		struct PCS_MemberListViaCached : public sox::Marshallable
		{
			enum {uri = (70 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uGrpID;
			uint32_t m_uUid;
			uint32_t m_uImlinkdId;

			std::vector<uint32_t> m_vecReq;

			PCS_MemberListViaCached(){}
			PCS_MemberListViaCached(uint32_t gid, const std::vector<uint32_t>& vec)
			{
				m_uGrpID = gid;
				m_vecReq = vec;
				m_uUid = 0;
				m_uImlinkdId = 0;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID << m_uUid << m_uImlinkdId;
				sox::marshal_container(p, m_vecReq);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID >> m_uUid >> m_uImlinkdId;
				sox::unmarshal_container(p, std::back_inserter(m_vecReq));
			}

		};
//		struct PCS_GrpFolderRoleListRes : public PCS_SyncGrpFolderMemberListRes
//		{
//			enum {uri = (72 << 8 | protocol::GINFO_SVID)};
//		};

		// Transfer the ownership to another user
		struct PCS_TransferGroupOwner : public CGInfoBaseMsg
		{
			enum {uri = (31 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uNewOwnerUID;

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseMsg::marshal(p);
				p << m_uNewOwnerUID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseMsg::unmarshal(p);
				p >> m_uNewOwnerUID;
			}

		};

		struct PCS_TransferGroupOwnerRes : public CGInfoBaseResMsg
		{
			enum {uri = (32 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uNewOwnerUID;

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseResMsg::marshal(p);
				p << m_uNewOwnerUID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseResMsg::unmarshal(p);
				p >> m_uNewOwnerUID;
			}

		};

		struct PCS_TransferGroupOwnerBroc : public PCS_TransferGroupOwnerRes
		{
			enum {uri = (132 << 8 | protocol::GINFO_SVID)};
		};

		// Ask to join a group
		struct PCS_JoinGroup :public CGInfoBaseMsg
		{
			enum {uri = (33 << 8 | protocol::GINFO_SVID)};

			//Authentication message could be empty if it is not required
			// by the group
			std::string m_strAuthMsg;

			uint32_t m_uReqUID; // will be filled

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseMsg::marshal(p);
				p << m_strAuthMsg << m_uReqUID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseMsg::unmarshal(p);
				p >> m_strAuthMsg >> m_uReqUID;
			}
		};

		// User joined
		struct PCS_JoinGroupRes : public CGInfoBaseResMsg
		{
			enum {uri = (34 << 8 | protocol::GINFO_SVID)};

			PCS_JoinGroupRes() {}
			PCS_JoinGroupRes(uint32_t uGrpID, uint32_t uFldID, uint32_t uReqUID, uint32_t uRC = RES_SUCCESS):
			CGInfoBaseResMsg(uGrpID, uFldID, uReqUID, uRC){}
		};

		struct PCS_JoinGroupBroc : public PCS_JoinGroupRes
		{
			enum {uri = (134 << 8 | protocol::GINFO_SVID)};
		};

		// UserA invites userB to join the group. The request is sent to ginfo
		// for authentication first. Then ginfo fills the checksum and forwards
		// the request to UserB. If userB accepts, PCS_InvitedToGroup will be
		// sent.
		struct PCS_InviteUserToGroup : public CGInfoBaseMsg
		{
			enum {uri = (35 << 8 | protocol::GINFO_SVID)};

			// Folder admin can also invite. But folderID where the admin belongs
			//  should be provided
			uint32_t m_uFldID;
			uint32_t m_uInviteeUID;
			uint32_t m_uInviterUID; // will be filled by ginfo with the requester's UID
			uint32_t m_uChecksum; // will be filled by ginfo
			std::string m_strInvitationMsg;
			std::string m_strExtMsg;
			uint32_t m_uType;
			//time_t m_objTime;

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseMsg::marshal(p);
				p << m_uFldID << m_uInviteeUID << m_uInviterUID
					<< m_uChecksum << m_strInvitationMsg << m_strExtMsg << m_uType;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseMsg::unmarshal(p);
				p >> m_uFldID >> m_uInviteeUID >> m_uInviterUID
					>> m_uChecksum >> m_strInvitationMsg >> m_strExtMsg >> m_uType;
			}
		};

		// Used as ack only
		struct PCS_InviteUserToGroupRes : public CGInfoBaseResMsg
		{
			enum {uri = (36 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uInviteeUID;
			uint32_t m_uType;

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseResMsg::marshal(p);
				p <<  m_uInviteeUID << m_uType;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseResMsg::unmarshal(p);
				p >>  m_uInviteeUID >> m_uType;
			}

		};

		//Justin fang -TODO: BUSYBOX-INVITE
		//Msg for inviting uids to group or folder in batch.
		struct PCS_InviteUserToGrpFldBatch : public CGInfoBaseMsg
		{
			enum {uri = (120 << 8 | protocol::GINFO_SVID)};

			//members list.
			uint32_t 			m_uFid;
			uint32_t 			m_uInviterUID;/*filled by ginfo*/
			std::set<uint32_t> 	m_setInviteeUIDs;
			uint32_t 			m_uCheckSum;/*filled by ginfo*/
			std::string 		m_strWelcomeMsg;
			std::string 		m_strExtMsg;
			EInvitationType 	m_eInvitionType;

			//marshal && unmarshal
			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseMsg::marshal(p);

				p << m_uFid << m_uInviterUID;
				sox::marshal_container(p, m_setInviteeUIDs);
				p << m_uCheckSum << m_strWelcomeMsg << m_strExtMsg << (uint32_t)m_eInvitionType;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				uint32_t uInvitionType;

				CGInfoBaseMsg::unmarshal(p);

				p >> m_uFid >> m_uInviterUID;
				sox::unmarshal_container(p, std::inserter(m_setInviteeUIDs, m_setInviteeUIDs.begin()));
				p >> m_uCheckSum >> m_strWelcomeMsg >> m_strExtMsg >> uInvitionType;
				m_eInvitionType = (EInvitationType)uInvitionType;
			}
		};

		struct PCS_InviteUserToGrpFldBatchRes : public virtual sox::Marshallable
		{
			enum {uri = (121 << 8 | protocol::GINFO_SVID)};

			//members list.
			uint32_t 			m_uGid;
			uint32_t 			m_uFid;
			uint32_t 			m_uReqUID;
			EInvitationType 	m_eInvitionType;
			std::map<uint32_t, uint32_t> m_mapUidRCode;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGid << m_uFid << m_uReqUID << (uint32_t)m_eInvitionType;
				sox::marshal_container(p, m_mapUidRCode);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				uint32_t uInvitionType;

				p >> m_uGid >> m_uFid >> m_uReqUID >> uInvitionType;
				sox::unmarshal_container(p, std::inserter(m_mapUidRCode, m_mapUidRCode.begin()));
				m_eInvitionType = (EInvitationType)uInvitionType;
			}
		};

		// The invitee accepts the invitation by sending this message.
		// checksum will be verified to check if the inivtation is genuine
		struct PCS_AcceptInvitation : public CGInfoBaseMsg
		{
			enum {uri = (37 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uInviterUID;
			uint32_t m_uChecksum;
			uint32_t m_uFldID;
			uint32_t m_uType;
			uint32_t m_uInvCode;

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseMsg::marshal(p);
				p << m_uInviterUID << m_uChecksum << m_uFldID << m_uType << m_uInvCode;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseMsg::unmarshal(p);
				p >> m_uInviterUID >> m_uChecksum >> m_uFldID >> m_uType >> m_uInvCode;
			}

		};

		// The uReqUID is invited and joined
		struct PCS_AcceptInvitationRes : public CGInfoBaseResMsg
		{
			enum {uri = (38 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uInviterUID;
			uint32_t m_uFldID;
			uint32_t m_uType;
			bool m_bFromDefaultFolder; //Folder invitation only

			PCS_AcceptInvitationRes()
			{
				m_uInviterUID = m_uFldID = m_uType = 0;
				m_bFromDefaultFolder = false;
			}

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseResMsg::marshal(p);
				p << m_uInviterUID << m_uFldID << m_bFromDefaultFolder << m_uType;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseResMsg::unmarshal(p);
				p >> m_uInviterUID >> m_uFldID >> m_bFromDefaultFolder >> m_uType;
			}
		};

		struct PCS_InvitedToGroupBroc : public PCS_AcceptInvitationRes
		{
			enum {uri = (138 << 8 | protocol::GINFO_SVID)};
		};

		struct PCS_AddGroupMember : public CGInfoBaseMsg
		{
			enum {uri = (39 << 8 | protocol::GINFO_SVID)};

			// UID of the new member
			uint32_t m_uUID;
			//bool m_bReject;

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseMsg::marshal(p);
				//p << uint32_t(0); //compatibility hack!
				p << m_uUID;// << m_bReject;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseMsg::unmarshal(p);
				p >> m_uUID;
//				m_bReject = false; //compatibility hacke!
//				if (0==m_uUID)
//					p >> m_uUID >> m_bReject;
			}

		};

		struct PCS_AddGroupMemberRes : public CGInfoBaseResMsg
		{
			enum {uri = (40 << 8 | protocol::GINFO_SVID)};

			// UID of the new member
			uint32_t m_uNewUID;

			PCS_AddGroupMemberRes(){};

			PCS_AddGroupMemberRes(uint32_t uGrpID, uint32_t uReqUID,
				uint32_t uNewUID, uint32_t uRC = RES_SUCCESS) :
				CGInfoBaseResMsg(uGrpID, uGrpID, uReqUID, uRC),
				m_uNewUID(uNewUID){}

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseResMsg::marshal(p);
				p << m_uNewUID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseResMsg::unmarshal(p);
				p >> m_uNewUID;
			}

		};

		struct PCS_AddGroupMemberBroc : public PCS_AddGroupMemberRes
		{
			enum {uri = (140 << 8 | protocol::GINFO_SVID)};
		};

		struct PCS_QuitGroupOrFolder : public CGInfoBaseMsg
		{
			enum {uri = (41 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uFldID;

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseMsg::marshal(p);
				p << m_uFldID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseMsg::unmarshal(p);
				p >> m_uFldID;;
			}

		};

		struct PCS_QuitGroupOrFolderRes : public CGInfoBaseResMsg
		{
			enum {uri = (42 << 8 | protocol::GINFO_SVID)};

			bool m_bBecomeIndep; //Do not belong to any folder after deletion

			PCS_QuitGroupOrFolderRes() {};

			PCS_QuitGroupOrFolderRes(uint32_t uGrpID, uint32_t uFldID, uint32_t uReqUID,
				uint32_t uRC=RES_SUCCESS) :
				CGInfoBaseResMsg(uGrpID, uFldID, uReqUID, uRC)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseResMsg::marshal(p);
				p << m_bBecomeIndep;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseResMsg::unmarshal(p);
				p >> m_bBecomeIndep;
			}
		};

		struct PCS_QuitGroupOrFolderBroc : public PCS_QuitGroupOrFolderRes
		{
			enum {uri = (142 << 8 | protocol::GINFO_SVID)};
		};

		struct PCS_CopyGrpFolderMembers : public CGInfoBaseMsg
		{
			enum {uri = (45 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uFromFldID;
			uint32_t m_uToFldID;
			std::set<uint32_t> m_setUIDs;

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseMsg::marshal(p);
				p <<  m_uFromFldID << m_uToFldID;
				sox::marshal_container(p, m_setUIDs);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseMsg::unmarshal(p);
				p >>  m_uFromFldID >> m_uToFldID;
				sox::unmarshal_container(p, std::inserter(m_setUIDs, m_setUIDs.begin()));
			}

		};

		struct PCS_CopyGrpFolderMembersRes : public PCS_CopyGrpFolderMembers
		{
			enum {uri = (46 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uResCode;
			uint32_t m_uReqUID;

			PCS_CopyGrpFolderMembersRes(PCS_CopyGrpFolderMembers& obj) :
				PCS_CopyGrpFolderMembers(obj), m_uResCode (RES_SUCCESS), m_uReqUID(0)
			{};

			PCS_CopyGrpFolderMembersRes() :
				PCS_CopyGrpFolderMembers(), m_uResCode (RES_SUCCESS), m_uReqUID(0)
			{};


			virtual void marshal(sox::Pack &p) const
			{
				PCS_CopyGrpFolderMembers::marshal(p);
				p <<  m_uResCode << m_uReqUID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_CopyGrpFolderMembers::unmarshal(p);
				p >>  m_uResCode >> m_uReqUID;
			}

		};

		struct PCS_CopyGrpFolderMembersBroc : public PCS_CopyGrpFolderMembersRes
		{
			enum {uri = (146 << 8 | protocol::GINFO_SVID)};
		};

		struct PCS_MoveGrpFolderMembers : public PCS_CopyGrpFolderMembers
		{
			enum {uri = (47 << 8 | protocol::GINFO_SVID)};
		};

		struct PCS_MoveGrpFolderMembersRes : public PCS_CopyGrpFolderMembersRes
		{
			enum {uri = (48 << 8 | protocol::GINFO_SVID)};

			PCS_MoveGrpFolderMembersRes(PCS_MoveGrpFolderMembers& obj) :
				PCS_CopyGrpFolderMembersRes(obj)
			{};

			PCS_MoveGrpFolderMembersRes() :
				PCS_CopyGrpFolderMembersRes()
			{};

		};

		struct PCS_MoveGrpFolderMembersBroc : public PCS_MoveGrpFolderMembersRes
		{
			enum {uri = (148 << 8 | protocol::GINFO_SVID)};
		};

		struct PCS_KickUserOutOfGrpOrFld2 : public CGInfoBaseMsg
		{
			enum {uri = (49 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uFldID;
			std::vector<uint32_t> m_vecUIDs;

			PCS_KickUserOutOfGrpOrFld2() {};

			PCS_KickUserOutOfGrpOrFld2(uint32_t uGrpID, uint32_t uFldID) :
				m_uFldID(uFldID)
			{
				m_uGrpID = uGrpID;
			}

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseMsg::marshal(p);
				p << m_uFldID;
				sox::marshal_container(p, m_vecUIDs);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseMsg::unmarshal(p);
				p >> m_uFldID;
				sox::unmarshal_container(p, std::back_inserter(m_vecUIDs));
			}

		};

        //Justin fang -TODO:BUSYBOX-DISMISS
        struct PCS_KickUserOutOfGrpOrFld : public CGInfoBaseMsgExt
        {
            enum {uri = (1049 << 8 | protocol::GINFO_SVID)};

            uint32_t m_uFldID;
            std::vector<uint32_t> m_vecUIDs;
            std::string m_strExtMsg;

            PCS_KickUserOutOfGrpOrFld() {};

            PCS_KickUserOutOfGrpOrFld(uint32_t uGrpID, uint32_t uFldID) :
                m_uFldID(uFldID)
            {
                m_uGrpID = uGrpID;
            }

            virtual void marshal(sox::Pack &p) const
            {
                CGInfoBaseMsgExt::marshal(p);
                p << m_uFldID;
                sox::marshal_container(p, m_vecUIDs);
                p << m_strExtMsg;
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                CGInfoBaseMsgExt::unmarshal(p);
                p >> m_uFldID;
                sox::unmarshal_container(p, std::back_inserter(m_vecUIDs));
                p >> m_strExtMsg;
            }

        };

		struct PCS_KickUserOutOfGrpOrFldRes2 : public CGInfoBaseResMsg
		{
			enum {uri = (50 << 8 | protocol::GINFO_SVID)};

			//uint32_t m_uKickedUID;
			std::vector<uint32_t> m_vecKickedUIDs;
			std::vector<uint32_t> m_vecIndepentUIDs;
			std::map<uint32_t, uint32_t> m_mapNotKicked;//UID-Reason

			PCS_KickUserOutOfGrpOrFldRes2() {};

			PCS_KickUserOutOfGrpOrFldRes2(uint32_t uGrpID, uint32_t uFldID, uint32_t uReqUID,
				uint32_t uRC=RES_SUCCESS) :
				CGInfoBaseResMsg(uGrpID, uFldID, uReqUID, uRC)
				{}

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseResMsg::marshal(p);
				sox::marshal_container(p, m_vecKickedUIDs);
				sox::marshal_container(p, m_vecIndepentUIDs);
				sox::marshal_container(p, m_mapNotKicked);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseResMsg::unmarshal(p);
				sox::unmarshal_container(p, std::back_inserter(m_vecKickedUIDs));
				sox::unmarshal_container(p, std::back_inserter(m_vecIndepentUIDs));
				sox::unmarshal_container(p, std::inserter(m_mapNotKicked, m_mapNotKicked.begin()));
			}

		};

        //Justin fang -TODO:BUSYBOX-DISMISS
        struct PCS_KickUserOutOfGrpOrFldRes : public CGInfoBaseResMsgExt
        {
            enum {uri = (1050 << 8 | protocol::GINFO_SVID)};

            //uint32_t m_uKickedUID;
            std::vector<uint32_t> m_vecKickedUIDs;
            std::vector<uint32_t> m_vecIndepentUIDs;
            std::map<uint32_t, uint32_t> m_mapNotKicked;//UID-Reason
            std::string m_strExtMsg;

            PCS_KickUserOutOfGrpOrFldRes() {};

            PCS_KickUserOutOfGrpOrFldRes(uint32_t uGrpID, uint32_t uFldID, uint32_t uReqUID,
                uint32_t uRC=RES_SUCCESS) :
                CGInfoBaseResMsgExt(uGrpID, uFldID, uReqUID, uRC)
                {}

            virtual void marshal(sox::Pack &p) const
            {
                CGInfoBaseResMsgExt::marshal(p);
                sox::marshal_container(p, m_vecKickedUIDs);
                sox::marshal_container(p, m_vecIndepentUIDs);
                sox::marshal_container(p, m_mapNotKicked);
                p << m_strExtMsg;
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                CGInfoBaseResMsgExt::unmarshal(p);
                sox::unmarshal_container(p, std::back_inserter(m_vecKickedUIDs));
                sox::unmarshal_container(p, std::back_inserter(m_vecIndepentUIDs));
                sox::unmarshal_container(p, std::inserter(m_mapNotKicked, m_mapNotKicked.begin()));
                p >> m_strExtMsg;
            }

        };

		struct PCS_KickUserOutOfGrpOrFldBroc2 : public PCS_KickUserOutOfGrpOrFldRes2
		{
			enum {uri = (150 << 8 | protocol::GINFO_SVID)};
		};

        //Justin fang -TODO:BUSYBOX-DISMISS
        struct PCS_KickUserOutOfGrpOrFldBroc : public PCS_KickUserOutOfGrpOrFldRes
        {
            enum {uri = (1150 << 8 | protocol::GINFO_SVID)};
        };

		struct PCS_AddGrpOrFldAdmin : public CGInfoBaseMsg
		{
			enum {uri = (51 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uFldID;
			uint32_t m_uUID;

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseMsg::marshal(p);
				p << m_uFldID << m_uUID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseMsg::unmarshal(p);
				p >> m_uFldID >> m_uUID;
			}

		};

		struct PCS_AddGrpOrFldAdminRes : public CGInfoBaseResMsg
		{
			enum {uri = (52 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uNewAdminUID;

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseResMsg::marshal(p);
				p << m_uNewAdminUID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseResMsg::unmarshal(p);
				p >> m_uNewAdminUID;
			}

		};

		struct PCS_AddGrpOrFldAdminBroc : public PCS_AddGrpOrFldAdminRes
		{
			enum {uri = (152 << 8 | protocol::GINFO_SVID)};
		};

		struct PCS_RevokeGrpOrFldAdmin : public CGInfoBaseMsg
		{
			enum {uri = (53 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uFldID;
			uint32_t m_uUID;

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseMsg::marshal(p);
				p << m_uFldID << m_uUID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseMsg::unmarshal(p);
				p >> m_uFldID >> m_uUID;
			}

		};

		struct PCS_RevokeGrpOrFldAdminRes : public CGInfoBaseResMsg
		{
			enum {uri = (54 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uAdminUID;

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseResMsg::marshal(p);
				p << m_uAdminUID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseResMsg::unmarshal(p);
				p >> m_uAdminUID;
			}
		};

		struct PCS_RevokeGrpOrFldAdminBroc : public PCS_RevokeGrpOrFldAdminRes
		{
			enum {uri = (154 << 8 | protocol::GINFO_SVID)};
		};

		// Ask to join a folder
		struct PCS_JoinFolder :public PCS_JoinGroup
		{
			enum {uri = (55 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uFldID;

			virtual void marshal(sox::Pack &p) const
			{
				PCS_JoinGroup::marshal(p);
				p << m_uFldID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_JoinGroup::unmarshal(p);
				p >> m_uFldID;
			}
		};

		struct PCS_JoinFolderRes : public CGInfoBaseResMsg
		{
			enum {uri = (56 << 8 | protocol::GINFO_SVID)};
		};

		struct PCS_AddFolderMember : public PCS_AddGroupMember
		{
			enum {uri = (57 << 8 | protocol::GINFO_SVID)};

			// UID of the new member
			uint32_t m_uFldID;
			EAddToGrpOrFolderType m_enType;

			virtual void marshal(sox::Pack &p) const
			{
				PCS_AddGroupMember::marshal(p);
				p << m_uFldID;
				p.push_uint8(m_enType);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_AddGroupMember::unmarshal(p);
				p >> m_uFldID;
				m_enType= (EAddToGrpOrFolderType)p.pop_uint8();
			}
		};

		struct PCS_AddFolderMemberRes : public PCS_AddGroupMemberRes
		{
			enum {uri = (57 << 8 | protocol::GINFO_SVID)};

			PCS_AddFolderMemberRes()
			{
				m_bFromDefaultFolder = false;
			}

			PCS_AddFolderMemberRes(uint32_t uGrpID, uint32_t uFldID, uint32_t uReqUID,
				uint32_t uNewUID, EAddToGrpOrFolderType enType, uint32_t uRC = RES_SUCCESS) :
				PCS_AddGroupMemberRes(uGrpID, uReqUID, uNewUID, uRC)
			{
				m_uFldID = uFldID;
				m_bFromDefaultFolder = false;
				m_enType = enType;
			}

			bool m_bFromDefaultFolder;
			EAddToGrpOrFolderType m_enType;

			virtual void marshal(sox::Pack &p) const
			{
				PCS_AddGroupMemberRes::marshal(p);
				p << m_bFromDefaultFolder;
				p.push_uint8(m_enType);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_AddGroupMemberRes::unmarshal(p);
				p >> m_bFromDefaultFolder;
				m_enType= (EAddToGrpOrFolderType)p.pop_uint8();
			}

		};

		struct PCS_AddFolderMemberBroc : public PCS_AddFolderMemberRes
		{
			enum {uri = (156 << 8 | protocol::GINFO_SVID)};
		};

		struct PCS_ImportFromChannel : public CGInfoBaseMsgExt
		{
			enum {uri = (58 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uChannelID;
			uint32_t m_uChannelShortID;
			std::set<uint32_t> m_setRole;

			PCS_ImportFromChannel() : m_uChannelID(0)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseMsgExt::marshal(p);
				p << m_uChannelID << m_uChannelShortID;
				sox::marshal_container(p, m_setRole);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseMsgExt::unmarshal(p);
				p >> m_uChannelID >> m_uChannelShortID;
				sox::unmarshal_container(p, std::inserter(m_setRole,
					m_setRole.begin()));
			}
		};

		struct PCS_ImportFromChannelRes : public CGInfoBaseResMsgExt
		{
			enum {uri = (59 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uTotal;
			uint32_t m_uSuccess;
			uint32_t m_uChannelShortID;

			FldUIDMapType m_mapFldUids; //UID list for each fid

			std::set<uint32_t> m_setFromDefault; //Users whom are moved from the default folder
			std::set<uint32_t> m_setNewToGroup;  //Users whom are added to the group

			PCS_ImportFromChannelRes()
			{
				m_uTotal = m_uSuccess = 0;
			}

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseResMsgExt::marshal(p);
				p << m_uTotal << m_uSuccess << m_uChannelShortID;
				sox::marshal_container(p, m_mapFldUids);
				sox::marshal_container(p, m_setFromDefault);
				sox::marshal_container(p, m_setNewToGroup);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseResMsgExt::unmarshal(p);
				p >> m_uTotal >> m_uSuccess >> m_uChannelShortID;
				sox::unmarshal_container(p, std::inserter(m_mapFldUids,
					m_mapFldUids.begin()));
				sox::unmarshal_container(p, std::inserter(m_setFromDefault,
					m_setFromDefault.begin()));
				sox::unmarshal_container(p, std::inserter(m_setNewToGroup,
					m_setNewToGroup.begin()));
			}
		};

		struct PCS_ImportFromChannelBroc : public PCS_ImportFromChannelRes
		{
			enum {uri = (158 << 8 | protocol::GINFO_SVID)};
		};

		struct PCS_ImportFromChannelBrocEx : public PCS_ImportFromChannelRes
		{
			enum {uri = (159 << 8 | protocol::GINFO_SVID)};
		};

		struct PCS_NewGroup : public protocol::gprops::IChannelIDBase
		{
			enum {uri = (3 << 8 | protocol::GROUPID_SVID)};

			protocol::gprops::CMapGroupPropsInfo m_mapProps;
			protocol::gprops::GGroupPropsMask m_mask;

			PCS_NewGroup()
			{
				m_mask.m_uMask = 0;
			}

			virtual void marshal(sox::Pack &p) const
			{
				m_mapProps.marshal(p);
				p << m_mask.m_uMask;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				m_mapProps.unmarshal(p);
				p >> m_mask.m_uMask;
			}

			virtual void ReplaceChIDs (const std::map<uint32_t, uint32_t>& mapChIDs, std::vector<uint32_t>& vecNonExist, bool IsUpdate = false)
			{
				m_mapProps.ReplaceChIDs(mapChIDs, vecNonExist, IsUpdate);
			}
		};


		struct PCS_NewGroupWithActCode : public PCS_NewGroup
		{
			enum {uri = (4 << 8 | protocol::GROUPID_SVID)};
			std::string m_strActCode;

			virtual void marshal(sox::Pack &p) const
			{
				PCS_NewGroup::marshal(p);
				p << m_strActCode;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_NewGroup::unmarshal(p);
				p >> m_strActCode;
			}

		};


		struct PCS_NewGroupRes : public PCS_NewGroup
		{
			enum {uri = (1002 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uGrpID;
			uint32_t m_uAliasID; //External GID
			uint32_t m_uResCode;
			uint32_t m_uOwnerID;

			PCS_NewGroupRes(PCS_NewGroup& obj) :
			PCS_NewGroup(obj), m_uGrpID(0), m_uAliasID(0), m_uResCode(RES_SUCCESS), m_uOwnerID(0)
			{}

			PCS_NewGroupRes() :
			PCS_NewGroup(), m_uGrpID(0), m_uAliasID(0), m_uResCode(RES_SUCCESS), m_uOwnerID(0)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID << m_uAliasID << m_uResCode << m_uOwnerID;
				PCS_NewGroup::marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				//CGInfoBaseMsg::unmarshal(p);
				//p >> m_uResCode;
				p >> m_uGrpID >> m_uAliasID >> m_uResCode >> m_uOwnerID;
				PCS_NewGroup::unmarshal(p);
			}
		};

		//struct PCS_NewGroupFolder : public CGInfoBaseMsg, public CPrimitiveGroup

		struct PCS_NewGroupFolder : public protocol::gprops::IChannelIDBase
		{
			enum {uri = (1003 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uGrpID;
			protocol::gprops::CMapFolderPropsInfo m_mapProps;
			protocol::gprops::GFolderPropsMask m_mask;

			virtual void marshal(sox::Pack &p) const
			{
				m_mapProps.marshal(p);
				p << m_mask.m_uMask;
				p << m_uGrpID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				m_mapProps.unmarshal(p);
				p >> m_mask.m_uMask;
				p >> m_uGrpID;
			}

			virtual void ReplaceChIDs (const std::map<uint32_t, uint32_t>& mapChIDs, std::vector<uint32_t>& vecNonExist, bool IsUpdate = false)
			{
				m_mapProps.ReplaceChIDs(mapChIDs, vecNonExist, IsUpdate);
			}
		};


		struct PCS_NewGroupFolderRes : public PCS_NewGroupFolder
		{
			enum {uri = (1004 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uGrpID; //Opps, this is redundant
			uint32_t m_uFldID;
			uint32_t m_uReqUID; // UID of the operation requester`
			uint32_t m_uResCode;

			PCS_NewGroupFolderRes(PCS_NewGroupFolder& obj) :
			PCS_NewGroupFolder(obj)
			{InitBaseResVal();}

			PCS_NewGroupFolderRes() :
			PCS_NewGroupFolder()
			{InitBaseResVal();}

			//The creator is added to the new folder automatically. This flags whether
			// the creator is removed from the default folder.
			bool m_bDelFromDefaultFolder;

			virtual void InitBaseResVal()
			{
				m_uGrpID = 0;
				m_uFldID = 0;
				m_uReqUID = 0;
				m_uResCode = RES_SUCCESS;
				m_bDelFromDefaultFolder = false;
			}

			virtual void marshal(sox::Pack &p) const
			{
				PCS_NewGroupFolder::marshal(p);
				p << m_uGrpID << m_uFldID << m_uReqUID << m_uResCode;
				p << m_bDelFromDefaultFolder;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_NewGroupFolder::unmarshal(p);
				p >> m_uGrpID >> m_uFldID >> m_uReqUID >> m_uResCode;
				p >> m_bDelFromDefaultFolder;
			}
		};


		struct PCS_NewGroupFolderBroc : public PCS_NewGroupFolderRes
		{
			enum {uri = (105 << 8 | protocol::GINFO_SVID)};
		};

		///////////////////////////////
		struct PCS_DelGroup2 : public CGInfoBaseMsg
		{
			enum {uri = (5 << 8 | protocol::GINFO_SVID)};
		};

		struct PCS_DelGroupRes2 : public CGInfoBaseResMsg
		{
			enum {uri = (6 << 8 | protocol::GINFO_SVID)};
		};

		struct PCS_DelGroupBroc2 : public PCS_DelGroupRes2
		{
			enum {uri = (106 << 8 | protocol::GINFO_SVID)};
		};

		struct PCS_DelGroupFolder2 : public CGInfoBaseMsg
		{
			enum {uri = (7 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uFldID;

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseMsg::marshal(p);
				p << m_uFldID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseMsg::unmarshal(p);
				p >> m_uFldID;
			}

		};

		struct PCS_DelGroupFolderRes2 : public CGInfoBaseResMsg
		{
			enum {uri = (8 << 8 | protocol::GINFO_SVID)};

			// Users who become independent in the group
			std::set<uint32_t> m_setIndepUIDs;

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseResMsg::marshal(p);
				sox::marshal_container(p, m_setIndepUIDs);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseResMsg::unmarshal(p);
				sox::unmarshal_container(p, std::inserter(m_setIndepUIDs, m_setIndepUIDs.begin()));
			}

		};

		struct PCS_DelGroupFolderBroc2 : public PCS_DelGroupFolderRes2
		{
			enum {uri = (108 << 8 | protocol::GINFO_SVID)};
		};
		///////////////////////////////

		struct PCS_DelGroup : public CGInfoBaseMsgExt
		{
			enum {uri = (1005 << 8 | protocol::GINFO_SVID)};
			std::string m_strExtMsg;

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseMsgExt::marshal(p);
				p << m_strExtMsg;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseMsgExt::unmarshal(p);
				p >> m_strExtMsg;
			}

		};

		struct PCS_DelGroupRes : public CGInfoBaseResMsgExt
		{
			enum {uri = (1006 << 8 | protocol::GINFO_SVID)};
			std::string m_strExtMsg;

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseResMsgExt::marshal(p);
				p << m_strExtMsg;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseResMsgExt::unmarshal(p);
				p >> m_strExtMsg;
			}

		};

		struct PCS_DelGroupBroc : public PCS_DelGroupRes
		{
			enum {uri = (1106 << 8 | protocol::GINFO_SVID)};
		};

		struct PCS_DelGroupFolder : public CGInfoBaseMsgExt
		{
			enum {uri = (1007 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uFldID;
			std::string m_strExtMsg;

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseMsgExt::marshal(p);
				p << m_uFldID << m_strExtMsg;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseMsgExt::unmarshal(p);
				p >> m_uFldID >> m_strExtMsg;
			}

		};

		struct PCS_DelGroupFolderRes : public CGInfoBaseResMsgExt
		{
			enum {uri = (1008 << 8 | protocol::GINFO_SVID)};

			// Users who become independent in the group
			std::set<uint32_t> m_setIndepUIDs;
			std::set<uint32_t> m_setUIDs;
			std::string m_strExtMsg;

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseResMsgExt::marshal(p);
				sox::marshal_container(p, m_setIndepUIDs);
				p << m_strExtMsg;
				sox::marshal_container(p, m_setUIDs);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseResMsgExt::unmarshal(p);
				sox::unmarshal_container(p, std::inserter(m_setIndepUIDs, m_setIndepUIDs.begin()));
				p >> m_strExtMsg;
				sox::unmarshal_container(p, std::inserter(m_setUIDs, m_setUIDs.begin()));
			}

		};

		struct PCS_DelGroupFolderBroc : public PCS_DelGroupFolderRes
		{
			enum {uri = (1108 << 8 | protocol::GINFO_SVID)};
		};


		struct PCS_RejectRequest : public CGInfoBaseMsg
		{
			enum {uri = (10 << 8 | protocol::GINFO_SVID)};

			PCS_RejectRequest() :
				m_uFldID(0), m_uResponserUID(0)
			{}

			uint32_t m_uFldID;
			ERejectType m_enType; //not used
			uint32_t m_uRequesterUID; // the original requester
			uint32_t m_uResponserUID; //sender of the rejection...filled by ginfo
			std::string m_strReason;

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseMsg::marshal(p);
				p << m_uFldID;
				p.push_uint8(m_enType);
				p << m_uRequesterUID << m_uResponserUID << m_strReason;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseMsg::unmarshal(p);
				p >> m_uFldID;
				m_enType = (ERejectType)p.pop_uint8();
				p >> m_uRequesterUID >> m_uResponserUID >> m_strReason;
			}

		};

//		struct PCS_RejectRequestRes : public CGInfoBaseResMsg
//		{
//			enum {uri = (11 << 8 | protocol::GINFO_SVID)};
//
//			uint32_t m_uReqesterUID;
//			// uReqUID is the responser's UID
//
//			virtual void marshal(sox::Pack &p) const
//			{
//				CGInfoBaseResMsg::marshal(p);
//				p << m_uReqesterUID;
//			}
//
//			virtual void unmarshal(const sox::Unpack &p)
//			{
//				CGInfoBaseResMsg::unmarshal(p);
//				p >> m_uReqesterUID;
//			}
//		};

		struct PCS_GenerateInvitationCode : public sox::Marshallable
		{
			enum {uri = (11 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uGrpID; //GID to join
			uint32_t m_uFldID; //m_uFldID==uGrpID for group, otherwise for folder
			uint32_t m_uExpiry; //Expiry time in minutes
			uint32_t m_uCount; //How many times it can be used

			PCS_GenerateInvitationCode() {}

			PCS_GenerateInvitationCode(uint32_t uGrpID, uint32_t uFldID, uint32_t uExp, uint32_t uCnt):
				m_uGrpID(uGrpID), m_uFldID(uFldID), m_uExpiry(uExp), m_uCount(uCnt) {}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID << m_uFldID << m_uExpiry << m_uCount;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID >> m_uFldID >> m_uExpiry >> m_uCount;
			}
		};

		struct PCS_GenerateInvitationCodeRes : public PCS_GenerateInvitationCode
		{
			enum {uri = (11 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uResCode;
			uint32_t m_uChecksum;
			uint32_t m_uCodeID;

			PCS_GenerateInvitationCodeRes() {}

			PCS_GenerateInvitationCodeRes(PCS_GenerateInvitationCode& obj):
				PCS_GenerateInvitationCode(obj), m_uResCode(RES_SUCCESS), m_uChecksum(0), m_uCodeID(0) {}

			virtual void marshal(sox::Pack &p) const
			{
				PCS_GenerateInvitationCode::marshal(p);
				p << m_uResCode << m_uChecksum << m_uCodeID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_GenerateInvitationCode::unmarshal(p);
				p >>  m_uResCode >> m_uChecksum >> m_uCodeID;
			}
		};

		struct PCS_NotInGroup : public sox::Marshallable
		{
			enum {uri = (1 << 8 | protocol::GINFO_SVID)};

			std::vector<uint32_t> m_vecGrp;

			PCS_NotInGroup(){}
			PCS_NotInGroup(uint32_t uGrpID) : m_vecGrp(1,uGrpID)
				{}

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_vecGrp);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::back_inserter(m_vecGrp));
			}

		};

		// for testing
		struct PCS_TestSignal : public virtual sox::Marshallable
		{
			enum {uri = (255 << 8 | protocol::GINFO_SVID)};

			virtual void marshal(sox::Pack &p) const
			{
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
			}
		};

        /*The message for UI requests Joined FID List and Admin FID List.*/
        struct PCS_GetJoinedAdminFlds : public sox::Marshallable
        {
            enum {uri = (201 << 8 | protocol::GINFO_SVID)};

            std::set<uint32_t> m_setReqGids; //the set of GIDs requested for

            virtual void marshal(sox::Pack &p) const
            {
                sox::marshal_container(p, m_setReqGids);
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                sox::unmarshal_container(p, std::inserter(m_setReqGids, m_setReqGids.begin()));
            }
        };

        /**
         * This message is used for:
         * 1 - GListCache Responds the Joined-Admin FID Lists to UI.
         * 2 - GInfo Repsonds the Joined-Admin FID Lists to UI.
         **/
        struct PCS_GetJoinedAdminFldsRes : public sox::Marshallable
        {
            enum {uri = (202 << 8 | protocol::GINFO_SVID)};

            typedef std::map<uint32_t, std::set<uint32_t> > mapList;
            typedef std::map<uint32_t, std::set<uint32_t> >::const_iterator citList;
			std::set<uint32_t> m_setGhostGids;

            //Members List.
            uint32_t m_uResCode;
            mapList m_mapJoinedFldsList;//map<GID, set<FIDs> >
            mapList m_mapAdminFldsList;//map<GID, set<FIDs> >

            //Marshal & Unmarshal
            virtual void marshal(sox::Pack &p) const
            {
                p<<m_uResCode;
                sox::marshal_container(p, m_mapJoinedFldsList);
                sox::marshal_container(p, m_mapAdminFldsList);
                sox::marshal_container(p, m_setGhostGids);
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                p>>m_uResCode;
                sox::unmarshal_container(p, std::inserter(m_mapJoinedFldsList, m_mapJoinedFldsList.begin()));
                sox::unmarshal_container(p, std::inserter(m_mapAdminFldsList, m_mapAdminFldsList.begin()));
                sox::unmarshal_container(p, std::inserter(m_setGhostGids, m_setGhostGids.begin()));
            }
        };

		/*The message for UI requests group member sum*/
		struct PCS_GetGMemberSum : public sox::Marshallable
		{
			enum {uri = (203 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uGrpID;
			uint32_t m_uUid;
			uint32_t m_uImlinkdId;

			PCS_GetGMemberSum()
			{
				m_uGrpID = m_uUid = m_uImlinkdId = 0;
			}

			//Marshal & Unmarshal
			virtual void marshal(sox::Pack &p) const
			{
				p<<m_uGrpID<<m_uUid<<m_uImlinkdId;

			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p>>m_uGrpID>>m_uUid>>m_uImlinkdId;
			}
		};

		/**
		* This message is used for:
		* 1 - GListCache Responds the group member sum to UI.
		**/
		struct PCS_GetGMemberSumRes : public sox::Marshallable
		{
			enum {uri = (204 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uGrpID;
			uint32_t m_uSum;
			uint32_t m_uResCode;

			//Marshal & Unmarshal
			virtual void marshal(sox::Pack &p) const
			{
				p<<m_uGrpID;
				p<<m_uSum;
				p<<m_uResCode;

			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p>>m_uGrpID;
				p>>m_uSum;
				p>>m_uResCode;
			}
		};

		//GRP_CHAT_BAN
		/*The msg to set the ban list.*/
		struct PCS_AddGrpOrFldChatBanList : public sox::Marshallable
		{
			enum {uri = (12 << 8 | protocol::GINFO_SVID)};

			//members list
			uint32_t 			m_uGid;
			uint32_t 			m_uFid;
			std::set<uint32_t>  m_setUids;
			E_BANREASON 		m_eBanReason;
			std::string 		m_strCustReason;
			uint32_t 			m_uReserve;

			//construct && deconstruct
			PCS_AddGrpOrFldChatBanList(void) :
				m_uGid(0),
				m_uFid(0),
				m_eBanReason(E_CUSTOM_REASON),
				m_strCustReason(""),
				m_uReserve(0)
				{
					//m_setUids.clear();
				}

			PCS_AddGrpOrFldChatBanList
			(
				const uint32_t uGid, const uint32_t uFid,
				const E_BANREASON eBanRsn, const std::string &strRsn
			) :
				m_uGid(uGid),
				m_uFid(uFid),
				m_eBanReason(eBanRsn),
				m_strCustReason(strRsn),
				m_uReserve(0)
			{
				//m_setUids.clear();
			}

			//marshal && unmarshal
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGid << m_uFid;
				sox::marshal_container(p, m_setUids);
				p << (uint32_t)m_eBanReason << m_strCustReason << m_uReserve;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				uint32_t uBanReason;

				p >> m_uGid >> m_uFid;
				sox::unmarshal_container(p, std::inserter(m_setUids, m_setUids.begin()));
				p >> uBanReason >> m_strCustReason >> m_uReserve;

				m_eBanReason = (E_BANREASON)uBanReason;
			}
		};

		struct PCS_AddGrpOrFldChatBanListRes : public CGInfoBaseResMsgExt
		{
			enum {uri = (13 << 8 | protocol::GINFO_SVID)};

			//members list
			std::set<uint32_t>  m_setSucUids;
			std::set<uint32_t> 	m_setFailUids;
			E_BANREASON 		m_eBanReason;
			std::string 		m_strCustReason;
			EUserRole 			m_eMyRole;//The Role of receiver, not the operator.
			uint32_t 			m_uReserve;

			//construct && deconstruct
			PCS_AddGrpOrFldChatBanListRes(void) :
				CGInfoBaseResMsgExt(0,0,0),
				m_eBanReason(E_SYS_REASON_1),
				m_strCustReason(""),
				m_eMyRole(ENUM_UNKNOWN_ROLE),
				m_uReserve(0)
			{
				//m_setSucUids.clear();
				//m_setFailUids.clear();
			}

			PCS_AddGrpOrFldChatBanListRes
			(
				const uint32_t uGid, const uint32_t uFid, const uint32_t uReqUid,
				const E_BANREASON eRsn, const std::string &strRsn, const EUserRole eRole
			) :
				CGInfoBaseResMsgExt(uGid,uFid,uReqUid),
				m_eBanReason(eRsn),
				m_strCustReason(strRsn),
				m_eMyRole(eRole),
				m_uReserve(0)
			{
				//m_setSucUids.clear();
				//m_setFailUids.clear();
			}

			//marshal && unmarshal
			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_setSucUids);
				sox::marshal_container(p, m_setFailUids);
				p << (uint32_t)m_eBanReason << m_strCustReason << (uint32_t)m_eMyRole << m_uReserve;
				CGInfoBaseResMsgExt::marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				uint32_t uMyRole;
				uint32_t uBanReason;

				sox::unmarshal_container(p, std::inserter(m_setSucUids, m_setSucUids.begin()));
				sox::unmarshal_container(p, std::inserter(m_setFailUids, m_setFailUids.begin()));
				p >> uBanReason >> m_strCustReason >> uMyRole >> m_uReserve;

				m_eBanReason = (E_BANREASON)uBanReason;
				m_eMyRole = (EUserRole)uMyRole;
				CGInfoBaseResMsgExt::unmarshal(p);
			}
		};

		struct PCS_AddGrpOrFldChatBanListBroc : public PCS_AddGrpOrFldChatBanListRes
		{
			enum {uri = (113<< 8 | protocol::GINFO_SVID)};
		};

		/*The msg to set the unban list.*/
		struct PCS_DelGrpOrFldChatBanList : public sox::Marshallable
		{
			enum {uri = (14 << 8 | protocol::GINFO_SVID)};

			//members list
			uint32_t 			m_uGid;
			uint32_t 			m_uFid;
			std::set<uint32_t> 	m_setUnbanUids;

			//construct && deconstruct
			PCS_DelGrpOrFldChatBanList(void) :
				m_uGid(0),
				m_uFid(0)
			{
				//m_setUnbanUids.clear();
			}

			PCS_DelGrpOrFldChatBanList(const uint32_t uGid, const uint32_t uFid, const std::set<uint32_t>  &setUids) :
				m_uGid(uGid),
				m_uFid(uFid),
				m_setUnbanUids(setUids)
			{}

			//marshal && unmarshal
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGid << m_uFid;
				sox::marshal_container(p, m_setUnbanUids);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGid >> m_uFid;
				sox::unmarshal_container(p, std::inserter(m_setUnbanUids, m_setUnbanUids.begin()));
			}
		};

		struct PCS_DelGrpOrFldChatBanListRes : public CGInfoBaseResMsgExt
		{
			enum {uri = (15 << 8 | protocol::GINFO_SVID)};

			//members list.
			//uint32_t 			m_uGid;
			//uint32_t 			m_uFid;
			//uint32_t 			m_uReqUid;
			std::set<uint32_t> 	m_setSucUids;
			std::set<uint32_t> 	m_setFailUids;
			EUserRole 			m_eMyRole;//The Role of receiver, not the operator.

			//construct && deconstruct
			PCS_DelGrpOrFldChatBanListRes(void) :
				//m_uGid(0),
				//m_uFid(0),
				//m_uReqUid(0),
				CGInfoBaseResMsgExt(0,0,0),
				m_eMyRole(ENUM_UNKNOWN_ROLE)
			{
				//m_setSucUids.clear();
				//m_setFailUids.clear();
			}

			PCS_DelGrpOrFldChatBanListRes(const uint32_t uGid, const uint32_t uFid, const uint32_t uReqUid) :
				//m_uGid(uGid),
				//m_uFid(uFid),
				//m_uReqUid(uReqUid),
				CGInfoBaseResMsgExt(uGid, uFid, uReqUid),
				m_eMyRole(ENUM_UNKNOWN_ROLE)
			{
				//m_setSucUids.clear();
				//m_setFailUids.clear();
			}

			//marshal && unmarshal
			virtual void marshal(sox::Pack &p) const
			{
				//p << m_uGid << m_uFid << m_uReqUid;
				sox::marshal_container(p, m_setSucUids);
				sox::marshal_container(p, m_setFailUids);
				p << (uint32_t)m_eMyRole;
				CGInfoBaseResMsgExt::marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				uint32_t uMyRole;

				//p >> m_uGid >> m_uFid >> m_uReqUid;
				sox::unmarshal_container(p, std::inserter(m_setSucUids, m_setSucUids.begin()));
				sox::unmarshal_container(p, std::inserter(m_setFailUids, m_setFailUids.begin()));
				p >> uMyRole;
				CGInfoBaseResMsgExt::unmarshal(p);

				m_eMyRole = (EUserRole)uMyRole;
			}
		};

		struct PCS_DelGrpOrFldChatBanListBroc : public PCS_DelGrpOrFldChatBanListRes
		{
			enum {uri = (115 << 8 | protocol::GINFO_SVID)};
		};

		/*The msg to get the ban list.*/
		struct PCS_GetGrpChatBanList : public sox::Marshallable
		{
			enum {uri = (16 << 8 | protocol::GINFO_SVID)};

			//members list.
			uint32_t 			m_uGid;
			std::set<uint32_t>  m_setFids;

			//construct && deconstruct
			PCS_GetGrpChatBanList(void) :m_uGid(0)
			{
				//m_setFids.clear();
			}

			PCS_GetGrpChatBanList(const uint32_t uGid, const std::set<uint32_t> &setFids) :
				m_uGid(uGid),
				m_setFids(setFids)
			{}

			//marshal && unmarshal
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGid;
				sox::marshal_container(p, m_setFids);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGid;
				sox::unmarshal_container(p, std::inserter(m_setFids, m_setFids.begin()));
			}
		};

		struct PCS_GetGrpChatBanListRes : public sox::Marshallable
		{
			enum {uri = (17 << 8 | protocol::GINFO_SVID)};

			//members list.
			uint32_t 		m_uGid;
			FldUIDMapType 	m_mapGFidUids;

			//construct && deconstruct
			PCS_GetGrpChatBanListRes(void) : m_uGid(0)
			{
				//m_mapGFidUids.clear();
			}

			PCS_GetGrpChatBanListRes(const uint32_t uGid) : m_uGid(uGid)
			{
				//m_mapGFidUids.clear();
			}

			PCS_GetGrpChatBanListRes(uint32_t uGid, const FldUIDMapType& mapGFidUids)
				: m_uGid(uGid), m_mapGFidUids(mapGFidUids){}

			//marshal && unmarshal
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGid;
				sox::marshal_container(p, m_mapGFidUids);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGid;
				sox::unmarshal_container(p, std::inserter(m_mapGFidUids, m_mapGFidUids.begin()));
			}
		};

		struct PCS_AddGrpReport : public sox::Marshallable
		{
			enum {uri = (1 << 8) | protocol::GAUTH_SVID};

			/*The update list: Add, delete, empty.*/
			uint32_t m_uGrpID;
			uint32_t m_uReqesterUID;
			uint32_t m_uReportUID;

			//marshal && unmarshal
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID << m_uReqesterUID << m_uReportUID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID >> m_uReqesterUID >> m_uReportUID;
			}
		};

////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
		struct PSS_GetSessionInfoBase : public sox::Marshallable
		{
			uint32_t m_uChannelID;
			uint32_t m_uUID;
			uint32_t m_uSvrID;
			uint32_t m_uAppURI;
			std::string m_strAppCtx;

			PSS_GetSessionInfoBase ()
			{
				m_uChannelID = 0;
				m_uUID = 0;
				m_uSvrID = 0;
				m_uAppURI = 0;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uChannelID << m_uUID << m_uSvrID << m_uAppURI << m_strAppCtx;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uChannelID >> m_uUID >> m_uSvrID >> m_uAppURI >> m_strAppCtx;
			}
		};

		struct PSS_GetChannelMembersByRole : public PSS_GetSessionInfoBase
		{
			enum {uri = (210 << 8 | protocol::SDB_SVID)};

			std::set<uint32_t> m_setRole;

			virtual void marshal(sox::Pack &p) const
			{
				PSS_GetSessionInfoBase::marshal(p);
				sox::marshal_container(p, m_setRole);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PSS_GetSessionInfoBase::unmarshal(p);
				sox::unmarshal_container(p, std::inserter(m_setRole,
					m_setRole.begin()));
			}
		};

		//需要将请求包原包返回，因此继承请求报文
		struct PSS_GetChannelMembersByRoleRes : public PSS_GetChannelMembersByRole
		{
			enum {uri = (211 << 8 | protocol::GINFO_SVID)};

			typedef std::map<uint32_t, std::set<uint32_t> > ChUserMapType;

			ChUserMapType m_mapUser;

			PSS_GetChannelMembersByRoleRes()
			{
			}

			PSS_GetChannelMembersByRoleRes(PSS_GetChannelMembersByRole& objOri):
			PSS_GetChannelMembersByRole(objOri)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				PSS_GetChannelMembersByRole::marshal(p);
				sox::marshal_container(p, m_mapUser);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PSS_GetChannelMembersByRole::unmarshal(p);
				sox::unmarshal_container(p, std::inserter(m_mapUser,
					m_mapUser.begin()));
			}

		};
#endif
////////////////////////////////////////////////////////////////////////////////////////////////

		/*The message for UI requests group member sum from DBD*/
		struct PCS_GetGMemberSumDBD : public sox::Marshallable
		{
			enum {uri = (212 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uPageNo; // filled by protocol, dbd returns as is.
			std::set<uint32_t> m_setGrpIds;

			PCS_GetGMemberSumDBD(){}

			PCS_GetGMemberSumDBD(uint32_t uPageNo, const std::set<uint32_t>& setGrpIds)
				: m_uPageNo(uPageNo), m_setGrpIds(setGrpIds){}

			//Marshal & Unmarshal
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uPageNo;
				sox::marshal_container(p, m_setGrpIds);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uPageNo;
				sox::unmarshal_container(p, std::inserter(m_setGrpIds, m_setGrpIds.begin()));
			}
		};

		/*The message from DBD to UI, response of group member sum*/
		struct PCS_GetGMemberSumDBDRes : public sox::Marshallable
		{
			enum {uri = (213 << 8 | protocol::GINFO_SVID)};

			uint32_t m_uPageNo; // filled by protocol, dbd returns as is.

			// key  : groupId
			// value: sum of group members
			std::map<uint32_t, uint32_t> m_mapMemberSum;

			PCS_GetGMemberSumDBDRes(){}

			PCS_GetGMemberSumDBDRes(const std::map<uint32_t, uint32_t>& mapMemberSum)
				: m_mapMemberSum(mapMemberSum){}

			//Marshal & Unmarshal
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uPageNo;
				sox::marshal_container(p, m_mapMemberSum);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uPageNo;
				sox::unmarshal_container(p, std::inserter(m_mapMemberSum, m_mapMemberSum.begin()));
			}
		};

	} //namespace ginfo{
} //namespace protocol{



#endif // PGINFO_H_INCLUDED
