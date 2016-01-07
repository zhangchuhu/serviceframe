 #pragma once

#include <vector>
#include <string>

#include "iproperty.h"
#include "protocol/const.h"
#include "core/base_svid.h"
#include "./pverifycodepng.h"
#include "common/protocol/iImV2.h"
#include "common/protocol/IImHeadPhoto.h"
#include "pimcommon.h"
#include "common/protocol/pimdb.h"

using namespace std;

//本文件描述的消息定义，是客户端和服务器交互的消息定义

namespace protocol
{
	namespace im
	{

		struct CImHeadPhotoInfoData: public sox::Marshallable 
		{
		public:
			CImHeadPhotoInfoData()
			{
				m_uId=-1;
				m_uHeadPhoto=-1;
			}

			CImHeadPhotoInfoData(uint32_t uId,uint32_t uHeadPhoto,const std::string &strCustomHeadPhotoPath)
			{
				m_uId=uId;
				m_uHeadPhoto=uHeadPhoto;
				m_strCustomHeadPhotoPath=strCustomHeadPhotoPath;
			}

			uint32_t m_uId;
			uint32_t m_uHeadPhoto;//头像
			std::string m_strCustomHeadPhotoPath;//自定义头像路径

			virtual void marshal(sox::Pack &p) const
			{
				p<<m_uId<<m_uHeadPhoto<<m_strCustomHeadPhotoPath;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p>>m_uId>>m_uHeadPhoto>>m_strCustomHeadPhotoPath;
			}
		};

		//群取头像列表结构
		struct CImHeadPhotoInfoData2: public sox::Marshallable 
		{
		public:
			CImHeadPhotoInfoData2()
			{
				m_uId=-1;
				m_uHeadPhoto=-1;
			}

			CImHeadPhotoInfoData2(uint32_t uId,uint32_t uHeadPhoto)
			{
				m_uId=uId;
				m_uHeadPhoto=uHeadPhoto;
			}

			uint32_t m_uId;
			uint32_t m_uHeadPhoto;//头像

			virtual void marshal(sox::Pack &p) const
			{
				p<<m_uId<<m_uHeadPhoto;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p>>m_uId>>m_uHeadPhoto;
			}
		};

		//客户端发送过来更新自己的信息（包括所在频道，所在游戏，头像），服务端收到该消息后响应PCS_UpdateMyImPresenceRes1结果
		struct PCS_UpdateMyImPresence1  : public sox::Marshallable 
		{
			enum {uri = (1 << 8) | protocol::IMAPP_SVID};
			PCS_UpdateMyImPresence1(){}
			PCS_UpdateMyImPresence1(E_PRESENCE_TYPE uTypeId,uint32_t uXuhao) 
			{
				m_uXuHao=uXuhao;
				m_uTypeId=uTypeId;

			}

			PCS_UpdateMyImPresence1(E_PRESENCE_TYPE uTypeId,uint32_t uXuhao,const string& strData) 
			{
				m_uXuHao=uXuhao;
				m_uTypeId=uTypeId;
				m_strData=strData;

			}

			E_PRESENCE_TYPE m_uTypeId;	//类别（头像，频道，游戏）
			uint32_t m_uXuHao;//类别下的序号
			string m_strData;//附加信息


			virtual void marshal(sox::Pack &p) const
			{
				p << m_uXuHao;
				p.push_uint8(m_uTypeId);
				p<<m_strData;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >>m_uXuHao;
				m_uTypeId=(E_PRESENCE_TYPE)p.pop_uint8();
				p>>m_strData;
			}
		};


		//对PCS_UpdateMyImPresence1的响应
		struct PCS_UpdateMyImPresenceRes1  : public PCS_CommonHead
		{
			enum {uri = (2 << 8) | protocol::IMAPP_SVID};
			PCS_UpdateMyImPresenceRes1(){}
			PCS_UpdateMyImPresenceRes1(E_PRESENCE_TYPE uTypeId,uint32_t uXuhao) 
			{
				m_uXuHao=uXuhao;
				m_uTypeId=uTypeId;

			}

			PCS_UpdateMyImPresenceRes1(E_PRESENCE_TYPE uTypeId,uint32_t uXuhao,const string& strData) 
			{
				m_uXuHao=uXuhao;
				m_uTypeId=uTypeId;
				m_strData=strData;

			}

			E_PRESENCE_TYPE m_uTypeId;	//类别（头像，频道，游戏）
			uint32_t m_uXuHao;////类别下的序号
			string m_strData;//附加信息


			virtual void marshal(sox::Pack &p) const
			{
				PCS_CommonHead::marshal(p);
				p << m_uXuHao;
				p.push_uint8(m_uTypeId);
				p<<m_strData;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				p >>m_uXuHao;
				m_uTypeId=(E_PRESENCE_TYPE)p.pop_uint8();
				p>>m_strData;

			}
		};

		
		//客户端发出的消息。服务端收到该消息后取回多个好友的相关信息，响应PCS_GetBuddyGameListRes1结果。
		struct 	PCS_GetBuddyGameList1 : public protocol::im::CUidSet
		{
			enum {uri = (3 << 8) | protocol::IMAPP_SVID};
			PCS_GetBuddyGameList1(){};
		};

		//对PCS_GetBuddyGameList1的响应
		struct 	PCS_GetBuddyGameListRes1 : public PCS_CommonHead
		{
			enum {uri = (4 << 8) | protocol::IMAPP_SVID};

			PCS_GetBuddyGameListRes1(){}

			vector<CObjList1> m_vGame;//好友所在游戏列表

			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				marshal_container(p, m_vGame);

			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				unmarshal_container(p, std::back_inserter(m_vGame));

			}

		};


		//客户端发出的用于获得自己的游戏数据。服务端收到该消息后响应PCS_GetMyGameRes1结果。
		struct PCS_GetMyGame1 : public sox::Voidmable		
		{

			enum {uri = (5 << 8) | protocol::IMAPP_SVID};
						
			

		};

		struct PCS_GetMyGameRes1 : public PCS_CommonHead
		{
			enum {uri = (6<< 8) | protocol::IMAPP_SVID};
			PCS_GetMyGameRes1()
			{
				m_uGameId=-1;
			}
			uint32_t m_uGameId;//游戏ID
			string m_strData;//游戏的附加数据

			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				p << m_uGameId<<m_strData;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				p >> m_uGameId>>m_strData;
			}



		};

		//客户端发出的。服务端收到该消息后取回多个好友的频道相关信息，响应PCS_GetBuddyChanelListRes1结果。
		struct 	PCS_GetBuddyChanelList1 : public protocol::im::CUidSet
		{
			enum {uri = (7 << 8) | protocol::IMAPP_SVID};
			PCS_GetBuddyChanelList1(){}
		};

		//对PCS_GetBuddyChanelList1的响应
		struct 	PCS_GetBuddyChanelListRes1 : public PCS_CommonHead
		{
			enum {uri = (8 << 8) | protocol::IMAPP_SVID};

			PCS_GetBuddyChanelListRes1(){}

			vector<CObjList1> m_vChanel;//好友所在频道列表

			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				marshal_container(p, m_vChanel);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				unmarshal_container(p, std::back_inserter(m_vChanel));
			}

		};

		//客户端发出的。取某个人所在频道，响应PCS_GetBuddyChanelRes1结果。
		struct PCS_GetBuddyChanel1: public sox::Marshallable 
		{
			enum {uri = (9 << 8) | protocol::IMAPP_SVID};
			PCS_GetBuddyChanel1(){}
			uint32_t m_uId;//某人的UID
			virtual void marshal(sox::Pack &p) const 
			{
				p<<m_uId;
			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p>>m_uId;	
			}

		};

		//对PCS_GetBuddyChanel1的响应
		struct 	PCS_GetBuddyChanelRes1 : public PCS_CommonHead
		{
			enum {uri = (10 << 8) | protocol::IMAPP_SVID};
			PCS_GetBuddyChanelRes1(){}
			CObjList1 m_objData;//频道数据
			virtual void marshal(sox::Pack &p) const 
			{
				m_objData.marshal(p);
			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
				m_objData.unmarshal(p);
			}

		};


		//客户端发出的。取某个人所在游戏，
		struct PCS_GetBuddyGame1: public sox::Marshallable 
		{
			enum {uri = (11 << 8) | protocol::IMAPP_SVID};
			PCS_GetBuddyGame1(){}
			uint32_t m_uId;//某人的UID
			virtual void marshal(sox::Pack &p) const 
			{
				p<<m_uId;
			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p>>m_uId;	
			}

		};

		//对PCS_GetBuddyGame1的响应
		struct 	PCS_GetBuddyGameRes1 : public PCS_CommonHead
		{
			enum {uri = (12 << 8) | protocol::IMAPP_SVID};
			PCS_GetBuddyGameRes1(){}
			CObjList1 m_objData;//游戏数据
			virtual void marshal(sox::Pack &p) const 
			{
				m_objData.marshal(p);
			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
				m_objData.unmarshal(p);
			}

		};

		///////////////////////////////////////////////////////////////////////////////////////
		//add by kdjie 2009.11.10

		//客户端发出的。取自己头像，响应PCS_GetMyHeadPhotoRes1结果。
		struct 	PCS_GetMyHeadPhoto1 : public sox::Marshallable 
		{
			enum {uri = (13 << 8) | protocol::IMAPP_SVID};
			PCS_GetMyHeadPhoto1(){}

			virtual void marshal(sox::Pack &p) const 
			{
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
			}
		};

		//对PCS_GetMyHeadPhoto1的响应
		struct 	PCS_GetMyHeadPhotoRes1 : public PCS_CommonHead
		{
			enum {uri = (14 << 8) | protocol::IMAPP_SVID};
			PCS_GetMyHeadPhotoRes1() {}

			uint32_t m_uHeadPhoto;//头像ID
			std::string m_strCustomHeadPhotoPath;//如果是自定义头像，这个是URL

			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				p << m_uHeadPhoto << m_strCustomHeadPhotoPath;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				p >> m_uHeadPhoto >> m_strCustomHeadPhotoPath;
			}
		};

		//客户端发出的。服务端收到该消息后取回多个好友的头像相关信息，响应PCS_GetBuddyHeadPhotoListRes1结果。
		struct 	PCS_GetBuddyHeadPhotoList1 : public protocol::im::CUidSet 
		{
			enum {uri = (15 << 8) | protocol::IMAPP_SVID};
			PCS_GetBuddyHeadPhotoList1(){}
		};

		struct 	PCS_GetBuddyHeadPhotoListRes1 : public sox::Marshallable 
		{
			enum {uri = (16 << 8) | protocol::IMAPP_SVID};
			PCS_GetBuddyHeadPhotoListRes1(){}

			vector<uint32_t> m_vuId;//好友UID列表
			vector<CObjList1> m_vHeadPhoto;//好友头像列表
			vector<CObjList2> m_vCustomHeadPhotoPath;//好友的自定义头像列表
			uint32_t resCode;

			virtual void marshal(sox::Pack &p) const 
			{
				p << resCode;
				marshal_container(p, m_vuId);
				marshal_container(p, m_vHeadPhoto);
				marshal_container(p, m_vCustomHeadPhotoPath);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> resCode;
				unmarshal_container(p, std::back_inserter(m_vuId));
				unmarshal_container(p, std::back_inserter(m_vHeadPhoto));
				unmarshal_container(p, std::back_inserter(m_vCustomHeadPhotoPath));
			}
		};


		struct PCS_GetBuddyHeadPhoto1 : public sox::Marshallable
		{
			enum {uri = (33 << 8) | protocol::IMAPP_SVID};//消息序号要留意protocol目录里面的pimapp.h的序号，免得冲突
			PCS_GetBuddyHeadPhoto1(){}

			uint32_t m_Bid;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_Bid;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_Bid;
			}
		};

		struct PCS_GetBuddyHeadPhotoRes1 : public sox::Marshallable
		{
			enum {uri = (34 << 8) | protocol::IMAPP_SVID};
			PCS_GetBuddyHeadPhotoRes1(){}

			uint32_t m_Bid;
			uint32_t m_uHeadPhoto;
			std::string m_strCustomHeadPhotoPath;
			uint32_t m_uResCode;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uResCode;
				p << m_Bid;
				p << m_uHeadPhoto;
				p << m_strCustomHeadPhotoPath;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uResCode;
				p >> m_Bid;
				p >> m_uHeadPhoto;
				p >> m_strCustomHeadPhotoPath;
			}
		};


		
		struct 	PCS_GetHeadPhotoList4Group : public protocol::im::CUidSet
		{
			enum {uri = (35 << 8) | protocol::IMAPP_SVID};
			PCS_GetHeadPhotoList4Group(){}
		};

		struct 	PCS_GetHeadPhotoList4GroupRes :  public PCS_CommonHead
		{
			enum {uri = (36 << 8) | protocol::IMAPP_SVID};
			PCS_GetHeadPhotoList4GroupRes(){}
			vector<CImHeadPhotoInfoData> m_vecData;

			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				marshal_container(p, m_vecData);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				unmarshal_container(p, std::back_inserter(m_vecData));
			}
		};


		struct 	PCS_ImHeadPhotoBeChanged : public sox::Marshallable 
		{
			enum {uri = (37 << 8) | protocol::IMAPP_SVID};
			PCS_ImHeadPhotoBeChanged(){}

			CImHeadPhotoInfoData m_objData;

			virtual void marshal(sox::Pack &p) const 
			{
				m_objData.marshal(p);
				
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				m_objData.unmarshal(p);
			}
		};

		//取群用户列表头像（自定义头像转为系统默认）
		struct 	PCS_GetHeadPhotoList4Group2 : public protocol::im::CUidSet
		{
			enum {uri = (38 << 8) | protocol::IMAPP_SVID};
			PCS_GetHeadPhotoList4Group2(){}
		};

		struct 	PCS_GetHeadPhotoList4Group2Res :  public PCS_CommonHead
		{
			enum {uri = (39 << 8) | protocol::IMAPP_SVID};

			vector<CImHeadPhotoInfoData2> m_vecData;

			PCS_GetHeadPhotoList4Group2Res(){}

			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				marshal_container(p, m_vecData);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				unmarshal_container(p, std::back_inserter(m_vecData));
			}
		};

		//add end

		struct PCS_GetMyHeadPhoto16x9 : public  sox::Marshallable 
		{
			enum {uri = (100 << 8) | protocol::IMAPP_SVID};
			PCS_GetMyHeadPhoto16x9(){}

			virtual void marshal(sox::Pack &p) const 
			{
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
			}
		};

		struct 	PCS_GetMyHeadPhotoRes16x9 : public PCS_CommonHead
		{
			enum {uri = (101 << 8) | protocol::IMAPP_SVID};
			PCS_GetMyHeadPhotoRes16x9() {}

			uint32_t m_uHeadPhoto;//头像ID
			std::string m_strCustomHeadPhotoPath;//如果是自定义头像，这个是URL

			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				p << m_uHeadPhoto << m_strCustomHeadPhotoPath;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				p >> m_uHeadPhoto >> m_strCustomHeadPhotoPath;
			}
		};

		struct PCS_GetBuddyHeadPhoto16x9 : public sox::Marshallable
		{
			enum {uri = (102 << 8) | protocol::IMAPP_SVID};//消息序号要留意protocol目录里面的pimapp.h的序号，免得冲突
			PCS_GetBuddyHeadPhoto16x9(){}

			uint32_t m_Bid;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_Bid;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_Bid;
			}
		};

		struct PCS_GetBuddyHeadPhotoRes16x9 : public sox::Marshallable
		{
			enum {uri = (103 << 8) | protocol::IMAPP_SVID};
			PCS_GetBuddyHeadPhotoRes16x9(){}

			uint32_t m_Bid;
			uint32_t m_uHeadPhoto;
			std::string m_strCustomHeadPhotoPath;
			uint32_t m_uResCode;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uResCode;
				p << m_Bid;
				p << m_uHeadPhoto;
				p << m_strCustomHeadPhotoPath;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uResCode;
				p >> m_Bid;
				p >> m_uHeadPhoto;
				p >> m_strCustomHeadPhotoPath;
			}
		};

		struct 	PCS_GetBuddyHeadPhotoList16x9 : public protocol::im::CUidSet 
		{
			enum {uri = (104 << 8) | protocol::IMAPP_SVID};
			PCS_GetBuddyHeadPhotoList16x9(){}
		};

		struct 	PCS_GetBuddyHeadPhotoListRes16x9 : public sox::Marshallable 
		{
			enum {uri = (105 << 8) | protocol::IMAPP_SVID};
			PCS_GetBuddyHeadPhotoListRes16x9(){}

			vector<uint32_t> m_vuId;//好友UID列表
			vector<CObjList1> m_vHeadPhoto;//好友头像列表
			vector<CObjList2> m_vCustomHeadPhotoPath;//好友的自定义头像列表
			uint32_t resCode;

			virtual void marshal(sox::Pack &p) const 
			{
				p << resCode;
				marshal_container(p, m_vuId);
				marshal_container(p, m_vHeadPhoto);
				marshal_container(p, m_vCustomHeadPhotoPath);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> resCode;
				unmarshal_container(p, std::back_inserter(m_vuId));
				unmarshal_container(p, std::back_inserter(m_vHeadPhoto));
				unmarshal_container(p, std::back_inserter(m_vCustomHeadPhotoPath));
			}
		};


		struct 	PCS_GetHeadPhotoList4Group16x9 : public protocol::im::CUidSet
		{
			enum {uri = (106 << 8) | protocol::IMAPP_SVID};
			PCS_GetHeadPhotoList4Group16x9(){}
		};

		struct 	PCS_GetHeadPhotoList4Group2Res16x9 :  public PCS_CommonHead
		{
			enum {uri = (107 << 8) | protocol::IMAPP_SVID};

			vector<CImHeadPhotoInfoData2> m_vecData;

			PCS_GetHeadPhotoList4Group2Res16x9(){}

			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				marshal_container(p, m_vecData);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				unmarshal_container(p, std::back_inserter(m_vecData));
			}
		};


		
	} //end im
}
