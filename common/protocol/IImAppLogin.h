#ifndef  I_IM_MAKE_FRIEND_LOGIN_H
#define  I_IM_MAKE_FRIEND_LOGIN_H

#include "common/protocol/const.h"
#include "common/packet.h"

namespace protocol
{
	namespace imapp
	{
		class IAppLoginRegister
		{
		public:

			IAppLoginRegister(const uint32_t uAppId)
			{
				//ASSERT(uAppId >= ENUM_APP_MAKE_FRIEND);
				m_uMyAppId = uAppId;
			}

			// 登陆成功回调
			virtual void OnAppLogin(const protocol::E_IMSTATUS& emStatus) = 0;

			// 改变状态回调
			// emStatus == IMOFFLINE 为登出
			virtual void OnAppStatusChange(const protocol::E_IMSTATUS& emStatus) = 0;

			// 获取app用户在线状态
			virtual void OnAppBatchGetStatusRes(const std::map<uint32_t, protocol::E_IMSTATUS>& mapUid2St) = 0;

			uint32_t GetMyAppId() const { return m_uMyAppId; } 

		private:

			uint32_t m_uMyAppId;
		};
		
		class IAppLoginWatcherBase
		{
		public:
			virtual void watch(IAppLoginRegister * w) = 0;
			virtual void revoke(IAppLoginRegister * w) = 0;
		};

		class IAppLogin : public IAppLoginWatcherBase
		{
		public:
			
			// 登陆
			virtual void AppLogin(const IAppLoginRegister* p, protocol::E_IMSTATUS emSt = protocol::IMONLINE) = 0;

			// 登出
			virtual void AppLogout(const IAppLoginRegister* p) = 0;

			// 保留接口，获取登陆状态
			virtual protocol::E_IMSTATUS AppGetMyStatus(const IAppLoginRegister* p) = 0;

			// 获取粉丝在线状态
			virtual void AppBatchGetStatusByUid(const IAppLoginRegister* p, const std::vector<uint32_t>& vecUids) = 0;
		};
	}
}

#endif
