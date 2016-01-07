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

//���ڱ���imsearch.cppʱ���ִ˴��������⣬������ע�͹���������ʹ�ã��Ժ��ٽ��
//������������
struct PSS_GTopicSearchReq: public sox::Marshallable {
	enum {
		uri = (20 << 8) | protocol::ACCESS_SVID
	};

	//keyΪGroupId, vector�д�Ÿ�Ⱥ�µ�����FolderID��vectorΪ��ʱ��������
	std::map<uint32_t, std::vector<uint32_t> > mapGroupAndFolder;
	uint32_t uSender; //�û�ID
	std::string strKeywords; //����ؼ����Կո�ָ�
	uint32_t uStartTime; //����ʱ������Ŀ�ʼ��
	uint32_t uStopTime; //����ʱ������Ľ�����
	uint32_t uLocation; //0:����; 1:����; 2:����+����; Ĭ����0
	uint32_t uFlag; //����0������1���ö�2

	uint32_t uPageNo; //�ڼ�ҳ,Ĭ��Ϊ1
	uint32_t uPageLen; //ÿҳ������Ĭ����20

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

//����������Ӧ
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

//������Ϣ��������
struct PCS_ChatMsgSearchReq: public sox::Marshallable {
	enum {
		uri = (21 << 8) | protocol::ACCESS_SVID
	};

	uint32_t uUserID; //�û�ID
	uint32_t uFriendID; //ֻ�����ض����ѵ������¼;Ϊ��ʱΪ���к���
	std::string strKeywords; //����ؼ����Կո�ָ�
	uint32_t uStartTime; //��ʼʱ��
	uint32_t uStopTime; //����ʱ��
	uint32_t uPageNo; //�ڼ�ҳ,Ĭ��Ϊ1
	uint32_t uPageLen; //ÿҳ������Ĭ����20
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


//������Ϣ������Ӧ
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

//������������ӷ�ҳ����
struct PSS_GTopicMsgListReq: public sox::Marshallable {
	enum {
		uri = (22 << 8) | protocol::ACCESS_SVID
	};

	uint32_t uTopicId; // ����ID
	uint32_t uFolderId; // ��id
	uint32_t uGroupId; // Ⱥid
	uint64_t timeMsgStart; //�����Ϣ���
	uint64_t timeMsgStop; //�յ���Ϣ���
	uint32_t uMsgCount; //��Ҫ����Ϣ������Ĭ����20

	/*ע�⣺
	 0.�α귽ʽ��ȡ��Ϣ����uuidMsgStart��Ϣ��ǰȡuMsgCount��,��������uuidMsgStopΪֹ
	 1.Ӧ�ñ�֤uuidMsgStart��uuidMsgStop���ø��£���ʱ�������
	 2.���uuidMsgStartΪ�գ����������Ϣ��ʼȡ
	 3.uMsgCount��uuidMsgStopΪ��ʱ�������壬��uuidMsgStop�ǿ�ʱ������uMsgCount
	 4.���ؽ������uuidMsgStart��Ϣ����������uuidMsgStop��Ϣ
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

	uint32_t uLeftTopicMsgCount; //����Ϣ֮ǰ��ʣ����Ϣ����
	uint64_t timeMsgNext; //��һ����Ϣ�ı�ţ����Ϊ0���ʾ��ǰû����Ϣ��
	uint32_t uTopicId; // ����ID
	uint32_t uFolderId; // ��id
	uint32_t uGroupId; // Ⱥid
	std::vector<STopicMsgInfo> vecTopicMsgList; //���շ���ʱ�䵹������

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

//�����б�����
struct PSS_GTopicListReq: public sox::Marshallable {
	enum {
		uri = (23 << 8) | protocol::ACCESS_SVID
	};

	uint32_t uFolderId; // ��id
	uint32_t uGroupId; // Ⱥid
	uint32_t uStartTime; //��ʼʱ��
	uint32_t uStopTime; //����ʱ��
	uint32_t uTopicCount; //ÿҳ������Ĭ����20

	/*ע�⣺
	 0.�α귽ʽ��ȡ����ID�б���uStartTime��Ϣ��ǰȡuTopicCount��,��������uStopTimeΪֹ
	 1.Ӧ�ñ�֤uStartTime��uStopTime���ø��£���ʱ�������
	 2.���uStartTimeΪ�գ�����������⿪ʼȡ
	 3.uTopicCount��uStopTimeΪ0ʱ�������壬��uStopTime��0ʱ������uTopicCount
	 4.���ؽ������uStartTimeʱ��㣬��������uStopTimeʱ���
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

	uint32_t uFolderId; // ��id
	uint32_t uGroupId; // Ⱥid
	std::vector<uint32_t> vecTopicIDList; //��������������ʱ�䵹������

	virtual void marshal(sox::Pack &p) const {
		p << uFolderId << uGroupId;
		marshal_container(p, vecTopicIDList);
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> uFolderId >> uGroupId;
		unmarshal_container(p, std::back_inserter(vecTopicIDList));		
	}
};

//uSTartTime֮ǰ���µ�������Ŀ
struct PSS_LastTopicCountReq: public sox::Marshallable {
	enum {
		uri = (24 << 8) | protocol::ACCESS_SVID
	};
	uint32_t uFolderId; // ��id
	uint32_t uGroupId; // Ⱥid
	uint32_t uStartTime; //STartTime֮ǰ����

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
	uint32_t uFolderId; // ��id
	uint32_t uGroupId; // Ⱥid
	virtual void marshal(sox::Pack &p) const {
		p << uTopicCount << uFolderId << uGroupId;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> uTopicCount >> uFolderId >> uGroupId;
	}
};

//uSTartTime֮ǰ���µ�������Ϣ��Ŀ
struct PSS_LastTopicMsgCountReq: public sox::Marshallable {
	enum {
		uri = (25 << 8) | protocol::ACCESS_SVID
	};
	uint32_t uTopicId; // ����ID
	uint32_t uFolderId; // ��id
	uint32_t uGroupId; // Ⱥid
	uint32_t uStartTime; //STartTime֮ǰ����

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
	uint32_t uTopicId; // ����ID
	uint32_t uFolderId; // ��id
	uint32_t uGroupId; // Ⱥid

	virtual void marshal(sox::Pack &p) const {
		p << uMsgCount << uTopicId << uFolderId << uGroupId;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> uMsgCount >> uTopicId >> uFolderId >> uGroupId;
	}
};

//������ϢCount����
struct PCS_ChatMsgCountReq: public sox::Marshallable {
	enum {
		uri = (26 << 8) | protocol::ACCESS_SVID
	};

	uint32_t uUserID; //�û�ID
	uint32_t uFriendID; //ֻ�����ض����ѵ������¼;Ϊ��ʱΪ���к���
	uint32_t uStartTime; //��ʼʱ��
	uint32_t uStopTime; //����ʱ��
	uint32_t uPageNo; //�ڼ�ҳ,Ĭ��Ϊ1
	uint32_t uPageLen; //ÿҳ������Ĭ����20
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
	uint32_t uUserID; //�û�ID
	uint32_t uFriendID; //ֻ�����ض����ѵ������¼;Ϊ��ʱΪ���к���
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

	uint32_t uTopicId; // ����ID
	uint32_t uFolderId; // ��id
	uint32_t uGroupId; // Ⱥid
	uint64_t uuidMsgStart; //�����Ϣ���
	uint64_t uuidMsgStop; //�յ���Ϣ���

	/* warning warning warning */
	uint32_t uOffset;
	/* warning warning warning */

	uint32_t uMsgCount; //��Ҫ����Ϣ������Ĭ����20

	/*ע�⣺
	 0.�α귽ʽ��ȡ��Ϣ����uuidMsgStart��Ϣ��ǰȡuMsgCount��,��������uuidMsgStopΪֹ
	 1.Ӧ�ñ�֤uuidMsgStart��uuidMsgStop���ø��£���ʱ�������
	 2.���uuidMsgStartΪ�գ����������Ϣ��ʼȡ
	 3.uMsgCount��uuidMsgStopΪ��ʱ�������壬��uuidMsgStop�ǿ�ʱ������uMsgCount
	 4.���ؽ������uuidMsgStart��Ϣ����������uuidMsgStop��Ϣ
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

	uint32_t uTopicId; // ����ID
	uint32_t uFolderId; // ��id
	uint32_t uGroupId; // Ⱥid
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

	uint32_t uFolderId; // ��id
	uint32_t uGroupId; // Ⱥid
	uint64_t timeMsgStart; //�����Ϣ���
	uint64_t timeMsgStop; //�յ���Ϣ���
	uint32_t uMsgCount; //��Ҫ����Ϣ������Ĭ����20

	/*ע�⣺
	 0.�α귽ʽ��ȡ��Ϣ����uuidMsgStart��Ϣ��ǰȡuMsgCount��,��������uuidMsgStopΪֹ
	 1.Ӧ�ñ�֤uuidMsgStart��uuidMsgStop���ø��£���ʱ�������
	 2.���uuidMsgStartΪ�գ����������Ϣ��ʼȡ
	 3.uMsgCount��uuidMsgStopΪ��ʱ�������壬��uuidMsgStop�ǿ�ʱ������uMsgCount
	 4.���ؽ������uuidMsgStart��Ϣ����������uuidMsgStop��Ϣ
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

	uint32_t uLeftTopicMsgCount; //����Ϣ֮ǰ��ʣ����Ϣ����
	uint64_t timeMsgNext; //��һ����Ϣ�ı�ţ����Ϊ0���ʾ��ǰû����Ϣ��
	uint32_t uFolderId; // ��id
	uint32_t uGroupId; // Ⱥid
	std::vector<STopicMsgInfo> vecTopicMsgList; //���շ���ʱ�䵹������

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
	uint32_t uPageNo; //�ڼ�ҳ,Ĭ��Ϊ1
	uint32_t uPageLen; //ÿҳ������Ĭ����20

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
	uint32_t uGroupCount; //���������Ŀ
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

	uint32_t uFolderId; // ��id
	uint32_t uGroupId; // Ⱥid
	/*
	uint64_t timeMsgStart; //�����Ϣ���
	uint64_t timeMsgStop; //�յ���Ϣ���
	uint32_t uStartTime; //��ʼʱ��
	uint32_t uStopTime; //����ʱ��
	*/
	uint32_t uPageNo; //�ڼ�ҳ,Ĭ��Ϊ1
	uint32_t uPageLen; //ÿҳ������Ĭ����20
	uint32_t uDirection;

	/*ע�⣺
	 0.��ҳ��ʽ��ȡ��Ϣ
	 1.uDirectionΪ0�����һҳΪʱ�����ϵ�;uDirectionΪ1�����һҳΪʱ�����µ�
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

	uint32_t uMsgCount; //��Ϣ���� ==0
	uint32_t uPageNo;
	uint32_t uFolderId; // ��id
	uint32_t uGroupId; // Ⱥid
	std::vector<STopicMsgInfo> vecTopicMsgList; //���շ���ʱ�䵹������

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

//������Ϣ�������󣨴�ȺID�汾��
struct PCS_ChatMsgSearchGrpReq: public sox::Marshallable {
	enum {
		uri = (34 << 8) | protocol::ACCESS_SVID
	};

	uint32_t uUserID; //�û�ID
	uint32_t uFriendID; //ֻ�����ض����ѵ������¼;Ϊ��ʱΪ���к���
	std::string strKeywords; //����ؼ����Կո�ָ�
	uint32_t uStartTime; //��ʼʱ��
	uint32_t uStopTime; //����ʱ��
	uint32_t uPageNo; //�ڼ�ҳ,Ĭ��Ϊ1
	uint32_t uPageLen; //ÿҳ������Ĭ����20
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


//������Ϣ������Ӧ����ȺID�汾��
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

