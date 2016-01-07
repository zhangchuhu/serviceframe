#include "app_imchat_db.h"

#include <string>

#include "common/core/form.h"
#include "core/sox/logger.h"
#include "server_common/protocol/prouter.h"
#include "../app_link_d/redis_helper.h"
#include "../vip_dao/SingleInstance.h"
#include "server_common/server-lib/UrlHelper.h"

using namespace std;

using namespace app::imchat;
using namespace protocol;
using namespace core;
using namespace libredis;
using namespace server::router;

BEGIN_FORM_MAP(ImChatDb)
	ON_UREQUEST(PCS_ChatMsg, &ImChatDb::OnMsgArrival)
END_FORM_MAP()


void ImChatDb::OnMsgArrival(uint32_t uid, protocol::PCS_ChatMsg* pobj)
{
	ImUserStatusCommon im_user_status;
	string key=SingleDaoIns<RedisHelper>::getInstance()->uid2Key(pobj->to_uid);
	SingleDaoIns<RedisHelper>::getInstance()->redisGet(key,im_user_status);
	PUidTarget  pt;
	pt.ruri=pobj->uri;
	pt.from = uidFrom(pobj->to_uid,ss->getServerId());
	pt.uid=pobj->to_uid;
	PacketToString(*pobj,pt.load);
	sDispatcher->dispatchByServerIdSV(im_user_status.m_uLinkdId,(uint32_t) pt.uri, pt);
	log(Info,"[ImChatDb::%s] key=%s, from_uid=%d,to_uid=%d,text=%s,to_serverid=%d",__FUNCTION__,key.c_str(),pobj->from_uid,pobj->to_uid,pobj->text.c_str(),im_user_status.m_uLinkdId);
}