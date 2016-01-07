 #pragma once

#include <vector>
#include <string>
#include <map>

#include "iproperty.h"
#include "protocol/const.h"
#include "core/base_svid.h"
//#include "./pverifycodepng.h"
#include "common/protocol/IGmemberInfo.h"
#include "PGCheck.h"
#include "pimcommon.h"
#include "zip_tpl.h"

using namespace std;

namespace protocol
{
	namespace gmemberinfo
	{
		struct CGMemberInfoSimpleData:public sox::Marshallable 
		{
			uint32_t m_uGid;//群ID
			uint32_t m_uId;//用户ID
			string m_strNickNameInGroup;
			protocol::uinfo::SEX m_eSex;
			CGMemberInfoSimpleData()
			{
				m_uGid=-1;
				m_uId=-1;
				m_eSex=protocol::uinfo::female;
			}

			CGMemberInfoSimpleData(uint32_t uId,uint32_t uGid,const string& strNickName,protocol::uinfo::SEX eSex)
			{
				m_uGid=uGid;
				m_uId=uId;
				m_strNickNameInGroup=strNickName;
				m_eSex=eSex;
			}


			virtual void marshal(sox::Pack &p) const 
			{
				p<<m_uGid<<m_uId<<m_strNickNameInGroup;
				p.push_uint8(m_eSex);
			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p>>m_uGid>>m_uId>>m_strNickNameInGroup;
				m_eSex=(protocol::uinfo::SEX)p.pop_uint8();
			}
		};


		struct CGMemberInfoDetailData:public CGMemberInfoSimpleData 
		{

			CGMemberInfoDetailData()
			{
				m_bIsAssist=false;
			}

			CGMemberInfoDetailData(uint32_t uId,uint32_t uGid,const string& strNickName,const string& strTel,
				const string& strEmail,const string& strAddress,const string& strRemark,bool bIsAsist,protocol::uinfo::SEX eSex)
				:CGMemberInfoSimpleData(uId,uGid,strNickName,eSex)
			{
				m_strTel=strTel;
				m_strEmail=strEmail;
				m_strAddress=strAddress;
				m_strRemark=strRemark;
				m_bIsAssist=bIsAsist;
			}

			string m_strTel;
			string m_strEmail;
			string m_strAddress;
			string m_strRemark;
			bool m_bIsAssist;//true 代表允许管理员协助修改群名片

			virtual void marshal(sox::Pack &p) const 
			{
				CGMemberInfoSimpleData::marshal(p);
				p<<m_strTel<<m_strEmail<<m_strAddress<<m_strRemark<<m_bIsAssist;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				CGMemberInfoSimpleData::unmarshal(p);
				p>>m_strTel>>m_strEmail>>m_strAddress>>m_strRemark>>m_bIsAssist;
			}
		};




		struct CGMemberInfoData:public sox::Marshallable 
		{

		public:
			CGMemberInfoData(uint32_t uGid,uint32_t uId,	const std::string	&strNickNameInGroup,bool bSex,
				const std::string &strTel,const std::string &strEmail,const std::string &strAddress,	const std::string &strRemark,
				bool bIsAssist)
			{
				SetData(uGid,uId,strNickNameInGroup,bSex,strTel,strEmail,strAddress,strRemark,bIsAssist);

			}

			void SetData(uint32_t uGid,uint32_t uId,	const std::string	&strNickNameInGroup,bool bSex,
				const std::string &strTel,const std::string &strEmail,const std::string &strAddress,	const std::string &strRemark,
				bool bIsAssist)
			{
				
				m_uGid=uGid;
				m_uId=uId;
				m_strNickNameInGroup=strNickNameInGroup;
				m_bSex=bSex;
				//
				m_strTel=strTel;
				m_strEmail=strEmail;
				m_strAddress=strAddress;
				m_strRemark=strRemark;
				m_bIsAssist=bIsAssist;
			}

			CGMemberInfoData()
			{
				m_uGid=-1;
				m_uId=-1;
				m_bSex=false;
				m_bIsAssist=false;

			}

			uint32_t m_uGid;//群ID
			uint32_t m_uId;//用户ID
			std::string	m_strNickNameInGroup;
			bool m_bSex;//true 代表男，false代表女
			std::string m_strTel;
			std::string m_strEmail;
			std::string m_strAddress;
			std::string m_strRemark;
			bool m_bIsAssist;//true 代表允许管理员协助修改群名片


		public:
			virtual void marshal(sox::Pack &p) const 
			{
				p<<m_uGid<<m_uId<<m_strNickNameInGroup<<m_bSex<<m_strTel<<m_strEmail<<m_strAddress<<m_strRemark<<m_bIsAssist;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p>>m_uGid>>m_uId>>m_strNickNameInGroup>>m_bSex>>m_strTel>>m_strEmail>>m_strAddress>>m_strRemark>>m_bIsAssist;
			}

		};


		//本文件描述的消息定义，是客户端和服务器交互的消息定义


		//取群内某些成员的群名片
		struct PCS_GetGMemberInfo : public sox::Marshallable 
		{
			enum {uri = (1 << 8) | protocol::GMEMBERINFO_SVID};
			PCS_GetGMemberInfo(){};

			map<uint32_t,vector<uint32_t> > m_mapUid;
			EGMemberInfoType m_eInfoType;


			virtual void marshal(sox::Pack &p) const 
			{
				map<uint32_t,vector<uint32_t> >::const_iterator pIt ;
				p<<(uint32_t)m_mapUid.size();

				
				for(pIt=m_mapUid.begin();pIt!=m_mapUid.end();++pIt)
				{
					p<<pIt->first;
					marshal_container(p, pIt->second);
				}
				p.push_uint8(m_eInfoType);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				uint32_t uSize;
				uint32_t uGid;
				vector<uint32_t> vecUid;
				p>>uSize;
				for (uint32_t i=0;i<uSize;i++)
				{
					vecUid.clear();
					p>>uGid;
					unmarshal_container(p, std::back_inserter(vecUid));
					m_mapUid[uGid]=vecUid;
				}
				m_eInfoType =(EGMemberInfoType) p.pop_uint8();
			}

		};

		struct PCS_GetGMemberInfoSimpleRes : public protocol::im::PCS_CommonHead 
		{
			enum {uri = (2 << 8) | protocol::GMEMBERINFO_SVID};
			PCS_GetGMemberInfoSimpleRes(){};

			vector<CGMemberInfoSimpleData> m_vecData;//要看的人
			uint32_t m_uGid;//他的群ID

			virtual void marshal(sox::Pack &p) const 
			{
				protocol::im::PCS_CommonHead::marshal(p);
				p<<m_uGid;
				marshal_container(p, m_vecData);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				protocol::im::PCS_CommonHead::unmarshal(p);
				p>>m_uGid;
				unmarshal_container(p, std::back_inserter(m_vecData));
			}

		};


		struct PCS_GetGMemberInfoDetailRes : public protocol::im::PCS_CommonHead 
		{
			enum {uri = (3 << 8) | protocol::GMEMBERINFO_SVID};
			PCS_GetGMemberInfoDetailRes(){};

			vector<CGMemberInfoDetailData> m_vecData;//要看的人
			uint32_t m_uGid;//他的群ID

			virtual void marshal(sox::Pack &p) const 
			{
				protocol::im::PCS_CommonHead::marshal(p);
				p<<m_uGid;
				marshal_container(p, m_vecData);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				protocol::im::PCS_CommonHead::unmarshal(p);
				p>>m_uGid;
				unmarshal_container(p, std::back_inserter(m_vecData));
			}

		};



		//PCS_GetGMemberInfo的响应
		struct PCS_GetGMemberInfoRes : public protocol::im::PCS_CommonHead 
		{
			enum {uri = (4 << 8) | protocol::GMEMBERINFO_SVID};
			PCS_GetGMemberInfoRes(){};

			vector<CGMemberInfoData> m_vecData;//要看的人
			uint32_t m_uGid;//他的群ID

			virtual void marshal(sox::Pack &p) const 
			{
				protocol::im::PCS_CommonHead::marshal(p);
				p<<m_uGid;
				marshal_container(p, m_vecData);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				protocol::im::PCS_CommonHead::unmarshal(p);
				p>>m_uGid;
				unmarshal_container(p, std::back_inserter(m_vecData));
			}

		};

		//修改自己群名片或帮群成员修改群名片
		struct PCS_SetGMemberInfo : public protocol::gcheck::PCS_GroupCommon
		{
			enum {uri = (5 << 8) | protocol::GMEMBERINFO_SVID};
			PCS_SetGMemberInfo(){};

			
			CGMemberInfoDetailData m_objMemberInfo;
			
			virtual void marshal(sox::Pack &p) const 
			{
				protocol::gcheck::PCS_GroupCommon::marshal(p);
				m_objMemberInfo.marshal(p);
			
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				protocol::gcheck::PCS_GroupCommon::unmarshal(p);
				m_objMemberInfo.unmarshal(p);
			}

		};
//没有使用阿龙类之前
// 		struct PCS_SetGMemberInfo : public sox::Marshallable 
// 		{
// 			enum {uri = (5 << 8) | protocol::GMEMBERINFO_SVID};
// 			PCS_SetGMemberInfo(){};
// 			
// 			//CGMemberInfoData m_objMemberInfo;
// 			CGMemberInfoDetailData m_objMemberInfo;
// 			string m_strSignature;//ginfo算出的签名
// 			uint32_t m_uPrivilege;//修改者的权限
// 			
// 			virtual void marshal(sox::Pack &p) const 
// 			{
// 				m_objMemberInfo.marshal(p);
// 				p<<m_strSignature<<m_uPrivilege;
// 			}
// 
// 			virtual void unmarshal(const sox::Unpack &p) 
// 			{
// 				m_objMemberInfo.unmarshal(p);
// 				p>>m_strSignature>>m_uPrivilege;
// 			}
// 
// 		};

		//PCS_SetGMemberInfo的响应
		struct PCS_SetGMemberInfoRes : public protocol::im::PCS_CommonHead 
		{
			enum {uri = (6 << 8) | protocol::GMEMBERINFO_SVID};
			PCS_SetGMemberInfoRes(){};

			//CGMemberInfoData m_objMemberInfo;
			CGMemberInfoDetailData m_objMemberInfo;
			virtual void marshal(sox::Pack &p) const 
			{
				protocol::im::PCS_CommonHead::marshal(p);
				m_objMemberInfo.marshal(p);
			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
				protocol::im::PCS_CommonHead::unmarshal(p);
				m_objMemberInfo.unmarshal(p);
			}

		};


		//广播群名片修改
		struct PCS_BrocastGMemberInfo : public sox::Marshallable 
		{
			enum {uri = (7 << 8) | protocol::GMEMBERINFO_SVID};
			PCS_BrocastGMemberInfo(){};

			CGMemberInfoDetailData m_objMemberInfo;
			
			virtual void marshal(sox::Pack &p) const 
			{
				m_objMemberInfo.marshal(p);
			
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				m_objMemberInfo.unmarshal(p);
			}

		};


		struct PCS_DelGMemberInfo : public protocol::gcheck::PCS_GroupCommon
		{
			enum {uri = (8 << 8) | protocol::GMEMBERINFO_SVID};
			PCS_DelGMemberInfo(){};

			uint32_t m_uId;
			uint32_t m_uGid;

			virtual void marshal(sox::Pack &p) const 
			{
				protocol::gcheck::PCS_GroupCommon::marshal(p);
				p<<m_uId<<m_uGid;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				protocol::gcheck::PCS_GroupCommon::unmarshal(p);
				p>>m_uId>>m_uGid;
			}

		};


		struct PCS_DelGMemberInfoAll : public protocol::gcheck::PCS_GroupCommon
		{
			enum {uri = (9 << 8) | protocol::GMEMBERINFO_SVID};
			PCS_DelGMemberInfoAll(){};

			uint32_t m_uGid;

			virtual void marshal(sox::Pack &p) const 
			{
				protocol::gcheck::PCS_GroupCommon::marshal(p);
				p<<m_uGid;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				protocol::gcheck::PCS_GroupCommon::unmarshal(p);
				p>>m_uGid;
			}

		};
		
		/*
		//将结构体转换成字符串
		inline void Packet2String(const sox::Marshallable &objMarshal, std::string &strPacket)
		{
			//将原包序列化，并装载
			sox::PackBuffer pb;
			sox::Pack pk(pb);
			objMarshal.marshal(pk);

			strPacket.assign(pk.data(), pk.size());
		}*/
		
		struct PSS_GFillMemberInfo : public sox::Marshallable 
		{
			enum {uri = (10 << 8) | protocol::GMEMBERINFO_SVID};
			PSS_GFillMemberInfo(){};
   
            uint32_t m_gid;
			vector<uint32_t> m_vecUid;
			EGMemberInfoType m_eInfoType;
			//uint32_t m_uOriUri; //原包uri，解析并派发原包时使用
			//std::string m_strOriPacket; //原包数据
			
			//PSS_GFillMemberInfo(uint32_t Uri, const sox::Marshallable &objMarshal) 
			//{
			//	m_uOriUri = Uri;
			//	Packet2String(objMarshal, m_strOriPacket);
			//}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_gid;
				sox::marshal_container(p, m_vecUid);
				//p << m_uOriUri;
				//p << m_strOriPacket;
				p.push_uint8(m_eInfoType);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_gid;
				sox::unmarshal_container(p, back_inserter(m_vecUid));
				//p >> m_uOriUri;
				//p >> m_strOriPacket;
				m_eInfoType =(EGMemberInfoType) p.pop_uint8();
			}

		};

		struct PSS_GUpdateMemberInfo : public protocol::im::PCS_CommonHead 
		{
			enum {uri = (11 << 8) | protocol::GMEMBERINFO_SVID};
			PSS_GUpdateMemberInfo(){};
            uint32_t index;  //0 for change; 1 for delete specific info in gid;2 for delete all info in gid;  
			uint32_t m_uGid; 
			uint32_t m_uUid; 
			CGMemberInfoDetailData m_objMemberInfo;//要更新资料的人
			

			virtual void marshal(sox::Pack &p) const 
			{
				protocol::im::PCS_CommonHead::marshal(p);
				p << index;
				p << m_uGid;
				p << m_uUid;
				m_objMemberInfo.marshal(p);
				
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				protocol::im::PCS_CommonHead::unmarshal(p);
				p >> index;
				p >> m_uGid;
				p >> m_uUid;
				m_objMemberInfo.unmarshal(p);
			}

		};

		struct PSS_DelGMemberInfo : public protocol::gcheck::PCS_GroupCommon
		{
			enum {uri = (12 << 8) | protocol::GMEMBERINFO_SVID};
			PSS_DelGMemberInfo(){};

			uint32_t m_uId;
			uint32_t m_uGid;

			virtual void marshal(sox::Pack &p) const 
			{
				protocol::gcheck::PCS_GroupCommon::marshal(p);
				p<<m_uId<<m_uGid;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				protocol::gcheck::PCS_GroupCommon::unmarshal(p);
				p>>m_uId>>m_uGid;
			}

		};


		struct PSS_DelGMemberInfoAll : public protocol::gcheck::PCS_GroupCommon
		{
			enum {uri = (13 << 8) | protocol::GMEMBERINFO_SVID};
			PSS_DelGMemberInfoAll(){};

			uint32_t m_uGid;

			virtual void marshal(sox::Pack &p) const 
			{
				protocol::gcheck::PCS_GroupCommon::marshal(p);
				p<<m_uGid;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				protocol::gcheck::PCS_GroupCommon::unmarshal(p);
				p>>m_uGid;
			}

		};

		struct PSS_GFillMemberInfoRes : public protocol::im::PCS_CommonHead 
		{
			enum {uri = (1 << 8) | protocol::GMEMBERINFOCACHE_SVID};
			PSS_GFillMemberInfoRes(){};

			//PSS_GFillMemberInfoRes(uint32_t Uri, const sox::Marshallable &objMarshal) 
			//{
			//	m_uOriUri = Uri;
			//	Packet2String(objMarshal, m_strOriPacket);
			//}

			uint32_t m_uGid;//他的群ID
			vector<CGMemberInfoDetailData> m_vecData;//要看的人
			EGMemberInfoType m_eInfoType;
			//uint32_t m_uOriUri;          //原包uri，解析并派发原包时使用
			//std::string m_strOriPacket; //原包数据

			virtual void marshal(sox::Pack &p) const 
			{
				protocol::im::PCS_CommonHead::marshal(p);
				p << m_uGid;
				marshal_container(p, m_vecData);
				//p << m_uOriUri;
				//p << m_strOriPacket;
				p.push_uint8(m_eInfoType);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				protocol::im::PCS_CommonHead::unmarshal(p);
				p>> m_uGid;
				unmarshal_container(p, std::back_inserter(m_vecData));
				//p >> m_uOriUri;
				//p >> m_strOriPacket;
				m_eInfoType =(EGMemberInfoType) p.pop_uint8();
			}
		};

		typedef ZipMarshal<PCS_GetGMemberInfoSimpleRes, (14 << 8) | protocol::GMEMBERINFO_SVID> PCS_GetGzipGMemberInfoSimpleRes;

	}
}
