#include "MultiImHashServer.h"

using namespace core;
using namespace server;
using namespace server::im;
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//��ӷ�����

bool CRHashServerValueHelper::AddServer(uint32_t uServerValue)
{
	boost::mutex::scoped_lock sl(m_mutexValue);

	return CHashServerValueHelper::AddServer(uServerValue);
}

//ɾ��������

bool CRHashServerValueHelper::RemoveServer(uint32_t uServerValue)
{
	boost::mutex::scoped_lock sl(m_mutexValue);

	return CHashServerValueHelper::RemoveServer(uServerValue);
}

//������з�����

void CRHashServerValueHelper::ClearServer()
{
	boost::mutex::scoped_lock sl(m_mutexValue);

	CHashServerValueHelper::ClearServer();
}

//�������ӣ��Ӽ���������һ��Valueֵ���������Ϊ��,���� NONE_SERVER_ID

uint32_t CRHashServerValueHelper::GetServerValueByFactor(uint32_t uFactor)
{
	boost::mutex::scoped_lock sl(m_mutexValue);

	return CHashServerValueHelper::GetServerValueByFactor(uFactor);
}

//��������Value�ڼ������Ƿ����

bool CRHashServerValueHelper::IsServerValueExist(uint32_t uServerValue)
{
	boost::mutex::scoped_lock sl(m_mutexValue);

	return CHashServerValueHelper::IsServerValueExist(uServerValue);
}

//ȡȫ��������ID����

void CRHashServerValueHelper::GetServerValueSet(std::set<uint32_t> &setServerValue)
{
	boost::mutex::scoped_lock sl(m_mutexValue);

	CHashServerValueHelper::GetServerValueSet(setServerValue);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
