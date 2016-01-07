#ifndef SERVER_COMMON___SQ_CLIENT___XSTATISTIC_H
#define SERVER_COMMON___SQ_CLIENT___XSTATISTIC_H
#include "server_common/common/LinuxMutex.h"
#include "server_common/helper/TimerHandler.h"
#include "server_common/common/any_cast.h"
#include <set>

using namespace server::xlinux;

namespace server{ namespace sync_queue {

	class IntergerTypeStatistic
	{
	public:
		explicit IntergerTypeStatistic(uint64_t u64IntervalMs);

		void add(uint64_t u64);

		std::string getPercentageStatistic();		

		bool onTimer();
		TimerHandler<IntergerTypeStatistic, &IntergerTypeStatistic::onTimer> m_timer;

		LinuxMutex m_mtx;
		std::multiset<uint64_t> m_mset;

		std::string m_strPercentage;
		volatile uint64_t m_u64Avg;
		volatile uint64_t m_u64Max;

	private:
		IntergerTypeStatistic();
	};

	class IntergerStatisticPS
	{
	public:
		explicit IntergerStatisticPS(uint64_t u64IntervalMs);
		
		void inc();

		bool onTimer();
		TimerHandler<IntergerStatisticPS, &IntergerStatisticPS::onTimer> m_timer;


		uint64_t m_u64IntervalMs;

		volatile uint64_t m_u64PS;
		volatile uint64_t m_u64Sum;
	};
}}

#endif	// SERVER_COMMON___SQ_CLIENT___XSTATISTIC_H


