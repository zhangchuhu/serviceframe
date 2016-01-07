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
			virtual void OpWCBuddy2(uint32_t uBid, uint32_t uBGid,const UniString& strRemark, E_WAITCHECKLIST_OP op, const UniString& strMsg/*����ܾ��Ļ��Ǿܾ��ı�*/)=0;

			//add by kdjie, 2010.7.12
			//���л�ǿ�ƼӺ��ѹ��ܣ�A ǿ�Ƽ� B��B���պ���ñ��ӿ�ǿ�Ƽ�A��BΪ����
			virtual void AddBuddyForce(
				uint32_t uBid, uint32_t uBGid, //����ӵĺ���uid�ͷ���ID������ID����ʹ��DEFAULT_GROUP_ID
				const UniString &strRemarkMySelf, //Ϊ�������ù����ҵı�ע
				const UniString &strRemarkFriend, //Ϊ�����øú��ѵı�ע
				const UniString &strMsg) = 0; //����������Ϣ
			//add end
		};
	}
}
 
#endif
