#ifndef __PSEARCH_H__
#define __PSEARCH_H__

#include <vector>
#include <string>

#include "common/packet.h" // for "sox::Marshallable"
#include "common/iproperty.h"
#include "common/protocol/const.h"
#include "common/core/base_svid.h"
#include "common/protocol/ISearchRecord.h"
#include "common/protocol/pgtopicbase.h"

using namespace protocol::gtopic;

namespace protocol {
namespace search {

//由于编译imsearch.cpp时发现此处存在问题，所以先注释供后续工作使用，稍后再解决
//主题搜索请求
struct PSS_GTopicSearchReq: public sox::Marshallable {
	enum {
		uri = (20 << 8) | protocol::ACCESS_SVID
	};

	//key为GroupId, vector中存放该群下的子组FolderID，vector为空时不限子组
	std::map<uint32_t, std::vector<uint32_t> > mapGroupAndFolder;
	uint32_t uSender; //用户ID
	std::string strKeywords; //多个关键词以空格分隔
	uint32_t uStartTime; //发表时间区间的开始点
	uint32_t uStopTime; //发表时间区间的结束点
	uint32_t uLocation; //0:标题; 1:正文; 2:标题+正文; 默认是0
	uint32_t uFlag; //所有0，精华1，置顶2

	uint32_t uPageNo; //第几页,默认为1
	uint32_t uPageLen; //每页数量，默认是20

	virtual void marshal(sox::Pack &p) const {
		marshal_container(p, mapGroupAndFolder);
		p << uSender << strKeywords << uStartTime << uStopTime << uLocation
				<< uFlag << uPageNo << uPageLen;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		unmarshal_container(p, std::inserter(mapGroupAndFolder,
				mapGroupAndFolder.begin()));
		p >> uSender >> strKeywords >> uStartTime >> uStopTime >> uLocation
				>> uFlag >> uPageNo >> uPageLen;
	}

};

//主题搜索响应
struct PSS_GTopicSearchRes: public sox::Marshallable {
	enum {
		uri = (20 << 8) | protocol::ACCESS_SVID
	};
	uint32_t uTotalTopicCount;
	std::vector<GIDFIDTID> vTopicIDList;

	virtual void marshal(sox::Pack &p) const {
		p << uTotalTopicCount;
		marshal_container(p, vTopicIDList);

	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> uTotalTopicCount;
		//unmarshal_containerEx(p, vTopicIDList);
		unmarshal_container(p, std::back_inserter(vTopicIDList));		
	}
};

//聊天消息搜索请求
struct PCS_ChatMsgSearchReq: public sox::Marshallable {
	enum {
		uri = (21 << 8) | protocol::ACCESS_SVID
	};

	uint32_t uUserID; //用户ID
	uint32_t uFriendID; //只搜索特定好友的聊天记录;为空时为所有好友
	std::string strKeywords; //多个关键词以空格分隔
	uint32_t uStartTime; //开始时间
	uint32_t uStopTime; //结束时间
	uint32_t uPageNo; //第几页,默认为1
	uint32_t uPageLen; //每页数量，默认是20
	uint32_t uTaskId; //by Client

	virtual void marshal(sox::Pack &p) const {

		p << uUserID << uFriendID << strKeywords << uStartTime << uStopTime
				<< uPageNo << uPageLen << uTaskId;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> uUserID >> uFriendID >> strKeywords >> uStartTime >> uStopTime
				>> uPageNo >> uPageLen >> uTaskId;
	}

};


//聊天消息搜索响应
struct PCS_ChatMsgSearchRes: public sox::Marshallable {
	enum {
		uri = (21 << 8) | protocol::ACCESS_SVID
	};

	uint32_t uTaskId; //by Client
	uint32_t uTotalmsgCount;
	std::vector<SChatMsgInfo> vChatMsgList;

	virtual void marshal(sox::Pack &p) const {
		p << uTaskId << uTotalmsgCount;
		marshal_container(p, vChatMsgList);
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> uTaskId >> uTotalmsgCount;
		unmarshal_containerEx(p, vChatMsgList);
	}

};

//单个主题的帖子分页请求
struct PSS_GTopicMsgListReq: public sox::Marshallable {
	enum {
		uri = (22 << 8) | protocol::ACCESS_SVID
	};

	uint32_t uTopicId; // 主题ID
	uint32_t uFolderId; // 组id
	uint32_t uGroupId; // 群id
	uint64_t timeMsgStart; //起点消息编号
	uint64_t timeMsgStop; //终点消息编号
	uint32_t uMsgCount; //需要的消息条数，默认是20

	/*注意：
	 0.游标方式获取消息，从uuidMsgStart消息往前取uMsgCount条,或者碰到uuidMsgStop为止
	 1.应该保证uuidMsgStart比uuidMsgStop来得更新，即时间戳更晚
	 2.如果uuidMsgStart为空，则从最新消息开始取
	 3.uMsgCount在uuidMsgStop为空时才有意义，当uuidMsgStop非空时，忽略uMsgCount
	 4.返回结果包含uuidMsgStart消息，但不包括uuidMsgStop消息
	 */

	virtual void marshal(sox::Pack &p) const {
		p << uTopicId << uFolderId << uGroupId << timeMsgStart << timeMsgStop
				<< uMsgCount;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> uTopicId >> uFolderId >> uGroupId >> timeMsgStart >> timeMsgStop
				>> uMsgCount;
	}
};

struct PSS_GTopicMsgListRes: public sox::Marshallable {
	enum {
		uri = (22 << 8) | protocol::ACCESS_SVID
	};

	uint32_t uLeftTopicMsgCount; //该消息之前的剩余消息总量
	uint64_t timeMsgNext; //下一条消息的编号；如果为0则表示往前没有消息了
	uint32_t uTopicId; // 主题ID
	uint32_t uFolderId; // 组id
	uint32_t uGroupId; // 群id
	std::vector<STopicMsgInfo> vecTopicMsgList; //按照发表时间倒序排列

	virtual void marshal(sox::Pack &p) const {
		p << uLeftTopicMsgCount << timeMsgNext << uTopicId << uFolderId
				<< uGroupId;
		marshal_container(p, vecTopicMsgList);
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> uLeftTopicMsgCount >> timeMsgNext >> uTopicId >> uFolderId
				>> uGroupId;
		unmarshal_containerEx(p, vecTopicMsgList);
	}
};

//主题列表请求
struct PSS_GTopicListReq: public sox::Marshallable {
	enum {
		uri = (23 << 8) | protocol::ACCESS_SVID
	};

	uint32_t uFolderId; // 组id
	uint32_t uGroupId; // 群id
	uint32_t uStartTime; //开始时间
	uint32_t uStopTime; //结束时间
	uint32_t uTopicCount; //每页数量，默认是20

	/*注意：
	 0.游标方式获取主题ID列表，从uStartTime消息往前取uTopicCount条,或者碰到uStopTime为止
	 1.应该保证uStartTime比uStopTime来得更新，即时间戳更晚
	 2.如果uStartTime为空，则从最新主题开始取
	 3.uTopicCount在uStopTime为0时才有意义，当uStopTime非0时，忽略uTopicCount
	 4.返回结果包含uStartTime时间点，但不包括uStopTime时间点
	 */
	virtual void marshal(sox::Pack &p) const {
		p << uFolderId << uGroupId << uStartTime << uStopTime << uTopicCount;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> uFolderId >> uGroupId >> uStartTime >> uStopTime >> uTopicCount;
	}

};

struct PSS_GTopicListRes: public sox::Marshallable {
	enum {
		uri = (23 << 8) | protocol::ACCESS_SVID
	};

	uint32_t uFolderId; // 组id
	uint32_t uGroupId; // 群id
	std::vector<uint32_t> vecTopicIDList; //按照主题最后更新时间倒序排列

	virtual void marshal(sox::Pack &p) const {
		p << uFolderId << uGroupId;
		marshal_container(p, vecTopicIDList);
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> uFolderId >> uGroupId;
		unmarshal_container(p, std::back_inserter(vecTopicIDList));		
	}
};

//uSTartTime之前更新的主题数目
struct PSS_LastTopicCountReq: public sox::Marshallable {
	enum {
		uri = (24 << 8) | protocol::ACCESS_SVID
	};
	uint32_t uFolderId; // 组id
	uint32_t uGroupId; // 群id
	uint32_t uStartTime; //STartTime之前更新

	virtual void marshal(sox::Pack &p) const {
		p << uFolderId << uGroupId << uStartTime;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> uFolderId >> uGroupId >> uStartTime;
	}
};

struct PSS_LastTopicCountRes: public sox::Marshallable {
	enum {
		uri = (24 << 8) | protocol::ACCESS_SVID
	};

	uint32_t uTopicCount;
	uint32_t uFolderId; // 组id
	uint32_t uGroupId; // 群id
	virtual void marshal(sox::Pack &p) const {
		p << uTopicCount << uFolderId << uGroupId;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> uTopicCount >> uFolderId >> uGroupId;
	}
};

//uSTartTime之前更新的主题消息数目
struct PSS_LastTopicMsgCountReq: public sox::Marshallable {
	enum {
		uri = (25 << 8) | protocol::ACCESS_SVID
	};
	uint32_t uTopicId; // 主题ID
	uint32_t uFolderId; // 组id
	uint32_t uGroupId; // 群id
	uint32_t uStartTime; //STartTime之前更新

	virtual void marshal(sox::Pack &p) const {
		p << uTopicId << uFolderId << uGroupId << uStartTime;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> uTopicId >> uFolderId >> uGroupId >> uStartTime;
	}
};

struct PSS_LastTopicMsgCountRes: public sox::Marshallable {
	enum {
		uri = (25 << 8) | protocol::ACCESS_SVID
	};

	uint32_t uMsgCount;
	uint32_t uTopicId; // 主题ID
	uint32_t uFolderId; // 组id
	uint32_t uGroupId; // 群id

	virtual void marshal(sox::Pack &p) const {
		p << uMsgCount << uTopicId << uFolderId << uGroupId;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> uMsgCount >> uTopicId >> uFolderId >> uGroupId;
	}
};

//聊天消息Count请求
struct PCS_ChatMsgCountReq: public sox::Marshallable {
	enum {
		uri = (26 << 8) | protocol::ACCESS_SVID
	};

	uint32_t uUserID; //用户ID
	uint32_t uFriendID; //只搜索特定好友的聊天记录;为空时为所有好友
	uint32_t uStartTime; //开始时间
	uint32_t uStopTime; //结束时间
	uint32_t uPageNo; //第几页,默认为1
	uint32_t uPageLen; //每页数量，默认是20
	uint32_t uTaskId; //by client
	virtual void marshal(sox::Pack &p) const {

		p << uUserID << uFriendID << uStartTime << uStopTime << uPageNo
				<< uPageLen << uTaskId;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> uUserID >> uFriendID >> uStartTime >> uStopTime >> uPageNo
				>> uPageLen >> uTaskId;
	}

};

struct PCS_ChatMsgCountRes: public sox::Marshallable {
	enum {
		uri = (26 << 8) | protocol::ACCESS_SVID
	};
	uint32_t uUserID; //用户ID
	uint32_t uFriendID; //只搜索特定好友的聊天记录;为空时为所有好友
	uint32_t uTotalmsgCount;
	uint32_t umsgCount;
	std::string strStartMsgID;
	std::string strStopMsgID;
	uint32_t uTaskId; //by client
	virtual void marshal(sox::Pack &p) const {
		p << uUserID << uFriendID << uTotalmsgCount << umsgCount
				<< strStartMsgID << strStopMsgID << uTaskId;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> uUserID >> uFriendID >> uTotalmsgCount >> umsgCount
				>> strStartMsgID >> strStopMsgID >> uTaskId;
	}

};

struct PCS_GTopicMsgCountReq: public sox::Marshallable {
	enum {
		uri = (27 << 8) | protocol::ACCESS_SVID
	};

	uint32_t uTopicId; // 主题ID
	uint32_t uFolderId; // 组id
	uint32_t uGroupId; // 群id
	uint64_t uuidMsgStart; //起点消息编号
	uint64_t uuidMsgStop; //终点消息编号

	/* warning warning warning */
	uint32_t uOffset;
	/* warning warning warning */

	uint32_t uMsgCount; //需要的消息条数，默认是20

	/*注意：
	 0.游标方式获取消息，从uuidMsgStart消息往前取uMsgCount条,或者碰到uuidMsgStop为止
	 1.应该保证uuidMsgStart比uuidMsgStop来得更新，即时间戳更晚
	 2.如果uuidMsgStart为空，则从最新消息开始取
	 3.uMsgCount在uuidMsgStop为空时才有意义，当uuidMsgStop非空时，忽略uMsgCount
	 4.返回结果包含uuidMsgStart消息，但不包括uuidMsgStop消息
	 */

	virtual void marshal(sox::Pack &p) const {
		p << uTopicId << uFolderId << uGroupId << uuidMsgStart << uuidMsgStop
				<< uOffset << uMsgCount;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> uTopicId >> uFolderId >> uGroupId >> uuidMsgStart >> uuidMsgStop
				>> uOffset >> uMsgCount;
	}
};

struct PCS_GTopicMsgCountRes: public sox::Marshallable {
	enum {
		uri = (27 << 8) | protocol::ACCESS_SVID
	};

	uint32_t uTopicId; // 主题ID
	uint32_t uFolderId; // 组id
	uint32_t uGroupId; // 群id
	uint64_t uStartMsgID;
	uint64_t uStopMsgID;

	virtual void marshal(sox::Pack &p) const {
		p << uTopicId << uFolderId << uGroupId << uStartMsgID << uStopMsgID;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> uTopicId >> uFolderId >> uGroupId >> uStartMsgID >> uStopMsgID;
	}
};

// packet to get group by group number.
struct PCS_GetGrpById: public sox::Marshallable{
	enum { uri = (28 << 8) | protocol::ACCESS_SVID};

	uint32_t m_uGID;		//group number for users to remember
	virtual void marshal(sox::Pack &p) const {
		p << m_uGID;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uGID;
	}
};

// packet to response for getting group by group number.
struct PCS_GetGrpByIdRes: public sox::Marshallable{
	enum { uri = (28 << 8) | protocol::ACCESS_SVID};

	uint32_t m_uResCode;
	protocol::search::CGSearchRecordData m_objGrpRec;
	virtual void marshal(sox::Pack &p) const {
		p << m_uResCode << m_objGrpRec;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uResCode >> m_objGrpRec;
	}
};

// packet to get group by group name.
struct PCS_GetGrpByName: public sox::Marshallable{
	enum { uri = (29 << 8) | protocol::ACCESS_SVID};

	std::string m_strGName;
	uint32_t uClass;
	uint32_t uSubclass;
	
	virtual void marshal(sox::Pack &p) const {
		p << m_strGName << uClass << uSubclass;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_strGName >> uClass >> uSubclass;
	}
};

// packet to response for getting group by group name.
struct PCS_GetGrpByNameRes: public sox::Marshallable{
	enum { uri = (29 << 8) | protocol::ACCESS_SVID};

	uint32_t m_uResCode;
	std::vector<protocol::search::CGSearchRecordData> m_vecGrpRecs;
	virtual void marshal(sox::Pack &p) const {
		p << m_uResCode;
		marshal_container(p, m_vecGrpRecs);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uResCode;
		unmarshal_containerEx(p, m_vecGrpRecs);
	}
};

struct PSS_GChatMsgListReq: public sox::Marshallable {
	enum {
		uri = (30 << 8) | protocol::ACCESS_SVID
	};

	uint32_t uFolderId; // 组id
	uint32_t uGroupId; // 群id
	uint64_t timeMsgStart; //起点消息编号
	uint64_t timeMsgStop; //终点消息编号
	uint32_t uMsgCount; //需要的消息条数，默认是20

	/*注意：
	 0.游标方式获取消息，从uuidMsgStart消息往前取uMsgCount条,或者碰到uuidMsgStop为止
	 1.应该保证uuidMsgStart比uuidMsgStop来得更新，即时间戳更晚
	 2.如果uuidMsgStart为空，则从最新消息开始取
	 3.uMsgCount在uuidMsgStop为空时才有意义，当uuidMsgStop非空时，忽略uMsgCount
	 4.返回结果包含uuidMsgStart消息，但不包括uuidMsgStop消息
	 */

	virtual void marshal(sox::Pack &p) const {
		p << uFolderId << uGroupId << timeMsgStart << timeMsgStop
				<< uMsgCount;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> uFolderId >> uGroupId >> timeMsgStart >> timeMsgStop
				>> uMsgCount;
	}
};

struct PSS_GChatMsgListRes: public sox::Marshallable {
	enum {
		uri = (30 << 8) | protocol::ACCESS_SVID
	};

	uint32_t uLeftTopicMsgCount; //该消息之前的剩余消息总量
	uint64_t timeMsgNext; //下一条消息的编号；如果为0则表示往前没有消息了
	uint32_t uFolderId; // 组id
	uint32_t uGroupId; // 群id
	std::vector<STopicMsgInfo> vecTopicMsgList; //按照发表时间倒序排列

	virtual void marshal(sox::Pack &p) const {
		p << uLeftTopicMsgCount << timeMsgNext << uFolderId
				<< uGroupId;
		marshal_container(p, vecTopicMsgList);
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> uLeftTopicMsgCount >> timeMsgNext >> uFolderId
				>> uGroupId;
		unmarshal_containerEx(p, vecTopicMsgList);
	}
};


struct PCS_GetGrpById2: public sox::Marshallable{
	enum { uri = (31 << 8) | protocol::ACCESS_SVID};

	uint32_t m_uGID;		//group number for users to remember
	virtual void marshal(sox::Pack &p) const {
		p << m_uGID;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uGID;
	}
};

// packet to response for getting group by group number.
struct PCS_GetGrpByIdRes2: public sox::Marshallable{
	enum { uri = (31 << 8) | protocol::ACCESS_SVID};

	uint32_t m_uResCode;
	protocol::search::CGSearchRecordData2 m_objGrpRec;
	virtual void marshal(sox::Pack &p) const {
		p << m_uResCode << m_objGrpRec;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uResCode >> m_objGrpRec;
	}
};

// packet to get group by group name.
struct PCS_GetGrpByName2: public sox::Marshallable{
	enum { uri = (32 << 8) | protocol::ACCESS_SVID};

	std::string m_strGName;
	uint32_t uClass;
	uint32_t uSubclass;
	uint32_t uPageNo; //第几页,默认为1
	uint32_t uPageLen; //每页数量，默认是20

	virtual void marshal(sox::Pack &p) const {
		p << m_strGName << uClass << uSubclass << uPageNo << uPageLen;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_strGName >> uClass >> uSubclass >> uPageNo >> uPageLen;
	}
};

// packet to response for getting group by group name.
struct PCS_GetGrpByNameRes2: public sox::Marshallable{
	enum { uri = (32 << 8) | protocol::ACCESS_SVID};

	uint32_t m_uResCode;
	uint32_t uGroupCount; //搜索结果数目
	uint32_t uPageNo;

	std::vector<protocol::search::CGSearchRecordData2> m_vecGrpRecs;
	virtual void marshal(sox::Pack &p) const {
		p << m_uResCode << uGroupCount << uPageNo;
		marshal_container(p, m_vecGrpRecs);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> m_uResCode >> uGroupCount >> uPageNo;
		unmarshal_containerEx(p, m_vecGrpRecs);
	}
};

//Get GChatMessage by Page
struct PCS_GChatMsgGetReq: public sox::Marshallable {
	enum {
		uri = (33 << 8) | protocol::ACCESS_SVID
	};

	uint32_t uFolderId; // 组id
	uint32_t uGroupId; // 群id
	/*
	uint64_t timeMsgStart; //起点消息编号
	uint64_t timeMsgStop; //终点消息编号
	uint32_t uStartTime; //开始时间
	uint32_t uStopTime; //结束时间
	*/
	uint32_t uPageNo; //第几页,默认为1
	uint32_t uPageLen; //每页数量，默认是20
	uint32_t uDirection;

	/*注意：
	 0.翻页方式获取消息
	 1.uDirection为0，则第一页为时间最老的;uDirection为1，则第一页为时间最新的
	 */

	virtual void marshal(sox::Pack &p) const {
		p << uFolderId << uGroupId << uPageNo << uPageLen
				<< uDirection;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> uFolderId >> uGroupId >> uPageNo >> uPageLen
				>> uDirection;
	}
};

struct PCS_GChatMsgGetRes: public sox::Marshallable {
	enum {
		uri = (33 << 8) | protocol::ACCESS_SVID
	};

	uint32_t uMsgCount; //消息总量 ==0
	uint32_t uPageNo;
	uint32_t uFolderId; // 组id
	uint32_t uGroupId; // 群id
	std::vector<STopicMsgInfo> vecTopicMsgList; //按照发表时间倒序排列

	virtual void marshal(sox::Pack &p) const {
		p << uMsgCount << uPageNo << uFolderId
				<< uGroupId;
		marshal_container(p, vecTopicMsgList);
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> uMsgCount >> uPageNo >> uFolderId
				>> uGroupId;
		unmarshal_containerEx(p, vecTopicMsgList);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////

//聊天消息搜索请求（带群ID版本）
struct PCS_ChatMsgSearchGrpReq: public sox::Marshallable {
	enum {
		uri = (34 << 8) | protocol::ACCESS_SVID
	};

	uint32_t uUserID; //用户ID
	uint32_t uFriendID; //只搜索特定好友的聊天记录;为空时为所有好友
	std::string strKeywords; //多个关键词以空格分隔
	uint32_t uStartTime; //开始时间
	uint32_t uStopTime; //结束时间
	uint32_t uPageNo; //第几页,默认为1
	uint32_t uPageLen; //每页数量，默认是20
	uint32_t uTaskId; //by Client

	virtual void marshal(sox::Pack &p) const {

		p << uUserID << uFriendID << strKeywords << uStartTime << uStopTime
			<< uPageNo << uPageLen << uTaskId;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> uUserID >> uFriendID >> strKeywords >> uStartTime >> uStopTime
			>> uPageNo >> uPageLen >> uTaskId;
	}

};


//聊天消息搜索响应（带群ID版本）
struct PCS_ChatMsgSearchGrpRes: public sox::Marshallable {
	enum {
		uri = (34 << 8) | protocol::ACCESS_SVID
	};

	uint32_t uTaskId; //by Client
	uint32_t uTotalmsgCount;
	std::vector<SChatMsgInfoGrp> vChatMsgList;

	virtual void marshal(sox::Pack &p) const {
		p << uTaskId << uTotalmsgCount;
		marshal_container(p, vChatMsgList);
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> uTaskId >> uTotalmsgCount;
		unmarshal_containerEx(p, vChatMsgList);
	}

};

}
}
#endif

