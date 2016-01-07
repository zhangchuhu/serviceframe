//Types that can be shared by UI and protocol
#ifndef IGINFODATATYPES_H_INCLUDED
#define IGINFODATATYPES_H_INCLUDED

#include "common/unistr.h"
#include "common/protocol/PGInfoCommonTypes.h"
#include <vector>
#include <set>
#include <map>


namespace protocol
{
	namespace ginfo
	{
		// Constants
		const uint32_t N_VOID_ID = 0;
		const uint32_t N_IS_ADMIN = 1;
		const uint32_t N_NON_ADMIN = 0;
		const std::vector<uint32_t> N_VOID_VECTOR;

		// For now, the structures here just reflect the tables in DB
		// They are subject to change based on the needs of UI.


		// FolderID/UID list
		typedef std::set<uint32_t> SetIDListT;
		
		//for test
		//typedef std::vector<uint32_t> VecIDListT;
		//typedef std::map<uint32_t, VecIDListT> FolderMemberListT2;
		//typedef google::sparse_hash_map<uint32_t, HSetIDListT> FolderMemberListT;

		// key: UID, value: the role of the user
		typedef std::map<uint32_t, protocol::ginfo::EUserRole> UserRoleT;




		// key: Folder ID, or group ID for group admins
		typedef std::map<uint32_t, UserRoleT> FolderRoleListT;
		//typedef std::map<uint32_t, IDListT> FolderRoleListT;

		// key: Group ID
		typedef std::map<uint32_t, SetIDListT> GroupFolderIDListT;
		
		// key: Folder ID, or group ID for independent members
		typedef std::map<uint32_t, SetIDListT> FolderMemberListTv0;


		// Reasons for OnGInfoAdd/OnGInfoRemove events.
		
		enum EChangeReason {
			ENUM_GET_FOLDER_LIST, //0
			ENUM_GET_FOLDER_MEMBERS, //1
			ENUM_NEW_GROUP, //2
			ENUM_NEW_FOLDER, //3 GInfo does not broadcast new folder
			ENUM_DEL_GROUP, //4 Will be raised for the requester who dismissed the group
			ENUM_DEL_GROUP_BROC, //5 Will be raised for other members
			ENUM_DEL_FOLDER, //6 If I'm in the folder
			ENUM_DEL_FOLDER_BROC, //7 If i'm not in the folder
			ENUM_QUIT_GROUP, //8 For user who quits the group
			ENUM_QUIT_GROUP_BROC, //9 For other members
			ENUM_QUIT_FOLDER, //10 I quit a folder
			ENUM_QUIT_FOLDER_BROC, //11
			ENUM_KICKOUT_GROUP, //12 I am forced out of a group
			ENUM_KICKOUT_GROUP_BROC, //13 Other member is forced out
			ENUM_KICKOUT_FOLDER, //14 I am forced out of a folder
			ENUM_KICKOUT_FOLDER_BROC, //15 A member (could be myself) is forced out
			ENUM_JOIN_GROUP, //16 User joined a group
			ENUM_JOIN_GROUP_BROC, //17 Other members received the event that a user joined
			ENUM_APPROVED_TO_JOIN, //18 My join group request is approved
			ENUM_APPROVED_TO_JOIN_BROC, //19 Other members received the event that a user joined
			ENUM_INVITED_TO_JOIN, //20 The user accepted the join invitation
			ENUM_INVITED_TO_JOIN_BROC, //21 Other members received the event that a user joined
			ENUM_COPY_MEMBERS, //22 I'm copied
			ENUM_COPY_MEMBERS_BROC, //23
			ENUM_MOVE_MEMBERS, //24 I'm moved
			ENUM_MOVE_MEMBERS_BROC, //25 Other members are moved
			ENUM_ADD_TO_FOLDER, //26 My join folder request is approved
			ENUM_ADD_TO_FOLDER_BROC, //27 Other members received the event that a user joined
			ENUM_GET_MY_JOINED_FOLDERS, //28
			ENUM_NOT_IN_GROUP, //29
			ENUM_OTHER_REASON //			
		};
		
		// Group/folder information
		struct CGrpMiniInfo
		{
			uint32_t m_uParentID;
			//Flag which indicates whether the serving user is a member of the folder
			//For example, UI may want to know whether the current user is allowed to open a
			// folder. The member list may not be available, so this flag which is returned by
			// the server should be checked.
			bool m_bIsMember;

			CGrpMiniInfo()
			{
				m_uParentID = 0;
				m_bIsMember = false;
			}

		};
		
		// Group/folder information
		struct CGrpBaseInfo : public CGrpMiniInfo
		{
			GrpInfoMask m_objMask;
			UniString m_strName;
			UniString m_strBulletin;
			UniString m_strDesc;
			protocol::ginfo::EAuthMode m_enAuthMode;
			//uint16_t m_uChRoler;
			//uint32_t m_uChannelID;
			std::map<uint32_t, uint16_t> m_mapChannelRole;
						
			CGrpBaseInfo()
			{
				m_objMask.uMaskWord = 0;
				m_enAuthMode = ENUM_UNKNOWN_MODE;	
			}
			
			
			void SetName(UniString& strName)
			{
				m_objMask.b.Name = true;
				m_strName = strName;
			}
			void SetBulletin(UniString strBulletin)
			{
				m_objMask.b.Bulletin = true;
				m_strBulletin = strBulletin;			
			}
			void SetDesc(UniString strDesc)
			{
				m_objMask.b.Desc = true;
				m_strDesc = strDesc;			
			}

			void SetAuthMode(protocol::ginfo::EAuthMode enAuthMode)
			{
				m_objMask.b.GrpAuthMode = true;
				m_enAuthMode = enAuthMode;
			}

			void AddChannelRoler(uint32_t uChannelID, uint16_t uChRoler, bool bReset = false)
			{
				//m_uChannelID = uChannelID;
				//m_uChRoler = uChRoler;
				if (bReset) m_mapChannelRole.clear();
				m_mapChannelRole[uChannelID] = uChRoler;
			}
		};

		//GInfo doesn't handle this information which is handled by GProps now.
		//It just relays the information from UI to GProps.
		struct CGroupInfo : public CGrpBaseInfo
		{
			uint32_t m_uOwnerID;
			uint16_t m_uCategory;
			uint16_t m_uSubCategory;
			uint32_t m_uLogoIndex;
			//Is member allowed to initiate an adhoc chatting with another member?
			bool m_bIsAdhocChatAllowed;
			//Is member allowed to access a folder where the member doesn't belong.
			bool m_bIsPrivate;
			UniString m_strLogoURL;
			
			CGroupInfo() 
			{
				m_uCategory = m_uSubCategory = m_uLogoIndex = m_uOwnerID = 0;
			
			}
			
			void SetOwnerID(uint32_t uOwnerID)
			{
				m_objMask.b.GrpOwnerID = true;
				m_uOwnerID = uOwnerID;
			}
			void SetCategory(uint16_t uCategory, uint16_t uSubCategory)
			{
				m_objMask.b.GrpCategory = true;
				m_uCategory = uCategory;
				m_uSubCategory = uSubCategory;
			}
			void SetLogoIndex(uint32_t uLogoIndex)
			{
				m_objMask.b.GrpLogoIndex = true;
				m_uLogoIndex = uLogoIndex;
			}
			void SetIsAdhocChatAllowed(bool bIsAdhocChatAllowed)
			{
				m_objMask.b.GrpAdhocChatSetting = true;
				m_bIsAdhocChatAllowed = bIsAdhocChatAllowed;
			}
			void SetIsPrivate(bool bIsPrivate)
			{
				m_objMask.b.GrpPrivacySetting = true;
				m_bIsPrivate = bIsPrivate;
			}
			void SetLogoURL(UniString& strLogoURL)
			{
				m_objMask.b.GrpLogoURL = true;
				m_strLogoURL = strLogoURL;
			}
		};


		struct InvitationCode : public sox::Marshallable
		{
			uint32_t m_uChk;
			uint32_t m_uCode;
			uint32_t m_uGrpID;
			uint32_t m_uFldID;
			std::string m_strMessage;
			std::string m_strBase64;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uChk << m_uCode << m_uGrpID << m_uFldID << m_strMessage;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uChk >> m_uCode >> m_uGrpID >> m_uFldID >> m_strMessage;
			}
		};
		
		// The fully qualified ID which contains GID, FID and UID.
		struct FullyQualifiedID
		{
			uint32_t m_uGrpID;
			uint32_t m_uFldID;
			uint32_t m_uUID;

			FullyQualifiedID() :
				m_uGrpID(N_VOID_ID), m_uFldID(N_VOID_ID), m_uUID(N_VOID_ID)
			{
			}

			FullyQualifiedID(uint32_t uGrpID, uint32_t uFldID, uint32_t uUID) :
				m_uGrpID(uGrpID), m_uFldID(uFldID), m_uUID(uUID)
			{
			}

			const FullyQualifiedID& make(uint32_t uGrpID, uint32_t uFldID, uint32_t uUID)
			{
				m_uGrpID = uGrpID;
				m_uFldID = uFldID;
				m_uUID = uUID;
				return *this;
			}
			
			friend std::ostream & operator << (std::ostream & t, FullyQualifiedID& i)
			{
				t << i.m_uGrpID << "/" << i.m_uFldID << "/" << i.m_uUID;
				return t;
			}

		};

		typedef std::vector<FullyQualifiedID> FQIDVectorT;

		struct FullyQualifiedIDWithRole
		{
			uint32_t m_uGrpID;
			uint32_t m_uFldID;
			uint32_t m_uUID;
			protocol::ginfo::EUserRole m_enRole;

			FullyQualifiedIDWithRole() :
			m_uGrpID(N_VOID_ID), m_uFldID(N_VOID_ID), m_uUID(N_VOID_ID),m_enRole(ENUM_UNKNOWN_ROLE)
			{
			}

			FullyQualifiedIDWithRole(uint32_t uGrpID, uint32_t uFldID, uint32_t uUID, protocol::ginfo::EUserRole enRole) :
			m_uGrpID(uGrpID), m_uFldID(uFldID), m_uUID(uUID),m_enRole(enRole)
			{
			}

			const FullyQualifiedIDWithRole& make(uint32_t uGrpID, uint32_t uFldID, uint32_t uUID, protocol::ginfo::EUserRole uRole)
			{
				m_uGrpID = uGrpID;
				m_uFldID = uFldID;
				m_uUID = uUID;
				m_enRole = uRole;
				return *this;
			}
			
			friend std::ostream & operator << (std::ostream & t, FullyQualifiedIDWithRole& i)
			{
				t << i.m_uGrpID << "/" << i.m_uFldID << "/" << i.m_uUID << "/" << i.m_enRole;
				return t;
			}

// 			friend std::string & operator << (std::string & t, FullyQualifiedIDWithRole& i)
// 			{
// 				char buffer [24];
// 				t.append(itoa(i.m_uGrpID, buffer, 10)).append("/");
// 				//t << i.m_uGrpID << "/" << i.m_uFldID << "/" << i.m_uUID << "/" << i.m_enRole;
// 				return t;
// 			}
		};

		typedef std::vector<FullyQualifiedIDWithRole> FQIDRoleVectorT;

		struct GInfoGroupChangeNotice
		{
			uint32_t m_uGrpID;
			//GrpInfoMask m_objMask;
			const CGroupInfo* objInfo;
		};

		struct GInfoFolderChangeNotice
		{
			uint32_t m_uGrpID;
			uint32_t m_uFldID;
			//GrpInfoMask m_objMask;
			const CGrpBaseInfo* objInfo;
		};

		typedef std::vector<GInfoGroupChangeNotice> GInfoGroupChangeNoticeVectorT;

		typedef std::vector<GInfoFolderChangeNotice> GInfoFolderChangeNoticeVectorT;

		// key: Folder ID
		typedef std::map<uint32_t, CGrpMiniInfo> GroupFolderInfoListT;

		// key: Group ID
		//typedef std::map<uint32_t, CGroupInfo> GroupInfoListT;

	}
}

#endif // IGINFODATATYPES_H_INCLUDED
