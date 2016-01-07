#pragma once

#include <map>
#include <string>
#include <time.h>

#include "core/base_svid.h"
#include "protocol/const.h"
#include "common/protocol/pimcommon.h"
#include "common/protocol/pimdb.h"


namespace server
{
	namespace im
	{		
//imapp和imlist间的通讯协议

		struct PCS_UpdateMyImPresence2  : public sox::Marshallable 
		{
			enum {uri = (17 << 8) | protocol::IMAPP_SVID};
			PCS_UpdateMyImPresence2(){}
		
			PCS_UpdateMyImPresence2(uint32_t uId, protocol::im::E_PRESENCE_TYPE uTypeId, uint32_t uXuhao,const string& strData) 
			{
				m_uXuHao=uXuhao;
				m_uTypeId=uTypeId;
				m_strData=strData;
				m_uId=uId;

			}

			protocol::im::E_PRESENCE_TYPE m_uTypeId;	
			uint32_t m_uXuHao;

			string m_strData;
			uint32_t m_uId;


			virtual void marshal(sox::Pack &p) const
			{
				p << m_uXuHao;
				p.push_uint8(m_uTypeId);
				p<<m_strData;
				p<<m_uId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >>m_uXuHao;
				m_uTypeId=(protocol::im::E_PRESENCE_TYPE)p.pop_uint8();
				p>>m_strData;
				p>>m_uId;
			}
		};



		struct PCS_UpdateMyImPresenceRes2  : public protocol::im::PCS_CommonHead
		{
			enum {uri = (18 << 8) | protocol::IMAPP_SVID};
			PCS_UpdateMyImPresenceRes2(){}
			
			PCS_UpdateMyImPresenceRes2(uint32_t uId, protocol::im::E_PRESENCE_TYPE uTypeId, uint32_t uXuhao,const string& strData) 
			{
				m_uXuHao=uXuhao;
				m_uTypeId=uTypeId;
				m_strData=strData;
				m_uId=uId;

			}

			protocol::im::E_PRESENCE_TYPE m_uTypeId;	
			uint32_t m_uXuHao;

			string m_strData;
			uint32_t m_uId;


			virtual void marshal(sox::Pack &p) const
			{
				protocol::im::PCS_CommonHead::marshal(p);
				p << m_uXuHao;
				p.push_uint8(m_uTypeId);
				p<<m_strData;
				p<<m_uId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				p >>m_uXuHao;
				m_uTypeId=(protocol::im::E_PRESENCE_TYPE)p.pop_uint8();
				p>>m_strData;
				p>>m_uId;

			}
		};



		struct PCS_GetMyGame2 : public sox::Marshallable 
		{

			enum {uri = (19 << 8) | protocol::IMAPP_SVID};
			PCS_GetMyGame2(){};
			uint32_t m_uId;

			virtual void marshal(sox::Pack &p) const
			{
				p<<m_uId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p>>m_uId;

			}



		};

		struct PCS_GetMyGameRes2 : public protocol::im::PCS_CommonHead
		{
			enum {uri = (20<< 8) | protocol::IMAPP_SVID};
			PCS_GetMyGameRes2()
			{
				m_uGameId=-1;
			}
			uint32_t m_uGameId;
			string m_strData;
			uint32_t m_uId;

			virtual void marshal(sox::Pack &p) const 
			{
				protocol::im::PCS_CommonHead::marshal(p);
				p << m_uGameId<<m_strData;
				p<<m_uId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				protocol::im::PCS_CommonHead::unmarshal(p);
				p >> m_uGameId>>m_strData;
				p>>m_uId;
			}

		};


		struct 	PCS_GetBuddyChanelList2 : public sox::Marshallable 
		{
			enum {uri = (21 << 8) | protocol::IMAPP_SVID};
			PCS_GetBuddyChanelList2(){}

			vector<uint32_t> m_vecUid;
			uint32_t m_uId;
			virtual void marshal(sox::Pack &p) const 
			{
				marshal_container(p, m_vecUid);
				p<<m_uId;

			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				unmarshal_container(p, std::back_inserter(m_vecUid));
				p>>m_uId;

			}

		};

		struct 	PCS_GetBuddyChanelListRes2 : public protocol::im::PCS_CommonHead
		{
			enum {uri = (22 << 8) | protocol::IMAPP_SVID};

			PCS_GetBuddyChanelListRes2(){}

			vector<protocol::im::CObjList1> m_vChanel;
			uint32_t m_uId;

			virtual void marshal(sox::Pack &p) const 
			{
				protocol::im::PCS_CommonHead::marshal(p);
				marshal_container(p, m_vChanel);
				p<<m_uId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				protocol::im::PCS_CommonHead::unmarshal(p);
				unmarshal_container(p, std::back_inserter(m_vChanel));
				p>>m_uId;
			}

		};


		struct 	PCS_GetBuddyGameList2 : public sox::Marshallable 
		{
			enum {uri = (23 << 8) | protocol::IMAPP_SVID};
			PCS_GetBuddyGameList2(){};

			vector<uint32_t> m_vecUid;
			uint32_t m_uId;
			virtual void marshal(sox::Pack &p) const 
			{
				marshal_container(p, m_vecUid);
				p<<m_uId;

			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				unmarshal_container(p, std::back_inserter(m_vecUid));
				p>>m_uId;

			}

		};

		struct 	PCS_GetBuddyGameListRes2 : public protocol::im::PCS_CommonHead
		{
			enum {uri = (24 << 8) | protocol::IMAPP_SVID};

			PCS_GetBuddyGameListRes2(){}

			vector<protocol::im::CObjList1> m_vGame;
			uint32_t m_uId;

			virtual void marshal(sox::Pack &p) const 
			{
				protocol::im::PCS_CommonHead::marshal(p);
				marshal_container(p, m_vGame);
				p<<m_uId;

			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				protocol::im::PCS_CommonHead::unmarshal(p);
				unmarshal_container(p, std::back_inserter(m_vGame));
				p>>m_uId;

			}

		};


		struct 	PCS_GetBuddyExtraDataList2 : public sox::Marshallable 
		{
			enum {uri = (25 << 8) | protocol::IMAPP_SVID};
			PCS_GetBuddyExtraDataList2(){};

			vector<uint32_t> m_vecUid;
			uint32_t m_uId;
			virtual void marshal(sox::Pack &p) const 
			{
				marshal_container(p, m_vecUid);
				p<<m_uId;

			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				unmarshal_container(p, std::back_inserter(m_vecUid));
				p>>m_uId;

			}


		};



		struct 	PCS_GetBuddyExtraDataListRes2 : public protocol::im::PCS_CommonHead
		{
			enum {uri = (26 << 8) | protocol::IMAPP_SVID};

			PCS_GetBuddyExtraDataListRes2(){}

			vector<protocol::im::CObjList2> m_vData;
			uint32_t m_uId;

			virtual void marshal(sox::Pack &p) const 
			{
				protocol::im::PCS_CommonHead::marshal(p);
				marshal_container(p, m_vData);
				p<<m_uId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				protocol::im::PCS_CommonHead::unmarshal(p);
				unmarshal_container(p, std::back_inserter(m_vData));
				p>>m_uId;
			}

		};


	
		//add by kdjie 2009.11.10
		struct 	PCS_GetMyHeadPhoto2 : public sox::Marshallable 
		{
			enum {uri = (27 << 8) | protocol::IMAPP_SVID};
			PCS_GetMyHeadPhoto2(){}
			PCS_GetMyHeadPhoto2(uint32_t uId) { m_uId = uId; }

			uint32_t m_uId;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uId;
			}
		};

		struct 	PCS_GetMyHeadPhotoRes2 : public protocol::im::PCS_CommonHead
		{
			enum {uri = (28 << 8) | protocol::IMAPP_SVID};
			PCS_GetMyHeadPhotoRes2(){}
			PCS_GetMyHeadPhotoRes2(uint32_t uId) { m_uId = uId; }

			uint32_t m_uId;
			uint32_t m_uHeadPhoto;
			std::string m_strCustomHeadPhotoPath;

			virtual void marshal(sox::Pack &p) const 
			{
				protocol::im::PCS_CommonHead::marshal(p);
				p << m_uId << m_uHeadPhoto << m_strCustomHeadPhotoPath;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				protocol::im::PCS_CommonHead::unmarshal(p);
				p>> m_uId >> m_uHeadPhoto >> m_strCustomHeadPhotoPath;
			}
		};

		struct 	PCS_GetBuddyHeadPhotoList2 : public sox::Marshallable 
		{
			enum {uri = (29 << 8) | protocol::IMAPP_SVID};
			PCS_GetBuddyHeadPhotoList2(){}

			vector<uint32_t> m_vuId;
			uint32_t m_uId;

			virtual void marshal(sox::Pack &p) const 
			{
				marshal_container(p, m_vuId);
				p<<m_uId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				unmarshal_container(p, std::back_inserter(m_vuId));
				p>>m_uId;
			}
		};

		struct 	PCS_GetBuddyHeadPhotoListRes2 : public sox::Marshallable 
		{
			enum {uri = (30 << 8) | protocol::IMAPP_SVID};
			PCS_GetBuddyHeadPhotoListRes2(){}

			uint32_t m_uId;
			vector<uint32_t> m_vuId;
			vector<protocol::im::CObjList1> m_vHeadPhoto;
			vector<protocol::im::CObjList2> m_vCustomHeadPhotoPath;
			uint32_t resCode;

			virtual void marshal(sox::Pack &p) const 
			{
				p << resCode;
				marshal_container(p, m_vuId);
				marshal_container(p, m_vHeadPhoto);
				marshal_container(p, m_vCustomHeadPhotoPath);
				p<<m_uId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> resCode;
				unmarshal_container(p, std::back_inserter(m_vuId));
				unmarshal_container(p, std::back_inserter(m_vHeadPhoto));
				unmarshal_container(p, std::back_inserter(m_vCustomHeadPhotoPath));
				p>>m_uId;
			}
		};

	//end by kdjie 2009.11.10

		struct 	PCS_GetUserInfo2 : public sox::Marshallable 
		{
			enum {uri = (31 << 8) | protocol::IMAPP_SVID};

			PCS_GetUserInfo2(){}
			PCS_GetUserInfo2(const protocol::im::SUserinfo1& objData)
			{
				m_objUinfo=objData;
			}

			protocol::im::SUserinfo1 m_objUinfo;
			uint32_t m_uId;

			virtual void marshal(sox::Pack &p) const 
			{
				
				p << m_objUinfo<<m_uId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				
				p >> m_objUinfo>>m_uId;
			}

		};



		struct 	PCS_GetUserInfoRes2 : public protocol::im::PCS_CommonHead
		{
			enum {uri = (32 << 8) | protocol::IMAPP_SVID};

			PCS_GetUserInfoRes2(){}
			PCS_GetUserInfoRes2(const protocol::im::SUserinfo1& objData)
			{
				m_objUinfo=objData;
			}

			protocol::im::SUserinfo1 m_objUinfo;
			uint32_t m_uId;
			

			virtual void marshal(sox::Pack &p) const 
			{
				protocol::im::PCS_CommonHead::marshal(p);
				p << m_objUinfo<<m_uId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				protocol::im::PCS_CommonHead::unmarshal(p);
				p >> m_objUinfo>>m_uId;
			}

		};




		//add by kdjie, 2010.5.14

		//用户的头像改变了，需要通知imcache进行更新
		struct PSS_UpdateUInfoPhoto : public sox::Marshallable 
		{
			enum {uri = (33 << 8) | protocol::IMAPP_SVID};

			uint32_t m_uUid;
			uint32_t m_uPhotoIndex; //系统头像编号
			std::string m_strPhotoCustom; //用户自定义头像

			PSS_UpdateUInfoPhoto() {}
			PSS_UpdateUInfoPhoto(uint32_t uUid, uint32_t uPhotoIndex) : m_uUid(uUid), m_uPhotoIndex(uPhotoIndex) {}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uUid << m_uPhotoIndex << m_strPhotoCustom;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uUid >> m_uPhotoIndex >> m_strPhotoCustom;
			}
		};

		struct PSS_GetUserInfo3 : public protocol::im::PCS_CommonHead
		{
			enum {uri = (34 << 8) | protocol::IMAPP_SVID};

			uint32_t m_uImlinkdId;
			uint32_t m_uUid;
			protocol::im::SUserinfo1 m_objUinfo;

			PSS_GetUserInfo3() {}
			PSS_GetUserInfo3(uint32_t uImlinkdId, uint32_t uUid, const protocol::im::SUserinfo1 &objUinfo) :
				m_uImlinkdId(uImlinkdId), m_uUid(uUid), m_objUinfo(objUinfo) {}

			virtual void marshal(sox::Pack &p) const 
			{
				protocol::im::PCS_CommonHead::marshal(p);
				p << m_uImlinkdId << m_uUid << m_objUinfo;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				protocol::im::PCS_CommonHead::unmarshal(p);
				p >> m_uImlinkdId >> m_uUid >> m_objUinfo;
			}
		};


		struct PSS_GetUserInfo16x9 : public protocol::im::PCS_CommonHead
		{
			enum {uri = (40 << 8) | protocol::IMAPP_SVID};

			uint32_t m_uImlinkdId;
			uint32_t m_uUid;
			protocol::im::SUserinfo1 m_objUinfo;

			PSS_GetUserInfo16x9() {}
			PSS_GetUserInfo16x9(uint32_t uImlinkdId, uint32_t uUid, const protocol::im::SUserinfo1 &objUinfo) :
			m_uImlinkdId(uImlinkdId), m_uUid(uUid), m_objUinfo(objUinfo) {}

			virtual void marshal(sox::Pack &p) const 
			{
				protocol::im::PCS_CommonHead::marshal(p);
				p << m_uImlinkdId << m_uUid << m_objUinfo;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				protocol::im::PCS_CommonHead::unmarshal(p);
				p >> m_uImlinkdId >> m_uUid >> m_objUinfo;
			}
		};

		//add end
	}
}


