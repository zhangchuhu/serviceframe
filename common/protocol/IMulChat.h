#pragma once 

#include "common/unistr.h"
#include "common/int_types.h"
#include "common/protocol/const.h"

#include "pmsg.h"
#include "PMulChatDef.h"

#include <iostream>
#include <string>
#include <vector>

namespace protocol
{
	namespace im
	{

		struct IMulChatWatcher
		{
			virtual ~IMulChatWatcher() {};

			//-----------------------------------------------------------------------------------------------

			//֪ͨUI�������˻Ự�Ľ�����ɹ���ʧ�ܰ汾��
			//���۳ɹ�/ʧ�ܣ�protocol�ڲ��������PCS_ApplyCreateMulChatRes�������û�ִ��PCS_BuddyStatusChange_toClinet״̬��������
			//protocol�����ж�Ҳ���ܵ��ø�OnCreateMulChatRes�ӿ�

			virtual void OnCreateMulChatRes(
				const uint64_t &uKey,							//����ʱ������KEY��ԭ������
				const EMULCHAT_ERRCODE &uResCode				//������
				) = 0;
			virtual void OnCreateMulChatRes(
				const uint64_t &uKey,							//����ʱ������KEY��ԭ������
				const EMULCHAT_ERRCODE &uResCode,				//�����룬ENUM_MULCHAT_SUCCESS��ʾ�ɹ�
				const std::wstring &wstrGuid,					//���صĻỰΨһ��ʶ��ʧ��ʱ�ò�����Ч
				const std::vector<SWideUserItem> &vecInvitesOnline,	//���ߵ��û��б�
				const std::vector<SWideUserItem> &vecInvitesOffline	//���ߵ��û��б�
				) = 0;

			//֪ͨUI�����û�����Ľ�����ɹ���ʧ�ܰ汾��
			//���۳ɹ�/ʧ�ܣ�protocol�ڲ��������PCS_ApplyInviteMulChatRes�������û�ִ��PCS_BuddyStatusChange_toClinet״̬��������

			virtual void OnInviteMembersRes(
				const std::wstring &wstrGuid,					//�ỰΨһ��ʶ
				const EMULCHAT_ERRCODE &uResCode				//������
				) = 0;
			virtual void OnInviteMembersRes(
				const std::wstring &strGuid,					//�ỰΨһ��ʶ
				const EMULCHAT_ERRCODE &uResCode,				//�����룬ENUM_MULCHAT_SUCCESS��ʾ�ɹ�
				const std::vector<SWideUserItem> &vecInvitesOnline,	//���ߵ��û��б�
				const std::vector<SWideUserItem> &vecInvitesOffline	//���ߵ��û��б�
				) = 0;

			//֪ͨUI�Լ������������˻Ự

			virtual void OnInviteMySelf(
				const std::wstring &wstrGuid,					//�ỰΨһ��ʶ	
				const SWideUserItem &stInviter,						//������
				const std::vector<SWideUserItem> &vecMembers		//��ǰ�ĳ�Ա�б�
				) = 0;

			//֪ͨUI�������³�Ա

			virtual void OnMembersJoin(
				const std::wstring &wstrGuid,					//�ỰΨһ��ʶ	
				const SWideUserItem &stInviter,						//������
				const std::vector<SWideUserItem> &vecInvites,		//�����ӵ��û��б�
				const bool &bCreate									//�Ƿ��ʹ��������־
				) = 0;

			//֪ͨUI�û��뿪�Ự

			virtual void OnMembersLeave(
				const std::wstring &wstrGuid,					//�ỰΨһ��ʶ
				const SWideUserItem &stMember						//�뿪���û�
				) = 0;

			//���û��� < 3 ʱ��֪ͨUIɾ�����˻Ự�����л���IMģʽ

			virtual void OnDelMulChat(
				const std::wstring &wstrGuid,					//�ỰΨһ��ʶ
				const SWideUserItem &stOther						//�л���IMģʽ�Ľ�����
				) = 0;

			//���û�����Ϣʱ����������⵽���û����ߣ���֪ͨUI
			//protocol�ڲ���ִ��PCS_BuddyStatusChange_toClinet״̬��������

			virtual void OnMemberOffline(
				const std::wstring &wstrGuid,					//�ỰΨһ��ʶ
				const std::vector<SWideUserItem> &vecMembersOffline	//���ߵ��û��б�
				) = 0;

			//�Ự/��Ա�ָ�

			virtual void OnMemberResume(
				const std::wstring &wstrGuid,					//�ỰΨһ��ʶ
				const std::vector<SWideUserItem> &vecMembers		//���ӵĻָ��ĳ�Ա�б�
				) = 0;

			//-----------------------------------------------------------------------------------------------

			//֪ͨUI�յ����˻Ự����Ϣ

			virtual void OnMulChatMessage(
				const std::wstring &wstrGuid,					//�ỰΨһ��ʶ
				const SWideUserItem &stFrom,
				const protocol::session::TextChat& text,		//��Ϣ����
				const uint32_t& uSendTime,						//��Ϣ����ʱ��
				const uint32_t& uSeqId,							//��Ϣ���к�

				const std::wstring &wstrMsgGuid					//��ϢGUID��s*1000000+us����ʾ��
				) = 0;

			//֪ͨUI�յ����˻Ự����Ϣ��Ӧ

			virtual void OnMulChatMessageRes(
				const std::wstring &wstrGuid,					//�ỰΨһ��ʶ
				const SWideUserItem &stFrom,						//��Ϣ��Ӧ��
				const uint32_t &uSeqId							//��Ϣ���к�
				) = 0;

			//���˷�����Ϣ�󣬷������᷵�ظ���Ϣ��GUID��s*1000000+us����ʾ��

			virtual void OnMulChatMessageGuidRes(
				const std::wstring &wstrGuid,					//�ỰΨһ��ʶ
				const uint32_t &uSeqId,							//��Ϣ���к�
				const uint32_t& uSendTime,						//��Ϣ����ʱ��
				const std::wstring &wstrMsgGuid					//��ϢGUID��s*1000000+us����ʾ��
				) = 0;

			//-----------------------------------------------------------------------------------------------

			//֪ͨUI���ĻỰ����

			virtual void OnSetMulChatTopic(
				const std::wstring &wstrGuid,					//�ỰΨһ��ʶ
				const std::wstring &wstrTopic					//�Ự����
				) = 0;

			//�յ���Ա��״̬�ı�
			virtual void OnMulChatChangeStatus(
				const std::wstring &wstrGuid,					//�ỰΨһ��ʶ
				const uint32_t &uMemberId,						//��Աid
				const protocol::E_IMSTATUS &emStatus			//״̬
				) {};

			//�����ʹ��protocol��ά�����˻Ự��ʱ��
			//���Լ����Ժ󣬻��յ����н����˵���Ϣ��Ӧ�������ʱ���ղ�����Ӧ�Ļ������ϻص�����
			virtual void OnMulChatMessageFail(
				const std::wstring &wstrGuid,					//�ỰΨһ��ʶ
				const SWideUserItem &stUser,					//��Ӧ��ʱ���û�
				const protocol::session::TextChat& objText,		//��Ϣ������
				const uint32_t &uSeqId							//��Ϣ�����к�
				) {};
		};

		struct IMulChatWatcherBase
		{
			virtual ~IMulChatWatcherBase() {};

			virtual void watch(IMulChatWatcher *w) = 0;
			virtual void revoke(IMulChatWatcher *w) = 0;
		};

		class IMulChatManager : public virtual IMulChatWatcherBase
		{
		public:
			virtual ~IMulChatManager() {};

			//���ñ����ǳ�
			//�������ǳƷ����ı����Ҫ�ٴ�����
			virtual void SetMyNick(
				const std::wstring& wstrMyNick
				) = 0;

			//�������˻Ự
			virtual void CreateMulChat(
				uint64_t uKey,									//�����ĸ�����KEY�����ڴ����ɹ���ԭ������
				const std::vector<SWideUserItem> &vecInvites		//����ʱ��������û��б��������Լ����������������û�
				) = 0;

			//��ָ���Ķ��˻Ự��ӳ�Ա
			virtual void InviteMembers(
				const std::wstring& wstrGuid,					//�ỰΨһ��ʶ
				const std::vector<SWideUserItem> &vecInvites		//��������û��б��������Լ����������������û�
				) = 0;

			//�뿪���˻Ự�����ڹر�YYǰ�����ñ��ӿڣ�
			virtual void LeaveMulChat(
				const std::wstring& wstrGuid					//�ỰΨһ��ʶ
				) = 0;

			//���Ͷ��˻Ự��Ϣ
			virtual void SayToMembers(
				const std::wstring& wstrGuid,					//�ỰΨһ��ʶ
				const protocol::session::TextChat& text,		//��Ϣ����
				uint32_t uSeqId									//��Ϣ���к�
				) = 0;

			//��������
			virtual void SetTopic(
				const std::wstring& wstrGuid,					//�ỰΨһ��ʶ
				const std::wstring& wstrTopic					//����
				) = 0;

			//��������Ա�㲥�Լ�״̬
			virtual void ChangeMyStatus(
				const std::wstring& wstrGuid,					//�ỰΨһ��ʶ
				const protocol::E_IMSTATUS& emStatus			//״̬
				) = 0;

			//ȡ������Ա������״̬
			virtual void GetMemberStatus(
				const std::wstring& wstrGuid					//�ỰΨһ��ʶ
				) = 0;
		};
	}
}
