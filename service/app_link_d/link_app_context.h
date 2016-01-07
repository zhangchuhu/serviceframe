#ifndef __LINK_APP_CONTEXT_H__
#define __LINK_APP_CONTEXT_H__

#include "core/corelib/MfcAppContext.h"
#include "server_common/protocol/prouter.h"
#include "common/core/form.h"
#include "iuid_dispatcher.h"
#include "common/core/iserver.h"

namespace server{
	namespace applinkd{
		class LinkAppContext : public core::PHClass, 
			public core::MfcAppcontext, 
			public IUidDispatcherAware, 
			public core::IServerIdDispatcherAware,
			public core::IServerAware
		{
			std::map<int, std::string> mm;
		public:

			DECLARE_FORM_MAP

			LinkAppContext();

			virtual void DefaultDispatch(core::Request &request, core::IConn *conn);

			void routeU(server::router::PUidTarget *ut);

		};

	}
}

#endif /*LINKAPPCONTEXT_H_*/

