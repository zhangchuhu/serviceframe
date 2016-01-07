#ifndef SERVER_COMMON___COMMON___LINUXMUTEXCONDITION_H
#define SERVER_COMMON___COMMON___LINUXMUTEXCONDITION_H
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>
#include "LinuxMutex.h"


namespace server{ namespace xlinux{

	class LinuxMutexCondition
	{
	public:
		LinuxMutexCondition(LinuxMutex *m = NULL)
		{
			assert(pthread_cond_init(&m_pthreadCondition, NULL) == 0);
			if (m != NULL) {
				m_mutex = m;
				m_own_mutex = false;
			} else {
				m_mutex = new LinuxMutex();
				m_own_mutex = true;
			}
		}

		~LinuxMutexCondition()
		{
			pthread_cond_destroy(&m_pthreadCondition);
			if (m_own_mutex) {
				delete m_mutex;
			}
		}

		void notify()
		{
			assert(::pthread_cond_signal(&m_pthreadCondition) == 0);
		}

		void notifyAll()
		{
			assert(::pthread_cond_broadcast(&m_pthreadCondition) == 0);
		}

		void wait()
		{
			assert(0 == pthread_cond_wait(&m_pthreadCondition, m_mutex->pthreadMutex()));
		}

		bool wait(int ms)
		{
			struct timeval tv;
			assert(gettimeofday(&tv, NULL) == 0);

			struct timespec ts;			
			ts.tv_sec = tv.tv_sec + ms / 1000;
			ts.tv_nsec = tv.tv_usec + ms % 1000 * 1000;
			if (pthread_cond_timedwait(&m_pthreadCondition, m_mutex->pthreadMutex(), &ts) == 0) {
				return true;
			} else {
				return false; //return false if timedout
			}
		}

		server::xlinux::LinuxMutex& getLinuxMutex()
		{
			return *m_mutex;
		}

	private:
		server::xlinux::LinuxMutex	*m_mutex;
		bool m_own_mutex;
		pthread_cond_t				m_pthreadCondition;
	};

}}

#endif

