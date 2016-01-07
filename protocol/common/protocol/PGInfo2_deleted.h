#pragma once 

#include "common/packet.h" //sox::Marshallable
#include "common/core/base_svid.h"
#include "common/iproperty.h"
#include "common/res_code.h"

//#include "protocol/PGInfoCommonTypes.h" //Shared definitions for server/protocol/UI

namespace protocol
{
	namespace ginfo2
	{

		// The common fields for all GInfo messages
		struct CGInfoBaseMsg : public virtual sox::Marshallable
		{
			uint32_t m_uGrpID; // Group ID on which the operation is applied
			//uint32_t m_uSenderID; // UID of the operation requester
			//uint32_t m_uErrCode;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpID; // << m_uErrCode;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpID; // >> m_uErrCode;
			}

		};

		// UserA invites userB to join the group. The request is sent to ginfo
		// for authentication first. Then ginfo fills the checksum and forwards
		// the request to UserB. If userB accepts, PCS_InvitedToGroup will be
		// sent.
		struct PCS_InviteUserToGroup : public CGInfoBaseMsg
		{
			enum {uri = (35 << 8 | protocol::GINFO_SVID)};

			// Folder admin can also invite. But folderID where the admin belongs
			//  should be provided
			uint32_t m_uFldID;
			uint32_t m_uInviteeUID;
			uint32_t m_uInviterUID; // will be filled by ginfo with the requester's UID
			uint32_t m_uChecksum; // will be filled by ginfo
			std::string m_strInvitationMsg;
			std::string m_strExtMsg;
			uint32_t m_uType;
			//time_t m_objTime;

			virtual void marshal(sox::Pack &p) const
			{
				CGInfoBaseMsg::marshal(p);
				p << m_uFldID << m_uInviteeUID << m_uInviterUID
					<< m_uChecksum << m_strInvitationMsg << m_strExtMsg << m_uType;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CGInfoBaseMsg::unmarshal(p);
				p >> m_uFldID >> m_uInviteeUID >> m_uInviterUID
					>> m_uChecksum >> m_strInvitationMsg >> m_strExtMsg >> m_uType;
			}
		};
	}
}
