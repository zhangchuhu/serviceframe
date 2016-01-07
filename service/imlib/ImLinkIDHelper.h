#ifndef __ImLinkIdHelper_h__
#define  __ImLinkIdHelper_h__

#include <string.h>
#include <algorithm>
#include<map>
#include<vector>
#include"./ConsistentHashing.h"
#include"server_common/server-lib/iclient.h"

namespace server
{
	class ImLinkIDHelper
	{
	public:
		ImLinkIDHelper();

		//添加服务器
		bool addServer(uint32_t imlinkid);

		//删除服务器
		bool removeServer(uint32_t imlinkid);

		//从uid转换到imlinkid,如果imlinkid集合为空,返回 NONE_SERVER_ID
		uint32_t uid2Imlinkid(uint32_t uid);

		//imlinkid是否存在
		bool imlinkidExists(uint32_t imlinkid);

		//取全部imlink集合
		void getImLinkMapSet(std::vector<uint32_t> &vtImLinkSet);

		void clearServer();
	private:
		UserConsistentHash m_hash;
		std::vector<uint32_t> m_imLinkdSet;			//imlinkd的集合
	};

	class WatchImLinkDaemonClient : public core::IDaemonClientAwareW
	{
	public:
		//字面意思有点儿不对,应该是初始化时Daemon发出当前服务器列表
		virtual void onRefreshSV();

		virtual void onRecoverRefreshSV();
		
		//服务器增加
		virtual void onAddSV(const sdaemon::FavorStrSV & fs);		

		//服务器删减
		virtual void onServerRemovedSV(const sdaemon::FavorStrSV & fs);

		virtual void onServerStatusChangeSV(const sdaemon::FavorStrSV & fs);

		//Imlinkd发生变化
		virtual void onLinkIDSetChanged(){}
	protected:
		ImLinkIDHelper m_imlinkIdHelper;
	};
}

#endif		//__ImLinkIdHelper_h__
