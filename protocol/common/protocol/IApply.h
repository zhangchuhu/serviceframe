#ifndef IApply_h__
#define IApply_h__
#include "common/protocol/const.h"
#include "ChannelRoler.h"
namespace protocol{
	namespace uinfo{
		//频道的ID，频道的昵称，批准人的userID,批准人的昵称，角色，同意否  
		struct ApplyPassInfo{
			std::wstring sid;
			std::wstring asid;
			std::wstring channelName;
			std::wstring adminUid;
			std::wstring adminNick;
			protocol::session::ChannelRoler role;
			bool		 bPass;
		};
	
		struct IApplyResultWatcher{
			virtual void onApplyPassResult(const ApplyPassInfo &) = 0;
		};

		struct IApplyResultWatchBase{
			virtual void watch(IApplyResultWatcher *w) = 0;
			virtual void revoke(IApplyResultWatcher *w) = 0;
		};
		
		struct IApplyWatcher{
			/*
				用法：
				UI应该先保存当前申请的频道sid，在回调到来时和回传的sid相比较，
				如果不等，说明不是这个频道的的回调应予以丢弃！！
			*/
			virtual void onApply(const std::wstring &sid, ApplyResult res)= 0;
			virtual void onPrepareCheck(const std::wstring &sid, ApplyResult res)= 0;
		};

		struct IApplyGuildWatchBase{
			virtual void watch(IApplyWatcher *w) = 0;
			virtual void revoke(IApplyWatcher *w) = 0;
		};

		//该接口用于频道中申请入会
		struct IApplyGuild: public IApplyGuildWatchBase{
			virtual ~IApplyGuild(){}
			virtual void apply(const std::wstring &sid, const std::wstring &reason) = 0;
			virtual void prepareCheck(const std::wstring  &sid) = 0;

			virtual void watchResult(IApplyResultWatcher *w) = 0;
			virtual void revokeResult(IApplyResultWatcher *w) = 0;
		};

		struct ApplyGuildInfo{
			std::wstring sid;
			std::wstring asid;
			std::wstring name;
			std::wstring applyReason;
		};

		struct IApplyGuildListWatcher{
			virtual void onRefresh(const std::vector<ApplyGuildInfo> &) = 0;
		};

		struct IApplyGuildListWatchBase{
			virtual void watch(IApplyGuildListWatcher *) = 0;
			virtual void revoke(IApplyGuildListWatcher *) = 0;
		};
		//先做成注销再取
		//该接口用于显示申请列表
		struct IApplyGuildList: public IApplyGuildListWatchBase{
			virtual ~IApplyGuildList(){}
			//等待服务器返回，永远成功
			virtual void abandon(const std::wstring &sid) = 0;
			//触发服务器获取申请列表
			virtual void sync() = 0;
		};

	}
}
#endif // IApply_h__