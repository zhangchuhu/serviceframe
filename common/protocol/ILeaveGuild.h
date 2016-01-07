#ifndef ILeaveGuild_h__
#define ILeaveGuild_h__
#include "ChannelRoler.h"
#include "common/int_types.h"

namespace protocol{
	namespace session{
		struct LeaveGuildInfo{
			std::wstring name;
			std::wstring sid;
			std::wstring asid;
			ChannelRoler oldRoler;
			uint32_t jifen;
			time_t leaveTime;			
			time_t clearJifenTime;
			std::wstring adminOldName;		
			bool isAutoLeave;				//�������뿪���Ǳ�����Ա�ߵ�
		};

		struct ILeaveGuildWatcher{
			virtual void onRefresh(const std::vector<LeaveGuildInfo> &) = 0;
		};

		struct ILeaveGuildWatchBase{
			virtual void watch(ILeaveGuildWatcher *) = 0;
			virtual void revoke(ILeaveGuildWatcher *) = 0;
		};
		//����Ѿ�ȥ���������ٴӷ�����ȡ��ֻ����ע����Ż���ȥȡ��
		struct ILeaveGuild: public ILeaveGuildWatchBase{
			virtual ~ILeaveGuild(){}
			//����б�
			virtual void clear() = 0;
			//���øýӿ�����һ�η�����ͬ��
			virtual void sync() = 0;
		};
	}
}
#endif // ILeaveGuild_h__