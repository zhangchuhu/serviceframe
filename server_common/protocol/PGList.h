#pragma once

#include "common/packet.h"
#include "common/core/base_svid.h"

namespace server
{

namespace glist
{

	//glistcache向glist请求用户群接收模式列表
	struct PSS_GetRecvModes : public sox::Marshallable
	{
		enum {uri = (100 << 8) | protocol::GLIST_SVID};

		uint32_t m_uUid;

		void SetUid(uint32_t uUid){
			m_uUid = uUid;
		}

		virtual void marshal(sox::Pack &p) const{
			p << m_uUid;
		}

		virtual void unmarshal(const sox::Unpack &p) {
			p >> m_uUid;
		}
	};

	//glistcache向glist请求用户群接收模式列表
	struct PSS_GetGrpAlias : public sox::Marshallable
	{
		enum {uri = (101 << 8) | protocol::GLIST_SVID};

		uint32_t m_uUid;

		void SetUid(uint32_t uUid){
			m_uUid = uUid;
		}

		virtual void marshal(sox::Pack &p) const{
			p << m_uUid;
		}

		virtual void unmarshal(const sox::Unpack &p) {
			p >> m_uUid;
		}
	};

	//glistcache向glist请求用户群拒收主题列表
	struct PSS_GetRefuTopics: public sox::Marshallable
	{
		enum {uri = (102 << 8) | protocol::GLIST_SVID};

		uint32_t m_uUid;

		virtual void marshal(sox::Pack &p) const{
			p << m_uUid;
		}

		virtual void unmarshal(const sox::Unpack &p) {
			p >> m_uUid;
		}
	};

	// packet to add a m_uGrpId to group list of uid.
	struct PSS_AddGList: public sox::Marshallable
	{
		enum {uri = (104 << 8 | protocol::GLIST_SVID)};

		uint32_t m_uGrpId;
		uint32_t m_uUid;
		bool m_bCreateGroup; // true for create group; false for join group.
		virtual void marshal(sox::Pack &p) const {
			p << m_uGrpId << m_uUid << m_bCreateGroup;
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> m_uGrpId >> m_uUid >> m_bCreateGroup;
		}
	};

	struct PSS_CreateOrDelTopic: public sox::Marshallable
	{
		enum {uri = (105 << 8 | protocol::GLIST_SVID)};

		uint32_t m_uUid;
		uint32_t m_uGroupId;
		uint32_t m_uFolderId;
		uint32_t m_uTopicId;
		bool bCreateOrDel; // true for create topic; false for del topic.
		virtual void marshal(sox::Pack &p) const {
			p << m_uUid << m_uGroupId << m_uFolderId << m_uTopicId << bCreateOrDel;
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> m_uUid >> m_uGroupId >> m_uFolderId >> m_uTopicId >> bCreateOrDel;
		}
	};

    //Justin fang -TODO:BUSYBOX-BETA6
    struct PSS_BatchJoinGid: public sox::Marshallable
    {
        enum {uri = (106 << 8 | protocol::GLIST_SVID)};

        //members list
        uint32_t        m_uGid;
        std::set<uint32_t>   m_setUids;

        //construct && deconstruct
        PSS_BatchJoinGid() : m_uGid(0)
        {
            m_setUids.clear();
        }

        PSS_BatchJoinGid(const uint32_t uGid, const std::set<uint32_t> &cnSetUids) :
            m_uGid(uGid),
            m_setUids(cnSetUids)
            {}

        ~PSS_BatchJoinGid()
        {
            m_setUids.clear();
        }

        //marshal && unmarshal
        virtual void marshal(sox::Pack &p) const
        {
            p <<m_uGid;
            sox::marshal_container(p, m_setUids);
        }
        virtual void unmarshal(const sox::Unpack &p)
        {
            p >>m_uGid;
            sox::unmarshal_container(p, std::inserter(m_setUids, m_setUids.begin()));

        }
    };


}

}

