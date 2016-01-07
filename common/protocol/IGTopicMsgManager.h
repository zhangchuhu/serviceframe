#ifndef IGTOPICMSG_MANAGER_H
#define IGTOPICMSG_MANAGER_H

//#include <vector>
#include "common/protocol/PGTopicMsg.h"

namespace protocol
{
	namespace gmsgcache
	{
		class IGTopicMsgManagerWatcher
		{
		public:
			// 删除某成员在群或组的所有言论(全部主题和回复)的动作的响应
			// rescode : 如果成功,返回 RES_SUCCESS
			//			 否则为其他 RES_ERETRY	
			// vecFids为 UpdateGMemberMsgs 传入的fid(s)参数
			//   如果vecFids为空，说明删除的是群及其下所有分组下的言论(*当前仅此情形)
			//   如果不为空，则说明删除的是gid->fid(s)下的所有言论
			virtual void OnUpdateGMemberMsgsRes(const uint32_t& rescode,	  // response code
									const uint32_t& uid,         		  	  // 删除动作的操作对象
									const uint32_t& groupId,                  // 待删除对象所在群
									const std::vector<uint32_t>& vecFids) = 0;// 待删除对象所在组

			// 删除某成员在群(的所有言论(全部主题和回复)的动作超时
			// 	vecFids为 UpdateGMemberMsgs 传入的fid(s)参数
			//   如果vecFids为空，说明删除的是群及其下所有分组下的言论(*当前仅此情形)
			//   如果不为空，则说明删除的是gid->fid(s)下的所有言论
			virtual void OnGMemberUpdateMsgsTimeout(const uint32_t& uid,     // 删除动作的操作对象
									const uint32_t& groupId,                 // 被删除对象所在群
									const std::vector<uint32_t>& vecFids) = 0;	// 被删除对象所在组									
		};

		class IOpGTopicMsgManager
		{
		public:

			virtual void watch(IGTopicMsgManagerWatcher *w) = 0;

			virtual void revoke(IGTopicMsgManagerWatcher *w) = 0;
		};

		class IGTopicMsgManager : public virtual IOpGTopicMsgManager
		{
		public:

			virtual ~IGTopicMsgManager() {}

			// 向服务器请求 删除某成员在群或组的所有言论(全部主题和回复)，目前仅群(!)管理员可以操作。
			// uid: 待删除对象uid,
			// groupId: 成员所在群ID,
			// vecFids: 成员所在组ID, 设成vector主要是考虑到将来扩展。
			// 	1) 如果仅删除uid在gid下某个fid分组的言论，就填入单个fid
			//  2) 如果需要删除uid在gid下多个fid分组的言论, 就填入多个fid(s)
			//  3) (*目前仅要求此种情况)如果需要删除uid在gid下(包括gid所有分组fid(s))的所有言论, vecFids请置空！
			// 
			// text: 删除回复时用，客户端期望删除之后原回复所显示的内容。
			virtual void UpdateGMemberMsgs(uint32_t uid, uint32_t groupId, const std::vector<uint32_t>& vecFids, 
												const protocol::gmsgcache::GTopicTextChat& text) = 0;
		};		
	}
}

#endif
