#pragma once

#include "PGameInfoType.h"

namespace protocol
{
	namespace imgame
	{
		class IBottomInfoWatcher
		{
		public:
			// ֪ͨUI���������صĵײ���Ϣ
			// std::map<uint32_t,std::string> : �ײ���Ϣ
			//		key		:	�������͡�"0" => ��Ϸ����
			//		value	:	XML��ʽ���ַ���
			virtual void OnGetBottomInfoRes(const std::map<EBottomInfoType,std::string>& mapButtomInfo) = 0;	

			// Callback to GetPluginInfo. ֪ͨUI, �����û������Ƽ������ʾ���棬 ���ǻ�ȡ��Ϸ�����Ƽ���bottomInfo
			virtual void OnHintUser(const EGameName& gameName, const EGameBuddyConfig& userConfig){};

			// UI�����û���Ϸ������ã�������������Ӧ��֪ͨUI
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
