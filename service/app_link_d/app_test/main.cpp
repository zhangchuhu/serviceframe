#include <string>

#include "BlockingConn.h"
#include "../../app_protocol/app_protocol.h"
#include "server_common/protocol/prouter.h"

using namespace std;
using namespace core;
using namespace server::router;
using namespace protocol;

int main(int sz, char* args[])
{
	string strIp = "106.38.255.199";
	uint32_t uPort = 19802;

	//if (sz == 3)
	//{
	//	strIp = args[1];
	//	uPort = atoi(args[2]);
	//}
	//else
	//{
	//	cout << "Usage: ./conntest [host port] " << endl;
	//	return 0;
	//}

	CSimpleBlockConn client(strIp, uPort);
	PLoginImLinkd req;
	req.uid=50012417;
	req.cookie="test";
	req.sha_passwd="test";
	req.account="test";
	req.status=APP_IMONLINE;
    client.SendPacket(req.uri, req);
	PCS_ChatMsg chat_msg;
	chat_msg.from_uid=50012417;
	chat_msg.to_uid=50012418;
	chat_msg.text="test";
	chat_msg.m_uSeqId=0;
	chat_msg.m_uSendTime=1;
	chat_msg.m_bSenderBuddy=false;
	client.SendPacket(chat_msg.uri,chat_msg);
	//while(true)
	//{
	//	PCS_ChatMsg receive_msg;
	//	client.GetPacket(receive_msg.uri,receive_msg);
	//	printf("from_uid=%d,to_uid=%d,text=%s",receive_msg.from_uid,receive_msg.to_uid,receive_msg.text.c_str());
	//}
    cout<<"test"<<endl;
	return 0;
}

