#pragma  once
#include "ilist.h"
#include "iaction.h"
#include "common/misc.h"
#include "const.h"
#include "psessionbase.h"
#include "authcode.h"
#include "isessioninfo.h"
#include "ISessionMemberList.h"
#include "KickOffType.h"
#include "common/protocol/pluginInfo.h"

namespace protocol{
	namespace session{
	
		struct SessionCreateInfo{
		public:
			SessionCreateInfo(const UniString& n, bool lim, bool pub, 
				const UniString& h, const std::string& pa, const UniString& pi,
				int tp, const UniString &tStr, VoiceQC vq = quality, ISPType sp = CTL)
			{
				name	  = n;
				islimit   = lim;
				isPub     = pub;
				hello	  = h;
				password = pa;
				pid		 = pi;
				type	= tp;
				typeStr = tStr;
				qc		= vq;
				isp		= sp;
			}

			SessionCreateInfo(){}
		public:
			UniString name;
			//int ctype;
			//int gtype;
			bool islimit;
			bool isPub;
			UniString hello;
			std::string password;
			UniString pid;
			int type;
			UniString typeStr;
			UniString sid; //��������д
			VoiceQC qc;
			ISPType isp;
		};


		struct ISessionManagerWatcher{
			virtual void onRefresh() = 0;
			/*
				err define:
				�û������� 404
				��û��Ȩ�޽��иò��������Ǹ�Ƶ������Ա,����Ȩ�޶Կ�ʧ�ܣ� 403
			*/
			virtual void onError(int err) = 0;
		};

		struct ISessionInfo: public virtual ISessionInfoBase{
			virtual void setName(const UniString &n) = 0;
			virtual void changeMemberRight(const UniString &uid, ChannelRoler nr) = 0;
			virtual void setPub(bool p) = 0;
			virtual void setLimit(bool l) = 0;
			virtual void setPass(const std::string &md5Pass) = 0;
			virtual void setBulletin(const UniString &hello) = 0;
			virtual void setMicTime(int t) = 0;
	

			virtual void setType(GuildType ) = 0;
			virtual void setTypeString(const UniString &) = 0;

			virtual void setMemberLimited(uint32_t ml) = 0;

			virtual void setArea(int n) = 0;
			virtual void setProvince(int n) = 0;
			virtual void setCity(int n) = 0;
			
			virtual void update() = 0;

			virtual void watchManager(ISessionManagerWatcher *) = 0;

			virtual void revokeManager(ISessionManagerWatcher *) = 0;

			virtual void setIntroduce(const UniString&) = 0;

			virtual void setVoiceQC(VoiceQC qc) = 0;

			virtual void setTxtLimit(bool b) = 0;

			virtual void setTxtLimitTime(int secs) = 0;

			/**
			 * ���ù���logo
			 *
			 * @param	buildInLogoIndex	>=1ʱ��ʾϵͳ����logo
											=0ʱ��ʾ�û��Զ���logo
			 *
			 * @param	logUrl				�Զ���logo��url��
											��nBuildInLogoIndex>0��������ʱlogUrl�ÿմ��Ϳ����ˡ�
			 */
			virtual void setGuildLogoIndex(int nBuildInLogoIndex, const UniString& logUrl) = 0;

			virtual void setGuestMaxTxtLen(int ) = 0;

			virtual void setGuestJoinMic(bool ) = 0;

			virtual void setGuestWaitTime(int) = 0;

			virtual void setEnableGuestLimit(bool isEnable) = 0;

			virtual void setLimitTxtOnlyForGuest(bool bLimit) = 0;

			virtual void setLimitGuestAccess(bool isLimit) = 0;

			virtual void setGuestAccessTopLimit(bool isLimit) = 0;

			/*	
					 ��������ģʽ���Ƿ��ֹ�ο�����
					 true��ʾ��ֹ������false��ʾ��������
			*/
			virtual void setGuestVoiceChat(bool bLimit) = 0;

			virtual void setStyle(ChannelStyle style) = 0;

			virtual void setSendUrlTxt(bool bLimit) = 0;
		};
		
		struct TextChat;
		struct VoiceChat;
		struct PChatVoice;

		struct IChatWatch {
			virtual void onText(const UniString &from, const UniString &to, const TextChat &) = 0;
			virtual void onVoice(const UniString &from, const UniString &to, uint32_t seq, uint32_t timeStamp) = 0;
			virtual void onStopVoice(const UniString &who) = 0;
			virtual void onPacketLoss(uint32_t user) = 0;
			virtual void onPlayLoss(const uint32_t user, const uint32_t seq, const bool queueEmpty) = 0;
			virtual void onSendAudioPacket(uint32_t user) = 0;
			virtual void onResendAudioPacket(uint32_t user) = 0;
		};

		struct PChatVoice;
		struct PChatQualityVoiceRes;

		struct AudioPacket
		{
			uint32_t from;
			uint32_t seq;
			uint32_t timeStamp;
			uint32_t sid;
			std::string data;
		};

		struct IPacketNotifyer
		{
			virtual void on_packet_loss(uint32_t user, int seq) = 0;
		};

		struct IVoicePullNotifyer {	
			virtual void on_pull_data(const uint32_t from, const uint32_t sid, const uint32_t seq, const uint32_t timeStamp) = 0;
			virtual void on_play_loss(const uint32_t userID, const uint32_t seq, 
				const bool queueEmpty) = 0;
			virtual void on_clear_delay() = 0;
		};

		enum ServiceMode
		{
			qualityFirstServiceMode,
			lowLatencyFirstServiceMode
		};

		struct IAudioDataMgr 
		{
			virtual void Add(uint32_t userID, uint32_t sid, uint32_t seq, const std::string& data, uint32_t timeStamp) = 0;
			virtual void SetIgnore(uint32_t userID, bool ignore) = 0;
			virtual void OnDataEnd(uint32_t userID) = 0;			
			virtual void SetPacketNotifyer(IPacketNotifyer* pPacketNotifyer) = 0;
			virtual void SetVoicePullNotifyer(IVoicePullNotifyer* pVoicePullNotifyer) = 0;
		};

		struct IUserGuild{

		};

		struct IMicrophoneListWatcher;
		struct IMicrophoneList: public IListBaseEx{

			virtual void watchMicrophoneList(protocol::session::IMicrophoneListWatcher*) = 0;
			virtual void revokeMicrophoneList(protocol::session::IMicrophoneListWatcher*) = 0;

			virtual void joinQueue(void) = 0;
			virtual void leaveQueue() = 0;
			virtual void mute(bool mute) = 0;
			virtual void kickOff(const UniString &uid) = 0;
			virtual void doubleTime(void) = 0;
			virtual void move(const UniString &uid, bool down) = 0;
			virtual void disableQueue(void) = 0;
			virtual void allowQueue(void) = 0;
			virtual int	 getTime(void) = 0;
			virtual bool getMute(void) = 0;
			virtual bool getDisable(void) = 0;
			virtual uinfo::SEX getSex(const UniString& uid) = 0;
			virtual ChannelRoler getRoler(const UniString &hid, const UniString &channelId) = 0;

			virtual void kickAllOff() = 0;
			virtual void move2top(const UniString &uid) = 0;
			virtual HULI getTopQueueUser() const = 0;
			
		};

		enum SessionOperate{
			CREATE_SUBCHANNEL,
			DISSMISS_SUBCHANNEL,
			CHANGE_FOLDER,
			UPDATE_SINFO,
			UPDATE_MEMBERROLER,
			CHANGE_STYLE,
			KICKOFF,
			JOIN_SYNC,
			SETKEYACTIVE
		};

		enum BroadcastReceiverRoler{
			BROADCAST_TO_ALL     = 0x01,
			BROADCAST_TO_MANAGER = 0x02,
			BROADCAST_TO_MEMBER  = 0x04,
			BROADCAST_TO_VIP     = 0x08,
			BROADCAST_TO_GUEST   = 0x10
		};


		struct SessOperation
		{	
				enum OpCode
				{
					NONE = 0,
					CREATE_SUBCHANNEL_CHK,		// ������Ƶ��
					DISSMISS_SUBCHANNEL_CHK,	// ��ɢ��Ƶ��
					CHANGE_INFO,				// ������Ƶ����Ϣ
					ADD_MANAGER,				// ��ӹ���Ա
					REMOVE_MANAGER,				// ɾ������Ա
					ADD_SUB_MANAGER,			// ɾ����Ƶ������Ա
					REMOVE_SUB_MANAGER,			// �����Ƶ������Ա
					ADD_MEMBER,					// ��ӳ�Ա
					REMOVE_MEMBER,				// ɾ����Ա
					ADD_VICE_OWNER,				// ��Ӹ��᳤
					REMOVE_VICE_OWNER,			// ɾ�����᳤
					JOIN_CHANNEL,				// �������Ƽ�Ƶ��
					JOIN_WITHOUTPASSWD,			// ����û������Ƶ��
					KICKOFF,					// ����
					VOICE,						// ����
					TEXT,						// ����
					CHANGESTYLE,				// ����ģʽ
					TUOREN,						// ����
					SETKEYACTIVE,				// ���̼���
					DISABLE_VOICE,				// ��ֹ����
					ENABLE_VOICE,				// ��������
					ENABLE_TEXT,				// ��ֹ�ı�
					DISABLE_TEXT,				// �����ı�
					BROADCAST,					// �㲥
					DRAGETO_QUEUE,				// �Ͻ�����
					JOIN_QUEUE,					// ����	
					LEAVE_QUEUE,				// �˳�����
					MOVEUP_QUEUE,				// ��������
					MOVEDOWN_QUEUE,				// ��������
					KICK_QUEUE,					// �߳�����
					MUTE_QUEUE,					// ��ֹ����XXXX Ӧ���ǿ���
					UNMUTE_QUEUE,				// ��������XXXX Ӧ���Ƿ���
					DOUBLETIME_QUEUE,			// ˫������ʱ��
					DISABLE_QUEUE,				// ��������
					ALLOW_QUEUE,				// ��������
					DRAG_PEOPLE,				// ����
					DROP_PEOPLE,				// ����

					EDIT_CHANNEL_INFO,			// �༭Ƶ����Ϣ��chenzhou 2008/4/8
					SHOW_AS_MANAGER,			// �Ƿ���ʾ�ڹ���Ա�б�
					DISABLE_ALL_TEXT,
					ENABLE_ALL_TEXT,
					SETALLKEYACTIVE,
					ADD_VIP,
					REMOVE_VIP,
					ADD_TMPVIP,
					REMOVE_TMPVIP,
					ADD_BAN_ID,
					REMOVE_BAN_ID,
					ADD_BAN_IP,
					REMOVE_BAN_IP,

					MOVETOP_QUEUE,
					CLEAR_QUEUE,
					IGNORE_TEXT,
					IGNORE_VOICE,
					QUIT_GUILD,					//�˳�����
					DRAG_ROOM,					//��Ƶ��
					DROP_ROOM,					//��Ƶ��
					DIRECT_KICKOFF,
					SHOW_GUILD_MANAGER			//��ʾ���������


				};
				SessOperation(){}
				SessOperation(OpCode opcode)
				{
						this->opcode = opcode;
				}
				SessOperation(OpCode opcode,const HULI& room){
					this->opcode = opcode;
					this->room = room;
				}
				SessOperation(OpCode opcode,const HULI& room, const HULI& user){
						this->opcode = opcode;
						this->room = room;
						this->user = user;
				}
				OpCode opcode;
				HULI   user;	//����������
				HULI   room;	//������Ƶ��
		};
		struct SessDropOperation : public  SessOperation{
			SessDropOperation(OpCode opcode,const HULI& target,const HULI& room) : SessOperation(opcode,target){
				this->room = room;
			}
			HULI	room;
		};


		struct IRulerChecker{
				virtual bool canDo(SessOperation* operation) = 0;
		};



		struct IMicrophoneListWatcher{
			virtual void onSync(void) = 0;
			virtual void onAllow(const UniString &manipulator) = 0;
			virtual void onDisable(const UniString &manipulator) = 0;
			virtual void onKickOffQueue(const UniString &manipulator,const UniString &uid) = 0;
			virtual void onDoubleTimeQueue(const UniString &manipulator,const UniString &uid, int sec) = 0;
			virtual void onTurn(const UniString& uid, int sec) = 0;
			virtual void onTimeout(const UniString& uid) = 0;
			virtual void onLeave(const UniString& uid) = 0;
			virtual void onDrag(const UniString &manipulator,const UniString& uid) = 0;
			virtual void onMute(const UniString &manipulator,bool mute) = 0;
			virtual void onClearQueue(const UniString &manipulator) =0;
			virtual void onMove(const UniString &manipulator,const UniString &uid, bool down) = 0;
			virtual void onMove2top(const UniString &manipulator,const UniString &uid) = 0;
		};


		// TODO: ����ӿ�̫�֣��ܶ�ʱ��ֻ�õ����к�Сһ���ֽӿ�
		struct ISessionWatcher {
			enum AnnounceType
			{
				EMERGENCY = 0,
				ANNOUNCEMENT,
				AD,
				NOTICE,
				WARNING,
				ERR,
				ANSWER_QUESTION
			};
			
			virtual void onPosChange(const UniString &from) = 0;
			virtual void onStyleChange(const UniString &uid) = 0;
			//1 admin 2 op 3 who 4 from_role to 5 to_role in 6 where
			virtual void onRolerChange() = 0;
			
			//�޸ĸýӿڣ��������ͣ�ָ���Ǳ��߳�Ƶ�����Ǳ���ID��IP��0��ʾ���߳���1��ʾ����ID, 2��ʾ����IP};
			virtual void onKickOff(const UniString &who, const UniString &toCh, const UniString &admin, const UniString &reason, uint32_t sec, KickOffType type) = 0;
			
			virtual void onRingBusy() = 0;
			//target is you
			virtual void onSetKeyActive(const UniString & manipulator) = 0;
			virtual void onStopSession(AnnounceType type, const UniString &msg) = 0;
			//xxx bool
			virtual void onDisableVoice(const UniString &manipulator, const UniString &uid, bool bDisable) = 0;
			//xxx bool
			virtual void onDisableText(const UniString &manipulator, const UniString& uid, bool bDisable) = 0;

			virtual void onTuoren(const UniString &manipulator,const UniString &from,const UniString &to) = 0;
			virtual void onSetRoomKeyActive(const UniString &manipulator) = 0;
			virtual void onDisableAllText(const UniString &manipulator, const UniString &channelId, bool bDisable) = 0;
			virtual void onTxtLimit(const UniString &manipulator, const UniString &channelId, bool bLimit) = 0;
			virtual void echoRoleChanged(const UniString &admin, const UniString &who, ChannelRoler from , ChannelRoler to, const UniString &where) = 0;

			/**
			 * @param creator	��Ƶ��������
			 * @param sid		��Ƶ����id
			 * @param pid		��Ƶ���ĸ�Ƶ��id
			 */
			virtual void echoAddSubChannel(const UniString& creator, const UniString& sid, const UniString& pid) = 0;

			/**
			 * @param creator	��Ƶ��ɾ����
			 * @param sid		��Ƶ����id
			 * @param pid		��Ƶ���ĸ�Ƶ��id
			 */
			virtual void echoDelSubChannel(const UniString& admin, const UniString& sid, const UniString& pid) = 0;

			/**
			 * ���˽�������еĻ���
			 *
			 * @param admin	������
			 * @param uid	������
			 */
			virtual void echoTuoRenToMaixuQueue(const UniString& admin, const UniString& uid) = 0; 

			/**
			 * �����߳�������еĻ���
			 *
			 * @param admin	������
			 * @param uid	������
			 */
			virtual void echoKickOffMaixuQueue(const UniString& admin, const UniString& uid) = 0;

			/**
			 * �������Ļ���
			 *
			 * @param admin	������
			 */
			virtual void echoClearMaixuQueue(const UniString& admin) = 0;

			/**
			 * �Ĺ����Ļ���
			 * 
			 * @param admin �޸Ĺ�����
			 */
			virtual void echoBulletinChange(const UniString& admin) = 0;

			/**
			 * �Ĺ���logo���֪ͨ
			 */
			virtual void onGuildLogoChange(void) = 0;

			/**
			 * @param operater	������
			 * @param sid		Ƶ����id
			 * @param bset		trueΪ���ýӴ�Ƶ����falseΪȡ���Ӵ�Ƶ��
			 */
			virtual void echoAlterJiedai(const UniString& operater, const UniString& sid, const bool& bset) = 0;

			/**
			 * @param operater	������
			 * @param sid		Ƶ����id
			 * @param status		1��ʾ���������˴��֣�2��ʾ��ֹ�����˴��֣�3��ʾ��ֹ�οʹ���
			 */
			virtual void echoChannelTextStatus(const UniString& operater, const UniString& sid, uint32_t status) = 0;

			virtual void echoIsValidMgmtPasswd(const bool isValid) = 0;

			virtual void onCodecTypeChange(const protocol::session::CodecRate preCodecType) = 0;

			virtual void onMicrophoneTimeChange(const uint32_t& newTime, const UniString &uid) = 0;

			virtual void echoGuestVoiceChat(const bool& bLimit) = 0;

			virtual void guestVoiceChatChange(const UniString& sid) = 0;

			virtual void onOverViceOwnerNumLimit() = 0;

            //�յ�������ƽ̨��Ϣ��֮��ص��ͷ��˵Ľӿ�
            virtual void getAppSessionData(const std::string &data) = 0;
            //֪ͨ�ͻ���appSession��״̬��appSessionStatusĿǰȡ4��ֵ������=1���Ͽ�=2;��������=3;�л�=4
            virtual void onAppSessionStatus(uint32_t appTypeId, uint32_t appSessionStatus) = 0;
            // add App plugin
            virtual void onAddApp(const yyplugin::appMgr::PluginInfo& appPluginInfo) = 0;
            virtual void onDelApp(uint32_t appTypeId, bool opByOw, std::wstring& appName) = 0;
            virtual void onAppDownLinkData(uint32_t appTypeId, std::string& appData) = 0;

			virtual void onCustomMessage(uint32_t fromUid, const std::string &msg) = 0;

			//˽�Ĺ��ٱ�����
			virtual void onAdReport(const UniString uid, const UniString sid) = 0;
		};

		struct ISessionWatchBase{
			virtual void watch(ISessionWatcher *) = 0;
			virtual void revoke(ISessionWatcher *) = 0;
		};

		struct IGuildActionWatcher{
			virtual void onLeave(uint32_t sid) = 0;
			virtual void onSettle(uint32_t sid, uint16_t resCode) = 0;
		};

		typedef struct lossRate{
			uint64_t fromId;
			uint64_t toId;
			uint32_t sendCount;
			uint32_t recvCount;
		}lossRate;
		typedef std::vector<lossRate> routeLossRate;
		typedef std::vector<routeLossRate> routeLossRateReport;
		typedef std::vector<routeLossRateReport> routeLossRateReports;

		struct IAudioSession
		{	
		public:
			//������Ƶ������
			virtual void SetAudioDataMgr(IAudioDataMgr* pAudioDataMgr) = 0;

			virtual void stopVoice(bool broadcast) = 0;

			virtual void broadcastVoice(const VoiceChat &voice, const UniString &chId,
				uint32_t seq, uint32_t receiverMask) = 0;

			virtual int say(const VoiceChat &voice, uint32_t seq) = 0;
		};

		struct ISession: 
			public virtual IOperatorWatchBase, 
			public virtual ISessionWatchBase,
			public virtual IAudioSession
		{
			enum ChatResult{
				SENDED,
				TEXT_OVERFLOW,
				VOICE_OVERFLOW
			};

			enum ChatType
			{
				TEXT,
				VOICE
			};
			//��ȡUI��ʾ��SID
			virtual UniString getSid() = 0;  // ��λid
			virtual UniString getCur() = 0;
						
			virtual ISessionMemberList *getSessionList() = 0;

			// TODO: ���ص�	ISesisonInfo* pSInfo
			//				pSinfo->getPid()��û��_@S��׺��Ƶ��id
			virtual ISessionInfo *getSessionInfo(const UniString &sid) = 0;
			virtual IMicrophoneList *getMicrophoneList() = 0;
			virtual IRulerChecker *getRulerChecker() = 0;
			virtual void changeStyle(ChannelStyle s) = 0;
			virtual ChannelStyle getCurrentStyle() = 0;

			virtual void createSubChannel(const SessionCreateInfo& info) = 0;
			virtual void dismissSubChannel(const UniString &sid) = 0;
			
			virtual int say(const TextChat &text) = 0;

			virtual void sayTo(const UniString& uid, ChatType type) = 0;

			virtual UniString getToUser(ChatType type) = 0;

			virtual void leave() = 0;

			virtual void invite(const UniString &uid) = 0;
			virtual void accept() = 0;

			virtual void watchChat(IChatWatch *) = 0;
			virtual void revokeChat(IChatWatch *) = 0;
			virtual UniString getLastError() const = 0;
		
			virtual int calcNetLatence(int timeStamp) = 0;

			virtual uint32_t getPing(void) = 0;
			virtual uint32_t getP2pRtt() const = 0;
			virtual uint32_t getUnlinkP2pRtt() const = 0;
			virtual uint32_t getDownlinkP2pRtt() const = 0;
			virtual uint32_t getMaxDlP2pRtt() const = 0;
			virtual uint32_t getMinDlP2pRtt() const = 0;
			virtual uint32_t getMaxUlP2pRtt() const = 0;
			virtual uint32_t getMinUlP2pRtt() const = 0;
			virtual uint32_t getMaxP2pRtt() const = 0;
			virtual uint32_t getMinP2pRtt() const = 0;
			virtual void resetP2pRtt() = 0;
			virtual uint32_t getAvgP2pRtt() const = 0;
			virtual void getAvgSegRttInfo(std::vector<std::pair<std::string, std::string> >& segRttInfo) = 0;
			virtual void getMaxSegRttInfo(std::vector<std::pair<std::string, std::string> >& segRttInfo) = 0;

			virtual	void getRouteLossRateReports(uint32_t uid,routeLossRateReports& routeData) const = 0;

			virtual	bool checkSessionVersion() const = 0;

			virtual int	 getServerTime() = 0;
			//��ȡ��������
			virtual CodecRate getCodecRate() = 0;

			virtual void sendSessMgmtPasswd(const std::string& passwd) = 0;

			virtual std::vector<std::wstring> getSrvIpAddr() = 0;
			virtual std::vector<std::wstring> getMediaIpAddr() = 0;
			virtual std::wstring getVoicePkgSrcIp() const = 0;
			virtual uint8_t getVoicePkgSrcConnType() const = 0;
			virtual uint32_t getAudioVoiceSendCount() const = 0;
			virtual uint32_t getAudioVoicePlayCount() const = 0;

			/******************************
			*****����ƽ̨�ӿ�
			******************************/
			//�����õ���·�����򷵻�true�����򷵻�false
			virtual bool sendAppSessionData(const std::string& data) = 0;
			virtual bool sendPluginStatus(uint32_t pluginVersion) = 0;
			virtual uint32_t getAppTypeId() = 0;
			virtual uint32_t getPluginVersion() = 0;
			virtual uint32_t getForceUpdateVersion() = 0; 
			virtual UniString getPluginMD5() = 0; 
			virtual UniString getAppName() = 0;
			virtual UniString getPluginURL() = 0;
			virtual bool sendAppUserData(uint32_t appTypeId, std::string& appUsrData) = 0; // sendAppSessionData() is obsoleted.

			/****************************************************
			���л� �����޸��û��ǳƵĽӿڣ������޸���ʱ�û����ǳ�
			*****************************************************/
			virtual void changeNick(const std::string &newNick, uinfo::SEX s, const std::string &sign) = 0;

			// uid: broadcast to everyone if uid is empty.
			virtual bool sendCustomMessage(const UniString &toUid, const std::string &msg) = 0;

			//wuji start
			virtual void GetStatics(UniString &strMsg)=0;//�����Ϣ����ͳ�ƣ���������ǰ�ˣ�ý��ǰ�ˣ������д����
			//wuji end

			virtual void startBroadcastVoice(const std::vector<uint32_t>& subs) = 0;
			virtual void stopBroadcastVoice() = 0;
			virtual void broadcastText2(const TextChat &text, const UniString &chId,
										uint32_t receiverMask, const std::vector<uint32_t>& subs) = 0;
			virtual uint32_t getUnFrozenTime() const = 0;
			virtual bool hasJiedai() const = 0;

			virtual void sendAdReportToSignalProxy(const UniString &adUid) = 0;

		};
	}
}
