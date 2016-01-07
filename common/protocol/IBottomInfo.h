#pragma once

#include "PGameInfoType.h"

namespace protocol
{
	namespace imgame
	{
		class IBottomInfoWatcher
		{
		public:
			// 通知UI服务器返回的底部信息
			// std::map<uint32_t,std::string> : 底部信息
			//		key		:	数据类型。"0" => 游戏好友
			//		value	:	XML格式的字符串
			virtual void OnGetBottomInfoRes(const std::map<EBottomInfoType,std::string>& mapButtomInfo) = 0;	

			// Callback to GetPluginInfo. 通知UI, 开启用户好友推荐插件提示界面， 或是获取游戏好友推荐的bottomInfo
			virtual void OnHintUser(const EGameName& gameName, const EGameBuddyConfig& userConfig){};

			// UI设置用户游戏插件配置，服务器返回响应后，通知UI
			virtual void OnSetGameBuddyConfigRes(const EGameName& gameName, const EGameBuddyConfig& userConfig){};
		};

		class IOpBottomInfo
		{
		public:

			virtual void watch(IBottomInfoWatcher *w) = 0;

			virtual void revoke(IBottomInfoWatcher *w) = 0;
		};

		class IBottomInfo : public virtual IOpBottomInfo
		{
		public:

			virtual ~IBottomInfo() {}

			// Start game info collector.
			// must be called before any other game related interfaces 
			//virtual void StartGameInfoColletor() = 0;

			// Get Plug-in,called when user successfully login imlinkd!
			virtual void GetPluginInfo(const EGameName gameName) = 0;

			// Get bottom info from server
			virtual void GetBottomInfo(const EBottomInfoType infoType) = 0;

			// Set user config
			virtual void SetGameBuddyConfig(const EGameName gameName, const EGameBuddyConfig userConfig) = 0;
		};		
	}
}
