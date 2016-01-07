#ifndef _MEMCACHED_CONFIG_IMP2_H_
#define _MEMCACHED_CONFIG_IMP2_H_

#include <string>
#include <map>
#include <vector>
#include "common/int_types.h"
#include "server_common/config/IConfigLoader.h"
#include "protocol/common/watch_helper.h"
#include "MemcachedConfigImp.h"

namespace server{
	namespace config{
		//struct memcachedParam{
		//	std::string ip;
		//	uint16_t port;
		//};

		//struct IMemcachedWatcher{
		//	virtual ~IMemcachedWatcher(){}
		//	virtual void onMemcachedConfRefreshed() = 0;
		//};

		//struct IMemcachedWatcherBase{
		//	virtual ~IMemcachedWatcherBase(){}
		//	void virtual watch(IMemcachedWatcher *w) = 0;
		//	void virtual revoke(IMemcachedWatcher *w) = 0;
		//};

		//struct IMemcachedConfig : public virtual IMemcachedWatcherBase{
		//	virtual ~IMemcachedConfig(){}
		//	void virtual getMemcachedInfo(std::map<std::string, std::vector<memcachedParam> >&) = 0;
		//	void virtual getMemcachedInfoByName(const std::string& name, std::vector<memcachedParam>&) = 0;
		//};


		class MemcachedConfigImp2 :  public IConfigLoader,
			public IMemcachedConfig,
			public WatchHelper<IMemcachedWatcherBase, IMemcachedWatcher>{
		public:
			void SetPersonConf(const std::string &strConf)
			{
				m_strPersonConf = strConf;
			}
			void load(FILE *fp);

			void virtual getMemcachedInfo(std::map<std::string, std::vector<memcachedParam> >&);
			void virtual getMemcachedInfoByName(const std::string& name, std::vector<memcachedParam>&);

		private:
			typedef std::map<std::string, std::vector<memcachedParam> > t_memcached_map;
			t_memcached_map memcachedInfo;
			bool loaded;

			std::string m_strPersonConf;
		};
	}
}


#endif
