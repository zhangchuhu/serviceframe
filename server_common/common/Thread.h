#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <string.h>

class Runnable {
public:
	virtual ~Runnable() {}

	virtual void run() = 0;
};	//Runnable

class Function0: public Runnable {
	void (*func_)();
public:
	explicit Function0(void (*func)()): func_(func) {}

	virtual ~Function0() {}

	virtual void run() {
		(*func_)();
	}
};	//Function0

template<class Object>
class Method0: public Runnable {
	typedef void (Object::*Method)();
	Object *obj_;
	Method mf_;

public:
	explicit Method0(Object *obj, Method mf): obj_(obj), mf_(mf) {}

	virtual ~Method0() {}

	virtual void run() {
		(obj_->*mf_)();
	}
};	//Method0

template<class Object, class ParamType>
class Method1: public Runnable {
	typedef void (Object::*Method)(ParamType);
	Object *obj_;
	Method mf_;
	ParamType pt_; 

public:
	explicit Method1(Object *obj, Method mf, ParamType pt): obj_(obj), mf_(mf), pt_(pt) {}

	virtual ~Method1() {}

	virtual void run() {
		(obj_->*mf_)(pt_);
	}
};	//Method1


class Thread {
	Thread(const Thread &);

	static void *proc(void *arg) {
		Runnable *r = static_cast<Runnable *>(arg);
		r->run();
		delete r;
		return NULL;
	}

public:
	Thread(Runnable *run): runnable_(run) {}

	template<class T>
	Thread(T *t, void (T::*mf)()) {
		runnable_ = new Method0<T>(t, mf);
	}

	virtual ~Thread() {}

	void start() {
		pthread_create(&tid_, NULL, &Thread::proc, runnable_);
	}

	void detach() {
		pthread_detach(tid_);
	}

	void join() {
		pthread_join(tid_, NULL);
	}

	static void sleep(int milliseconds) {
		struct timespec ts;

		ts.tv_sec = milliseconds / 1000;
		ts.tv_nsec = milliseconds % 1000 * 1000;
		nanosleep(&ts, NULL);
	}

	static int getCurrentIdent() {
		return (int)syscall(SYS_gettid);
	}

	static void schedule(Runnable *r) {
		Thread *thread = new Thread(r);
		thread->start();
		thread->detach();
		delete thread;
	}

	static void schedule(void(*func)()) {
		schedule(new Function0(func));
	}

	template<class Object>
	static void schedule(Object *obj, void(Object::*mf)()) {
		schedule(new Method0<Object>(obj, mf));
	}

	template<class Object, class ParamType>
	static void schedule(Object *obj, void(Object::*mf)(ParamType), ParamType pt) {
		schedule(new Method1<Object, ParamType>(obj, mf, pt));
	}

private:
	pthread_t tid_;
	Runnable *runnable_;
};	//Thread


#endif
