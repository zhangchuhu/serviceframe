#ifndef SYNCHRNOIZED_H
#define SYNCHRNOIZED_H

#include <pthread.h>
#include <sys/time.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

class Synchronizable;

class Mutex {
	friend class Synchronizable;
	pthread_mutex_t mutex_;

	Mutex(const Mutex &);	//not allow copy
public:
	Mutex() {
		assert(pthread_mutex_init(&mutex_, NULL) == 0);
	}

	~Mutex() {
		assert(pthread_mutex_destroy(&mutex_) == 0);
	}
	
	void lock() {    
		assert(pthread_mutex_lock(&mutex_) == 0);  
	}

	void unlock() {    
		assert(pthread_mutex_unlock(&mutex_) == 0);  
	}
};	//Mutex

class MutexLock {
	Mutex *mutex_;
public:
	MutexLock(Mutex *mutex): mutex_(mutex) {
		mutex_->lock();
	}

	~MutexLock() {
		mutex_->unlock();
	}
};	//MutexLock

class Synchronizable {
	Mutex *mutex_;
	bool owner_;
	pthread_cond_t cond_;

	Synchronizable(const Synchronizable &); //not allow copy
public:
	Synchronizable() {
		mutex_ = new Mutex();
		owner_ = true;
		assert(pthread_cond_init(&cond_, NULL) == 0);  
	}

	Synchronizable(Mutex *mutex) {
		mutex_ = mutex;
		owner_ = false;
		assert(pthread_cond_init(&cond_, NULL) == 0);
	}

	virtual ~Synchronizable() {
		if (owner_) {
			delete mutex_;
		}
		pthread_cond_destroy(&cond_);
	}

	inline void lock() {
		mutex_->lock();
	}

	inline void unlock() {
		mutex_->unlock();
	}

	void wait() {
		assert(pthread_cond_wait(&cond_, &mutex_->mutex_) == 0);
	}

	bool wait(int milliseconds) {
		struct timeval tv;
		struct timespec ts;

		assert(gettimeofday(&tv, NULL) == 0);
		ts.tv_sec = tv.tv_sec + milliseconds / 1000;
		ts.tv_nsec = tv.tv_usec + milliseconds % 1000 * 1000;

		if (pthread_cond_timedwait(&cond_, &mutex_->mutex_, &ts) == 0) {
			return true;
		} else {
			return false; //return false if timedout
		}
	}
	
	void notify() {
		assert(pthread_cond_signal(&cond_) == 0);
	}

	void notifyAll() {
		assert(pthread_cond_broadcast(&cond_) == 0);
	}
};	//Synchronizable

class Synchronized {
	Synchronizable *s_;

	Synchronized(const Synchronized &); //not allow copy
public:
	explicit Synchronized(Synchronizable *s): s_(s) { s_->lock(); }

	explicit Synchronized(Synchronizable &s): s_(&s) { s_->lock(); }

	virtual ~Synchronized() { s_->unlock(); }
};	//Synchronized

#endif
