#ifndef ISESSIONMANAGER_H_
#define ISESSIONMANAGER_H_
#include "common/protocol/iaction.h"
#include "common/unistr.h"
#include "common/protocol/psessionbase.h"
#include "common/protocol/isession.h"

#include <vector>

namespace protocol{
namespace session{

struct SearchResult{
	UniString id;
	UniString name;
	UniString owner;
	bool bLimit;
	bool bPub;
};
struct IFindHandler{
	virtual void onResult(int resCode, const std::vector<SearchResult> &res) = 0;
};


struct ISession;
struct TextChat;
struct VoiceChat;

struct ISessionHandler{
	enum Status{
		JOIN1,       //�������ӣ���ȡlist��������ַ
		JOIN2,		 //ͨ��join�ĵ�ַ��ȥͬ���б� �����
		CONNECTED,   //���ӳɹ�
		NETBROKEN,   //�������
		CLOSING,     //���ڹر�
		INVALID,   //session �򿪴���,
		KICKOFF,
		JOIN_KICKOFF,
		JOIN_KICKOFF_BAN_ID,
		JOIN_KICKOFF_BAN_IP,
		RECONNECTING,
		CHANNEL_MOVED,
		SESSION_FROZEN, //Ƶ��������
		SESSION_BUSY, //������æ������onClose(SESSION_BUSY, s, delay)������delay�����PDenyAccessSession˵��  //added by xmp
		CHANNEL_FULL,  //Ƶ��������
		SESSION_FROZEN_WITH_TIME
	};

	enum NetErrCode{
		BIG_RECONNECT_ERR,            // �����������У�δ�ܸ�Session�ٴν�������
		ACCESS_FAILED,                // �û�δ˳������Ƶ�����͵����������
		NO_PROXY_INFO,                // session����ǰ��ip��ַΪ��
		RECV_BUFFER_OVERFLOW,         // ���ջ��������
	};

	virtual void onAssign(ISession *) = 0;
	virtual void onConnected(ISession *) = 0;
	virtual void onValid(ISession *) = 0;
	virtual void onStatus(Status st, ISession *) = 0;
	virtual void onClose(int reason, ISession *, int err) = 0;
	virtual void onSessionIdRecollected(ISession *sess, uint32_t oldSid, uint32_t realSid, const UniString &name) = 0;
	virtual void onNotifyNetErrCode(NetErrCode errCode, ISession *) = 0;
};

struct SessionCreateInfo;
struct ICreateHandler{
	virtual void onCreate(const SessionCreateInfo &, uint16_t res) = 0;
};

struct ISessionManager: public virtual IOperatorWatchBase{
	enum Operate{
		CREATE,
		JOIN,
		DISMISS,
	};

	virtual void create(const SessionCreateInfo &, ICreateHandler *) = 0;
	virtual void revokeCreateHandler(ICreateHandler *) = 0;
	
	//��Ϊ֧��ֱ�ӽ���Ƶ��
	virtual ISession *join(const UniString &sid, ISessionHandler* h, const UniString &subSid = _T("")) = 0;
    //xxx ??
	virtual void dismiss(const UniString &sid) = 0;
	//virtual void doAction(IAction *) = 0;
	virtual void findByChannelId(const UniString &id, IFindHandler *) = 0;

	virtual void findByChannelName(const UniString &name, IFindHandler*) = 0;
	
	virtual void findByLocate(int area, int province, int city, IFindHandler *) = 0;
	
	virtual void findByType(GuildType type, const UniString &typeName, IFindHandler *) = 0;

	virtual void revokeFindHandler(IFindHandler *) = 0;

	virtual ISession *forceJoin(const UniString &sid, ISessionHandler *, const std::string &sip, int sport) = 0;

	//added by xmp
	//[flag]  -1: newest(max) & def,  2: PJoinSession2_40, 3: PJoinSession3_00
	virtual ISession *join_ex(const UniString &sid, ISessionHandler* h, const UniString &subSid, int flag) = 0;
};
}
}
#endif /*ISESSIONMANAGER_H_*/
