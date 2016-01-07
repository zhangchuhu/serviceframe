#include "Thread.h"
#include <time.h>

void *Thread::proc(void *arg) {
	Thread *thd = (Thread *) arg;
	thd->run();
	return NULL;
}

Thread::Thread(): detached_(false) {}

Thread::~Thread() {
	if (!detached_) {
		join();
	}
}

void Thread::join() {
	pthread_join(tid_, NULL);
	detached_ = true;
}

void Thread::detach() {
	detached_ = true;
	pthread_detach(tid_);
}

void Thread::start() {
	pthread_create(&tid_, NULL, &Thread::proc, this);
}

/*
void Thread::Schedule(Runnable *run) {
	Thread *thread = new Thread(run);
	thread->Start();
	thread->Detach();
	delete thread;
}
*/

void Thread::sleep(int milliseconds) {
	struct timespec ts;

	ts.tv_sec = milliseconds / 1000;
	ts.tv_nsec = milliseconds % 1000 * 1000;
	nanosleep(&ts, NULL);
}

