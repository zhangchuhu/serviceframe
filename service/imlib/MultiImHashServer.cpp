#include "MultiImHashServer.h"

using namespace core;
using namespace server;
using namespace server::im;
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//添加服务器

bool CRHashServerValueHelper::AddServer(uint32_t uServerValue)
{
	boost::mutex::scoped_lock sl(m_mutexValue);

	return CHashServerValueHelper::AddServer(uServerValue);
}

//删除服务器

bool CRHashServerValueHelper::RemoveServer(uint32_t uServerValue)
{
	boost::mutex::scoped_lock sl(m_mutexValue);

	return CHashServerValueHelper::RemoveServer(uServerValue);
}

//清空所有服务器

void CRHashServerValueHelper::ClearServer()
{
	boost::mutex::scoped_lock sl(m_mutexValue);

	CHashServerValueHelper::ClearServer();
}

//根据因子，从集合中挑出一个Value值，如果集合为空,返回 NONE_SERVER_ID

uint32_t CRHashServerValueHelper::GetServerValueByFactor(uint32_t uFactor)
{
	boost::mutex::scoped_lock sl(m_mutexValue);

	return CHashServerValueHelper::GetServerValueByFactor(uFactor);
}

//检查服务器Value在集合中是否存在

bool CRHashServerValueHelper::IsServerValueExist(uint32_t uServerValue)
{
	boost::mutex::scoped_lock sl(m_mutexValue);

	return CHashServerValueHelper::IsServerValueExist(uServerValue);
}

//取全部服务器ID集合

void CRHashServerValueHelper::GetServerValueSet(std::set<uint32_t> &setServerValue)
{
	boost::mutex::scoped_lock sl(m_mutexValue);

	CHashServerValueHelper::GetServerValueSet(setServerValue);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
