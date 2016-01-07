#ifndef SERVER_COMMON___COMMON___LINUXSPIN_H
#define SERVER_COMMON___COMMON___LINUXSPIN_H

#include <unistd.h>
#include <pthread.h>

namespace server{ namespace linux{

	class LinuxSpin
	{
	public:
		LinuxSpin()
		{
			int r=pthread_spin_init(&_lock,PTHREAD_PROCESS_SHARED);
			assert(r==0);
		}
		~LinuxSpin()
		{
			int r=pthread_spin_destroy(&_lock);
			assert(r==0);
		}
	public:
		void lock()
		{
			while(pthread_spin_lock(&_lock)!=0){}
		}
		void unlock()
		{
			while(pthread_spin_unlock(&_lock)!=0){}
		}
	private:
		pthread_spinlock_t _lock;
	};

}}


#endif	// SERVER_COMMON___COMMON___LINUXSPIN_H

