#pragma once

#include <vector>

#include "common/int_types.h"
#include "common/unistr.h"
#include "common/protocol/const.h"



namespace protocol
{
	namespace im
	{
		//ר������Ⱥ�洢UInfo Simple�Ľṹ
		struct SUInfoSimple4Grp
		{
			uint32_t m_uId;
			UniString m_strNickname;
			protocol::uinfo::SEX m_sex;

			SUInfoSimple4Grp() : m_uId(-1), m_sex(protocol::uinfo::female) {}
			SUInfoSimple4Grp(uint32_t uUid, const UniString &strNick, protocol::uinfo::SEX sex) :
			m_uId(uUid), m_strNickname(strNick), m_sex(sex) {}

			SUInfoSimple4Grp& operator = (const SUInfoSimple4Grp& obj)
			{
				if (this == &obj)
				{
					return *this;
				}
				m_uId = obj.m_uId;
				m_strNickname = obj.m_strNickname;
				m_sex = obj.m_sex;
				return *this;
			}
		};

		enum EImUserInfoType
		{
			ENUM_IM_UINFO_SIMPLE= 0,
			ENUM_IM_UINFO_DETAIL= 1,
		};

		struct CImUserInfoSimpleClientData
		{
			CImUserInfoSimpleClientData()
			{
				m_uId=-1;
				m_sex=protocol::uinfo::female;
				m_eInfoType=ENUM_IM_UINFO_SIMPLE;
			}
			CImUserInfoSimpleClientData& operator = (const CImUserInfoSimpleClientData& obj)
			{
				if (this == &obj)
				{
					return *this;
				}
				m_uId = obj.m_uId;
				m_strNickname = obj.m_strNickname;
				m_sex = obj.m_sex;
				m_eInfoType = obj.m_eInfoType;
				return *this;
			}

			uint32_t m_uId;
			UniString m_strNickname;
			//protocol::uinfo::SEX m_sex;
			//EImUserInfoType m_eInfoType;
			uint8_t m_sex;
			uint8_t m_eInfoType;
		};

		struct CImUserInfoDetailClientData:public CImUserInfoSimpleClientData
		{

			CImUserInfoDetailClientData()
			{
				m_uBirthDay=(uint32_t)-1;
				m_uArea=(uint32_t)-1;
				m_uProvince=(uint32_t)-1;
				m_uCity=(uint32_t)-1;
				m_uJiFen=(uint32_t)-1;
			}

			CImUserInfoDetailClientData& operator = (const CImUserInfoDetailClientData& obj)
			{
				if (this == &obj)
				{
					return *this;
				}
				m_uId = obj.m_uId;
				m_strNickname = obj.m_strNickname;
				m_sex = obj.m_sex;
				m_eInfoType = obj.m_eInfoType;

				m_uBirthDay = obj.m_uBirthDay;
				m_uArea = obj.m_uArea;
				m_uProvince = obj.m_uProvince;
				m_uCity = obj.m_uCity;
				m_uJiFen = obj.m_uJiFen;
				m_strSign = obj.m_strSign;
				m_strIntro = obj.m_strIntro;
				return *this;
			}

			
			uint32_t m_uBirthDay;
			uint32_t m_uArea;
			uint32_t m_uProvince;
			uint32_t m_uCity;
			uint32_t m_uJiFen;
			UniString m_strSign;
			UniString m_strIntro;
		};

		struct IImUInfoWatcher
		{
			//UIʵ�ֵĽӿ�
			virtual void OnSimpleChange(const std::vector<CImUserInfoSimpleClientData*>& vecData) = 0;
			virtual void OnDetailChange(const std::vector<CImUserInfoDetailClientData*>& vecData) = 0;

			//�µ�Ⱥ�б�uinfo�ص�
			virtual void OnSimp4GrpChange(const std::vector<SUInfoSimple4Grp*>& vecData) {}
		};

		struct IImUInfoWatchBase 
		{
			virtual void watch(IImUInfoWatcher *) = 0;
			virtual void revoke(IImUInfoWatcher *) = 0;
		};

		//�ײ�ʵ�ֵĽӿ�
		struct IImUInfo:public virtual IImUInfoWatchBase
		{
			virtual void GetSimpleImUInfo(const std::vector<uint32_t/*���鿴�˵�ID�б�*/> &vecUid)=0;
			virtual void GetDetailImUInfo(const std::vector<uint32_t/*���鿴�˵�ID�б�*/> &vecUid)=0;
		};

	}
}
