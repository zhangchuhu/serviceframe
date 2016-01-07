#ifndef CHANNELROLERENUM_H_
#define CHANNELROLERENUM_H_
namespace protocol {
namespace session {

enum ChannelRoler {
	NUL_ROLE = 0,
	VISITOR = 20,   //VISITOR
	NORMAL = 25,		//U
	DELETED = 50,		//unuse
	TMPVIP = 66,		//TMPVIP
	VIP = 88,			//VIP
	MEMBER = 100,		//R
	CMANAGER = 150,		//CA
	PMANAGER = 175,		//PA
	MANANGER = 200,		//MAN
	VICE_OWNER = 230,   //VO
	OWNER = 255,		//OW
	KEFU = 300,			//CS
	POLICE = 400,       //
	SA = 1000,			//SA
};

}
}
#endif /*CHANNELROLERENUM_H_*/
