#ifndef ASYNC_THRIFT_CONN_FACTORY_H_
#define ASYNC_THRIFT_CONN_FACTORY_H_
#include <string>
#include "core/sox/snox.h"
#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include "thrift_conn_factory.h"


namespace server {  namespace thrift{


	struct ConnectionMap{
		~ConnectionMap();

		typedef std::vector<std::pair<server::config::ZkThriftConfig,ThriftConn> > connect_map_t;
		connect_map_t connects;

		ThriftConn getConnection(const server::config::ZkThriftConfig &param, IConnectionCreator *);
		void releaseConnection(ThriftConn conn, IConnectionCreator *);
	};

	class AsyncThriftConnFactory : public ThriftConnFactory, public sox::Handler
	{
		typedef boost::recursive_try_mutex::scoped_lock scoped_lock;      
		typedef boost::recursive_try_mutex::scoped_try_lock scoped_try_lock;            
		mutable boost::recursive_try_mutex m_mutex;

		std::vector<server::config::ZkThriftConfig> badHosts;

		typedef std::map<std::string, std::pair<std::vector<server::config::ZkThriftConfig>, uint32_t> > name_hosts_t;
		name_hosts_t names;

		std::string writeName;
		std::string readName;
		IConnectionCreator *connCreator;

		int oldDefaultDbId;
		server::config::ZkThriftConfig *config;
	public:
		AsyncThriftConnFactory();

		virtual  ThriftConn getConnection();
		virtual ThriftConn getConnectionByName(const std::string &name);
		//			virtual mysqlpp::Connection *getConnectionByNameDef();

		virtual bool recoverConnection(ThriftConn ptr, const std::string &cname);
		virtual void releaseConnection(ThriftConn conn);

		//mysqlpp::Connection *createConnect(const &host);

		void handle(int sig);

		void connectBroken(const server::config::ZkThriftConfig &host);

		void setDefaultHost(const server::config::ZkThriftConfig &name);


		void setConnectionCreator(IConnectionCreator *c);

	private:
		ThriftConn getConnection(const server::config::ZkThriftConfig &param);
		bool isBad(const server::config::ZkThriftConfig &host);
	};
}}

#endif /*ISQLCONNFACTORY_H_*/
