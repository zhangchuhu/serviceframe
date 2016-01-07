#include "MultiImServersInfo.h"

using namespace server;
using namespace std;

// 通过服务器类型名和网络类型，获取相同网络下同一类型的所有服务器ID
vector<uint32_t> CMultiImServersInfo::GetServersIDByNameType(const std::string& name, const uint32_t type)
{
	boost::mutex::scoped_lock s1(m_mutex);
	return CImServersInfo::GetServersIDByNameType(name, type);
}

vector<uint32_t> CMultiImServersInfo::GetServersIDByNameTypeEx(const std::string& name, const uint32_t type)
{
	boost::mutex::scoped_lock s1(m_mutex);
	return CImServersInfo::GetServersIDByNameTypeEx(name, type);
}

// 通过服务器类型名，获取一类型的所有服务器ID
vector<uint32_t> CMultiImServersInfo::GetServersIDByName(const std::string& name)
{
	boost::mutex::scoped_lock s1(m_mutex);
	return CImServersInfo::GetServersIDByName(name); 
}

// 通过服务器名和网络类型，随即获取一个服务器ID
uint32_t CMultiImServersInfo::RandomServerIDByNameType(const std::string& name, const uint32_t type)
{
	boost::mutex::scoped_lock s1(m_mutex);
	return CImServersInfo::RandomServerIDByNameType(name, type);
}

uint32_t CMultiImServersInfo::RandomServerIDByNameTypeEx(const std::string& name, const uint32_t type)
{
	boost::mutex::scoped_lock s1(m_mutex);
	return CImServersInfo::RandomServerIDByNameTypeEx(name, type);
}

// 通过服务器名，随即获取一个服务器ID
uint32_t CMultiImServersInfo::RandomServerIDByName(const std::string& name)
{
	boost::mutex::scoped_lock s1(m_mutex);
	return CImServersInfo::RandomServerIDByName(name);
}

// 获取网络类型
uint32_t CMultiImServersInfo::GetNetTypeByID(const uint32_t serverID)
{
	boost::mutex::scoped_lock s1(m_mutex);
	return CImServersInfo::GetNetTypeByID(serverID);
}

// 判断两个服务器是否相同网络
bool CMultiImServersInfo::IsSameNetType(const uint32_t srvId1, const uint32_t srvId2)
{
	boost::mutex::scoped_lock s1(m_mutex);
	return CImServersInfo::IsSameNetType(srvId1, srvId2);
}

// 添加电信，网通 或 双线服务器
void CMultiImServersInfo::onAddSV(const sdaemon::FavorStrSV & fs)
{
	boost::mutex::scoped_lock s1(m_mutex);
	return CImServersInfo::onAddSV(fs);
}

// 
void CMultiImServersInfo::onRecoverRefreshSV()
{
	boost::mutex::scoped_lock s1(m_mutex);
	return CImServersInfo::onRecoverRefreshSV();
}

//
void CMultiImServersInfo::onServerStatusChangeSV(const sdaemon::FavorStrSV & fs)
{
	boost::mutex::scoped_lock s1(m_mutex);
	return CImServersInfo::onServerStatusChangeSV(fs);
}

//
void CMultiImServersInfo::onServerRemovedSV(const sdaemon::FavorStrSV & fs)
{
	boost::mutex::scoped_lock s1(m_mutex);
	return CImServersInfo::onServerRemovedSV(fs);
}

//
void CMultiImServersInfo::onRefreshSV()
{
	boost::mutex::scoped_lock s1(m_mutex);
	return CImServersInfo::onRefreshSV();
}
