#ifndef PGAME_INFO_TYPES_H
#define PGAME_INFO_TYPES_H

namespace protocol
{
	namespace imgame
	{
		enum EBottomInfoType
		{
			ENUM_ALL = 0,		// 全部信息
			ENUM_GAME_FRIEND,	// 游戏好友信息
			ENUM_GAME_AD,		// 游戏广告信息
			ENUM_UNKNOWN_TYPE
		};

		enum EGameName
		{
			ENUM_WOW = 0,
			ENUM_UNKNOWN_GAME
		};

		enum EGameBuddyConfig
		{
			ENUM_WAIT = 0,			// 等待用户确认
			ENUM_OPEN,				// 开启游戏好友推荐功能
			ENUM_CLOSE,				// 关闭游戏好友推荐功能
			ENUM_WITHOUT_GAME		// 用户关闭了游戏好友推荐功能、或者从未确认过，并且本地未安装游戏
		};
	}
}
#endif

