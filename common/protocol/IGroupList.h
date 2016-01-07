#ifndef IGROUPLIST_H_
#define IGROUPLIST_H_

#include "IGroupListWatcher.h"

namespace protocol{

namespace glist{

// interface for group list operations.
struct IGroupList: 
	public virtual IGrpListWatchBase
{
	virtual void Sync() = 0;

	virtual void Reset() = 0;

	//virtual void SetOwner(uint32_t uOwnerUID) = 0;

	// *************** group list *************

	// get the group ids of the user.
	// result returned by Call Back.
	virtual void DoGetGroupList() = 0;

    //Justin fang -TODO:BUSYBOX-BETA6-RM
#if 0    
	// *************** receive mode *************

	// folder has no receive mode.

	// the user gets receive modes of groups it belongs
	// special case: result returned directly.
	//Justin fang -TODO: BUSYBOX-OPTIMIZE
	virtual void DoGetReceiveMode(std::map<uint32_t, std::set<protocol::glist::CRecvMode> > &mapRecvModes) = 0;//map<GID, set<[FID, Mode]>>

	//virtual void DoSetReceiveMode(const protocol::glist::CRecvMode& objRecvMode) = 0;

	// the user sets receive mode of a group or folder.
	// set group's recvMode when uGrpId == uFolderId.
	virtual void DoSetReceiveMode(uint32_t uGrpId, uint32_t uFolderId, ERecvMode eRecvMode) = 0;

	// *************** refuse topic ***********

	// the user gets refuse topics of groups or folders it belongs to.
	// result returned by Call Back.
	virtual void DoGetRefuseTopic() = 0;

	// refuse a topic
	virtual void DoAddRefuseTopic(uint32_t uGroupId, uint32_t uFolderId, uint32_t uTopicId) = 0;
	// refuse several topics
	virtual void DoBatAddRefuseTopic(uint32_t uGroupId, uint32_t uFolderId, const std::vector<uint32_t>& vecTopicId) = 0;

	// cancel refuse.
	virtual void DoRemoveRefuseTopic(uint32_t uGroupId, uint32_t uFolderId, uint32_t uTopicId) = 0;
#endif

	// **************** group alias ***************

	// get a group's alias.
	// result returned by Call Back.
	virtual void DoGetGroupAlias() = 0;

	// set a group's alias.  the original name as default.
	virtual void DoSetGroupAlias(uint32_t uGroupId, const std::wstring& groupAlias) = 0;

    //Justin fang -TODO:BUSYBOX-BETA6-RM
#if 0    
	// *************** care topic ***********

	// the user gets care topics of groups or folders it belongs to.
	// result returned by Call Back.
	virtual void DoGetCareTopic() = 0;

	// care a topic
	virtual void DoAddCareTopic(uint32_t uGroupId, uint32_t uFolderId, uint32_t uTopicId, 
		protocol::glist::ECareType eCareType) = 0;

	// care several topics.
	virtual void DoBatchAddCareTopic(uint32_t uGroupId, uint32_t uFolderId, 
		const std::vector<CCareTopic>& vecCareTopics) = 0;

	// cancel care.
	virtual void DoRemoveCareTopic(uint32_t uGroupId, uint32_t uFolderId, uint32_t uTopicId) = 0; 

    //Justin fang -TODO: BUSYBOX-DCHAT
    // *************** Default Chat Zone Mode ***********
    /**---------------------------------------------------------------------------
    * Function     : DoSetDChatMode
    *
    * Description  : This routine handles the User sets the Default Chat Mode of a Group of Folder.
    *               When set the Default Chat Mode for a group, uGrpId equals to uFolderId.(uGrpId==uFolderId)
    *
    * @param       : uGrpId - Group ID
    *               uFolderId - Folder ID
    *               eDChatMode - the Mode to be set  
    *               
    * @return      : N/A
    *---------------------------------------------------------------------------*/
	virtual void DoSetDChatMode(uint32_t uGrpId, uint32_t uFolderId, EDChatMode eDChatMode) = 0;

    /**---------------------------------------------------------------------------
    * Function     : DoGetDChatMode
    *
    * Description  : This routine handles the User gets the Default Chat Mode of a Group of Folder.
    *               When get the Default Chat Mode for a group, uGrpId equals to uFolderId.(uGrpId==uFolderId)
    *
    * @param       : mapDChatMode - the reference of map storing the result of Default Chat Mode
    *               
    * @return      : N/A
    *---------------------------------------------------------------------------*/
    virtual void DoGetDChatMode(std::map<uint32_t, std::set<SDChatMode> > &mapDChatMode) = 0;
#endif

    //Justin fang -TODO:BUSYBOX-BETA6
    virtual void DoGetMsgReceiveMode(std::map<uint32_t, std::set<CMsgRcvMode> > &mapMsgRcvModes) = 0;
    virtual void DoSetMsgReceiveMode(uint32_t uGid, uint32_t uFid, EMsgRcvMode eMsgRcvMode) = 0;
    
};

} // namespace glist

} // namespace protocol 

#endif
