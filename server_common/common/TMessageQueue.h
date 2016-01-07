#ifndef SERVER_COMMON___COMMON___TMESSAGEQUEUE_H
#define SERVER_COMMON___COMMON___TMESSAGEQUEUE_H
#include "LinuxMutexCondition.h"
#include "ScopedLock.h"

namespace server{ namespace xlinux {

template <typename SequenceContainer>
class TMessageQueue
{
public:
	typedef typename SequenceContainer::value_type      value_type;
	typedef typename SequenceContainer::size_type       size_type;
	typedef          SequenceContainer                  container_type;

	typedef typename SequenceContainer::reference       reference;
	typedef typename SequenceContainer::const_reference const_reference;
	TMessageQueue() : m_szQueueLimit(200000) {}

	// 0 success, -1 fail
	int push(const value_type& __x)
	{
		server::xlinux::ScopedLock<server::xlinux::LinuxMutex> lock(m_linxCondition.getLinuxMutex());
		if (m_seqContainer.size() >= m_szQueueLimit)
			return -1;

		m_seqContainer.push_back(__x);
		m_linxCondition.notify();
		return 0;
	}

	// wait if empty
	value_type pop()
	{
		server::xlinux::ScopedLock<server::xlinux::LinuxMutex> lock(m_linxCondition.getLinuxMutex());
		while (m_seqContainer.empty())
			m_linxCondition.wait();

		value_type tmp = m_seqContainer.front();
		m_seqContainer.pop_front();
		return tmp;
	}

	size_type size() const
	{
		server::xlinux::ScopedLock<server::xlinux::LinuxMutex> lock(m_linxCondition.getLinuxMutex());
		return m_seqContainer.size();
	}

	// 0 success, -1 fail
	int pop_util_empty(value_type& out)
	{
		server::xlinux::ScopedLock<server::xlinux::LinuxMutex> lock(m_linxCondition.getLinuxMutex());
		if (m_seqContainer.empty())
		{
			return -1;
		}
		else
		{
			out = m_seqContainer.front();
			m_seqContainer.pop_front();
			return 0;
		}
	}

	void setQueueLimit(size_type sz)
	{
		m_szQueueLimit = sz;
	}

    size_type getQueueLimit(void)
    {
        return m_szQueueLimit;
    }


protected:
	mutable size_type							m_szQueueLimit;
	SequenceContainer							m_seqContainer;
	mutable server::xlinux::LinuxMutexCondition	m_linxCondition;
};

}}
#endif // SERVER_COMMON___COMMON___TMESSAGEQUEUE_H
