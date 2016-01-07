#include "LinkdManager.h"
#include "core/sox/sockethelper.h"

using namespace protocol::imlbs;
using namespace server::imlbs;
using namespace core;
using namespace std;



CLinkdManager::CLinkdManager()
{
	m_uIndex = 0;
	m_uNotifyCounter = 0;
}

void CLinkdManager::__AdjustLinkd(uint32_t uLinkdId, SLinkdInfo& res)
{
	if (res.m_uCounter >= MAX_NUM_ONLINE_LINKD && res.m_bValid != false)
	{
		// linkd counter大于限值，但标志可用
		__RemoveValue(m_validLinkd, uLinkdId);
		res.m_bValid = false;
		
		string strIp = sox::addr_ntoa(res.m_uIp);
		log (Info, "[CLinkdManager::__AdjustLinkd] imlinkd: %s online-num is %d. out limit.", strIp.data(), res.m_uCounter);
	}
	else if (res.m_uCounter < MAX_NUM_ONLINE_LINKD && res.m_bValid != true)
	{
		// linkd counter 小于限值，但标志不可用
		m_validLinkd.push_back(uLinkdId);
		res.m_bValid = true;
		
		string strIp = sox::addr_ntoa(res.m_uIp);
		log (Info, "[CLinkdManager::__AdjustLinkd] imlinkd: %s update online-num %d. change to valid.", strIp.data(), res.m_uCounter);
	}
}

void CLinkdManager::SetImLinkdInfo(uint32_t uLinkdId, uint32_t uOnline)
{
	map<uint32_t, SLinkdInfo>::iterator it = m_mapLinkdInfo.find(uLinkdId);
	if (it == m_mapLinkdInfo.end())
	{
		if (m_uNotifyCounter % 1500 == 0)
		{
			// 没有记录该imlinkd信息
			log (Error, "[CLinkdManager::SetImLinkdInfo] imlinkd(%d) update online-info, but not in my map. bug !!!!", uLinkdId);

			// imlinkd与daemon连接完整，但是imlinkd向imlbs上报端口的包丢了，通知imlinkd重发
			answer(PSS_NotifyImlinkdReSendPorts::uri, RES_SUCCESS, PSS_NotifyImlinkdReSendPorts());
		}
		m_uNotifyCounter++;

		return;
	}

	it->second.m_uCounter = uOnline;
	__AdjustLinkd(uLinkdId, it->second);
}

// 同组imlinkd向imlbs注册信息

void CLinkdManager::SetImLinkdInfoByGroup(uint32_t uLinkdId, uint32_t uOnline, uint32_t uIp, const std::vector<uint16_t>& vecPorts)
{
	// 兼容。。。
	vector<uint16_t> tmpPorts(vecPorts.rbegin(), vecPorts.rend());
	m_uNotifyCounter = 0;

	map<uint32_t, SLinkdInfo>::iterator itMap = m_mapLinkdInfo.find(uLinkdId);
	if (itMap != m_mapLinkdInfo.end())
	{
		// 存在linkdID
		itMap->second.m_uIp = uIp;
		itMap->second.m_vecPorts = tmpPorts;
		itMap->second.m_uCounter = uOnline;
		__AdjustLinkd(uLinkdId, itMap->second);
	}
	else
	{
		if (uOnline >= MAX_NUM_ONLINE_LINKD)
		{
			m_mapLinkdInfo.insert(make_pair(uLinkdId, SLinkdInfo(false, uOnline, uIp, tmpPorts)));
		}
		else
		{
			m_mapLinkdInfo.insert(make_pair(uLinkdId, SLinkdInfo(true, uOnline, uIp, tmpPorts)));
			m_validLinkd.push_back(uLinkdId);
		}
	}
}

// 同ISP的imlinkd向imlbs注册信息

void CLinkdManager::SetImLinkdInfoByISP(uint32_t uLinkdId, const std::string& strIp, const std::vector<uint16_t>& vecPorts)
{
	// 兼容。。。
	vector<uint16_t> tmpPorts(vecPorts.rbegin(), vecPorts.rend());
	m_uNotifyCounter = 0;

	map<uint32_t, SLinkdInfoSameISP>::iterator itMap = m_mapLinkdInfoSameISP.find(uLinkdId);
	if (itMap != m_mapLinkdInfoSameISP.end())
	{
		// 更新记录
		itMap->second.m_strIp = strIp;
		itMap->second.m_vecPorts = tmpPorts;
	}
	else
	{
		m_mapLinkdInfoSameISP.insert(make_pair(uLinkdId, SLinkdInfoSameISP(strIp, tmpPorts)));
		m_allLinkd.push_back(uLinkdId);
	}
}

void CLinkdManager::DelImLinkdByGroup(uint32_t uLinkdId)
{
	m_mapLinkdInfo.erase(uLinkdId);
	__RemoveValue(m_validLinkd, uLinkdId);
}

void CLinkdManager::DelImLinkdByISP(uint32_t uLinkdId)
{
	m_mapLinkdInfoSameISP.erase(uLinkdId);
	__RemoveValue(m_allLinkd, uLinkdId);
}

// imlbs 收到ping请求后，获取imlinkd信息，要做上限校验
bool CLinkdManager::GetImlinkInfo2(uint32_t& uGroupId, uint32_t &ip, std::vector<uint16_t> &ports)
{
	if (m_validLinkd.empty())
	{
		return false;
	}

	VecLinkd_T::iterator it = m_validLinkd.begin() + ++m_uIndex % m_validLinkd.size();

	uint32_t uLinkdId = *it;
	
	map<uint32_t, SLinkdInfo>::iterator itMap = m_mapLinkdInfo.find(uLinkdId);
	if (itMap != m_mapLinkdInfo.end())
	{
		ip = itMap->second.m_uIp;
		ports = itMap->second.m_vecPorts;
		uGroupId = m_pLbsRequest->GetGroupId();
		
		itMap->second.m_uCounter ++;
		if (itMap->second.m_uCounter >= MAX_NUM_ONLINE_LINKD)
		{
			itMap->second.m_bValid = false;
			__RemoveValue(m_validLinkd, uLinkdId);
			//m_invalidLinkd.push_back(uLinkdId);
		}

		return true;
	}
	else
	{
		log (Error, "[CLinkdManager::GetImlinkInfo2] linkd (%d) in valid list, but not in map!!!", uLinkdId);
		m_validLinkd.erase(it);
	}
	return false;
}

// imlbs收到旧UI请求，或者强制登陆请求，不检查imlinkd上限
bool CLinkdManager::GetImlinkInfo(std::string &ip, std::vector<uint16_t> &ports)
{
	if (m_allLinkd.empty())
	{
		return false;
	}
		
	VecLinkd_T::iterator it = m_allLinkd.begin() + (++m_uIndex % m_allLinkd.size());	
	map<uint32_t, SLinkdInfoSameISP>::iterator itMap = m_mapLinkdInfoSameISP.find(*it);
	if (itMap != m_mapLinkdInfoSameISP.end())
	{
		ip = itMap->second.m_strIp;
		ports = itMap->second.m_vecPorts;

		return true;
	}
	else
	{
		log (Error, "[CLinkdManager::GetImlinkInfo] linkd (%d) in list, but not in map!!!", *it);

		m_allLinkd.erase(it);
	}

	return false;
}

void CLinkdManager::__RemoveValue(VecLinkd_T& vec, uint32_t value)
{
	for (VecLinkd_T::iterator it = vec.begin(); it != vec.end(); ++it)
	{
		if (*it == value)
		{
			vec.erase(it);
			break;
		}
	}
}

// 打印本机房相关imlinkd的信息

void CLinkdManager::Trace()
{
	log (Info, "[CLinkdManager::Trace] Trace same group Linkd Info");
	for (map<uint32_t, SLinkdInfo>::iterator it = m_mapLinkdInfo.begin(); it != m_mapLinkdInfo.end(); ++it)
	{
		SLinkdInfo& tmRes = it->second;
		string strIp = sox::addr_ntoa(tmRes.m_uIp);
		if (tmRes.m_bValid)
		{
			log (Info, "[CLinkdManager::Trace] Valid imlink: srvId(%d)-online(%d)-%s", it->first, tmRes.m_uCounter, strIp.data());
		}
		else
		{
			log (Info, "[CLinkdManager::Trace] InValid imlink: srvId(%d)-online(%d)-%s", it->first, tmRes.m_uCounter, strIp.data());
		}
	}
}
