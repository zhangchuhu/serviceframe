#ifndef PGTOPIC_H_INCLUDED
#define PGTOPIC_H_INCLUDED

#include <vector>
#include <string>

#include "common/iproperty.h"
#include "protocol/const.h"
#include "core/base_svid.h"
#include "common/protocol/pgtopicbase.h"
#include "common/packet.h" // for "sox::Marshallable"

#include "PGCheck.h" // use for check in update and delete topic
namespace protocol {
namespace gtopic {
	// gtopic base struct, every request and response have GrpID and FldrID for UI
	struct GTopicBase:public sox::Marshallable
	{
		GTopicBase(){}
		
		GTopicBase(const GTopicBase& msg)
		{
			uGrpID = msg.uGrpID;
			uFldrID = msg.uFldrID;
			uSender = msg.uSender;
		}
		
		GTopicBase(uint32_t gid,uint32_t fid,uint32_t sender):
			uGrpID(gid),
			uFldrID(fid),
			uSender(sender)
		{
		}
		
		uint32_t uGrpID;
		uint32_t uFldrID;
		uint32_t uSender;
		
		virtual void marshal(sox::Pack &p) const
		{
			p << uGrpID << uFldrID << uSender;
		}
		
		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> uGrpID >> uFldrID >> uSender;
		}
	};

	struct PCS_CreateGTopicResBase: public GTopicBase{
		GTopicBaseInfoSvr objTopic;
		
		PCS_CreateGTopicResBase(){}
		
		PCS_CreateGTopicResBase(const PCS_CreateGTopicResBase& msg):
			GTopicBase(msg),
			objTopic(msg.objTopic)
		{
		}
		
		virtual void marshal(sox::Pack &p) const
		{
			GTopicBase::marshal(p);
			objTopic.marshal(p);
		}
		
		virtual void unmarshal(const sox::Unpack &p)
		{
			GTopicBase::unmarshal(p);
			objTopic.unmarshal(p);
		}
	};

	struct PCS_CreateGTopicRes: public PCS_CreateGTopicResBase{
		enum {uri = (11<< 8 | GTOPIC_SVID)};
		
		uint32_t uResCode;
		uint32_t uSeqID;

		virtual void marshal(sox::Pack &p) const
		{
			p << uResCode << uSeqID;
			PCS_CreateGTopicResBase::marshal(p);
		}
		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> uResCode >> uSeqID;
			PCS_CreateGTopicResBase::unmarshal(p);
		}
	};

	struct PCS_CreateGTopicNotify: public PCS_CreateGTopicResBase{
		enum {uri = (101 << 8 | GTOPIC_SVID)};
		
		PCS_CreateGTopicNotify(){}
		
		PCS_CreateGTopicNotify(const PCS_CreateGTopicRes& msg)
			:PCS_CreateGTopicResBase(msg)
		{
		}
	};

	struct PCS_DeleteGTopicResBase: public GTopicBase{
		PCS_DeleteGTopicResBase(){}
		
		PCS_DeleteGTopicResBase(const PCS_DeleteGTopicResBase& msg):
			GTopicBase(msg),uTopicID(msg.uTopicID)
		{
		}
			
		PCS_DeleteGTopicResBase(const uint32_t uGrpID,const uint32_t uFldrID,
			const uint32_t sender,const uint32_t uTopicID):
			GTopicBase(uGrpID,uFldrID,sender),
			uTopicID(uTopicID)
		{
		}
		
		uint32_t uTopicID;

		virtual void marshal(sox::Pack &p) const
		{
			GTopicBase::marshal(p);
			p << uTopicID;
		}
		
		virtual void unmarshal(const sox::Unpack &p)
		{
			GTopicBase::unmarshal(p);
			p >> uTopicID;
		}
	};

	struct PCS_DeleteGTopicRes: public PCS_DeleteGTopicResBase{
		enum {uri = (12 << 8 | GTOPIC_SVID)};
		
		uint32_t uResCode;
		
		PCS_DeleteGTopicRes(){}
		
		virtual void marshal(sox::Pack &p) const
		{
			PCS_DeleteGTopicResBase::marshal(p);
			p << uResCode;
		}
		
		virtual void unmarshal(const sox::Unpack &p)
		{
			PCS_DeleteGTopicResBase::unmarshal(p);
			p >> uResCode;
		}
	};

	struct PCS_DeleteGTopicNotify: public PCS_DeleteGTopicResBase{
		enum {uri = (102 << 8 | GTOPIC_SVID)};
		
		PCS_DeleteGTopicNotify(){}
		
		PCS_DeleteGTopicNotify(const PCS_DeleteGTopicRes& msg)
			:PCS_DeleteGTopicResBase(msg)
		{
		}

		PCS_DeleteGTopicNotify(const uint32_t uGID, const uint32_t uFID,
			const uint32_t uUID, const uint32_t uTID):
			PCS_DeleteGTopicResBase(uGID,uFID,uUID,uTID)
		{
		}
	};

	struct PCS_UpdateGTopicPinnedResBase: public GTopicBase{
		uint32_t uTopicID;
		uint32_t uModifyTime;
		uint32_t uFlag;
		
		PCS_UpdateGTopicPinnedResBase(){}
		
		PCS_UpdateGTopicPinnedResBase(const PCS_UpdateGTopicPinnedResBase& msg):
			GTopicBase(msg),
			uTopicID(msg.uTopicID),
			uModifyTime(msg.uModifyTime),
			uFlag(msg.uFlag)
		{
		}
		
		virtual void marshal(sox::Pack &p) const
		{
			GTopicBase::marshal(p);
			p << uTopicID << uModifyTime << uFlag;
		}
		
		virtual void unmarshal(const sox::Unpack &p)
		{
			GTopicBase::unmarshal(p);
			p >> uTopicID >> uModifyTime >> uFlag;
		}
	};

	struct PCS_UpdateGTopicPinnedRes: public PCS_UpdateGTopicPinnedResBase{
		enum {uri = (13 << 8 | GTOPIC_SVID)};
		
		uint32_t uResCode;
		
		virtual void marshal(sox::Pack &p) const
		{
			PCS_UpdateGTopicPinnedResBase::marshal(p);
			p << uResCode;
		}
		
		virtual void unmarshal(const sox::Unpack &p)
		{
			PCS_UpdateGTopicPinnedResBase::unmarshal(p);
			p >> uResCode;
		}
	};

	struct PCS_UpdateGTopicPinnedNotify: public PCS_UpdateGTopicPinnedResBase{
		enum {uri = (103 << 8 | GTOPIC_SVID)};
		
		PCS_UpdateGTopicPinnedNotify(){}
		
		PCS_UpdateGTopicPinnedNotify(const PCS_UpdateGTopicPinnedRes& msg)
			:PCS_UpdateGTopicPinnedResBase(msg)
		{
		}
	};

	struct PCS_UpdateGTopicInfoResBase: public GTopicBase{
		PCS_UpdateGTopicInfoResBase(){}
		
		PCS_UpdateGTopicInfoResBase(const PCS_UpdateGTopicInfoResBase& msg):
			GTopicBase(msg),
			uMask(msg.uMask),
			objTopic(msg.objTopic)
		{
		}
		
		PCS_UpdateGTopicInfoResBase(const uint32_t uGrpID,const uint32_t uFldrID,
			const uint32_t uSender,const uint32_t uMask,const GTopicBaseInfoSvr& objTopic):
			GTopicBase(uGrpID,uFldrID,uSender),
			uMask(uMask),
			objTopic(objTopic)
		{
		}

		uint32_t uMask;
		GTopicBaseInfoSvr objTopic;
		
		virtual void marshal(sox::Pack &p) const
		{
			GTopicBase::marshal(p);
			p << uMask;
			objTopic.marshal(p);
		}
		
		virtual void unmarshal(const sox::Unpack &p)
		{
			GTopicBase::unmarshal(p);
			p >> uMask;
			objTopic.unmarshal(p);
		}
	};

	struct PCS_UpdateGTopicInfoRes: public PCS_UpdateGTopicInfoResBase{
		enum {uri = (14 << 8 | GTOPIC_SVID)};
		
		uint32_t uResCode;
		uint32_t uSeqID;
		
		virtual void marshal(sox::Pack &p) const
		{
			PCS_UpdateGTopicInfoResBase::marshal(p);
			p << uResCode << uSeqID;
		}
		
		virtual void unmarshal(const sox::Unpack &p)
		{
			PCS_UpdateGTopicInfoResBase::unmarshal(p);
			p >> uResCode >> uSeqID;
		}
	};

	struct PCS_UpdateGTopicInfoNotify: public PCS_UpdateGTopicInfoResBase{
		enum {uri = (104 << 8 | GTOPIC_SVID)};
		
		PCS_UpdateGTopicInfoNotify(){}
		
		PCS_UpdateGTopicInfoNotify(const PCS_UpdateGTopicInfoRes& msg)
			:PCS_UpdateGTopicInfoResBase(msg)
		{
		}
	};
	/*
	struct PCS_GetGTopicIDs: public sox::Marshallable{
		enum {uri = (5 << 8 | GTOPIC_SVID)};
		
		PCS_GetGTopicIDs(){}
		
		PCS_GetGTopicIDs(std::vector<GIDFID>& gidtids):
			vecGidFid(gidtids)
		{
		}

		std::vector<GIDFID> vecGidFid;

		virtual void marshal(sox::Pack &p) const
		{
			marshal_container(p, vecGidFid);
		}
		virtual void unmarshal(const sox::Unpack &p)
		{
			unmarshal_container(p, std::back_inserter(vecGidFid));
		}
	};

	struct PCS_GetGTopicIDsRes: public sox::Marshallable{
		enum {uri = (5 << 8 | GTOPIC_SVID)};

		std::vector<FIDTID> vecFidTid;
		std::vector<GIDFID> vecGidFid;

		PCS_GetGTopicIDsRes(){}

		PCS_GetGTopicIDsRes(std::vector<FIDTID>& vecFidTid,std::vector<GIDFID>& vecGidFid):
			vecFidTid(vecFidTid),
			vecGidFid(vecGidFid)
		{
		}
			
		virtual void marshal(sox::Pack &p) const
		{
			marshal_container(p, vecFidTid);
			marshal_container(p, vecGidFid);
		}
		
		virtual void unmarshal(const sox::Unpack &p)
		{
			unmarshal_container(p, std::back_inserter(vecFidTid));
			unmarshal_container(p, std::back_inserter(vecGidFid));
		}
	};*/

	struct PCS_GetGTopicSimpleInfosReq: public sox::Marshallable{
		enum {uri = (5 << 8 | GTOPIC_SVID)};
		
		// std::vector<GIDFIDTID> vecGidFidTid;
		uint32_t uGrpID;
		uint32_t uFldrID;
		std::vector<uint32_t> vecTopicIDs;
		
		PCS_GetGTopicSimpleInfosReq(){}
		
		PCS_GetGTopicSimpleInfosReq(
			uint32_t uGrpID, uint32_t uFldrID, std::vector<uint32_t>& vecTopicIDs):
			uGrpID(uGrpID),
			uFldrID(uFldrID),
			vecTopicIDs(vecTopicIDs)
		{
		}
		
		virtual void marshal(sox::Pack &p) const
		{
			p << uGrpID
			<< uFldrID;
			marshal_container(p, vecTopicIDs);
		}
		
		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> uGrpID
			>> uFldrID;
			unmarshal_container(p, std::back_inserter(vecTopicIDs));
		}
	};

	struct PCS_GetGTopicSimpleInfosRes: public sox::Marshallable{
		enum {uri = (15 << 8 | GTOPIC_SVID)};

		uint32_t uGrpID;
		uint32_t uFldrID;
		std::vector<GTopicBaseInfoSvr> vecTopicInfos;
		std::vector<uint32_t> vecNotExistTids; // use for check which id in not exist.
		// std::vector<GIDFIDTID> vecGidFidTid;
		
		PCS_GetGTopicSimpleInfosRes(){}
		PCS_GetGTopicSimpleInfosRes(
			uint32_t gid, uint32_t fid, std::vector<GTopicBaseInfoSvr> & vecTemp, std::vector<uint32_t>& vecTid):
			uGrpID(gid),
			uFldrID(fid),
			vecTopicInfos(vecTemp),
			vecNotExistTids(vecTid)
		{
		}

		virtual void marshal(sox::Pack &p) const
		{
			p << uGrpID
			<< uFldrID;
		//	marshal_container(p, vecGidFidTid);
			marshal_container(p, vecTopicInfos);
			marshal_container(p, vecNotExistTids);
		}
		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> uGrpID
			>> uFldrID;
		//	unmarshal_container(p, std::back_inserter(vecGidFidTid));
			unmarshal_container(p, std::back_inserter(vecTopicInfos));
			unmarshal_container(p, std::back_inserter(vecNotExistTids));
		}
	};

	struct PCS_GetGTopicDetailInfosReq: public PCS_GetGTopicSimpleInfosReq{
		enum {uri = (6 << 8 | GTOPIC_SVID)};

		PCS_GetGTopicDetailInfosReq(){}

		PCS_GetGTopicDetailInfosReq(
			uint32_t uGrpID, uint32_t uFldrID, std::vector<uint32_t>& vecTopicIDs):
			PCS_GetGTopicSimpleInfosReq(uGrpID,uFldrID,vecTopicIDs)
		{
		}
	};

	struct PCS_GetGTopicDetailInfosRes: public PCS_GetGTopicSimpleInfosRes{
		enum {uri = (16 << 8 | GTOPIC_SVID)};
		/*PCS_GetGTopicSimpleInfosRes(
			uint32_t gid, uint32_t fid, std::vector<GTopicBaseInfoSvr> & vecTemp, std::vector<uint32_t>& vecTid):
			PCS_GetGTopicSimpleInfosRes(gid,fid,vecTemp,vecTid)
		{
		}*/
	};
	/*
	struct PCS_GetPinnedTopicIDs: public sox::Marshallable{
		enum {uri = (7 << 8 | GTOPIC_SVID)};

		uint32_t uGrpID;
		std::vector<uint32_t> vecFldrIDs;
		
		PCS_GetPinnedTopicIDs(){}
		
		PCS_GetPinnedTopicIDs(const uint32_t gid,const std::vector<uint32_t>& flds ):
			uGrpID(gid),
			vecFldrIDs(flds)
		{
		}

		virtual void marshal(sox::Pack &p) const
		{
			p << uGrpID;
			marshal_container(p, vecFldrIDs);
		}
		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> uGrpID;
			unmarshal_container(p, std::back_inserter(vecFldrIDs));
		}
	};

	struct PCS_GetPinnedTopicIDsRes: public sox::Marshallable{
		enum {uri = (7 << 8 | GTOPIC_SVID)};

		uint32_t uGrpID;		
		std::map<uint32_t, std::vector<uint32_t> > mapFID2TIDs;

		PCS_GetPinnedTopicIDsRes(){}

		PCS_GetPinnedTopicIDsRes(uint32_t gid, std::map<uint32_t, std::vector<uint32_t> > mapFID2TIDs):
			uGrpID(gid),
			mapFID2TIDs(mapFID2TIDs)
		{
		}
			
		virtual void marshal(sox::Pack &p) const
		{
			p <<  uGrpID;	
			marshal_container(p, mapFID2TIDs);
		}
		
		virtual void unmarshal(const sox::Unpack &p)
		{
			p >>  uGrpID;
			unmarshal_container(p, std::inserter(mapFID2TIDs,mapFID2TIDs.begin()));
		}
	};*/
	
	struct PCS_CreateGTopic: public protocol::gcheck::PCS_GroupCommon{
		enum {uri = (1 << 8 | GTOPIC_SVID)};

		PCS_CreateGTopic(){}

		PCS_CreateGTopic(const uint32_t uGrpID,const uint32_t uFldrID,
			const uint32_t sender, const uint32_t uSeqID,
			const std::string& strTitle,const std::string& strDescription,
			const uint32_t uFlag,const uint32_t uPermission):
			PCS_GroupCommon(sender,uGrpID,uFldrID),
			uLinkdID(0),
			uSeqID(uSeqID),
			strTitle(strTitle),
			strDescription(strDescription),
			uFlag(uFlag),
			uPermission(uPermission)
		{
		}

		uint32_t uLinkdID; // this feild is filled by gauth, protocol do not care about it
		uint32_t uSeqID;
		std::string strTitle;
		std::string strDescription;
		uint32_t uFlag;
		uint32_t uPermission;

		virtual void marshal(sox::Pack &p) const
		{
			PCS_GroupCommon::marshal(p);
			p << uLinkdID << uSeqID << uFlag << uPermission << strTitle << strDescription;
		}

		virtual void unmarshal(const sox::Unpack &p)
		{
			PCS_GroupCommon::unmarshal(p);
			p >> uLinkdID >> uSeqID >> uFlag >> uPermission >> strTitle >> strDescription;
		}
	};

	struct PCS_DeleteGTopic: public protocol::gcheck::PCS_GroupCommon{
		enum {uri = (2 << 8 | GTOPIC_SVID)};

		uint32_t uTopicID;
//		bool bBroadcast;

		PCS_DeleteGTopic(){}

		PCS_DeleteGTopic(const uint32_t uGrpID,const uint32_t uFldrID,const uint32_t usender,
			const uint32_t uTopicID)://,const bool bBroadcast):
			PCS_GroupCommon(usender,uGrpID,uFldrID),
			uTopicID(uTopicID)
//			bBroadcast(bBroadcast)
		{
		}

		virtual void marshal(sox::Pack &p) const
		{
			PCS_GroupCommon::marshal(p);
			p << uTopicID;
//			p << bBroadcast;
		}

		virtual void unmarshal(const sox::Unpack &p)
		{
			PCS_GroupCommon::unmarshal(p);
			p >> uTopicID;
//			p >> bBroadcast;
		}
	};

	struct PCS_UpdateGTopicPinned: public protocol::gcheck::PCS_GroupCommon{
		enum {uri = (3 << 8 | GTOPIC_SVID)};

		uint32_t uTopicID;
		uint32_t uFlag;

		PCS_UpdateGTopicPinned(){}

		PCS_UpdateGTopicPinned(const uint32_t uGrpID,const uint32_t uFldrID,const uint32_t uSender,
			const uint32_t uTopicID, const uint32_t uFlag):
			PCS_GroupCommon(uSender,uGrpID,uFldrID),
			uTopicID(uTopicID),
			uFlag(uFlag)
		{
		}

		virtual void marshal(sox::Pack &p) const
		{
			PCS_GroupCommon::marshal(p);
			p << uTopicID << uFlag;
		}

		virtual void unmarshal(const sox::Unpack &p)
		{
			PCS_GroupCommon::unmarshal(p);
			p >> uTopicID >> uFlag;
		}
	};

	struct PCS_UpdateGTopicInfo: public protocol::gcheck::PCS_GroupCommon{
		enum {uri = (4 << 8 | GTOPIC_SVID)};

		uint32_t uSeqID;
		uint32_t uMask;
		GTopicBaseInfoSvr objTopic;

		PCS_UpdateGTopicInfo(const uint32_t uGrpID,const uint32_t uFldrID,const uint32_t uSender,
			const uint32_t uSeqID, const uint32_t uMask, const GTopicBaseInfoSvr& objTopic):
			PCS_GroupCommon(uSender,uGrpID,uFldrID),
			uSeqID(uSeqID),
			uMask(uMask),
			objTopic(objTopic)
		{
		}

		PCS_UpdateGTopicInfo(){}

		virtual void marshal(sox::Pack &p) const
		{
			PCS_GroupCommon::marshal(p);
			p << uSeqID << uMask;
			objTopic.marshal(p);
		}

		virtual void unmarshal(const sox::Unpack &p)
		{
			PCS_GroupCommon::unmarshal(p);
			p >> uSeqID >> uMask;
			objTopic.unmarshal(p);
		}
		
	};

	struct PCS_BatchDeleteGTopic: public protocol::gcheck::PCS_GroupCommon{
		enum {uri = (8 << 8 | GTOPIC_SVID)};

		std::vector<uint32_t> vecTIDs;

		PCS_BatchDeleteGTopic(){}

		PCS_BatchDeleteGTopic(const uint32_t uGrpID,const uint32_t uFldrID,const uint32_t usender,
			const std::vector<uint32_t>& vecTIDs):
			PCS_GroupCommon(usender,uGrpID,uFldrID),
			vecTIDs(vecTIDs)
		{
		}

		virtual void marshal(sox::Pack &p) const
		{
			PCS_GroupCommon::marshal(p);
			marshal_container(p, vecTIDs);
		}

		virtual void unmarshal(const sox::Unpack &p)
		{
			PCS_GroupCommon::unmarshal(p);
			unmarshal_container(p, std::back_inserter(vecTIDs));
		}
	};

	struct PCS_BatchDeleteGTopicResBase: public GTopicBase{
		PCS_BatchDeleteGTopicResBase(){}
		
		PCS_BatchDeleteGTopicResBase(const PCS_BatchDeleteGTopicResBase& msg):
			GTopicBase(msg),vecTIDs(msg.vecTIDs)
		{
		}
			
		PCS_BatchDeleteGTopicResBase(const uint32_t uGrpID,const uint32_t uFldrID,
			const uint32_t sender,const std::vector<uint32_t>& vecTIDs):
			GTopicBase(uGrpID,uFldrID,sender),
			vecTIDs(vecTIDs)
		{
		}
		
		std::vector<uint32_t> vecTIDs;

		virtual void marshal(sox::Pack &p) const
		{
			GTopicBase::marshal(p);
			marshal_container(p, vecTIDs);
		}
		
		virtual void unmarshal(const sox::Unpack &p)
		{
			GTopicBase::unmarshal(p);
			unmarshal_container(p, std::back_inserter(vecTIDs));
		}
	};

	struct PCS_BatchDeleteGTopicRes: public PCS_BatchDeleteGTopicResBase{
		enum {uri = (18 << 8 | GTOPIC_SVID)};
		
		uint32_t uResCode;
		
		PCS_BatchDeleteGTopicRes(){}
		
		virtual void marshal(sox::Pack &p) const
		{
			PCS_BatchDeleteGTopicResBase::marshal(p);
			p << uResCode;
		}
		
		virtual void unmarshal(const sox::Unpack &p)
		{
			PCS_BatchDeleteGTopicResBase::unmarshal(p);
			p >> uResCode;
		}
	};

	struct PCS_BatchDeleteGTopicNotify: public PCS_BatchDeleteGTopicResBase{
		enum {uri = (108 << 8 | GTOPIC_SVID)};
		
		PCS_BatchDeleteGTopicNotify(){}
		
		PCS_BatchDeleteGTopicNotify(const PCS_BatchDeleteGTopicRes& msg)
			:PCS_BatchDeleteGTopicResBase(msg)
		{
		}

		PCS_BatchDeleteGTopicNotify(const uint32_t uGID, const uint32_t uFID,
			const uint32_t uUID, const std::vector<uint32_t>& vecTIDs):
			PCS_BatchDeleteGTopicResBase(uGID,uFID,uUID,vecTIDs)
		{
		}
	};

	
} // gtopic
} // protocol

#endif
