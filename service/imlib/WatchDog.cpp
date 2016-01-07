#include "common/core/form.h"
#include "WatchDog.h"

using namespace core;
using namespace server;


BEGIN_FORM_MAP(CWatchDogAppContext)
    ON_VOID(PSS_ServerPing, &CWatchDogAppContext::OnPing)
END_FORM_MAP()


void CWatchDogAppContext::OnPing()
{
    PSS_ServerPingRes obj;
    answer(PSS_ServerPingRes::uri, RES_SUCCESS, obj);
}

