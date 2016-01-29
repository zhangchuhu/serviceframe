#include "PxThriftMsgHandler.h"

#include "server_common/server-lib/id_def.h"
#include "common/core/form.h"
#include "server_back/thrift-bridge/BufferThriftTransport.h"
#include <transport/TTransportException.h>
#include <protocol/TBinaryProtocol.h>
#include <TApplicationException.h>
#include "GlobalCommon.h"
#include "request_fsm.h"

using namespace std;
using namespace server::dao_proxy;
using namespace core;

using namespace server::thrift;
using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

using boost::shared_ptr;


ThriftPxMsgHandler::ThriftPxMsgHandler()
{
	shared_ptr< TProtocolFactory > protocolFactory( new TBinaryProtocolFactory(20*1024*1024, 0, false, true) ); //setStringSizeLimit to 20MB
	m_inputProtocolFactory=protocolFactory;
}

ThriftPxMsgHandler::~ThriftPxMsgHandler()
{
	
}

int ThriftPxMsgHandler::process(uint32_t cid, const char *buf, size_t len)
{
	try{
		shared_ptr<BufferThriftTransport> input(new BufferThriftTransport(cid, buf, len));
		shared_ptr<TProtocol> inputProtocol = m_inputProtocolFactory->getProtocol(input);
		bool hit_hooker=false;
		try
		{
			///decode head
			::apache::thrift::protocol::TProtocol* iprot = inputProtocol.get();
			std::string fname;
			std::string fname_full;
			::apache::thrift::protocol::TMessageType mtype;
			int32_t seqid;
			iprot->readMessageBegin(fname_full, mtype, seqid);

			RouteReq* proute_header=0;
			RouteReq route_header;
			
			if(fname_full=="RouteReq")
			{
				/////route head
				route_header.read(iprot);
				proute_header=&route_header;
				iprot->readMessageEnd();
				{
					const char * buf_L=route_header.upper_layer.data();
					size_t len_L=route_header.upper_layer.size();
					shared_ptr<BufferThriftTransport> input_L(new BufferThriftTransport(cid
						,buf_L  ,len_L  ));
					shared_ptr<TProtocol> inputProtocol_L = m_inputProtocolFactory->getProtocol(input_L);
					::apache::thrift::protocol::TProtocol* iprot_L = inputProtocol_L.get();
					std::string fname_L;
					std::string fname_full_L;
					::apache::thrift::protocol::TMessageType mtype_L;
					int32_t seqid_L;
					iprot_L->readMessageBegin(fname_full_L, mtype_L, seqid_L);
					{
						fname_L=fname_full_L;
						///////////fix service prefix compatible
						size_t divp=fname_L.find_first_of('.',0);
						if(divp!=std::string::npos && divp!=0)
						{
							fname_L=fname_L.substr(0,divp);
						}
						///new request fsm
						ReqFSM* pfsm=ReqFSM::createFSM();
						pfsm->headofthrift_headid=seqid;
						pfsm->start(seqid_L,cid,fname_full_L,proute_header ,fname_L,buf_L,len_L);
						if(gl_msghook->is_hooked(&(proute_header->caller_name) ,&fname_L))
						{
							hit_hooker=true;
						}
					}
				}
			}
			else
			{
				/////no head call
				fname=fname_full;
				///////////fix service prefix compatible
				size_t divp=fname.find_first_of('.',0);
				if(divp!=std::string::npos && divp!=0)
				{
					fname=fname.substr(0,divp);
				}
				///new request fsm
				ReqFSM* pfsm=ReqFSM::createFSM();
				pfsm->start(seqid,cid,fname_full,NULL,fname,buf,len);
				if(gl_msghook->is_hooked(0 ,&fname))
				{
					hit_hooker=true;
				}
			}
			////////////////////////////////////////////
			if(hit_hooker&&gl_msghook->b_active)
			{
				/////////////send to that cid
				char dtt[4];
				*((int*)dtt) = htonl(len);

				IConn* pcon = gl_service_connection_manager->getConnectById((uint32_t)gl_msghook->m_cid);
				if(pcon)
				{
					try{
						pcon->sendBin(dtt,4 , -1);
						pcon->sendBin(buf,len , -1);
					}catch(std::exception &ex){
					pcon->setEnable(false);
					log(Error, "send error!:%s", ex.what());
					gl_msghook->clear_hook();
					}
				}
				else
				{
					gl_msghook->clear_hook();
				}
			}
		}
		catch (TTransportException& ttx)
		{
			log(Warn, "ThriftPxMsgHandler client exception: %s", ttx.what());
		}
		catch (TException& tx)
		{
			log(Warn, "ThriftPxMsgHandler exception: %s", tx.what());
		}
	}
	catch(std::exception &ex )
	{
		log(Warn, "unkown excpetion :%s", ex.what());
	}
	return 0;
}

int ThriftPxMsgHandler::generate_fname_not_found_pack(int32_t seqid,std::string& fname,std::string& out_thrift)
{
	char bbbb[8];
	uint32_t cid=0;
	char* buf=bbbb;
	size_t len=5;
	shared_ptr<BufferThriftTransport> output(new BufferThriftTransport(cid, buf, len));
	shared_ptr<TProtocol> outputProtocol = m_inputProtocolFactory->getProtocol(output);

	::apache::thrift::protocol::TProtocol* oprot = outputProtocol.get();

	::apache::thrift::TApplicationException x(
		::apache::thrift::TApplicationException::UNKNOWN_METHOD
		, "Invalid method name: '"+fname+"'");
	oprot->writeMessageBegin(fname, ::apache::thrift::protocol::T_EXCEPTION, seqid);
	x.write(oprot);
	oprot->writeMessageEnd();
	oprot->getTransport()->flush();
	oprot->getTransport()->writeEnd();
	///////////////////
	out_thrift.clear();
	output->getOutputBuffer(out_thrift);
	return 0;
}
int ThriftPxMsgHandler::generate_fname_unknown_exception_pack(int32_t seqid,std::string& fname,std::string& exception_desc,std::string& out_thrift)
{
	char bbbb[8];
	uint32_t cid=0;
	char* buf=bbbb;
	size_t len=5;
	shared_ptr<BufferThriftTransport> output(new BufferThriftTransport(cid, buf, len));
	shared_ptr<TProtocol> outputProtocol = m_inputProtocolFactory->getProtocol(output);

	::apache::thrift::protocol::TProtocol* oprot = outputProtocol.get();

	::apache::thrift::TApplicationException x(
		::apache::thrift::TApplicationException::UNKNOWN
		, exception_desc);
	oprot->writeMessageBegin(fname, ::apache::thrift::protocol::T_EXCEPTION, seqid);
	x.write(oprot);
	oprot->writeMessageEnd();
	oprot->getTransport()->flush();
	oprot->getTransport()->writeEnd();
	///////////////////
	out_thrift.clear();
	output->getOutputBuffer(out_thrift);
	return 0;
}
int ThriftPxMsgHandler::generate_routeresp_pack(int32_t seqid,RouteResp* rtresp,std::string& out_thrift)
{
	char bbbb[8];
	uint32_t cid=0;
	char* buf=bbbb;
	size_t len=5;
	shared_ptr<BufferThriftTransport> output(new BufferThriftTransport(cid, buf, len));
	shared_ptr<TProtocol> outputProtocol = m_inputProtocolFactory->getProtocol(output);

	::apache::thrift::protocol::TProtocol* oprot = outputProtocol.get();

	std::string fname="RouteResp";
	oprot->writeMessageBegin(fname, ::apache::thrift::protocol::T_REPLY, seqid);
	rtresp->write(oprot);
	oprot->writeMessageEnd();
	oprot->getTransport()->flush();
	oprot->getTransport()->writeEnd();
	///////////////////
	out_thrift.clear();
	output->getOutputBuffer(out_thrift);
	return 0;
}

