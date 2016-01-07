//common types for both server side and protocol side
#ifndef PGINFOCOMMONTYPES_H_INCLUDED
#define PGINFOCOMMONTYPES_H_INCLUDED

#include "common/packet.h" //sox::Marshallable
#include "common/iproperty.h"

#include <vector>

namespace protocol{
	namespace ginfo{
		enum EAuthMode {
			ENUM_NO_AUTH, //Allow anyone to join without authentication
			ENUM_NEED_AUTH, //Need approval from admin
			ENUM_NO_ADDING, //Do not allow anyone to join
			ENUM_BY_SESSION, //Need auth by session
			ENUM_UNKNOWN_MODE //Abnormal value
		};

		enum EUserRole { //Note: Sorted by privilege level.
			ENUM_GRP_OW, //Group owner
			ENUM_GRP_ADM, //Group admin
			ENUM_FLD_ADM, //Folder admin
			ENUM_UNKNOWN_ROLE //Abnormal value
		};

		enum ERejectType {
			ENUM_REJ_JOIN_GROUP,//0
			ENUM_REJ_JOIN_FOLDER,//1
			ENUM_REJ_INV_GROUP,//2
			ENUM_REJ_INV_FOLDER,//3
			ENUM_REJ_UNKNOWN //Abnormal value
		};

		enum EInvitationType {
			ENUM_INV_TO_GROUP,//0
			ENUM_INV_TO_FOLDER,//1
			ENUM_INV_CODE_TO_GROUP,//2
			ENUM_INV_CODE_TO_FOLDER,//3
			ENUM_INV_UNKNOWN //Abnormal value
		};

		enum EAddToGrpOrFolderType {
			ENUM_ADDTO_FLD_APPROVED, //0 - Admin approves a member's join request
			ENUM_ADDTO_FLD_MOVED, //1 -- Admin moves a member to a folder
			ENUM_ADDTO_FLD_DIRECT, //2 -- Joins a folder directly
			ENUM_ADDTO_OTHER, //3 -- For group operations, or the join folder request is pending
		};

		//GRP_CHAT_BAN
		enum E_BANREASON
		{
			E_SYS_REASON_1 = 0,
			E_SYS_REASON_2,
			E_SYS_REASON_3,
			E_CUSTOM_REASON = 0xff
		};

		// Max number of groups a user can own
		const uint16_t N_MAX_GROUP_OWNED_BY_USER = 5;

		// Max number of group/folder admin
		const uint16_t N_MAX_GROUP_ADMIN_NUM = 1000;

		// Max number of folders per gorup
		const uint16_t N_MAX_GROUP_FOLDER_NUM = 2000;

		struct CGroupCategory
		{
			uint16_t uCategory;
			uint16_t uSubCategory;
		};

		struct CGroupLogo
		{
			uint32_t uLogoIndex;
			std::string strLogoURL;
		};

		union GrpInfoMask
		{
			struct Bits {
				bool Name:1;
				bool Bulletin:1;
				bool Desc:1;
				bool GrpOwnerID:1;
				bool GrpCategory:1;
				bool GrpAuthMode:1;
				bool GrpLogoIndex:1;
				bool GrpLogoURL:1;
				bool GrpAdhocChatSetting:1;
				bool GrpPrivacySetting:1; //May require user to re-login to take effect if this option is changed.
				//bool FldList:1;
				//bool FldName:1; //Require FldList=1 Names are always contained now
				//bool FldMemberList; //Require FldList=1 *deprecated*
				//bool DefaultFldMemberList; //List of independant members(大群未分组成员)
				//bool IsFldMember:1;
			} b;

			// Filter constants.
			// Not very safe. The ordering is architecture dependent.
			enum
			{
				ENUM_FILTER_BASIC = 0x7, // name=1, bulletin=1, desc=1
				ENUM_FILTER_GROUP = 0x3F8 //1111111000
			};

			uint32_t uMaskWord;

		};

		// *** Deprecated! ***
		union GrpMemberRole
		{
			struct Bits {
				bool Member:1; //Indicates that it's a member of the folder. Can be false for group admin
				bool Unavailable:1; //Init value=true. Indicates that the role info is unavailable
				bool Admin:1; //Is a group/folder admin
			} b;

			uint16_t uMaskHalfWord;
		};

		// This is the element of the folder information vector which is carried
		// in the GetGroupInfo response.
		// It contains the folder's GID, name and its member list
		struct CGrpFolderInfo : public sox::Marshallable
		{
			//std::string m_strFldName;
			bool m_bIsMember;
			//GFldMemberInfoMapType m_mapMembList;

			CGrpFolderInfo():m_bIsMember(false) {}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_bIsMember;
				//p << m_strFldName << m_bIsMember;
				//sox::marshal_container(p, m_mapMembList);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_bIsMember;
				//p >> m_strFldName >> m_bIsMember;
				//sox::unmarshal_container(p, std::inserter(m_mapMembList, m_mapMembList.begin()));
			}

		};

		// Mapping of FldID - CGrpFolderInfo, can be used as the folder list of a group
		// The key of the pair must be comparable and packable/unpackable.
		// The value must be packable/unpackable. It's usually a marshablle.
		typedef std::map<uint32_t, CGrpFolderInfo> FID2FldInfoMapType;

		// New - Folder list of the groups
		struct CGrpFldListRes : public sox::Marshallable
		{
			FID2FldInfoMapType m_mapFolderList;

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_mapFolderList);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::inserter(m_mapFolderList, m_mapFolderList.begin()));
			}
		};

		// New - Mapping of GrpID - CGrpFldListRes. Folder lists of multiple groups
		typedef std::map<uint32_t, CGrpFldListRes> MultiGroupFolderListsMapType;

		// This is the element of the GetGroupInfo response vector
		struct CGrpInfoRes : public sox::Properties
		{
			// Most of the results are stored in member props.

			// The folder information is not put into props to avoid
			// converting it into a string.
			// Availability of this information can still be determinated by checking
			// it's size: size() == 0 means it's not available.
			//FID2FldInfoMapType m_mapFldList;

			virtual void marshal(sox::Pack &p) const
			{
				Properties::marshal(p);
				//sox::marshal_container(p, m_mapFldList);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				Properties::unmarshal(p);
			}
		};

		// Mapping of GrpID - CGrpInfoRes, can be used as the group list
		typedef std::map<uint32_t, CGrpInfoRes> GID2GrpInfoMapType;

///////////////////////////////////////////////////////////////////////////////

		// Member info with role
		struct CGrpMembInfo : public sox::Marshallable
		{
			EUserRole m_enRole;

			CGrpMembInfo()
			{
				m_enRole = ENUM_UNKNOWN_ROLE;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p.push_uint8(m_enRole);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				m_enRole = (EUserRole)p.pop_uint8();
			}
		};

		// Mapping of UID <-> UserInfo, can be used as the member list of a folder
		typedef std::map<uint32_t, CGrpMembInfo> MemberInfoMapType;

		typedef std::vector<uint32_t> MemberVectorType;

		// Marshallable for MemberInfoMapType to pack/unpack it
		struct CMemberInfoListMar : public sox::Marshallable
		{
			//The member list with role info
			MemberInfoMapType m_mapMemInf;

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_mapMemInf);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::inserter(m_mapMemInf,
					m_mapMemInf.begin()));
			}

		};

		// Marshallable for MemberVectorType to pack/unpack it
		struct CMemberListMar : public sox::Marshallable
		{
			MemberVectorType m_vecMem;
			uint16_t m_uRes; // 0 - Forbidden, 1 - Successful

			CMemberListMar() :
				m_uRes(0)
			{

			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uRes;
				sox::marshal_container(p, m_vecMem);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uRes;
				sox::unmarshal_container(p, std::back_inserter(m_vecMem));
			}

		};

		// key: Folder ID
		typedef std::map<uint32_t, CMemberInfoListMar> FldMemberInfoListMapType;

		// key: Folder ID
		typedef std::map<uint32_t, CMemberListMar> FldMemberListMapType;

		typedef std::map<uint32_t, MemberVectorType> IDListMapType; //map of vectors

		typedef std::map<uint32_t, std::set<uint32_t> > FldUIDMapType; //map of sets
	}
}

#endif // PGINFOCOMMONTYPES_H_INCLUDED
