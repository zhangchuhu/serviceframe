#ifndef __I_SYS_MSG_H__
#define __I_SYS_MSG_H__

#include <string>
#include "common/unistr.h"
#include "const.h"

namespace protocol{
	namespace sysmsg{



		struct ISysMsgWatcher {
			virtual void onGetSysMsg(
				const SysMsgDisplayType &enDisType,		//��ʾ���ͣ��Զ��رա��û��ֶ��ر�
				const UniString &strCatalog,			//ϵͳ��Ϣ����
				const UniString &strURL,				//��Ϣ����URL
				const UniString &strTitle,				//ϵͳ��Ϣ����
				const UniString& strMsg					//ϵͳ��Ϣ����
				) = 0;

			virtual void OnComplainResult(
				const std::wstring &strGuid,	//Ͷ��id
				const uint32_t &uSeqId,			//�ڼ���Ͷ��
				const uint32_t &uChannelId,		//Ƶ��id����λ��
				const std::wstring &strResult,	//������
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
