#pragma once

#include <vector>
#include <string>
#include "common/unistr.h"
#include "common/protocol/const.h"

namespace protocol
{
	namespace gmemberinfo
	{

		enum EGMemberInfoType
		{
			ENUM_GROUP_MEMBER_INFO_SIMPLE= 0,
			ENUM_GROUP_MEMBER_INFO_DETAIL= 1,
			ENUM_GROUP_MEMBER_INFO_SIMPLE_GZIP
		};


		struct CGMemberInfoSimpleClientData
		{
			CGMemberInfoSimpleClientData()
			{
				m_uId = INVALID_UID;
				m_uGid = INVALID_UID;
				m_eSex = protocol::uinfo::female;
				m_eInfoType=ENUM_GROUP_MEMBER_INFO_SIMPLE;
			}
			uint32_t m_uGid;//群ID
			uint32_t m_uId;//用户ID
			UniString m_strNickNameInGroup;
			//protocol::uinfo::SEX m_eSex;
			//EGMemberInfoType m_eInfoType;
			uint8_t m_eSex;
			uint8_t m_eInfoType;

			CGMemberInfoSimpleClientData& operator = (const CGMemberInfoSimpleClientData& obj)
			{
				if (this == &obj)
				{
					return *this;
				}
				m_uGid = obj.m_uGid;
				m_uId = obj.m_uId;
				m_strNickNameInGroup = obj.m_strNickNameInGroup;
				m_eSex = obj.m_eSex;
				m_eInfoType = obj.m_eInfoType;
				return *this;
			}
		};


		struct CGMemberInfoDetailClientData:public CGMemberInfoSimpleClientData
		{

			CGMemberInfoDetailClientData()
			{
				m_bIsAssist=false;
			}


			UniString m_strTel;
			UniString m_strEmail;
			UniString m_strAddress;
			UniString m_strRemark;
			bool m_bIsAssist;//true 代表允许管理员协助修改群名片


		};

		struct IGmemberInfoWatcher
		{
			//UI实现的接口
			virtual void OnSimpleChange(const std::vector<CGMemberInfoSimpleClientData*>& vecData) = 0;
			virtual void OnDetailChange(const std::vector<CGMemberInfoDetailClientData*>& vecData) = 0;
			
		};

			
		struct IGmemberInfoWatchBase 
		{
			virtual void watch(IGmemberInfoWatcher *) = 0;
			virtual void revoke(IGmemberInfoWatcher *) = 0;
		};

		//底层实现的接口
		struct IGmemberInfo:public virtual IGmemberInfoWatchBase
		{
			////查看自己或别人的群名片
			//virtual void GetGMemberInfo(const std::map<uint32_t /*群ID*/,std::vector<uint32_t/*被查看人的ID列表*/> > &mapUid)=0;
			virtual void GetGMemberInfoSimple(const std::map<uint32_t /*群ID*/,std::vector<uint32_t/*被查看人的ID列表*/> > &mapUid)=0;
			virtual void GetGMemberInfoDetail(const std::map<uint32_t /*群ID*/,std::vector<uint32_t/*被查看人的ID列表*/> > &mapUid)=0;

			//修改自己群名片或帮群成员修改群名片
			//virtual void SetGMemberInfo(const CGMemberInfoData& objMemberInfo)=0;
			virtual void SetGMemberInfo(const CGMemberInfoDetailClientData& objMemberInfo)=0;
		};

	}
}
