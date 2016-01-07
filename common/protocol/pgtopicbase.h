#ifndef PGTOPICBASE_H_INCLUDED
#define PGTOPICBASE_H_INCLUDED

#include <vector>
#include <string>

#include "common/packet.h" // for "sox::Marshallable"
#include "common/iproperty.h"
#include "common/protocol/const.h"
#include "common/core/base_svid.h"
namespace protocol {
namespace gtopic {
	struct GTopicBaseInfoSvr: public sox::Marshallable{
		GTopicBaseInfoSvr()
		{
			uTopicID=0;
			uGrpID=0;
			uUID=0;
			uCreateTime=0;
			uModifyTime=0;
			uFlag=0;
			uPermission=0;
		}

		GTopicBaseInfoSvr(
			uint32_t uTopicID,
			uint32_t uGrpID,
			uint32_t uUID,
			uint32_t uCreateTime,
			uint32_t uModifyTime,
			uint32_t uFlag,
			uint32_t uPermission,
			const std::string& strTitle,
			const std::string& strDescription):
			uTopicID(uTopicID),
			uGrpID(uGrpID),
			uUID(uUID),
			uCreateTime(uCreateTime),
			uModifyTime(uModifyTime),
			uFlag(uFlag),
			uPermission(uPermission),
			strTitle(strTitle),
			strDescription(strDescription)
		{
		}
		
		uint32_t uTopicID;
		uint32_t uGrpID;
		uint32_t uUID;
		uint32_t uCreateTime;
		uint32_t uModifyTime;
		uint32_t uFlag;
		uint32_t uPermission;
		std::string strTitle;
		std::string strDescription;

		virtual void marshal(sox::Pack &p) const
		{
			p << uTopicID 
			<< uGrpID 
			<< uUID 
			<< uCreateTime 
			<< uModifyTime 
			<< uFlag
			<< uPermission
			<< strTitle 
			<< strDescription;
		}
		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> uTopicID 
			>> uGrpID 
			>> uUID 
			>> uCreateTime 
			>> uModifyTime 
			>> uFlag
			>> uPermission
			>> strTitle 
			>> strDescription;
		}
	};

	enum EGTopicInfoType
	{
		ENUM_GROUP_TOPIC_SIMPLE= 0,
		ENUM_GROUP_TOPIC_DETAIL= 1,
	};

	struct GTopicSimpleInfo{
		GTopicSimpleInfo()
		{
			m_eInfoType= ENUM_GROUP_TOPIC_SIMPLE;
			m_uTopicID=0;
			m_uGrpID=0;
			m_uUID=0;
			m_uCreateTime=0;
			m_uModifyTime=0;
			m_uFlag=0;
			m_uPermission=0;
		}
		
		GTopicSimpleInfo(
			uint32_t uTopicID,
			uint32_t uGrpID,
			uint32_t uUID,
			uint32_t uCreateTime,
			uint32_t uModifyTime,
			uint32_t uFlag,
			uint32_t uPermission,
			std::wstring& strTitle):
			m_eInfoType(ENUM_GROUP_TOPIC_SIMPLE),
			m_uTopicID(uTopicID),
			m_uGrpID(uGrpID),
			m_uUID(uUID),
			m_uCreateTime(uCreateTime),
			m_uModifyTime(uModifyTime),
			m_uFlag(uFlag),
			m_uPermission(uPermission),
			m_strTitle(strTitle)
		{
		}
		
		EGTopicInfoType m_eInfoType;
		uint32_t m_uTopicID;
		uint32_t m_uGrpID;
		uint32_t m_uUID;
		uint32_t m_uCreateTime;
		uint32_t m_uModifyTime;
		uint32_t m_uFlag;
		uint32_t m_uPermission;
		std::wstring m_strTitle;
	};

	struct GTopicDetailInfo:public GTopicSimpleInfo{
		GTopicDetailInfo():GTopicSimpleInfo()
		{
		}
		
		GTopicDetailInfo(
			uint32_t uTopicID,
			uint32_t uGrpID,
			uint32_t uUID,
			uint32_t uCreateTime,
			uint32_t uModifyTime,
			uint32_t uFlag,
			uint32_t uPermission,
			std::wstring& strTitle,
			std::wstring& strDesc):
			GTopicSimpleInfo(
				uTopicID,
				uGrpID,
				uUID,
				uCreateTime,
				uModifyTime,
				uFlag,
				uPermission,
				strTitle
			),
			m_strDescription(strDesc)
		{
			m_eInfoType=ENUM_GROUP_TOPIC_DETAIL;
		}

		std::wstring m_strDescription;
	};
	/*
	struct GTopicBaseInfo{
		GTopicBaseInfo()
		{
			uTopicID=0;
			uGrpID=0;
			uUID=0;
			uCreateTime=0;
			uModifyTime=0;
//			uLastReplyTime=0;
//			uReplyCount=0;
			uFlag=0;
			uPermission=0;
		}
		GTopicBaseInfo& operator = (const GTopicBaseInfo& obj)
		{
			if (this == &obj)
			{
				return *this;
			}
			uTopicID = obj.uTopicID;
			uGrpID = obj.uGrpID;
			uUID = obj.uUID;
			uCreateTime = obj.uCreateTime;
			uModifyTime = obj.uModifyTime;
			uFlag = obj.uFlag;
			uPermission = obj.uPermission;
			return *this;
		}
		
		uint32_t uTopicID;
		uint32_t uGrpID;
		uint32_t uUID;
		uint32_t uCreateTime;
		uint32_t uModifyTime;
//		uint32_t uLastReplyTime;
//		uint32_t uReplyCount;
		uint32_t uFlag;
		uint32_t uPermission;
		std::wstring strTitle;
		std::wstring strDescription;
	}

	struct GidFidTopicInfo{
		GidFidTopicInfo()
		{
			uGrpID=0;
			uFldrID=0;
			TopicInfo = NULL;
		}
		GidFidTopicInfo(const uint32_t gid,const uint32_t fid,GTopicBaseInfo* topic):
			uGrpID(gid),
			uFldrID(fid),
			TopicInfo(topic)
		{
		}

		uint32_t uGrpID;
		uint32_t uFldrID;
		GTopicBaseInfo* TopicInfo;
	};
	*/

	union GTopicReqMask
	{
		struct Bits {
			bool ModifyTime:1;
			bool Flag:1;
			bool Title:1;
			bool Description:1;
			bool Permission:1;
//			bool TopicID:1;
//			bool GrpID:1;
//			bool UID:1;
//			bool CreateTime:1;
//			bool LastReplyTime:1;
//			bool ReplyCount:1;
		} b;
		/*
		// Filter constants.
		// Not very safe. The ordering is architecture dependent.
		enum
		{
			ENUM_FILTER_MODIFY = 0x1,      // 0001
			ENUM_FILTER_PINNED = 0x2,      // 0010
			ENUM_FILTER_TITLE = 0x4,      // 0100
			ENUM_FILTER_DESC = 0x8,       // 1000
		};
		*/
		uint32_t uMaskWord;
	};

	enum GTopicPermission
	{
		ENUM_GTOPIC_ALL,            //Allow all the user in the group/folder to reply to this topic
		ENUM_GTOPIC_ADMIN,       //Only allow the admin of the group/folder to reply to this topic
		ENUM_GTOPIC_NONE,        // None of the user is allowed to reply
		ENUM_GTOPIC_UNKNOWN  //Abnormal value
	};

	union GTopicStateMask
	{
		struct Bits {
			bool Pinned:1;  // ÖÃ¶¥
			bool prime:1;	// ¾«»ª
		} b;
		uint32_t uMaskWord;
	};

	struct GIDFID:public sox::Marshallable{
		GIDFID(){
			uGrpID = 0;
			uFldrID = 0;
		}
		
		GIDFID(const uint32_t gid,const uint32_t fid):
			uGrpID(gid),
			uFldrID(fid)
		{
		}
		
		uint32_t uGrpID;
		uint32_t uFldrID;
		
		virtual void marshal(sox::Pack &p) const
		{
			p << uGrpID << uFldrID;
		}
		
		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> uGrpID >> uFldrID;
		}
	};

	struct FIDTID:public sox::Marshallable{
		FIDTID(){
			uFldrID = 0;
			uTopicID = 0;
		}
		
		FIDTID(const uint32_t fid,const uint32_t tid):
			uFldrID(fid),
			uTopicID(tid)
		{
		}
			
		uint32_t uFldrID;
		uint32_t uTopicID;
		
		void marshal(sox::Pack &p) const
		{
			p << uTopicID << uFldrID;
		}
		
		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> uTopicID >> uFldrID;
		}
	};

	struct GIDFIDTID:public sox::Marshallable{
		GIDFIDTID()
		{
			uGrpID = 0;
			uFldrID = 0;
			uTopicID = 0;
		}
		
		GIDFIDTID(const uint32_t gid,const uint32_t fid,const uint32_t tid):
			uGrpID(gid),
			uFldrID(fid),
			uTopicID(tid)
		{
		}
		
		void marshal(sox::Pack &p) const
		{
			p << uGrpID << uFldrID << uTopicID;
		}
		
		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> uGrpID >> uFldrID >> uTopicID;
		}
		
		bool operator <(const GIDFIDTID other) const {
			if (uGrpID < other.uGrpID)
				return true;
			else if (uFldrID < other.uFldrID)
				return true;
			else if (uTopicID < other.uTopicID)
				return true;
			else
				return false;
		}
		
		uint32_t uGrpID;
		uint32_t uFldrID;
		uint32_t uTopicID;
	};

	struct STopicReadInfo : public sox::Marshallable
	{
		GIDFIDTID      m_uTopicId;      // topic id
		uint32_t       m_uSumMsgNum;    // sum msg number
		uint32_t       m_uUpdateTime;   // latesttime

		virtual void marshal(sox::Pack &p) const
		{
			p << m_uTopicId << m_uSumMsgNum << m_uUpdateTime;
		}

		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> m_uTopicId >> m_uSumMsgNum >> m_uUpdateTime;
		}
	};

	struct STopicReadInfoA : public sox::Marshallable
	{
		//uint32_t       m_uTopicId;      // topic id
		uint32_t       m_uSumMsgNum;    // sum msg number
		uint32_t       m_uUpdateTime;   // latesttime

		virtual void marshal(sox::Pack &p) const
		{
			p << m_uSumMsgNum << m_uUpdateTime;
		}

		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> m_uSumMsgNum >> m_uUpdateTime;
		}
	};
/*	struct LatestTimeCount:public sox::Marshallable{
		LatestTimeCount()
		{
			m_uTopicID = 0;
			m_uLatestTime = 0;
			m_uCounter = 0;
		}
		
		LatestTimeCount(const uint32_t uTID,const uint32_t uTime,const uint32_t uCounter):
			m_uTopicID(uTID),
			m_uLatestTime(uTime),
			m_uCounter(uCounter)
		{
		}
		
		void marshal(sox::Pack &p) const
		{
			p << m_uTopicID << m_uLatestTime << m_uCounter;
		}
		
		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> m_uTopicID >> m_uLatestTime >> m_uCounter;
		}
		
		uint32_t m_uTopicID;
		uint32_t m_uLatestTime;
		uint32_t m_uCounter;
	};

	typedef std::vector<LatestTimeCount> GTopicLatestTimeCountVectorT;

	struct GidFidTopicInfo:public sox::Marshallable{
		GidFidTopicInfo(){
			uGrpID=0;
			uFldrID=0;
		}
		GidFidTopicInfo(const uint32_t gid,const uint32_t fid,const GTopicInfo topic):uGrpID(gid),uFldrID(fid),vecTopicInfo(topic){}
		uint32_t uGrpID;
		uint32_t uFldrID;
		GTopicInfo vecTopicInfo;
		void marshal(sox::Pack &p) const
		{
			p << uGrpID << uFldrID;
			vecTopicInfo.marshal(p);
		}
		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> uGrpID >> uFldrID;
			vecTopicInfo.unmarshal(p);
		}
	};*/
	
} // gtopic
} // protocol

#endif
