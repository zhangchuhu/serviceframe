#ifndef THRIFT_CONN_FACTORY_H_
#define THRIFT_CONN_FACTORY_H_
#include <string>

#include <boost/thread/thread.hpp>
#include <protocol/TBinaryProtocol.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>


#include "zk_thrift_config.h"


namespace server {  namespace thrift{

typedef boost::shared_ptr<apache::thrift::protocol::TProtocol> ThriftConn;

class ThriftConnFactory{

	public:
		virtual ~ThriftConnFactory(){}

		//virtual mysqlpp::Connection *getConnection(const std::string &host) = 0;

		virtual ThriftConn getConnectionByName(const std::string &name) = 0;

		virtual bool recoverConnection(ThriftConn ptr, const std::string &cname) = 0;
		virtual void releaseConnection(ThriftConn conn) = 0;
		};

class ThriftConnFactoryAware{
	ThriftConnFactory *factory;
public:
	ThriftConnFactory *getThriftConnFactory(){
		return factory;
	}

	void setThriftConnFactory(ThriftConnFactory *factory){
		this->factory = factory;
	}
};

struct IConnectionCreator{
	virtual ~IConnectionCreator(){}
	virtual ThriftConn createConnect(const server::config::ZkThriftConfig &param) = 0;
	virtual void logError(ThriftConn ) = 0;
	virtual void destroy(ThriftConn ) = 0;
};

}}

#endif 
