#include "XStatistic.h"
#include <sstream>
#include "server_common/common/ScopedLock.h"

using namespace server::sync_queue;
using namespace server::xlinux;

IntergerTypeStatistic::IntergerTypeStatistic(uint64_t u64IntervalMs)
	: m_timer(this)
	, m_u64Avg(0)
	, m_u64Max(0)
{
	m_timer.start(u64IntervalMs);
}

void IntergerTypeStatistic::add(uint64_t u64)
{
	ScopedLock<LinuxMutex> lock(m_mtx);
	m_mset.insert(u64);
}

std::string IntergerTypeStatistic::getPercentageStatistic()
{
	ScopedLock<LinuxMutex> lock(m_mtx);	
	return m_strPercentage;
}

bool IntergerTypeStatistic::onTimer()
{
	std::stringstream ssPercentage;
	ScopedLock<LinuxMutex> lock(m_mtx);
	uint64_t u64MSetSize = m_mset.size();
	if (0 == u64MSetSize)
	{
		m_strPercentage.clear();
		m_u64Avg = 0;
		m_u64Max = 0;
		return true;
	}
	
	uint64_t u64Sum = 0;
	uint64_t u64Max = 0;
	uint64_t u64Interval = u64MSetSize/10;
	uint64_t u64Count = 0;
	uint64_t u64TotalCount = 0;
	for (std::multiset<uint64_t>::iterator it = m_mset.begin(); it != m_mset.end(); ++it)
	{
		if (*it > u64Max)
				u64Max = *it;

		u64Sum += *it;		

		++u64Count;
		++u64TotalCount;
		if (u64Count > u64Interval)
		{
			u64Count = 0;
			if (!ssPercentage.str().empty())
				ssPercentage << ", ";
			ssPercentage << (u64TotalCount * 100 / u64MSetSize) << "% < " << *it;
		}
	}

	m_u64Avg = u64Sum / u64MSetSize;
	m_u64Max = u64Max;
	m_strPercentage = ssPercentage.str();
	m_mset.clear();

	return true;
}

IntergerStatisticPS::IntergerStatisticPS(uint64_t u64IntervalMs)
	: m_timer(this)
	, m_u64IntervalMs(u64IntervalMs)
	, m_u64PS(0)
	, m_u64Sum(0)
{
	m_timer.start(u64IntervalMs);
}

void IntergerStatisticPS::inc()
{
	++m_u64Sum;
}

bool IntergerStatisticPS::onTimer()
{
	m_u64PS = m_u64Sum * 1000 / m_u64IntervalMs;
	m_u64Sum = 0;
	return true;
}
