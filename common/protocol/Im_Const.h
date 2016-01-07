#ifndef  __IM_CONST_H_INCLUDE__
#define  __IM_CONST_H_INCLUDE__

#define INVALID_IMLINKD    0xffffffff
#define IM_INVALID_AREAID  0xffffffff

// ��ǰ�ˣ�ǰ�˵ı�� �� Ӧ�ñ�ʶ
enum EImFrontEndNo
{
	ENUM_FRONT_NONE = -1,				//

	ENUM_FRONT_TCP = 1,                 // Ĭ��ǰ��
	ENUM_FRONT_UDP = 2, 			    // ��tcpǰ����һ���UDP
	ENUM_FRONT_MUL_UDP1 = 3,            // ��һ·udpǰ��
	ENUM_FRONT_MUL_UDP2	= 4,			// �ڶ�·udpǰ��
	ENUM_FRONT_MULTI_TCP_UDP1,          // ��һ·�͵���·���

	// �����ΪӦ�õı�ʶ�����ڱ������Ӧ�õ�����״̬����ǰ�˱�ʶ

	ENUM_FRONT_MAKE_FRIEND = 17         // app status 
};

// app id

enum EImAppId
{
	ENUM_APP_MAKE_FRIEND = 17           // ����
};

// ��ǰ��ת�����ĵ���Դ
enum EImMultForwardFrom
{
	ENUM_MULT_FORWARD_FROM_CLIENT = 0,   // client require to forward
	ENUM_MULT_FORWARD_FROM_SRV           // server require to forward
};

// yy�ͻ�����Դ
enum EClientType
{
	ENUM_CLIENT_PC = 0,
	ENUM_CLIENT_MOBILE
};

namespace protocol
{
	namespace im
	{
		// �жϿͻ��˰汾��Ϣ
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
