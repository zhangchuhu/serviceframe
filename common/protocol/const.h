#ifndef CROSS_PLANTFORM_CONST_H_
#define CROSS_PLANTFORM_CONST_H_

#define LBS_SERVER_PORT1 1001
#define LBS_SERVER_PORT2 2001
#define LBS_SERVER_PORT3 3001
#define LBS_SERVER_PORT80 80


#define MAX_SVID_NUM 20
#define SOCKET_TIMEOUT 60000
#define MYCOLLECTION_FOLDER 1000000000
#define PUBLICCHANNEL_FOLDER 1000000001
#define RECEMMONDCHANNEL_FOLDER 1000000002
#define HOT_FOLDER 1000000003
#define UDP_OFFSET 10000
#define INVALID_UID 0xffffffff
#define INVALID_CID 0xffffffff
#define NONE_SERVER_ID 0xffffffff

#define MAX_CHANNEL_SIZE 3
#ifdef WIN32
#define FAV_FOLDER		_T("1000000000")
#define FAV_PUBLIC		_T("1000000001")
#define FAV_RECOMMEND	_T("1000000002")
#define FAV_HOT			_T("1000000003")
#define KICKOUT_PID _T("")
#endif

#define SEQPAGE					1000
#define PACKET_SEQ_INCREMENT	2
#define SEQORIGINAL				1000

#define DEF_SESSIONKEY_LENGTH 16

#define ADMIN_CHANNEL 0

#define NO_IMID -1

namespace protocol{
	enum E_ONLINESTAT{ONLINE, OFFLINE};
	enum E_IMSTATUS
	{
		IMONLINE, 
		IMHIDE, 
		IMOFFLINE,
		ENUM_IMSTATUS_BUSY,    //  在protocol中提前做好兼容，对UI来说 等同于在线
		ENUM_IMSTATUS_LEAVE,   //  方便后续忙碌、离开等状态的扩展
		ENUM_IMSTATUS_GAMING   //  游戏中
	};
}

#define ISONLINE(imState) (((imState) == protocol::IMOFFLINE || (imState) == protocol::IMHIDE) ? false : true)

namespace protocol{
	namespace im{
		enum E_WAITCHECKLIST_OP{ ACCEPT, NOTACCEPT,CANCEL };//cancel代表既不同意也不拒绝
		enum E_ITEMTYPE{	BUDDY,	FOLDER};
	}
}



namespace protocol {
namespace uinfo {
enum SEX {
	female,
	male,
	unknown
};

}
}

namespace protocol{
namespace login{
enum LinkStatus {
	//sync_uinfo = 100,
	//sync_ulist = 180,
	LINK_INIT,
	LINK_LBS,
	EXCHANGE_PASSWORD,
	LBS_ERROR,
	LINK_AUTH,
	LOGIN_SUCCESS,
	LOGIN_RETRY,
	PASSWD_ERROR,
	SERVER_ERROR,
	NET_BROKEN,
	TIMEOUT,
	KICKOFF,
	LOGOUT,
	UNKNOWN,
	PROTOCOL_OLD,
	LINK_GETMAIL,
	NON_EMAIL,
	LBS_SHUTDOWN,
	RELOGIN_SUCCESS,
	SERVER_REDIRECT,
	USER_NONEXIST,
	UDB_NOTENABLE,
	GLOBAL_BAN,
	MUL_KICK,			//对多开用户发送踢人消息时使用
	IM_LOGIN_ERROR,
	IM_LOGIN_SUCCESS,
	IM_LOGOUT,
	IM_LOGIN_NO_IMID,		//用户没有imid, 不能登陆,限号测试时使用



	//added by zzw , 20091012
	IM_LOGIN_IMLINK_TIMEOUT,  //连接imlinkd超时
	IM_LOGIN_IMLBS_TIMEOUT,  //连接imlbsd超时
	IM_LOGIN_LOGINING,  //登陆中

	//wuji start 2009.10.20
	IM_LOGIN_REQUEST_IMID_FAILED,		//用户申请不了imid
	FREEZE_BAN,
	IM_RELOGINED,							//通知UI，IM重登成功
	IM_LOGIN_START, 						//IM开始登陆
	//wuji end 

	//end
	//success = 200,
	//	passwd_err = 401,
	//	
	// timeout	  = 405,
	//	unknow	  = 406
};

enum LinkStatus2 {
	GET_CODE_INFO,			//下发代码使用，服务器返回代码列表
	GET_CODE,				//下发代码使用，服务器根据代码ID返回代码
};

}
}

enum ISPType{
	AUTO_DETECT = 0,
	CTL = 1,	//电信
	CNC = 2,	//网通
	CNII = 4,	//铁通
	EDU = 8,	//教育网
	WBN	= 16,	//长城宽带
	MOB = 32,  //移动
	BGP = 64,  //BGP
	HK  = 128, //香港
	BRA = 256, // 巴西

	GENERAL_DEPLOY = 3,	// general session, main deploy type
	SIFU_DEPLOY = 45,		
	PIP_DEPLOY = 46,
	IM_DEPLOY = 47,
	MAFIA_DEPLOY = 48,
};

enum AreaType{//同网络类型的定义数字必需连续(分配的时候会用)
  AREA_UNKNOWN = 0,
  CTL_EAST = 16,    //电信东区     10000  (1<<4) + 0
  CTL_WEST = 17,    //电信西区     10001  (1<<4) + 1
  CTL_SOUTH = 18,   //电信南区     10010  (1<<4) + 2
  CTL_NORTH = 19,   //电信北区     10011  (1<<4) + 3
  CNC_NE = 32,      //网通东北    100000 (2<<4) + 0
  CNC_NC = 33,      //网通华北    100001 (2<<4) + 1
  CNII_AREA = 64,   //铁通       1000000 (4<<4) + 1
  EDU_AREA = 128,   //教育网    10000000 (8<<4) +0
  WBN_AREA = 256,	  //长城宽带 100000000 (16<<4) +0
  MOB_AREA = 512,   //移动    1000000000 (32<<4) + 0
  BGP_AREA = 1024,  //BGP    10000000000 (64<<4) + 0
  HK_AREA  = (128<<4), //香港
  BRA_AREA = (256<<4), //巴西
};

enum ChannelDeploy {
	UNKNOWN_SESSION = 0,
	GENERAL_SESSION = 3,	// general session, main deploy type
	SIFU_SESSION = 45,		
	PIP_SESSION = 46,
	IM_SESSION = 47,
	MAFIA_SESSION = 48,
};
enum KickType{
	KICKOUT_CHANNEL,
	LOGIN_FREEZED
};

namespace protocol
{
namespace sysmsg
{
	enum SysMsgDisplayType
	{
		//10秒后自动关闭系统消息窗口
		ENUM_SYSMSG_AUTO_CLOSE = 0,
		//用户手动关闭
		ENUM_SYSMSG_USER_CLOSE,

		ENUM_SYSMSG_PRIVILEDGE,
		//邮箱密保
		ENUM_SYSMSG_MAILCHANGE,

		//频道活动提醒
		ENUM_SYSMSG_ACTIVITY
	};
}
namespace uinfo{
	enum ApplyResult{
		APPLY_SUCCESS,
		NOT_ENOUGH_JIFEN,
		IN_QUIT_COOLDOWN,
		EXCEED_MAX_APPLYCOUNT,
		ALREADY_MEMBER,
		WAITFOR_APPROVE,
	};
}
}

namespace protocol
{
namespace p2p
{

	enum P2PTransmitType
	{
		ENUM_P2P_TRANSMIT_FILE = 0,		//传输文件
		ENUM_P2P_TRANSMIT_DIRECTORY,	//文件夹
		ENUM_P2P_TRANSMIT_CHAT,			//聊天消息
		//ENUM_P2P_TRANSMIT_VOICE			//语音
	};

	enum P2PTransmitAction
	{
		ENUM_P2P_ACTION_ACCEPT = 0,	//对方已接受
		ENUM_P2P_ACTION_REFUSE,		//对方已拒绝
		ENUM_P2P_ACTION_NOT_SUPPORT      //对方版本不支持
	};

	enum P2PTransmitStatus
	{	
		ENUM_P2P_TRANSMIT_WAITING = 0,		//正在建立连接
		ENUM_P2P_TRANSMIT_READY,			//双方连接已建立，开始传输
		ENUM_P2P_TRANSMIT_FINISH,			//传输完成
		ENUM_P2P_TRANSMIT_IO_ERROR,			//文件系统错误			
		ENUM_P2P_TRANSMIT_NETWORK_ERROR,	//传输出现错误，任务已中断
		ENUM_P2P_TRANSMIT_FAILED,			//连接建立失败
		ENUM_P2P_TRANSMIT_CANCEL,
		ENUM_P2P_TRANSMIT_UNKNOW
	};

	enum P2PNatType
	{
		ENUM_P2P_NAT_UNKNOWN = 0,
		ENUM_P2P_NAT_OPEN_INTERNET,
		ENUM_P2P_NAT_FULL_CONE,
		ENUM_P2P_NAT_RESTRICT_CONE,
		ENUM_P2P_NAT_UDP_FIREWALL,
		ENUM_P2P_NAT_SYMMETRIC,
		ENUM_P2P_NAT_UDP_BLOCK
	};

	enum E_P2P_MYROLE
	{
		E_P2P_SENDER = 0,
		E_P2P_RECEIVER
	};
	
	enum E_P2P_METHOD
	{
		E_P2P_UDP = 0,
		E_P2P_TCP
	};

}
}

#endif /*CROSS_PLANTFORM_CONST_H_*/
