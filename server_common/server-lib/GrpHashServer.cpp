#include "GrpHashServer.h"

using namespace core;
using namespace server;
using namespace server::grp;
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//添加服务器

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

//删除服务器

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

//清空所有服务器

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

//根据因子，从集合中挑出一个Value值，如果集合为空,返回 NONE_SERVER_ID

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

//检查服务器Value在集合中是否存在

bool CHashServerValueHelper2::IsServerValueExist(uint32_t uServerValue)
{
	return ( m_setServerValue.find(uServerValue) != m_setServerValue.end() );
}

//取全部服务器ID集合

void CHashServerValueHelper2::GetServerValueSet(std::set<uint32_t> &setServerValue)
{
	setServerValue = m_setServerValue;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
