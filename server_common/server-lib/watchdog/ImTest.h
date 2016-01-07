#pragma once
#include "core/iserver.h"
#include "server_common/server-lib/RouteTarget.h"
#include "server_common/protocol/pimtrans.h"
#include "server_common/protocol/PImBridge.h"

using namespace protocol::im;
using namespace server::im;
using namespace server::imbridge;

class CImTest:
    public core::IRouteTarget,
    public core::IDaemonServerAware
{
public:
    void OnTestList(uint32_t uSvrID);
};

