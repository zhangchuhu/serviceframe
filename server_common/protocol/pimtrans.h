#ifndef SERVER_PIMTRNAS_H_
#define SERVER_PIMTRNAS_H_

#include "iproperty.h"
#include "common/packet.h"
#include "protocol/const.h"
#include "core/base_svid.h"
#include "protocol/common/protocol/pimcommon.h"

using namespace std;

namespace server
{
	namespace im
	{

		//将结构体转换成字符串
		inline void Packet2String(const sox::Marshallable &objMarshal, std::string &strPacket)
		{
			//将原包序列化，并装载
			sox::PackBuffer pb;
			sox::Pack pk(pb);
			objMarshal.marshal(pk);

			strPacket.assign(pk.data(), pk.size());
		}

		//其它服务器请求imtransfer广播的协议
		struct PSS_Broadcast  : public sox::Marshallable 
		{
			enum {uri = (11 << 8) | protocol::IMTRANS_SVID};

			PSS_Broadcast() {};
			PSS_Broadcast(uint32_t uri, const string &strOriPacket) 
			{
				m_ouri = uri;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}
			PSS_Broadcast(uint32_t uri, const sox::Marshallable &objMarshal) 
			{
				m_ouri = uri;
				Packet2String(objMarshal, m_strOriPacket);
			}
			void SetSenderUid(uint32_t uSenderUid)
			{
				m_uSenderUid = uSenderUid;
			}

			uint32_t m_ouri; //原包uri，用于imtrans广播时使用
			uint32_t m_uSenderUid; //发送人用户Id
			string   m_strOriPacket; //原包数据

			virtual void marshal(sox::Pack &p) const
			{
				p << m_ouri;
				p << m_uSenderUid;
				p << m_strOriPacket;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_ouri;
				p >> m_uSenderUid;
				p >> m_strOriPacket;
			}
		};

		//其它服务器请求imtransfer转发的协议
		struct PSS_Forward : public sox::Marshallable
		{
			enum {uri = (12 << 8) | protocol::IMTRANS_SVID};

			PSS_Forward() {};
			PSS_Forward(uint32_t uri, const string &strOriPacket) 
			{
				m_ouri = uri;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}
			void SetSenderUid(uint32_t uSenderUid)
			{
				m_uSenderUid = uSenderUid;
			}
			void SetRecvUidVect(const vector<uint32_t> &vecRecvUid)
			{
				m_vecRecvUid.clear();
				m_vecRecvUid.assign(vecRecvUid.begin(), vecRecvUid.end());
			}
			void SetRecvUid(uint32_t uSenderUid)
			{
				m_vecRecvUid.push_back(uSenderUid);
			}
			void ResetRecvUid()
			{
				m_vecRecvUid.clear();
			}

			uint32_t m_ouri; //原包uri，用于imtrans广播时使用
			uint32_t m_uSenderUid; //发送人用户Id
			vector<uint32_t> m_vecRecvUid;
			string   m_strOriPacket; //原包数据

			virtual void marshal(sox::Pack &p) const
			{
				p << m_ouri;
				p << m_uSenderUid;
				sox::marshal_container(p, m_vecRecvUid);
				p << m_strOriPacket;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_ouri;
				p >> m_uSenderUid;
				sox::unmarshal_container(p, back_inserter(m_vecRecvUid));
				p >> m_strOriPacket;
			}
		};


		//imtransfer向imlist请求填充好友列表
		struct PSS_FillBuddyList  : public sox::Marshallable 
		{
			enum {uri = (100 << 8) | protocol::IMDB_SVID};

			PSS_FillBuddyList() {};
			PSS_FillBuddyList(uint32_t uri, const string &strOriPacket) 
			{
				m_ouri = uri;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}
			void SetSenderUid(uint32_t uSenderUid)
			{
				m_uSenderUid = uSenderUid;
			}

			uint32_t m_ouri; //原包uri
			uint32_t m_uSenderUid; //发送人用户Id
			string   m_strOriPacket; //原包数据

			virtual void marshal(sox::Pack &p) const
			{
				p << m_ouri;
				p << m_uSenderUid;
				p << m_strOriPacket;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_ouri;
				p >> m_uSenderUid;
				p >> m_strOriPacket;
			}
		};

		//imlist向imtransfer返回填充好友列表后的包
		struct PSS_FillBuddyListRes  : public sox::Marshallable 
		{
			enum {uri = (13 << 8) | protocol::IMTRANS_SVID};

			PSS_FillBuddyListRes() {};
			PSS_FillBuddyListRes(uint32_t uri, const string &strOriPacket) 
			{
				m_ouri = uri;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}
			void SetSenderUid(uint32_t uSenderUid)
			{
				m_uSenderUid = uSenderUid;
			}
			void SetResCode(uint32_t uResCode = RES_SUCCESS)
			{
				m_uResCode = uResCode;
			}
			void SetBuddyList(const vector<protocol::im::CBuddyListNoRemark> &vecBuddyList)
			{
				m_vecBuddyList = vecBuddyList;
			}

			uint32_t m_ouri; //原包uri，用于解包时使用
			uint32_t m_uResCode; //响应值

			uint32_t m_uSenderUid; //发送人用户Id
			vector<protocol::im::CBuddyListNoRemark> m_vecBuddyList; //发送人好友列表

			string m_strOriPacket; //原来的包

			virtual void marshal(sox::Pack &p) const
			{
				p << m_ouri;
				p << m_uResCode;
				p << m_uSenderUid;
				sox::marshal_container(p, m_vecBuddyList);
				p << m_strOriPacket;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_ouri;
				p >> m_uResCode;
				p >> m_uSenderUid;
				sox::unmarshal_container(p, back_inserter(m_vecBuddyList));
				p >> m_strOriPacket;
			}
		};

		//imlist向imtransfer通知增加好友
		struct PSS_AddBuddyNotify : public sox::Marshallable
		{
			enum {uri = (14 << 8) | protocol::IMTRANS_SVID};

			PSS_AddBuddyNotify() {};
			void SetUid(uint32_t Uid)
			{
				m_Uid = Uid;
			}
			void SetBids(vector<uint32_t> vecBid)
			{
				m_vecBid.clear();
				m_vecBid.assign(vecBid.begin(), vecBid.end());
			}

			uint32_t m_Uid;
			vector<uint32_t> m_vecBid; //暂时定义为增量用户集合

			virtual void marshal(sox::Pack &p) const
			{
				p << m_Uid;
				sox::marshal_container(p, m_vecBid);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_Uid;
				sox::unmarshal_container(p, back_inserter(m_vecBid));
			}
		};

		//imlist向imtransfer通知删除好友
		struct PSS_DelBuddyNotify : public sox::Marshallable
		{
			enum {uri = (15 << 8) | protocol::IMTRANS_SVID};

			PSS_DelBuddyNotify() {};
			void SetUid(uint32_t Uid)
			{
				m_Uid = Uid;
			}
			void SetBids(vector<uint32_t> vecBid)
			{
				m_vecBid.clear();
				m_vecBid.assign(vecBid.begin(), vecBid.end());
			}

			uint32_t m_Uid;
			vector<uint32_t> m_vecBid; //暂时定义为增量用户集合

			virtual void marshal(sox::Pack &p) const
			{
				p << m_Uid;
				sox::marshal_container(p, m_vecBid);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_Uid;
				sox::unmarshal_container(p, back_inserter(m_vecBid));
			}
		};

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//其它服务器请求imtransfer批量广播的协议

		struct PSS_BatchBroadcast  : public sox::Marshallable
		{
			enum {uri = (16 << 8) | protocol::IMTRANS_SVID};

			PSS_BatchBroadcast() {};

			std::vector<PSS_Broadcast> m_vecBroadcast; //批量的广播包

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_vecBroadcast);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				sox::unmarshal_container(p, back_inserter(m_vecBroadcast));
			}
		};

		//wuji start im2.2新加入的接口

		//imlist向imtransfer通知黑名单多了数据
		struct PSS_AddBlackListNotify : public sox::Marshallable
		{
			enum {uri = (17 << 8) | protocol::IMTRANS_SVID};

			PSS_AddBlackListNotify() {};
			PSS_AddBlackListNotify(uint32_t uId,uint32_t uBid)
			{
				m_uId=uId;
				m_uBid=uBid;
			};
			uint32_t m_uId;
			uint32_t m_uBid;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uId <<m_uBid;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uId >>m_uBid;			
			}
		};

		//imlist向imtransfer通知黑名单少了数据
		struct PSS_DelBlackListNotify : public sox::Marshallable
		{
			enum {uri = (18 << 8) | protocol::IMTRANS_SVID};

			PSS_DelBlackListNotify() {};
			PSS_DelBlackListNotify(uint32_t uId,uint32_t uBid)
			{
				m_uId=uId;
				m_uBid=uBid;
			};
			uint32_t m_uId;
			uint32_t m_uBid;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uId <<m_uBid;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uId >>m_uBid;			
			}
		};





		//wuji end

		//add by kdjie, 2010.3.17

		enum ESomeListType
		{
			ENUM_LIST_BUDDY = 1, //好友列表
			ENUM_LIST_BLACK = 2, //黑名单列表
			ENUM_LIST_BLACKBY = 4, //被加黑名单列表
			ENUM_LIST_FOLDER = 8, //folderlist
			ENUM_IMID = 16, //YY号
			ENUM_LIST_BLOCK = 32 //被阻塞名单列表
			

		};

		// 取预登陆信息(好友列表/黑名单/阻塞名单)转到imtransfer后，暂时保留的与imlist交互的协议，会去掉
		struct PSS_FillUserList : public sox::Marshallable
		{
			enum {uri = (23 << 8) | protocol::IMTRANS_SVID};

			uint32_t m_uSenderUid; //发送人用户Id
			uint32_t m_uOriUri; //原包uri
			string   m_strOriPacket; //原包数据

			uint32_t m_uListType; //请求的列表类型

			//检查请求的类型
			bool IsFillBuddyList() { return ((m_uListType & ENUM_LIST_BUDDY) == ENUM_LIST_BUDDY) ; }
			bool IsFillBlackList() { return ((m_uListType & ENUM_LIST_BLACK) == ENUM_LIST_BLACK) ; }
			bool IsFillBlackByList() { return ((m_uListType & ENUM_LIST_BLACKBY) == ENUM_LIST_BLACKBY) ; }

			void SetFillBuddyList(bool bSet = true) { bSet ? (m_uListType |= ENUM_LIST_BUDDY) : (m_uListType &= ~ENUM_LIST_BUDDY); }
			void SetFillBlackList(bool bSet = true) { bSet ? (m_uListType |= ENUM_LIST_BLACK) : (m_uListType &= ~ENUM_LIST_BLACK); }
			void SetFillBlackByList(bool bSet = true) { bSet ? (m_uListType |= ENUM_LIST_BLACKBY) : (m_uListType &= ~ENUM_LIST_BLACKBY); }


			PSS_FillUserList() { m_uListType = 0; };
			void LoadPacket(uint32_t uUri, const std::string &strPacket)
			{
				m_uOriUri = uUri;
				m_strOriPacket.assign(strPacket.data(), strPacket.size());
			}
			void LoadPacket(uint32_t uUri, const sox::Marshallable &objMarshal)
			{
				//将原包序列化
				sox::PackBuffer pb;
				sox::Pack pk(pb);
				objMarshal.marshal(pk);

				//将原包打好包
				m_uOriUri = uUri;
				m_strOriPacket.assign(pk.data(), pk.size());
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uSenderUid;
				p << m_uOriUri;
				p << m_strOriPacket;

				p << m_uListType;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uSenderUid;
				p >> m_uOriUri;
				p >> m_strOriPacket;

				p >> m_uListType;
			}
		};

		struct PSS_FillUserListRes : public sox::Marshallable
		{
			enum {uri = (23 << 8) | protocol::IMTRANS_SVID};

			uint32_t m_uSenderUid; //发送人用户Id
			uint32_t m_uOriUri; //原包uri
			string   m_strOriPacket; //原包数据

			uint32_t m_uListType;

			vector<protocol::im::CBuddyListNoRemark> m_vecBuddyList; //发送人好友列表
			std::vector<uint32_t> m_vecBlackList;
			std::vector<uint32_t> m_vecBlackByList;
			

			//检查请求的类型
			bool IsFillBuddyList() { return ((m_uListType & ENUM_LIST_BUDDY) == ENUM_LIST_BUDDY) ; }
			bool IsFillBlackList() { return ((m_uListType & ENUM_LIST_BLACK) == ENUM_LIST_BLACK) ; }
			bool IsFillBlackByList() { return ((m_uListType & ENUM_LIST_BLACKBY) == ENUM_LIST_BLACKBY) ; }

			void SetFillBuddyList(bool bSet = true) { bSet ? (m_uListType |= ENUM_LIST_BUDDY) : (m_uListType &= ~ENUM_LIST_BUDDY); }
			void SetFillBlackList(bool bSet = true) { bSet ? (m_uListType |= ENUM_LIST_BLACK) : (m_uListType &= ~ENUM_LIST_BLACK); }
			void SetFillBlackByList(bool bSet = true) { bSet ? (m_uListType |= ENUM_LIST_BLACKBY) : (m_uListType &= ~ENUM_LIST_BLACKBY); }

			PSS_FillUserListRes() { m_uListType = 0; };
			PSS_FillUserListRes(uint32_t uri, const string &strOriPacket) 
			{
				m_uOriUri = uri;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
				m_uListType = 0;
			}

			void SetSenderUid(uint32_t uSenderUid)
			{
				m_uSenderUid = uSenderUid;
			}

			void LoadPacket(uint32_t uUri, const string &strPacket)
			{
				m_uOriUri = uUri;
				m_strOriPacket.assign(strPacket.data(), strPacket.size());
			}
			void LoadPacket(uint32_t uUri, const sox::Marshallable &objMarshal)
			{
				//将原包序列化
				sox::PackBuffer pb;
				sox::Pack pk(pb);
				objMarshal.marshal(pk);

				//将原包打好包
				m_uOriUri = uUri;
				m_strOriPacket.assign(pk.data(), pk.size());
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uSenderUid;
				p << m_uOriUri;
				p << m_strOriPacket;
				p << m_uListType;

				sox::marshal_container(p, m_vecBuddyList);
				sox::marshal_container(p, m_vecBlackList);
				sox::marshal_container(p, m_vecBlackByList);
			
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uSenderUid;
				p >> m_uOriUri;
				p >> m_strOriPacket;
				p >> m_uListType;

				sox::unmarshal_container(p, back_inserter(m_vecBuddyList));
				sox::unmarshal_container(p, back_inserter(m_vecBlackList));
				sox::unmarshal_container(p, back_inserter(m_vecBlackByList));

			}
		};


		// 取预登陆信息(好友列表/黑名单/阻塞名单)转到imtransfer后，与imlist交互的协议
		struct PSS_FillUserList1 : public sox::Marshallable
		{
			enum {uri = (24 << 8) | protocol::IMTRANS_SVID};

			uint32_t m_uSenderUid; //发送人用户Id
			uint32_t m_uListType; //请求的列表类型
			uint32_t m_uLinkId;//用户登录的imlink id,用于转发imid的时候需要

			//检查请求的类型
			bool IsFillBuddyList() { return ((m_uListType & ENUM_LIST_BUDDY) == ENUM_LIST_BUDDY) ; }
			bool IsFillBlackList() { return ((m_uListType & ENUM_LIST_BLACK) == ENUM_LIST_BLACK) ; }
			bool IsFillBlackByList() { return ((m_uListType & ENUM_LIST_BLACKBY) == ENUM_LIST_BLACKBY) ; }

			bool IsFillFolderList() { return ((m_uListType & ENUM_LIST_FOLDER) == ENUM_LIST_FOLDER) ; }
			bool IsFillImid() { return ((m_uListType & ENUM_IMID) == ENUM_IMID) ; }
			bool IsFillBlockList() { return ((m_uListType & ENUM_LIST_BLOCK) == ENUM_LIST_BLOCK) ; }

			void SetFillBuddyList(bool bSet = true) { bSet ? (m_uListType |= ENUM_LIST_BUDDY) : (m_uListType &= ~ENUM_LIST_BUDDY); }
			void SetFillBlackList(bool bSet = true) { bSet ? (m_uListType |= ENUM_LIST_BLACK) : (m_uListType &= ~ENUM_LIST_BLACK); }
			void SetFillBlackByList(bool bSet = true) { bSet ? (m_uListType |= ENUM_LIST_BLACKBY) : (m_uListType &= ~ENUM_LIST_BLACKBY); }

			void SetFillFolderList(bool bSet = true) { bSet ? (m_uListType |= ENUM_LIST_FOLDER) : (m_uListType &= ~ENUM_LIST_FOLDER); }
			void SetFillImid(bool bSet = true) { bSet ? (m_uListType |= ENUM_IMID) : (m_uListType &= ~ENUM_IMID); }
			void SetFillBlockList(bool bSet = true) { bSet ? (m_uListType |= ENUM_LIST_BLOCK) : (m_uListType &= ~ENUM_LIST_BLOCK); }

			PSS_FillUserList1() { m_uListType = 0; };
		
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uSenderUid << m_uListType<<m_uLinkId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p>> m_uSenderUid >> m_uListType>>m_uLinkId;
			}
		};

		struct PSS_FillUserListRes1 : public sox::Marshallable
		{
			enum {uri = (25 << 8) | protocol::IMTRANS_SVID};

			uint32_t m_uSenderUid; //发送人用户Id
			uint32_t m_uListType;
			uint32_t m_uLinkId;//用户登录的imlink id,用于转发imid的时候需要

			vector<protocol::im::CBuddyListNoRemark> m_vecBuddyList; //发送人好友列表
			std::vector<uint32_t> m_vecBlackList;
			std::vector<uint32_t> m_vecBlackByList;
			//追加的数据
			std::vector<uint32_t>		m_vecBlockUid;		//阻塞名单
			std::vector<protocol::im::CFolderList> m_vecFolderList;		//分组列表
			uint32_t m_uImid;//发送人YY号



			//检查请求的类型
			bool IsFillBuddyList() { return ((m_uListType & ENUM_LIST_BUDDY) == ENUM_LIST_BUDDY) ; }
			bool IsFillBlackList() { return ((m_uListType & ENUM_LIST_BLACK) == ENUM_LIST_BLACK) ; }
			bool IsFillBlackByList() { return ((m_uListType & ENUM_LIST_BLACKBY) == ENUM_LIST_BLACKBY) ; }

			bool IsFillFolderList() { return ((m_uListType & ENUM_LIST_FOLDER) == ENUM_LIST_FOLDER) ; }
			bool IsFillImid() { return ((m_uListType & ENUM_IMID) == ENUM_IMID) ; }
			bool IsFillBlockList() { return ((m_uListType & ENUM_LIST_BLOCK) == ENUM_LIST_BLOCK) ; }


			void SetFillBuddyList(bool bSet = true) { bSet ? (m_uListType |= ENUM_LIST_BUDDY) : (m_uListType &= ~ENUM_LIST_BUDDY); }
			void SetFillBlackList(bool bSet = true) { bSet ? (m_uListType |= ENUM_LIST_BLACK) : (m_uListType &= ~ENUM_LIST_BLACK); }
			void SetFillBlackByList(bool bSet = true) { bSet ? (m_uListType |= ENUM_LIST_BLACKBY) : (m_uListType &= ~ENUM_LIST_BLACKBY); }

			void SetFillFolderList(bool bSet = true) { bSet ? (m_uListType |= ENUM_LIST_FOLDER) : (m_uListType &= ~ENUM_LIST_FOLDER); }
			void SetFillImid(bool bSet = true) { bSet ? (m_uListType |= ENUM_IMID) : (m_uListType &= ~ENUM_IMID); }
			void SetFillBlockList(bool bSet = true) { bSet ? (m_uListType |= ENUM_LIST_BLOCK) : (m_uListType &= ~ENUM_LIST_BLOCK); }


			PSS_FillUserListRes1() { m_uListType = 0; };
			
			void SetSenderUid(uint32_t uSenderUid)
			{
				m_uSenderUid = uSenderUid;
			}

			
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uSenderUid<<m_uListType<<m_uLinkId;
				sox::marshal_container(p, m_vecBuddyList);
				sox::marshal_container(p, m_vecBlackList);
				sox::marshal_container(p, m_vecBlackByList);

				sox::marshal_container(p, m_vecBlockUid);
				sox::marshal_container(p, m_vecFolderList);
				p<<m_uImid;

			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uSenderUid>>m_uListType>>m_uLinkId;
				sox::unmarshal_container(p, back_inserter(m_vecBuddyList));
				sox::unmarshal_container(p, back_inserter(m_vecBlackList));
				sox::unmarshal_container(p, back_inserter(m_vecBlackByList));

				sox::unmarshal_container(p, back_inserter(m_vecBlockUid));
				sox::unmarshal_container(p, back_inserter(m_vecFolderList));
				p>>m_uImid;
			}
		};


		//屏蔽名单
		struct PSS_AddBlockListNotify : public sox::Marshallable
		{
			enum {uri = (26 << 8) | protocol::IMTRANS_SVID};

			PSS_AddBlockListNotify() {};
			PSS_AddBlockListNotify(uint32_t uId,uint32_t uBid)
			{
				m_uId=uId;
				m_uBid=uBid;
			};
			uint32_t m_uId;
			uint32_t m_uBid;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uId <<m_uBid;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uId >>m_uBid;			
			}
		};

		//imlist向imtransfer通知屏蔽名单少了数据
		struct PSS_DelBlockListNotify : public sox::Marshallable
		{
			enum {uri = (27 << 8) | protocol::IMTRANS_SVID};

			PSS_DelBlockListNotify() {};
			PSS_DelBlockListNotify(uint32_t uId,uint32_t uBid)
			{
				m_uId=uId;
				m_uBid=uBid;
			};
			uint32_t m_uId;
			uint32_t m_uBid;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uId <<m_uBid;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uId >>m_uBid;			
			}
		};

		//分组列表发生变化
		struct PSS_AddFolderListNotify : public sox::Marshallable
		{
			enum {uri = (28 << 8) | protocol::IMTRANS_SVID};

			PSS_AddFolderListNotify() {};
			PSS_AddFolderListNotify(uint32_t uId,uint32_t uPid)
			{
				m_uId=uId;
				m_uPid=uPid;
			};
			uint32_t m_uId;
			uint32_t m_uPid;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uId <<m_uPid;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uId >>m_uPid;			
			}
		};

		//分组发生变化
		struct PSS_DelFolderListNotify : public sox::Marshallable
		{
			enum {uri = (29 << 8) | protocol::IMTRANS_SVID};

			PSS_DelFolderListNotify() {};
			PSS_DelFolderListNotify(uint32_t uId,uint32_t uPid)
			{
				m_uId=uId;
				m_uPid=uPid;
			};
			uint32_t m_uId;
			uint32_t m_uPid;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uId <<m_uPid;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uId >>m_uPid;			
			}
		};





		//add end

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//buddylist 缓存准确率检查的协议
		struct PSS_BuddyListCheck : public sox::Marshallable
		{
			enum{uri = (30 << 8) | protocol::IMTRANS_SVID};

			PSS_BuddyListCheck(){};

			uint32_t maxCheckNum;	//最大检查数量
			uint32_t serverId;	//imtransfer的serverId,方便定位问题
			//uint32_t为用户UID, vector为用户的好友
			std::map<uint32_t, std::vector<protocol::im::CBuddyListNoRemark> > m_mapBuddyList;

			virtual void marshal(sox::Pack &p) const
			{
				p << maxCheckNum << serverId;
				//sox::marshal_container(p, m_mapBuddyList);

				uint32_t uMapSize = m_mapBuddyList.size();
				p.push_uint32(uMapSize);

				for (std::map<uint32_t, std::vector<protocol::im::CBuddyListNoRemark> >::const_iterator iterBuddy = m_mapBuddyList.begin();
					iterBuddy != m_mapBuddyList.end(); iterBuddy ++)
				{
					//uint32_t uUid = iterBuddy->first;
					//std::vector<protocol::im::CBuddyListNoRemark> &vecBuddy = iterBuddy->second;

					p.push_uint32(iterBuddy->first);
					sox::marshal_container(p, iterBuddy->second);
				}
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> maxCheckNum >> serverId;
				//sox::unmarshal_container(p, inserter(m_mapBuddyList, m_mapBuddyList.begin()));

				uint32_t uMapSize = p.pop_uint32();
				for (uint32_t i = 0; i < uMapSize; i ++)
				{
					uint32_t uUid = p.pop_uint32();

					std::vector<protocol::im::CBuddyListNoRemark> vecBuddy;
					sox::unmarshal_container(p, back_inserter(vecBuddy));

					m_mapBuddyList.insert( std::make_pair(uUid, vecBuddy) );
				}
			}
		};

		//服务器获取好友列表
		struct PSS_GetBuddyList : public sox::Marshallable
		{
			enum {uri = (31 << 8) | protocol::IMTRANS_SVID};

			PSS_GetBuddyList() {};
			PSS_GetBuddyList(uint32_t uUid, uint32_t uImlinkd, uint32_t uUri, const sox::Marshallable &objMarshal)
			{
				m_uUid = uUid;
				m_uImlinkd = uImlinkd;
				m_uOriUri = uUri;
				sox::PacketToString(objMarshal, m_strOriPacket);
			};

			uint32_t m_uUid;
			uint32_t m_uImlinkd;
			uint32_t m_uOriUri;
			std::string m_strOriPacket;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uImlinkd << m_uOriUri << m_strOriPacket;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uUid >> m_uImlinkd >> m_uOriUri >> m_strOriPacket;			
			}
		};
		
		//服务器获取好友列表响应
		struct PSS_GetBuddyListRes : public sox::Marshallable
		{
			enum {uri = (32 << 8) | protocol::IMTRANS_SVID};

			PSS_GetBuddyListRes() {};
			PSS_GetBuddyListRes(uint32_t uUid, uint32_t uImlinkd, const std::vector<uint32_t> &vec, uint32_t uUri, const std::string &str)
			{
				m_uUid = uUid;
				m_uImlinkd = uImlinkd;
				m_vecBuddys = vec;
				m_uOriUri = uUri;
				m_strOriPacket = str;
			};

			uint32_t m_uUid;
			uint32_t m_uImlinkd;
			std::vector<uint32_t> m_vecBuddys;
			uint32_t m_uOriUri;
			std::string m_strOriPacket;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uImlinkd << m_uOriUri << m_strOriPacket;
				marshal_container(p, m_vecBuddys);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uUid >> m_uImlinkd >> m_uOriUri >> m_strOriPacket;
				//unmarshal_container(p, std::inserter(m_setBuddys, m_setBuddys.begin()));
				unmarshal_container(p, std::back_inserter(m_vecBuddys));
			}
		};

		//更新好友列表时通知服务器
		struct PSS_UpdateBuddyList : public sox::Marshallable
		{
			enum {uri = (33 << 8) | protocol::IMTRANS_SVID};

			PSS_UpdateBuddyList() {};
			PSS_UpdateBuddyList(uint32_t uOper, uint32_t uUid, const std::vector<uint32_t> &vec)
			{
				m_uOper = uOper;
				m_uUid = uUid;
				m_vecUpdateBuddys = vec;
			};
			uint32_t m_uOper; //0:add, 1:delete
			uint32_t m_uUid;
			std::vector<uint32_t> m_vecUpdateBuddys;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uOper << m_uUid;
				marshal_container(p, m_vecUpdateBuddys);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uOper >> m_uUid;
				//unmarshal_container(p, std::inserter(m_setUpdateBuddys, m_setUpdateBuddys.begin()));
				unmarshal_container(p, std::back_inserter(m_vecUpdateBuddys));
			}
		};
		// 通过imtransfer转发的请求协议总数

		enum EPacketUri
		{
			ENUM_IMVOICE_REQ = 0,        // 1对1 语音请求
			ENUM_IMVOICE_AGREE,          // 同意1对1
			ENUM_IMVOICE_REJECT,         // 拒接1对1
			ENUM_IMVOICE_EXIT            // 退出1对1
		};

		struct PSS_ForwardStatis : public sox::Marshallable
		{
			enum{uri = (34 << 8) | protocol::IMTRANS_SVID};

			PSS_ForwardStatis() {}
			PSS_ForwardStatis(const std::map<uint32_t, uint64_t>& mapUri2Count)
				: m_mapUri2Count(mapUri2Count)
			{
			}

			std::map<uint32_t, uint64_t> m_mapUri2Count;

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_mapUri2Count);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::inserter(m_mapUri2Count, m_mapUri2Count.begin()));
			}
		};
	}
}

#endif
