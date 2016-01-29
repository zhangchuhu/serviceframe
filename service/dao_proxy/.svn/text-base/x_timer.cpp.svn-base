#include "x_timer.hpp"

namespace x_timer
{

TimerContainer::TimerContainer(size_t capacity)
        :_capacity(capacity),
        _timer_count(0)
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    _base_time = tv.tv_sec;
	_last_get_time = tv.tv_sec;
	_fake_time_line= tv.tv_sec;
}

TimerContainer::TimerContainer(time_t base_time, size_t capacity)
        :_capacity(capacity),
        _base_time(base_time),
        _timer_count(0)
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
	_last_get_time = tv.tv_sec;
	_fake_time_line= base_time;
}

TimerContainer::~TimerContainer()
{

}

int TimerContainer::init()
{
	for(uint32_t j = 0;j < tvec_base_t::HEADSIZE;j++)
	{
		init_list_head(j);
	}
	_tvec_base.timer_jiffies = 0;
	return 0;
}

void TimerContainer::schedule_i(Timer* timer,uint32_t node_id)
{
    // unsigned long expires = timer->_expires + _tvec_base.timer_jiffies;
    // unsigned long idx = timer->_expires;

	unsigned long expires = timer->_expires;// + _tvec_base.timer_jiffies;
    unsigned long idx = expires - _tvec_base.timer_jiffies;

    LISTNODE* vec;
    uint32_t vec_idd;

    if (idx < TVR_SIZE) 
    {
        int i = expires & TVR_MASK;
        vec = _tvec_base.tv1 + i;
		vec_idd=i;
    } 
    else if (idx < 1 << (TVR_BITS + TVN_BITS)) 
    {
        int i = (expires >> TVR_BITS) & TVN_MASK;
        vec = _tvec_base.tv2 + i;
		vec_idd=TVR_SIZE + i;
    } 
    else if (idx < 1 << (TVR_BITS + 2 * TVN_BITS)) 
    {
        int i = (expires >> (TVR_BITS + TVN_BITS)) & TVN_MASK;
        vec = _tvec_base.tv3 + i;
		vec_idd=TVR_SIZE+TVN_SIZE + i;
    } 
    else if (idx < 1 << (TVR_BITS + 3 * TVN_BITS)) 
    {
        int i = (expires >> (TVR_BITS + 2 * TVN_BITS)) & TVN_MASK;
        vec = _tvec_base.tv4 + i;
		vec_idd=TVR_SIZE+2*TVN_SIZE + i;
    } else if ((signed long) idx < 0) {
        /*
         * Can happen if you add a timer with expires == jiffies,
         * or you set a timer to go off in the past
         */
        int i = (_tvec_base.timer_jiffies & TVR_MASK);
        vec = _tvec_base.tv1 + i;
        vec_idd=i;
    } 
    else 
    {
        int i;
        /* If the timeout is larger than 0xffffffff on 64-bit
         * architectures then we use the maximum timeout:
         */
        if (idx > 0xffffffffUL) {
            idx = 0xffffffffUL;
            expires = idx + _tvec_base.timer_jiffies;
        }
        i = (expires >> (TVR_BITS + 3 * TVN_BITS)) & TVN_MASK;
        vec = _tvec_base.tv5 + i;
        vec_idd=TVR_SIZE+3*TVN_SIZE + i;
    }
    /*
     * Timers are FIFO:
     */
	add_tail_list(&timer->_entry,node_id
				,vec,vec_idd);
}


uint64_t TimerContainer::schedule (const TimerEventHandler &handler,
                            time_t future_time,
                            time_t interval,
                            void* param)
{
	if(_timer_count>=_capacity)
	{
		return 0;
	}
	uint64_t new_timer_id;
	Timer* timer_to_sched;
	if(_datas.new_id(new_timer_id,timer_to_sched)<0)
	{
		return 0;
	}
    timer_to_sched->set(handler, param, (future_time + _tvec_base.timer_jiffies), interval);
	uint32_t offset_;
	uint32_t useless_;
	id_map_t::IDFORMAT_t::divide(new_timer_id,offset_,useless_);

    schedule_i(timer_to_sched
    	,tvec_base_t::HEADSIZE+offset_);

    _timer_count++;
    return new_timer_id;
}


int TimerContainer::cancel(uint64_t timer_id)
{
	Timer* timer_to_sched;
	if(_datas.delete_id(timer_id,timer_to_sched)<0)
	{
		return -1;
	}
	del_node_list(&timer_to_sched->_entry);
    _timer_count--;
    return 0;
}

int TimerContainer::expire (time_t jiffies_inner)
{
	Timer* expired_timer;

	while (jiffies_inner >=  _tvec_base.timer_jiffies)
    {
		uint32_t index = _tvec_base.timer_jiffies & TVR_MASK;
        /*
         * Cascade timers:
         */
        if (!index &&
                (!cascade(_tvec_base.tv2, INDEX(0))) &&
                (!cascade(_tvec_base.tv3, INDEX(1))) &&
                !cascade(_tvec_base.tv4, INDEX(2)))
        {
            cascade(_tvec_base.tv5, INDEX(3));
        }

		LISTNODE* getlist=_tvec_base.tv1 + index;

        while(getlist->next!=index)
        {
        	uint32_t this_node_off=getlist->next;
            expired_timer = get_timer_bynodid(this_node_off);
			LISTNODE* node_of_next=&(expired_timer->_entry);
			LISTNODE* next_of_next=get_node(node_of_next->next);
			////////////detach
			next_of_next->prev=index;
			getlist->next=node_of_next->next;
			////////////////
			time_t interval = expired_timer->get_interval();

            if(interval > 0)
            {
				expired_timer->set_expires( ( expired_timer->get_interval() + _tvec_base.timer_jiffies ) );
                schedule_i(expired_timer,this_node_off);
			}
			else
			{
				_datas.raw_delete(this_node_off-tvec_base_t::HEADSIZE);
				_timer_count--;
			}
			//time out event here
			expired_timer->timeout();
        }

        ++_tvec_base.timer_jiffies;
    }

    return 0;
}


unsigned int TimerContainer::cascade(TimerContainer::LISTNODE *tv,unsigned int index)
{
    /* cascade all the timers from tv up one level */
	LISTNODE* getlist=tv+index;
	uint32_t this_nod_id=getlist-_tvec_base._vec;
	LISTNODE work_list;

	work_list.next=getlist->next;
	work_list.prev=getlist->prev;
	getlist->next=this_nod_id;
	getlist->prev=this_nod_id;

	// We are removing _all_ timers from the list, so we don't  have to
	// detach them individually, just clear the list afterwards.

    while (work_list.next!=this_nod_id) 
    {
    	uint32_t cur_node_id=work_list.next;
    	Timer* tmp=get_timer_bynodid(cur_node_id);
    	work_list.next=tmp->_entry.next;
        schedule_i(tmp,cur_node_id);
    }
    return index;
}

TimerContainer::LISTNODE* TimerContainer::get_node(uint32_t nodeid)
{
	if(nodeid<tvec_base_t::HEADSIZE)
	{
		return _tvec_base._vec+nodeid;
	}
	else
	{
		id_map_t::offset_t off=
			nodeid-tvec_base_t::HEADSIZE;
		Timer* p_timer;
		_datas.raw_find(off,p_timer);
		return &(p_timer->_entry);	
	}
}

TimerContainer::Timer* TimerContainer::get_timer_bynodid(uint32_t nodeid)
{
	Timer* p_timer;
	_datas.raw_find(nodeid-tvec_base_t::HEADSIZE,p_timer);
	return p_timer;
}

void TimerContainer::init_list_head(uint32_t nodeid)
{
	LISTNODE* nod=get_node(nodeid);
	nod->prev=nodeid;
	nod->next=nodeid;
}

void TimerContainer::add_tail_list(
			 LISTNODE* new_nod
			 ,uint32_t new_nod_id
			,LISTNODE* head
			,uint32_t head_id)
{
	uint32_t pre_node_id=head->prev;
	LISTNODE* pre_node=get_node(head->prev);
	head->prev = new_nod_id;
	new_nod->next = head_id;
	new_nod->prev = pre_node_id;
	pre_node->next = new_nod_id;
}

void TimerContainer::del_node_list(LISTNODE* new_nod)
{
	LISTNODE* pre_node=get_node(new_nod->prev);
	LISTNODE* nex_node=get_node(new_nod->next);
	pre_node->next=new_nod->next;
	nex_node->prev=new_nod->prev;
}

inline
unsigned int TimerContainer::INDEX(int N)
{
    return	(_tvec_base.timer_jiffies >> (TVR_BITS + N * TVN_BITS)) & TVN_MASK;
}

inline
bool TimerContainer::is_empty (void) const
{
    return (_timer_count==0);
}
inline
uint32_t TimerContainer::get_timer_count(void) const
{
	return _timer_count;
}

void TimerContainer::poll()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);

	time_t diff_jiffies = tv.tv_sec - _last_get_time;
	if(diff_jiffies>10)
	{
		diff_jiffies=10;
	}
	if(diff_jiffies!=0)
	{
		_last_get_time = tv.tv_sec;
		_fake_time_line += diff_jiffies;
	}

	time_t jiffies_inner = _fake_time_line - _base_time;
	//time_t jiffies_inner = tv.tv_sec - _base_time;

    //if(jiffies_inner > 3600 + _tvec_base.timer_jiffies)
    //{
    //    return;
	//}
	if(jiffies_inner >=  _tvec_base.timer_jiffies)
	{ 
		expire(jiffies_inner);
	}
}


}


