#pragma once

#include "common/unistr.h"
#include "common/int_types.h"
#include "common/protocol/PImMsg.h"

namespace protocol
{
	namespace gtransfer
	{

		//Ⱥ��ʱ�Ự�۲��߽ӿڶ���

		class IGChatWatcher
		{
		public:
			virtual ~IGChatWatcher() {};

			//������Ϣ����֪ͨ
			virtual void OnMsgArrive(const uint32_t			&uGrpID,	//ȺID
				const uint32_t			&uSenderId,	//������Ϣ��Ⱥ��Ա
				const protocol::im::ImTextChat&  ImText, //���յ���Ϣ��
				const uint32_t			&uSendTime	//��Ϣ�����������ʱ���
				) = 0;

			//gchat msg timeout
			virtual void OnMsgTimeout(const uint32_t			&uGrpID,	//ȺID
				const uint32_t			&uReceiverId,    //������Ϣ�ĳ�Ա
				const uint32_t 			&uMsgId,         //������Ϣ��ˮ��
				const protocol::im::ImTextChat&  ImText //���յ���Ϣ��
				) = 0;

			//���͵�������Ϣ��Ӧ֪ͨ
			virtual void OnMsgRes(const uint32_t			&uGrpID,	//ȺID
				const uint32_t			&uRecverId,	//���ͻ�Ӧ�Ľ�����
				const uint32_t			&uMsgId		//������Ϣ����ˮ��
				) = 0;

			//�յ�����״̬֪ͨ
			virtual void OnInputStatus(const uint32_t &uSenderId) = 0;	//��������״̬��Ⱥ��Ա
		};

		//�����߲����ӿڶ���

		class IOpGChatWatcher
		{
		public:
			virtual ~IOpGChatWatcher() {};

			virtual void watch(IGChatWatcher *w) = 0;
			virtual void revoke(IGChatWatcher *w) = 0;
		};

		//Ⱥ��ʱ�Ự�ӿڶ���

		class IGChat : public virtual IOpGChatWatcher
		{
		public:
			virtual ~IGChat() {};

			//����������Ϣ
			//ʱ����ɷ���������
			virtual void SendMessage(uint32_t			uGrpID,		//ȺID 
				uint32_t			uRecverId,	//������Ϣ��Ⱥ��Ա
				uint32_t			uMsgId,		//��������Ψһȫ�ֱ�ʶ����Ϣ
				const protocol::im::ImTextChat&	ImText //���͵���Ϣ����
				) = 0;

			//���ʹ���״̬
			virtual void SendInputStatus(uint32_t uRecverId) = 0;	//��������״̬��Ⱥ��Ա
		};

	}
}