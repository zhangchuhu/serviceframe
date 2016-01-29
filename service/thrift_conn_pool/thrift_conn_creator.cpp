#include "thrift_conn_creator.h"
#include "core/sox/logger.h"

using namespace server::config;
using namespace server::thrift;

ThriftConn  ThriftConnCreator::createConnect(const server::config::ZkThriftConfig &param){
	scoped_lock lock(m_mutex);
	ThriftConn conn;
	boost::shared_ptr<apache::thrift::transport::TSocket> pSocket(new apache::thrift::transport::TSocket(param.host,param.port));
	pSocket->setConnTimeout(param.time_out);
	pSocket->setRecvTimeout(param.time_out);
	boost::shared_ptr<apache::thrift::transport::TFramedTransport> pTransport(new apache::thrift::transport::TFramedTransport(pSocket));
	ThriftConn Protocol (new apache::thrift::protocol::TBinaryProtocol(pTransport));
	conn = Protocol;
	try
	{
		pTransport->open();
		log(Info,"[ThriftConnCreator::createConnect] thrift open!\n");
	}
	catch (apache::thrift::transport::TTransportException& e)
	{
		const apache::thrift::transport::TTransportException::TTransportExceptionType etype = e.getType();
		log( Info,"[ThriftConnCreator::createConnect]TTransportException : %s TTransportExceptionType = %u\n", e.what(),etype );
		if (etype != apache::thrift::transport::TTransportException::INTERRUPTED && etype != apache::thrift::transport::TTransportException::BAD_ARGS )
		{
			log( Error,"[ThriftConnCreator::createConnect] reset thrift connect\n" );
			conn.reset();
		}
	}
	catch (apache::thrift::TException& e)
	{
		log( Info,"[ThriftConnCreator::createConnect]TException : %s ", e.what());
		conn.reset();
	}
	catch (...)
	{
		log(Error,"[ThriftConnCreator::createConnect] Unknow Exception\n");
		conn.reset();
	}
	return conn;
}


void ThriftConnCreator::destroy(ThriftConn conn){
	log(Info,"[ThriftConnCreator::destroy] \n");
	conn.reset();
}

