//群动态通知接口
#ifndef PROTOCOL_IGROUP_ACTIONS_WATCHER_H
#define PROTOCOL_IGPROP_ACTIONS_WATCHER_H

#include "common/unistr.h"
#include "common/int_types.h"
#include "protocol/PGInfoCommonTypes.h"
#include <vector>

namespace protocol
{
namespace gactions
{
	struct IGroupActionsWatcher
	{
		// If the user is added by an admin, uAddedBy will be the admin's UID, otherwise it is N_VOID_ID
		// If the user is added to the default folder, uFldID is uGrpID
		virtual void OnNewMember(const uint32_t& uGrpID, const uint32_t& uFldID, const uint32_t& uAddedBy,
			const std::vector<uint32_t>& vecUIDs) = 0;

		virtual void OnNewMemberFromChannel(const uint32_t& uGrpID, const uint32_t& uFldID, const uint32_t& uChannelId,
			const uint32_t& uAddedBy,const std::vector<uint32_t>& vecUIDs){}
		
		virtual void OnDelMember(const uint32_t& uGrpID, const uint32_t& uFldID, const uint32_t& uDeletedBy,
			const std::vector<uint32_t>& vecUIDs) = 0;

        //Justin fang -TODO:BUSYBOX-DISMISS TOU
        virtual void OnKickMember(const uint32_t& uGrpID, const uint32_t& uFldID, const UniString &strGFidName, 
            const uint32_t& uDeletedBy, const std::vector<uint32_t>& vecUIDs) {};
        
		virtual void OnMoveMember(const uint32_t& uGrpID, const uint32_t& uFromFldID, const uint32_t& uToFldID,
			const uint32_t& uMovedBy, const std::vector<uint32_t>& vecUIDs) = 0;

		virtual void OnMoveOutFldPop(const uint32_t &uGid, const uint32_t &uFromFid, 
			const uint32_t &uMoveBy, const std::vector<uint32_t> &vecUIDs) {}//= 0;

		virtual void OnNewFolder(const uint32_t& uGrpID, const uint32_t& uFldID, const uint32_t& uAddedBy) = 0;		

        //Justin fang -TODO:BUSYBOX-DISMISS TOU
		virtual void OnDelFolder(const uint32_t& uGrpID, const uint32_t& uFldID, const UniString &strFlderName, const uint32_t& uDeletedBy){};

        //Justin fang -TODO:BUSYBOX-DISMISS TOU
        virtual void OnDismissGrp(const uint32_t& uGrpID, const UniString &strGrpName, const uint32_t& uDeletedBy){};
		
		// Examples:
		// Add a new folder admin: GID=17,FID=117,Setter=11,Adm=12340020,Role=2
		// Add a new group admin: GID=17,FID=0,Setter=11,Adm=12340020,Role=1
		// Transfer OW: GID=17,FID=0,Setter=11,Adm=12340020,Role=0
		// Note that OnDelAdmin is NOT triggered if the new admin has a role before (eg, FldAdm -> GrpAdm)
		virtual void OnNewAdmin(const uint32_t& uGrpID, const uint32_t& uFldID, const uint32_t& uSetBy, 
			const uint32_t& uAdmin, const protocol::ginfo::EUserRole& enNewRole) = 0;
		
		virtual void OnDelAdmin(const uint32_t& uGrpID, const uint32_t& uFldID, const uint32_t& uSetBy, 
			const uint32_t& uAdmin, const protocol::ginfo::EUserRole& enOldRole) = 0;

		//GRP_CHAT_BAN
		/*Notify myself banned. (pop)*/
		virtual void OnBanNotifyMe
		(
			const uint32_t &uReqUid, const uint32_t &uGid, const uint32_t &uFid, 
			const protocol::ginfo::E_BANREASON &eBanRsn, const std::string &strRsn
		){}// = 0;

		/*Notify admin banned list.*/
		virtual void OnBanNotifyAdmin
		(
			const uint32_t &uReqUid, const uint32_t &uGid, const uint32_t &uFid,
			const std::set<uint32_t> &setBanUids,
			const protocol::ginfo::E_BANREASON &eBanRsn, const std::string &strRsn
		){}// = 0;

		/*Notify myself unbanned.*/
		virtual void OnUnbanNotifyMe(const uint32_t &uReqUid, const uint32_t &uGid, const uint32_t &uFid){}// = 0;

		/*Notify admin unbanned list.*/
		virtual void OnUnbanNotifyAdmin
		(
			const uint32_t &uReqUid, const uint32_t &uGid, 
			const uint32_t &uFid,const std::set<uint32_t> &setBanUids
		){}// = 0;
		
	
	};

	struct IGroupActions
	{
		virtual void watch(IGroupActionsWatcher *w) = 0;
		virtual void revoke(IGroupActionsWatcher *w) = 0;
	};
	
}
}
#endif

 