#ifndef THRIFT_CONN_CREATOR_H_
#define THRIFT_CONN_CREATOR_H_


#include <string>

#include <boost/thread/thread.hpp>
#include <protocol/TBinaryProtocol.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>

#include "thrift_conn_factory.h"
#include "core/sox/logger.h"


namespace server {  namespace thrift{


class ThriftConnCreator: public IConnectionCreator{

protected:

	typedef boost::mutex::scoped_lock scoped_lock;

	mutable boost::mutex m_mutex;

public:
	virtual ThriftConn createConnect(const server::config::ZkThriftConfig &param);

	virtual void logError(ThriftConn );

	virtual void destroy(ThriftConn );
};
	}
}

#endif
