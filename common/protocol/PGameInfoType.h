#ifndef PGAME_INFO_TYPES_H
#define PGAME_INFO_TYPES_H

namespace protocol
{
	namespace imgame
	{
		enum EBottomInfoType
		{
			ENUM_ALL = 0,		// ȫ����Ϣ
			ENUM_GAME_FRIEND,	// ��Ϸ������Ϣ
			ENUM_GAME_AD,		// ��Ϸ�����Ϣ
			ENUM_UNKNOWN_TYPE
		};

		enum EGameName
		{
			ENUM_WOW = 0,
			ENUM_UNKNOWN_GAME
		};

		enum EGameBuddyConfig
		{
			ENUM_WAIT = 0,			// �ȴ��û�ȷ��
			ENUM_OPEN,				// ������Ϸ�����Ƽ�����
			ENUM_CLOSE,				// �ر���Ϸ�����Ƽ�����
			ENUM_WITHOUT_GAME		// �û��ر�����Ϸ�����Ƽ����ܡ����ߴ�δȷ�Ϲ������ұ���δ��װ��Ϸ
		};
	}
}
#endif

