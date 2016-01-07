#ifndef __IIMAPPFANS_H__
#define __IIMAPPFANS_H__

#include <string>
#include "common/unistr.h"
#include "const.h"
#include "iImV2.h"
#include "pmsg.h"
#include "iimsearch.h"
#include <exception>

namespace protocol{
	namespace imapp{

		// 联系人属性
		enum ContactProperty {
			FAN = 0,               // 粉丝
			CARE = 1,              // 关注的人
		};

		struct CSlimContactInfo
		{
			uint32_t uid;          // 联系人的uid
			std::string nickname;  // 联系人的昵称
		};

		// 完整的联系人信息
		struct CContactInfo
		{
			uint32_t uid;            // 联系人的uid
			std::string nickname;    // 昵称
			uint32_t head_image_version;       // 头像，小图
			bool vip;               // 是否vip，true表示是vip，false表示不是vip
			bool bAlreadyDetail;	 //是否已经获取过详细信息
		};

		struct IAppJyFansRegister  
		{
			//获取到简略好友列表
			virtual void OnGetSlimContactList(const ContactProperty &type, const std::vector<CSlimContactInfo> &vtData) = 0;

			//获取到详细好友信息
			virtual void OnGetContactInfoList(const std::vector<CContactInfo> &vtData) = 0;

			//添加粉丝回应
			virtual void OnAddRes(const uint32_t &uid, const uint32_t &operationResult) = 0;
			
			//删除粉丝回应
			virtual void OnDelRes(const uint32_t &uid,const uint32_t &operationResult) = 0;

			//被某人关注
			virtual void OnFansAdd(const uint32_t &bid) = 0;
		};

		struct IAppJyFansWatcher//底层需要实现的接口,供UI调用
		{
			virtual ~IAppJyFansWatcher(){};
			virtual void watch(IAppJyFansRegister *w) = 0;
			virtual void revoke(IAppJyFansRegister *w) = 0;
		};


		class IAppJyFansBase : virtual public IAppJyFansWatcher
		{
		public:
			//获取联系人消息响应
			virtual void GetSlimContactList(ContactProperty Type) = 0;

			//获取粉丝详细信息(会存在一次调用,分开两次甚至多次回调的可能,原因是本地如果有缓存的那部分,直接返回,没有缓存的另一部分,服务器获取后再返回)
			virtual void GetContactInfo(const std::vector<uint32_t>& contactIDs) = 0;

			//关注某人
			virtual void AddFans(const uint32_t uPeerId, const std::string& nickname) = 0;

			//不再关注某人
			virtual void DelFans(const uint32_t uPeerId) = 0;

			//同步接口,根据本地缓存,直接返回是否是粉丝或关注的人
			virtual bool IsMyFans(uint32_t uid, ContactProperty eMyType) = 0;
		};


	};
};


#endif //__IIMAPPFANS_H__
