#include "ImTest.h"
#include "server_common/server-lib/id_def.h"
#include "core/sox/logger.h"

using namespace server::im;

void CImTest::OnTestList(uint32_t uSvrID)
{
    log(Info, "[CImTest::%s]: %u", __FUNCTION__, uSvrID);
    PSS_FillBuddyList obj;
    obj.m_uSenderUid = 77794965;
    routeS(uSvrID, PSS_FillBuddyList::uri, obj);
}

