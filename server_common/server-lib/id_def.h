#ifndef STR_ID_DEF_H_
#define STR_ID_DEF_H_

#define DAEMON_SERVERID 0

#define SUFFIX_PIP_NODE "pipm_node"	// old pipm

#define SUFFIX_UDB_NEW "act_accnt"
#define SUFFIX_ALL "_a"           //all service
#define SUFFIX_CHANNEL "ch"	//session manager
#define SUFFIX_SMANAGER_PROXY "smproxy"			// SM proxy, by liwei
#define SUFFIX_SDB	"sd"	//Sesion info Db
#define SUFFIX_LOGDB "ld"	// logdbd_d: LogDB for session operates
#define SUFFIX_UDB	"du"  //Duowan User db
#define SUFFIX_ROUTE	"_r"  //Route
#define SUFFIX_UINFO  "uf"	//u info
#define SUFFIX_INVITECODE "inc"  //invite code
#define SUFFIX_LBS		"bs" //load balance server
#define SUFFIX_LBSLITE		"bs_lite" // lite load balance server
#define SUFFIX_LBSMANAGER		"bs_mgr" //load balance server manager
#define SUFFIX_WEBYY_LBS		"webyy_bs" //load balance server for web and mobile
#define SUFFIX_LINKD	"_lk"	// linkd
#define SUFFIX_SLIST	"sl"	// slistd
#define SUFFIX_SESSION "se"		// sessiond
#define SUFFIX_PIPSESSION "pipse"		// sessiond
#define SUFFIX_SIFU_SESSION "sifuse"
#define SUFFIX_IM_SESSION "imp2pse"		// session for im p2p voice
#define SUFFIX_MAFIA_SESSION "killerse"	// session for mafia party game
#define SUFFIX_PIPSERVER  "pipm"        // pip manage server
#define SUFFIX_IMDB  "md" //imlist_d
#define SUFFIX_CATALOG "ca"  // catalog
#define SUFFIX_REGISTER "reg" // register_from
#define SUFFIX_AGENT_REGISTER "areg" //register agent
#define SUFFIX_ONLINEDB "on"
#define SUFFIX_ONLINEDB_ST "on_st"	// online user statistic
#define SUFFIX_ONLINEDB_USER "online_user"	// online user
#define SUFFIX_SESSCACHE "sc" // session cache server
#define SUFFIX_HS_SESSCACHE "hsc" // handlersocket session cache server
#define SUFFIX_SESSCACHE_PL "sc_pl" // session cache server panglu
#define SUFFIX_CANODE "canode"
#define SUFFIX_IPLIST "il"    // ip list
#define SUFFIX_ONLINEAUX "aux" // online_aux
#define SUFFIX_ONLINEMISC "misc" //online misc
#define SUFFIX_DBDPROXY "dbdpr" // dbd_proxy
#define SUFFIX_WEBDBGATEWAY "webdb_gw" // webdb_gateway
#define SUFFIX_SENDCODE "sc"
#define SUFFIX_YGAUTH	"yga"
#define SUFFIX_SMSGW	"sms_gw"
#define SUFFIX_VIPDB    "vipdb"
#define SUFFIX_AIRTICKETAPP "airticketapp"
#define SUFFIX_GLOBALBAN "globalban"
#define SUFFIX_KICKOFF "kickoff"
#define SUFFIX_LINKD_MQ "linkd_mq"

//for smanager
#define SUFFIX_SMOBSERVER "smobserver"
#define SUFFIX_SMANAGER "smgr"	//real session manager
#define SUFFIX_SMDUPCHANNEL "smdupchannel"
#define SUFFIX_PRESS "pres"	//pres to smd(ch)
#define SUFFIX_SMPROXYADMIN "smproxyadmin"
#define SUFFIX_SMLOGGER "smanagerlogger"
#define SUFFIX_SMPREMANAGER "smpremanager"
#define SUFFIX_PRETREAT_FROZEN "pretreatfrozen"
#define SUFFIX_PRETREAT_LOCKED "pretreatlocked"
#define SUFFIX_PRETREAT_REDIRECT "pretreatredirect"
#define SUFFIX_LOCAL_SMDISPATCHER "localdispatcher" //local smdispatcher
#define SUFFIX_LOCAL_SMNODE "localsmgr" //local smnode
#define SUFFIX_LOCAL_SMPROXYADMIN "localsmproxyadmin"
#define SUFFIX_SMGR_SYNC "smgrsync"
#define SUFFIX_OPTSPIPMGR "optspipmgr"
#define SUFFIX_TEXTCHAT "textchat"
#define SUFFIX_EXT_BROAD "ext_broad"
#define SUFFIX_SEDISPATCHER "sedispatcher"
#define SUFFIX_DISP_CONFIG "disp_config"

#define SUFFIX_VIDEO_ENTRY "videoentry"

//dupsession
#define SUFFIX_DUPSESSION "ds"	//dupsession_d

#define SUFFIX_IMONLINED "imol"
#define SUFFIX_IMONINDEX "imonindex"
#define SUFFIX_IMONBRIDGE  "imonbridge"
#define SUFFIX_IMONLINEINFO "imolinfo"

#define SUFFIX_IMLINKD "imlk"
#define SUFFIX_IMCAHTD "imct"
#define SUFFIX_SYSMSGD "sm"		//sysmsgd
#define SUFFIX_IMLBSD "imlbs"

#define SUFFIX_IMONLINE_CACHED "iomem"  // imonline_cached
#define SUFFIX_IMBRIDGE "imbridge"      // imbridge
//wuji start
#define SUFFIX_IMIDD "imid"
#define SUFFIX_IMAPP "imapp"
#define SUFFIX_IMUINFO "imuinfo"
#define SUFFIX_IMGETIMID "imgetimid"

#define SUFFIX_IMLIST2 "imlist2"
#define SUFFIX_IMLIST3 "imlist3"
#define SUFFIX_IMTRANS2 "imtrans2"
#define SUFFIX_IMOFFLINEMSG2 "imofflinemsg2"
#define SUFFIX_SESSION_SAVELOG "sessionsavelog"
#define SUFFIX_LOGDB2 "logdb2"
#define SUFFIX_WEBLOGDB2 "weblogdb2"

#define SUFFIX_SESSION_SAVELOG_WAN "savelogwan"
#define SUFFIX_LOGDB_WAN "logdbwan"

#define SUFFIX_SM_READPROXY "smreadproxy"
#define SUFFIX_SESSION_WEBGUILD "sessionwebguild"

#define SUFFIX_SERVICE_SAVELOG "servicesavelog"

//wuji end
#define SUFFIX_IMCAHT2D "imct2"
//added by zzw ,20090909
#define SUFFIX_IMOLMD "imolm"
#define SUFFIX_ACCESS "access"
#define SUFFIX_Pay "pay"
//end

//add by kdjie
#define SUFFIX_IMTRANS "imtrans"
#define SUFFIX_IMONDISP "imondisp"
#define SUFFIX_IMONLINE2 "imon2"
#define SUFFIX_IMAPP_OFFLINEMSG "imappoffmsg"

#define SUFFIX_IMONLINEV2      "imolv2"
#define SUFFIX_IMONLINE_PROXY  "imolproxy"
#define SUFFIX_IMONBRIDGEV2    "imonbridgev2"
#define SUFFIX_IMONINDEXV2     "imonindexv2"

#define SUFFIX_GTRANS "gtrans"
#define SUFFIX_GMSGCACHE "gmsgcache"
//added by lijin
#define SUFFIX_GCHAT "gchat"
#define SUFFIX_GCHATDIS "gchatdis"
#define SUFFIX_GCHATSTAT "gchatstat"
#define SUFFIX_GAUTH "gauth"
//end lijin
#define SUFFIX_GTOPIC "gtopic"
#define SUFFIX_GTOPICCACHE "gtopiccache"
#define SUFFIX_GLISTCACHE "glistcache"
#define SUFFIX_MSGSTORE "msgstore"
#define SUFFIX_MSGPACKER "msgpacker"

//added by roc
#define SUFFIX_GMEMBERINFO "gmemberinfo"
#define SUFFIX_GMEMBERINFOCACHE "gmemberinfocache"
#define SUFFIX_GVERIFYCODE "gverifycode"
//#define SUFFIX_GINFO "ginfo"
#define SUFFIX_GOFFLINEMSG "gofflinemsg"
#define SUFFIX_GOFFLINE "goffline"
#define SUFFIX_GLIST "glist"
#define SUFFIX_GROUPID "groupid"
//add end

//added by maxn
#define SUFFIX_GPROPCACHE "gpropcache"
#define SUFFIX_GPROP "gprops"

#define SUFFIX_IMGAMEPRO "imgamefarm"

//for mysql test
#define SUFFIX_MYSQLONLINEDB "mon"
#define SUFFIX_MYSQLLINKD	"m_lk"
#define SUFFIX_MYSQLSMANAGER  "mch"
//added  by zzw , 20090921
#define SUFFIX_IMSEARCH  "imse"
//end
#define SUFFIX_IMHASHSETINFO  "imhash"
#define SUFFIX_IMMAKWUP  "immu"
#define SUFFIX_TEST_SMGR	"tsmgr"		// smgr??·????

//added byt linyue
#define SUFFIX_GINFO "ginf"

//add by long
#define SUFFIX_GCHECK "gchk"
#define SUFFIX_HASHPROXY "hashproxy"
#define SUFFIX_IMLOCALCACHE "imlocalcache"

//add by liuzhibin
#define SUFFIX_ROAMD	"roamd"


//for oracle test
#define SUFFIX_ORACLESDB	"osd"	//Sesion info Db for oracle
#define SUFFIX_ORACLESLIST	"osl"	//slist		  Db for oracle
#define SUFFIX_ORACLEUINFO	"ouf"	//uinfo		  Db for oracle
#define SUFFIX_ORACLEUDB	"odu"	//uinfo		  Db for oracle

#define SUFFIX_ACCESS "access"  // for access server
#define SUFFIX_WATCHDOG "watchdog"
#define SUFFIX_GMONITOR "gmonitor"

//added by lbc
#define SUFFIX_IMROUTER "imrouter"  
#define SUFFIX_GMSGREADINFO "gmsgreadinfo"

#define SUFFIX_GAUTH "gauth"
#define SUFFIX_IMCACHE      "imcache"

#define SUFFIX_P2PBRIDGE "p2pbridge"
#define SUFFIX_P2PLINKD "p2plinkd"

#define SUFFIX_IMSTAT      "imstat"

#define SUFFIX_GACTIVITY   "gactivity"

#define SUFFIX_YAUTHD "yauthd"
#define SUFFIX_OPENID_FORWARDER "openid_forwarder"

#define SUFFIX_MOBILEAPP "mobileapp"

//added by maxn
//uinfo
#define SUFFIX_SERVICEUINFO "uis"  
#define SUFFIX_SERVICETEMPUINFO "tuis"  
#define SUFFIX_SERVICEUINFO_NOTIFY "uinfo_notify"
#define SUFFIX_SERVICE_SDBD	"uissdbd"
#define SUFFIX_ANTIJUNK "antijunk"

//exuinfo
#define SUFFIX_SERVICEUINFO_EX "exuis"  
#define SUFFIX_SERVICETEMPUINFO_EX "extuis"  
#define SUFFIX_SERVICEUINFO_EX_NOTIFY "exuinfo_notify"
#define SUFFIX_SERVICEUINFO_EX_SDBD	"exuinfo_sdbd"

//chinfo

#define SUFFIX_SERVICECHINFO "chs"  
#define SUFFIX_SERVICECHINFO_DBD	"sidproperty"
#define SUFFIX_SERVICECHINFO_NOTIFY	"chinfo_notify"
#define SUFFIX_SERVICE_AUTHENTICATION 	"roler_auth_service"
#define SUFFIX_SERVICECHINFO_STAT "chs_stat"

//stat&alarm
#define SUFFIX_SERVICE_STAT_DBD 	"stat_db"//v4 解耦使用统计报警dbd进程
#define SUFFIX_SERVICE_STAT 	"bt_stat"//v4 解耦使用统计报警进程



// added by zxz|2011-08-08
#define SUFFIX_LOGINSERVER "lg"
#define SUFFIX_IPUSUALLOGIN "iplg"
#define SUFFIX_LG_MINFO  "lgmif"
#define SUFFIX_LG_LOGINSAFE "lgsafe"
#define SUFFIX_LG_MONITOR "mntr"

// added by suchuhui
#define SUFFIX_UDB_IP_BUBBLE "uipbub"
#define SUFFIX_HIS_PWD	"hp"
#define SUFFIX_SMS_BK	"sms_bk_d"
#define SUFFIX_LG_KDC	"lgkdc"
#define SUFFIX_NEWPWDDB	"newpwddb"
#define SUFFIX_GEN_NUM	"gennum"

// added by zhangxu ,2012-7-19
#define SUFFIX_LG_STATISTICS "lgstatisticsd"
// added by zhangxu , 2012-10-30
#define SUFFIX_LG_VERIFYPWD "lgvpwd"
// added by zhangxu , 2012-11-14
#define SUFFIX_LG_SECAGENT "lgsecagentd"
// added by zhangxu , 2012-12-19
#define SUFFIX_LG_SMSCODE "lgsmscoded"
// added by zhangxu , 2012-12-25
#define SUFFIX_XM_XUIDUDB "xm_xuidudb"
#define SUFFIX_LG_XMANREG "lgxmanregd"

#define SUFFIX_VIP_HTTPAPI "viphttpapi"
#define SUFFIX_VIP_LOGIC   "viplogic"
#define SUFFIX_IM_VIP      "imvip"
#define SUFFIX_VIP_SYSMSG  "vipsysmsg"
#define SUFFIX_VIP2_SYSMSG  "vip2_sysmsg"
#define SUFFIX_VIP_APP		"vipapp"
#define SUFFIX_VIP_WEBRING  "imvipwebring"
#define SUFFIX_VIP_EXPRESSION "vip_expression"
#define SUFFIX_VIP_PAY_WORKER "payworker"
#define SUFFIX_VIP_PAY "vippay"
#define SUFFIX_VIP_SCORE "vip_score"
#define SUFFIX_VIP_SMS "vip_sms"
#define SUFFIX_VIP_SHOWACTIVITY "vip_showactivity"
#define SUFFIX_VIP_LOTTERY "vip_lottery"
#define SUFFIX_VIP_MMS_VERIFY "mmsverify"

#define SUFFIX_DIAMOND_PAY_WORKER "vip2payworker"
#define SUFFIX_DIAMOND_APP "vip2app"
#define SUFFIX_DIAMOND_PIECES "vip2pieces"
#define SUFFIX_DIAMOND_TASK_SYSTEM "vip2tasksystem"


#define SUFFIX_OWVIP_PAY_WORKER "owpayworker"
#define SUFFIX_OWVIP_APP		"vip3app"

#define SUFFIX_SYSMSGV1 "sysmsgv1"

#define SUFFIX_SLAVE_DB_REDIS "slaverds"

#define SUFFIX_APPMGR "appmgr"

#define SUFFIX_ACTIVATEYY "act_yy"

//added by yangzhuoyan
#define SUFFIX_IMDATA_BRIDGE "imdatabridge"
#define SUFFIX_IMDATA_DISPATCHER "imdatadispatcher"
#define SUFFIX_IMAGE_SOURCE "imagesource"
#define SUFFIX_IMAGE_FRONT "imagefront"


#define SUFFIX_BULLETINDB "db_bulletindb"

#define SUFFIX_CONF_SERVER "conf_srv"
#define SUFFIX_CONF_SERVER_SYNC "conf_srv_sync"


#define SUFFIX_COMP "compsrvd"
#define SUFFIX_DOWNLOAD_CTRL "download_ctrld"

#define SUFFIX_LKONLINE "lkol"
#define SUFFIX_SPONLINE "spol"

//added for wuerping
#define SUFFIX_YYAC_DAEMON_DATA "yyac_dd"  // yyac_daemon_data

#define SUFFIX_LG_MQCONVERTOR "mq_convertor" // Generic YY to MQ convertor

#define SUFFIX_LOGINCACHE "loginCache"
#define SUFFIX_APP_TEMPLATE "app_template"
#define SUFFIX_APP_COMPONENT_GRAY "app_component_gray"
#define SUFFIX_APP_COMPONENT_CONTROL "app_component_control"

#define SUFFIX_ANTI_VERIFY "anti_verify_login_svr"
#define SUFFIX_QA_STAT "qa_stat"
#define SUFFIX_LINKD_CONFIG	"linkdconfig"	// linkdConfig(linkdCongestConfig)
#define SUFFIX_PROBE_CA	"probe_data_ca"	// ca for probe stat data

#define SUFFIX_MR_LOCATION_SERVER "mrls" // mr location server

#define SUFFIX_PROBE_DATA_MGR "probe_data_mgr"

#define SUFFIX_MOBILE_PROXY "mobile_proxy"
#define SUFFIX_MPBS "mpbs" //mobile proxy balance server
#define SUFFIX_MOBILE_ONLINE "mobile_online"

#define SUFFIX_PREREGUIDMGR_SIGNAL "PreRegUidMgr_Signal"
#define SUFFIX_APONLINEPROXY        "apolproxy"

#endif

