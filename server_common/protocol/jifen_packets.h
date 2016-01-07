/************************************************************************/
/* Description:                                                         */
/*			Jifen operation protocols                                   */
/*                                                                      */ 
/* author: ZhangMin(zhangmin@chinduo.com)                               */ 
/************************************************************************/
#ifndef JIFEN_PACKETS_H_
#define JIFEN_PACKETS_H_

#include "../../common/packet.h"
#include "../../common/core/base_svid.h"

#include <vector>

using namespace std;
using namespace sox;
//using namespace protocol::svcCommon;

namespace protocol
{
	namespace service
	{
		struct JiFen : public sox::Marshallable
		{
			uint32_t  m_uUid;
			uint32_t  m_uUserJiFen;
			uint32_t  m_sMemberJiFen;

			virtual void marshal(Pack & pk) const {
				pk << m_uUid << m_uUserJiFen << m_sMemberJiFen;
			}

			virtual void unmarshal(const Unpack &up) {
				up >> m_uUid >> m_uUserJiFen >> m_sMemberJiFen;
			}
		};

		// Client protocol
		struct PCS_GetJifenByUidsReq : public sox::Marshallable
		{
			enum {uri = (32 << 8 | protocol::SERVICE_SVID)};
			uint32_t uid;
			uint32_t sid;
			vector<uint32_t> uids;
			virtual void marshal(Pack &pk) const {
				pk << uid<<sid;
				marshal_container(pk, uids);
			}
			virtual void unmarshal(const Unpack &up) {
				up >> uid>>sid;
				unmarshal_container(up, std::back_inserter(uids));
			}
		};

		struct PCS_GetJifenByUidsRes : public sox::Marshallable {
			enum {uri = (33 << 8 | protocol::SERVICE_SVID)};
			std::vector<JiFen> data;
			virtual void marshal(sox::Pack &pk) const {
				marshal_container(pk, data);
			}
			virtual void unmarshal(const sox::Unpack &up) {
				unmarshal_container(up, std::back_inserter(data));
			}
		};

		struct PCS_UpdateJifenReq : public sox::Marshallable
		{
			enum {uri = (34 << 8 | protocol::SERVICE_SVID)};
			uint32_t uid;
			uint32_t sid;

			virtual void marshal(sox::Pack & pk) const {
				pk << uid << sid;
			}

			virtual void unmarshal(const Unpack & up) {
				up >> uid >> sid;
			}
		};

		struct PCS_UpdateJifenRes : public sox::Marshallable
		{
			enum {uri = (35 << 8 | protocol::SERVICE_SVID)};

			std::vector<JiFen> data;
			virtual void marshal(sox::Pack &pk) const {
				marshal_container(pk, data);
			}

			virtual void unmarshal(const Unpack &up) {
				unmarshal_container(up, std::back_inserter(data));
			}
		};

		//server protocol
		struct PSS_GetJifenFromSdbReq : public sox::Marshallable
		{
			enum {uri = (36 << 8 | protocol::SERVICE_SVID)};

			string transId;
			uint32_t sid;
			std::vector<uint32_t> uids;

			virtual void marshal(sox::Pack &pk) const {
				pk << transId << sid;
				marshal_container(pk, uids);
			}

			virtual void unmarshal(const Unpack & up) {
				up >> transId >> sid;
				unmarshal_container(up, std::back_inserter(uids));
			}
		};

		struct PSS_GetJifenFromSdbRes : public sox::Marshallable
		{
			enum {uri = (37 << 8 | protocol::SERVICE_SVID)};

			string transId;
			uint32_t sid;
			std::vector<JiFen> data;

			virtual void marshal(sox::Pack &pk) const {
				pk << transId << sid;
				marshal_container(pk, data);
			}

			virtual void unmarshal(const Unpack & up) {
				up >> transId >> sid;
				unmarshal_container(up, std::back_inserter(data));
			}
		};

		struct PCS_BrocastJifen : public PCS_GetJifenByUidsRes
		{
			enum {uri = (38 << 8 | protocol::SERVICE_SVID)};
		};
	}
}
#endif /*JIFEN_PACKETS_H_*/
