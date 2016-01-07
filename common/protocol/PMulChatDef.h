#pragma once 

#include <string>

namespace protocol
{
	namespace im
	{
		//多人会话错误码

		enum EMULCHAT_ERRCODE
		{
			ENUM_MULCHAT_SUCCESS = 0,
			ENUM_MULCHAT_MEMBER_LESS,							//会话的人数不够
			ENUM_MULCHAT_MEMBER_LIMIT,							//超出了规定的人数限制（限于创建和加人时）
			ENUM_MULCHAT_INVITE_ONLINE_EMPTY					//邀请的在线用户为空
		};

		//多人会话用户项

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
