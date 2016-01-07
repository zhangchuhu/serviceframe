#pragma once


#include <vector>
#include <map>
#include "common/int_types.h"

namespace protocol
{
	namespace im
	{
		struct IImIdInfoWatcher
		{
			//UI实现的接口
			virtual void OnChange(const std::map<uint32_t,uint32_t>& mapData) = 0;
		};

		struct IImIdInfoWatchBase 
		{
			virtual void watch(IImIdInfoWatcher *) = 0;
			virtual void revoke(IImIdInfoWatcher *) = 0;
		};

		//底层实现的接口
		struct IImIdInfo:public virtual IImIdInfoWatchBase
		{
			virtual void GetImIdInfo(const std::vector<uint32_t/*被查看人的ID列表*/> &vecUid)=0;
		};
	}
}