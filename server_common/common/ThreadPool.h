#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/bind.hpp>

#include <tbb/atomic.h>

#include <assert.h>
#include <deque>
#include <vector>
#include <string.h>

namespace server {

class ThreadPool: boost::noncopyable
{
public:
	typedef boost::function<void()> Task;

	explicit ThreadPool(const std::string& threadPoolName)
		: m_strThreadPoolName(threadPoolName), m_iQueueSize(0), m_bRunning(true)
		, m_iReq(0), m_iProc(0)
	{
		m_aiActive = 0;
	}

	~ThreadPool()
	{
		for (std::vector<boost::thread*>::iterator it = m_vtThread.begin();
				it != m_vtThread.end(); ++it)
		{
			(*it)->join();
			delete (*it);
		}
	}

	void init(int numThreads, int queueSize)
	{
		assert(m_vtThread.empty());
		m_iQueueSize = queueSize;

		for (int i = 0; i < numThreads; ++i)
		{
			char id[32];
			snprintf(id, sizeof id, "%d", i);
			boost::thread* pThread = new boost::thread(
					boost::bind(&ThreadPool::runInThread, this), m_strThreadPoolName + id);
			assert(pThread != NULL);
			m_vtThread.push_back(pThread);
		}
	}

	/**
	 * @return	0	push success
	 * 			-1	fail, queue full
	 */
	int push(const Task& task)
	{
		if (m_vtThread.empty())
		{
			task();
		}
		else
		{
			boost::mutex::scoped_lock lock(m_mutex);
			while (m_queue.size() > m_iQueueSize)
			{
				return -1;
			}

			m_queue.push_back(task);
			m_condition.notify_one();

			++m_iReq;
		}

		return 0;
	}

	void stop()
	{
		m_bRunning = false;
		boost::mutex::scoped_lock lock(m_mutex);
		m_condition.notify_all();
	}

private:
	class ScopeIncDec
	{
	public:
		tbb::atomic<int>& m_ai;
		explicit ScopeIncDec(tbb::atomic<int>& ai) : m_ai(ai)
		{
			m_ai++;
		}

		~ScopeIncDec()
		{
			m_ai--;
		}
	};

	void runInThread()
	{
		while (m_bRunning)
		{
			Task task;

			{
				boost::mutex::scoped_lock lock(m_mutex);
				// always use a while-loop, due to spurious wakeup
				while (m_queue.empty())
				{
					if (!m_bRunning)
						return ;

					m_condition.wait(m_mutex);
				}

				task = m_queue.front();
				m_queue.pop_front();
				++m_iProc;
			}

			ScopeIncDec scopeIncDec(m_aiActive);
			if (task)
			{
				task();
			}
		}
	}

public:
	boost::mutex 					m_mutex;
	boost::condition 				m_condition;
	std::string 					m_strThreadPoolName;
	std::vector<boost::thread*> 	m_vtThread;
	std::deque<Task> 				m_queue;
	int 							m_iQueueSize;
	volatile bool 					m_bRunning;
	int								m_iReq;
	int								m_iProc;
	tbb::atomic<int>				m_aiActive;
};

}

#endif
