#ifdef HAS_GTOPIC
#ifndef IGTOPIC_H_INCLUDED
#define IGTOPIC_H_INCLUDED

#include "pgtopicbase.h"
#include "common/unistr.h"
#include <string>

namespace protocol{
	namespace gmsgcache{
		struct STopicPopInfo;
	}
}
namespace protocol{
	namespace gtopic{
		struct IGTopicWatcher//UI实现的接口
		{
			virtual ~IGTopicWatcher(){}

			// notify ui the result of creating a topic
			virtual void onCreateGTopicRes(const uint32_t& uResCode,const uint32_t& uGrpID,
				const uint32_t& uFldrID,const GTopicDetailInfo& objTopic)=0;

			// notify ui a topic is created
			virtual void onCreateGTopicNotify(const uint32_t& uGrpID,const uint32_t& uFldrID,
				const GTopicDetailInfo& objTopic)=0;

			// notify ui the result of deleting a topic
			virtual void onDeleteGTopicRes(const uint32_t& uResCode,const uint32_t& uGrpID,
				const uint32_t& uFldrID,const std::vector<uint32_t>& vecTIDs)=0;

			// notify ui a topic is deleted
			virtual void onDeleteGTopicNotify(const uint32_t& uGrpID,const uint32_t& uFldrID,
				const std::vector<uint32_t>& vecTIDs)=0;

			// notify ui the result of updating a topic
			virtual void onUpdateGTopicInfoRes(const uint32_t& uResCode,const uint32_t& uGrpID,
				const uint32_t& uFldrID, const uint32_t& uMask,const GTopicDetailInfo& objTopic)=0;

			// notify ui a topic is modified
			virtual void onUpdateGTopicInfoNotify(const uint32_t& uGrpID,const uint32_t& uFldrID,
				const uint32_t& uMask,const GTopicDetailInfo& objTopic)=0;

			// notify ui the result of updating pinned field of a topic
			virtual void onUpdateGTopicPinnedRes(const uint32_t& uResCode,const uint32_t& uGrpID,
				const uint32_t& uFldrID,const uint32_t& uTopicID, const uint32_t& uFlag,const uint32_t& uModifyTime)=0;

			// notify ui pinned field of a topic is modified
			virtual void onUpdateGTopicPinnedNotify(const uint32_t& uGrpID,const uint32_t& uFldrID,
				const uint32_t& uTopicID, const uint32_t& uFlag,const uint32_t& uModifyTime)=0;

			// notify ui the result of getting topic base infos by topic ids
			virtual void onGetGTopicSimpleInfos(const uint32_t& uGrpID, const uint32_t& uFldrID, 
				const std::vector<GTopicSimpleInfo*>& vecBaseInfos)=0;

			// notify ui the result of getting topic detail info by topic ids
			// different between baseinfo and detail info is description. 
			virtual void onGetGTopicDetailInfos(const uint32_t& uGrpID, const uint32_t& uFldrID, 
				const std::vector<GTopicDetailInfo*>& vecDetailInfos)=0;

			/*
			// notify ui the result of getting topic latest reply time and count
			virtual void onGetGTopicPopInfos(const uint32_t& uGrpID, const uint32_t& uFldrID, 
				const std::vector<protocol::gmsgcache::STopicPopInfo>& vecTopicPopInfos)=0;

			// notify ui the result of getting topic ids by group and folder ids
			virtual void onGetGTopicIDs(const std::vector<GIDFIDTID>& vecTopicIDs)=0;

			// notify ui the result of gettting the sticky topic ids
			// map< uint32_t, std::vector<uint32_t> >
			//             fid      ,       vector< tid >
			virtual void onGetPinnedTopicIDs(const uint32_t& uGrpID, const std::map< uint32_t, std::vector<uint32_t> >& mapFID2TIDs)=0;
			*/

			/// debug ljp
			virtual void onLogCache(const std::string& strlog){
			};
			/// debug ljp end

		};

		struct IOpGTopicWatcher//底层需要实现的接口，供UI调用
		{
			virtual ~IOpGTopicWatcher(){}
			virtual void watch(IGTopicWatcher *w) = 0;
			virtual void revoke(IGTopicWatcher *w) = 0;
		};


		

		struct IGTopic : public virtual IOpGTopicWatcher//底层实现的接口
		{
			virtual ~IGTopic(){}

			// protocol will send request to create a topic 
			virtual void createGTopic(const uint32_t uGrpID,const uint32_t uFldrID,const std::wstring& strTitle,
				const std::wstring& strDescription,const uint32_t uFlag = 0,
				const uint32_t uPermission = ENUM_GTOPIC_ALL) = 0;
			
			// protocol will send request to delete a topic 
			virtual void deleteGTopic(const uint32_t uGrpID,const uint32_t uFldrID,const std::vector<uint32_t>& vecTIDs)=0;
				//,const bool bBroadcast=true) = 0;

			// protocol will send request to update a topic
			// !!!attention: update not include update flag info in GTopicBaseInfo. 
			//                     if you wanted to update flag info please call updateGTopicPinned().
			virtual void updateGTopicInfo(const uint32_t uGrpID,const uint32_t uFldrID,const uint32_t uMask,
				const GTopicDetailInfo& objTopic) = 0;

			// protocol will send request to update a topic in pinned field
			virtual void updateGTopicPinned(const uint32_t uGrpID,const uint32_t uFldrID,const uint32_t uTopicID, 
				const uint32_t uFlag) = 0;

			// protocol will send request to get topic infos by topic ids
			//virtual void getGTopicInfos(std::vector<GIDFIDTID>& vecTids) = 0; 
			//virtual void getGTopicInfos(const uint32_t uGrpID, const uint32_t uFldrID, 
			// 	const std::vector<uint32_t>& vecTids)=0;
			virtual void getGTopicSimpleInfos(const uint32_t uGrpID, const uint32_t uFldrID, 
				const std::vector<uint32_t>& vecTids)=0;

			virtual void getGTopicDetailInfos(const uint32_t uGrpID, const uint32_t uFldrID, 
				const std::vector<uint32_t>& vecTids)=0;

			/*
			// protocol will send request to get topic ids by folder ids
			virtual void getGTopicIDs(std::vector<GIDFID>& vecGidFids) = 0;
			//virtual void getGTopicIDs(const uint32_t uGrpID, const std::vector<uint32_t>& vecFids) = 0;

			// protocol will send request to get topic ids by folder ids
			virtual void getPinnedTopicIDs(const uint32_t uGrpID, const std::vector<uint32_t>& vecFids) = 0;
			*/

			/// clean the cache.
			virtual void reset()=0;

			/// debug ljp
			virtual void printGidTidCache(){};
			virtual void printTopicInfosCache(){};
			/// debug ljp end
		};
	}
}
#endif
#endif