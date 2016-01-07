#ifndef ID_H_20121225
#define ID_H_20121225
#include <stdint.h>
#include <assert.h>

namespace xlinux
{
	// 32bit (timestamp) + 20bit(ms * 1000 + us) + 12bit(external_id, < 4096)
	class IdGenerator
	{
	public:
		IdGenerator(uint64_t u64ExternalId)
			: m_u64LastT(0)
			, m_u64ExternalId(u64ExternalId)
		{
			if (m_u64ExternalId >= 4096)
			{
				printf("u64ExternalId>=4096\n");
				assert(false);
				exit(-1);
			}
		}

		uint64_t nextId()
		{
			uint64_t u64CurT = 0;
			do 
			{
				u64CurT = get_cur_sec_us_time();
			}
			while (u64CurT <= m_u64LastT);	

			uint64_t u64Ret = (u64CurT << 12) + m_u64ExternalId;
			m_u64LastT = u64CurT;
			return u64Ret;
		}

		// 32bit (timestamp) + 20bit(ms * 1000 + us)
		uint64_t get_cur_sec_us_time()
		{
		    struct timeval tv;
		    gettimeofday(&tv, NULL);
		    uint64_t u64Ret = ((uint64_t)tv.tv_sec << 32) + ((uint64_t)tv.tv_usec << 12);
		    return  u64Ret;
		}



		uint64_t m_u64LastT;
		uint64_t m_u64ExternalId;
	};
}

#endif
