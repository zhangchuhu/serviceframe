#pragma once 

#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"
#include "common/protocol/IGrpListDataType.h"

#include <iostream>
#include <string>

namespace protocol
{
	namespace glistcache
	{

		// 向glistCache请求个人群列表
		struct PCS_SyncGList : public sox::Marshallable
		{
			enum {uri = (6 << 8) | protocol::GLISTCACHE_SVID};

			uint32_t m_uUid;

			void SetUid(uint32_t uUid){
				m_uUid = uUid;
			}

			virtual void marshal(sox::Pack &p) const{
				p << m_uUid;
			}

			virtual void unmarshal(const sox::Unpack &p){
				p >> m_uUid;
			}
		};

		struct PCS_SyncGListRes: public sox::Marshallable
		{
			enum { uri = (7 << 8) | protocol::GLISTCACHE_SVID};

			uint32_t resCode;
			std::set<uint32_t> m_setGrpIds;
			virtual void marshal(sox::Pack &p) const {
				p << resCode;
				marshal_container(p, m_setGrpIds);
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> resCode;
				unmarshal_container(p, std::inserter(m_setGrpIds, m_setGrpIds.begin())); 
			}
		};

		//群成员状态通知
		struct PCS_GUserStatChanged : public sox::Marshallable
		{
			enum {uri = (101 << 8) | protocol::GLISTCACHE_SVID};

			PCS_GUserStatChanged() {};
			PCS_GUserStatChanged(/*uint32_t uGroupId, */uint32_t uUid, protocol::E_IMSTATUS emStats)
			{
				//m_uGroupId = uGroupId;
				m_uUid = uUid;
				m_emStats = emStats;
			}

			//uint32_t m_uGroupId;
			uint32_t m_uUid;
			protocol::E_IMSTATUS m_emStats;

			virtual void marshal(sox::Pack &p) const
			{
				//p << m_uGroupId;
				p << m_uUid;
				p.push_uint8(m_emStats);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				//p >> m_uGroupId;
				p >> m_uUid;
				m_emStats = (protocol::E_IMSTATUS)p.pop_uint8();
			}
		};

		//////////////////////////////////////////////////////////////
		
    	/*************************************************************
    	 * Requests for Multi Lists information: imlinkd->glist/glistcache, glistcache->glist    *
    	 * 1 - Group List                                            *
    	 * 2 - Receive Mode                                          *
    	 * 3 - Refuse Topic                                          *
    	 * 4 - Care Topic                                            *
    	 * 5 - DChat Zone Mode                                       *
    	 *************************************************************/
		struct PSS_SyncMLists : public sox::Marshallable
		{
			enum {uri = (102 << 8) | protocol::GLISTCACHE_SVID};

			uint32_t m_uUid;
			uint8_t m_byBitMask;

			void SetUid(uint32_t uUid)
			{
				m_uUid = uUid;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_byBitMask;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_byBitMask;
			}
		};

		// 获取群列表成功后的回应
        /*************************************************************
         * Respond of Multi Lists information: glist/glistcache->imlinkd, glist->glistcache *
         * 1 - Group List                                            *
         * 2 - Receive Mode                                          *
         * 3 - Refuse Topic                                          *
         * 4 - Care Topic                                            *
         * 5 - DChat Zone Mode                                       *
         *************************************************************/
		struct PCS_SyncMListsRes: public sox::Marshallable
		{
            typedef std::map<uint32_t, std::set<uint32_t> > typeMapRefuseTopics;//map<FID/GID, set<TopicId>>
            typedef std::map<uint32_t, std::set<protocol::glist::CCareTopic> > typeMapCareTopics;////map<FID/GID, set<[TopicId, CareType]>> 
            typedef std::map<uint32_t, std::set<protocol::glist::SDChatMode> > typeMapDChatMode;
            typedef std::map<uint32_t, std::set<protocol::glist::SDChatMode> >::iterator typeitMapDChatMode;
            typedef std::map<uint32_t, std::set<protocol::glist::CRecvMode> > typeMapGFidRcvMode;
            typedef std::map<uint32_t, std::set<protocol::glist::CMsgRcvMode> > typeMapGFidMsgRcvMode;
            
			enum { uri = (103 << 8) | protocol::GLISTCACHE_SVID};

			uint32_t m_uUid;
			uint8_t m_byBitMask;
			std::set<uint32_t> m_setGrpIds;
			typeMapGFidRcvMode m_mapRecvModes;
			//std::map<uint32_t, std::string> m_mapGroupAlias; // alias no need currently.
			typeMapRefuseTopics m_mapRefuTopics;//map<FID/GID, set<TopicID>>
			typeMapCareTopics m_mapCareTopics;//map<FID/GID, set<[TopicId, CareType]>>
			typeMapDChatMode m_mapDChatMode;
            typeMapGFidMsgRcvMode m_mapMsgRcvModes;//Justin fang -TODO:BUSYBOX-BETA6

			virtual void marshal(sox::Pack &p) const{
				p << m_uUid << m_byBitMask;
				marshal_container(p, m_setGrpIds);
				marshal_container(p, m_mapRecvModes);
				marshal_container(p, m_mapRefuTopics);
				marshal_container(p, m_mapCareTopics);
                marshal_container(p, m_mapDChatMode);
                marshal_container(p, m_mapMsgRcvModes);
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> m_uUid >> m_byBitMask;
				unmarshal_container(p, std::inserter(m_setGrpIds, m_setGrpIds.begin())); 
				unmarshal_container(p, std::inserter(m_mapRecvModes, m_mapRecvModes.begin()));
				unmarshal_container(p, std::inserter(m_mapRefuTopics, m_mapRefuTopics.begin()));
				unmarshal_container(p, std::inserter(m_mapCareTopics, m_mapCareTopics.begin()));
                unmarshal_container(p, std::inserter(m_mapDChatMode, m_mapDChatMode.begin()));
                unmarshal_container(p, std::inserter(m_mapMsgRcvModes, m_mapMsgRcvModes.begin()));
			}
		};

		/*
		struct PSS_Sync4ListsRes: public sox::Marshallable
		{
			enum { uri = (104 << 8) | protocol::GLISTCACHE_SVID};

			uint32_t m_uUid;
			uint8_t m_byBitMask;
			std::set<uint32_t> m_setGrpIds;
			//std::map<uint32_t, protocol::glist::ERecvMode> m_mapRecvModes;
			// second is recvMode, use uint32_t for marshallable.
			std::map<uint32_t, uint32_t> m_mapRecvModes;
			//std::map<uint32_t, std::string> m_mapGroupAlias; // alias no need currently.
			std::map<uint32_t, std::set<uint32_t> > m_mapRefuTopics;
			std::map<uint32_t, std::set<protocol::glist::CCareTopic> > m_mapCareTopics;

			virtual void marshal(sox::Pack &p) const{
				marshal_container(p, m_setGrpIds);
				marshal_container(p, m_mapRecvModes);
				marshal_container(p, m_mapRefuTopics);
				marshal_container(p, m_mapCareTopics);
			}

			virtual void unmarshal(const sox::Unpack &p) {
				unmarshal_container(p, std::inserter(m_setGrpIds, m_setGrpIds.begin())); 
				unmarshal_container(p, std::inserter(m_mapRecvModes, m_mapRecvModes.begin()));
				unmarshal_container(p, std::inserter(m_mapRefuTopics, m_mapRefuTopics.begin()));
				unmarshal_container(p, std::inserter(m_mapCareTopics, m_mapCareTopics.begin()));
			}
		};*/
		
	}
}
