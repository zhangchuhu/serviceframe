//文件描述:
//本文件为protocol与UI接口的一部分，为UI提供了GInfo的watcher和API接口
//IGInfo类为ginfo与UI的总体接口，UI通过此类设置watcher并通过protocol发送请求
//IGInfoRoleListWatcher类为role list变化的watcher，UI通过此类感知成员角色的变化。通过IGInfoRole类设置
//IGInfoListWatcher类为结构变化的watcher，UI通过此类感知分组/成员的列表变化。通过IGInfoLocalDataAccess类设置
//IGInfoGeneralWatcher类为请求的回应消息的watcher，主要用于感知某个请求的处理是否成功。通过IGInfo类设置
//修改历史:
//2009/12/30 LinYue Initial version
//2010/02/09 修改关于syncGrpFolderMemberLists的注释以及接口
//2010/02/20 将IDListT改为SetIDListT以表示它是一个set，将CGrpGroupInfo改为CGroupInfo，将GetFldInfoPtr的返回
//           类型改为CGrpMiniInfo

#ifndef IGINFO_H_INCLUDED
#define IGINFO_H_INCLUDED

#include "common/protocol/const.h"
#include "common/int_types.h"
#include "common/unistr.h"

//#include "protocol/common/protocol/pginfo.h"
#include "common/protocol/IGInfoWatchers.h" //Callbacks in UI to handle events from protocol
#include "common/protocol/IGInfoDataTypes.h"

namespace protocol
{
	namespace ginfo
	{

		// The watchers to be notified of role list events
		// And functions to check local role data.
		struct IGInfoRole :
			public virtual IGInfoRoleListWatcherSetterT
		{
			// Common function to check if an user owns the specified group
			// @param uUID: User ID. If uUID=0, self's UID will be used
			// @return True if the user owns the group. False if not.
			virtual bool CheckOwnership(uint32_t uGrpID, uint32_t uUID = 0) = 0;

			// Common function to check admin privilege
			// @param uUID: User ID. If uUID=0, self's UID will be used
			// @return True if the user is the group admin/OW. False if not.
			virtual bool CheckIsGrpAdmin(uint32_t uGrpID, uint32_t uUID = 0) = 0;

			// Common function to check admin privilege
			// @param uUID: User ID. If uUID=0, self's UID will be used
			// @return True if the user is a folder admin/group admin/OW. False if not.
			// Note that it still returns true if uUID is a group admin but not a folder admin
			virtual bool CheckIsFldOrGrpAdmin(uint32_t uGrpID, uint32_t uFldID, uint32_t uUID = 0) = 0;
			
			// Get the role of a member.
			// Examples:
			//  GetMemberRole(11,12,123) returns:
			//    ENUM_FLD_ADM if 123 is the folder admin of folder 12
			//    ENUM_GRP_ADM if 123 is the group admin of group 11
			//    ENUM_GRP_OW if 123 is the group OW of group 11
			//    ENUM_UNKNOWN_ROLE if 123 doesn't have a role, or the local role list is not ready
			virtual protocol::ginfo::EUserRole GetMemberRole(uint32_t uGrpID, uint32_t uFldID, uint32_t uUID = 0) = 0;
			
			// -1: uUID1 is less powerful than uUID2
			// 0: uUID1 is the same powerful as uUID2
			// 1: uUID1 is more powerful than uUID2
			virtual int32_t IsMorePowerful(uint32_t uGrpID, uint32_t uFldID, uint32_t uUID1, uint32_t uUID2) = 0;
			
			// Find the group OW
			virtual uint32_t FindGroupOW (uint32_t uGrpID) = 0;

		};

		// UI can invoke GetDataManager() to get an data manager to access local data
		struct IGInfoLocalDataAccess :
			public virtual IGInfoListWatcherSetterT
		{
			// Get group info including name, desc, setting...
			// Should be moved to GProperty
			//virtual const CGrpGroupInfo* GetGrpInfoPtr (uint32_t uGrpID) = 0;

			// Get folder info
			// Only parentID and isMember are available!
			virtual const CGrpMiniInfo* GetFldInfoPtr (uint32_t uFldID) = 0;

			// Get the folder list of a group
			virtual const SetIDListT* GetFolderListPtr (uint32_t uGrpID) = 0;

			// Get the role list of a group/folder
			// @param uFldID: If a folder ID is provided, folder admin list will be returned
			//                If a group ID is provided, group admin list which contains OW ID
			//                will be returned.
			virtual const UserRoleT* GetFolderRoleListPtr (uint32_t uFldID) = 0;

			// Get the member list of a folder
			// @param uFldID: If a folder ID is provided, folder member list will be returned
			//                If a group ID is provided, independent members will be returned 
			//virtual const HSetIDListT* GetFolderMemberListPtr (uint32_t uFldID) = 0;
			
			// Check whether myself is a member of a folder
			virtual bool IsFolderMember (uint32_t uFldID) = 0;
			
			// Get the number of groups owned by myself
			virtual int32_t CountOwnedGroups() = 0;

			virtual uint32_t GetFolderParent (uint32_t uFldID) = 0;
		};

		struct IGInfoBan ://GRP_CHAT_BAN
			public virtual IGInfoBanListWatcherSetterT
		{
			/*Check if the user banned in the specific group/folder.*/
			//virtual bool CheckIsBanInGrpFld(const uint32_t uGid, const uint32_t uFid, const uint32_t uUid = 0) = 0;
		};

		// This is the main interface of GInfo to UI.
		// Provide APIs to UI.
		struct IGInfo :
			public virtual IGInfoGenWatcherSetterT // Provide i/f for UI to set/revoke event callbacks
			//public virtual IGInfoRoleListWatcherSetterT,
		{
			virtual ~IGInfo(){}

			// Set the owner UID -- for testing only, should be removed from the i/f
			virtual void SetOwner(uint32_t uOwnerUID) = 0;

			// UI can watch role list changes and check role locally via the role manager
			virtual IGInfoRole* GetRoleManager() = 0;

			// UI can get local data such as the folder list/member list via the data manager
			virtual IGInfoLocalDataAccess* GetDataManager() = 0;

			// UI can get local data such as the folder list/member list via the data manager
			//virtual IGInfoBrocWatcherSetterT* GetBroadcastNotifier() = 0;
			
			///////////////////////////////////////////////////////////////////////////////////

			// Deprecated
			/*
			virtual void NewGroup(const UniString& strName, const UniString& strDesc, const UniString& strBulletin,
				const protocol::ginfo::EAuthMode enAuthMode, const uint16_t uCategory, const uint16_t uSubCategory,
				const uint32_t uLogoIndex, const UniString& strLogoURL,
				const bool bIsAdhocChatAllowed, const bool bIsPrivate) = 0;

			// Deprecated
			virtual void NewGroupFolder(uint32_t uGrpID,
				const UniString& strName, const UniString& strDesc, const UniString& strBulletin) = 0;
			*/

			// Create a new group. Group information is specified in objInfo.
			virtual void NewGroup(const CGroupInfo& objInfo) = 0;

			// Create a new group using the activation code. Group information is specified in objInfo.
			// If the activation code is invalid, the responded ResCode = RES_GINFO_BAD_ACT_CODE
			//virtual void NewGroupWithActCode(CGroupInfo& objInfo, uint32_t uActCode, uint8_t uCodeType) = 0;
			virtual void NewGroupWithActCode(const CGroupInfo& objInfo, const UniString& strActCode) = 0;
			
			// Create a new folder under the specified group. Folder information is specified in objInfo.
			virtual void NewGroupFolder(uint32_t uGrpID, const CGrpBaseInfo& objInfo) = 0;

			// Remove the specified folder. Members who do not belong to other folders will be moved
			// into the independent list.
			virtual void DelGroupFolder(uint32_t uGrpID, uint32_t uFldID) = 0;

			// Dismiss a group.
			virtual void DismissGroup(uint32_t uGrpID) = 0;

			// Retrieve info of multiple groups from the server. 
			// @Param objMask: The info types needed to be retrieved are specified in objMask.
			//virtual void GetGroupInfo(const std::vector<uint32_t>& vecGID, protocol::ginfo::GrpInfoMask objMask,
				//bool bIsFolder=false) = 0;

			// Deprecated, use the below version
			/*
			virtual void UpdateGroupOrFolderInfo(uint32_t uGrpID, uint32_t uFldID, UniString* strName,
				UniString* strDesc, UniString* strBulletin,
				protocol::ginfo::EAuthMode* enAuthMode, uint16_t* uCategory, uint16_t* uSubCategory,
				uint32_t* uLogoIndex, UniString* strLogoURL, bool* bIsAdhocChatAllowed, bool* bIsPrivate) = 0;

			// Update group info for a single group.
			virtual void UpdateGroupInfo(uint32_t uGrpID, CGrpGroupInfo& objInfo) = 0;

			// Update folder info for a single folder
			virtual void UpdateFolderInfo(uint32_t uGrpID, uint32_t uFldID, CGrpBaseInfo& objInfo) = 0;
			*/

			// Transfer the ownership of the group to another member
			virtual void TransferGroupOwner(uint32_t uGrpID, uint32_t uNewOwerUID) = 0;

			// Request to join a group
			virtual void JoinGroup(uint32_t uGrpID, const UniString& strAuthMsg) = 0;

			virtual void JoinGroupWithVerify(uint32_t uGrpID, const UniString& strAuthMsg, const uint32_t timestamp,const uint64_t uKey,const std::wstring m_strValue) = 0;

			virtual void GetVerifyCode(const uint64_t uFactor,const uint32_t timestamp,const uint64_t uKey,const std::wstring m_strValue)=0;

			// Copy a list of members from a folder to another one.
			// Cannot copy from the default folder whose FID == GID or vice versa
			virtual void CopyGrpFolderMembers(uint32_t uGrpID, uint32_t uFromFldID, uint32_t uToFldID,
				const std::set<uint32_t>& setUIDs) = 0;

			// Move a list of members from a folder to another one.
			// Can move from the default folder whose FID == GID, but CANNOT move to it.
			virtual void MoveGrpFolderMembers(uint32_t uGrpID, uint32_t uFromFldID, uint32_t uToFldID,
				const std::set<uint32_t>& setUIDs) = 0;
				
			// Get an invitation code which can be used to join a group directly
			virtual void GenerateInvitationCode(uint32_t uGrpID, uint32_t uFldID, uint32_t uExpiryInMinutes, 
				uint32_t uHeadcount) = 0;

			// Invite a user to a group/folder
			// Examples:
			//  - Group 11's admin invites user 1234 to join group 11:
			//    InviteUserToGroupOrFolder(11, 11, 1234, wstring(L"Please join our group!"), ENUM_INV_TO_GROUP);
			//  - Folder 110's admin invites user 1234 to join group 11:
			//    InviteUserToGroupOrFolder(11, 110, 1234, wstring(L"Please join our group!"), ENUM_INV_TO_GROUP);
			//  - Folder 110's admin invites user 1234 to join the folder. Note that 1234 must be a member of group 11.
			//    InviteUserToGroupOrFolder(11, 110, 1234, wstring(L"Please join our folder!"), ENUM_INV_TO_FOLDER);
			// Please put the group/folder name into strExtMsg
			virtual void InviteUserToGroupOrFolder(uint32_t uGrpID, uint32_t uFldID, uint32_t uInviteeUID,
				const UniString& strInvitationMsg, const UniString& strExtMsg,
				protocol::ginfo::EInvitationType enType) = 0;
			
			//Justin fang -TODO:BUSYBOX-INVITE
			//The UI API for inviting users to group or folder in batch.
			virtual void InviteUser2GrpOrFldInBatch
			(
				const uint32_t uGid, const uint32_t uFid, std::set<uint32_t> &setInviteeUids,
				const UniString &strWelcomeMsg, const UniString &strExtMsg, const EInvitationType eInvitionType
			) {}//= 0;

			// User accepts the invitation to a group/folder.
			// @Param uChecksum: The checksum is sent with the invitation. Just sent it back to the server
			//  for verification.
			// Examples:
			//  - User accepts admin 4321's invitation to join group 11:
			//    AcceptInvitation(11, 0, 4321, received_chk,  ENUM_INV_TO_GROUP);
			//  - Group member accepts admin 4321's invitation to join folder 110:
			//    AcceptInvitation(11, 110, 4321, received_chk,  ENUM_INV_TO_FOLDER);
			virtual void AcceptInvitation(uint32_t uGrpID, uint32_t uFldID, uint32_t uInviterUID, uint32_t uChecksum,
				uint32_t uInvCodeID, protocol::ginfo::EInvitationType enType) = 0;

			// Reject the invitation with a reason
			//virtual void RejectInvitation(uint32_t uGrpID, uint32_t uInviterUID, UniString& strRejectReason) = 0;
			// Example:
			//  - User rejects admin 4321's invitation to join group 11:
			//    RejectInvitation(11, 0, 4321, wstring(L"Here is the reject reason"),  ENUM_INV_TO_GROUP);
			//  - Group member rejects admin 4321's invitation to join folder 110:
			//    RejectInvitation(11, 110, 4321, wstring(L"Here is the reject reason"),  ENUM_INV_TO_FOLDER);
			virtual void RejectInvitation(uint32_t uGrpID, uint32_t uFldID, uint32_t uReqesterUID, 
				const UniString& strRejectReason, protocol::ginfo::EInvitationType enType) = 0;

			// Add a user as a group admin or folder admin. Note that if a folder admin becomes a group
			//  admin, it will be removed from all folder admin lists.
			virtual void AddGrpOrFldAdmin(uint32_t uGrpID, uint32_t uFldID, uint32_t uAdminUID) = 0;

			// Revoke a user's admin rights.
			virtual void RevokeGrpOrFldAdmin(uint32_t uGrpID, uint32_t uFldID, uint32_t uAdminUID) = 0;

			// Admin adds a user to a group
			// Use as an approval of user's join request.
			// Note that this is not used to add folder member
			virtual void ApproveJoinRequest (uint32_t uGrpID, uint32_t uUID) = 0;
			
			// Reject user's join request to a group (uFldID=N_VOID_ID) or a folder
			//virtual void RejectJoinRequest (uint32_t uGrpID, uint32_t uUID) = 0;
			virtual void RejectJoinRequest (uint32_t uGrpID, uint32_t uFldID, uint32_t uUID, 
				const UniString& strRejectReason) = 0;

			// Kick a member out of a group/folder
			virtual void KickGrpOrFldMember (uint32_t uGrpID, uint32_t uFldID, const std::vector<uint32_t>& vecUIDs) = 0;

			// User quits a group or folder
			virtual void QuitGrpOrFld (uint32_t uGrpID, uint32_t uFldID) = 0;

			// Request to join a folder
			virtual void JoinFolderRequest(uint32_t uGrpID, uint32_t uFldID,const UniString& strAuthMsg) = 0;	

			// Admin adds a user to a folder
			// Group admin/ow can add himself/herself to a folder without authorization
			// Example:
			//  Folder admin approve a user's join request: set uUID to the requester's UID, and enType to ENUM_ADDTO_FLD_APPROVED
			//  Group admin joins a folder: set uUID to his/her own UID, and enType to ENUM_ADDTO_FLD_DIRECT
			//  Admin moves a member into a folder: Set uUID to the member's UID, and enType = ENUM_ADDTO_FLD_MOVED
			virtual void AddUserToFolder (uint32_t uGrpID, uint32_t uFldID, uint32_t uUID, EAddToGrpOrFolderType enType) = 0;

			// Import members from a channel
			// Mask should be protocol::gprops::GImportChannelRoleMask
			virtual void ImportFromChannel(uint32_t uGrpID, uint32_t uChannelID, uint32_t uChannelShortID, uint16_t uRoleMask) = 0;

		public:
		
			// Get group folders with IsMember flags
			// bWithMyRole has no effect now. Use GetMyJoinedFolders+GetMyManagedFolders instead.
			virtual void GetGrpFolderLists (const std::vector<uint32_t>& vecGID, bool bWithMyRole=false) = 0;

			// Use to get the member list of a folder. uFldID=uGrpID to get the member list of the default folder.
			// Note that this will clear the local member list and the role list of the folder
			// If bGetGrpFullMember is set to true, full member list of the group will be returned, otherwise, 
			//  the the member list of the default folder will be returned.
			// Note that FID = GID for all members in the result of the full member list
			//Sample result:
			//m_pGInfo->GetGrpFolderMemberList(16, 16, true);
			//展开群16的全成员列表
			//[CGrpListTestRes::OnGInfoAdd] Triggered (Reason=1,FQID=16/16/11,FQID=16/16/12,FQID=16/16/13,
			// FQID=16/16/1112,FQID=16/16/112761,FQID=16/16/112762,FQID=16/16/65271659)
			//m_pGInfo->GetGrpFolderMemberList(16, 16, false);
			//展开群16的未分组成员列表
			//[CGrpListTestRes::OnGInfoAdd] Triggered (Reason=1,FQID=16/16/11,FQID=16/16/12,FQID=16/16/13,FQID=16/16/65271659)	
			virtual void GetGrpFolderMemberList (uint32_t uGrpID, uint32_t uFldID, bool bGetGrpFullMember = false) = 0;

			//Get the total number of members of a group
			virtual void GetGrpMemberSum (uint32_t uGrpID) = 0;
			
			// Use to get the role list of a folder/group. 
			// uFldID=uGrpID to get the group's admin/OW list (folders admin lists excluded)
			// Make sure this is called after GetGrpFolderMemberList! The role list can be clear by GetGrpFolderMemberList.
			virtual void GetGrpFolderRoleList (uint32_t uGrpID, uint32_t uFldID) = 0;

			// Get my joined folders when login
			//virtual void GetMyJoinedFolders (const std::vector<uint32_t>& vecGID) = 0;

			// Get folders managed by me when login
			// There will be no OnGInfoRoleAdd callbacks for managed folders since UI doesn't need them.
			//virtual void GetMyManagedFolders (const std::vector<uint32_t>& vecGID) = 0;			
			
			// Get my joined folders and folders managed by me when login
			// There will be no OnGInfoRoleAdd callbacks for managed folders since UI doesn't need them.
			virtual void GetMyJoinedAdminFlds (const std::vector<uint32_t>& vecGID) = 0;

			// ** Deprecated ** Use GetGrpFolderMemberList+GetGrpFolderRoleList instead
			// Sync local group member lists with server. Should be invoked only when a new GID is just added to 
			//  the local list to sync the member lists and role lists. To get the member list of a single folder,
			//  use UnfoldGrpFolderMemberList instead.			
			virtual void SyncGrpFolderMemberLists (uint32_t uGrpID) = 0;
			
			// Get the channels owned by me
			virtual void GetMyChannels() = 0;
			
#ifdef _DEBUG
			virtual void test() = 0;
#endif

			// This is just a wrapper of the above function
			//virtual void SyncMultiGrpFolderMemberLists (const std::vector<uint32_t>& vecGID) = 0;

			// Unfold a folder. The member list of the folder will be returned if members is allowed to
			//  open unjoined folders, or the user is a group admin/owner.
			//virtual void UnfoldGrpFolderMemberList (uint32_t uGrpID, uint32_t uFldID) = 0;
			
			
//  			virtual std::string EncodeInvitation(const InvitationCode& obj) = 0;
//  			
//  			virtual InvitationCode DecodeInvitation(const std::string& obj) = 0;
 			
			// Deprecated. Use RejectJoinRequest instead.
			//virtual void RejectUserToFolder (uint32_t uGrpID, uint32_t uFldID, uint32_t uUID) = 0;
					
			// For other servers. Check if the user is allowed to make changes to a group/folder.
			// For example, GMemberInfo/GTopic may need to check the permission before changing something. They
			//  can pass context data in strUserData and invoke this function to check the permission. strUserData
			//  will be echoed back in the response. On receipt of the response, the watcher can take subsequent
			//  actions according to the echoed strUserData. A cookie is also responded for further validation
			// of the authentication.
			//virtual void CheckSelfIsGrpOrFldAdmin(uint32_t uGrpID, uint32_t uFldID,
				//const std::string& strUserData) = 0;
				
			// Get folder list with IsMember indicator

			// Get member list with role

			//virtual const UniString& GetGrpName (uint32_t uGrpID) = 0;
			// For testing !!
			//virtual void OnGetGList(std::vector<uint32_t>& vecGIDs) = 0;

			//GRP_CHAT_BAN
			//UI can watch ban list changes and  check the ban state locally via the ban manager.
			virtual IGInfoBan *GetBanManager() = 0;

			//For UI to get the group ban list from server(gauth/ginfo).
			virtual void GetGrpFldBanList(const uint32_t uGid, const std::set<uint32_t> &setFids) = 0;

			//For UI to request to ban the users in the group/folder.
			virtual void AddGrpFldChatBanList(const uint32_t uGrpID, const uint32_t uFldID,const std::set<uint32_t> &setUIDs, 
					 const protocol::ginfo::E_BANREASON enType, const UniString &strCustReason) = 0;

			//For UI to request to unban the banned user in the group/folder.
			virtual void DelGrpFldBanList(const uint32_t uGid, const uint32_t uFid, const std::set<uint32_t> &setUids) = 0;

			virtual void AddGrpReport(const uint32_t uGrpID, const uint32_t uReqesterUID, const uint32_t uReportUID)=0;
			

		};

	}
}
#endif // IGINFO_H_INCLUDED
