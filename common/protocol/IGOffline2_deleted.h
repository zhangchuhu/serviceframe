#pragma once 

#include "common/unistr.h"
#include "common/int_types.h"

namespace protocol
{
	namespace goffline2
	{

		struct IGOfflineWatcher
		{
			virtual ~IGOfflineWatcher() {};
		};


		struct IGOfflineWatcherBase
		{
			virtual ~IGOfflineWatcherBase() {};

			virtual void watch(IGOfflineWatcher *w) = 0;
			virtual void revoke(IGOfflineWatcher *w) = 0;
		};

		class IGOffline : public virtual IGOfflineWatcherBase
		{
		public:
			virtual ~IGOffline() {};

			virtual void GetAction() = 0;
		};
	}
}
