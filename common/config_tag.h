#pragma once

namespace conf
{
	enum LocalCache
	{
		tag_SessionListCache = 0,
		tag_CataLogListCache
	};

	enum cf_tag_login
	{
		tag_lsrecentlist	= 10,
		tag_lsrecentuid,
		tag_lsisptype,
		tag_lsautologin = 20, 
		tag_cookie, 
		tag_imLoginStatus,
		tag_networkproxy
	};

	enum mysetcfg
	{	//������tagһ�ɺ�壬��Ȼ������
		tag_basesysinfo = 100,
		tag_talksysinfo,
		tag_talkmouseinfo,
		tag_talkkey,
		tag_session,
		tag_sessionpos,
		tag_talksnd,
		tag_mic_agc,
		tag_mic_enhance,
		tag_sessionulistwidth,
		tag_defaultinputdevice,
		tag_defaultoutputdevice,
		tag_managershow,
		tag_managerheight,
		tag_microphonelistheight,
		tag_messageinputheight,
		tag_bulletinClosedChannels,
		tag_sendmsguserctrladdenter,
		tag_JoinAndLeaveTips,
		tag_privateScreen,
		tag_skin,
		tag_privateChatSound,
		tag_DockWnd,
		tag_ImBaseHotKey,
		tag_ImInfoRemainder,
		tag_input_vol,
		tag_output_vol,
		tag_imSkin,
		tag_showRecentBuddyList,
		tag_showBackList,
		tag_LastPhotoChoose,
		tag_systemSound,
		tag_maxSoundFirst,
		tag_haveCheckSoundCard,
		tag_lastSkin,                        //�������һ�ε�½��Ƥ����
		tag_closeSound,					     //�ر���������
		tag_forbidInviteToChannel,			 //��ֹ���������Ƶ��
		tag_kalaokmixkind,					 //ʹ�����ֻ���ģʽ���п���OK	
		tag_reverbenabled,
		tag_reverbpreset,
		tag_reverbinputvolume1,
		tag_reverbvolume1,
		tag_isshowaliaseidmsg,
		tag_isshowonlinebuddy,     			// ��ʾ���ߺ���
		tag_shareInfo,						// ������Ϣ
		tag_soundHotKey,					//�������ص��ȼ�
		tag_showNickAndRemark,				// ��ʾ�ǳƺͱ�ע
		tag_micBoostScale,
		tag_micBoostKind,
		tag_IMMessageNoice,					//IM��Ϣ�����ѷ�ʽ
		tag_micDenoise1,
		tag_audioSafeMode1,
		tag_usePLC1,
		tag_haveAutoSetUsePLC1,
		tag_broadcastReceiverRoler,
		tag_imTransmitFilePath,
		tag_pipstartinfo,					// 0Ϊ�����ã�1Ϊ����
		tag_micDenoise2,
		tag_audioSafeMode2,
		tag_usePLC2,
		tag_haveAutoSetUsePLC2,
		tag_newgamereport,					//����Ϸ����
		tag_setPrivateChatAutoReply,	// 0��ʾû������˽���Զ��ظ���1��ʾ����
		tag_idxPrivateChat,				// ��¼��һ���Զ��ظ�
		tag_reverbinputvolume2,
		tag_reverbvolume2,
		tag_micSelectIdx,				//���õ���˷����
		tag_stereoMode,					//������ģʽ
		tag_lastSkin2,                    //�������һ�ε�½��Ƥ��(�µ�ʵ��)��
		tag_imvoice,							// im�����ؼ�����
		tag_shopnewsText,				//�̳�������Ϣ
		tag_clickKalaokBtnOperation,	//���Kalaok��ťʱ���еĲ���
		tag_playerPath,					//��ʹ�õĲ�����·��
		tag_haveShowSelectPlayerWnd,	//�Ƿ�򿪹�ѡ�񲥷�������
		tag_newAudioDeviceNotifyWays,		//������Ƶ�豸ʱ�Ĳ�����0ʼ����ʾ��1��ʼ��ʹ�����豸������ʾ��2�ǲ�ʹ�����豸Ҳ����ʾ
		tag_voiceChangeMode,			//����ģʽ
		tag_toneColor,					//��ɫ
		tag_exquisite,					//��Ϳ���
		tag_voiceEffectEnable,			//��Ч����
		tag_previousinputdevice,
		tag_previousoutputdevice
	};

	enum maintag
	{
		tag_windowrect = 200
	};

	enum cf_tag_session
	{
		tag_smsgfontdef = 300,			//Ƶ�����ڵ�����
		tag_smsinputvol,
		tag_smsoutputvol,
		tag_smsgfontyychat				//YY���촰�ڵ�����
	};

	// ��ס�ϴι���Ա����ʱ�Ķ���
	enum cf_tag_ban
	{
		tag_ban_id = 400,
		tag_ban_ip
	};
}
