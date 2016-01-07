////
//  PluginInfo - structure that is used by both client and protocol.
//
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __PluginInfo_h__
#define __PluginInfo_h__

#include <vector>
#include <string>

namespace yyplugin
{
	namespace appMgr
	{
		struct VersionInfo
		{
			unsigned int version;
			unsigned int yyMin;
			unsigned int yyMax;
			std::string md5;
			std::wstring url;
		};  //VersionInfo

		struct PluginInfo
		{
			unsigned int appID;
			std::wstring appFileName;	//music.dll
			std::wstring appDescription;	//����Ӧ��
			unsigned int recommendedAppVersion; //�Ƽ�ʹ�õĲ���汾
			std::vector<VersionInfo> versionSet;
			bool opByOw; // if true, a message need to be displayed.
		}; //PluginInfo
	}	//appMgr
}	//yyplugin

#endif // __PluginInfo_h__