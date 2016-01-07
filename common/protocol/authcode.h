#ifndef __DSPEAK_AUTH_CODE_H
#define __DSPEAK_AUTH_CODE_H

// obsolete operation
/*enum XXOpCode{
	CREATE_SUBCHANNEL_CHK,		// ������Ƶ��
	DISSMISS_SUBCHANNEL_CHK,	// ��ɢ��Ƶ��
	CHANGE_INFO,				// ������ƽ����Ϣ
	ADD_MANAGER,				// ��ӹ���Ա
	REMOVE_MANAGER,				// ɾ������Ա
	ADD_MEMBER,					// ��ӳ�Ա
	REMOVE_MEMBER,				// ɾ����Ա
	JOIN_CHANNEL,				// �������Ƽ�Ƶ��
	JOIN_WITHOUTPASSWD,			// ����û������ƽ��
	KICKOFF,					// ����
	VOICE,						// ����
	CHANGESTYLE,				// ����ģʽ
	TUOREN,						// ����
	SETKEYACTIVE,				// ���̼���
	DISABLE_VOICE,				// ��ֹ����
	ENABLE_VOICE,				// ��������
	ENABLE_TEXT,				// ��ֹ�ı�
	DISABLE_TEXT,				// �����ı�
	BROADCAST,					// �㲥
	DRAGETO_QUEUE,				// �Ͻ�����
	JOIN_QUEUE,					// ����	
	LEAVE_QUEUE,				// �˳�����
	MOVEUP_QUEUE,				// ��������
	MOVEDOWN_QUEUE,				// ��������
	KICK_QUEUE,					// �߳�����
	MUTE_QUEUE,					// ��ֹ����
	UNMUTE_QUEUE,				// ��������
	DOUBLETIME_QUEUE,			// ˫������ʱ��
	DISABLE_QUEUE,				// ��������
	ALLOW_QUEUE,				// ��������
	DRAG,						// ����
};*/

namespace protocol{
	namespace session{
		struct  session_auth
		{
			enum auth_code{
					none = 0,
					change_room_owner			=3,				//����Ƶ��������
					room_broadcast				=4,				//��������Ͻ��Ƶ���㲥
					remove_room					=5,				//ɾ��Ƶ��
					edit_room_info				=6,				//�޸�����Ͻ��Ƶ����Ϣ
					add_remove_child_room		=7,				//������ϽƵ���´���/ɾ����Ƶ��
					add_remove_room_member		=8,				//���/ɾ����Ա(R)
					add_remove_room_admin		=9,				//���/ɾ������Ա(MAN)
					add_remove_child_room_admin =10,			//���/ɾ������Ͻ��Ƶ������Ƶ������Ա��CA��
					add_remove_room_vip			=11,			//���/ɾ�����
					add_remove_room_tvip		=12,			//���/ɾ����ʱ���
					add_remove_room_ban_list	=13,			//���/ɾ��������(id/ip)
					kick_user_from_room			=14,			//���û��߳�Ƶ��
					kick_all_user_from_room		=15,			//�������û��߳�Ƶ��
					control_user_voice			=16,			//��ֹ/�����ֹ�û�����
					control_user_text			=17,			//��ֹ/�����ֹ�û�����
					control_room_voice			=18,			//��ֹ/�����ֹ����Ͻ��Ƶ������
					control_room_text			=19,			//��ֹ/�����ֹ����Ͻ��Ƶ������
					exclude_from_member_edit	=20,			//�������/ɾ����Ա
					exclude_from_admin_edit		=21,			//�������/ɾ��Ƶ������Ա
					exclude_from_chairman_mode	=22,			//������ϯģʽ
					exclude_from_vip_edit		=23,			//�������/ɾ�����
					exclude_from_tvip_edit		=24,			//�������/ɾ����ʱ���
					exclude_from_mic_order_mode =25,			//��������ģʽ
					exclude_from_limited_room   =26,			//��������Ƶ��
					exclude_from_voice_ban		=27,			//����������ֹ4
					exclude_from_text_ban		=28,			//�����ı���ֹ
					exclude_from_kick_user		=29,			//��������
					exclude_from_passwd_room	=30,			//��������
					exclude_from_schedule		=31,			//��ѡ��ܾ�����
					exclude_from_banlist		=32,			//���Ӻ�����IP/ID
					exclude_from_queue			=33,			//�����Ŷ�
					exclude_from_count_limit	=34,			//������������
					set_others_voice_by_key		=35,			//����������Ϊ����˵��
					set_room_voice_by_key		=36,			//���õ�ǰƵ��Ϊ��������
					schedule_people				=37,			//������Ա
					change_room_mode			=38,			//���ķ���ģʽ
					change_mic_mode				=39,			//��ֹ/�����ֹ����
					change_mic_order			=40,			//����/��������
					add_remove_from_mic_queue	=41,			//�������߳�ȥ
					//mute_mic_queue				=42,			//��������	
					change_mic_time				=43,			//��������ʱ��
					change_mic_owner			=44,			//����/�������
					remove_room_from_favor		=45,			//��Ƶ�����ղ��Ƴ�
					withdraw_from_guild			=46,			//�˳�����
					room_voice					=47,			//�ܷ�����
					room_text					=48,				//�ܷ������ı�
					schedule_room				=49,			//���ȷ���
					direct_kick_from_room		=50	,			//ֱ���߳�Ƶ��
					exclude_from_room_text_ban = 51,
					add_remove_room_vice_owner  =52             //���/ɾ�����᳤(VO)
				};
		};
	}
}


#endif //__DSPEAK_AUTH_CODE_H

