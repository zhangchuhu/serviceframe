#pragma once

#include "common/protocol/const.h"
#include <time.h>
#include "common/unistr.h"
#include "common/packet.h"

// added by linyue for testing
#include "common/int_types.h"
#include <string>
// end
namespace protocol{
	namespace im{

		struct IImLoginWatcher {
			//notify login status
			virtual void onNotifyIMLoginResult(const protocol::login::LinkStatus& status) = 0;

			//notify changing status result
			virtual void onChangeStatusRes(const protocol::E_IMSTATUS& status) = 0;

			virtual void onSyncTimeRes(const time_t &uSvrTime) {};
		};

		struct IImLoginWatchBase{
			virtual void watch(IImLoginWatcher * w) = 0;
			virtual void revoke(IImLoginWatcher * w) = 0;
		};

		class IImLogin : public virtual IImLoginWatchBase{
		public:
			virtual void changeStatus(protocol::E_IMSTATUS st) = 0;
			virtual protocol::E_IMSTATUS getImStatus() = 0;
			virtual bool isImUsable() = 0;
			//added by zzw , 20091012
			virtual int GetTimerInterval() = 0;
			virtual void LoginIM() = 0;
			//end

			//add by kdjie 20091105
			virtual void LogoutIM() = 0;

			//获取前端IP(imlink IP)
			virtual void GetImlinkId(UniString &strIp) = 0;

			virtual void SendToPeer(uint32_t uPeerId, uint32_t uURI, const sox::Marshallable &objMsg, bool bSaveOffline = false) = 0;

			virtual void setLoginInfo(uint32_t uid, std::string& cookie, std::string& pwd, std::string& acc, protocol::E_IMSTATUS loginst, ISPType &isp) = 0;

			virtual void login2() = 0;
		};

	   //added by zzw, 20091014
		class IImLBS {
		public:
			virtual int GetLBSInterval() = 0;
		};


		struct IImPeerMsgWatcher {
			virtual void OnPeerMessage(
				const uint32_t &uPeerId,		//消息发送者
				const uint32_t &uURI,			//消息uri
				const std::string &strMsg,		//二进制消息体
				const bool &bIsOffline			//是否离线消息
				) = 0;
		};

		struct IImPeerMsgWatchBase {
			virtual void watch(IImPeerMsgWatcher * w) = 0;
			virtual void revoke(IImPeerMsgWatcher * w) = 0;
		};
	}
}


