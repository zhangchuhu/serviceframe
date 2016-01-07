//修改历史:
//2009/12/30 LinYue Initial version

#ifndef IGINFOWATCHERS_H_INCLUDED
#define IGINFOWATCHERS_H_INCLUDED

#include "common/protocol/IGInfoDataTypes.h"

namespace protocol
{
	namespace ginfo
	{
		// Notify UI watchers of group/folder/member list changes
		// Note that it won't be invoked if the request is from goffline. It's for online (real time) request only!
		// For example, if someone is removed from the group, online users can be notified via OnGInfoRemove, but offlined
		//  users won't received this when they go onlined.
		//Watched via:
		//IGInfoLocalDataAccess* pGData = pProtocol->getGInfo()->GetDataManager();
		//pGData->watch(this);
		struct IGInfoListWatcher
		{
			// A member is moved -- member list only
			// Deprecated!
// 			virtual void OnGInfoMoved(const uint32_t& uGrpID, const uint32_t& uFromFldID, const uint32_t& uToFldID,
// 				const std::vector<uint32_t>& vecUIDs) = 0;

			// This is triggered if a group/folder/user is added. Remember to check enReason for the adding reason.
			virtual void OnGInfoAdd(const FQIDVectorT& vecFQIDs, const EChangeReason& enReason) = 0;

			// This is triggered if a group/folder/user is removed. Remember to check enReason for the adding reason.
			virtual void OnGInfoRemove(const FQIDVectorT& vecFQIDs, const EChangeReason& enReason) = 0;
			
			// Forbid to access a folder. For example, if a user tries to unfold a folder that he/she is not in
			//  and he/she is not a group admin/OW, the user may get this event. It depends on the group settings.
			//virtual void OnGInfoAccessDenied(const FQIDVectorT& vecFQIDs) = 0;

			//virtual void OnGInfoGroupChange(const protocol::ginfo::GInfoGroupChangeNoticeVectorT& vecNotice) = 0;

			//virtual void OnGInfoFolderChange(const protocol::ginfo::GInfoFolderChangeNoticeVectorT& vecNotice) = 0;
		};

		//Watched via:
		//IGInfoRole* pGInfoRole = pProtocol->getGInfo()->GetRoleManager();
		//pGInfoRole->watch(this);
		struct IGInfoRoleListWatcher
		{
			virtual void OnGInfoRoleAdd(const FQIDRoleVectorT& vecFQIDs) = 0;

			virtual void OnGInfoRoleRemove(const FQIDVectorT& vecFQIDs) = 0;
		};

		// Callbacks in UI to handle operation results
		// Triggered only when requestorID=selfUID in the message
		// For example, OnAddGroupMemberRes is triggered only on the requester of the AddGroupMember operation
		//  to indicate whether the operation has failed or succeeded. IGInfoListWatcher::OnGInfoAdd is triggered
		//  on all members in the group to notify the UI of the change of the list if the operation has succeeded.
		struct IGInfoGeneralWatcher
		{
			// Notify AddGroupMember response - triggered only when requestorID=selfUID in the message
			// Used as ACK
			virtual void OnApproveJoinRequestRes(const uint32_t& uRes, const uint32_t& uGrpID,
				const uint32_t& uUID) = 0;

			virtual void OnKickGrpOrFldMemberRes(const uint32_t& uRes, const uint32_t& uGrpID, const uint32_t& uFldID,
				const std::vector<uint32_t>& vecKickedUIDs, const std::map<uint32_t, uint32_t>& mapNotKicked) = 0;

			// Notify QuitGroupOrFolder response
			virtual void OnQuitGroupOrFolderRes(const uint32_t& uRes, const uint32_t& uGrpID, const uint32_t& uFldID) = 0;

			//RES_GINFO_DB_ERROR
			//RES_GINFO_EXCEEDED
			//RES_GINFO_INVALID_CHANNEL
			virtual void OnNewGroupRes(const uint32_t& uRes, const uint32_t& uGrpID, 
				const CGroupInfo& objInfo) = 0;

			//RES_GINFO_NO_PERMISSION
			//RES_GINFO_DB_ERROR
			//RES_GINFO_EXCEEDED
			//RES_GINFO_NO_CHANGE
			//RES_GINFO_INVALID_CHANNEL
			virtual void OnNewGroupFolderRes(const uint32_t& uRes, const uint32_t& uGrpID, 
				const uint32_t& uFldID, const CGrpBaseInfo& objInfo) = 0;

			virtual void OnDelGroupFolderRes(const uint32_t& uRes, const uint32_t& uGrpID, const uint32_t& uFldID) = 0;

			virtual void OnDismissGroupRes(const uint32_t& uRes, const uint32_t& uGrpID,
				const SetIDListT& setFldList) = 0;

			//virtual void OnGetGroupInfoRes(const std::vector<uint32_t>& vecGrpID) = 0;

			// If m_bIsPrivate is changed to true, UI may need to retrieve the member list of the folder that the user is not in.
			//virtual void OnUpdateGroupInfoRes(const uint32_t& uRes, const uint32_t& uGrpID) = 0;

			virtual void OnTransferGroupOwnerRes(const uint32_t& uRes, const uint32_t& uGrpID,
				const uint32_t& uUID) = 0;

			// Response to the message that the user sent to request to join a group
			virtual void OnJoinGroupRes(const uint32_t& uRes, const uint32_t& uGrpID) = 0;

			// Admin/OW receives a user's join request
			virtual void OnJoinGroupRequest(const uint32_t& uGrpID, const uint32_t& uRequester, const UniString& strAuthMsg) = 0;

			virtual void OnCopyGrpFolderMembersRes(const uint32_t& uRes, const uint32_t& uGrpID,
				const uint32_t& uFromFldID, const uint32_t& uToFldID, const std::set<uint32_t>& setUIDs) = 0;

			virtual void OnMoveGrpFolderMembersRes(const uint32_t& uRes, const uint32_t& uGrpID,
				const uint32_t& uFromFldID, const uint32_t& uToFldID, const std::set<uint32_t>& setUIDs) = 0;

// 			virtual void OnGenerateInvitationCodeRes(const uint32_t& uRes, const uint32_t& uGrpID,
// 				const uint32_t& uFldID, const uint32_t& uExpiry, const uint32_t& uCount, const uint32_t& uInvCodeID,
// 				const uint32_t& uChecksum) = 0;

			// Inviter received an ack
			virtual void OnInviteUserToGroupRes(const uint32_t& uRes, const uint32_t& uGrpID, const uint32_t& uFldID,
				const uint32_t& uInviteeUID, const protocol::ginfo::EInvitationType& enType) = 0;

			//Justin fang -TODO:BUSYBOX-INVITE
			virtual void OnInviteUsers2GrpFldRes(const uint32_t &uGid, const uint32_t &uFid, 
				const std::map<uint32_t, uint32_t> &mapUidsRCode, const EInvitationType &eInvitionType) {}//= 0;
			
			// Invitee received an invitation
			virtual void OnInvitedToGroupRequest(const uint32_t& uGrpID, const uint32_t& uFldID,
				const uint32_t& uInviterUID, const UniString& strInvitation, const UniString& strExt,
				const uint32_t& uChecksum,
				const protocol::ginfo::EInvitationType& enType) = 0;

			// Response to the message that the user sent to accept the invitation
			// RES_GINFO_COOKIE_ERROR - Fake invitation
			// RES_GINFO_INVALID_FOLDER - The group/folder doesn't exist anymore
			// RES_GINFO_NO_CHANGE - Already in
			// RES_GINFO_DB_ERROR - Server error
			virtual void OnAcceptedInvitationToGroupRes(const uint32_t& uRes, const uint32_t& uGrpID, const uint32_t& uFldID,
				const uint32_t& uInviterUID, const protocol::ginfo::EInvitationType& enType) = 0;

			// Response to the message that the user sent to reject the invitation
//			virtual void OnRejectInvitationToGroupRes(const uint32_t& uRes, const uint32_t& uGrpID,
//				const uint32_t& uInviterUID) = 0;

			// Inviter receives the rejection from the invitee.
// 			virtual void OnRejectInvitationToGroupRequest(const uint32_t& uGrpID,
// 				const uint32_t& uInviteeUID, const UniString& strReason) = 0;

			virtual void OnRejectInvitationOrRequest(const uint32_t& uGrpID, const uint32_t& uFldID, const ERejectType& enType,
				const uint32_t& uResponserUID, const UniString& strReason) = 0;

			//RES_GINFO_NO_CHANGE NewAdmin is an admin/ow already
			//RES_GINFO_NO_PERMISSION Requester has no permission
			//RES_GINFO_EXCEEDED The admin list is full
			//RES_GINFO_DB_ERROR Server error
			virtual void OnAddGrpOrFldAdminRes(const uint32_t& uRes, const uint32_t& uGrpID, const uint32_t& uFldID,
				const uint32_t& uUID) = 0;

			virtual void OnRevokeGrpOrFldAdminRes(const uint32_t& uRes, const uint32_t& uGrpID, const uint32_t& uFldID,
				const uint32_t& uUID) = 0;

			virtual void OnGetGroupFolderListRes(const uint32_t& uRes, const std::vector<uint32_t>& vecGrpID,
				const protocol::ginfo::FQIDVectorT& vecJoinedFIDs) = 0;

			virtual void OnGetGrpMemberSumRes(const uint32_t& uRes, const uint32_t& uGrpID,const uint32_t& uSum) = 0;

			//std::map<uint32_t, uint32_t> mapCIDs 
			//key是长位ID, value是短位ID
			virtual void OnGetMyChannelsRes(const std::map<uint32_t, uint32_t>& mapCIDs) = 0;

			//RES_GINFO_NO_PERMISSION -- Not a owner of the group
			//RES_GINFO_DB_ERROR	
			//uTotal: Number of users to be imported
			//uSuccess: Number of actual imported users
			virtual void OnImportFromChannelRes(const uint32_t& uRes, const uint32_t& uGrpID, const uint32_t& uChannel, 
				const uint32_t& uTotal, const uint32_t& uSuccess) = 0;


			// Response to the message that the user sent to request to join a folder/group(uFldID = N_VOID_ID)	
			// Or Notify if the admin approved/rejected the request of joining a folder
			// Rejected: uRes = RES_GINFO_ADMIN_REJECT **deprecated** OnRejectInvitationOrRequest is invoked if the request is rejected
			// Approved: uRes = RES_SUCCESS
			// Not allowed to join: uRes = RES_GINFO_NO_PERMISSION
			// Waiting for approval: uRes = RES_GINFO_ON_GOING 
			virtual void OnJoinGroupOrFolderRes(const uint32_t& uRes, const uint32_t& uGrpID, const uint32_t& uFldID,
				const uint32_t& uAdmin, const protocol::ginfo::EAddToGrpOrFolderType& enAddType) = 0;

			/*搜索好友UI回调
			* 对应接口:Asny_searchBuddy2
			* @param uRes: 返回状态枚举
			* @param uGrpID:请求加入的gid
			* @param old_key: 旧的验证图片的Key,如果没有，则为空
			* @param valid_key: 验证码的Key,  当用户需要输入验证码时,valid_key为非空
			* @param bin: 验证码图片的首地址,当用户需要输入验证码时,bin为非空.
			* @param size: 验证码图片所占内存长度;当用户需要输入验证码时,size为非0
			*/
			virtual void OnJoinGroupWithVerifyRes(const uint32_t& uRes,const uint32_t& uGrpID,const uint32_t& timestamp,const uint64_t& old_key,
				const uint64_t& valid_key,  const char* const & bin, const uint32_t &size) = 0;

			virtual void OnVerifyCodeRes(const uint32_t& uRes,const uint64_t& ufactor,const uint32_t& timestamp,const uint64_t& old_key,
				const uint64_t& valid_key,  const char* const & bin, const uint32_t &size) = 0;

			// Folder admin receives a user's join request
			// Please send AddUserToFolder to approve the user's request
			virtual void OnJoinFolderRequest(const uint32_t& uGrpID, const uint32_t& uFldID, 
				const uint32_t& uRequester, const UniString& strAuthMsg) = 0;

			virtual void OnAddUserToFolderRes(const uint32_t& uRes, const uint32_t& uGrpID, const uint32_t& uFldID,
				const uint32_t& uUID) = 0;

			//GRP_CHAT_BAN
			/**---------------------------------------------------------------------------
			* Function	   : OnAddGrpFldChatBanListRes (pop)
			*
			* Description  : This routine to notify UI the result of adding ban list. 
			*
			* @param	   : uGid - group id.
			*				uFid - folder id. (uFid==uGid for group operation.)
			*				setSucUids - the list of success uids.
			*				setFailUids - the list of failed uids.
			*				
			* @return	   : N/A
			*---------------------------------------------------------------------------*/
			virtual void OnAddGrpFldChatBanListRes
			(
				const uint32_t &uGid, const uint32_t &uFid, 
				const std::set<uint32_t> &setSucUids, 
				const std::set<uint32_t> &setFailUids
			){}// = 0;

			void OnReportGrpMemberRes
			(
				const uint32_t& uRes,
				const std::wstring& old_key,
				const std::wstring& valid_key,  
				const char* const & bin, const uint32_t &size
			){}
			
			/**---------------------------------------------------------------------------
			* Function	   : OnDelGrpFldChatBanListRes (pop)
			*
			* Description  : This routine to notify UI the result of deleting ban list. 
			*
			* @param	   : uGid - group id.
			*				uFid - folder id. (uFid==uGid for group operation.)
			*				setSucUids - the list of success uids.
			*				setFailUids - the list of failed uids.
			*				
			* @return	   : N/A
			*---------------------------------------------------------------------------*/
			virtual void OnDelGrpFldChatBanListRes
			(
				const uint32_t &uGid, const uint32_t &uFid, 
				const std::set<uint32_t> &setSucUids, 
				const std::set<uint32_t> &setFailUids
			){}// = 0;

		};
		
/*
		struct GDummyGInfoGenWatcher : public IGInfoGeneralWatcher
		{
			virtual void OnApproveJoinRequestRes(const uint32_t& uRes, const uint32_t& uGrpID,
				const uint32_t& uUID){}

			virtual void OnKickGrpOrFldMemberRes(const uint32_t& uRes, const uint32_t& uGrpID, const uint32_t& uFldID,
				const std::vector<uint32_t>& vecKickedUIDs){}

			virtual void OnQuitGroupOrFolderRes(const uint32_t& uRes, const uint32_t& uGrpID, const uint32_t& uFldID){}

			virtual void OnNewGroupRes(const uint32_t& uRes, const uint32_t& uGrpID,
				const CGroupInfo& objInfo){}

			virtual void OnNewGroupFolderRes(const uint32_t& uRes, const uint32_t& uGrpID, 
				const uint32_t& uFldID, const CGrpBaseInfo& objInfo){}

			virtual void OnDelGroupFolderRes(const uint32_t& uRes, const uint32_t& uGrpID, const uint32_t& uFldID){}

			virtual void OnDismissGroupRes(const uint32_t& uRes, const uint32_t& uGrpID,
				const SetIDListT& setFldList){}

			virtual void OnGetGroupInfoRes(const std::vector<uint32_t>& vecGrpID){}

			virtual void OnUpdateGroupInfoRes(const uint32_t& uRes, const uint32_t& uGrpID){}

			virtual void OnTransferGroupOwnerRes(const uint32_t& uRes, const uint32_t& uGrpID,
				const uint32_t& uUID){}

			virtual void OnJoinGroupRes(const uint32_t& uRes, const uint32_t& uGrpID){}

			virtual void OnJoinGroupRequest(const uint32_t& uGrpID, const uint32_t& uRequester, const UniString& strAuthMsg){}

			virtual void OnCopyGrpFolderMembersRes(const uint32_t& uRes, const uint32_t& uGrpID,
				const uint32_t& uFromFldID, const uint32_t& uToFldID, const std::set<uint32_t>& setUIDs){}

			virtual void OnMoveGrpFolderMembersRes(const uint32_t& uRes, const uint32_t& uGrpID,
				const uint32_t& uFromFldID, const uint32_t& uToFldID, const std::set<uint32_t>& setUIDs){}

			virtual void OnInviteUserToGroupRes(const uint32_t& uRes, const uint32_t& uGrpID, const uint32_t& uFldID,
				const uint32_t& uInviteeUID){}

			virtual void OnInvitedToGroupRequest(const uint32_t& uGrpID, const uint32_t& uFldID,
				const uint32_t& uInviterUID, const UniString& strInvitation, const uint32_t& uChecksum){}

			virtual void OnAcceptedInvitationToGroupRes(const uint32_t& uRes, const uint32_t& uGrpID,
				const uint32_t& uInviterUID){}

			virtual void OnRejectInvitationToGroupRes(const uint32_t& uRes, const uint32_t& uGrpID,
				const uint32_t& uInviterUID){}
				
			virtual void OnRejectInvitationToGroupRequest(const uint32_t& uGrpID,
				const uint32_t& uInviteeUID, const UniString& strReason){}

			virtual void OnAddGrpOrFldAdminRes(const uint32_t& uRes, const uint32_t& uGrpID, const uint32_t& uFldID,
				const uint32_t& uUID){}

			virtual void OnRevokeGrpOrFldAdminRes(const uint32_t& uRes, const uint32_t& uGrpID, const uint32_t& uFldID,
				const uint32_t& uUID){}

			virtual void OnGetGroupFolderListRes(const uint32_t& uRes, const std::vector<uint32_t>& vecGrpID,
				const protocol::ginfo::FQIDVectorT& vecJoinedFIDs){}		
		};
*/


/*
		struct IGInfoBroadcastWatcher
		{
			virtual void OnApproveJoinRequestBroc(const uint32_t& uReqUID, const uint32_t& uGrpID,
				const uint32_t& uUID) = 0;

			virtual void OnKickGrpOrFldMemberBroc(const uint32_t& uReqUID, const uint32_t& uGrpID, const uint32_t& uFldID,
				const std::vector<uint32_t>& vecKickedUIDs) = 0;

			virtual void OnQuitGroupOrFolderBroc(const uint32_t& uReqUID, const uint32_t& uGrpID, const uint32_t& uFldID) = 0;

			virtual void OnNewGroupFolderBroc(const uint32_t& uReqUID, const uint32_t& uGrpID, 
				const uint32_t& uFldID, const CGrpBaseInfo& objInfo) = 0;

			virtual void OnDelGroupFolderBroc(const uint32_t& uReqUID, const uint32_t& uGrpID, 
				const uint32_t& uFldID) = 0;

			virtual void OnDismissGroupBroc(const uint32_t& uReqUID, const uint32_t& uGrpID) = 0;

			virtual void OnTransferGroupOwnerBroc(const uint32_t& uReqUID, const uint32_t& uGrpID,
				const uint32_t& uUID) = 0;

			virtual void OnJoinGroupBroc(const uint32_t& uReqUID, const uint32_t& uGrpID) = 0;

			virtual void OnCopyGrpFolderMembersBroc(const uint32_t& uReqUID, const uint32_t& uGrpID,
				const uint32_t& uFromFldID, const uint32_t& uToFldID, const std::set<uint32_t>& setUIDs) = 0;

			virtual void OnMoveGrpFolderMembersBroc(const uint32_t& uReqUID, const uint32_t& uGrpID,
				const uint32_t& uFromFldID, const uint32_t& uToFldID, const std::set<uint32_t>& setUIDs) = 0;

			virtual void OnAcceptedInvitationToGroupBroc(const uint32_t& uReqUID, const uint32_t& uGrpID,
				const uint32_t& uInviterUID) = 0;

			virtual void OnAddGrpOrFldAdminBroc(const uint32_t& uReqUID, const uint32_t& uGrpID, const uint32_t& uFldID,
				const uint32_t& uUID) = 0;

			virtual void OnRevokeGrpOrFldAdminBroc(const uint32_t& uReqUID, const uint32_t& uGrpID, const uint32_t& uFldID,
				const uint32_t& uUID) = 0;

		};
*/

		//GRP_CHAT_BAN
		//Watched via:
		//IGInfoBan* pGInfoBan = pProtocol->getGInfo()->GetBanManager();
		//pGInfoBan->watch(this);
		struct IGInfoGrpBanListWatcher
		{
			/**---------------------------------------------------------------------------
			* Function	   : OnAddGrpFldChatBanList (update)
			*
			* Description  : This routine to notify UI add the ban list of a specific grp/folder. 
			*
			* @param	   : uGid - group id.
			*				uFid - folder id. (uFid==uGid for group operation.)
			*				setBanUids - the list of ban uids.
			*				
			* @return	   : N/A
			*---------------------------------------------------------------------------*/
			virtual void OnAddGrpFldChatBanList
			(
	  			const uint32_t &uGid, const uint32_t &uFid, 
	  			const uint32_t &uReqUid, const std::set<uint32_t> &setBanUids
			){} //= 0;

			/**---------------------------------------------------------------------------
			* Function	   : OnDelGrpFldChatBanList (update)
			*
			* Description  : This routine to notify UI delete the ban list of a specific grp/folder. 
			*
			* @param	   : uGid - group id.
			*				uFid - folder id. (uFid==uGid for group operation.)
			*				setBanUids - the list of ban uids.
			*				
			* @return	   : N/A
			*---------------------------------------------------------------------------*/
			virtual void OnDelGrpFldChatBanList
			(
				const uint32_t &uGid, const uint32_t &uFid, 
				const uint32_t &uReqUid, const std::set<uint32_t> &setUnbanUids
			){} //= 0;

			/**---------------------------------------------------------------------------
			* Function	   : OnGetGrpFldChatBanListRes (res)
			*
			* Description  : This routine to notify UI the response of getting ban list. 
			*
			* @param	   : uGid - group id.
			*				mapFidUids - map<FID, set<UIDs>> //folder ban list.
			*				
			* @return	   : N/A
			*---------------------------------------------------------------------------*/
			virtual void OnGetGrpFldChatBanListRes(const uint32_t &uGid, const protocol::ginfo::FldUIDMapType &mapFidUids){} //= 0;

			virtual void OnAddGrp5MinsBanList
			(
				const uint32_t &uGid, const uint32_t &uFid, 
				const uint32_t &uReqUid, const std::set<uint32_t> &setBanUids
			){}
			
		};
		
		// Provide APIs to set/cancel GInfo watchers for UI
		// Will be implemented by WatchHelper
		template <class T>
		struct IOpGInfoWatcher
		{
			virtual ~IOpGInfoWatcher(){}
			virtual void watch(T *w) = 0;
			virtual void revoke(T *w) = 0;
		};

		typedef IOpGInfoWatcher<IGInfoGeneralWatcher> IGInfoGenWatcherSetterT;
		typedef IOpGInfoWatcher<IGInfoListWatcher> IGInfoListWatcherSetterT;
		typedef IOpGInfoWatcher<IGInfoRoleListWatcher> IGInfoRoleListWatcherSetterT;
		//typedef IOpGInfoWatcher<IGInfoBroadcastWatcher> IGInfoBrocWatcherSetterT;
		typedef IOpGInfoWatcher<IGInfoGrpBanListWatcher> IGInfoBanListWatcherSetterT;//GRP_CHAT_BAN
	}
}
#endif //IGINFOWATCHERS_H_INCLUDED
