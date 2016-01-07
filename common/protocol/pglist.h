#ifndef PGLIST_H
#define PGLIST_H

#include "common/unistr.h"
#include "common/packet.h"
#include "common/int_types.h"
#include "common/res_code.h"
#include "common/core/base_svid.h"

#include "protocol/IGroupList.h"

#include <vector>
#include <string>
#include <utility>

namespace protocol {
namespace glist {


//********************** protocols ****************************

// packet to get group list of uid.
struct PCS_GetGList: public sox::Voidmable 
{
	enum { uri = (1 << 8) | GLIST_SVID};
};

// packet to return the result of getting group list of uid.
struct PCS_GetGListRes: public sox::Marshallable
{
	enum { uri = (1 << 8) | GLIST_SVID};

	uint32_t resCode;
	std::vector<uint32_t> m_vecGrpIds;
	virtual void marshal(sox::Pack &p) const {
		p << resCode;
		marshal_container(p, m_vecGrpIds);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> resCode;
		unmarshal_container(p, std::back_inserter(m_vecGrpIds)); 
	}
};

// packet to add a m_uGrpId to group list of uid.
struct PCS_AddGList: public sox::Marshallable 
{
	enum {uri = (2 << 8 | GLIST_SVID)};

	uint32_t m_uGrpId;
	bool bCreateGroup; // true for create group; false for join group.
	virtual void marshal(sox::Pack &p) const {
		p << m_uGrpId << bCreateGroup;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uGrpId >> bCreateGroup;
	}
};

// packet to notify imTranfer the result of AddGList, so
// that imTranfer can disable the cached gList of uid.
struct PCS_AddGListRes: public sox::Marshallable 
{
	enum {uri = (2 << 8 | GLIST_SVID)};

	uint32_t m_uUid;
	uint32_t m_uGrpId;
	uint32_t resCode;
	//bool bCreateGroup; // true for create group; false for join group.
	virtual void marshal(sox::Pack &p) const {
		p << m_uUid << m_uGrpId << resCode;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uUid >> m_uGrpId >> resCode;
	}
};

// packet to remove a m_uGrpId from group list of uid.
struct PCS_RemoveGList: public sox::Marshallable {
	enum {uri = (3 << 8 | GLIST_SVID)};

	uint32_t m_uGrpId;
	bool bDismissGrp;
	virtual void marshal(sox::Pack &p) const {
		p << m_uGrpId << bDismissGrp;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uGrpId >> bDismissGrp;
	}	
};
/*
// packet to remove a m_uGrpId from group list of uid.
struct PCS_RemoveGListUids: public sox::Marshallable {
	enum {uri = (3 << 8 | GLIST_SVID)};

	// groupInfo(gInfo) may kick several members at
	// a time, so vector here.
	std::vector<uint32_t> m_vecUIds;
	uint32_t m_uGrpId;
	virtual void marshal(sox::Pack &p) const {
		p << m_uGrpId;
		marshal_container(p, m_vecUIds);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uGrpId;
		unmarshal_container(p, std::back_inserter(m_vecUIds));
	}
};
*/
/*
// packet to notify imTranfer the result of RemoveGList, so
// that imTranfer can disable the cached gList of uid.
struct PCS_RemoveGListRes: public sox::Marshallable {
	enum {uri = (3 << 8 | GLIST_SVID)};

	uint32_t m_uGrpId;
	uint32_t resCode;
	virtual void marshal(sox::Pack &p) const {
		p << m_uGrpId << resCode;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uGrpId >> resCode;
	}		
};
*/
// *************** receive mode *************

// packet to get receive modes of uid's glist.
struct PCS_GetRecvModes: public sox::Voidmable
{
	enum { uri = (4 << 8) | GLIST_SVID};
};

// packet to return the result of getting receive modes of uid 
struct PCS_GetRecvModesRes: public sox::Marshallable
{
	enum { uri = (4 << 8) | GLIST_SVID};

	uint32_t resCode;
	//std::vector<CRecvMode> m_vecRecvModes;
//	std::map<uint32_t, std::vector<protocol::glist::CRecvMode> > m_mapRecvModes;
    std::map<uint32_t, std::set<protocol::glist::CRecvMode> > m_mapRecvModes;//Justin fang -TODO: BUSYBOX-OPTIMIZE

	virtual void marshal(sox::Pack &p) const 
	{
		p << resCode;
		marshal_container(p, m_mapRecvModes);
	}
	virtual void unmarshal(const sox::Unpack &p)
	{
		p >> resCode;
		unmarshal_container(p, std::inserter(m_mapRecvModes, m_mapRecvModes.begin()));
	}
};

struct PCS_SetRecvMode: public sox::Marshallable
{
	enum { uri = (5 << 8) | GLIST_SVID};

	uint32_t m_uGrpId;
	uint32_t m_uFolderId;
	ERecvMode m_eRecvMode;
	virtual void marshal(sox::Pack &p) const {
		p << m_uGrpId << m_uFolderId << (uint32_t) m_eRecvMode;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		uint32_t uRecvMode;
		p >> m_uGrpId >> m_uFolderId >> uRecvMode;
		m_eRecvMode = (ERecvMode) uRecvMode;
	}
};

struct PCS_SetRecvModeRes: public sox::Marshallable
{
	enum { uri = (5 << 8) | GLIST_SVID};

	uint32_t resCode;
	// need to return mode to update protocol cache.
	uint32_t m_uGrpId;
	uint32_t m_uFolderId;
	ERecvMode m_eRecvMode;
	virtual void marshal(sox::Pack &p) const {
		p << resCode << m_uGrpId << m_uFolderId << (uint32_t) m_eRecvMode; 
	}
	virtual void unmarshal(const sox::Unpack &p) {
		uint32_t uRecvMode;
		p >> resCode >> m_uGrpId >> m_uFolderId >> uRecvMode;
		m_eRecvMode = (ERecvMode) uRecvMode;
	}
};

// **************** group alias ***************

struct PCS_GetGrpAlias: public sox::Marshallable
{
	enum { uri = (9 << 8) | GLIST_SVID};
	uint32_t m_uUid;

	virtual void marshal(sox::Pack &p) const {
		p << m_uUid;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uUid;
	}
};

struct PCS_GetGrpAliasRes: public sox::Marshallable
{
	enum { uri = (9 << 8) | GLIST_SVID};

	std::map<uint32_t, std::string> m_mapGrpBynames;//Justin fang -TODO: BUSYBOX-LCACHE
	uint32_t resCode;
	virtual void marshal(sox::Pack &p) const
	{
		marshal_container(p, m_mapGrpBynames);
		p << resCode;
	}
	virtual void unmarshal(const sox::Unpack &p)
	{
		sox::unmarshal_container(p, std::inserter(m_mapGrpBynames, m_mapGrpBynames.begin()));
		p >> resCode;
	}
};

struct PCS_SetGrpAlias: public sox::Marshallable
{
	enum { uri = (10 << 8) | GLIST_SVID};

	CGroupAlias m_grpAlias;
	virtual void marshal(sox::Pack &p) const {
		p << m_grpAlias;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_grpAlias;
	}
};

struct PCS_SetGrpAliasRes: public sox::Marshallable
{
	enum { uri = (10 << 8) | GLIST_SVID};

	// need to return group alias, tell protocol update if remove grpAlias.
	CGroupAlias m_grpAlias;
	uint32_t resCode;
	virtual void marshal(sox::Pack &p) const {
		p << m_grpAlias << resCode;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_grpAlias >> resCode;
	}
};
/*
struct PCS_DelGrpAlias: public sox::Marshallable
{
	enum { uri = (11 << 8) | GLIST_SVID};

	uint32_t m_uGrpId;
	virtual void marshal(sox::Pack &p) const {
		p << m_uGrpId;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uGrpId;
	}
};

struct PCS_DelGrpAliasRes: public sox::Marshallable
{
	enum { uri = (11 << 8) | GLIST_SVID};

	uint32_t m_uGrpId;
	uint32_t resCode;
	virtual void marshal(sox::Pack &p) const {
		p << m_uGrpId << resCode;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uGrpId >> resCode;
	}
};
*/
// *************** refuse topic ***********

struct PCS_GetRefuTopics: public sox::Voidmable
{
	enum { uri = (6 << 8) | GLIST_SVID};
};

struct PCS_GetRefuTopicsRes: public sox::Marshallable
{
	enum { uri = (6 << 8) | GLIST_SVID};

	uint32_t resCode;
	std::map<uint32_t, std::set<uint32_t> > m_mapRefuTopics;
	virtual void marshal(sox::Pack &p) const {
		p << resCode;
		marshal_container(p, m_mapRefuTopics);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> resCode;
		unmarshal_container(p, std::inserter(m_mapRefuTopics, m_mapRefuTopics.begin()));
	}
};

// "set" means "add"
struct PCS_SetRefuTopic: public sox::Marshallable
{
	enum { uri = (7 << 8) | GLIST_SVID};

	uint32_t m_uGroupId; // not used at server-end
	uint32_t m_uFolderId; // set on this ID. (folder in group)
	uint32_t m_uTopicId;
	virtual void marshal(sox::Pack &p) const {
		p << m_uGroupId << m_uFolderId << m_uTopicId;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uGroupId >> m_uFolderId >> m_uTopicId;
	}
};

struct PCS_SetRefuTopicRes: public sox::Marshallable
{
	enum { uri = (7 << 8) | GLIST_SVID};

	uint32_t m_uGroupId; // help UI to find the groupId
	uint32_t m_uFolderId;
	uint32_t m_uTopicId;
	uint32_t resCode;
	virtual void marshal(sox::Pack &p) const {
		p << m_uGroupId << m_uFolderId  << m_uTopicId << resCode;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uGroupId >> m_uFolderId >> m_uTopicId >> resCode;
	}
};

// "cancel" means "remove"
struct PCS_CancelRefuTopic: public sox::Marshallable
{
	enum { uri = (8 << 8) | GLIST_SVID};

	uint32_t m_uGroupId; // not used at server-end
	uint32_t m_uFolderId; // cancel refuse on this ID. (folder in group)
	uint32_t m_uTopicId;
	virtual void marshal(sox::Pack &p) const {
		p << m_uGroupId << m_uFolderId << m_uTopicId;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uGroupId >> m_uFolderId >> m_uTopicId;
	}
};

struct PCS_CancelRefuTopicRes: public sox::Marshallable
{
	enum { uri = (8 << 8) | GLIST_SVID};

	uint32_t m_uGroupId; // not used at server-end
	uint32_t m_uFolderId; // cancel refuse on this ID. (folder in group)
	uint32_t m_uTopicId;
	uint32_t resCode;
	virtual void marshal(sox::Pack &p) const {
		p << m_uGroupId << m_uFolderId << m_uTopicId << resCode;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uGroupId >> m_uFolderId >> m_uTopicId >> resCode;
	}
};

// *************** care topic ***********

/*
struct CCareTopicVec: public sox::Marshallable{
	std::vector<CCareTopic> m_vecCareTopic;
	CCareTopicVec& operator = (const CCareTopicVec& objCareTopicVec){
		m_vecCareTopic = objCareTopicVec.m_vecCareTopic;
		return *this;
	}
	virtual void marshal(sox::Pack &p) const {
		marshal_container(p, m_vecCareTopic);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		unmarshal_container(p, std::back_inserter(m_vecCareTopic));
	}
};*/


struct PCS_GetCareTopics: public sox::Voidmable
{
	enum { uri = (12 << 8) | GLIST_SVID};
};

struct PCS_GetCareTopicsRes: public sox::Marshallable
{
	enum { uri = (12 << 8) | GLIST_SVID};

	uint32_t resCode;
	std::map<uint32_t, std::set<CCareTopic> > m_mapCareTopics;
	virtual void marshal(sox::Pack &p) const {
		p << resCode;
		marshal_container(p, m_mapCareTopics);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> resCode;
		sox::unmarshal_container(p, std::inserter(m_mapCareTopics, m_mapCareTopics.begin()));
	}
};

struct PCS_AddCareTopic: public sox::Marshallable
{
	enum { uri = (13 << 8) | GLIST_SVID};

	uint32_t m_uGroupId; // not used at server-end
	uint32_t m_uFolderId; // set on this ID. (folder in group)
	uint32_t m_uTopicId;
	ECareType m_eCareType;
	virtual void marshal(sox::Pack &p) const {
		p << m_uGroupId << m_uFolderId << m_uTopicId << (uint32_t)m_eCareType;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uGroupId >> m_uFolderId >> m_uTopicId;
		uint32_t uCareType;
		p >> uCareType;
		m_eCareType = (ECareType)uCareType;
	}
};

struct PCS_AddCareTopicRes: public sox::Marshallable
{
	enum { uri = (13 << 8) | GLIST_SVID};

	uint32_t m_uGroupId; // help UI to find the groupId
	uint32_t m_uFolderId;
	uint32_t m_uTopicId;
	ECareType m_eCareType;
	uint32_t resCode;
	virtual void marshal(sox::Pack &p) const 
	{
		p << resCode << m_uGroupId << m_uFolderId << m_uTopicId << (uint32_t)m_eCareType ;
	}
	virtual void unmarshal(const sox::Unpack &p) 
	{
		p >> resCode >> m_uGroupId >> m_uFolderId >> m_uTopicId;
		uint32_t uCareType;
		p >> uCareType;
		m_eCareType = (ECareType)uCareType;
	}
};

struct PCS_RemoveCareTopic: public sox::Marshallable
{
	enum { uri = (14 << 8) | GLIST_SVID};

	uint32_t m_uGroupId; // not used at server-end
	uint32_t m_uFolderId; // cancel refuse on this ID. (folder in group)
	uint32_t m_uTopicId;
	virtual void marshal(sox::Pack &p) const {
		p << m_uGroupId << m_uFolderId << m_uTopicId;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uGroupId >> m_uFolderId >> m_uTopicId;
	}
};

struct PCS_RemoveCareTopicRes: public sox::Marshallable
{
	enum { uri = (14 << 8) | GLIST_SVID};

	uint32_t m_uGroupId; // not used at server-end
	uint32_t m_uFolderId; // cancel care on this ID. (folder in group)
	uint32_t m_uTopicId;
	uint32_t resCode;
	virtual void marshal(sox::Pack &p) const {
		p << m_uGroupId << m_uFolderId << m_uTopicId << resCode;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uGroupId >> m_uFolderId >> m_uTopicId >> resCode;
	}
};

// gTopic removes topic, so have to remove the topic in caredTopic and refusedTopic
struct PCS_RemoveTopic: public sox::Marshallable
{
	enum { uri = (15 << 8) | GLIST_SVID};

	uint32_t m_uTopicId;
	virtual void marshal(sox::Pack &p) const {
		p << m_uTopicId;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uTopicId;
	}
};

struct PCS_BatchAddCareTopic: public sox::Marshallable
{
	enum { uri = (16 << 8) | GLIST_SVID};

	uint32_t m_uGroupId;
	uint32_t m_uFolderId;
	std::vector<CCareTopic> m_vecCareTopics;
	virtual void marshal(sox::Pack &p) const {
		p << m_uGroupId << m_uFolderId;
		marshal_container(p, m_vecCareTopics);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uGroupId >> m_uFolderId;
		unmarshal_container(p, std::back_inserter(m_vecCareTopics));
	}
};

struct PCS_BatAddCareTopicRes: public sox::Marshallable
{
	enum { uri = (17 << 8) | GLIST_SVID};

	uint32_t m_uGroupId;
	uint32_t m_uFolderId;
	std::vector<CCareTopic> m_vecCareTopics;
	virtual void marshal(sox::Pack &p) const {
		p << m_uGroupId << m_uFolderId;
		marshal_container(p, m_vecCareTopics);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uGroupId >> m_uFolderId;
		unmarshal_container(p, std::back_inserter(m_vecCareTopics));
	}
};

// "set" means "add". batch set
struct PCS_BatSetRefuTopic: public sox::Marshallable
{
	enum { uri = (18 << 8) | GLIST_SVID};

	uint32_t m_uGroupId; // not used at server-end
	uint32_t m_uFolderId; // set on this ID. (folder in group)
	std::vector<uint32_t> m_vecTopicId;
	virtual void marshal(sox::Pack &p) const {
		p << m_uGroupId << m_uFolderId;
		marshal_container(p, m_vecTopicId);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uGroupId >> m_uFolderId;
		unmarshal_container(p, std::back_inserter(m_vecTopicId));
	}
};

struct PCS_BatSetRefuTopicRes: public sox::Marshallable
{
	enum { uri = (19 << 8) | GLIST_SVID};

	uint32_t m_uGroupId; // not used at server-end
	uint32_t m_uFolderId; // set on this ID. (folder in group)
	std::vector<uint32_t> m_vecTopicId;
	virtual void marshal(sox::Pack &p) const {
		p << m_uGroupId << m_uFolderId;
		marshal_container(p, m_vecTopicId);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uGroupId >> m_uFolderId;
		unmarshal_container(p, std::back_inserter(m_vecTopicId));
	}
};

//Justin fang -TODO: BUSYBOX-DCHTA
// *************** D-Chat Mode ***********
//Request msg to set D-Chat Mode, from protocol.
struct PCS_SetDChatMode: public sox::Marshallable
{
    enum { uri = (20 << 8) | GLIST_SVID};

    uint32_t m_uGrpId;
    uint32_t m_uFolderId;
    EDChatMode m_eDChatMode;
    virtual void marshal(sox::Pack &p) const
    {
        p << m_uGrpId << m_uFolderId << (uint32_t)m_eDChatMode;
    }
    virtual void unmarshal(const sox::Unpack &p)
    {
        uint32_t uDChatMode;
        p >> m_uGrpId >> m_uFolderId >> uDChatMode;
        m_eDChatMode = (EDChatMode)uDChatMode;
    }
};

//Justin fang -TODO: BUSYBOX-DCHTA
//Respond msg from glist to protocol.
struct PCS_SetDChatModeRes: public sox::Marshallable
{
    enum { uri = (21 << 8) | GLIST_SVID};

    uint32_t m_uResCode;
    uint32_t m_uGrpId;
    uint32_t m_uFolderId;
    EDChatMode m_eDChatMode;
    virtual void marshal(sox::Pack &p) const
    {
        p<<m_uResCode<<m_uGrpId<<m_uFolderId<<(uint32_t)m_eDChatMode;
    }
    virtual void unmarshal(const sox::Unpack &p)
    {
        uint32_t uDChatMode;
        p>>m_uResCode>>m_uGrpId>>m_uFolderId>>uDChatMode;
        m_eDChatMode = (EDChatMode)uDChatMode;
    }
};

//Justin fang -TODO:BUSYBOX-BETA6
struct PCS_SetMsgRecvMode: public sox::Marshallable
{
    enum { uri = (22 << 8) | GLIST_SVID};

    //member list.
    uint32_t m_uGid;
    uint32_t m_uFid;
    EMsgRcvMode m_eMsgRcvMode;

    //construct && deconstruct
    PCS_SetMsgRecvMode(void):
        m_uGid(0),
        m_uFid(0),
        m_eMsgRcvMode(E_MSG_RCV_DEFAULT)
        {}
        
    PCS_SetMsgRecvMode(const uint32_t uGid, const uint32_t uFid):
        m_uGid(uGid),
        m_uFid(uFid),
        m_eMsgRcvMode(E_MSG_RCV_DEFAULT)
        {}
    
    PCS_SetMsgRecvMode(const uint32_t uGid, const uint32_t uFid, const EMsgRcvMode eMsgRcvMode):
        m_uGid(uGid),
        m_uFid(uFid),
        m_eMsgRcvMode(eMsgRcvMode)
        {}

    ~PCS_SetMsgRecvMode(void){}

    //marshal && unmarshal
    virtual void marshal(sox::Pack &p) const
    {
        p<<m_uGid<<m_uFid<<(uint32_t)m_eMsgRcvMode;
    }
    virtual void unmarshal(const sox::Unpack &p)
    {
        uint32_t uMsgRcvMode;
        
        p>>m_uGid>>m_uFid>>uMsgRcvMode;
        m_eMsgRcvMode = (EMsgRcvMode)uMsgRcvMode;
    }
};

//Justin fang -TODO:BUSYBOX-BETA6
struct PCS_SetMsgRecvModeRes: public sox::Marshallable
{
    enum { uri = (23 << 8) | GLIST_SVID};

    //member list.
    uint32_t resCode;
    uint32_t m_uGid;
    uint32_t m_uFid;
    EMsgRcvMode m_eMsgRcvMode;

    //construct && deconstruct
    PCS_SetMsgRecvModeRes(void) :
        resCode(RES_SUCCESS),
        m_uGid(0),
        m_uFid(0),
        m_eMsgRcvMode(E_MSG_RCV_DEFAULT)
        {}
        
    PCS_SetMsgRecvModeRes(const uint32_t uRes, const uint32_t uGid, const uint32_t uFid) :
        resCode(uRes),
        m_uGid(uGid),
        m_uFid(uFid),
        m_eMsgRcvMode(E_MSG_RCV_DEFAULT)
        {}
    
    PCS_SetMsgRecvModeRes(const uint32_t uRes, const uint32_t uGid, const uint32_t uFid, const EMsgRcvMode eMsgRcvMode) :
        resCode(uRes),
        m_uGid(uGid),
        m_uFid(uFid),
        m_eMsgRcvMode(eMsgRcvMode)
        {}

    ~PCS_SetMsgRecvModeRes(void){}

    //marshal && unmarshal
    virtual void marshal(sox::Pack &p) const
    {
        p<<resCode<<m_uGid<<m_uFid<<(uint32_t)m_eMsgRcvMode; 
    }
    
    virtual void unmarshal(const sox::Unpack &p)
    {
        uint32_t uMsgRcvMode;
        
        p>>resCode>>m_uGid>>m_uFid>>uMsgRcvMode;
        m_eMsgRcvMode = (EMsgRcvMode) uMsgRcvMode;
    }
};

} // glist
} // protocol

#endif

