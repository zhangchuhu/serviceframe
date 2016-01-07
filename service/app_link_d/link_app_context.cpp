#include "link_app_context.h"

#include "../app_protocol/app_protocol.h"
#include "server_common/server-lib/UrlHelper.h"
#include "server_common/server-lib/BRouteWriter.h"
#include "core/sox/sockethelper.h"


using namespace std;
using namespace core;
using namespace server::applinkd;
using namespace server::router;
using namespace protocol;


BEGIN_FORM_MAP(LinkAppContext)
	ON_REQUEST(PUidTarget, &LinkAppContext::routeU)
END_FORM_MAP()

LinkAppContext::LinkAppContext(){
	//mm[UINFO_SVID] = SUFFIX_UINFO;
	//mm[SLIST_SVID] = SUFFIX_SLIST;
	//mm[SMANAGER_SVID] = SUFFIX_CHANNEL;
	//mm[SDB_SVID] = SUFFIX_SDB;
    //mm[IMDB_SVID] = SUFFIX_IMDB;
	//mm[CATALOG_SVID] = SUFFIX_CATALOG;
	
	/* modify by Grj
	mm[IMDB_SVID] = SUFFIX_IMDB;
	mm[IMCHATD_SVID] = SUFFIX_IMCAHTD;
	mm[IMONLINED_SVID] = SUFFIX_IMONLINED;
	mm[IMCHAT2D_SVID] = SUFFIX_IMCAHT2D;
	mm[IMOFFLINEMSG_SVID] = SUFFIX_IMOLMD;
	mm[IMSEARCH_SVID] = SUFFIX_IMSEARCH;
	mm[IMIDD_SVID] = SUFFIX_IMIDD;
	//wuji start
	mm[IMAPP_SVID] = SUFFIX_IMAPP;
	mm[IMTRANS_SVID] = SUFFIX_IMTRANS;
	mm[IMUINFO_SVID] = SUFFIX_IMUINFO;
	mm[IMGETIMID_SVID] = SUFFIX_IMGETIMID;
	//wuji end
	mm[IMONDISP_SVID] = SUFFIX_IMONDISP;
	mm[IMONLINE2_SVID] = SUFFIX_IMONLINE2;
	*/
	mm[APP_IMCHAT_SVID]="app_imchat";
}


//其他服务器通过imlinkd发送给用户
void LinkAppContext::routeU(server::router::PUidTarget *ut)
{
	Sender s;
	s.setUri(ut->ruri);
	s.setResCode(ut->resCode);
	s.marshall(ut->load.data(), ut->load.size());

	log(Info, "[LinkAppContext::routeU]: route from: %s, to uid: %d, uri: 0X%04x", ut->from.data(), ut->uid, ut->ruri);


	int res = uDispatcher->dispatchByUid(ut->uid, s ,true);
	
	if(res != RES_SUCCESS)
	{
		log(Error, "[LinkAppContext::routeU]: error occuer when dispatching uid = %u, res = %d, uri: %ld, from:%s", ut->uid, res, ut->ruri, ut->from.c_str());
	}
}


//客户端发过来的包，不属于imlinkd自己处理的消息，发送给其他服务器处理
void LinkAppContext::DefaultDispatch(Request &request, IConn *conn)
{
	//load config
	std::string from = request.getKey();
	uint32_t uid=uDispatcher->getUidByCid(conn->getConnId());
	if(from.empty())
	{

		uint32_t svid = request.getUri() & 0xff;
		std::map<int, std::string>::iterator it = mm.find(svid);
		if(it != mm.end())
		{
			PRouter pr;
			pr.from = uidFrom(uid,ss->getServerId());
			pr.to = it->second;
			pr.resCode = request.getResCode();
			pr.ruri = request.getUri();
			request.leftPack(pr.load);
			pr.ttl = 1;

			// 默认发送到随即
			sDispatcher->dispatchToServerRandom(pr.to, PRouter::uri, pr);

			log(Info, "[LinkAppContext::DefaultDispatch]: from user = %s to server = %s, uri = %u/%u",
				pr.from.data(), pr.to.data(), pr.ruri & 0xff, pr.ruri >> 8);
		}
		else
		{
			string fromIp = sox::addr_ntoa(conn->getPeerIp());
			log(Error, "[LinkAppContext::DefaultDispatch]: svid not in map: %d, uri: 0x%x. peerIp(%s)-peerPort(%d)-uid(%d)",
				svid, request.getUri(), fromIp.data(), conn->getPeerPort(), uid);
		}
	}
	else
	{
		log(Error, "[LinkAppContext::DefaultDispatch]: linkd proc unkown from:%s uri: 0x%x", from.data(), request.getUri());
	}
	
}

