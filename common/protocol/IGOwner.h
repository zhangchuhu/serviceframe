#pragma once

#include <vector>
#include "common/int_types.h"

namespace protocol
{
	namespace gcheck
	{
		//�۲��߽ӿڶ���
		class IGOwnerWatcher
		{
		public:
			virtual ~IGOwnerWatcher() {};

			//��ȡȺӵ����
			virtual void OnGetGrpOwnerRes(
				const uint32_t			&uGrpId,	//ȺID
				const uint32_t			&uOwId		//Ⱥӵ����ID
			) = 0;
		};

		//�۲��߲����ӿڶ���
		class IOpGOwnerWatcher
		{
		public:
			virtual ~IOpGOwnerWatcher() {};

			virtual void watch(IGOwnerWatcher *w) = 0;
			virtual void revoke(IGOwnerWatcher *w) = 0;
		};

		//��ȡȺ��ʼ�˽ӿڶ���
		class IGOwner : public virtual IOpGOwnerWatcher
		{
		public:
			virtual ~IGOwner() {};

			//������ȡȺӵ����
			virtual void BatchGetGrpOwner(const std::vector<uint32_t> &vecGrpId) = 0;
		};

	}
}