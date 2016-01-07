#include "ImMemDBHelper.h"

using namespace server::im;
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//�����û�����״̬������ImLinkdId

bool CImMemDBHelper::SetUserOnlineInfo(uint32_t Uid, const protocol::E_IMSTATUS &emStatus, uint32_t uImLinkdId)
{
	CUserOnlineInfo stUserOnline(Uid, uImLinkdId, emStatus);
	return __SetUserOnlineToCache(Uid, stUserOnline);
}

bool CImMemDBHelper::SetUserOnlineInfo(uint32_t Uid, CUserOnlineInfo &stUserOnline)
{
	return __SetUserOnlineToCache(Uid, stUserOnline);
}

//��ȡ�û�����״̬������ImLinkdId
//���߷���false,��������true

bool CImMemDBHelper::GetUserOnlineInfo(uint32_t Uid, protocol::E_IMSTATUS &emStatus, uint32_t &uImLinkdId)
{
	CUserOnlineInfo stUserOnline;
	bool bGetOnline = __GetUserOnlineFromCache(Uid, stUserOnline);
	if (bGetOnline)
	{
		emStatus = stUserOnline.m_emStatus;
		uImLinkdId = stUserOnline.m_uImLinkdId;
	}
	else
	{
		emStatus = protocol::IMOFFLINE;
		uImLinkdId = -1;
	}
	return stUserOnline.IsNotOffline(); //��Ȼ��ȡʧ�ܣ���Ȼ���Է�������ʹ�ã���ΪstUserOnline����ʱ����ʹ������
}

//���߷���false,��������true

bool CImMemDBHelper::GetUserOnlineInfo(uint32_t Uid, CUserOnlineInfo &stUserOnline)
{
	bool bGetOnline = __GetUserOnlineFromCache(Uid, stUserOnline);
	if (!bGetOnline)
	{
		stUserOnline.m_emStatus = protocol::IMOFFLINE;
		stUserOnline.m_uImLinkdId = -1;
	}
	return stUserOnline.IsNotOffline();
}

//���߷���false,��������true

bool CImMemDBHelper::GetUserOnlineInfo(uint32_t Uid, uint32_t& uImLinkdId)
{
	CUserOnlineInfo stUserOnline;
	bool bGetOnline = __GetUserOnlineFromCache(Uid, stUserOnline);
	if (bGetOnline)
	{
		uImLinkdId = stUserOnline.m_uImLinkdId;
	}
	else
	{
		uImLinkdId = -1;
	}
	return stUserOnline.IsNotOffline();
}

//�����û�����״̬

bool CImMemDBHelper::EraseUserOnlineInfo(uint32_t Uid)
{
	return __EraseUserOnlineInfo(Uid);
}

//����Online memcached

bool CImMemDBHelper::ClearAllOnlineInfo()
{
	return __ClearAllOnlineInfo();
}

//���������û�����״̬������ImLinkdId

bool CImMemDBHelper::BatchSetUserOnlineInfo(vector<CUserOnlineInfo> &vecUserOnline)
{
	server::mc::Memcached* mc = getMemcachedFactory()->getMemcached(IMONLINE_MEMDB);
	if (mc == NULL)
		return false;

	for (vector<CUserOnlineInfo>::iterator iterUserOnline = vecUserOnline.begin();
		iterUserOnline != vecUserOnline.end();
		iterUserOnline ++)
	{
		//__SetUserOnlineToCache(iterUserOnline->m_Uid, *iterUserOnline); //��ʹ�ô�����Ϊ�˷�ֹѭ����ε���getMemcached
		mc->mcSet(__Uid2Key(iterUserOnline->m_Uid), *iterUserOnline, EXPIRATION_TIME);
	}
	return true;
}

//������ȡ�û�����״̬������ImLinkdId

bool CImMemDBHelper::BatchGetUserOnlineInfo(const vector<uint32_t> &vecUid, std::vector<CUserOnlineInfo> &vecUserOnline)
{
	//�ڷ��ؼ��У���������������UID���ؽ����������鲻����¼����Ϊ���ߣ�

	return __BatchGetUserOnlineInfo(vecUid, vecUserOnline);

	//vecUserOnline.clear();

	//for (vector<uint32_t>::const_iterator iterUid = vecUid.begin();
	//	 iterUid != vecUid.end();
	//	 iterUid ++)
	//{
	//	CUserOnlineInfo stUserOnline;
	//	bool bGetOnline = __GetUserOnlineFromCache(*iterUid, stUserOnline);
	//	if (!bGetOnline)
	//	{
	//		stUserOnline.m_Uid = *iterUid;
	//		stUserOnline.m_uImLinkdId = -1;
	//		stUserOnline.m_emStatus = protocol::IMOFFLINE;
	//	}
	//	vecUserOnline.push_back(stUserOnline);
	//}

	//return true;
}

//���������û�����״̬������ImLinkdId

bool CImMemDBHelper::BatchEraseUserOnlineInfo(const vector<uint32_t> &vecUid)
{
	server::mc::Memcached* mc = getMemcachedFactory()->getMemcached(IMONLINE_MEMDB);
	if (mc == NULL)
		return false;

	for (vector<uint32_t>::const_iterator iterUid = vecUid.begin();
		iterUid != vecUid.end();
		iterUid ++)
	{
		//__EraseUserOnlineInfo(*iterUid);
		mc->mcDel(__Uid2Key(*iterUid));
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����/��ȡ������û�������Ϣ�����̰߳�ȫ��

bool CImMemDBHelper::__SetUserOnlineToCache(uint32_t Uid, CUserOnlineInfo &stUserOnline)
{
	server::mc::Memcached* mc = getMemcachedFactory()->getMemcached(IMONLINE_MEMDB);
	if (mc == NULL)
		return false;

	return mc->mcSet(__Uid2Key(Uid), stUserOnline, EXPIRATION_TIME);
}

bool CImMemDBHelper::__GetUserOnlineFromCache(uint32_t Uid, CUserOnlineInfo &stUserOnline)
{
	server::mc::Memcached* mc = getMemcachedFactory()->getMemcached(IMONLINE_MEMDB);
	if (mc == NULL)
		return false;

	return mc->mcGet(__Uid2Key(Uid), stUserOnline);
}

bool CImMemDBHelper::__EraseUserOnlineInfo(uint32_t Uid)
{
	server::mc::Memcached* mc = getMemcachedFactory()->getMemcached(IMONLINE_MEMDB);
	if (mc == NULL)
		return false;

	return mc->mcDel(__Uid2Key(Uid));
}

bool CImMemDBHelper::__ClearAllOnlineInfo()
{
	server::mc::Memcached* mc = getMemcachedFactory()->getMemcached(IMONLINE_MEMDB);
	if (mc == NULL)
		return false;

	return mc->mcFlush();
}

bool CImMemDBHelper::__BatchGetUserOnlineInfo(const std::vector<uint32_t> &vecUid, std::vector<CUserOnlineInfo> &vecUserOnline)
{
	server::mc::Memcached* mc = getMemcachedFactory()->getMemcached(IMONLINE_MEMDB);
	if (mc == NULL)
		return false;

	//��������KEY
	std::vector<std::string> vecKey;
	for (std::vector<uint32_t>::const_iterator iterUid = vecUid.begin(); 
		 iterUid != vecUid.end();
		 iterUid ++)
	{
		vecKey.push_back(__Uid2Key(*iterUid));
	}

	std::map<std::string, CUserOnlineInfo> mapUserOnline;
	bool bGet = mc->mcGetMulti(vecKey, mapUserOnline);
	if (!bGet)
		return false;

	vecUserOnline.clear();

	for (std::vector<uint32_t>::const_iterator iterUid = vecUid.begin();
		iterUid != vecUid.end();
		iterUid ++)
	{
		//�Ȳ鷵�ص�mapUserOnline���Ƿ������ݣ��Ա��ַ����������ṩ������һ����
		std::map<std::string, CUserOnlineInfo>::const_iterator iterUser = mapUserOnline.find(__Uid2Key(*iterUid));
		if (iterUser != mapUserOnline.end())
		{
			vecUserOnline.push_back( iterUser->second );
		}
		else
		{
			CUserOnlineInfo stUserOnline(*iterUid, -1, protocol::IMOFFLINE);
			vecUserOnline.push_back(stUserOnline);
		}
	}


	//vecUserOnline.clear();
	////ѭ������ѻ�ȡ���û�
	//for (std::map<std::string, CUserOnlineInfo>::iterator iterUser = mapUserOnline.begin();
	//	 iterUser != mapUserOnline.end();
	//	 iterUser ++)
	//{
	//	vecUserOnline.push_back( iterUser->second );
	//}
	////���ʲ����ߵ��û�
	//for (std::vector<uint32_t>::const_iterator iterUid = vecUid.begin();
	//	iterUid != vecUid.end();
	//	iterUid ++)
	//{
	//	if (mapUserOnline.find(__Uid2Key(*iterUid)) == mapUserOnline.end())
	//	{
	//		CUserOnlineInfo stUserOnline(*iterUid, -1, protocol::IMOFFLINE);
	//		vecUserOnline.push_back(stUserOnline);
	//	}
	//}

	return true;
}

bool CImMemDBHelper::__BatchSetUserOnlineInfo(std::vector<CUserOnlineInfo> &vecUserOnline)
{
	//server::mc::Memcached* mc = getMemcachedFactory()->getMemcached(IMONLINE_MEMDB);
	//if (mc == NULL)
	//	return false;

	////��������KEY
	//vector<std::string> vecKey;
	//for (std::vector<CUserOnlineInfo>::const_iterator iterUser = vecUserOnline.begin(); 
	//	iterUser != vecUserOnline.end();
	//	iterUser ++)
	//{
	//	vecKey.push_back(__Uid2Key(iterUser->m_Uid));
	//}

	//mc->mcSetMulti(vecKey, );

	return true;
}

bool CImMemDBHelper::__BatchEraseUserOnlineInfo(const vector<uint32_t> &vecUid)
{
	return true;
}

inline string CImMemDBHelper::__Uid2Key(uint32_t Uid)
{
	stringstream ss;
	ss << IMONLINE_PREFIX << Uid;
	return ss.str();
}
