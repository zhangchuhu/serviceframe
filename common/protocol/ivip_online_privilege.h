#pragma once

#include "const.h"
#include "common/int_types.h"


namespace protocol
{
	namespace vip
	{
		enum EOnlinePriOp
		{
			ENUM_SET_HIDE_TO = 0,        // �������߶�������
			ENUM_SET_UNHIDE_TO,          // �����������ɼ�
			ENUM_UNSET_HIDE_TO,          // ȡ�����߶�������
			ENUM_UNSET_UNHIDE_TO         // ȡ���������ɼ�
		};

		enum EOnlinePriResCode
		{
			ENUM_ONLINE_PRI_OK = 0,      // ���óɹ�
			ENUM_ONLINE_PRI_OVERLIMIT,   // ����������������
			ENUM_ONLINE_PRI_NOT_VIP,     // �û���vip
			ENUM_ONLINE_PRI_DBERR
		};

		class IVipOnlineWatcher
		{
		public:

			virtual ~IVipOnlineWatcher() {}

			// �������߶������� / �������ɼ��� ��Ӧ
			virtual void onSetOnlinePrivilegeRes(const uint32_t& uBuddyId, const EOnlinePriOp& emOp, const EOnlinePriResCode& res_code) = 0;

			// ȡ�������ù��ĺ����б�
			// @uMaxNum  �û���ǰ�ȼ��ɹ�ע����������
			virtual void onGetOnlinePirvilege(const std::set<uint32_t>& hideList, const std::set<uint32_t>& unHideList, const uint32_t& uMaxNum) = 0;
		};

		class IVipOnlineWatcherBase
		{
		public:

			virtual ~IVipOnlineWatcherBase() {}
			virtual void watch(IVipOnlineWatcher *w) = 0;
			virtual void revoke(IVipOnlineWatcher *w) = 0;
		};

		class ICVipOnlinePrivilege : public virtual IVipOnlineWatcherBase
		{
		public:
		
			// set online privilege
			virtual void setOnlinePrivilege(uint32_t uBuddyId, EOnlinePriOp emOp) = 0;
		};
	}
}

