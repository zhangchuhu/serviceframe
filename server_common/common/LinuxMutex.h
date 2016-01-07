#ifndef SERVER_COMMON___COMMON___LINUXMUTEX_H
#define SERVER_COMMON___COMMON___LINUXMUTEX_H
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

namespace server{ namespace xlinux{

	class LinuxMutex
	{
	public:
		LinuxMutex()
		{
			assert(0 == pthread_mutex_init(&_lock, NULL));
		}
		~LinuxMutex()
		{
			assert(0 == pthread_mutex_destroy(&_lock));
		}

		void lock()
		{
			assert(0 == pthread_mutex_lock(&_lock));
		}

		void unlock()
		{
			assert(0 == pthread_mutex_unlock(&_lock));
		}

		pthread_mutex_t* pthreadMutex()
		{
			return &_lock;
		}

	private:
		pthread_mutex_t _lock;
	};

}}

#endif

