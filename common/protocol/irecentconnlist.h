#ifndef __IRECENTCONNLIST_H__
#define __IRECENTCONNLIST_H__

#include "common/protocol/ilist.h"
#include "common/protocol/const.h"

namespace protocol
{
	namespace im
	{
		struct IRecentConnList : public protocol::im::IImListBase
		{
			virtual ~IRecentConnList(){}
			void virtual delRecConnBuddy(const HULI &bid) = 0;
			void virtual delAllRecConnBuddy() = 0;
		};
	}
}

#endif