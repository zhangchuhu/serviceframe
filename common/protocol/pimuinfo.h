 #pragma once

#include <vector>
#include <string>

#include "iproperty.h"
#include "protocol/const.h"
#include "core/base_svid.h"
//#include "./pverifycodepng.h"
#include "common/protocol/IImUInfo.h"
#include "pimcommon.h"
#include "zip_tpl.h"

using namespace std;

namespace protocol
{
	namespace im
	{
	//本文件描述的消息定义，是客户端和服务器交互的消息定义
		//从imuinfo服务能够拿到的数据
		struct CImUserInfoAllData:public sox::Marshallable 
		{
			CImUserInfoAllData()
			{
				m_uId=(uint32_t)-1;
				m_uBirthDay=(uint32_t)-1;
				m_uArea=(uint32_t)-1;
				m_uProvince=(uint32_t)-1;
				m_uCity=(uint32_t)-1;

			}
			uint32_t m_uId;
			std::string m_strNickname;
			protocol::uinfo::SEX m_sex;
			uint32_t m_uBirthDay;
			uint32_t m_uArea;
			uint32_t m_uProvince;
			uint32_t m_uCity;
			std::string m_strResume;
			uint32_t m_uVersion;
			uint32_t m_uJiFen;
			std::string m_strSign;
			std::string m_strIntro;
			std::string m_strAccount;
			uint32_t m_uValidate;
			std::string m_strLastTime;
			std::string m_strRegisterTime;
			uint32_t m_uLastIp;
			std::string m_strPassport;

			virtual void marshal(sox::Pack &p) const 
			{
				p<<m_uId<<m_strNickname<<m_uBirthDay<<m_uArea<<m_uProvince<<m_uCity<<m_strResume<<m_uVersion<<m_uJiFen;
				p<<m_strSign<<m_strIntro<<m_strAccount<<m_uValidate<<m_strLastTime<<m_strRegisterTime<<m_uLastIp<<m_strPassport;
				p.push_uint8( m_sex);

			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p>>m_uId>>m_strNickname>>m_uBirthDay>>m_uArea>>m_uProvince>>m_uCity>>m_strResume>>m_uVersion>>m_uJiFen;
				p>>m_strSign>>m_strIntro>>m_strAccount>>m_uValidate>>m_strLastTime>>m_strRegisterTime>>m_uLastIp>>m_strPassport;
				m_sex = (protocol::uinfo::SEX)p.pop_uint8();

			}

		};



		struct CImUserInfoSimpleData:public sox::Marshallable 
		{
			CImUserInfoSimpleData()
			{
				m_uId=(uint32_t)-1;
				m_sex=protocol::uinfo::female;
			}
			uint32_t m_uId;
			std::string m_strNickname;
			protocol::uinfo::SEX m_sex;

			virtual void marshal(sox::Pack &p) const 
			{
				p <<  m_uId <<  m_strNickname;
				p.push_uint8( m_sex);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >>  m_uId >>  m_strNickname;
				m_sex = (protocol::uinfo::SEX)p.pop_uint8();
			}
		};
			

		struct CImUserInfoDetailData:CImUserInfoSimpleData 
		{
			CImUserInfoDetailData()
			{
				m_uBirthDay=(uint32_t)-1;
				m_uArea=(uint32_t)-1;
				m_uProvince=(uint32_t)-1;
				m_uCity=(uint32_t)-1;
				m_uJiFen=(uint32_t)-1;
			}
			uint32_t m_uBirthDay;
			uint32_t m_uArea;
			uint32_t m_uProvince;
			uint32_t m_uCity;
			uint32_t m_uJiFen;
			string m_strSign;
			string m_strIntro;


			virtual void marshal(sox::Pack &p) const 
			{
				CImUserInfoSimpleData::marshal(p);
				p<<m_uBirthDay<<m_uArea<<m_uProvince<<m_uCity<<m_uJiFen<<m_strSign<<m_strIntro;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				CImUserInfoSimpleData::unmarshal(p);
				p>>m_uBirthDay>>m_uArea>>m_uProvince>>m_uCity>>m_uJiFen>>m_strSign>>m_strIntro;
			}
		};


		struct PCS_GetUInfoAllById : public sox::Marshallable 
		{
			enum {uri = (1 << 8) | protocol::IMUINFO_SVID};
			PCS_GetUInfoAllById(){}
			
			uint32_t m_uBid;//要查找的UID

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uBid;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uBid;
			}
		};

		struct PCS_GetUInfoAllByIdRes : public PCS_CommonHead
		{
			enum {uri = (2 << 8) | protocol::IMUINFO_SVID};
			PCS_GetUInfoAllByIdRes(){}

			CImUserInfoAllData m_objUInfo;

			virtual void marshal(sox::Pack &p) const
			{
				PCS_CommonHead::marshal(p);
				m_objUInfo.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				m_objUInfo.unmarshal(p);

			}
		};

		

		struct PCS_GetUInfoAllByList : public sox::Marshallable 
		{
			enum {uri = (3 << 8) | protocol::IMUINFO_SVID};
			PCS_GetUInfoAllByList(){}

			vector<uint32_t> m_vecUid;//好友的UID列表

			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_vecUid);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				unmarshal_container(p, std::back_inserter(m_vecUid));
			}
		};

		struct PCS_GetUInfoAllByListRes : public PCS_CommonHead
		{
			enum {uri = (4 << 8) | protocol::IMUINFO_SVID};
			PCS_GetUInfoAllByListRes(){}

			map<uint32_t, CImUserInfoAllData> m_mapUInfo;

			virtual void marshal(sox::Pack &p) const
			{
				PCS_CommonHead::marshal(p);
                p.push_uint32((uint32_t) m_mapUInfo.size());
                for (map<uint32_t, CImUserInfoAllData>::const_iterator i = m_mapUInfo.begin(); i != m_mapUInfo.end(); i++)
                {
                    p << i->first << i->second;
                }
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
                uint32_t uid;
				PCS_CommonHead::unmarshal(p);
                for (uint32_t i = p.pop_uint32(); i > 0; i--)
                {
                    p >> uid;
                    CImUserInfoAllData &data = m_mapUInfo[uid];
                    p >> data;
                }
			}
		};

		struct PCS_GetUInfoByList : public sox::Marshallable 
		{
			enum {uri = (5 << 8) | protocol::IMUINFO_SVID};
			PCS_GetUInfoByList(){}

			vector<uint32_t> m_vecUid;//UID列表
			EImUserInfoType m_eInfoType;


			virtual void marshal(sox::Pack &p) const
			{
				p.push_uint8(m_eInfoType);
				marshal_container(p, m_vecUid);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				m_eInfoType =(protocol::im::EImUserInfoType) p.pop_uint8();
				unmarshal_container(p, std::back_inserter(m_vecUid));
			}
		};

		struct PCS_GetUInfoSimpleByListRes : public PCS_CommonHead
		{
			enum {uri = (6 << 8) | protocol::IMUINFO_SVID};
			PCS_GetUInfoSimpleByListRes(){}

			map<uint32_t, CImUserInfoSimpleData> m_mapUInfo;

			virtual void marshal(sox::Pack &p) const
			{
				PCS_CommonHead::marshal(p);
                p.push_uint32((uint32_t) m_mapUInfo.size());
                for (map<uint32_t, CImUserInfoSimpleData>::const_iterator i = m_mapUInfo.begin(); i != m_mapUInfo.end(); i++)
                {
                    p << i->first << i->second;
                }
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
                uint32_t uid;
				PCS_CommonHead::unmarshal(p);
                for (uint32_t i = p.pop_uint32(); i > 0; i--)
                {
                    p >> uid;
                    CImUserInfoSimpleData &data = m_mapUInfo[uid];
                    p >> data;
                }
			}
		};


		struct PCS_GetUInfoDetailByListRes : public PCS_CommonHead
		{
			enum {uri = (7 << 8) | protocol::IMUINFO_SVID};
			PCS_GetUInfoDetailByListRes(){}

			vector<CImUserInfoDetailData> m_vecUInfo;

			virtual void marshal(sox::Pack &p) const
			{
				PCS_CommonHead::marshal(p);
				marshal_container(p, m_vecUInfo);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				unmarshal_container(p, std::back_inserter(m_vecUInfo));
			}
		};

		//当imuinfo信息改变时候，广播给群内所有成员。
		struct PCS_ImUInfoBeChanged4Group : public sox::Marshallable 
		{
			enum {uri = (8 << 8) | protocol::IMUINFO_SVID};
			PCS_ImUInfoBeChanged4Group(){}

			CImUserInfoDetailData m_objData;

			virtual void marshal(sox::Pack &p) const
			{
				m_objData.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				m_objData.unmarshal(p);
			}
		};

		struct PCS_GetGzipUInfo: public PCS_GetUInfoByList
		{
			enum {uri = (8 << 9) | protocol::IMUINFO_SVID};
		};

		typedef ZipMarshal<PCS_GetUInfoSimpleByListRes, (8 << 10) | protocol::IMUINFO_SVID> PCS_GetGzipUInfoSimpleRes;


	} //end im
}
