#pragma once

#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"
#include "common/protocol/IGrpListDataType.h"

#include <set>

using namespace std;
using namespace protocol::glist;

namespace server
{
	namespace glistcache
	{
		const uint8_t N_GRPLIST_BIT_POS = 0;
		const uint8_t N_RECVMODE_BIT_POS = 1;
		const uint8_t N_GRPBYNAME_BIT_POS = 2;//Justin fang -TODO: BUSYBOX-LCACHE
        const uint8_t N_GRPTOPICPROP_BIT_POS = 3;
        const uint8_t N_JNADMINFIDLIST_BIT_POS = 4;
        const uint8_t N_DCHATMODE_BIT_POS = 5;//Justin fang -TODO: BUSYBOX-DCHAT
        const uint8_t N_MSGRCVMODE_BIT_POS = 6;//Justin fang -TODO:BUSYBOX-BETA6
        
		//将结构体转换成字符串
		inline void Packet2String(const sox::Marshallable &objMarshal, std::string &strPacket)
		{
			//将原包序列化，并装载
			sox::PackBuffer pb;
			sox::Pack pk(pb);
			objMarshal.marshal(pk);

			strPacket.assign(pk.data(), pk.size());
		}

		//imonline2通知glistcache用户状态改变

		struct PSS_GUserStatChanged : public sox::Marshallable
		{
			enum {uri = (1 << 8) | protocol::GLISTCACHE_SVID};

			PSS_GUserStatChanged() {};
			PSS_GUserStatChanged(uint32_t uUid, protocol::E_IMSTATUS emStats)
			{
				m_uUid = uUid;
				m_emStats = emStats;
			}

			uint32_t m_uUid;
			protocol::E_IMSTATUS m_emStats;

			virtual void marshal(sox::Pack &p) const{
				p << m_uUid;
				p.push_uint8(m_emStats);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid;
				m_emStats = (protocol::E_IMSTATUS)p.pop_uint8();
			}
		};

		//glistcache向glist请求用户群列表

		struct PSS_GFillUserGroups : public sox::Marshallable
		{
			enum {uri = (2 << 8) | protocol::GLISTCACHE_SVID};

			PSS_GFillUserGroups() {};
			PSS_GFillUserGroups(uint32_t Uri, const std::string &strOriPacket)
			{
				m_uOriUri = Uri;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}
			PSS_GFillUserGroups(uint32_t Uri, const sox::Marshallable &objMarshal)
			{
				m_uOriUri = Uri;
				Packet2String(objMarshal, m_strOriPacket);
			}
			void SetUid(uint32_t uUid)
			{
				m_uUid = uUid;
			}

			uint32_t m_uUid;
			uint32_t m_uOriUri; //原包uri，解析并派发原包时使用
			std::string m_strOriPacket; //原包数据

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid;
				p << m_uOriUri;
				p << m_strOriPacket;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid;
				p >> m_uOriUri;
				p >> m_strOriPacket;
			}
		};

		//glist向glistcache回应用户群列表信息

		struct PSS_GFillUserGroupsRes : public sox::Marshallable
		{
			enum {uri = (3 << 8) | protocol::GLISTCACHE_SVID};

			uint32_t m_uUid;
			std::set<uint32_t> m_setGrpId; //回应的群列表

			uint32_t m_uOriUri; //原包uri，解析并派发原包时使用
			std::string m_strOriPacket; //原包数据

			PSS_GFillUserGroupsRes() {};
			PSS_GFillUserGroupsRes(uint32_t Uri, const std::string &strOriPacket)
			{
				m_uOriUri = Uri;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}
			PSS_GFillUserGroupsRes(uint32_t Uri, const sox::Marshallable &objMarshal)
			{
				m_uOriUri = Uri;
				Packet2String(objMarshal, m_strOriPacket);
			}
			void SetUid(uint32_t uUid)
			{
				m_uUid = uUid;
			}
			void SetGroups(const std::set<uint32_t> &setGroupId)
			{
				m_setGrpId = setGroupId;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid;
				sox::marshal_container(p, m_setGrpId);

				p << m_uOriUri;
				p << m_strOriPacket;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid;
				sox::unmarshal_container(p, std::inserter(m_setGrpId, m_setGrpId.begin()));

				p >> m_uOriUri;
				p >> m_strOriPacket;
			}
		};

		//glist通知glistcache更新指定用户的群列表
		struct PSS_GUserGroupsUpdate : public sox::Marshallable
		{
			enum {uri = (4 << 8) | protocol::GLISTCACHE_SVID};

            //member list.
			bool m_bAddOrDel;// update type: 1 means add, 0 means del.
			uint32_t m_uGroupId;
            set<uint32_t> m_setUid;
//			std::vector<uint32_t> m_vecUid;//Justin fang -TODO:BUSYBOX-BETA6

            //construct && deconstruct
			PSS_GUserGroupsUpdate() {};

            PSS_GUserGroupsUpdate(const bool bAddorDel, const uint32_t uGid, const set<uint32_t> &cnSetUids)
            {
                m_bAddOrDel = bAddorDel;
                m_uGroupId = uGid;
                m_setUid = cnSetUids;
            }

            ~PSS_GUserGroupsUpdate()
            {
                m_setUid.clear();
            }
#if 0
			PSS_GUserGroupsUpdate(const std::vector<uint32_t> &vecUid, bool bAddOrDel, uint32_t uGroupId)
			{
				m_bAddOrDel = bAddOrDel;
				m_uGroupId = uGroupId;
				m_vecUid.assign(vecUid.begin(), vecUid.end());
			}
#endif

            //marshal && unmarshal.
			virtual void marshal(sox::Pack &p) const
			{
				p << m_bAddOrDel << m_uGroupId;
//				sox::marshal_container(p, m_vecUid);
                sox::marshal_container(p, m_setUid);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_bAddOrDel >>m_uGroupId;
//				sox::unmarshal_container(p, back_inserter(m_vecUid));
                sox::unmarshal_container(p, std::inserter(m_setUid, m_setUid.begin()));
			}
		};


		//其它服务器通知glistcache广播数据到用户的所有群友

		struct PSS_GUserBoardcast : public sox::Marshallable
		{
			enum {uri = (5 << 8) | protocol::GLISTCACHE_SVID};

			PSS_GUserBoardcast() {};
			PSS_GUserBoardcast(uint32_t Uri, const std::string &strOriPacket)
			{
				m_uOriUri = Uri;
				m_strOriPacket.assign(strOriPacket.data(), strOriPacket.size());
			}
			PSS_GUserBoardcast(uint32_t Uri, const sox::Marshallable &objMarshal)
			{
				m_uOriUri = Uri;
				Packet2String(objMarshal, m_strOriPacket);
			}
			void SetUid(uint32_t uUid)
			{
				m_uUid = uUid;
			}

			uint32_t m_uUid;
			uint32_t m_uOriUri; //原包uri，解析并派发原包时使用
			std::string m_strOriPacket; //原包数据

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid;
				p << m_uOriUri;
				p << m_strOriPacket;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid;
				p >> m_uOriUri;
				p >> m_strOriPacket;
			}
		};


		//其它服务器通知glistcache批量广播数据到用户的所有群友

		struct PSS_BatchGUserBoardcast  : public sox::Marshallable
		{
			enum {uri = (8 << 8) | protocol::GLISTCACHE_SVID};

			PSS_BatchGUserBoardcast() {};

			std::vector<PSS_GUserBoardcast> m_vecBroadcast;

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_vecBroadcast);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, back_inserter(m_vecBroadcast));
			}
		};

		//glist向glistcache回应用户群别名列表信息
		struct PSS_GetGrpAliasRes : public sox::Marshallable
		{
			enum {uri = (9 << 8) | protocol::GLISTCACHE_SVID};

			PSS_GetGrpAliasRes() {};

			void SetUid(uint32_t uUid)
			{
				m_uUid = uUid;
			}
			void SetGrpAlias(const std::map<uint32_t, std::string> mapGrpAlias)//TOU
			{
				m_mapGrpBynames = mapGrpAlias;//Justin fang -TODO: BUSYBOX-LCACHE
			}

			uint32_t m_uUid;
			std::map<uint32_t, std::string> m_mapGrpBynames; //回应的群别名列表

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid;
				sox::marshal_container(p, m_mapGrpBynames);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid;
				sox::unmarshal_container(p, std::inserter(m_mapGrpBynames, m_mapGrpBynames.begin()));
			}
		};


		//glist通知glistcache更新指定用户的群别名列表
		struct PSS_GrpAliasUpdate: public sox::Marshallable
		{
			enum {uri = (10 << 8) | protocol::GLISTCACHE_SVID};

			uint32_t m_uUid;
			uint32_t m_uGroupId;
			std::string m_strGrpAlias;

			PSS_GrpAliasUpdate() {};

			PSS_GrpAliasUpdate(uint32_t uUid, uint32_t uGroupId, const std::string& strGrpAlias){
				m_uUid = uUid;
				m_uGroupId = uGroupId;
				m_strGrpAlias = strGrpAlias;
			}

			virtual void marshal(sox::Pack &p) const{
				p << m_uUid << m_uGroupId << m_strGrpAlias;
			}

			virtual void unmarshal(const sox::Unpack &p){
				p >> m_uUid >> m_uGroupId >> m_strGrpAlias;
			}
		};

		///////////////////////////////////////////接收模式///////////////////////////////////////////////

		//glist向glistcache回应用户群接收模式列表信息
		struct PSS_GetRecvModesRes : public sox::Marshallable
		{
			enum {uri = (11 << 8) | protocol::GLISTCACHE_SVID};

			uint32_t m_uUid;
			std::map<uint32_t, std::vector<CRecvMode> > m_mapRecvModes;//回应的群接收模式列表

			void SetUid(uint32_t uUid){
				m_uUid = uUid;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid;
				sox::marshal_container(p, m_mapRecvModes);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid;
				unmarshal_container(p, std::inserter(m_mapRecvModes, m_mapRecvModes.begin()));
			}
		};

		//glist通知glistcache更新指定用户的群接收模式列表
		struct PSS_RecvModeUpdate: public sox::Marshallable
		{
			enum {uri = (12 << 8) | protocol::GLISTCACHE_SVID};

			// update type: 1 means add, 0 means del.
			bool m_bAddOrDel;
			uint32_t m_uUid;
			uint32_t m_uGroupId;
			uint32_t m_uFolderId;
			ERecvMode m_eRecvMode;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uGroupId << m_uFolderId << (uint32_t)m_eRecvMode;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				uint32_t recvMode;
				p >> m_uUid >> m_uGroupId >> m_uFolderId >> recvMode;
				m_eRecvMode = (ERecvMode)recvMode;
			}
		};

		///////////////////////////////////////////拒收主题///////////////////////////////////////////////

		//glist向glistcache回应用户群拒收主题列表信息
		struct PSS_GetRefuTopicsRes : public sox::Marshallable
		{
			enum {uri = (13 << 8) | protocol::GLISTCACHE_SVID};

			uint32_t m_uUid;
			std::map<uint32_t, std::set<uint32_t> > m_mapRefuTopics; //回应的群拒收主题列表

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid;
				sox::marshal_container(p, m_mapRefuTopics);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid;
				sox::unmarshal_container(p, std::inserter(m_mapRefuTopics, m_mapRefuTopics.begin()));
			}
		};

		//glist通知glistcache更新指定用户的群拒收主题列表
		struct PSS_RefuTopicUpdate: public sox::Marshallable
		{
			enum {uri = (14 << 8) | protocol::GLISTCACHE_SVID};

			// update type: 1 means add, 0 means del.
			bool m_bAddOrDel;
			uint32_t m_uUid;
			uint32_t m_uFolderId;
			uint32_t m_uTopicId;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_bAddOrDel << m_uUid << m_uFolderId << m_uTopicId;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_bAddOrDel >> m_uUid >> m_uFolderId >> m_uTopicId;
			}
		};

		///////////////////////////////////////////关注主题///////////////////////////////////////////////

		//glist向glistcache回应用户群拒收主题列表信息
		struct PSS_GetCareTopicsRes: public sox::Marshallable
		{
			enum {uri = (15 << 8) | protocol::GLISTCACHE_SVID};

			uint32_t m_uUid;
			std::map<uint32_t, std::set<CCareTopic> > m_mapCareTopics;; //回应的群拒收主题列表

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid;
				sox::marshal_container(p, m_mapCareTopics);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid;
				sox::unmarshal_container(p, std::inserter(m_mapCareTopics, m_mapCareTopics.begin()));
			}
		};

		//glist通知glistcache更新指定用户的群接收模式列表
		struct PSS_CareTopicUpdate: public sox::Marshallable
		{
			enum {uri = (16 << 8) | protocol::GLISTCACHE_SVID};

			// update type: 1 means add, 0 means del.
			bool m_bAddOrDel;
			uint32_t m_uUid;
			uint32_t m_uFolderId;
			uint32_t m_uTopicId;
			ECareType m_eCareType;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_bAddOrDel << m_uUid << m_uFolderId << m_uTopicId << (uint32_t)m_eCareType;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				uint32_t careType;
				p >> m_bAddOrDel >> m_uUid >> m_uFolderId >> m_uTopicId >> careType;
				m_eCareType = (ECareType)careType;
			}
		};

		//glist通知glistcache解散了1个群
		struct PSS_DismissGrp : public sox::Marshallable
		{
			enum {uri = (17 << 8) | protocol::GLISTCACHE_SVID};
			uint32_t m_uGroupId;
			std::set<uint32_t> m_setFolderId;
			std::vector<uint32_t> m_vecUid;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId;
				sox::marshal_container(p, m_setFolderId);
				sox::marshal_container(p, m_vecUid);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId;
				sox::unmarshal_container(p, std::inserter(m_setFolderId, m_setFolderId.begin()));
				sox::unmarshal_container(p, back_inserter(m_vecUid));
			}
		};

		//glist通知glistcache解散了1个组
		struct PSS_DismissFolder : public sox::Marshallable
		{
			enum {uri = (18 << 8) | protocol::GLISTCACHE_SVID};
			uint32_t m_uFolderId;
			std::vector<uint32_t> m_vecUid;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uFolderId;
				sox::marshal_container(p, m_vecUid);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uFolderId;
				sox::unmarshal_container(p, back_inserter(m_vecUid));
			}
		};

		//解散群、组时, glist通知glistcache批量删除用户的群、组接收模式列表
		struct PSS_DelRecvModes: public sox::Marshallable
		{
			enum {uri = (19 << 8) | protocol::GLISTCACHE_SVID};

			uint32_t m_uGroupId;
			uint32_t m_uFolderId;
			//std::set<uint32_t> m_setUid;
			std::vector<uint32_t> m_vecUid;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uFolderId;
				sox::marshal_container(p, m_vecUid);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId >> m_uFolderId;
				//sox::unmarshal_container(p, std::inserter(m_setUid, m_setUid.begin()));
				sox::unmarshal_container(p, back_inserter(m_vecUid));
			}
		};

#if 0
		//解散群、组时, glist通知glistcache批量删除用户的群、组拒收主题列表
		struct PSS_DelRefuTopics: public sox::Marshallable
		{
			enum {uri = (20 << 8) | protocol::GLISTCACHE_SVID};

			uint32_t m_uGroupId;
			uint32_t m_uFolderId;
			//std::set<uint32_t> m_setUid;
			std::vector<uint32_t> m_vecUid;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uFolderId;
				sox::marshal_container(p, m_vecUid);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId >> m_uFolderId;
				//sox::unmarshal_container(p, std::inserter(m_setUid, m_setUid.begin()));
				sox::unmarshal_container(p, back_inserter(m_vecUid));
			}
		};

		//解散群、组时, glist通知glistcache批量删除用户的群、组关注主题列表
		struct PSS_DelCareTopics: public sox::Marshallable
		{
			enum {uri = (21 << 8) | protocol::GLISTCACHE_SVID};

			uint32_t m_uGroupId;
			uint32_t m_uFolderId;
			//std::set<uint32_t> m_setUid;
			std::vector<uint32_t> m_vecUid;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGroupId << m_uFolderId;
				sox::marshal_container(p, m_vecUid);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGroupId >> m_uFolderId;
				//sox::unmarshal_container(p, std::inserter(m_setUid, m_setUid.begin()));
				sox::unmarshal_container(p, back_inserter(m_vecUid));
			}
		};
#endif
        //解散群、组时, glist通知glistcache批量删除用户的群、组关注/拒收主题列表
        struct PSS_DelTopicPropAsDelFolder : public sox::Marshallable
        {
            enum {uri = (105 << 8) | protocol::GLISTCACHE_SVID};//TODO

            uint32_t m_uGroupId;
            uint32_t m_uFolderId;
            std::set<uint32_t> m_setUids;
            //std::vector<uint32_t> m_vecUid;

            virtual void marshal(sox::Pack &p) const
            {
                p << m_uGroupId << m_uFolderId;
                sox::marshal_container(p, m_setUids);
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                p >> m_uGroupId >> m_uFolderId;
                sox::unmarshal_container(p, std::inserter(m_setUids, m_setUids.begin()));
                //sox::unmarshal_container(p, back_inserter(m_vecUid));
            }
        };

		// 退出/踢出 组
		struct PSS_ExitFolder: public sox::Marshallable
		{
			enum {uri = (22 << 8) | protocol::GLISTCACHE_SVID};

			uint32_t m_uUid;
			uint32_t m_uGroupId;
			uint32_t m_uFolderId;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uGroupId << m_uFolderId;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_uGroupId >> m_uFolderId;
			}
		};

		// 退出/踢出 群
		struct PSS_ExitGroup: public sox::Marshallable
		{
			enum {uri = (23 << 8) | protocol::GLISTCACHE_SVID};

			uint32_t m_uUid;
			uint32_t m_uGroupId;
			uint32_t m_uFolderId;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uGroupId << m_uFolderId;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_uGroupId >> m_uFolderId;
			}
		};

		//glist通知glistcache"批量"更新指定用户的群关注主题列表
		struct PSS_BatCareTopicUpdate: public sox::Marshallable
		{
			enum {uri = (24 << 8) | protocol::GLISTCACHE_SVID};

			uint32_t m_uUid;
			uint32_t m_uFolderId;
			std::vector<CCareTopic> m_vecCareTopics;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uFolderId;
				marshal_container(p, m_vecCareTopics);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_uFolderId;
				unmarshal_container(p, std::back_inserter(m_vecCareTopics));
			}
		};

		//glist通知glistcache"批量"更新指定用户的群拒收主题列表
		struct PSS_BatRefuTopicUpdate: public sox::Marshallable
		{
			enum {uri = (25 << 8) | protocol::GLISTCACHE_SVID};

			uint32_t m_uUid;
			uint32_t m_uFolderId;
			std::vector<uint32_t> m_vecTopicId;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uFolderId;
				marshal_container(p, m_vecTopicId);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_uFolderId;
				unmarshal_container(p, std::back_inserter(m_vecTopicId));
			}
		};

        //Justin fang -TODO: BUSYBOX-FID
        /**********************************
         * User's Joined FID List and Admin FID List.  *
         **********************************/
        /*Obj Class - */
        struct MyJoinOrAdminFldsUpdateList : public sox::Marshallable
        {
            //Define enum type Request
            enum EFldsListUpdateReq
            {
                E_JOINED_FID_CLEAR = 0, //Clear up UID's Joined FID List.
                E_JOINED_FID_ADD,    //Add a Joined FID to List for UID.
                E_JOINED_FID_DEL,    //Delete a Joined FID from List for UID.

                E_ADMIN_FID_CLEAR = 0xFF, //Clear up UID's Admin FID List.
                E_ADMIN_FID_ADD,       //Add a Admin FID to List for UID.
                E_ADMIN_FID_DEL,       //Delete a Admin FID to List for UID.

                E_FID_INVALID_REQUEST = 0xFFFFFFFF //Invalid
            };

            //Members List
            uint32_t m_uUID;
            EFldsListUpdateReq m_eFldsListUpdateReq;
            std::set<uint32_t> m_setFldList;

            MyJoinOrAdminFldsUpdateList() :
                m_uUID(0),
                m_eFldsListUpdateReq(E_FID_INVALID_REQUEST)
            {}

            MyJoinOrAdminFldsUpdateList(EFldsListUpdateReq eReuqest, uint32_t uUID):
                m_uUID(uUID),
                m_eFldsListUpdateReq(eReuqest)
            {}

            MyJoinOrAdminFldsUpdateList(EFldsListUpdateReq eReuqest, uint32_t uUID, const std::set<uint32_t>& setFldList):
                m_uUID(uUID),
                m_eFldsListUpdateReq(eReuqest),
                m_setFldList(setFldList)
            {}

            ~MyJoinOrAdminFldsUpdateList(){}

            //Key define
//            bool operator < (const myjoinoradminfldsupdatelist &objjoinedadminfldsuplist) const
//            {
//                return (m_uuid < objjoinedadminfldsuplist.m_ufolderid);
//            }

            //Marshal & Unmarshal
            virtual void marshal(sox::Pack &p) const
            {
                p<<m_uUID<<(uint32_t)m_eFldsListUpdateReq;
				sox::marshal_container(p, m_setFldList);
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                uint32_t uRequest;

                p>>m_uUID;
                p>>uRequest;
                m_eFldsListUpdateReq = (EFldsListUpdateReq)uRequest;
                sox::unmarshal_container(p, std::inserter(m_setFldList, m_setFldList.begin()));
            }
        };

        /*Msg - Ginfo notifies glistcache to update Joined FID List or Admin FID List.*/
        struct PSS_JoinedAdminFldsUpdate : public sox::Marshallable
        {
            typedef std::vector<MyJoinOrAdminFldsUpdateList> typeUpMemList;//vector<[UID, ReqType]>
            typedef std::vector<MyJoinOrAdminFldsUpdateList>::const_iterator typecnitUpMemList;//vector<[UID, ReqType]>

            enum {uri = (52 << 8) | protocol::GLISTCACHE_SVID};

            uint32_t m_uGid;
            typeUpMemList m_UpdateMemList;

            PSS_JoinedAdminFldsUpdate(){}
            PSS_JoinedAdminFldsUpdate(uint32_t uGid):m_uGid(uGid){}

            virtual void marshal(sox::Pack &p) const
            {
                p << m_uGid;
                sox::marshal_container(p, m_UpdateMemList);
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                p >> m_uGid;
                sox::unmarshal_container(p, std::back_inserter(m_UpdateMemList));
            }
        };

        //Justin fang -TODO: BUSYBOX-DCHAT LCACHE
        //glist notifies glistcache to set D-Chat Mode item.
        struct PSS_SetDChatModeItem: public sox::Marshallable
        {
            enum {uri = (27 << 8) | protocol::GLISTCACHE_SVID};
        
            uint32_t m_uUid;
            uint32_t m_uGroupId;
            uint32_t m_uFolderId;
            EDChatMode m_eDChatMode;
        
            virtual void marshal(sox::Pack &p) const
            {
                p<<m_uUid<<m_uGroupId<<m_uFolderId<<(uint32_t)m_eDChatMode;
            }
        
            virtual void unmarshal(const sox::Unpack &p)
            {
                uint32_t uDChatMode;
                p>>m_uUid>>m_uGroupId>>m_uFolderId>>uDChatMode;
                m_eDChatMode = (EDChatMode)uDChatMode;
            }
        };

        //Justin fang -TODO: BUSYBOX-DCHAT
        /*glist notifies glistcache to delete DChat Mode.*/
		struct PSS_DelFidDChatMode: public sox::Marshallable
		{
			enum {uri = (28 << 8) | protocol::GLISTCACHE_SVID};

			uint32_t m_uGid;
			uint32_t m_uFid;
            std::set<uint32_t> m_setUids;

			virtual void marshal(sox::Pack &p) const
			{
				p<<m_uGid<<m_uFid;
                sox::marshal_container(p, m_setUids);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p>>m_uGid>>m_uFid;
                sox::unmarshal_container(p, std::inserter(m_setUids, m_setUids.begin()));
			}
		};

        //Justin fang -TODO: BUSYBOX-GM
        /**
         * Gmonitor requests to get Group List.
         **/
		struct PSS_GetUserGrpList: public sox::Marshallable
		{
			enum {uri = (29 << 8) | protocol::GLISTCACHE_SVID};

            //Member list.
			uint32_t m_uUid;
            uint32_t m_uSvidDist;//((SVID << 8) |(Sub Flag))

            //Construct && Deconstruct
            PSS_GetUserGrpList() : 
                m_uUid(0), 
                m_uSvidDist(0xFF00)
                {}

            PSS_GetUserGrpList(uint32_t uUid, uint32_t uSvidDist) : 
                m_uUid(uUid),
                m_uSvidDist(uSvidDist)
                {}

            ~PSS_GetUserGrpList(){}    

            //Marshal && unmarshal
			virtual void marshal(sox::Pack &p) const
			{
				p<<m_uUid<<m_uSvidDist;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p>>m_uUid>>m_uSvidDist;
			}
		};

        //Justin fang -TODO: BUSYBOX-GM
        /**
         * Answer gmonitor with User's Group List.
         **/
		struct PSS_GetUserGrpListRes: public sox::Marshallable
		{
			enum {uri = (30 << 8) | protocol::GLISTCACHE_SVID};

            //Member list.
			uint32_t m_uUid;
            uint32_t m_uSvidDist;//((SVID << 8) |(Sub Flag))
            std::set<uint32_t> m_setGids;

            //Construct && Deconstruct
            PSS_GetUserGrpListRes() {}

            ~PSS_GetUserGrpListRes() {}
            
            //Marshal && unmarshal
			virtual void marshal(sox::Pack &p) const
			{
				p<<m_uUid<<m_uSvidDist;
                sox::marshal_container(p, m_setGids);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p>>m_uUid>>m_uSvidDist;
                sox::unmarshal_container(p, std::inserter(m_setGids, m_setGids.begin()));
			}
		};        

        //Justin fang -TODO: BUSYBOX-2DBTB
        //GList notifies GListCache to delete a Topic.
        struct PSS_DelTopic : public sox::Marshallable
        {
            enum {uri = (26 << 8) | protocol::GLISTCACHE_SVID};
            uint32_t m_uGrpFldId;
            uint32_t m_uTopicId;
            std::set<uint32_t> m_setUids;
        
            virtual void marshal(sox::Pack &p) const
            {
                p <<m_uGrpFldId<<m_uTopicId;
                sox::marshal_container(p, m_setUids);
            }
        
            virtual void unmarshal(const sox::Unpack &p) 
            {
                p >>m_uGrpFldId>>m_uTopicId;
                sox::unmarshal_container(p, std::inserter(m_setUids, m_setUids.begin()));
            }
        };

        //Justin fang -TODO:BUSYBOX-BETA6
        struct PSS_MsgRcvModeUpdate: public sox::Marshallable
        {
            enum {uri = (31 << 8) | protocol::GLISTCACHE_SVID};

            //member list.
            uint32_t m_uUid;
            uint32_t m_uGid;
            uint32_t m_uFid;
            EMsgRcvMode m_eMsgRcvMode;

            //marshal && unmarshal
            virtual void marshal(sox::Pack &p) const
            {
                p<<m_uUid<<m_uGid<<m_uFid<<(uint32_t)m_eMsgRcvMode;
            }
        
            virtual void unmarshal(const sox::Unpack &p)
            {
                uint32_t uMsgRcvMode;
                
                p>>m_uUid>>m_uGid>>m_uFid>>uMsgRcvMode;
                m_eMsgRcvMode = (EMsgRcvMode)uMsgRcvMode;
            }
        };

        //Justin fang -TODO:BUSYBOX-BETA6D
        struct PSS_DelMsgRcvModes: public sox::Marshallable
        {
            enum {uri = (32 << 8) | protocol::GLISTCACHE_SVID};
        
            uint32_t m_uGid;
            uint32_t m_uFid;
            std::set<uint32_t> m_setUids;
        
            virtual void marshal(sox::Pack &p) const
            {
                p << m_uGid << m_uFid;
                sox::marshal_container(p, m_setUids);
            }
        
            virtual void unmarshal(const sox::Unpack &p)
            {
                p >> m_uGid >> m_uFid;
                sox::unmarshal_container(p, std::inserter(m_setUids, m_setUids.begin()));
            }
        };


	}
}
