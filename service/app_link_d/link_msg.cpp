#include "link_msg.h"

#include "common/core/form.h"
#include "server_common/server-lib/UrlHelper.h"

using namespace  server::applinkd;
using namespace protocol;
using namespace core;

BEGIN_FORM_MAP(LinkMsg)
	ON_UREQUEST(PCS_ChatMsg, &LinkMsg::OnChatMsgFromUser)
	ON_REQUEST(PCS_ChatMsg, &LinkMsg::OnChatMsgFromServer)
END_FORM_MAP()

void LinkMsg::OnChatMsgFromUser(uint32_t uUid, protocol::PCS_ChatMsg *pMsg)
{
	std::string strMyFrom = uidFrom(uUid, server->getServerId());
	uint32_t uChatId = GetChatByRoll(pMsg->m_uSeqId + pMsg->GetNumber());
	routeS(strMyFrom, uChatId, PCS_MultiRouteChatMsg::uri, *pMsg);

	log(Info, "[LinkMsg::OnChatMsgFromUser]: {from:%u to:%u seqid:%u type:%u num:%u} route to imchat2[%u]",
		uUid, pMsg->id, pMsg->m_uSeqId, pMsg->GetProtocol(), pMsg->GetNumber(), uChatId);
}
void LinkMsg::OnChatMsgFromServer(protocol::PCS_ChatMsg *pMsg)
{

}