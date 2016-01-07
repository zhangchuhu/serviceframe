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
			UniString sid; //服务器填写
			VoiceQC qc;
			ISPType isp;
		};


		struct ISessionManagerWatcher{
			virtual void onRefresh() = 0;
			/*
				err define:
				用户不存在 404
				你没有权限进行该操作（不是该频道管理员,或者权限对抗失败） 403
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
			 * 设置公会logo
			 *
			 * @param	buildInLogoIndex	>=1时表示系统内置logo
											=0时表示用户自定义logo
			 *
			 * @param	logUrl				自定义logo的url。
											若nBuildInLogoIndex>0，传参数时logUrl置空串就可以了。
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
					 设置自由模式下是否禁止游客语音
					 true表示禁止语音，false表示可以语音
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
					CREATE_SUBCHANNEL_CHK,		// 创建子频道
					DISSMISS_SUBCHANNEL_CHK,	// 解散子频道
					CHANGE_INFO,				// 更改子频道信息
					ADD_MANAGER,				// 添加管理员
					REMOVE_MANAGER,				// 删除管理员
					ADD_SUB_MANAGER,			// 删除子频道管理员
					REMOVE_SUB_MANAGER,			// 添加子频道管理员
					ADD_MEMBER,					// 添加成员
					REMOVE_MEMBER,				// 删除会员
					ADD_VICE_OWNER,				// 添加副会长
					REMOVE_VICE_OWNER,			// 删除副会长
					JOIN_CHANNEL,				// 加入限制及频道
					JOIN_WITHOUTPASSWD,			// 加入没有密码频道
					KICKOFF,					// 踢人
					VOICE,						// 语音
					TEXT,						// 文字
					CHANGESTYLE,				// 更改模式
					TUOREN,						// 拖人
					SETKEYACTIVE,				// 键盘激活
					DISABLE_VOICE,				// 禁止语音
					ENABLE_VOICE,				// 启用语音
					ENABLE_TEXT,				// 禁止文本
					DISABLE_TEXT,				// 启用文本
					BROADCAST,					// 广播
					DRAGETO_QUEUE,				// 拖进卖序
					JOIN_QUEUE,					// 抢麦	
					LEAVE_QUEUE,				// 退出麦序
					MOVEUP_QUEUE,				// 上移麦序
					MOVEDOWN_QUEUE,				// 下移麦序
					KICK_QUEUE,					// 踢出麦序
					MUTE_QUEUE,					// 禁止抢麦XXXX 应该是控麦
					UNMUTE_QUEUE,				// 启用抢麦XXXX 应该是放麦
					DOUBLETIME_QUEUE,			// 双倍麦序时间
					DISABLE_QUEUE,				// 禁用麦序
					ALLOW_QUEUE,				// 启用麦序
					DRAG_PEOPLE,				// 拖人
					DROP_PEOPLE,				// 放人

					EDIT_CHANNEL_INFO,			// 编辑频道信息，chenzhou 2008/4/8
					SHOW_AS_MANAGER,			// 是否显示在管理员列表
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
					QUIT_GUILD,					//退出工会
					DRAG_ROOM,					//拖频道
					DROP_ROOM,					//放频道
					DIRECT_KICKOFF,
					SHOW_GUILD_MANAGER			//显示公会管理窗口


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
				HULI   user;	//被操作的人
				HULI   room;	//所处的频道
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


		// TODO: 这个接口太胖，很多时候只用到其中很小一部分接口
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
			
			//修改该接口，加上类型，指出是被踢出频道还是被封ID、IP，0表示被踢出，1表示被封ID, 2表示被封IP};
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
			 * @param creator	子频道创建者
			 * @param sid		子频道的id
			 * @param pid		子频道的父频道id
			 */
			virtual void echoAddSubChannel(const UniString& creator, const UniString& sid, const UniString& pid) = 0;

			/**
			 * @param creator	子频道删除者
			 * @param sid		子频道的id
			 * @param pid		子频道的父频道id
			 */
			virtual void echoDelSubChannel(const UniString& admin, const UniString& sid, const UniString& pid) = 0;

			/**
			 * 拖人进麦序队列的回显
			 *
			 * @param admin	操作者
			 * @param uid	被加者
			 */
			virtual void echoTuoRenToMaixuQueue(const UniString& admin, const UniString& uid) = 0; 

			/**
			 * 将人踢出麦序队列的回显
			 *
			 * @param admin	操作者
			 * @param uid	被踢者
			 */
			virtual void echoKickOffMaixuQueue(const UniString& admin, const UniString& uid) = 0;

			/**
			 * 清空麦序的回显
			 *
			 * @param admin	操作者
			 */
			virtual void echoClearMaixuQueue(const UniString& admin) = 0;

			/**
			 * 改公告后的回显
			 * 
			 * @param admin 修改公告者
			 */
			virtual void echoBulletinChange(const UniString& admin) = 0;

			/**
			 * 改公会logo后的通知
			 */
			virtual void onGuildLogoChange(void) = 0;

			/**
			 * @param operater	操作者
			 * @param sid		频道的id
			 * @param bset		true为设置接待频道，false为取消接待频道
			 */
			virtual void echoAlterJiedai(const UniString& operater, const UniString& sid, const bool& bset) = 0;

			/**
			 * @param operater	操作者
			 * @param sid		频道的id
			 * @param status		1表示允许所有人打字，2表示禁止所有人打字，3表示禁止游客打字
			 */
			virtual void echoChannelTextStatus(const UniString& operater, const UniString& sid, uint32_t status) = 0;

			virtual void echoIsValidMgmtPasswd(const bool isValid) = 0;

			virtual void onCodecTypeChange(const protocol::session::CodecRate preCodecType) = 0;

			virtual void onMicrophoneTimeChange(const uint32_t& newTime, const UniString &uid) = 0;

			virtual void echoGuestVoiceChat(const bool& bLimit) = 0;

			virtual void guestVoiceChatChange(const UniString& sid) = 0;

			virtual void onOverViceOwnerNumLimit() = 0;

            //收到第三方平台信息包之后回调客服端的接口
            virtual void getAppSessionData(const std::string &data) = 0;
            //通知客户端appSession的状态，appSessionStatus目前取4个值，正常=1；断开=2;重新连上=3;切换=4
            virtual void onAppSessionStatus(uint32_t appTypeId, uint32_t appSessionStatus) = 0;
            // add App plugin
            virtual void onAddApp(const yyplugin::appMgr::PluginInfo& appPluginInfo) = 0;
            virtual void onDelApp(uint32_t appTypeId, bool opByOw, std::wstring& appName) = 0;
            virtual void onAppDownLinkData(uint32_t appTypeId, std::string& appData) = 0;

			virtual void onCustomMessage(uint32_t fromUid, const std::string &msg) = 0;

			//私聊广告举报回显
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
			//设置音频流管理
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
			//获取UI显示的SID
			virtual UniString getSid() = 0;  // 长位id
			virtual UniString getCur() = 0;
						
			virtual ISessionMemberList *getSessionList() = 0;

			// TODO: 返回的	ISesisonInfo* pSInfo
			//				pSinfo->getPid()是没有_@S后缀的频道id
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
			//获取语音码率
			virtual CodecRate getCodecRate() = 0;

			virtual void sendSessMgmtPasswd(const std::string& passwd) = 0;

			virtual std::vector<std::wstring> getSrvIpAddr() = 0;
			virtual std::vector<std::wstring> getMediaIpAddr() = 0;
			virtual std::wstring getVoicePkgSrcIp() const = 0;
			virtual uint8_t getVoicePkgSrcConnType() const = 0;
			virtual uint32_t getAudioVoiceSendCount() const = 0;
			virtual uint32_t getAudioVoicePlayCount() const = 0;

			/******************************
			*****开放平台接口
			******************************/
			//发送用的链路正常则返回true，否则返回false
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
			画中画 增加修改用户昵称的接口，用来修改临时用户的昵称
			*****************************************************/
			virtual void changeNick(const std::string &newNick, uinfo::SEX s, const std::string &sign) = 0;

			// uid: broadcast to everyone if uid is empty.
			virtual bool sendCustomMessage(const UniString &toUid, const std::string &msg) = 0;

			//wuji start
			virtual void GetStatics(UniString &strMsg)=0;//获得信息用于统计，例如信令前端，媒体前端，上下行带宽等
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
