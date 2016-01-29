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

		//! ��ʼ����ע�����������ܵ���һ��
		bool Init( ) ;

		//! ע���ע������
		void RegisterConfig( const std::string& type,const std::string& name );

		//! �����������ƻ�ȡ������Ϣָ�룬�û���Ҫhold��ָ��
		ZkConfigItem* GetConfigItem( const std::string& key )  ;

		//! �����������ƻ�ȡ���õ�ip
		std::string GetConfigIp( const std::string& key );

		//! �����������ƻ�ȡ���õĶ˿�
		int32_t GetConfigPort( const std::string& key );
	public:
		//! �����������ƻ�ȡthrift client
		template<typename T>
		bool GetThriftClient( const std::string& key ,::vip::common::NoBlockingThriftClient<T>* client);

	private:
		std::string proc_name;
		std::set<std::string> care_list; // ��ע�������б�
		std::map<std::string,ZkConfigItem*> config_list_; // ��ȡ�ɹ�������

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