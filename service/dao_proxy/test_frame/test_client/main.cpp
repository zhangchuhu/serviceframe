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

using namespace std;
using namespace seda;
using boost::shared_ptr;

using namespace server::thrift;
using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

std::string gl_callname;

boost::shared_ptr<TProtocolFactory> m_inputProtocolFactory;

int generate_routeresp_pack(int32_t seqid,RouteReq* rtreq,std::string& out_thrift)
{
        char bbbb[8];
        uint32_t cid=0;
        char* buf=bbbb;
        size_t len=5;
        shared_ptr<BufferThriftTransport> output(new BufferThriftTransport(cid, buf, len));
        shared_ptr<TProtocol> outputProtocol = m_inputProtocolFactory->getProtocol(output);

        ::apache::thrift::protocol::TProtocol* oprot = outputProtocol.get();

        std::string fname="RouteReq";
        oprot->writeMessageBegin(fname, ::apache::thrift::protocol::T_CALL, seqid);
        rtreq->write(oprot);
        oprot->writeMessageEnd();
        oprot->getTransport()->flush();
        oprot->getTransport()->writeEnd();
        ///////////////////
        out_thrift.clear();
        output->getOutputBuffer(out_thrift);
        return 0;
}

class Press_test
{
public:
	Press_test(){}
	~Press_test(){}
public:
	void push_X(uint32_t x)
	{
		std::string ss;
		std::vector<char> snd;
		for(uint32_t i=0;i<x;++i)
		{
			ss.append(yum.data(),yum.data()+yum.size());
		}
		snd.assign(ss.begin(),ss.end());
		rc.call(snd,0,snd,800);
	}
	void gen_thrift_head()
	{
		shared_ptr< TProtocolFactory > protocolFactory( new TBinaryProtocolFactory(20*1024*1024, 0, false, true) ); //setStringSizeLimit to 20MB
		m_inputProtocolFactory=protocolFactory;
		//
		RouteReq rt;
		rt.context="";
		rt.caller_name=gl_callname;
		rt.authorize_token="";//
		rt.route_option="";
		rt.priority_hint=0;
		rt.trace_debug="test_client";
		rt.coding=0;
		rt.upper_layer.clear();
		rt.upper_layer.append(g+4,g+31);
		std::string nohead_tem;
		generate_routeresp_pack(0,&rt,nohead_tem);
		////////////
		char dtt[4];
		*((int*)dtt) = htonl(nohead_tem.size());
		yum.clear();
		yum.assign(dtt,4);
		yum.append(nohead_tem);
	}

public:
	remote_call rc;
	unsigned char g[31];
	std::string yum;
};



int main(int argc,char* argv[])
{
	if(argc!=6)
	{
		cerr <<"Usage: " <<argv[0] <<" <prx_ip> <port> <sum_pack> <pack_pers> <call_name>" <<endl;
		return -1;
	}
	std::string prx_ip;
	int portt;
	uint32_t sum_packs=0;
	uint32_t pack_pers=0;
	prx_ip=argv[1];
	JLexical::Cast(argv[2],portt);
	JLexical::Cast(argv[3],sum_packs);
	JLexical::Cast(argv[4],pack_pers);

	gl_callname=argv[5];

	////prepare packet
	unsigned char f[31];
	f[0]=0;////back pack len
	f[1]=0;
	f[2]=0;
	f[3]=0x1B;///back len
	f[4]=0x80;///version
	f[5]=0x01;
	f[6]=0;
	f[7]=0x01;///type
	f[8]=0;   ///name len
	f[9]=0;
	f[10]=0;
	f[11]=0x0F;  ///
	f[12]='d';  ///name contex
	f[13]='a';
	f[14]='o';
	f[15]='_';
	f[16]='m';
	f[17]='a';
	f[18]='l';
	f[19]='f';
	f[20]='u';
	f[21]='n';
	f[22]='c';
	f[23]='t';
	f[24]='i';
	f[25]='o';
	f[26]='n';
	f[27]=0;
	f[28]=0;
	f[29]=0;
	f[30]=0;

	////////////////
	RunEnv ev;
	Stage<Press_test>* stg=ev.create_stage<Press_test>(655360, 20);
	stg->get_handle()->rc.setup(prx_ip.c_str(),portt,200);
	memmove(stg->get_handle()->g,f,31);
	stg->get_handle()->gen_thrift_head();
	//stg->get_handle()->push_X(2);
	//return 0;

	ev.start();
	for(uint32_t cs=0;cs<sum_packs;cs+=pack_pers)
	{
		uint32_t pakpert=pack_pers/20;
		if(pakpert==0)
		{
			pakpert=1;
		}
		//////////////////////
		for(int i=0;i<20;++i)
		{
			stg->push(&Press_test::push_X,pakpert,true);
			usleep(50000);
		}
	}
	return 0;
}


