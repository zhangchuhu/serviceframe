#ifndef SERVER_COMMON___COMMON___SCOPEDLOCK_H
#define SERVER_COMMON___COMMON___SCOPEDLOCK_H

namespace server{ namespace xlinux{

	template<typename T>
	class ScopedLock
	{
	public:
		ScopedLock(T& lock) : m_tLock(lock)
		{
			m_tLock.lock();
		}

		~ScopedLock()
		{
			m_tLock.unlock();
		}

	private:
		T& m_tLock;

		ScopedLock();
	};

}}


#endif	// SERVER_COMMON___COMMON___SCOPEDLOCK_H

