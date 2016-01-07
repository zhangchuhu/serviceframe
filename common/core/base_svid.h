#ifndef BASE_SVID_PROTOCOL_H_
#define BASE_SVID_PROTOCOL_H_
#include <string>

namespace protocol{
		enum svid {
			BASEAPP_SVID  = 0,  // ????????,???????????????????????????
			////////////////////////////////////////////////////////////////////////////
			// basic            // ???? ???               locate-hash
			LBS_SVID   = 1,  // lbs
			SESSION_SVID  = 2,  // 10 ???????????     eSession
			UINFO_SVID    = 3,  // 10 ????????           eHashUid???????????uid
			LINKD_SVID = 4,		//02 ??????   eHashUid
			CLIENT_SVID =5,
			SLIST_SVID  = 6,
			SDB_SVID = 7,    //session db
			SMANAGER_SVID = 8,
			ONLINE_SVID = 9,
			UDB_SVID = 10,
			ROUTE_SVID = 11,
			GVERIFYCODE_SVID = 12,
			CATALOG_SVID = 13,
			SMDISPATCHER_SVID = 14,
			DUPSESSION_SVID = 15,
			ROAM_SVID = 16,
                        //added by zhouweifang , 2011-10-17
                        INVITEDB_SVID = 17,
                        //ended by zhouweifang
		
			
			DAEMON_SVID = 18,
			ECHO_SVID = 19,	//echod
			GAMELIST_SVID = 20,

			ONLINEDB_SVID = 22,
			IMLINKD_SVID = 23,
			IMONLINED_SVID = 24,
			IMCHATD_SVID = 25,
			SYSMSGD_SVID = 26,
			IMIDD_SVID=27,
			VERIFYCODE_SVID=28,
			IMDB_SVID = 29,
			IMLBS_SVID = 30,
			//added by zzw ,20090909
			IMOFFLINEMSG_SVID = 31,

			IMCHAT2D_SVID = 32,	//IMCHAT  V2.0?��???

			IMSEARCH_SVID = 33,
			IMHASHSET_SVID = 34,
			IMAPP_SVID = 35,//imapp ʹ��
			//end

			// add by grj 20091123
			// end
			//add by kdjie, 2009.11.23
			IMTRANS_SVID = 36, //imtransfer???
			IMONLINECACHE_SVID = 37,

			IMUINFO_SVID=38,
			IMGETIMID_SVID=39,

			IMONDISP_SVID = 40, //imondispatcher???
			IMONLINE2_SVID = 41, //imonline2???
			IMGAMEPRO_SVID = 42, // imgamefarm ʹ��

			IMBRIDGE_SVID=43,    // imbridge

			IMONINDEX_SVID=44,   // imonindex
			IMONBRIDGE_SVID=45,  // imonbridge

      GUANBO_SVID = 46, // guang bo

	  		NOTIFYSERVER_SVIDI=47,
			TIPSSERVER_SVID=48,
			THRIFTPROXY_SVID=49,    //lg_userinfo_d:infor trans
			
			SESSCACHE_SVID = 50,

			GTRANS_SVID = 51, //gtransfer???
			GMSGCACHE_SVID = 52, //gmsgcache???
			GLISTCACHE_SVID = 53, //glistcache???
            APPSESSION_SVID = 54, //session appserver ???

			ACCESS_SVID = 55,			//�洢/����������ʹ��
            IMCACHE_SVID = 56,

			//add by lijin
			PAY_SVID = 58, //pay server
			GCHAT_SVID = 57, //gchat_d
			GCHATSTAT_SVID = 64,
			//lijin end

			LOCATION_SERVER_SVID = 68,

			GLIST_SVID = 69, // for groupList. add by luowl

			//Added by Linyue
			//Below are SVIDs for gourp system
			GINFO_SVID = 70, //SVID for ginfo_d process

			GTOPIC_SVID = 71, //SVID for gtopic_d process

			GSEARCH_SVID = 72, //SVID for gsearch_d process

			GPROPERTY_SVID = 73,
			//add by maxn
			GPROPCACHE_SVID = 74, 

			GTOPICCACHE_SVID = 75, // SVID for gtopiccache_d process

			GROUPID_SVID = 76, //SVID for Group_d process
			
			IMGENERALAPP_SVID = 59,

			//wuji start
			GOFFLINE_SVID=60,
			GOFFLINEMSG_SVID=61,
			GMEMBERINFO_SVID=62,
			GMEMBERINFOCACHE_SVID=63,
			//wuji end


			//add by long
			GCHECK_SVID = 80,	//gcheck_d
			HASHPROXY_SVID = 81,	//hashproxy_d
			IMLOCALCACHE_SVID = 82, //imlocalcache_d
			//long end

			GAUTH_SVID = 83,
			IMROUTER_SVID = 84,

			IPLIST_SVID = 85,
			PIP_SVID = 86,
			CANODE_SVID = 87, //Session Cache Node
			SERVICE_SVID = 88, // Service, Service Manager
			ONLINEAUX_SVID = 89,
			ONLINEMISC_SVID = 90,

//			IMBDOOR_SVID = 252, //ImBDoor����

			//long add for p2p
			P2P_SVID = 91,
			P2PBRIDGE_SVID = 92,

			//wuji start
			SESSION_SAVE_LOG_SVID=93,//������־�����ؽ���
			SESSION_WEBGUILD_SVID=94,//����webϵͳ����session�������
			SERVICE_SAVE_LOG_SVID=96,//service������־�����ؽ���
			//wuji end
			DBNODE_SVID = 95,
			SYNC_QUEUE_SVID = 96,
			P2PLINKD_SVID = 97,
			VIDEO_SVID = 98, // video_entry
			SERVICE_C2S_SVID = 99,  //ҵ��Ϳͻ��˽���

			ONLINE_ST_SVID = 100,	// online ͳ��
			IMGAME_SVID = 101,     // ��ϷIM

			SENDCODE_SVID = 102,   // �·�����
			
			BACKSTAT_SVID = 103, // ��̨ͳ��SVID
			MOBILE_APP_SVID = 104,
			YAUTHD_SVID = 105, // YAuthD��YY������֤ƽ��
			
			IMONLINE_PROXY_SVID = 106,  // imonline_proxy
			IMONLINEV2_SVID = 107,      // imonlineV2
			IMONINDEXV2_SVID = 108,     // imonindexV2
			APONLINE_SVID = 108,  // aponline by im

			GMSGREADINFO_SVID = 109, // gmsgreadinfo_d

			CARD_EXCHANGE_SVID = 110,	//YY��Ƭ����
			IMONBRIDGEV2_SVID = 111,    // imonBridgeV2

			YGAUTHD_SVID = 111, // Game������֤
			SMS_SVID = 112, //���ŷ���
			LG_FREQ_IP_SVID = 113, // ����¼
			LG_ACCT_SAFETY_SVID = 114, // ��ȫ�ȼ�
			LG_ACCT_HISTORY_SVID = 115, // �û���ʷ
			LG_KDC_SVID = 116, // Ʊ������
			LG_GENERIC_INTERNAL_SVID = 117,
			LG_COMMON_SVID = 118,  // ��¼��������ͨ��

			AIRTICKETAPP_SVID = 120,
			SECUSERINFO_SVID = 123, /// UDB guoxijun
			FREQLOGIN_SVID = 124, /// UDB guoxijun
			THRIFTRECOGNIZE_DB_SVID = 128, ////  ���� thrift Э���ռλ����Ҳ�Ҫ��
			WEB_GH_SVID = 129,			/// �������ܺ�Ԫռ��
			VIP_SVID = 149, //VIP����ϵͳ
			MAKINGFRIEND_APP_SVID = 150, // YY����Ӧ��
			IMAPP_OFFLINEMSG_SVID = 151, // app������Ϣ
			
			YYREPUTATION_IM_SVID = 168, // YY�ɳ���ϵIM��������

			APP_TEMPLATE_SVID = 169, // Ƶ��ģ��
			APP_COMPONENT_GRAY_SVID = 170, // Ӧ�� �����
			APP_COMPONENT_CONTROL_SVID = 171, // Ӧ�� �����
			
			LINKD_TRANSMIT_SVID = 172, // linkd ͸��ҵ��
            PROBE_DATA_SVID = 173, // Probe Data related msg

			//for dbconsistency project
			DBCONSISTENCY_SVID = 175, // add by lianyongjian,2011-10-24
			SMS_BK_SVID = 176,		// sms back 

			IMOFFLINEMSGV2_SVID = 180,
			MSGCHECK_SVID = 181,

			QASYSTEM_SVID = 182,  // �ʴ�ϵͳ
			QAGAME_SVID = 183, //��Ϸ�û����ݷ���ϵͳ��Ŀǰ���ʴ�ϵͳ�ã�
			QAMSG_SVID = 184,  // �ʴ�ϵͳ��Ϣ
			QAOFFLINEMSG_SVID = 185, //�ʴ�ϵͳ������Ϣ
			QATRANSFER_SVID = 186,  // �ʴ�ϵͳת��
			QAONLINE_SVID = 187, //�ʴ�ϵͳ����
			PLUGIN_SVID = 188, //���Э��
			QASEARCH_SVID = 189,//�ʴ�����
			QASTAT_SVID = 190, //�ʴ�ͳ��
			QACITIZEN_SVID = 191, //�ʴ�ϵͳ������Ϣ
			QATOPIC_SVID = 192, //�ʴ�ϵͳ����

			FACE_SVID = 200, //��������
			APPMGR_SVID = 210,   // Ӧ�ù�������
			COMPONENT_SVID = 211,
			DOWNLOAD_CONTROL_SVID = 212,

            SE_DISPATCHER_SVID = 215,
      
    
			GANTIAD_SVID = 230,          //Ⱥ�����
			ANTIJUNK_SVID = 231,  // �����
			
			MQ_SVID = 236, //mq ��;

			VIP_SVINFO=232,             //vip_vinfo
            SERVERLOGIN_SVID = 233,     // added by zhangxu 2012-10-24
			VIP_VICON_SVID=243,        //vip_vcion
			VIP_EXPRESSION_SVID = 244, // ��Ա��������
			VIP_PRIVILEGE_SVID = 245,  // ��Ա������Ȩ
			VIP_WEB_RING_SVID = 246, //��Աwebring ����serverId
			VIP_RING_SVID = 247,      
			IM_VIP_SRVID = 249,      // ���ѵ�vip��Ϣ
			MSGSECURE_SVID = 250,
			IMSTAT_SVID = 251,
			IMBDOOR_SVID = 252, //ImBDoor����
			IMIDUPDATE_SVID = 253, //��yy��Э��
			GMONITOR_SVID = 254, //gmonitorʹ��
			BASEAPP_MAX_SVID = 255 // �����Ļ��������ŷ�Χ

		};
}
#endif /*BASE_SVID_H_*/
