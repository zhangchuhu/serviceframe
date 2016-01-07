#include "ImTransHelper.h"
#include "common/protocol/const.h"
#include "server_common/server-lib/UrlHelper.h"

using namespace server::im;
using namespace server;
using namespace std;

// imapp����λ�ñ��뿪��
//#define IMAPP_DX_ONLY   // imappֻ�����ڵ��Ż���

// ���Ͱ���imtransfer
bool CImTransHelper::_RouteBySrvId(uint32_t Uid, uint32_t linkdId, uint32_t Uri, const sox::Marshallable &obj)
{
	uint32_t srvType = CImServersInfo::GetNetTypeByID(linkdId);
	uint32_t transId = CImTransHelper::GetTransferID(Uid, srvType);

	if (transId != (uint32_t)-1)
	{
		routeS(transId, Uri, obj);
		log (Debug, "[CImTransHelper::_RouteBySrvId]: Ok. uid(%u)-linkd(%u)-transfer(%u)-Uri(0x%x)", Uid, linkdId, transId, Uri);
		return true;
	}

	log (Error, "[CImTransHelper::_RouteBySrvId]: Faild. uid(%u)-linkd(%u)-transfer(%u)-Uri(0x%x)", Uid, linkdId, transId, Uri);
	return false;
}

// ���Ͱ���imtransfer
bool CImTransHelper::_RouteByNetType(uint32_t uid, uint32_t netType, uint32_t uri, const sox::Marshallable &obj)
{
	uint32_t transId = CImTransHelper::GetTransferID(uid, netType);

	if (transId != (uint32_t)-1)
	{
		routeS(transId, uri, obj);
		log (Debug, "[CImTransHelper::_RouteByNetType]: Ok. uid(%u)-netType(%u)-transfer(%u)-uri(0x%x)", uid, netType, transId, uri);
		return true;
	}

	log (Error, "[CImTransHelper::_RouteByNetType]: Faild. uid(%u)-netType(%u)-transfer(%u)-uri(0x%x)", uid, netType, transId, uri);
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ����hash��ȡimtransfer��serverID
uint32_t CImTransHelper::GetTransferID(uint32_t Uid, uint32_t netType)
{
	uint32_t transId = -1;
	switch (netType)
	{
	case CTL:
		transId = m_CTLHelper.uid2ImTransid(Uid);
		break;
	case CNC:
		transId = m_CNCHelper.uid2ImTransid(Uid);
		break;
	default:
		log (Error, "[CImTransHelper::GetTransferID]: server type incorrect, use CTL hash default. uid(%u)-netType(%u)", Uid, netType);
		//transId = m_CTLHelper.uid2ImTransid(Uid);
	}
    
	return transId;
}

void CImTransHelper::BatchGetTransferID(const std::vector<uint32_t>& vecUid, 
								std::vector<uint32_t>& vecTransId, 
								uint32_t netType)
{
	vecTransId.clear();
	for (vector<uint32_t>::const_iterator it = vecUid.begin(); it != vecUid.end(); it++)
	{
		vecTransId.push_back(CImTransHelper::GetTransferID(*it, netType));
	}
}

void CImTransHelper::BatchGetTransferID(const std::set<uint32_t>& vecUid, 
								std::vector<uint32_t>& vecTransId, 
								uint32_t netType)
{
	vecTransId.clear();
	for (set<uint32_t>::const_iterator it = vecUid.begin(); it != vecUid.end(); it++)
	{
		vecTransId.push_back(CImTransHelper::GetTransferID(*it, netType));
	}
}

void CImTransHelper::BatchGetTransferByimlinkd(const std::vector<uint32_t>& vecUid, 
								std::vector<uint32_t>& vecTransId, 
								uint32_t linkdId)
{
	uint32_t netType = CImServersInfo::GetNetTypeByID(linkdId);
	CImTransHelper::BatchGetTransferID(vecUid, vecTransId, netType);
}

void CImTransHelper::BatchGetTransferByimlinkds(std::vector<CIMUserTransfer>& vec)
{
	uint32_t netType;
	for (vector<CIMUserTransfer>::iterator it = vec.begin(); it != vec.end(); it++)
	{
		netType = CImServersInfo::GetNetTypeByID((*it).m_linkdID);
		(*it).m_transID = CImTransHelper::GetTransferID((*it).m_uid, netType);
	}
}

// ����transferID���͹㲥��
void CImTransHelper::BroadcastS(uint32_t uid, 
						uint32_t transId, 
						uint32_t uri, 
						const sox::Marshallable& obj)
{
	PSS_Broadcast obj1(uri, Pack2String(obj));
	obj1.SetSenderUid(uid);

	routeS(transId, PSS_Broadcast::uri, obj1);
}

// ����transferID����ת����
void CImTransHelper::FowardS(uint32_t uid,
						uint32_t transId,
						const vector<uint32_t> &vecRecvUid,
						uint32_t uri,
						const sox::Marshallable& obj)
{
	PSS_Forward objForward(uri, Pack2String(obj));
	objForward.SetSenderUid(uid);
	objForward.SetRecvUidVect(vecRecvUid);

	routeS(transId, PSS_Forward::uri, objForward);
}

// �㲥������imtransfer

void CImTransHelper::RouteAllT(uint32_t uUri, const sox::Marshallable& obj)
{
	vector<uint32_t> vecIds = GetServersIDByName(SUFFIX_IMTRANS);
	//string strTemp;
	for (vector<uint32_t>::iterator it = vecIds.begin(); it != vecIds.end(); it++)
	{
		routeS(*it, uUri, obj);
		//log (Info, "[CImTransHelper::RouteAllT]: route to imtransfer(%u)-Uri(%u) ok.", *it, uUri);
	}
}


//��������Ҫ�����ǽ��ض��İ�·�ɸ�hashset(uid)���imtransfer
//
// Uid :        ������Uid
// linkdId��    ����������imlinkd��ID
// netType:     ������������������ CTL��CNC
// Uri��        ԭ��uri
// objMarshall������
bool CImTransHelper::RouteT(uint32_t Uid, 
						uint32_t linkdId, 
						uint32_t Uri, 
						const sox::Marshallable &objMarshal)
{
	return _RouteBySrvId(Uid, linkdId, Uri, objMarshal);
}

bool CImTransHelper::RouteNetTypeT(uint32_t uid,
						uint32_t netType, 
						uint32_t uri, 
						const sox::Marshallable &objMarshal)
{
	return _RouteByNetType(uid, netType, uri, objMarshal);
}

bool CImTransHelper::RouteAsAnswerT(uint32_t Uid, 
						uint32_t Uri, 
						const sox::Marshallable &obj)
{
	string from = GetFrom();
	uint32_t linkdId;
	if (!from.empty() && DomainName::getSid(from, linkdId))
	{
		return CImTransHelper::RouteT(Uid, linkdId, Uri, obj);
	}

	return false;
}

//��������Ҫ�����ǽ�һ�����еİ������PSS_Broadcast��������imtransfer�㲥
//
// Uid :        ������Uid
// linkdId��    ����������imlinkd��ID
// netType:     ������������������ CTL��CNC
// Uri��        ԭ��uri
// objMarshall������
bool CImTransHelper::BroadcastT(uint32_t Uid, 
							uint32_t linkdId, 
							uint32_t Uri, 
							const sox::Marshallable &objMarshal)
{
	//����㲥�����ͳ�ȥ
	PSS_Broadcast objBroadcast(Uri, Pack2String(objMarshal));
	objBroadcast.SetSenderUid(Uid);
	
	return _RouteBySrvId(Uid, linkdId, PSS_Broadcast::uri, objBroadcast);
}

bool CImTransHelper::BroadcastNetTypeT(uint32_t uid, 
							uint32_t netType, 
							uint32_t uri, 
							const sox::Marshallable &objMarshal)
{
	PSS_Broadcast objBroadcast(uri, Pack2String(objMarshal));
	objBroadcast.SetSenderUid(uid);

	return _RouteByNetType(uid, netType, PSS_Broadcast::uri, objBroadcast);
}

bool CImTransHelper::BroadcastAsAnswerT(uint32_t Uid, 
							uint32_t Uri, 
							const sox::Marshallable &objMarshal)
{
	string from = GetFrom();
	uint32_t linkdId;
	if (!from.empty() && DomainName::getSid(from, linkdId))
	{
		return CImTransHelper::BroadcastT(Uid, linkdId, Uri, objMarshal);
	}

	return false;
}

//��������Ҫ�����ǽ�һ�����еİ������PSS_Forward��������imtransferת��
//
// Uid :        ������Uid
// linkdId��    ����������imlinkd��ID
// netType:     ������������������ CTL��CNC
// vecRecvUid:  ������Uids
// Uri��        ԭ��uri
// objMarshall������
bool CImTransHelper::ForwardT(uint32_t Uid, 
							uint32_t linkdId, 
							const vector<uint32_t> &vecRecvUid, 
							uint32_t Uri, 
							const sox::Marshallable &objMarshal)
{
	//����ת�������ͳ�ȥ
	PSS_Forward objForward(Uri, Pack2String(objMarshal));
	objForward.SetSenderUid(Uid);
	objForward.SetRecvUidVect(vecRecvUid);

	return _RouteBySrvId(Uid, linkdId, PSS_Forward::uri, objForward);
}

//
bool CImTransHelper::ForwardNetTypeT(uint32_t uid, 
							uint32_t netType, 
							const vector<uint32_t> &vecRecvUid, 
							uint32_t uri, 
							const sox::Marshallable &objMarshal)
{
	//����ת�������ͳ�ȥ
	PSS_Forward objForward(uri, Pack2String(objMarshal));
	objForward.SetSenderUid(uid);
	objForward.SetRecvUidVect(vecRecvUid);

	return _RouteByNetType(uid, netType, PSS_Forward::uri, objForward);
}

//
bool CImTransHelper::ForwardAsAnswerT(uint32_t Uid, 
							const vector<uint32_t> &vecRecvUid, 
							uint32_t Uri, 
							const sox::Marshallable &objMarshal)
{
	string from = GetFrom();
	uint32_t linkdId;
	if (!from.empty() && DomainName::getSid(from, linkdId))
	{
		return CImTransHelper::ForwardT(Uid, linkdId, vecRecvUid, Uri, objMarshal);
	}

	return false;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CImTransHelper::__AddServer(const sdaemon::FavorStrSV& fs)
{
	if (fs.sstr == SUFFIX_IMTRANS && fs.serverStatus == sdaemon::RUNNING)
	{
		uint32_t srvType = CImServerFunc::GetNetType(fs);
		switch (srvType)
		{
		case CTL:
			m_CTLHelper.addServer(fs.serverId);
			log(Info, "[CImTransfer::__AddServer]:add imtransfer(serverid: %u, type: %u) to CTL-hash", fs.serverId, srvType);
			break;
		case CNC:
			m_CNCHelper.addServer(fs.serverId);
			log(Info, "[CImTransfer::__AddServer]:add imtransfer(serverid: %u, type: %u) to CNC-hash", fs.serverId, srvType);
			break;
		default:
			log (Error, "[CImTransHelper::__AddServer]: error, imtransfer(serverid: %u, type: %u). Incorrect !", fs.serverId, srvType);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CImTransHelper::onRefreshSV()
{
	CImServersInfo::onRefreshSV();

	//�������, �����
	m_CNCHelper.clearServer();
	m_CTLHelper.clearServer();
	std::vector<sdaemon::FavorStrSV> ff = client->getServersSV();
	for (std::vector<sdaemon::FavorStrSV>::iterator it = ff.begin(); 
		it != ff.end(); 
		it ++)
	{
		__AddServer(*it);
	}
}

void CImTransHelper::onAddSV(const sdaemon::FavorStrSV & fs)
{
	CImServersInfo::onAddSV(fs);
	__AddServer(fs);
	//log(Info, "[CImTransfer::onAddSV]:add imtransfer(serverid: %d) to hash", fs.serverId);
}

//��Ҫʵ�ִ��麯��, ����hash��ϵ�仯(hash rebuild)
void CImTransHelper::onServerRemovedSV(const sdaemon::FavorStrSV & fs)
{
	CImServersInfo::onServerRemovedSV(fs);
	if (fs.sstr == SUFFIX_IMTRANS)
	{
		uint32_t srvType = CImServerFunc::GetNetType(fs);
		switch (srvType)
		{
		case CNC:
			m_CNCHelper.removeServer(fs.serverId);
			log(Info, "[CImTransfer::onServerRemovedSV]:delete imtransfer(serverid: %u, type: %u) from CNC-hash", fs.serverId, srvType);
			break;
		case CTL:
			m_CTLHelper.removeServer(fs.serverId);
			log(Info, "[CImTransfer::onServerRemovedSV]:delete imtransfer(serverid: %u, type: %u) from CTL-hash", fs.serverId, srvType);
			break;
		default:
			log (Error, "[CImTransHelper::onServerRemovedSV]:error, server type is %u. Incorrect !", srvType);
		}
	}
}

void CImTransHelper::onRecoverRefreshSV()
{
	CImServersInfo::onRecoverRefreshSV();
	CImTransHelper::onRefreshSV();
}

void CImTransHelper::onServerStatusChangeSV(const sdaemon::FavorStrSV & fs)
{
	CImServersInfo::onAddSV(fs);
	CImTransHelper::onAddSV(fs);
}
