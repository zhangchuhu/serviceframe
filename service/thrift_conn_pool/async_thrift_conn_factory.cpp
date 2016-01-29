#include "async_thrift_conn_factory.h"

#include "core/sox/logger.h"
#include <boost/thread/tss.hpp>

#define RECONNECT_TIME 30 * 1000

using namespace server::config;
using namespace server::thrift;


boost::thread_specific_ptr<ConnectionMap> ptr;

ConnectionMap::~ConnectionMap(){
	//memory leak
	/*for(connect_map_t::iterator it = connects.begin(); it != connects.end(); ++it){
		delete it->second;
	}*/
}

ThriftConn ConnectionMap::getConnection(const server::config::ZkThriftConfig &param, IConnectionCreator *creator){
	for(connect_map_t::iterator it = connects.begin(); it != connects.end(); ++it){
		if(it->first == param){
			return it->second;
		}
	}

	ThriftConn c = creator->createConnect(param);
	if(c.get()!=NULL)
		connects.push_back(std::make_pair(param, c));
	return c;
}


void ConnectionMap::releaseConnection(ThriftConn conn, IConnectionCreator *destoryer){
	connect_map_t tmp;
	for(connect_map_t::iterator it = connects.begin(); it != connects.end(); ++it){
		if(it->second != conn){
			tmp.push_back(*it);
		}
	}

	if(tmp.size() == connects.size()){
	//log error  info
	}

	destoryer->destroy(conn);

	connects = tmp;
}




bool AsyncThriftConnFactory::isBad(const server::config::ZkThriftConfig  &host){
	if(badHosts.empty()){
		return false;
	}else{
		return std::find(badHosts.begin(), badHosts.end(), host) != badHosts.end();
	}
}


ThriftConn AsyncThriftConnFactory::getConnection(const server::config::ZkThriftConfig &name) {

	ThriftConn c;
	if(isBad(name)){
		return c;
	}

	ConnectionMap *cm = NULL;
	if (ptr.get() == NULL){
		cm = new ConnectionMap();
		ptr.reset(cm);
	}else{
		cm = (ConnectionMap *)ptr.get();
	}

	c = cm->getConnection(name, connCreator);
	if(!c.get()){
		connectBroken(name);
	}
	return c;
}

void AsyncThriftConnFactory::releaseConnection(ThriftConn conn) {
	scoped_lock lock(m_mutex);
	ConnectionMap *cm = (ConnectionMap *)ptr.get();
	if(cm){
		cm->releaseConnection(conn, connCreator);
	}else{
		//错误
	}
}

void AsyncThriftConnFactory::connectBroken(const server::config::ZkThriftConfig &host){
	std::vector<ZkThriftConfig>::iterator it = std::find(badHosts.begin(), badHosts.end(), host);
	if(it == badHosts.end()){
		badHosts.push_back(host);
	}

	if(badHosts.size() == 1){
		select_timeout(RECONNECT_TIME);
	}
}
//if timeout not in main thread XXX危险
void AsyncThriftConnFactory::handle(int sig){
	scoped_try_lock lock(m_mutex);

  if (lock)
  {
  	if(sig == sox::SEL_TIMEOUT){
  		ConnectionMap *cm = NULL;
  		if (ptr.get() == NULL){
  			cm = new ConnectionMap();
  			ptr.reset(cm);
  		}else{
  			cm = (ConnectionMap *)ptr.get();
  		}

  		std::vector<ZkThriftConfig> failed;

  		for(std::vector<ZkThriftConfig>::iterator it = badHosts.begin(); it != badHosts.end(); ++it){
  			ThriftConn conn = cm->getConnection(*it, connCreator);
  			if(!conn.get()){
  				failed.push_back(*it);
  			}else{
  				log(Info, "reconnect to host:%s success", (*it).host.data());
  			}
  		}

  		badHosts = failed;

  		if(!badHosts.empty())
  			select_timeout(RECONNECT_TIME);
  	}
  }
  else
  {
    //failed to get mutex, wait till next timeout.
    log(Error, "[AsyncSqlConnFactory::handle], failed to get mutex, wait!!!");
    select_timeout(RECONNECT_TIME);
  }
}

ThriftConn AsyncThriftConnFactory::getConnectionByName(const std::string &name){
	scoped_lock lock(m_mutex);
	ThriftConn ret;
	if(name.empty())
	{
		ret = getConnection();
	}
	else
	{
		name_hosts_t::iterator it = names.find(name);
		if(it != names.end() && !it->second.first.empty())
		{
			std::vector<ZkThriftConfig> &hosts = it->second.first;

			size_t sz = hosts.size();

			uint32_t &inc = it->second.second;
			uint32_t old = inc;
			
			do
			{
				if(++inc >= sz)
				{
					inc = 0;
				}
				ret = getConnection(hosts[inc]);
			}while(ret.get() == NULL && inc != old);
			return ret;
		}
		else
		{
			log(Warn, "no name %s host", name.data());
		}
	}
	return ret;
}

bool AsyncThriftConnFactory::recoverConnection(ThriftConn ptr, const std::string &cname){
	if(ptr.get()){
		releaseConnection(ptr);
		ptr = getConnectionByName(cname);
		if(ptr.get()){
				return true;
		}else{
			log(Warn, "recover get connection null");
		}
	}else{
		log(Info, "escape recover empty connect");
	}
	return false;
}


void AsyncThriftConnFactory::setConnectionCreator(IConnectionCreator *c){
	scoped_lock lock(m_mutex);
	connCreator = c;
}
