#ifndef PUDB_H_20100613
#define PUDB_H_20100613

#include "common/packet.h"
#include "common/core/base_svid.h"


namespace protocol
{
namespace udb
{
	// ע�����UDB���û�
	struct PRegisterDuowanUdb : public sox::Marshallable {
		enum {uri = (5 << 8) | protocol::UDB_SVID};

		std::string user_name;
		std::string passwd;
		std::string email;
		std::string refer;	// ע����Դ
		std::string client_mac;

		virtual void marshal(sox::Pack &pk) const {
			pk << user_name << passwd << email << refer << client_mac;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> user_name >> passwd >> email >> refer >> client_mac;
		}
	};

	struct PRegisterDuowanUdbResp : public sox::Marshallable {
		enum {uri = (7 << 8) | protocol::UDB_SVID};

		std::string user_name;
		uint16_t error_code;

		virtual void marshal(sox::Pack &pk) const {
			pk << user_name << error_code;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> user_name >> error_code;
		}
	};

}
}

#endif // PUDB_H_20100613
