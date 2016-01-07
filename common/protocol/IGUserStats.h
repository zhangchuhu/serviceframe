#pragma once 

#include "common/unistr.h"
#include "common/int_types.h"
#include "common/protocol/const.h"

namespace protocol
{
	namespace glistcache
	{

		//Ⱥ��Ա״̬�ı�֪ͨ�ӿ�

		class IGUserStatsWatcher
		{
		public:
			virtual ~IGUserStatsWatcher() {};

			//�����û�״̬֪ͨ

			virtual void OnGetGUserStats(const std::map<uint32_t,protocol::E_IMSTATUS> &mapGUserStats) = 0;

			//�����û�״̬֪ͨ

			virtual void OnGUserStatChange(//const uint32_t				&uGrpID,	//ȺID
										   const uint32_t				&uUid,		//�û�ID
										   const protocol::E_IMSTATUS	&emState	//�û�״̬
										   ) = 0;
		};

		//�����߲����ӿڶ���

		class IOpGUserStatsWatcher
		{
		public:
			virtual ~IOpGUserStatsWatcher() {};

			virtual void watch(IGUserStatsWatcher *w) = 0;
			virtual void revoke(IGUserStatsWatcher *w) = 0;
		};

		//Ⱥ��Ա״̬�ı�ӿڶ���

		class IGUserStats : public virtual IOpGUserStatsWatcher
		{
		public:
			virtual ~IGUserStats() {};

			//���������û���״̬

			virtual void GetGUserStats(const std::vector<uint32_t> &vecUid) = 0;
		};

	}
}