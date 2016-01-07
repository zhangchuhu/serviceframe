#include "./ImTransferIDHelper.h"
#include "core/sox/logger.h"
#include "server_common/server-lib/id_def.h"

using namespace server;
using namespace std;

ImTransferIDHelper::ImTransferIDHelper()
{
}

//��ӷ�����
bool ImTransferIDHelper::addServer(uint32_t imtransid)
{
	std::vector<uint32_t>::iterator iter = std::find(m_imTransSet.begin(), m_imTransSet.end(), imtransid);
	if(iter == m_imTransSet.end())
	{
		m_imTransSet.push_back(imtransid);
		m_hash.AddServer(imtransid);
	}	
	return true;
}

//ɾ��������
bool ImTransferIDHelper::removeServer(uint32_t imtransid)
{
	std::vector<uint32_t>::iterator iter = m_imTransSet.begin();
	if((iter = std::find(m_imTransSet.begin(), m_imTransSet.end(),  imtransid)) != m_imTransSet.end())
	{
		m_imTransSet.erase(iter);
		m_hash.RemoveServer(imtransid);
		return true;
	}
	return false;
}

//��uidת����imtransid,���imtransid����Ϊ��,���� NONE_SERVER_ID
uint32_t ImTransferIDHelper::uid2ImTransid(uint32_t uid)
{
	//std::stringstream ss;
	//for(uint32_t i = 0; i < m_imTransSet.size(); ++i)
	//{
	//	ss << m_imTransSet[i] << ", ";
	//}
	uint32_t result = m_hash.GetServer(uid);
	//log(Info, "[ImTransferIDHelper::uid2ImTransid]:uid: [%ld]   imtransid:[%ld]   m_imTransSet = [%s] ", uid, result, ss.str().c_str());
	return result;
}

//imtransid�Ƿ����
bool ImTransferIDHelper::imtransidExists(uint32_t imtransid)
{
	return ( std::count(m_imTransSet.begin(), m_imTransSet.end(), imtransid) >= 1 );
}

//ȡȫ��imtransid����
void ImTransferIDHelper::getImtransMapSet(std::vector<uint32_t> &vtImTransSet)
{
	vtImTransSet.clear();
	vtImTransSet.assign(m_imTransSet.begin(), m_imTransSet.end());
}

void ImTransferIDHelper::clearServer()
{
	for(std::vector<uint32_t>::iterator iter = m_imTransSet.begin(); iter != m_imTransSet.end(); ++iter)
	{
		m_hash.RemoveServer(*iter);
	}
	m_imTransSet.clear();
}

//������˼�е������,Ӧ���ǳ�ʼ��ʱDaemon������ǰ�������б�
void WatchImTransferDaemonClient::onRefreshSV()
{
	m_imTransIdHelper.clearServer();	//�����hashset

	FunLog funlog("WatchImTransferDaemonClient::onRefreshSV", true);
	std::vector<sdaemon::FavorStrSV> ff = client->getServersSV();
	
	for(std::vector<sdaemon::FavorStrSV>::iterator it = ff.begin(); it != ff.end(); ++it)
	{
		//����imtrans hashӳ��
		if(it->sstr == SUFFIX_IMTRANS && it->serverStatus == sdaemon::RUNNING)
		{
			funlog.logWithFunName(Info, "add imtrans(serverid: %d) to hash", it->serverId);
			m_imTransIdHelper.addServer(it->serverId);
		}
	}
	onTransferIDSetChanged();
}

//����������
void WatchImTransferDaemonClient::onAddSV(const sdaemon::FavorStrSV & fs)
{
	FunLog funlog("WatchImTransferDaemonClient::onAddSV", true);

	if(fs.sstr == SUFFIX_IMTRANS && fs.serverStatus == sdaemon::RUNNING)
	{
		funlog.logWithFunName(Info, "add imtrans(serverid: %d) to hash", fs.serverId);
		m_imTransIdHelper.addServer(fs.serverId);
		onTransferIDSetChanged();
	}

	if(fs.sstr == SUFFIX_IMHASHSETINFO && fs.serverStatus == sdaemon::RUNNING)
	{
		onTransferIDSetChanged();
	}
}

//������ɾ��
void WatchImTransferDaemonClient::onServerRemovedSV(const sdaemon::FavorStrSV & fs)
{
	FunLog funlog("WatchImTransferDaemonClient::onServerRemovedSV", true);	

	//�����imlinkd�Ͽ�,��Ҫ���·����ⲿ����Ӱ���û�,���������µ�½���imlinkd
	if(fs.sstr == SUFFIX_IMTRANS && m_imTransIdHelper.imtransidExists(fs.serverId))
	{
		funlog.logWithFunName(Info, "delete imtrans(serverid: %d) from hash", fs.serverId);
		m_imTransIdHelper.removeServer(fs.serverId);
		onTransferIDSetChanged();
	}
}

void WatchImTransferDaemonClient::onRecoverRefreshSV()
{
	onRefreshSV();
}

void WatchImTransferDaemonClient::onServerStatusChangeSV(const sdaemon::FavorStrSV & fs)
{
    onAddSV(fs);
}
