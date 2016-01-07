#pragma once

#include <string>
#include "common/packet.h"
#include "common/protocol/const.h"
#include "core/base_svid.h"
#include "common/protocol/IGSearchRecord.h"

using namespace std;

namespace protocol
{
	namespace gsearch
	{
		// packet to get group by group id.
		struct PCS_GetGrpById: public sox::Marshallable
		{
			enum { uri = (1 << 8) | GSEARCH_SVID};

			uint32_t m_uGID;
			virtual void marshal(sox::Pack &p) const {
				p << m_uGID;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> m_uGID;
			}
		};

		// packet to response for getting group by group id.
		struct PCS_GetGrpByIdRes: public sox::Marshallable
		{
			enum { uri = (1 << 8) | GSEARCH_SVID};

			uint32_t m_uResCode;
			protocol::gsearch::CGSearchRecordData m_objGrpRec;
			virtual void marshal(sox::Pack &p) const {
				p << m_uResCode << m_objGrpRec;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> m_uResCode >> m_objGrpRec;
			}
		};

		// packet to get group by group name.
		struct PCS_GetGrpByName: public sox::Marshallable
		{
			enum { uri = (2 << 8) | GSEARCH_SVID};

			std::string m_strGName;
			virtual void marshal(sox::Pack &p) const {
				p << m_strGName;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> m_strGName;
			}
		};

		// packet to response for getting group by group name.
		struct PCS_GetGrpByNameRes: public sox::Marshallable
		{
			enum { uri = (2 << 8) | GSEARCH_SVID};

			uint32_t m_uResCode;
			std::vector<protocol::gsearch::CGSearchRecordData> m_vecGrpRecs;
			virtual void marshal(sox::Pack &p) const {
				p << m_uResCode;
				marshal_container(p, m_vecGrpRecs);
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> m_uResCode;
				unmarshal_containerEx(p, m_vecGrpRecs);
			}
		};

	}
}
