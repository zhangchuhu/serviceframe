#ifndef __PIMBDOOR_H___
#define __PIMBDOOR_H___

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"
#include <string>
#include <vector>

using namespace std;

namespace server{
	namespace imbdoor{

		struct PSS_ImBDoor : public sox::Marshallable {
			enum {uri = (1 << 8) | protocol::IMBDOOR_SVID};

			uint32_t m_uConnId;
			uint32_t m_uUid;
			string   m_string;

			virtual void marshal(sox::Pack &pk) const {
				pk << m_uConnId<< m_uUid<<m_string;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> m_uConnId>> m_uUid>>m_string;
			}
		};

		struct PSS_ImBDoorRes : public sox::Marshallable {
			enum {uri = (2 << 8) | protocol::IMBDOOR_SVID};

			uint32_t m_uConnId;
			uint32_t m_uUid;
			string   m_string;

			virtual void marshal(sox::Pack &pk) const {
				pk << m_uConnId<< m_uUid<<m_string;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> m_uConnId>> m_uUid>>m_string;
			}
		};

		struct PSS_ImBDoorSendMsgToUsers : public sox::Marshallable {
			enum {uri = (3 << 8) | protocol::IMBDOOR_SVID};

			uint32_t m_uFromUid;
			vector<uint32_t> m_vToUids;  
			string   m_strMessage;

			virtual void marshal(sox::Pack &pk) const {
				pk << m_uFromUid;
				marshal_container(pk, m_vToUids);
				pk<<m_strMessage;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> m_uFromUid;
				sox::unmarshal_container(up, std::back_inserter(m_vToUids));
				up>>m_strMessage;
			}
		};

	}
}

#endif
