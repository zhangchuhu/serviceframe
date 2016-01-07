#include "ImHashServer.h"

using namespace core;
using namespace server;
using namespace server::im;
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//��ӷ�����

bool CHashServerValueHelper::AddServer(uint32_t uServerValue)
{
	std::set<uint32_t>::iterator iterServer = m_setServerValue.find(uServerValue);
	if (iterServer == m_setServerValue.end())
	{
		m_setServerValue.insert(uServerValue);
		m_Hash.AddServer(uServerValue);

		log(Info, "[CHashServerValueHelper::AddServer] ServerValue[%u]", uServerValue);
	}

	return true;
}

//ɾ��������

bool CHashServerValueHelper::RemoveServer(uint32_t uServerValue)
{
	std::set<uint32_t>::iterator iterServer = m_setServerValue.find(uServerValue);
	if (iterServer != m_setServerValue.end())
	{
		m_setServerValue.erase(uServerValue);
		m_Hash.RemoveServer(uServerValue);

		log(Info, "[CHashServerValueHelper::RemoveServer] ServerValue[%u]", uServerValue);
		return true;
	}

	return false;
}

//������з�����

void CHashServerValueHelper::ClearServer()
{
	for (std::set<uint32_t>::iterator iterServer = m_setServerValue.begin(); 
		 iterServer != m_setServerValue.end();
		 iterServer ++)
	{
		m_Hash.RemoveServer(*iterServer);

		log(Info, "[CHashServerValueHelper::ClearServer]");
	}
	m_setServerValue.clear();
}

//�������ӣ��Ӽ���������һ��Valueֵ���������Ϊ��,���� NONE_SERVER_ID

uint32_t CHashServerValueHelper::GetServerValueByFactor(uint32_t uFactor)
{
	std::stringstream ss;
	for (std::set<uint32_t>::iterator iterValue = m_setServerValue.begin(); 
		iterValue != m_setServerValue.end();
		iterValue ++)
	{
		ss << *iterValue << ",";
	}

	uint32_t uValue = m_Hash.GetServer(uFactor);
	log(Info, "[CHashServerValueHelper::GetServerValueByFactor] Factor[%d] -> ServerValue[%u] m_vecServerValue = [%s] ", uFactor, uValue, ss.str().c_str());

	return uValue;
}

//��������Value�ڼ������Ƿ����

bool CHashServerValueHelper::IsServerValueExist(uint32_t uServerValue)
{
	return ( m_setServerValue.find(uServerValue) != m_setServerValue.end() );
}

//ȡȫ��������ID����

void CHashServerValueHelper::GetServerValueSet(std::set<uint32_t> &setServerValue)
{
	setServerValue = m_setServerValue;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
