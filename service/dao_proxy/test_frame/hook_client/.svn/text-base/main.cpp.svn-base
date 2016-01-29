#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdint.h>
#include <signal.h>
#include "server_common/seda/syncall/remote_call.hpp"
#include "server_common/seda/common/any_cast.h"
#include "server_common/seda/stage/stage_base.h"
#include "route_types.h"
#include "server_back/thrift-bridge/BufferThriftTransport.h"
#include <transport/TTransportException.h>
#include <protocol/TBinaryProtocol.h>
#include <TApplicationException.h>
#include <boost/shared_ptr.hpp>
#include "common/core/sender.h"
#include "common/core/request.h"
#include "server_common/protocol/pdbnode.h"
#include "hook_call.hpp"

using namespace std;
using namespace seda;


using namespace server::db_node;
using core::Sender;
using core::Request;
using boost::shared_ptr;

using namespace server::thrift;
using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;


class hookframe: public ISeqFraming
{
public:
	hookframe(){}
	virtual ~hookframe(){};
	///more==0 means no more, more==1 means is more
	virtual int frame(const char* dat,int len,int& more)
	{
		if (len < 10)
		{
			return 10;
		}
		////
		int fea1=0;
		int fea2=0;
		unsigned char* testbyte=(unsigned char*)dat;
		////FeatureOne
		if(*(testbyte+4)==(unsigned char)(128))
		{///thrift
			fea1=1;
		}
		else
		{///YY
			fea1=2;
		}
		////FeatureTwo
		if( (*(testbyte+8)==(unsigned char)(0))
			&&(*(testbyte+9)==(unsigned char)(0))  )
		{///thrift
			fea2=1;
		}
		else
		{///YY
			fea2=2;
		}
		////////////////////////////////////////
		if(fea2!=fea1)
		{
			cerr<<"[hookframe::frame]: discrimination conflict";
			return -1;
		}
		//////////////////////
		if(fea1==1)
		{//Thrift
			uint32_t length = *(uint32_t*)dat;
			length = (uint32_t)ntohl(length);
			length+=4;

			more=1;
			if(len>=(int)length)
			{
				cout <<"thrift length: " <<length <<endl;
			}
			return length;
		}
		else
		{//YY
			uint32_t length = *(uint32_t*)dat;
			more=0;
			if(len>=(int)length)
			{
				cout <<"yy length: " <<length <<endl;
			}
			return length;
		}
		return -1;
	}
};


int main(int argc,char* argv[])
{
	if(argc!=4)
	{
		cerr <<"Usage: " <<argv[0] 
		<<" <ip> <port> <map>(app=$,dao=$,sendperten=$,charge=$)" <<endl;
		return -1;
	}

	////connect
	string ip=argv[1];
	int port=0;
	JLexical::Cast(argv[2],port);
	string command_map=argv[3];

	hook_call hcc;
	hcc.setup(ip.c_str(),port,1);

	////send
	server::db_node::DBAdminReq req;
	req.req_id=rand();
	req.option=111;
	req.command=command_map;

	Sender sdr(server::db_node::DBAdminReq::uri, req);
	sdr.setResCode(RES_SUCCESS);
	sdr.endPack();
	vector<char> send;
	send.assign(sdr.header(),sdr.header()+sdr.bodySize()+sdr.headerSize());

	////wait receive
	hookframe hff;

	hcc.call(send,&hff,300000);

	return 0;
}


