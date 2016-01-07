#include "./ImLinkIDHelper.h"
#include "core/sox/logger.h"
#include "server_common/server-lib/id_def.h"

using namespace server;
using namespace std;

ImLinkIDHelper::ImLinkIDHelper()
{

}


//��ӷ�����
bool ImLinkIDHelper::addServer(uint32_t imlinkid)
{
	std::vector<uint32_t>::iterator iter = std::find(m_imLinkdSet.begin(), m_imLinkdSet.end(), imlinkid);
	if(iter == m_imLinkdSet.end())
	{
		m_imLinkdSet.push_back(imlinkid);
		m_hash.AddServer(imlinkid);
	}	
	return true;
}

//ɾ��������
bool ImLinkIDHelper::removeServer(uint32_t imlinkid)
{
	std::vector<uint32_t>::iterator iter = m_imLinkdSet.begin();
	if((iter = std::find(m_imLinkdSet.begin(), m_imLinkdSet.end(),  imlinkid)) != m_imLinkdSet.end())
	{
		m_imLinkdSet.erase(iter);
		m_hash.RemoveServer(imlinkid);
		return true;
	}
	else
	{
		return false;
	}	
}

//��uidת����imlinkid,���imlinkid����Ϊ��,���� NONE_SERVER_ID
uint32_t ImLinkIDHelper::uid2Imlinkid(uint32_t uid)
{
	std::stringstream ss;
	for(uint32_t i = 0; i < m_imLinkdSet.size(); ++i)
	{
		ss << m_imLinkdSet[i] << ", ";
	}
	uint32_t result = m_hash.GetServer(uid);
	log(Info, "[ImLinkIDHelper::uid2Imlinkid]:uid: [%ld]   imlinkdid:[%ld]   m_imLinkdSet = [%s] ", uid, result, ss.str().c_str());
	return result;
}

//imlinkid�Ƿ����
bool ImLinkIDHelper::imlinkidExists(uint32_t imlinkid)
{
	return ( std::count(m_imLinkdSet.begin(), m_imLinkdSet.end(), imlinkid) >= 1 );
}

//ȡȫ��imlink����
void ImLinkIDHelper::getImLinkMapSet(std::vector<uint32_t> &vtImLinkSet)
{
	vtImLinkSet.clear();
	vtImLinkSet.assign(m_imLinkdSet.begin(), m_imLinkdSet.end());
}

void ImLinkIDHelper::clearServer()
{
	for(std::vector<uint32_t>::iterator iter = m_imLinkdSet.begin(); iter != m_imLinkdSet.end(); ++iter)
	{
		m_hash.RemoveServer(*iter);
	}
	m_imLinkdSet.clear();
}

//������˼�е������,Ӧ���ǳ�ʼ��ʱDaemon������ǰ�������б�
void WatchImLinkDaemonClient::onRefreshSV()
{
	m_imlinkIdHelper.clearServer();	//�����hashset

	FunLog funlog("WatchImLinkDaemonClient::onRefreshSV", true);
	std::vector<sdaemon::FavorStrSV> ff = client->getServersSV();
	
	for(std::vector<sdaemon::FavorStrSV>::iterator it = ff.begin(); it != ff.end(); ++it)
	{
		//����imlind hashӳ��
		if(it->sstr == SUFFIX_IMLINKD && it->serverStatus == sdaemon::RUNNING)
		{
			funlog.logWithFunName(Info, "add imlinkd(serverid: %d) to hash", it->serverId);
			m_imlinkIdHelper.addServer(it->serverId);
		}
	}
	onLinkIDSetChanged();
}

//����������
void WatchImLinkDaemonClient::onAddSV(const sdaemon::FavorStrSV & fs)
{
	FunLog funlog("WatchImLinkDaemonClient::onAddSV", true);

	if(fs.sstr == SUFFIX_IMLINKD && fs.serverStatus == sdaemon::RUNNING)
	{
		funlog.logWithFunName(Info, "add imlinkd(serverid: %d) to hash", fs.serverId);
		m_imlinkIdHelper.addServer(fs.serverId);
		onLinkIDSetChanged();
	}

	if(fs.sstr == SUFFIX_IMHASHSETINFO && fs.serverStatus == sdaemon::RUNNING)
	{
		onLinkIDSetChanged();
	}
}

//������ɾ��
void WatchImLinkDaemonClient::onServerRemovedSV(const sdaemon::FavorStrSV & fs)
{
	FunLog funlog("WatchImLinkDaemonClient::onServerRemovedSV", true);	

	//�����imlinkd�Ͽ�,��Ҫ���·����ⲿ����Ӱ���û�,���������µ�½���imlinkd
	if(fs.sstr == SUFFIX_IMLINKD && m_imlinkIdHelper.imlinkidExists(fs.serverId))
	{
		funlog.logWithFunName(Info, "delete imlinkd(serverid: %d) from hash", fs.serverId);
		m_imlinkIdHelper.removeServer(fs.serverId);
		onLinkIDSetChanged();
	}
}

void WatchImLinkDaemonClient::onRecoverRefreshSV()
{
	onRefreshSV();
}

void WatchImLinkDaemonClient::onServerStatusChangeSV(const sdaemon::FavorStrSV & fs)
{
    onAddSV(fs);
}
