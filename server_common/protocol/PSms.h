#ifndef PSMS_H_20110809
#define PSMS_H_20110809

#include "core/base_svid.h"
#include "common/res_code.h"
#include "common/packet.h"

namespace server
{
	namespace sms
	{
		enum
		{
			IM = 1,
			MUSIC_APP1,
			MUSIC_APP2,
			IM_DISMISS,
			MUSIC_APP3,
			SHORT_CHID,
			MUSIC_APP4,
			MUSIC_APP5
		};
		enum 
		{
			SMS_OK = 1,
			SMS_FAIL = -1,
			SMS_TOO_OFTEN = -2,
			SMS_AUTH_FAIL = -3,
			SMS_AUTH_TO = -4,
			SMS_APPID_FAIL = -5,
			SMS_MOBILE_FAIL = -6,
			SMS_NO_RECORD = -7
		};

		struct PQueryMobile : public sox::Marshallable 
		{
			enum {uri = (31 << 8) | protocol::UDB_SVID};

			PQueryMobile():uid(-1), flag(0){}

			uint32_t uid;
			std::string passport;
			uint32_t flag; // flag = 1 means uid, 2 means passport, 3 means all
			std::string context;
			std::string clientIp;
			std::string serverIp;

			virtual void marshal(sox::Pack &pk) const
			{
				pk << uid << passport << flag << context << clientIp << serverIp;
			}
			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> uid >> passport >> flag >> context >> clientIp >> serverIp;
			}
		};

		struct PQueryMobileRes : public sox::Marshallable 
		{
			enum {uri = (32 << 8) | protocol::UDB_SVID};

			PQueryMobileRes():uid(-1), retvalue(0){}

			uint32_t uid;
			std::string passport;
			uint32_t retvalue;
			std::string context;

			virtual void marshal(sox::Pack &pk) const
			{
				pk << uid << passport << retvalue << context;
			}
			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> uid >> passport >> retvalue >> context;
			}
		};

		struct PRequestSendSMS : public sox::Marshallable
		{
			enum {uri = (33 << 8) | protocol::UDB_SVID};

			PRequestSendSMS():uid(-1), flag(0), appid(-1){}

			uint32_t uid;
			std::string passport;
			uint32_t flag; // flag = 1 means uid, 2 means passport, 3 means all
			uint32_t appid;
			std::string context;
			std::string clientIp;
			std::string serverIp;

			virtual void marshal(sox::Pack &pk) const
			{
				pk << uid << passport << flag << appid << context << clientIp << serverIp;
			}
			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> uid >> passport >> flag >> appid >> context >> clientIp >> serverIp;
			}
		};

		struct PRequestSendSMSRes : public sox::Marshallable
		{
			enum {uri = (34 << 8) | protocol::UDB_SVID};

			PRequestSendSMSRes():uid(-1), retvalue(0){}

			uint32_t uid;
			std::string passport;
			uint32_t retvalue;
			std::string context;

			virtual void marshal(sox::Pack &pk) const
			{
				pk << uid << passport << retvalue << context;
			}
			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> uid >> passport >> retvalue >> context;
			}
		};

		struct PCheckAuthCode : public sox::Marshallable
		{
			enum {uri = (35 << 8) | protocol::UDB_SVID};

			PCheckAuthCode():uid(-1), flag(0){}

			uint32_t uid;
			std::string passport;
			uint32_t flag; // flag = 1 means uid, 2 means passport, 3 means all
			uint32_t appid;
			std::string authCode;
			std::string context;
			std::string clientIp;
			std::string serverIp;

			virtual void marshal(sox::Pack &pk) const
			{
				pk << uid << passport << flag << appid << authCode << context << clientIp << serverIp;
			}
			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> uid >> passport >> flag >> appid >> authCode >> context >> clientIp >> serverIp;
			}
		};

		struct PCheckAuthCodeRes : public sox::Marshallable 
		{
			enum {uri = (36 << 8) | protocol::UDB_SVID};

			PCheckAuthCodeRes():uid(-1), retvalue(0){}

			uint32_t uid;
			std::string passport;
			uint32_t retvalue;
			std::string context;

			virtual void marshal(sox::Pack &pk) const
			{
				pk << uid << passport << retvalue << context;
			}
			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> uid >> passport >> retvalue >> context;
			}
		};

	}
}

#endif
