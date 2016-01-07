#include "GrpHashServer.h"

using namespace core;
using namespace server;
using namespace server::grp;
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//��ӷ�����

bool CHashServerValueHelper2::AddServer(uint32_t uServerValue)
{
	std::set<uint32_t>::iterator iterServer = m_setServerValue.find(uServerValue);
	if (iterServer == m_setServerValue.end())
	{
		m_setServerValue.insert(uServerValue);
		m_Hash.AddServer(uServerValue);

		log(Info, "[CHashServerValueHelper2::AddServer] ServerValue[%u]", uServerValue);
	}

	return true;
}

//ɾ��������

bool CHashServerValueHelper2::RemoveServer(uint32_t uServerValue)
{
	std::set<uint32_t>::iterator iterServer = m_setServerValue.find(uServerValue);
	if (iterServer != m_setServerValue.end())
	{
		m_setServerValue.erase(uServerValue);
		m_Hash.RemoveServer(uServerValue);

		log(Info, "[CHashServerValueHelper2::RemoveServer] ServerValue[%u]", uServerValue);
		return true;
	}

	return false;
}

//������з�����

void CHashServerValueHelper2::ClearServer()
{
	for (std::set<uint32_t>::iterator iterServer = m_setServerValue.begin(); 
		 iterServer != m_setServerValue.end();
		 iterServer ++)
	{
		m_Hash.RemoveServer(*iterServer);

		log(Info, "[CHashServerValueHelper2::ClearServer]");
	}
	m_setServerValue.clear();
}

//�������ӣ��Ӽ���������һ��Valueֵ���������Ϊ��,���� NONE_SERVER_ID

uint32_t CHashServerValueHelper2::GetServerValueByFactor(uint32_t uFactor)
{
	/*
	std::stringstream ss;
	for (std::set<uint32_t>::iterator iterValue = m_setServerValue.begin(); 
		iterValue != m_setServerValue.end();
		iterValue ++)
	{
		ss << *iterValue << ",";
	}
	*/

	uint32_t uValue = m_Hash.GetServer(uFactor);
//	log(Info, "[CHashServerValueHelper2::GetServerValueByFactor] Factor[%d] -> ServerValue[%u]", uFactor, uValue);//Justin fang -TODO: BUSYBOX-TIDYUP 

	return uValue;
}

//��������Value�ڼ������Ƿ����

bool CHashServerValueHelper2::IsServerValueExist(uint32_t uServerValue)
{
	return ( m_setServerValue.find(uServerValue) != m_setServerValue.end() );
}

//ȡȫ��������ID����

void CHashServerValueHelper2::GetServerValueSet(std::set<uint32_t> &setServerValue)
{
	setServerValue = m_setServerValue;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
