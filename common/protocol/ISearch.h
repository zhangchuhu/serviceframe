/*
Note: In protocol, all data with std::string type must be coded with UTF8
*/
#ifndef ISEARCH_H 
#define ISEARCH_H

#include <vector>
#include "ISearchRecord.h"
#include "pgtopicbase.h"

using namespace protocol::gtopic;

namespace protocol
{
	namespace search
	{

		struct ISearchWatcher
		{
			virtual ~ISearchWatcher(){};
			
			// for UI, take action when PSS_GTopicSearchRes is received
			virtual void OnGTopicSearchRes(const uint32_t &topicCount, const std::vector<GIDFIDTID> &records) = 0;

			// for UI, take action when PSS_GTopicMsgListRes is received
			virtual void OnGTopicMsgListRes(const uint32_t &topicCount, const uint32_t &GID, const uint32_t &FID, const uint32_t &TID, const uint64_t &nextMsg, const std::vector<STopicMsgInfo> &records) = 0;

			// for UI, take action when PSS_GChatMsgListRes is received
			virtual void OnGChatMsgListRes(const uint32_t &topicCount, const uint32_t &GID, const uint32_t &FID, const uint64_t &nextMsg, const std::vector<STopicMsgInfo> &records) = 0;

			//MsgCount==0
			virtual void OnGChatMsgGetRes(const uint32_t &msgCount, const uint32_t &pageno ,const uint32_t &GID, const uint32_t &FID, const std::vector<STopicMsgInfo> &records) = 0;

			// for UI, take action when PSS_GTopicListRes is received
			virtual void OnGTopicListRes(const uint32_t &FID, const uint32_t &GID, const std::vector<uint32_t> &records) = 0;

			// for UI, take action when PCS_GTopicMsgCountRes is received
			virtual void OnGTopicMsgCountRes(const uint32_t &TID, const uint32_t &FID, const uint32_t &GID, const uint64_t &startMsgID, const uint64_t &stopMsgID) = 0;

			// for UI, take action when PCS_GetGrpByIdRes is received
			virtual void OnGetGrpByIdRes(const uint32_t &retCode, const protocol::search::CGSearchRecordData &objRecord) = 0;

			// for UI, take action when PCS_GetGrpByNameRes is received
			virtual void OnGetGrpByNameRes(const uint32_t &retCode, const std::vector<protocol::search::CGSearchRecordData> &vecRecords) = 0;

			// for UI, take action when PCS_GetGrpByIdRes is received
			virtual void OnGetGrpByIdRes2(const uint32_t &retCode, const protocol::search::CGSearchRecordData2 &objRecord) = 0;

			// for UI, take action when PCS_GetGrpByNameRes is received
			virtual void OnGetGrpByNameRes2(const uint32_t &retCode,const uint32_t &uGroupCount,const uint32_t &uPageNo,const std::vector<protocol::search::CGSearchRecordData2> &vecRecords) = 0;

			// for UI, take action when PCS_GetGMemberSumRes is received(result of group search)
			virtual void OnGetGMemberSumDBDRes(const uint32_t& uPageNo, const std::map<uint32_t, uint32_t>& mapGidMemberSum) = 0;//{};// 实现后要改成纯虚函数
		};

		struct IOpSearchWatcher//底层需要实现的接口，供UI调用
		{
			virtual ~IOpSearchWatcher(){};
			virtual void watch(ISearchWatcher *w) = 0;
			virtual void revoke(ISearchWatcher *w) = 0;
		};

		struct ISearch : public virtual IOpSearchWatcher
		{
			virtual ~ISearch(){};
			
			//for UI, send PSS_GTopicSearchReq message
			virtual void DoGTopicSearch(std::map<uint32_t, std::vector<uint32_t> >  &mapGroupAndFolder, //key为GroupId, vector中存放该群下的子组FolderID，vector为空时不限子组
										uint32_t UID, 
										std::string keywords,
										uint32_t startTime,
										uint32_t stopTime,
										uint32_t location,
										uint32_t flag,
										uint32_t pageno = 1,
										uint32_t pagelen = 20) = 0;	

			//for UI, send PSS_GTopicMsgListReq message
			virtual void DoGTopicMsgList(uint32_t TID, 
										uint32_t FID, 
										uint32_t GID,
										uint64_t msgStartID,
										uint64_t msgStopID,
										uint32_t msgCount) = 0;

			//for UI, send PSS_GChatMsgListReq message
			virtual void DoGChatMsgList(uint32_t FID, 
										uint32_t GID,
										uint64_t msgStartID,
										uint64_t msgStopID,
										uint32_t msgCount) = 0;


			//for UI, send PSS_GTopicListReq message
			/*注意：
			 0.游标方式获取主题ID列表，从uStartTime消息往前取uTopicCount条,或者碰到uStopTime为止
			 1.应该保证uStartTime比uStopTime来得更新，即时间戳更晚
			 2.如果uStartTime为0，则从最新主题开始取
			 3.uTopicCount在uStopTime为0时才有意义，当uStopTime非0时，忽略uTopicCount
			 4.返回结果包含uStartTime时间点，但不包括uStopTime时间点
			 */
			 virtual void DoGChatMsgGet(uint32_t FID, 
										uint32_t GID,
										uint32_t pageno,
										uint32_t pagelen,
										uint32_t direction) = 0;

			virtual void DoGTopicList(uint32_t FID, 
										uint32_t GID,
										uint32_t startTime,
										uint32_t stopTime,
										uint32_t topicCount) = 0;
			
			//for UI, send PCS_GTopicMsgCountReq message
			/*注意：
			 0.游标方式获取消息，从uuidMsgStart消息往前取uMsgCount条,或者碰到uuidMsgStop为止
			 1.应该保证uuidMsgStart比uuidMsgStop来得更新，即时间戳更晚
			 2.如果uuidMsgStart为空，则从最新消息开始取
			 3.uMsgCount在uuidMsgStop为空时才有意义，当uuidMsgStop非空时，忽略uMsgCount
			 4.返回结果包含uuidMsgStart消息，但不包括uuidMsgStop消息
			 */
			virtual void DoGTopicMsgCount(uint32_t TID, 
										uint32_t FID, 
										uint32_t GID,
										uint64_t msgStartID,
										uint64_t msgStopID,
										uint32_t offset,
										uint32_t msgCount) = 0;

			//for UI, send PCS_GetGrpById message
			virtual void DoGetGrpById(uint32_t uGID) = 0;	

			//for UI, send PCS_GetGrpById message
			virtual void DoGetGrpByName(std::string strGName, uint32_t uClass, uint32_t uSubclass) = 0;

			//for UI, send PCS_GetGrpById2 message
			virtual void DoGetGrpById2(uint32_t uGID) = 0;	

			//for UI, send PCS_GetGrpById2 message
			virtual void DoGetGrpByName2(std::string strGName, uint32_t uClass, uint32_t uSubclass ,uint32_t uPageNo ,uint32_t uPageLen) = 0;

		};
	}
}
#endif

