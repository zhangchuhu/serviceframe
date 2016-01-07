#ifndef SERVER_COMMON___COMMON___SCOPEDDELETE_H
#define SERVER_COMMON___COMMON___SCOPEDDELETE_H

namespace server{ namespace xlinux{

	template<typename T>
	class ScopedDelete
	{
	public:
		explicit ScopedDelete(T p) : m_p(p) 
		{}

		~ScopedDelete()
		{
			if (NULL != m_p)
				delete m_p;
		}

		T& get() 
		{ 
			return m_p; 
		}

		T& operator -> ()
		{
			return m_p;
		}

	private:
		T m_p;

		ScopedDelete();
	};

}}


#endif	// SERVER_COMMON___COMMON___SCOPEDDELETE_H

