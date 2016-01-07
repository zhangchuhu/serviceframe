/*
	test TMessageQueue.
		statistic for 10,000,000 pop, push/pop per second>=700,000

 */
#include "TMessageQueue.h"
#include "PThread.h"
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <iostream>
#include <deque>
#include <time.h>
using namespace std;
using namespace server::xlinux;

class ThreadRunObj: public IFuncObj
{
public:
	ThreadRunObj(TMessageQueue<deque<long long> >& mq) : m_mq(mq), m_i(0) {}

	void run()
	{
		while (true)
		{
			m_i ++;
			m_mq.push(m_i);
//			sleep(2);
		}
	}
	
	long long m_i;
	TMessageQueue<deque<long long> >& m_mq;
};

int main(int argc, char** argv)
{
	TMessageQueue<deque<long long> > mq;
	mq.setQueueLimit(4000000);
	PThread pThread1(new ThreadRunObj(mq));	
	PThread pThread2(new ThreadRunObj(mq));	

	struct timeval tvLast;	
	gettimeofday(&tvLast, NULL); 	
	long long llCur = 1;
	long long llLast = llCur;

	while (true)
	{
		long long i =	mq.pop();
		if (0 != i)
			++llCur;

		if (llCur % 10000000 == 0)
		{
			struct timeval tvCur;	
			gettimeofday(&tvCur, NULL); 	
			long long u64PubTime = 1000000 * (tvCur.tv_sec -tvLast.tv_sec) + tvCur.tv_usec - tvLast.tv_usec; 
			tvLast = tvCur;
			
			cout << "pop/ps: " << (long long)((double)(llCur-llLast)/ (double)u64PubTime * 1000 * 1000) << endl;
			llLast = llCur;
		}
	}
	return 0;
}
