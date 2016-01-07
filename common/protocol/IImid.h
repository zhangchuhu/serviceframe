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
			//UIʵ�ֵĽӿ�
			virtual void OnChange(const std::map<uint32_t,uint32_t>& mapData) = 0;
		};

		struct IImIdInfoWatchBase 
		{
			virtual void watch(IImIdInfoWatcher *) = 0;
			virtual void revoke(IImIdInfoWatcher *) = 0;
		};

		//�ײ�ʵ�ֵĽӿ�
		struct IImIdInfo:public virtual IImIdInfoWatchBase
		{
			virtual void GetImIdInfo(const std::vector<uint32_t/*���鿴�˵�ID�б�*/> &vecUid)=0;
		};
	}
}