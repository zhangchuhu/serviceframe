#ifndef SERVER_COMMON___COMMON___PTHREAD_H
#define SERVER_COMMON___COMMON___PTHREAD_H
#include <pthread.h>
#include <syscall.h>
#include <assert.h>

namespace server{ namespace xlinux{

typedef void* FuncThreadRun(void* p);

// The memory of IFuncObj* is free-ed by PThread.
class PThread
{
public:
	explicit PThread(FuncThreadRun pFunc, void* pObj)
	{
		assert(0 == ::pthread_create(&m_pthreadId, NULL, pFunc, (void*)pObj));
	}

	virtual ~PThread()
	{
		::pthread_join(m_pthreadId, NULL);
	}

private:
	pthread_t		m_pthreadId;
	PThread();
};

template<typename T>
class ThreadLocal
{
public:
	ThreadLocal()
	{
		assert(::pthread_key_create(&key_, &ThreadLocal<T>::destroy_key) == 0);
	}

	virtual ~ThreadLocal()
	{
		::pthread_key_delete(key_);
	}

	T *Get()
	{
		T *t = (T *) ::pthread_getspecific(key_);
		return t;
	}

	void Set(T *t)
	{
		assert(::pthread_setspecific(key_, (void *) t) == 0);
	}

private:
	static void destroy_key(void *arg)
	{
		T *t = (T *) arg;
		if (t != NULL)
		{
			//Logger::debug("destory key");
			delete t;
		}
	}

private:
	pthread_key_t key_;
};

#define current_thread_id() syscall(SYS_gettid)

}}
#endif	// SERVER_COMMON___COMMON___PTHREAD_H
