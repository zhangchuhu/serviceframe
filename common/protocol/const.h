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
		ENUM_IMSTATUS_BUSY,    //  ��protocol����ǰ���ü��ݣ���UI��˵ ��ͬ������
		ENUM_IMSTATUS_LEAVE,   //  �������æµ���뿪��״̬����չ
		ENUM_IMSTATUS_GAMING   //  ��Ϸ��
	};
}

#define ISONLINE(imState) (((imState) == protocol::IMOFFLINE || (imState) == protocol::IMHIDE) ? false : true)

namespace protocol{
	namespace im{
		enum E_WAITCHECKLIST_OP{ ACCEPT, NOTACCEPT,CANCEL };//cancel����Ȳ�ͬ��Ҳ���ܾ�
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
	MUL_KICK,			//�Զ࿪�û�����������Ϣʱʹ��
	IM_LOGIN_ERROR,
	IM_LOGIN_SUCCESS,
	IM_LOGOUT,
	IM_LOGIN_NO_IMID,		//�û�û��imid, ���ܵ�½,�޺Ų���ʱʹ��



	//added by zzw , 20091012
	IM_LOGIN_IMLINK_TIMEOUT,  //����imlinkd��ʱ
	IM_LOGIN_IMLBS_TIMEOUT,  //����imlbsd��ʱ
	IM_LOGIN_LOGINING,  //��½��

	//wuji start 2009.10.20
	IM_LOGIN_REQUEST_IMID_FAILED,		//�û����벻��imid
	FREEZE_BAN,
	IM_RELOGINED,							//֪ͨUI��IM�صǳɹ�
	IM_LOGIN_START, 						//IM��ʼ��½
	//wuji end 

	//end
	//success = 200,
	//	passwd_err = 401,
	//	
	// timeout	  = 405,
	//	unknow	  = 406
};

enum LinkStatus2 {
	GET_CODE_INFO,			//�·�����ʹ�ã����������ش����б�
	GET_CODE,				//�·�����ʹ�ã����������ݴ���ID���ش���
};

}
}

enum ISPType{
	AUTO_DETECT = 0,
	CTL = 1,	//����
	CNC = 2,	//��ͨ
	CNII = 4,	//��ͨ
	EDU = 8,	//������
	WBN	= 16,	//���ǿ��
	MOB = 32,  //�ƶ�
	BGP = 64,  //BGP
	HK  = 128, //���
	BRA = 256, // ����

	GENERAL_DEPLOY = 3,	// general session, main deploy type
	SIFU_DEPLOY = 45,		
	PIP_DEPLOY = 46,
	IM_DEPLOY = 47,
	MAFIA_DEPLOY = 48,
};

enum AreaType{//ͬ�������͵Ķ������ֱ�������(�����ʱ�����)
  AREA_UNKNOWN = 0,
  CTL_EAST = 16,    //���Ŷ���     10000  (1<<4) + 0
  CTL_WEST = 17,    //��������     10001  (1<<4) + 1
  CTL_SOUTH = 18,   //��������     10010  (1<<4) + 2
  CTL_NORTH = 19,   //���ű���     10011  (1<<4) + 3
  CNC_NE = 32,      //��ͨ����    100000 (2<<4) + 0
  CNC_NC = 33,      //��ͨ����    100001 (2<<4) + 1
  CNII_AREA = 64,   //��ͨ       1000000 (4<<4) + 1
  EDU_AREA = 128,   //������    10000000 (8<<4) +0
  WBN_AREA = 256,	  //���ǿ�� 100000000 (16<<4) +0
  MOB_AREA = 512,   //�ƶ�    1000000000 (32<<4) + 0
  BGP_AREA = 1024,  //BGP    10000000000 (64<<4) + 0
  HK_AREA  = (128<<4), //���
  BRA_AREA = (256<<4), //����
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
		//10����Զ��ر�ϵͳ��Ϣ����
		ENUM_SYSMSG_AUTO_CLOSE = 0,
		//�û��ֶ��ر�
		ENUM_SYSMSG_USER_CLOSE,

		ENUM_SYSMSG_PRIVILEDGE,
		//�����ܱ�
		ENUM_SYSMSG_MAILCHANGE,

		//Ƶ�������
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
		ENUM_P2P_TRANSMIT_FILE = 0,		//�����ļ�
		ENUM_P2P_TRANSMIT_DIRECTORY,	//�ļ���
		ENUM_P2P_TRANSMIT_CHAT,			//������Ϣ
		//ENUM_P2P_TRANSMIT_VOICE			//����
	};

	enum P2PTransmitAction
	{
		ENUM_P2P_ACTION_ACCEPT = 0,	//�Է��ѽ���
		ENUM_P2P_ACTION_REFUSE,		//�Է��Ѿܾ�
		ENUM_P2P_ACTION_NOT_SUPPORT      //�Է��汾��֧��
	};

	enum P2PTransmitStatus
	{	
		ENUM_P2P_TRANSMIT_WAITING = 0,		//���ڽ�������
		ENUM_P2P_TRANSMIT_READY,			//˫�������ѽ�������ʼ����
		ENUM_P2P_TRANSMIT_FINISH,			//�������
		ENUM_P2P_TRANSMIT_IO_ERROR,			//�ļ�ϵͳ����			
		ENUM_P2P_TRANSMIT_NETWORK_ERROR,	//������ִ����������ж�
		ENUM_P2P_TRANSMIT_FAILED,			//���ӽ���ʧ��
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
