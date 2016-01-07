#pragma once 

#include "common/unistr.h"
#include "common/int_types.h"

namespace protocol
{
	namespace ginfo2
	{

		enum EInvitationType {
			ENUM_INV_TO_GROUP,//0
			ENUM_INV_TO_FOLDER,//1
			ENUM_INV_CODE_TO_GROUP,//2
			ENUM_INV_CODE_TO_FOLDER,//3
			ENUM_INV_UNKNOWN //Abnormal value
		};

		struct IGInfoGeneralWatcher
		{
			virtual ~IGInfoGeneralWatcher() {};

			// Invitee received an invitation
			virtual void OnInvitedToGroupRequest(const uint32_t& uGrpID, const uint32_t& uFldID,
				const uint32_t& uInviterUID, const UniString& strInvitation, const UniString& strExt,
				const uint32_t& uChecksum,
				const protocol::ginfo2::EInvitationType& enType) = 0;
		};

		struct IGInfoGeneralWatcherBase
		{
			virtual ~IGInfoGeneralWatcherBase() {};

			virtual void watch(IGInfoGeneralWatcher *w) = 0;
			virtual void revoke(IGInfoGeneralWatcher *w) = 0;
		};

		class IGInfoManager : public virtual IGInfoGeneralWatcherBase
		{
		public:
			virtual ~IGInfoManager() {};
		};
	}
}
