#ifndef __IWAITFORCHECKLIST_H__
#define __IWAITFORCHECKLIST_H__

#include "common/protocol/ilist.h"
#include "common/protocol/const.h"
#include "common/int_types.h"

namespace protocol
{
	namespace im
	{

		struct IWaitCheckList : public protocol::im::IImListBase
		{
			virtual ~IWaitCheckList(){}
			//
			virtual void OpWCBuddy(const HULI& hid, const HULI &gid, E_WAITCHECKLIST_OP op, const UniString& msg) = 0;
			virtual UniString get_Left_Msg(const HULI& hid) const = 0;
			virtual void OpWCBuddy2(uint32_t uBid, uint32_t uBGid,const UniString& strRemark, E_WAITCHECKLIST_OP op, const UniString& strMsg/*如果拒绝的话是拒绝文本*/)=0;

			//add by kdjie, 2010.7.12
			//画中画强制加好友功能，A 强制加 B，B接收后调用本接口强制加A、B为好友
			virtual void AddBuddyForce(
				uint32_t uBid, uint32_t uBGid, //欲添加的好友uid和分组ID，分组ID可以使用DEFAULT_GROUP_ID
				const UniString &strRemarkMySelf, //为好友设置关于我的备注
				const UniString &strRemarkFriend, //为我设置该好友的备注
				const UniString &strMsg) = 0; //操作附加信息
			//add end
		};
	}
}
 
#endif
