#ifndef __ZK_CONFIG_MANAGER_H__
#define __ZK_CONFIG_MANAGER_H__

#include <string>
#include <set>

#include "zk_config_item.h"
#include "../vip_lib/no_block_thrift_client.h"


namespace libzk { 
	class ZkConfigItem;

	// zh->context is a pointer to a WatcherAction instance
	// based on the event type and state, the watcher calls a specific watcher 
	// action method


	class ZkConfigManager: public ZkClientAware{
	public:

		ZkConfigManager(const std::string& proc_name );

		~ZkConfigManager();

	public:

		//! 初始化关注的配置名称能调用一次
		bool Init( ) ;

		//! 注册关注的配置
		void RegisterConfig( const std::string& type,const std::string& name );

		//! 根据配置名称获取配置信息指针，用户不要hold该指针
		ZkConfigItem* GetConfigItem( const std::string& key )  ;

		//! 根据配置名称获取配置的ip
		std::string GetConfigIp( const std::string& key );

		//! 根据配置名称获取配置的端口
		int32_t GetConfigPort( const std::string& key );
	public:
		//! 根据配置名称获取thrift client
		template<typename T>
		bool GetThriftClient( const std::string& key ,::vip::common::NoBlockingThriftClient<T>* client);

	private:
		std::string proc_name;
		std::set<std::string> care_list; // 关注的配置列表
		std::map<std::string,ZkConfigItem*> config_list_; // 获取成功的配置

	};
	template<typename T>
	bool ConfigManager::GetThriftClient( const std::string& key ,::vip::common::NoBlockingThriftClient<T>* client) {

		if ( GetConfigType( key ) == kTypeThrift ) {

			std::string ip = GetConfigIp( key );

			int32_t port = GetConfigPort( key ) ;

			if ( !ip.empty() && port != 0 ) {

				client->initIpAndPort( ip,port);

				return true;
			}
		}

		return false;
	}
}
#endif