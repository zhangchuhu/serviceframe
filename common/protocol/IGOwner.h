#pragma once

#include <vector>
#include "common/int_types.h"

namespace protocol
{
	namespace gcheck
	{
		//观察者接口定义
		class IGOwnerWatcher
		{
		public:
			virtual ~IGOwnerWatcher() {};

			//获取群拥有者
			virtual void OnGetGrpOwnerRes(
				const uint32_t			&uGrpId,	//群ID
				const uint32_t			&uOwId		//群拥有者ID
			) = 0;
		};

		//观察者操作接口定义
		class IOpGOwnerWatcher
		{
		public:
			virtual ~IOpGOwnerWatcher() {};

			virtual void watch(IGOwnerWatcher *w) = 0;
			virtual void revoke(IGOwnerWatcher *w) = 0;
		};

		//获取群创始人接口定义
		class IGOwner : public virtual IOpGOwnerWatcher
		{
		public:
			virtual ~IGOwner() {};

			//批量获取群拥有者
			virtual void BatchGetGrpOwner(const std::vector<uint32_t> &vecGrpId) = 0;
		};

	}
}