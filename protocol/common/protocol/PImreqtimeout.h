#pragma once

#include <vector>
#include <string>
#include <map>
#include "iproperty.h"
#include "protocol/const.h"
#include "core/base_svid.h"
#include "common/packet.h"
#include "protocol/PImreqtimeoutType.h"

namespace protocol
{
	namespace imreqtimeout
	{
		struct PCS_ImFailureReport : public sox::Marshallable
		{
			enum {uri = (101 << 8) | protocol::IMSTAT_SVID};

			uint32_t m_uReportTime;
			std::vector<SFailItem> m_vecReport;

			PCS_ImFailureReport(){}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uReportTime;
				sox::marshal_container(p, m_vecReport);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uReportTime;
				sox::unmarshal_container(p, std::back_inserter(m_vecReport));
			}
		};
	}
}


