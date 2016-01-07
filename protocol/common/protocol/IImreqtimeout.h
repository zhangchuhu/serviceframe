#ifndef IIMREQ_TIMEOUT_H
#define IIMREQ_TIMEOUT_H

#include <string>
#include "int_types.h"

namespace protocol
{
	namespace imreqtimeout
	{
		class IImReqTimeoutWatcher
		{
		public:
			virtual void OnTimeout(const uint32_t& uUri, const std::string& strData) = 0;
		};

		class IOpImReqTimeout
		{
		public:

			virtual void watch(IImReqTimeoutWatcher *w) = 0;

			virtual void revoke(IImReqTimeoutWatcher *w) = 0;
		};

		class IImReqTimeout : public virtual IOpImReqTimeout
		{
		};		
	}
}

#endif
