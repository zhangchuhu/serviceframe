#ifndef __I_SYS_MSG_H__
#define __I_SYS_MSG_H__

#include <string>
#include "common/unistr.h"
#include "const.h"

namespace protocol{
	namespace sysmsg{



		struct ISysMsgWatcher {
			virtual void onGetSysMsg(
				const SysMsgDisplayType &enDisType,		//显示类型：自动关闭、用户手动关闭
				const UniString &strCatalog,			//系统消息分类
				const UniString &strURL,				//消息链接URL
				const UniString &strTitle,				//系统消息标题
				const UniString& strMsg					//系统消息内容
				) = 0;

			virtual void OnComplainResult(
				const std::wstring &strGuid,	//投诉id
				const uint32_t &uSeqId,			//第几次投诉
				const uint32_t &uChannelId,		//频道id（长位）
				const std::wstring &strResult,	//处理结果
				const std::wstring &strAction,
				const uint32_t &uChannelAid
				) = 0;
		};

		struct ISysMsgWatchBase{
			virtual void watch(ISysMsgWatcher * w) = 0;
			virtual void revoke(ISysMsgWatcher * w) = 0;
		};

		struct ISysMsg : public virtual	ISysMsgWatchBase{
			virtual ~ISysMsg(){}
		};

	}
};


#endif
