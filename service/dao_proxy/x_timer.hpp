#ifndef _X_TIMER_HPP_
#define _X_TIMER_HPP_

#include <stdint.h>
#include <sys/time.h>

#include "server_common/seda/common/id_map.h"


using namespace std;
//using namespace __gnu_cxx;
namespace x_timer
{

class TimerEventHandler
{
public:
    virtual ~TimerEventHandler(){}
public:
    virtual void handle_timeout(void* param)=0;
};


class TimerContainer
{
private:
    enum
    {
        TVN_BITS = 6,
        TVR_BITS  = 8,
        TVN_SIZE  = (1 << TVN_BITS),
        TVR_SIZE  = (1 << TVR_BITS),
        TVN_MASK  = (TVN_SIZE - 1),
        TVR_MASK = (TVR_SIZE - 1),
    };
    struct LISTNODE
    {
    	uint32_t prev;
    	uint32_t next;
    };

	class Timer
	{
	public:
		/// Default constructor
		Timer() :_handler(NULL),
                _expires(0),
                _interval(0),               
                _param(NULL)
        {}
        Timer (const TimerEventHandler &handler,
               void* param,
               time_t expires,
               time_t interval)
                :_handler((TimerEventHandler*)&handler),
                _expires(expires),
                _interval(interval),
                _param(param)
        {
        }
        Timer(const Timer& o)
        		:_handler(o._handler)
        		,_expires(o._expires)
        		,_interval(o._interval)
        		,_entry(o._entry)
                ,_param(o._param)
        {
        }
        Timer& operator = (const Timer& o)
        {
        	_handler=o._handler;
        	_expires=o._expires;
        	_interval=o._interval;
        	_entry=o._entry;
            _param=o._param;
			return *this;
        }
        /// Destructor
        ~Timer(){}

        void set (    const TimerEventHandler &handler,
                      void* param,
                      time_t expires,
                      time_t interval)
        {
            _handler = (TimerEventHandler*)&handler;
            _expires = expires;
            _interval = interval;
            _param   =param;
        }
        void timeout()
        {
			if(_handler)
			{
				_handler->handle_timeout(_param);
			}
        }
		TimerEventHandler& get_handler (void)
        {
			return *_handler;
        }
        void set_handler (TimerEventHandler &handler)
        {
            _handler= &handler;
        }
        /// Get the timer value.
        time_t get_expires(void) const
        {
            return _expires;
        }
        /// Set the timer value.
        void set_expires (time_t timer_value)
        {
            _expires = timer_value;
        }
        /// Get the timer interval.
        time_t get_interval (void) const
        {
            return _interval;
        }
        /// Set the timer interval.
        void set_interval (time_t interval)
        {
            _interval = interval;
        }
        /// Get the timer_id.
        TimerEventHandler*  _handler;

        /// Time until the timer expires.
        time_t _expires;

        /// If this is a periodic timer this holds the time until the next
        /// timeout.
        time_t _interval;

        LISTNODE _entry;

        //
        void * _param;
    };

    struct tvec_base_t
    {
	    enum {HEADSIZE=TVR_SIZE+TVN_SIZE*4};
    	tvec_base_t()
    	{
    		timer_jiffies=0;
    		tv1=_vec;
			tv2=_vec+TVR_SIZE;
			tv3=_vec+TVR_SIZE+TVN_SIZE;
			tv4=_vec+TVR_SIZE+2*TVN_SIZE;
			tv5=_vec+TVR_SIZE+3*TVN_SIZE;
    	}
        time_t  timer_jiffies;
		LISTNODE* tv1;
        LISTNODE* tv2;
		LISTNODE* tv3;
		LISTNODE* tv4;
		LISTNODE* tv5;
        LISTNODE _vec[HEADSIZE];
    } ;
	typedef id_map::IDMap<Timer> id_map_t;
public:
    TimerContainer(size_t capacity);
    TimerContainer(time_t base_time, size_t capacity);
    ~TimerContainer();

    int init();
    /// True if queue is empty, else false.
    bool is_empty (void) const;
	uint32_t get_timer_count(void) const;

	///return value
	// 0: invalid(full) 
	// >0 : ok ,return unique timer_id
    uint64_t schedule (const TimerEventHandler &handler,
                   time_t future_time,
                   time_t interval = 0,
                   void* param = NULL);
    int cancel (uint64_t timer_id);
    int expire (time_t current_time);

    void poll();
	inline time_t get_current_time(){return _fake_time_line;}
private:
    void schedule_i (Timer* timer,uint32_t node_id);
    unsigned int cascade(LISTNODE *tv, unsigned int index);
    unsigned int INDEX(int N);

private:
	////node operation
	LISTNODE* get_node(uint32_t nodeid);
	Timer* get_timer_bynodid(uint32_t nodeid);
	void init_list_head(uint32_t nodeid);
	void add_tail_list(LISTNODE* new_nod,uint32_t new_nod_id,LISTNODE* head,uint32_t head_id);
	void del_node_list(LISTNODE* new_nod);

private:
    size_t	_capacity;
    time_t	_base_time;
	time_t	_last_get_time;
	time_t  _fake_time_line;
	id_map_t _datas;
    tvec_base_t	_tvec_base;
    uint32_t	_timer_count;
};


class MillisecondTimer
{
public:
	MillisecondTimer(TimerEventHandler &handler,
                      void* param=0)
	{
		_handler=&handler;
		_param=param;
		struct timeval tv;
		gettimeofday(&tv,NULL);
		_old_ms=((uint64_t)tv.tv_usec)/1000;
	}
	~MillisecondTimer(){}
public:
	void poll()
	{
		struct timeval tv;
		gettimeofday(&tv,NULL);
		uint64_t million_second=((uint64_t)tv.tv_usec)/1000;
		if(million_second!=_old_ms)
		{
			uint64_t pass_tim=million_second-_old_ms;
			if(million_second>_old_ms)
			{
				pass_tim=million_second-_old_ms;
			}
			else
			{
				pass_tim=(1000-_old_ms)+million_second;
			}
			
			if(pass_tim>100)
			{
				pass_tim=100;
			}
			_old_ms=million_second;
			_handler->handle_timeout(
				reinterpret_cast<void*>(pass_tim)
				);
		}
	}
private:
	TimerEventHandler*  _handler;
	void* _param;
	uint64_t _old_ms;
private:
	MillisecondTimer();
	MillisecondTimer(const MillisecondTimer& o);
	MillisecondTimer& operator =(const MillisecondTimer& o);
};


}


#endif //_X_TIMER_HPP_


