#pragma once 

#include <string>

namespace protocol
{
	namespace im
	{
		//���˻Ự������

		enum EMULCHAT_ERRCODE
		{
			ENUM_MULCHAT_SUCCESS = 0,
			ENUM_MULCHAT_MEMBER_LESS,							//�Ự����������
			ENUM_MULCHAT_MEMBER_LIMIT,							//�����˹涨���������ƣ����ڴ����ͼ���ʱ��
			ENUM_MULCHAT_INVITE_ONLINE_EMPTY					//����������û�Ϊ��
		};

		//���˻Ự�û���

		struct SWideUserItem
		{
			uint32_t m_uUid;
			std::wstring m_wstrNick;

			SWideUserItem() : m_uUid(0), m_wstrNick(L"") {};
			SWideUserItem(uint32_t uUid) : m_uUid(uUid), m_wstrNick(L"") {};
			SWideUserItem(uint32_t uUid, const std::wstring &wstrNick) : m_uUid(uUid), m_wstrNick(wstrNick) {};
		};

	}
}
