#pragma once

#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"

#include "protocol/common/protocol/PGTransfer.h" //为了能包含EOfflineMsgMode的定义

#include <iostream>
#include <string>

namespace server
{
	namespace gtransfer
	{
		////离线保存模式
		//enum EOfflineMsgMode
		//{
		//	ENUM_NOT_SAVE_OFFLINE = 0,
		//	ENUM_SAVE_OFFLINE_ACTION,
		//	ENUM_SAVE_OFFLINE_MSG
		//};

		////将结构体转换成字符串
		//inline void Packet2String(const sox::Marshallable &objMarshal, std::string &strPacket)
		//{
		//	//将原包序列化，并装载
		//	sox::PackBuffer pb;
		//	sox::Pack pk(pb);
		//	objMarshal.marshal(pk);

		//	strPacket.assign(pk.data(), pk.size());
		//}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		//通用的群/组广播结构

		struct SGBroadcast : public sox::Marshallable
		{
			SGBroadcast() 
			{
				m_uActionMode = TO_ALL_MODE;
				m_uFid = 0;
				m_bSync = false;
			};
			SGBroadcast(uint32_t Uri, const std::string &strOriPacket) 
			{
				m_uActionMode = TO_ALL_MODE;
				m_uFid = 0;
				m_uOriUri = Uri;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
				m_bSync = false;
			}
			SGBroadcast(uint32_t Uri, const sox::Marshallable &objMarshal) 
			{
				m_uActionMode = TO_ALL_MODE;				
				m_uFid = 0;
				m_uOriUri = Uri;
				protocol::gtransfer::Packet2String(objMarshal, m_strOriPacket);
				m_bSync = false;
			}

			void SetSendId(uint32_t Uid)
			{
				m_uSenderId = Uid;
			}
			void SetBroadCastId(uint32_t Bid)
			{
				m_uBroadCastId = Bid;
			}
			void SetKey(uint32_t key)
			{
				m_uHashKey = key;
			}
			void SetOfflineMode(protocol::gtransfer::EOfflineMsgMode emOfflineMode = protocol::gtransfer::ENUM_NOT_SAVE_OFFLINE)
			{
				m_emOfflineMode = emOfflineMode;
			}

			void SetOfflineActionMode(uint32_t uOfflineActionMode)
			{
				m_uActionMode = uOfflineActionMode;
			}
			void SetFid(uint32_t fid)
			{
				m_uFid = fid;
			}

			uint32_t m_uSenderId; //发送人ID，保存离线时需要使用
			uint32_t m_uHashKey; //hash key, 必然是群id
			uint32_t m_uBroadCastId; //广播范围,群/组id
			protocol::gtransfer::EOfflineMsgMode m_emOfflineMode; //离线消息模式

			uint32_t m_uOriUri; //原包uri，解析并派发原包时使用
			std::string m_strOriPacket; //原包数据
			
			uint32_t m_uFid; //组id，ginfo需要
			uint32_t m_uActionMode; //offline action mode

			bool m_bSync;
			std::vector<uint32_t> m_vecSyncUids;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uSenderId;
				p << m_uHashKey;
				p << m_uBroadCastId;
				p.push_uint8(m_emOfflineMode);

				p << m_uOriUri;
				p << m_strOriPacket;
				p << m_uFid;
				p << m_uActionMode;

				p.push_uint8(m_bSync);
				sox::marshal_container(p, m_vecSyncUids);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uSenderId;
				p >> m_uHashKey;
				p >> m_uBroadCastId;

				m_emOfflineMode = (protocol::gtransfer::EOfflineMsgMode)p.pop_uint8();

				p >> m_uOriUri;
				p >> m_strOriPacket;

				p >> m_uFid;
				p >> m_uActionMode;

				if (p.size() != 0)
				{
					m_bSync = (bool)p.pop_uint8();
				}

				if (p.size() != 0)
				{
					sox::unmarshal_container(p, std::back_inserter(m_vecSyncUids));
				}
			}
		};

		struct SGBroadcast2 : public sox::Marshallable
		{
			SGBroadcast2() 
			{
				m_bSync = false;
			};
			SGBroadcast2(uint32_t Uri, const std::string &strOriPacket) 
			{
				m_uOriUri = Uri;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
				m_bSync = false;
			}
			SGBroadcast2(uint32_t Uri, const sox::Marshallable &objMarshal) 
			{
				m_uOriUri = Uri;
				protocol::gtransfer::Packet2String(objMarshal, m_strOriPacket);
				m_bSync = false;
			}

			void SetSendId(uint32_t Uid)
			{
				m_uSenderId = Uid;
			}
			void SetGid(uint32_t Gid)
			{
				m_uGid = Gid;
			}
			void SetKey(uint32_t key)
			{
				m_uHashKey = key;
			}
			void SetOfflineMode(protocol::gtransfer::EOfflineMsgMode emOfflineMode = protocol::gtransfer::ENUM_NOT_SAVE_OFFLINE)
			{
				m_emOfflineMode = emOfflineMode;
			}

			uint32_t m_uSenderId; //发送人ID，保存离线时需要使用
			uint32_t m_uHashKey; //hash key
			uint32_t m_uGid; //群/组ID
			protocol::gtransfer::EOfflineMsgMode m_emOfflineMode; //离线消息模式

			uint32_t m_uOriUri; //原包uri，解析并派发原包时使用
			std::string m_strOriPacket; //原包数据
			bool m_bSync;
			std::vector<uint32_t> m_vecSyncUids;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uSenderId;
				p << m_uHashKey;
				p << m_uGid;
				p.push_uint8(m_emOfflineMode);

				p << m_uOriUri;
				p.push_varstr32(m_strOriPacket.data(), m_strOriPacket.length());
				p.push_uint8(m_bSync);
				sox::marshal_container(p, m_vecSyncUids);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uSenderId;
				p >> m_uHashKey;
				p >> m_uGid;
				m_emOfflineMode = (protocol::gtransfer::EOfflineMsgMode)p.pop_uint8();

				p >> m_uOriUri;
				m_strOriPacket = p.pop_varstr32();
				if (p.size() != 0)
				{
					m_bSync = (bool)p.pop_uint8();
				}
				
				if (p.size() != 0)
				{
					sox::unmarshal_container(p, std::back_inserter(m_vecSyncUids));
				}
			}
		};


		//其它服务器请求gtransfer转发的协议

		struct SGForward : public sox::Marshallable
		{
			SGForward() 
			{
				m_uGid = 0;
				m_uFid = 0;
				m_uActionMode = 0;
				m_bSync = false;
			};
			SGForward(uint32_t Uri, const std::string &strOriPacket) 
			{
				m_uGid = 0;
				m_uFid = 0;
				m_uActionMode = 0;
				m_uOriUri = Uri;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
				m_bSync = false;
			}
			SGForward(uint32_t Uri, const sox::Marshallable &objMarshal) 
			{
				m_uGid = 0;
				m_uFid = 0;
				m_uActionMode = 0;
				m_uOriUri = Uri;
				protocol::gtransfer::Packet2String(objMarshal, m_strOriPacket);
				m_bSync = false;
			}

			void SetSendId(uint32_t Uid)
			{
				m_uSenderId = Uid;
			}
			void SetRecvUidVect(const std::vector<uint32_t> &vecRecvUid)
			{
				m_vecRecvUid.clear();
				m_vecRecvUid.assign(vecRecvUid.begin(), vecRecvUid.end());
			}
			void SetOfflineMode(protocol::gtransfer::EOfflineMsgMode emOfflineMode = protocol::gtransfer::ENUM_NOT_SAVE_OFFLINE)
			{
				m_emOfflineMode = emOfflineMode;
			}
			
			void SetGid(uint32_t gid)
			{
				m_uGid = gid;
			}
			
			void SetFid(uint32_t fid)
			{
				m_uFid = fid;
			}
			
			void SetOfflineActionMode(uint32_t uOfflineActionMode)
			{
				m_uActionMode = uOfflineActionMode;
			}

			uint32_t m_uSenderId; //发送人ID
			std::vector<uint32_t> m_vecRecvUid; //需要接收该消息的用户列表
			protocol::gtransfer::EOfflineMsgMode m_emOfflineMode; //离线消息模式

			uint32_t m_uGid;
			uint32_t m_uFid;
			uint32_t m_uActionMode;

			uint32_t m_uOriUri; //原包uri，解析并派发原包时使用
			std::string m_strOriPacket; //原包数据
			bool m_bSync;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uSenderId;
				sox::marshal_container(p, m_vecRecvUid);
				p.push_uint8(m_emOfflineMode);

				p << m_uGid;
				p << m_uOriUri;
				p << m_strOriPacket;
				
				p << m_uFid;
				p << m_uActionMode;
				p.push_uint8(m_bSync);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uSenderId;
				sox::unmarshal_container(p, back_inserter(m_vecRecvUid));
				m_emOfflineMode = (protocol::gtransfer::EOfflineMsgMode)p.pop_uint8();

				p >> m_uGid;
				p >> m_uOriUri;
				p >> m_strOriPacket;

				p >> m_uFid;
				p >> m_uActionMode;
				if (p.size() > 0)
				{
					m_bSync = (bool)p.pop_uint8();
				}
			}
		};

		//其它服务器请求gtransfer转发的协议2

		struct SGForward2 : public sox::Marshallable
		{
			SGForward2() {};
			SGForward2(uint32_t Uri, const std::string &strOriPacket) 
			{
				m_uOriUri = Uri;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
				m_bSync = false;
			}
			SGForward2(uint32_t Uri, const sox::Marshallable &objMarshal) 
			{
				m_uOriUri = Uri;
				protocol::gtransfer::Packet2String(objMarshal, m_strOriPacket);
				m_bSync = false;
			}

			void SetSendId(uint32_t Uid)
			{
				m_uSenderId = Uid;
			}

			void SetSendNum(uint32_t number)
			{
				m_uSendNum = number;
			}
			void SetRecvUidVect1(const std::vector<uint32_t> &vecRecvUid)
			{
				m_vecRecvUid1.clear();
				m_vecRecvUid1.assign(vecRecvUid.begin(), vecRecvUid.end());
			}
			void SetRecvUidVect2(const std::vector<uint32_t> &vecRecvUid)
			{
				m_vecRecvUid2.clear();
				m_vecRecvUid2.assign(vecRecvUid.begin(), vecRecvUid.end());
			}
			void SetOfflineMode(protocol::gtransfer::EOfflineMsgMode emOfflineMode = protocol::gtransfer::ENUM_NOT_SAVE_OFFLINE)
			{
				m_emOfflineMode = emOfflineMode;
			}

			uint32_t m_uSenderId; //发送人ID
			uint32_t m_uSendNum; //用户列表1的发送数量
			std::vector<uint32_t> m_vecRecvUid1; //需要接收该消息的用户列表1
			std::vector<uint32_t> m_vecRecvUid2; //需要接收该消息的用户列表2
			protocol::gtransfer::EOfflineMsgMode m_emOfflineMode; //离线消息模式

			uint32_t m_uOriUri; //原包uri，解析并派发原包时使用
			std::string m_strOriPacket; //原包数据
			bool m_bSync;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uSenderId << m_uSendNum;
				sox::marshal_container(p, m_vecRecvUid1);
				sox::marshal_container(p, m_vecRecvUid2);
				p.push_uint8(m_emOfflineMode);

				p << m_uOriUri;
				p << m_strOriPacket;				
				p.push_uint8(m_bSync);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uSenderId >> m_uSendNum;
				sox::unmarshal_container(p, back_inserter(m_vecRecvUid1));
				sox::unmarshal_container(p, back_inserter(m_vecRecvUid2));
				m_emOfflineMode = (protocol::gtransfer::EOfflineMsgMode)p.pop_uint8();

				p >> m_uOriUri;
				p >> m_strOriPacket;
				if (p.size() > 0)
				{
					m_bSync = (bool)p.pop_uint8();
				}
			}
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		//其它服务器请求gtransfer广播的协议

		struct PSS_GBroadcast  : public sox::Marshallable 
		{
			enum {uri = (1 << 8) | protocol::GTRANS_SVID};

			PSS_GBroadcast() {};
			PSS_GBroadcast(uint32_t Uri, const std::string &strOriPacket) : m_stGBoardcast(Uri, strOriPacket) {};
			PSS_GBroadcast(uint32_t Uri, const sox::Marshallable &objMarshal) : m_stGBoardcast(Uri, objMarshal) {};

			SGBroadcast	m_stGBoardcast;

			virtual void marshal(sox::Pack &p) const
			{
				m_stGBoardcast.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				m_stGBoardcast.unmarshal(p);
			}
		};

		struct PSS_GBroadcast2  : public sox::Marshallable 
		{
			enum {uri = (13 << 8) | protocol::GTRANS_SVID};

			PSS_GBroadcast2() {};
			PSS_GBroadcast2(uint32_t Uri, const std::string &strOriPacket) : m_stGBoardcast(Uri, strOriPacket) {};
			PSS_GBroadcast2(uint32_t Uri, const sox::Marshallable &objMarshal) : m_stGBoardcast(Uri, objMarshal) {};

			SGBroadcast2	m_stGBoardcast;

			virtual void marshal(sox::Pack &p) const
			{
				m_stGBoardcast.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				m_stGBoardcast.unmarshal(p);
			}
		};

		//其它服务器请求gtransfer转发的协议

		struct PSS_GForward : public sox::Marshallable
		{
			enum {uri = (2 << 8) | protocol::GTRANS_SVID};

			PSS_GForward() {};
			PSS_GForward(uint32_t Uri, const std::string &strOriPacket) : m_stGForward(Uri, strOriPacket) {};
			PSS_GForward(uint32_t Uri, const sox::Marshallable &objMarshal) : m_stGForward(Uri, objMarshal) {};

			SGForward	m_stGForward;

			virtual void marshal(sox::Pack &p) const
			{
				m_stGForward.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				m_stGForward.unmarshal(p);
			}
		};

		//其它服务器请求gtransfer转发的协议2

		struct PSS_GForward2 : public sox::Marshallable
		{
			enum {uri = (3 << 8) | protocol::GTRANS_SVID};

			PSS_GForward2() {};
			PSS_GForward2(uint32_t Uri, const std::string &strOriPacket) : m_stGForward2(Uri, strOriPacket) {};
			PSS_GForward2(uint32_t Uri, const sox::Marshallable &objMarshal) : m_stGForward2(Uri, objMarshal) {};

			SGForward2	m_stGForward2;

			virtual void marshal(sox::Pack &p) const
			{
				m_stGForward2.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				m_stGForward2.unmarshal(p);
			}
		};

		//gtransfer向ginfo请求填充群/组成员列表
		//带上原包（这里可能是PSS_GBroadcast、PSS_GForward、PSS_GForward2等）

		struct PSS_GFillMembers2  : public sox::Marshallable 
		{
			enum {uri = (4 << 8) | protocol::GTRANS_SVID};

			PSS_GFillMembers2() {};
			PSS_GFillMembers2(uint32_t Uri, const std::string &strOriPacket) 
			{
				m_uOriUri = Uri;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}
			PSS_GFillMembers2(uint32_t Uri, const sox::Marshallable &objMarshal) 
			{
				m_uOriUri = Uri;
				protocol::gtransfer::Packet2String(objMarshal, m_strOriPacket);
			}

			void SetGidFid(uint32_t Gid, uint32_t Fid)
			{
				m_uGid = Gid;
				m_uFid = Fid;
			}

			uint32_t m_uGid; //群ID
			uint32_t m_uFid; //组ID
			uint32_t m_uOriUri; //原包uri，解析并派发原包时使用
			std::string m_strOriPacket; //原包数据

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGid;
				p << m_uFid;
				p << m_uOriUri;
				p << m_strOriPacket;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGid;
				p >> m_uFid;
				p >> m_uOriUri;
				p >> m_strOriPacket;
			}
		};

		//ginfo向gtransfer返回填充填充好成员后的包

		struct PSS_GFillMembersRes2  : public sox::Marshallable 
		{
			enum {uri = (5 << 8) | protocol::GTRANS_SVID};

			PSS_GFillMembersRes2() {};
			PSS_GFillMembersRes2(uint32_t Uri, const std::string &strOriPacket) 
			{
				m_uOriUri = Uri;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}
			PSS_GFillMembersRes2(uint32_t Uri, const sox::Marshallable &objMarshal) 
			{
				m_uOriUri = Uri;
				protocol::gtransfer::Packet2String(objMarshal, m_strOriPacket);
			}

			void SetGidFid(uint32_t Gid, uint32_t Fid)
			{
				m_uGid = Gid;
				m_uFid = Fid;
			}

			void SetMembers(const std::vector<uint32_t> &vecMembers)
			{
				m_vecMembers.assign(vecMembers.begin(), vecMembers.end());
			}

			uint32_t m_uGid; //群ID
			uint32_t m_uFid; //组ID
			std::vector<uint32_t> m_vecMembers; //群/组成员列表

			uint32_t m_uOriUri; //原包uri，解析并派发原包时使用
			std::string m_strOriPacket; //原包数据

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGid;
				p << m_uFid;
				sox::marshal_container(p, m_vecMembers);

				p << m_uOriUri;
				p << m_strOriPacket;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGid;
				p >> m_uFid;
				sox::unmarshal_container(p, back_inserter(m_vecMembers));

				p >> m_uOriUri;
				p >> m_strOriPacket;
			}
		};



		enum E_GMEM_UPDATE_OPER_FLAG
		{
			EMPTY_LIST = 0,
			ADD_LIST = 1,
			DEL_LIST = 2
		}; 

		//群/组所属用户列表更新信息
		struct SGMemUpdateList : public sox::Marshallable
		{
			uint32_t    m_uGroupId;         // 群ID
			uint32_t    m_uFolderId;        // 组ID
			uint32_t    m_uOper;            // 操作符,取值范围见E_GMEM_UPDATE_OPER_FLAG
			uint32_t    m_uCheckSum;        // 校验和
			std::vector<uint32_t> m_vecUidList; // 待处理成员列表

			SGMemUpdateList(){}
			SGMemUpdateList(uint32_t gid, uint32_t fid, uint32_t oper, uint32_t cksum,
						  			const std::vector<uint32_t>& uidlist)
				: m_uGroupId(gid), m_uFolderId(fid), m_uOper(oper), m_uCheckSum(cksum)
			{
				m_vecUidList = uidlist;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uFolderId << m_uOper << m_uCheckSum;
				sox::marshal_container(p, m_vecUidList);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGroupId >> m_uFolderId >> m_uOper >> m_uCheckSum;
				sox::unmarshal_container(p, std::back_inserter(m_vecUidList));
			}
		};

		//其它服务器通过gtranfer群/组的成员过期

		struct PSS_GMembersUpdate  : public sox::Marshallable 
		{
			enum {uri = (6 << 8) | protocol::GTRANS_SVID};

			std::vector<SGMemUpdateList> m_vecGmemList; //改变的群/组用户列表信息

			PSS_GMembersUpdate() {};
			PSS_GMembersUpdate(const std::vector<SGMemUpdateList> &vec)
			{
				m_vecGmemList.assign(vec.begin(), vec.end());
			}
			
			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_vecGmemList);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				sox::unmarshal_containerEx(p, m_vecGmemList);
			}
		};

		//群友之间临时会话消息
		//扩展了发送人

		struct PSS_GChatMsgExt : public protocol::gtransfer::PCS_GChatMsg2
		{
			enum {uri = (7 << 8) | protocol::GTRANS_SVID};

			PSS_GChatMsgExt() {};
			PSS_GChatMsgExt(uint32_t uGrpID, uint32_t uSenderId, uint32_t uRecverId, uint32_t uMsgId, std::string strText, uint32_t uSendTime, std::string strFrom) :
				PCS_GChatMsg2(uGrpID, uRecverId, uMsgId, strText, uSendTime)
			{
				m_uSenderId = uSenderId;
				m_strFrom = strFrom;
			}

			uint32_t m_uSenderId;
			std::string m_strFrom;

			virtual void marshal(sox::Pack &p) const
			{
				PCS_GChatMsg2::marshal(p);

				p << m_uSenderId << m_strFrom;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_GChatMsg2::unmarshal(p);

				p >> m_uSenderId >> m_strFrom;
			}
		};
		
		//gtransfer服务器向ginfo服务器请求folderlist
		struct PSS_GFoldersReq  : public sox::Marshallable 
		{
			enum {uri = (8 << 8) | protocol::GTRANS_SVID};

			uint32_t m_uGroupId;        //群Id			
			uint32_t m_uOriUri;         //原包uri，解析并派发原包时使用
			std::string m_strOriPacket; //原包数据

			PSS_GFoldersReq() {};
			PSS_GFoldersReq(uint32_t uUri, const sox::Marshallable &objMarshal)
				:m_uOriUri(uUri)
			{
				protocol::gtransfer::Packet2String(objMarshal, m_strOriPacket);
			}			
			PSS_GFoldersReq(uint32_t uUri, const std::string &strOriPacket)
				:m_uOriUri(uUri)
			{
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}

			void SetGid(uint32_t Gid)
			{
				m_uGroupId = Gid;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uOriUri << m_strOriPacket;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGroupId >> m_uOriUri >> m_strOriPacket;
			}
		};

		//PSS_GFolderListReq的响应
		struct PSS_GFoldersRes : public sox::Marshallable 
		{
			enum {uri = (9 << 8) | protocol::GTRANS_SVID};

			uint32_t m_uGroupId;     //群Id				
			std::vector<uint32_t> m_vecFolders;     //组Id列表	

			uint32_t m_uOriUri;         //原包uri，解析并派发原包时使用
			std::string m_strOriPacket; //原包数据

			PSS_GFoldersRes() {};
			PSS_GFoldersRes(uint32_t uri, const sox::Marshallable &objMarshal)
				:m_uOriUri(uri)
			{
				protocol::gtransfer::Packet2String(objMarshal, m_strOriPacket);
			}
			PSS_GFoldersRes(uint32_t uri, const std::string &strOriPacket)
				:m_uOriUri(uri)
			{
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}

			void SetGid(uint32_t Gid)
			{
				m_uGroupId = Gid;
			}

			void SetFolders(const std::vector<uint32_t> &vecFolders)
			{
				m_vecFolders.assign(vecFolders.begin(), vecFolders.end());
			}
				
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uOriUri << m_strOriPacket;
				sox::marshal_container(p, m_vecFolders);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGroupId >> m_uOriUri >> m_strOriPacket;
				sox::unmarshal_container(p, std::back_inserter(m_vecFolders));
			}
		};

		//ginfo服务器通知gtransfer服务器folderlist更新
		struct PSS_GFoldersUpdate  : public sox::Marshallable 
		{
			enum {uri = (10 << 8) | protocol::GTRANS_SVID};

			uint32_t m_uGroupId;     //群Id
			uint32_t m_uOper;        // 操作符,取值范围见E_GMEM_UPDATE_OPER_FLAG
			uint32_t m_uCheckSum;    // 校验和
			std::vector<uint32_t> m_vecFolders; // 待处理组列表

			PSS_GFoldersUpdate() {};
			PSS_GFoldersUpdate(uint32_t gid, uint32_t Oper, uint32_t Cksum,const std::vector<uint32_t>& vec)
				:m_uGroupId(gid), m_uOper(Oper), m_uCheckSum(Cksum)
			{
				m_vecFolders = vec;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uOper << m_uCheckSum;
				sox::marshal_container(p, m_vecFolders);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGroupId >> m_uOper >> m_uCheckSum;
				sox::unmarshal_container(p, std::back_inserter(m_vecFolders));
			}
		};
		
		//gchat request fid-imlinkd list from gtransfer
		struct PSS_GImlinkdReq : public sox::Marshallable
		{
 			enum {uri = (11 << 8) | protocol::GTRANS_SVID};

			uint32_t               m_uGroupId;             // group id
			uint32_t               m_uImlinkdId;           // imlinkd id
			std::vector<uint32_t>  m_vecFolderId;          // folder id

			PSS_GImlinkdReq(){}
			PSS_GImlinkdReq(uint32_t uGroupId, uint32_t uImlinkdId, const std::vector<uint32_t>& vec)
				:m_uGroupId(uGroupId), m_uImlinkdId(uImlinkdId)
			{
				m_vecFolderId = vec;
			}
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uImlinkdId;
				sox::marshal_container(p, m_vecFolderId);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGroupId >> m_uImlinkdId;
				sox::unmarshal_container(p, std::back_inserter(m_vecFolderId));
			}
		};

		struct PSS_GTransNoDispatch  : public sox::Marshallable 
		{
			enum {uri = (12 << 8) | protocol::GTRANS_SVID};
			PSS_GTransNoDispatch(){}
			virtual void marshal(sox::Pack &p) const{}
			virtual void unmarshal(const sox::Unpack &p) {}
		};

		struct PSS_GSyncForward2 : public sox::Marshallable
		{
			enum {uri = (14 << 8) | protocol::GTRANS_SVID};

			PSS_GSyncForward2(){}

			uint32_t m_uSenderId;
			uint32_t m_uOriUri;
			std::string m_strOriPacket;
			protocol::gtransfer::EOfflineMsgMode m_emOfflineMode;
			uint32_t m_uSendNum;
			uint32_t m_uSentNum;
			std::vector<uint32_t> m_vecRecvUid1;
			std::vector<uint32_t> m_vecRecvUid2;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uSenderId << m_uOriUri << m_strOriPacket << m_uSendNum << m_uSentNum;
				p.push_uint8(m_emOfflineMode);
				sox::marshal_container(p, m_vecRecvUid1);
				sox::marshal_container(p, m_vecRecvUid2);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uSenderId >> m_uOriUri >> m_strOriPacket >> m_uSendNum >> m_uSentNum;
				m_emOfflineMode = (protocol::gtransfer::EOfflineMsgMode)p.pop_uint8();
				sox::unmarshal_container(p, std::back_inserter(m_vecRecvUid1));
				sox::unmarshal_container(p, std::back_inserter(m_vecRecvUid2));
			}
		};

		//gtransfer向ginfo请求填充群/组成员列表
		//带上原包（这里可能是PSS_GBroadcast、PSS_GForward、PSS_GForward2等）

		struct PSS_GFillMembers  : public sox::Marshallable 
		{
			enum {uri = (15 << 8) | protocol::GTRANS_SVID};

			PSS_GFillMembers() {};
			PSS_GFillMembers(uint32_t Uri, const std::string &strOriPacket) 
			{
				m_uOriUri = Uri;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}
			PSS_GFillMembers(uint32_t Uri, const sox::Marshallable &objMarshal) 
			{
				m_uOriUri = Uri;
				protocol::gtransfer::Packet2String(objMarshal, m_strOriPacket);
			}

			void SetGidFid(uint32_t Gid, uint32_t Fid)
			{
				m_uGid = Gid;
				m_uFid = Fid;
			}

			uint32_t m_uGid; //群ID
			uint32_t m_uFid; //组ID
			uint32_t m_uOriUri; //原包uri，解析并派发原包时使用
			std::string m_strOriPacket; //原包数据

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGid;
				p << m_uFid;
				p << m_uOriUri;
				p.push_varstr32(m_strOriPacket.data(), m_strOriPacket.length());
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGid;
				p >> m_uFid;
				p >> m_uOriUri;
				m_strOriPacket = p.pop_varstr32();
			}
		};

		//ginfo向gtransfer返回填充填充好成员后的包
		struct PSS_GFillMembersRes  : public sox::Marshallable 
		{
			enum {uri = (16 << 8) | protocol::GTRANS_SVID};

			PSS_GFillMembersRes() {};
			PSS_GFillMembersRes(uint32_t Uri, const std::string &strOriPacket) 
			{
				m_uOriUri = Uri;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}
			PSS_GFillMembersRes(uint32_t Uri, const sox::Marshallable &objMarshal) 
			{
				m_uOriUri = Uri;
				protocol::gtransfer::Packet2String(objMarshal, m_strOriPacket);
			}

			void SetGidFid(uint32_t Gid, uint32_t Fid)
			{
				m_uGid = Gid;
				m_uFid = Fid;
			}

			void SetMembers(const std::vector<uint32_t> &vecMembers)
			{
				m_vecMembers.assign(vecMembers.begin(), vecMembers.end());
			}

			uint32_t m_uGid; //群ID
			uint32_t m_uFid; //组ID
			std::vector<uint32_t> m_vecMembers; //群/组成员列表

			uint32_t m_uOriUri; //原包uri，解析并派发原包时使用
			std::string m_strOriPacket; //原包数据

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGid;
				p << m_uFid;
				sox::marshal_container(p, m_vecMembers);

				p << m_uOriUri;
				p.push_varstr32(m_strOriPacket.data(), m_strOriPacket.length());
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGid;
				p >> m_uFid;
				sox::unmarshal_container(p, back_inserter(m_vecMembers));

				p >> m_uOriUri;
				m_strOriPacket = p.pop_varstr32();
			}
		};
	}
}
