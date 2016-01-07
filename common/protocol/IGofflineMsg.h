#pragma once


#include <vector>

#include "common/int_types.h"
#include "common/protocol/pimmsg.h"

namespace protocol
{
	namespace gofflinemsg
	{
		struct CGofflineMsgClientData
		{
			uint32_t			m_uGid;		//ȺID����Ϊֻ����Ⱥ�ڲ��ܵ�Ե����죬���Լ��Ƿ��ͷ�Ҳ�ǽ��շ����ڵ�Ⱥ��
			uint32_t			m_uSenderId;	//������Ϣ���û�ID
			protocol::im::ImTextChat m_objImText; //���յ���Ϣ��
			uint32_t			m_uSendTime;	//��Ϣ�����������ʱ���
		};


		struct IGofflineMsgWatcher//UIʵ�ֵĽӿ�
		{
			//ȡ��������Ϣ�����Ļص�
			//������������Ϣ�ķ����ˣ���Ϣ������ʱ�䣬Ⱥ
			//virtual void OnArrive(const HULI& fromid, const protocol::session::TextChat& text,const uint32_t & uSendTime) = 0;
			//virtual void OnArrive(vector<>/*���ܵĽṹ*/ ) = 0;
			virtual void OnArrive(const std::vector<CGofflineMsgClientData*> &vecData) = 0;
			
			//�洢������Ϣ����Ļص�
			//������������Ϣ�Ľ���������Ⱥ��������Ϣ�Ľ����ˣ���Ϣ��ˮ�ţ��洢���
			virtual void OnStore(const uint32_t &uGid, const uint32_t& uToId, const uint32_t & uMsgId, const uint32_t& uReason) = 0 ;

		};

		struct IOpGofflineMsgWatcher
		{
			virtual void watch(IGofflineMsgWatcher *w) = 0;
			virtual void revoke(IGofflineMsgWatcher *w) = 0;
		};

		//�ײ���Ҫʵ�ֵĽӿڣ���UI����
		struct IGofflineMsg:public virtual IOpGofflineMsgWatcher
		{
			virtual void Get()=0;

		};





	}
}