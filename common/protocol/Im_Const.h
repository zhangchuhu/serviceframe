#ifndef  __IM_CONST_H_INCLUDE__
#define  __IM_CONST_H_INCLUDE__

#define INVALID_IMLINKD    0xffffffff
#define IM_INVALID_AREAID  0xffffffff

// 多前端，前端的编号 或 应用标识
enum EImFrontEndNo
{
	ENUM_FRONT_NONE = -1,				//

	ENUM_FRONT_TCP = 1,                 // 默认前端
	ENUM_FRONT_UDP = 2, 			    // 和tcp前端在一起的UDP
	ENUM_FRONT_MUL_UDP1 = 3,            // 第一路udp前端
	ENUM_FRONT_MUL_UDP2	= 4,			// 第二路udp前端
	ENUM_FRONT_MULTI_TCP_UDP1,          // 第一路和第三路混合

	// 后面的为应用的标识，用于保存各个应用的在线状态，非前端标识

	ENUM_FRONT_MAKE_FRIEND = 17         // app status 
};

// app id

enum EImAppId
{
	ENUM_APP_MAKE_FRIEND = 17           // 交友
};

// 多前端转发报文的来源
enum EImMultForwardFrom
{
	ENUM_MULT_FORWARD_FROM_CLIENT = 0,   // client require to forward
	ENUM_MULT_FORWARD_FROM_SRV           // server require to forward
};

// yy客户端来源
enum EClientType
{
	ENUM_CLIENT_PC = 0,
	ENUM_CLIENT_MOBILE
};

namespace protocol
{
	namespace im
	{
		// 判断客户端版本信息
		class CClientVersionFunc
		{
		public:

			static void SetClientType(uint32_t& uVersion, EClientType emVer)
			{
				uVersion &= 0xffffff00;
				uVersion |= ((uint32_t)emVer) & 0xff;
			}

			static EClientType GetClientType(uint32_t uVersion)
			{
				return (EClientType)(uVersion & 0xff);
			}
		};
	}
}

#endif 
