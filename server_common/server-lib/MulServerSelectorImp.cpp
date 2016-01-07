#include "MulServerSelectorImp.h"

using namespace server;
using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����Ϊע��ָ�����͵ķ�����

void CRServerSelectorImp::RegisterServerSuffix(const std::string &strSuffix, EHashServerValueType emValueType, EHashFuntcionType emFuncType)
{
	boost::mutex::scoped_lock sl(m_mutexSelector);

	CServerSelectorImp::RegisterServerSuffix(strSuffix, emValueType, emFuncType);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����Ϊ���ڷ����������������ӿ�

void CRServerSelectorImp::onRefreshSV()
{
	boost::mutex::scoped_lock sl(m_mutexSelector);

	CServerSelectorImp::onRefreshSV();
}

void CRServerSelectorImp::onAddSV(const sdaemon::FavorStrSV & fs)
{
	boost::mutex::scoped_lock sl(m_mutexSelector);

	CServerSelectorImp::onAddSV(fs);
}

void CRServerSelectorImp::onServerRemovedSV(const sdaemon::FavorStrSV & fs)
{
	boost::mutex::scoped_lock sl(m_mutexSelector);

	CServerSelectorImp::onServerRemovedSV(fs);
}

void CRServerSelectorImp::onRecoverRefreshSV()
{
	CRServerSelectorImp::onRefreshSV();
}

void CRServerSelectorImp::onServerStatusChangeSV(const sdaemon::FavorStrSV & fs)
{
	CRServerSelectorImp::onAddSV( fs );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����Ϊ���ѡ��һ����Ҫ�ķ�����

uint32_t CRServerSelectorImp::RandomServer(const std::string &strSuffix)
{
	boost::mutex::scoped_lock sl(m_mutexSelector);

	return CServerSelectorImp::RandomServer(strSuffix);
}

uint32_t CRServerSelectorImp::RandomServerByNet(const std::string &strSuffix, uint32_t uNetType)
{
	boost::mutex::scoped_lock sl(m_mutexSelector);

	return CServerSelectorImp::RandomServerByNet(strSuffix, uNetType);
}

uint32_t CRServerSelectorImp::RandomServerByGroup(const std::string &strSuffix, uint32_t uGroupId)
{
	boost::mutex::scoped_lock sl(m_mutexSelector);

	return CServerSelectorImp::RandomServerByGroup(strSuffix, uGroupId);
}

uint32_t CRServerSelectorImp::RandomServerByNetGroup(const std::string &strSuffix, uint32_t uNetType, uint32_t uGroupId)
{
	boost::mutex::scoped_lock sl(m_mutexSelector);

	return CServerSelectorImp::RandomServerByNetGroup(strSuffix, uNetType, uGroupId);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����Ϊ��ϣѡ��һ����Ҫ�ķ�����

uint32_t CRServerSelectorImp::HashServer(uint32_t uKey, const std::string &strSuffix)
{
	boost::mutex::scoped_lock sl(m_mutexSelector);

	return CServerSelectorImp::HashServer(uKey, strSuffix);
}

bool CRServerSelectorImp::HashMultiServer(uint32_t uKey, uint32_t uValueCount, std::vector<uint32_t>& vecValues, const std::string &strSuffix)
{
	boost::mutex::scoped_lock sl(m_mutexSelector);

	return CServerSelectorImp::HashMultiServer(uKey, uValueCount, vecValues, strSuffix);
}

uint32_t CRServerSelectorImp::HashServerByNet(uint32_t uKey, const std::string &strSuffix, uint32_t uNetType)
{
	boost::mutex::scoped_lock sl(m_mutexSelector);

	return CServerSelectorImp::HashServerByNet(uKey, strSuffix, uNetType);
}

uint32_t CRServerSelectorImp::HashServerByGroup(uint32_t uKey, const std::string &strSuffix, uint32_t uGroupId)
{
	boost::mutex::scoped_lock sl(m_mutexSelector);

	return CServerSelectorImp::HashServerByGroup(uKey, strSuffix, uGroupId);
}

uint32_t CRServerSelectorImp::HashServerByNetGroup(uint32_t uKey, const std::string &strSuffix, uint32_t uNetType, uint32_t uGroupId)
{
	boost::mutex::scoped_lock sl(m_mutexSelector);

	return CServerSelectorImp::HashServerByNetGroup(uKey, strSuffix, uNetType, uGroupId);
}
