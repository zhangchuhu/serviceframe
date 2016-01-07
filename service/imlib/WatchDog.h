#pragma once
#include "core/corelib/MfcAppContext.h"
#include "server_common/protocol/WatchDog.h"
#include "server_common/server-lib/RouteTarget.h"

namespace server
{
    class CWatchDogAppContext:
        public core::PHClass,
        public core::IRouteTarget
    {
    public:
        DECLARE_FORM_MAP

        void OnPing();
    };
}


#define CONFIG_WATCHDOG_INIT \
    CWatchDogAppContext __watchdog;\
    __appContext.addEntry(CWatchDogAppContext::getFormEntries(), &__watchdog, &__watchdog);\
    __watchdog.setWriter(&__writer);

