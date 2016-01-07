#undef __CLASS__
#define __CLASS__ "CImStatistics"

#include <vector>
#include "ImStatistics.h"
#include "server_common/server-lib/Logger.h"
#include "server_common/server-lib/id_def.h"

using namespace core;
using namespace server;
using namespace server::imstatistics;
using std::pair;

#define SEND_TIMER_INTERVAL 300

boost::mutex statwriter_mux;
std::vector<ThreadStatWriter *> statwriters;

void cleanUpWriter(ThreadStatWriter *ptr){
	scoped_lock sl(statwriter_mux);
	for(std::vector<ThreadStatWriter*>::iterator it = statwriters.begin(); it != statwriters.end(); ++it)
	{
		if(*it == ptr)
		{
			statwriters.erase(it);
			break;
		}
	}
}

void ThreadStatWriter::RouteStat(uint32_t uStatId)
{
	if (uStatId <= MAX_STATIDBYDAY)
	{
		scoped_lock sl(m_mutex);

		MapStatIdNum_T::iterator it = m_mapStatIdNum.find(uStatId);
		if (it != m_mapStatIdNum.end())
		{
			(it->second)++;
		}
		else
		{
			m_mapStatIdNum.insert(pair<uint32_t, uint32_t>(uStatId, 1));
		}
	}
	else
	{
		FUNLOG(Info, "statid(%d) is valid for RouteStat 1", uStatId);
	}
}
void ThreadStatWriter::RouteStat(uint32_t uStatId, uint32_t uNum)
{
	if (uStatId > MAX_STATIDBYDAY)
	{
		scoped_lock sl(m_mutex);

		MapStatIdNum_T::iterator it = m_mapStatIdNum.find(uStatId);
		if (it != m_mapStatIdNum.end())
		{
			it->second = uNum;
		}
		else
		{
			m_mapStatIdNum.insert(pair<uint32_t, uint32_t>(uStatId, uNum));
		}
	}
	else
	{
		FUNLOG(Info, "statid(%d) is valid for RouteStat 2", uStatId);
	}
}

void ThreadStatWriter::Flush(std::vector<SStatData>& vecByTime, 
								std::vector<SStatData>& vecByDay, 
								std::vector<SStatData>& vecWithNum)
{

	std::vector<SStatData>& vecStatByTime = vecByTime;
	std::vector<SStatData>& vecStatByDay = vecByDay;	
	std::vector<SStatData>& vecStatWithNum = vecWithNum;
	{
		scoped_lock sl(m_mutex);

		for (MapStatIdNum_T::iterator it = m_mapStatIdNum.begin();
			it != m_mapStatIdNum.end();
			it++)
		{
			SStatData data(it->first, it->second);
			if(it->first <= MAX_STATIDBYTIME)
			{
				vecStatByTime.push_back(data);
				it->second = 0;
				FUNLOG(Debug, "statidbytime statid(%d) num(%d)", data.m_uStatId, data.m_uCount);
			}
			else if (it->first <= MAX_STATIDBYDAY)
			{
				vecStatByDay.push_back(data);
				it->second = 0;
				FUNLOG(Debug, "statidbyday statid(%d) num(%d)", data.m_uStatId, data.m_uCount);
			}
			else
			{
				vecStatWithNum.push_back(data);
				FUNLOG(Debug, "statidwithnum statid(%d) num(%d)", data.m_uStatId, data.m_uCount);
			}
		}
	}
}


CRImStatistics::CRImStatistics():ptr(cleanUpWriter)
{
    m_sendTimer.Init(this, &CRImStatistics::SendStatData);
    m_sendTimer.Start(SEND_TIMER_INTERVAL * 1000);
}

ThreadStatWriter* CRImStatistics::getThreadStatWriter()
{	
	if (ptr.get() == NULL){
		scoped_lock sl(statwriter_mux);
		ThreadStatWriter *c = new ThreadStatWriter();
		statwriters.push_back(c);
		ptr.reset(c);
	}

	return ptr.get();
}

void CRImStatistics::RouteStat(uint32_t uStatId)
{
	getThreadStatWriter()->RouteStat(uStatId);
}
void CRImStatistics::RouteStat(uint32_t uStatId, uint32_t uNum)
{
	getThreadStatWriter()->RouteStat(uStatId, uNum);
}

void CRImStatistics::SendStatData()
{

	std::vector<SStatData> vecStatByTime;
	std::vector<SStatData> vecStatByDay;	
	std::vector<SStatData> vecStatWithNum;
	{
		scoped_lock sl(statwriter_mux);
		for(std::vector<ThreadStatWriter*>::iterator it = statwriters.begin(); it != statwriters.end(); ++it)
		{
			(*it)->Flush(vecStatByTime, vecStatByDay, vecStatWithNum);
		}
	}

	if (!vecStatByTime.empty())
	{
		PSS_StatNotify obj(STATBYTIME, vecStatByTime);
		route(SUFFIX_IMSTAT, obj.uri, obj);
	}

	if (!vecStatByDay.empty())
	{
		PSS_StatNotify obj(STATBYDAY, vecStatByDay);
		route(SUFFIX_IMSTAT, obj.uri, obj);
	}

	if (!vecStatWithNum.empty())
	{
		PSS_StatNotify obj(STATWITHNUM, vecStatWithNum);
		route(SUFFIX_IMSTAT, obj.uri, obj);
	}
}

CImStatistics::CImStatistics()
{
	m_mapStatIdNum.clear();
	
    m_sendTimer.Init(this, &CImStatistics::SendStatData);
    m_sendTimer.Start(SEND_TIMER_INTERVAL * 1000);
}

void CImStatistics::RouteStat(uint32_t uStatId)
{
	if (uStatId <= MAX_STATIDBYDAY)
	{
		//boost::mutex::scoped_lock sllk(m_MutexStatData);

		MapStatIdNum_T::iterator it = m_mapStatIdNum.find(uStatId);
		if (it != m_mapStatIdNum.end())
		{
			(it->second)++;
		}
		else
		{
			m_mapStatIdNum.insert(pair<uint32_t, uint32_t>(uStatId, 1));
		}
	}
	else
	{
		FUNLOG(Info, "statid(%d) is valid for RouteStat 1", uStatId);
	}
}
void CImStatistics::RouteStat(uint32_t uStatId, uint32_t uNum)
{
	if (uStatId > MAX_STATIDBYDAY)
	{
		//boost::mutex::scoped_lock sllk(m_MutexStatData);

		MapStatIdNum_T::iterator it = m_mapStatIdNum.find(uStatId);
		if (it != m_mapStatIdNum.end())
		{
			it->second = uNum;
		}
		else
		{
			m_mapStatIdNum.insert(pair<uint32_t, uint32_t>(uStatId, uNum));
		}
	}
	else
	{
		FUNLOG(Info, "statid(%d) is valid for RouteStat 2", uStatId);
	}
}

void CImStatistics::SendStatData()
{

	std::vector<SStatData> vecStatByTime;
	std::vector<SStatData> vecStatByDay;	
	std::vector<SStatData> vecStatWithNum;
	{
		//boost::mutex::scoped_lock sllk(m_MutexStatData);
		for (MapStatIdNum_T::iterator it = m_mapStatIdNum.begin();
			it != m_mapStatIdNum.end();
			it++)
		{

			SStatData data(it->first, it->second);
			if(it->first <= MAX_STATIDBYTIME)
			{
				vecStatByTime.push_back(data);
				it->second = 0;
				FUNLOG(Debug, "statidbytime statid(%d) num(%d)", data.m_uStatId, data.m_uCount);
			}
			else if (it->first <= MAX_STATIDBYDAY)
			{
				vecStatByDay.push_back(data);
				it->second = 0;
				FUNLOG(Debug, "statidbyday statid(%d) num(%d)", data.m_uStatId, data.m_uCount);
			}
			else
			{
				vecStatWithNum.push_back(data);
				FUNLOG(Debug, "statidwithnum statid(%d) num(%d)", data.m_uStatId, data.m_uCount);
			}
		}
	}

	if (!vecStatByTime.empty())
	{
		PSS_StatNotify obj(STATBYTIME, vecStatByTime);
		route(SUFFIX_IMSTAT, obj.uri, obj);
	}

	if (!vecStatByDay.empty())
	{
		PSS_StatNotify obj(STATBYDAY, vecStatByDay);
		route(SUFFIX_IMSTAT, obj.uri, obj);
	}

	if (!vecStatWithNum.empty())
	{
		PSS_StatNotify obj(STATWITHNUM, vecStatWithNum);
		route(SUFFIX_IMSTAT, obj.uri, obj);
	}
}

