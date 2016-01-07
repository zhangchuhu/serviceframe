#ifndef IGROUPLISTWATCHER_H_
#define IGROUPLISTWATCHER_H_

#include "common/int_types.h"
#include "common/protocol/IGrpListDataType.h"

#include <vector>
#include <set>
#include <map>
#include <utility>

namespace protocol{

namespace glist{

// interface of group list watcher.
struct IGrpListWatcher
{
	// *************** group list *************

	// get group list.
	// not include folderIds.
	virtual void onGetGroupList(const std::set<uint32_t>& setGroupId) = 0;

	virtual void onNewGroup(uint32_t uGroupId, uint32_t uResCode) = 0;

	// if "uResCode==RES_EREQUEST", uGroupId added before!
	virtual void onAddGroup(uint32_t uGroupId, uint32_t uResCode) = 0;

    //Justin fang -TODO:BUSYBOX-BETA6-RM
#if 0    
	// *************** receive mode *************

	// the user sets receive mode of a group or folder.
	// set group's recvMode when uGrpId == uFolderId.
	virtual void onSetReceiveMode
	(
	    const uint32_t &uGroupId, 
	    const uint32_t &uFolderId, 
	    const ERecvMode &eRecvMode, 
	    const bool &bResult
    ) = 0;

	// *************** refuse topic ***********

	// get refused topics.
	//map<GID/FID, set<TIDs>>
	virtual void onGetRefuseTopics(const std::map<uint32_t, std::set<uint32_t> >&mapRefuTopic) = 0;

	// refuse a topic in group or folder (uFolderId).
	// uGroupId helps UI to judge uFolderId belongs to which uGroupId.
	// use forEachWatcherRef4, so add "const &" here.
	virtual void onAddRefuseTopic(const uint32_t& uGroupId, const uint32_t& uFolderId, 
								const uint32_t& uTopicId, const bool& bResult) = 0;
    
	// batch refuse topics in group or folder (uFolderId).
	// uGroupId helps UI to judge uFolderId belongs to which uGroupId.
	// use forEachWatcherRef4, so add "const &" here.
	virtual void onBatAddRefuseTopic(const uint32_t& uGroupId, const uint32_t& uFolderId, 
		const std::vector<uint32_t>& vecTopicId, const bool& bResult) = 0;

	// cancel refuse in group or folder (uFolderId)
	virtual void onRemoveRefuseTopic(const uint32_t& uGroupId, const uint32_t& uFolderId, 
									const uint32_t& uTopicId, const bool& bResult) = 0;
#endif    

	// **************** group alias ***************

	// get group alias.
	// folder has no alias.
	// map<groupId, groupAlias>
	virtual void onGetGroupAlias(const std::map<uint32_t, std::wstring>& mapGroupAlias) = 0;

	// set a group's alias. the original name as default.
	// bResult: true if success, false on else.
	virtual void onSetGroupAlias(uint32_t uGroupId, bool bResult) = 0;


    //Justin fang -TODO:BUSYBOX-BETA6-RM
#if 0    
	// *************** care topic ***********

	// get care topics.
	//map<GID/FID, set<[FID, CareType]>>
	virtual void onGetCareTopics(const std::map<uint32_t, std::set<CCareTopic> > &m_mapCareTopics) = 0;

	// add care topic in group or folder (uFolderId)
	// uGroupId helps UI to judge uFolderId belongs to which uGroupId.
	// use forEachWatcherRef4, so add "const &" here.
	virtual void onAddCareTopic(const uint32_t& uGroupId, const uint32_t& uFolderId, 
		const uint32_t& uTopicId, const bool& bResult) = 0;

	// batch add care topic in group or folder (uFolderId)
	// uGroupId helps UI to judge uFolderId belongs to which uGroupId.
	// use forEachWatcherRef4, so add "const &" here.
	virtual void onBatAddCareTopic(const uint32_t& uGroupId, const uint32_t& uFolderId, 
		const std::vector<CCareTopic>& vecCareTopics, const bool& bResult) = 0;

	// remove care topic in group or folder (uFolderId)
	virtual void onRemoveCareTopic(const uint32_t& uGroupId, const uint32_t& uFolderId, 
		const uint32_t& uTopicId, const bool& bResult) = 0;

    //Justin fang -TODO: BUSYBOX-DCHAT
    // *************** Default Chat Mode ***********
    
    /**---------------------------------------------------------------------------
    * Function     : onGetDChatMode
    *
    * Description  : This routine handles the User gets the Default Chat Mode of a Group of Folder.
    *               When get the Default Chat Mode for a group, uGrpId equals to uFolderId.(uGrpId==uFolderId)
    *
    * @param       : mapDChatMode - the reference of map storing the result of Default Chat Mode
    *                             map<GID, set<[FID, Mode]>>
    *               
    * @return      : N/A
    *---------------------------------------------------------------------------*/    
	virtual void onGetDChatMode(const std::map<uint32_t, std::set<SDChatMode> > &mapDChatMode) = 0;

    /**---------------------------------------------------------------------------
    * Function     : onSetDChatModeRes
    *
    * Description  : This routine to notify UI the result of setting request.
    *
    * @param       : uGroupId - Group ID
    *               uFolderId - Folder ID
    *               eDChatMode - Default Chat Mode
    *               bResult - true: success, false: failed.
    *               
    * @return      : N/A
    *---------------------------------------------------------------------------*/  
	virtual void onSetDChatModeRes(const uint32_t &uGroupId, const uint32_t &uFolderId, const protocol::glist::EDChatMode &eDChatMode, const bool &bResult) = 0;
#endif

    //Justin fang -TODO:BUSYBOX-BETA6
    virtual void onSetMsgReceiveMode(const uint32_t &uGid,const uint32_t &uFid, const protocol::glist::EMsgRcvMode &eMsgRcvMode, const bool &bResult) = 0;
        
};

struct IGrpListWatchBase
{
	virtual void watch(IGrpListWatcher *) = 0;
	virtual void revoke(IGrpListWatcher *) = 0;
};

} // namespace glist

} // namespace protocol 

#endif
