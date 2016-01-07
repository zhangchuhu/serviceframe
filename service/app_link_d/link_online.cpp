#include "link_online.h"

#include "common/core/form.h"
#include "core/sox/snox.h"
#include "common/res_code.h"
#include "redis_helper.h"
#include "../vip_dao/SingleInstance.h"


using namespace std;
using namespace  server::applinkd;
using namespace protocol;
using namespace sox;
using namespace core;
using namespace libredis;



BEGIN_FORM_MAP(LinkOnline)
		ON_LINK(PLoginImLinkd, &LinkOnline::onLoginIM)		//µÇÂ½½Ó¿Ú
END_FORM_MAP()

void LinkOnline::_add_into_logineds(uint32_t uid, uint32_t serialid, uint32_t cid, protocol::APP_IMSTATUS status)
{
	log(Info, "[LinkOnline::_add_into_logineds] uid = %d, serialid = %d, cid = %d, status = %d", uid, serialid, cid, status);
	ImLinkUser us;
	us.serialId = serialid;
	us.cid = cid;
	us.status = status;
	us.time = sox::env::now;

	logineds[uid] = us;
}

void LinkOnline::onLoginIM(protocol::PLoginImLinkd *p, core::IConn *conn)
{

	uint32_t uid=p->uid;
	_add_into_logineds(uid, conn->getSerialId(), conn->getConnId(), p->status);

	ImUserStatusCommon im_user_status;
	im_user_status.m_uUid=uid;
	im_user_status.m_uLinkdId=ss->getServerId();
	im_user_status.m_status=APP_IMONLINE;

	string key=SingleDaoIns<RedisHelper>::getInstance()->uid2Key(uid);
	SingleDaoIns<RedisHelper>::getInstance()->redisSet(key,im_user_status);

}
int LinkOnline::dispatchByUid(uint32_t uid, core::Sender &s, bool bTcp)
{

	users_t::iterator it = logineds.find(uid);
	if(it != logineds.end()){
		if(connManager->dispatchById(it->second.cid, s))
		{
			return RES_SUCCESS;
		}
		else
		{
			return RES_EVOLATILE;
		}
	}
	else
	{
		log(Error, "[LinkOnline::dispatchByUid]: uid = %d not logined on this imlinkd.", uid);
		return RES_ENONEXIST;
	}
}
int LinkOnline::dispatchByUid(uint32_t uid, uint32_t uri, sox::Marshallable &obj, bool bTcp) 
{
	Sender s(uri, obj);
	return dispatchByUid(uid, s, bTcp);
}

uint32_t LinkOnline::getUidByCid(uint32_t cid)
{
	map<uint32_t, ImLinkUser>::iterator iter=logineds.begin();
	for(;iter!=logineds.end();iter++)
	{
		if (cid==iter->second.cid)
		{
			return iter->first;
		}
	}
	return 0;
}