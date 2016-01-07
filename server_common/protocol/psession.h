#ifndef PSESSION_H_
#define PSESSION_H_
#include "common/protocol/psessionbase.h"
#include "common/protocol/psession.h"
#include "common/protocol/pslist.h"
#include "common/protocol/psinfo.h"
#include "common/core/base_svid.h"
#include "common/protocol/zip_tpl.h"
#include <vector>
#include <iterator>
namespace server {
namespace session {

  enum SMDISPATCHER_URI {
    PCoSessionLbsURI          = (  1 << 8 | protocol::SMDISPATCHER_SVID ),
    PCoSessionLbsResURI       = (  2 << 8 | protocol::SMDISPATCHER_SVID ),
    PUpdateSessionInfoURI     = (  3 << 8 | protocol::SMDISPATCHER_SVID ),
    PUpdateSessionInfoResURI  = (  4 << 8 | protocol::SMDISPATCHER_SVID ),
    PInjectSessInfoURI        = (  5 << 8 | protocol::SMDISPATCHER_SVID ),
    PInjectSessInfoResURI     = (  6 << 8 | protocol::SMDISPATCHER_SVID ),
    PAllocateChanelURI        = (  7 << 8 | protocol::SMDISPATCHER_SVID ),
    PAllocatechannelResURI    = (  8 << 8 | protocol::SMDISPATCHER_SVID ),
    PDetectTaskURI            = (  9 << 8 | protocol::SMDISPATCHER_SVID ),
    PDetectTaskResURI         = ( 10 << 8 | protocol::SMDISPATCHER_SVID ),
    PChkUserNumURI            = ( 11 << 8 | protocol::SMDISPATCHER_SVID ),
    PChkUserNumResURI         = ( 12 << 8 | protocol::SMDISPATCHER_SVID ),
    PCoSessionLbsHSURI        = ( 13 << 8 | protocol::SMDISPATCHER_SVID ),
    PCoSessionLbsResHSURI     = ( 14 << 8 | protocol::SMDISPATCHER_SVID ),
    PAllocateChanelHSURI      = ( 15 << 8 | protocol::SMDISPATCHER_SVID ),
    PAllocatechannelResHSURI  = ( 16 << 8 | protocol::SMDISPATCHER_SVID ),
    PUpdateSessionInfoHSURI   = ( 17 << 8 | protocol::SMDISPATCHER_SVID ),
    PInjectSessInfoHSURI      = ( 18 << 8 | protocol::SMDISPATCHER_SVID ),
    PChkUserNumHSURI          = ( 19 << 8 | protocol::SMDISPATCHER_SVID ),
    PChkUserNumResHSURI       = ( 20 << 8 | protocol::SMDISPATCHER_SVID ),
    PDetectTaskDc2URI         = ( 21 << 8 | protocol::SMDISPATCHER_SVID ),
    PDetectTaskResDc2URI      = ( 22 << 8 | protocol::SMDISPATCHER_SVID ),
    PCoSessionLbsRes2URI      = ( 23 << 8 | protocol::SMDISPATCHER_SVID ),
    PInjectSessInfo2URI       = ( 24 << 8 | protocol::SMDISPATCHER_SVID ),
    PAllocatechannelRes2URI   = ( 25 << 8 | protocol::SMDISPATCHER_SVID ),
    PUpdateSessionInfo2URI    = ( 26 << 8 | protocol::SMDISPATCHER_SVID ),
    PCoSessionLbsResHS2URI    = ( 27 << 8 | protocol::SMDISPATCHER_SVID ),
    PInjectSessInfoHS2URI     = ( 28 << 8 | protocol::SMDISPATCHER_SVID ),
    PAllocatechannelResHS2URI = ( 29 << 8 | protocol::SMDISPATCHER_SVID ),
    PUpdateSessionInfoHS2URI  = ( 30 << 8 | protocol::SMDISPATCHER_SVID ),
    PCoSessionLbsHS2URI       = ( 31 << 8 | protocol::SMDISPATCHER_SVID ),
    PAllocateChannelHS2URI    = ( 32 << 8 | protocol::SMDISPATCHER_SVID ),
    PChannelFrozenURI         = ( 33 << 8 | protocol::SMDISPATCHER_SVID ),
    PSManagerUpdateURI        = ( 34 << 8 | protocol::SMDISPATCHER_SVID ),
    PSManagerClearURI         = ( 35 << 8 | protocol::SMDISPATCHER_SVID ),
    PAllocateChannel2URI      = ( 36 << 8 | protocol::SMDISPATCHER_SVID ),
    PSmnodePingURI            = ( 37 << 8 | protocol::SMDISPATCHER_SVID ),
    PSmnodePongURI            = ( 38 << 8 | protocol::SMDISPATCHER_SVID ),
    PSmObserverPongURI        = ( 39 << 8 | protocol::SMDISPATCHER_SVID ),
    PSmdispatcherPongURI      = ( 40 << 8 | protocol::SMDISPATCHER_SVID ),
    PUpdateSmanagerSgConfigURI = ( 41 << 8 | protocol::SMDISPATCHER_SVID ),
    PRemoveSmanagerSgConfigURI = ( 42 << 8 | protocol::SMDISPATCHER_SVID ),
    PChkUserNumRes2URI         = ( 43 << 8 | protocol::SMDISPATCHER_SVID ),  
    PUpdateSmanagerChannelConfigURI = ( 44 << 8 | protocol::SMDISPATCHER_SVID ),
    PRemoveSmanagerChannelConfigURI	= ( 45 << 8 | protocol::SMDISPATCHER_SVID ),
    PCoSessionLbsHS3URI = ( 60 << 8 | protocol::SMDISPATCHER_SVID ),
    PCoSessionLbsResHS3URI = ( 61 << 8 | protocol::SMDISPATCHER_SVID ),
    PInjectSessInfoHS3URI     = ( 62 << 8 | protocol::SMDISPATCHER_SVID ),
    //deal with freeze and lock channel in smnode
    //local lookup and reply, reply increases channel state and time
    PCoSessionLbsHS4URI          = (  63 << 8 | protocol::SMDISPATCHER_SVID ),
    PCoSessionLbsResHS4URI       = (  64 << 8 | protocol::SMDISPATCHER_SVID ),
    //global lookup and reply, reply increases channel state and time
    PCoSessionLbsHS5URI          = (  65 << 8 | protocol::SMDISPATCHER_SVID ),
    PCoSessionLbsResHS5URI       = (  66 << 8 | protocol::SMDISPATCHER_SVID ),
    //allocate and allocate reply, reply increases channel state and time
    PAllocateChannelHS3URI    = ( 67 << 8 | protocol::SMDISPATCHER_SVID ),
    PAllocatechannelResHS3URI = ( 68 << 8 | protocol::SMDISPATCHER_SVID ),
    //smproxy read data from mysql and sync to smdispatcher
    PSyncAllowUidURI                  = (75 << 8 | protocol::SMDISPATCHER_SVID),
    PSyncAllowUidAckURI               = (76 << 8 | protocol::SMDISPATCHER_SVID),
    PSyncChannelConfigURI             = (77 << 8 | protocol::SMDISPATCHER_SVID),
    PSyncChannelConfigAckURI          = (78 << 8 | protocol::SMDISPATCHER_SVID),
    PSyncSgConfigURI                  = (79 << 8 | protocol::SMDISPATCHER_SVID),
    PSyncSgConfigAckURI               = (80 << 8 | protocol::SMDISPATCHER_SVID),

    PForceUpdateSessionInfoHSURI       = (81 << 8 | protocol::SMDISPATCHER_SVID),
    PInjectSessInfoHS4URI              = (82 << 8 | protocol::SMDISPATCHER_SVID ),

    PSmnodeStatusPingURI            = ( 83 << 8 | protocol::SMDISPATCHER_SVID ),
    PSmnodeStatusPongURI            = ( 84 << 8 | protocol::SMDISPATCHER_SVID ),    
  };

  //common/protocol/psmanager.h has already defined enmu SMANAGER_URI, but it is in
  // namespace protocol::session.
  enum SMANAGER_URI { // ( 100, 300 )
    PSessionSyncStartURI   = (129 << 8 | protocol::SMANAGER_SVID),
    PSessionSyncURI        = (130 << 8 | protocol::SMANAGER_SVID),
    PChannelRemovedURI     = (131 << 8 | protocol::SMANAGER_SVID), 
    PCheckDupURI           = (132 << 8 | protocol::SMANAGER_SVID), 
    PCheckDupResURI        = (132 << 8 | protocol::SMANAGER_SVID), 
    PSessionStartSyncURI   = (133 << 8 | protocol::SMANAGER_SVID),  
    PBatchSyncUsersURI     = (134 << 8 | protocol::SMANAGER_SVID),  
    PBatchSyncUserResURI   = (134 << 8 | protocol::SMANAGER_SVID),  
    PChannelRemoved2URI    = (135 << 8 | protocol::SMANAGER_SVID), 
    PSessionStartSync2URI  = (253 << 8 | protocol::SMANAGER_SVID),  
    PSessionStartSync3URI  = (260 << 8 | protocol::SMANAGER_SVID),  
    PSessionStartSync4URI  = (261 << 8 | protocol::SMANAGER_SVID),  
    PSessionStatUpdateURI  = (262 << 8 | protocol::SMANAGER_SVID),
    PUpdateChannelOnSessionURI  = (263 << 8 | protocol::SMANAGER_SVID), 
    PGetSessUserNumURI          = (264 << 8 | protocol::SMANAGER_SVID), 
    PSessionStartSync5URI       = (265 << 8 | protocol::SMANAGER_SVID),   
    PGetSessUserNumResURI       = (265 << 8 | protocol::SMANAGER_SVID),   // UNUSED
    PBatchChannelRemovedURI     = (266 << 8 | protocol::SMANAGER_SVID), 
    PRemoveChannelURI           = (267 << 8 | protocol::SMANAGER_SVID), 
    PMSmanagerStart2URI         = (268 << 8 | protocol::SMANAGER_SVID),
    PSessionSyncInfo5URI        = (269 << 8 | protocol::SMANAGER_SVID),
    PSessionStartSync6URI       = (270 << 8 | protocol::SMANAGER_SVID),
    PFreezeChannelURI           = (271 << 8 | protocol::SMANAGER_SVID),
    PRelocateChannelURI         = (272 << 8 | protocol::SMANAGER_SVID),  
    PRecyleChannelAsidURI       = (273 << 8 | protocol::SMANAGER_SVID),
    PChannelMaintainanceResURI  = (274 << 8 | protocol::SMANAGER_SVID),
    PDismissChannelURI          = (275 << 8 | protocol::SMANAGER_SVID),
    //for smproxy admin
    PSessionSyncInfo6URI        = (276 << 8 | protocol::SMANAGER_SVID),
    PProxySyncInfo3URI          = (5318 << 8 | protocol::SMANAGER_SVID),
    PCheckSGonProxyAdminURI     = (277 << 8 | protocol::SMANAGER_SVID),
    PNotifySGonProxyAdminURI    = (278 << 8 | protocol::SMANAGER_SVID),
    PMSmanagerStart4URI         = (279 << 8 | protocol::SMANAGER_SVID),
    // for open platform sdk
    PSessionLbs3URI             = (280 << 8 | protocol::SMANAGER_SVID),
    PSessionLbsRes5URI          = (281 << 8 | protocol::SMANAGER_SVID),
    //for smnode/session/smproxyadmin new messages
    PSessionStartSync7URI       = (282 << 8 | protocol::SMANAGER_SVID),
    PMSmanagerStart3URI         = (283 << 8 | protocol::SMANAGER_SVID),
    PSessionGroupStatUpdateURI  = (284 << 8 | protocol::SMANAGER_SVID),
    PUpdateChannelOnSession1URI = (285 << 8 | protocol::SMANAGER_SVID),
    PSessionSyncInfo7URI        = (286 << 8 | protocol::SMANAGER_SVID),
    PSessionUpdateInfoURI       = (287 << 8 | protocol::SMANAGER_SVID),
    PProxyAdminStopSyncSgsURI   = (288 << 8 | protocol::SMANAGER_SVID),
    PProxySyncInfo4URI          = (289 << 8 | protocol::SMANAGER_SVID),
    PSessionUpdateInfo1URI      = (290 << 8 | protocol::SMANAGER_SVID),
    PCheckSidUsersURI           = (291 << 8 | protocol::SMANAGER_SVID),
    PCheckSidUsersResURI        = (292 << 8 | protocol::SMANAGER_SVID),
    // For voice v4
    PNegotiateConfigURI         = (300 << 8 | protocol::SMANAGER_SVID),
    PNegotiateConfigResURI      = (301 << 8 | protocol::SMANAGER_SVID),
    PProxyRegisterURI           = (302 << 8 | protocol::SMANAGER_SVID),
    PProxyRegisterResURI        = (303 << 8 | protocol::SMANAGER_SVID),
    PProxyJoinURI               = (304 << 8 | protocol::SMANAGER_SVID),
    PProxyDropURI               = (305 << 8 | protocol::SMANAGER_SVID),
    PProxyInfoSyncURI           = (306 << 8 | protocol::SMANAGER_SVID),
    PProxyInfoUpdateURI         = (307 << 8 | protocol::SMANAGER_SVID),
    PProxyChannelsRemoveURI     = (308 << 8 | protocol::SMANAGER_SVID),
    PProxyPingURI               = (309 << 8 | protocol::SMANAGER_SVID),
    PProxyPingResURI            = (310 << 8 | protocol::SMANAGER_SVID),
    PMasterSessionChangeURI     = (311 << 8 | protocol::SMANAGER_SVID),
    PProxySyncChannelUsersURI   = (312 << 8 | protocol::SMANAGER_SVID),
    PProxyUnregisterURI         = (313 << 8 | protocol::SMANAGER_SVID),
    PGetMediaProxyInfoSMURI     = (314 << 8 | protocol::SMANAGER_SVID),
    PMediaProxySyncChannelUsersURI = (315 << 8 | protocol::SMANAGER_SVID),
    POnGetMediaProxyInfoSMURI   = (316 << 8 | protocol::SMANAGER_SVID),
    PGetSignalProxyInfoSMURI    = (317 << 8 | protocol::SMANAGER_SVID),
    POnGetSignalProxyInfoSMURI  = (318 << 8 | protocol::SMANAGER_SVID),
    PPingAppDaemonURI           = (319 << 8 | protocol::SMANAGER_SVID),
    PForceConnectMediaProxySMURI = (320 << 8 | protocol::SMANAGER_SVID),
    PPingAppDaemon2URI          = (321 << 8 | protocol::SMANAGER_SVID),

    PProxyAdminBroadcastURI     = (322 << 8 | protocol::SMANAGER_SVID),
    PProxyAdminBroadcastResURI  = (323 << 8 | protocol::SMANAGER_SVID),
    PProxyAdminBroadcastBatchURI= (324 << 8 | protocol::SMANAGER_SVID),
    PProxyAdminBroadcastBatchResURI = (325 << 8 | protocol::SMANAGER_SVID),
    PSubscribeEventURI          = (326 << 8 | protocol::SMANAGER_SVID),
    PUnsubscribeEventURI        = (327 << 8 | protocol::SMANAGER_SVID),

    PEventNotify_ChannelDestroyURI  = (328 << 8 | protocol::SMANAGER_SVID),
    PEventNotify_ChannelCreateURI   = (329 << 8 | protocol::SMANAGER_SVID),

    PGetSessionGroupListURI      = (330 << 8 | protocol::SMANAGER_SVID),
    PGetSessionGroupListResURI   = (331 << 8 | protocol::SMANAGER_SVID),

    PGetSessionGroupChannelListURI    = (332 << 8 | protocol::SMANAGER_SVID),
    PGetSessionGroupChannelListResURI = (333 << 8 | protocol::SMANAGER_SVID),
    PGetVideoProxyInfoFromAdmURI      = (334 << 8 | protocol::SMANAGER_SVID),
    PGetVideoProxyInfoResFromAdmURI   = (335 << 8 | protocol::SMANAGER_SVID),

    // 批量分配媒体和视频前端
    PGetMediaProxyInfoBatchURI        = (336 << 8 | protocol::SMANAGER_SVID),
    PGetMediaProxyInfoBatchResURI     = (337 << 8 | protocol::SMANAGER_SVID),
    PGetVideoProxyInfoBatchURI        = (338 << 8 | protocol::SMANAGER_SVID),
    PGetVideoProxyInfoBatchResURI     = (339 << 8 | protocol::SMANAGER_SVID),

    PProxyDropBatchURI                = (340 << 8 | protocol::SMANAGER_SVID),

    PProxyServiceSgInfosURI           = (350 << 8 | protocol::SMANAGER_SVID),

    //smproxy ping
    PSMProxyPingURI             = (400 << 8 | protocol::SMANAGER_SVID),
    PSMProxyPongURI             = (401 << 8 | protocol::SMANAGER_SVID),

    // for broadcast
    PBC_ProxyPingURI                 = (500 << 8 | protocol::SMANAGER_SVID),
    PBC_ProxyPingResURI              = (501 << 8 | protocol::SMANAGER_SVID),
    PBC_ProxyEvent_ChannelCreateURI  = (502 << 8 | protocol::SMANAGER_SVID),
    PBC_ProxyEvent_ChannelRemoveURI  = (503 << 8 | protocol::SMANAGER_SVID),
    PBC_ProxySyncURI                 = (504 << 8 | protocol::SMANAGER_SVID),
    PBC_RouteURI                     = (505 << 8 | protocol::SMANAGER_SVID),
    PBC_BroadCastToUserURI           = (506 << 8 | protocol::SMANAGER_SVID),
    PBC_BroadCastAckURI              = (507 << 8 | protocol::SMANAGER_SVID),
    PBC_BroadCastResURI              = (508 << 8 | protocol::SMANAGER_SVID),
    PBC_BroadCastToAllProxyURI       = (509 << 8 | protocol::SMANAGER_SVID),
    PBC_RouteBroadCastURI            = (510 << 8 | protocol::SMANAGER_SVID),
    PBC_ProxyEvent_GroupCreateURI    = (511 << 8 | protocol::SMANAGER_SVID),
    PBC_ProxyEvent_GroupRemoveURI    = (512 << 8 | protocol::SMANAGER_SVID),
    PBC_PingURI                      = (513 << 8 | protocol::SMANAGER_SVID),
    PBC_PingResURI                   = (514 << 8 | protocol::SMANAGER_SVID),
    PBC_BroadCastToProxyByChannelURI = (515 << 8 | protocol::SMANAGER_SVID),

    //smpretreat
    PpretreatRequestURI        = (600 << 8 | protocol::SMANAGER_SVID),
    PpretreatRequestResURI     = (601 << 8 | protocol::SMANAGER_SVID),
    PpretreatchannelURI        = (602 << 8 | protocol::SMANAGER_SVID),
    PpretreatChannelResURI     = (603 << 8 | protocol::SMANAGER_SVID),
    PreProcessManagerReqURI    = (604 << 8 | protocol::SMANAGER_SVID),
    PreProcessManagerResURI    = (605 << 8 | protocol::SMANAGER_SVID),
    PreProcessChannelURI       = (606 << 8 | protocol::SMANAGER_SVID),
    PInqueryChannelURI         = (607 << 8 | protocol::SMANAGER_SVID),
    PInqueryChannel1URI        = (608 << 8 | protocol::SMANAGER_SVID),
    
    PFreezeChannel1URI            = (609 << 8 | protocol::SMANAGER_SVID),
    PRelocateChannel1URI          = (610 << 8 | protocol::SMANAGER_SVID),  
    PRecyleChannelAsid1URI        = (611 << 8 | protocol::SMANAGER_SVID),
    PChannelMaintainanceRes1URI   = (612 << 8 | protocol::SMANAGER_SVID),
    PDismissChannel1URI           = (613 << 8 | protocol::SMANAGER_SVID),
    //local smproxy admin
    PLocalSmproxyAdminPingAppDaemonURI          = (614 << 8 | protocol::SMANAGER_SVID),
    PProxyRegisterToLocalSmroxyAdminURI = (615 << 8 | protocol::SMANAGER_SVID),
    PProxyUnregisterToLocalSmproxyAdminURI = (616 << 8 | protocol::SMANAGER_SVID),
    PProxyRegisterToLocalSmproxyadminResURI = (617 << 8 | protocol::SMANAGER_SVID),
    PProxySyncChannelUsersToLocalSmproxyAdminURI = (618 << 8 | protocol::SMANAGER_SVID),
    PProxyPingLocalSmproxyAdminURI  = (619 << 8 | protocol::SMANAGER_SVID),
    PProxyChannelsRemove1URI = (620 << 8 | protocol::SMANAGER_SVID),
    PProxyPingLocalSmproxyAdminResURI = (621 << 8 | protocol::SMANAGER_SVID),
    PProxyChannelsCreateURI = (622 << 8 | protocol::SMANAGER_SVID),
    //for meaure purpose
    PLocalSmnodeAllocateSignalProxyMeasureURI = (623 << 8 | protocol::SMANAGER_SVID),    
    PLocalSmnodeAllocateLeastLoadSignalProxyMeasureURI = (624 << 8 | protocol::SMANAGER_SVID),
    PLocalSmnodeAllocateChannelConcentrationSignalProxyMeasureURI = (625 << 8 | protocol::SMANAGER_SVID),
    PRedisGetSgTsMeasureURI = (626 << 8 | protocol::SMANAGER_SVID),
    PRedisGetProxyInfoMeasureURI = (627 << 8 | protocol::SMANAGER_SVID),
    PRedisGetProxyAllocateLoadMeasureURI = (628 << 8 | protocol::SMANAGER_SVID),
    PRedisIncrProxyAllocateLoadMeasureURI = (629 << 8 | protocol::SMANAGER_SVID),
    PRedisSetProxyAllocateLoadExpireMeasureURI = (630 << 8 | protocol::SMANAGER_SVID),
    PRedisGetSgProxysMeasureURI = (631 << 8 | protocol::SMANAGER_SVID),
    PRedisGetChannelProxysMeasureURI = (632 << 8 | protocol::SMANAGER_SVID),
    PLocalSmnodeAllocateSignalProxyGenResPackMeasureURI = (633 << 8 | protocol::SMANAGER_SVID),
    PRedisPingMeasureURI = (634 << 8 | protocol::SMANAGER_SVID),
    PRedisSetProxyInfoMeasureURI = (635 << 8 | protocol::SMANAGER_SVID),
    PRedisRemoveProxyInfoMeasureURI = (636 << 8 | protocol::SMANAGER_SVID),
    PRedisRemoveProxyAllocateLoadMeasureURI = (637 << 8 | protocol::SMANAGER_SVID),
    PRedisSetProxyTsMeasureURI = (638 << 8 | protocol::SMANAGER_SVID),
    PRedisSetProxyUserMeasureURI = (639 << 8 | protocol::SMANAGER_SVID),   
    PRedisSetSgProxyMeasureURI = (640 << 8 | protocol::SMANAGER_SVID),   
    PRedisRemoveSgProxyMeasureURI = (641 << 8 | protocol::SMANAGER_SVID),   
    PRedisRemoveSgInfoMeasureURI = (642 << 8 | protocol::SMANAGER_SVID),   
    PRedisSetSgTsMeasureURI = (643 << 8 | protocol::SMANAGER_SVID),
    PRedisRemoveSgTsMeasureURI = (644 << 8 | protocol::SMANAGER_SVID),
    PRedisRemoveChannelOnProxyMeasureURI = (645 << 8 | protocol::SMANAGER_SVID),
    PRedisRemoveChannelInfoMeasureURI = (646 << 8 | protocol::SMANAGER_SVID),
    PRedisSetChannelOnProxyMeasureURI = (647 << 8 | protocol::SMANAGER_SVID),
    PRedisCleanAllDataMeasureURI = (648 << 8 | protocol::SMANAGER_SVID),
    PRedisSetProxyStatusMeasureURI = (649 << 8 | protocol::SMANAGER_SVID),   
    PRedisCacheGetProxyInfoMeasureURI = (650 << 8 | protocol::SMANAGER_SVID),   
    PRedisSetSgInAllSgsMeasureURI = (651 << 8 | protocol::SMANAGER_SVID),
    PRedisRemoveSgInAllSgsMeasureURI = (652 << 8 | protocol::SMANAGER_SVID),
    PRedisGetSgInAllSgsMeasureURI = (653 << 8 | protocol::SMANAGER_SVID),
    PRedisCacheGetSgProxyMeasureURI = (654 << 8 | protocol::SMANAGER_SVID),       
    PRedisCacheLoadSettingMeasureURI = (655 << 8 | protocol::SMANAGER_SVID),      
    //local smproxy admin manage idle signal proxy
    PRequireProxyInfoURI = (680 << 8 | protocol::SMANAGER_SVID),      
    PRequireProxyInfoResURI = (681 << 8 | protocol::SMANAGER_SVID),
    PRequireInChargeAreaURI = (682 << 8 | protocol::SMANAGER_SVID),
    PRequireInChargeAreaResURI = (683 << 8 | protocol::SMANAGER_SVID),
    PRequireSignalProxyToLocalSmproxyAdminURI = (684 << 8 | protocol::SMANAGER_SVID),
    PSessionRequireSignalProxyURI = (685 << 8 | protocol::SMANAGER_SVID),
    PRequireSignalProxyToLocalSmproxyAdminResURI = (686 << 8 | protocol::SMANAGER_SVID),


		//smmonitor
		PJoinChannelCongestURI      = (700 << 8 | protocol::SMANAGER_SVID),
		PJoinChannelUidCongestURI   = (701 << 8 | protocol::SMANAGER_SVID),
		PJoinChannelLinkdCongestURI = (702 << 8 | protocol::SMANAGER_SVID),
		PAllocateChannelCongestURI  = (703 << 8 | protocol::SMANAGER_SVID),
		PAllocateNodeChangeURI      = (704 << 8 | protocol::SMANAGER_SVID),
		PServiceProxyCongestURI     = (705 << 8 | protocol::SMANAGER_SVID),
    //for YY client
		PDispatchByLinkdQosResURI   = (706 << 8 | protocol::SMANAGER_SVID),

		PRequestConsolePortURI      = (707 << 8 | protocol::SMANAGER_SVID),
    PNotifyConsolePortURI       = (708 << 8 | protocol::SMANAGER_SVID),

    PSessionLbsRes6URI      = (800 << 8 | protocol::SMANAGER_SVID),

    //for by passing testing
    PSessionLbsBypassingURI = (900 << 8 | protocol::SMANAGER_SVID),
    PSessionLbsBypassingZipResURI  = (901 << 8 | protocol::SMANAGER_SVID),

    //for normal interface 
    PLocateSidURI       = ( 950 << 8 | protocol::SMANAGER_SVID),
    PLocateSidResURI    = ( 951 << 8 | protocol::SMANAGER_SVID),

    PReqRatedSpIpsURI       = (1001 << 8 | protocol::SMANAGER_SVID),
    PReqRatedSpIpsResURI    = (1002 << 8 | protocol::SMANAGER_SVID),
    PInitialProxyInfoSyncReqURI = (1003 << 8 | protocol::SMANAGER_SVID),
    PInitialProxyInfoSyncResURI = (1004 << 8 | protocol::SMANAGER_SVID),
    PPeriodicProxyInfoSyncURI = (1005 << 8 | protocol::SMANAGER_SVID),
    PGroupIdConfigURI = (1006 << 8 | protocol::SMANAGER_SVID),

    PGetBanChannelURI = (2000 << 8 | protocol::SMANAGER_SVID),
    PBanChannelListURI = (2001 << 8 | protocol::SMANAGER_SVID),

    PSyncBanChannelUri = (2002 << 8 | protocol::SMANAGER_SVID),
  };

  enum GUANBO_URI
  {
	PSS_BroadcastMsgURI = ( 1 << 8 | protocol::GUANBO_SVID),
  };

  struct PDismissChannel : public sox::Marshallable {
    enum { uri = PDismissChannelURI };
    uint32_t taskId;
    uint32_t sid;

    PDismissChannel() : taskId(0), sid(0) {}
    virtual void marshal(sox::Pack &p) const {
      p << taskId << sid;
    }
    virtual void unmarshal(const sox::Unpack &p) {
      p >> taskId >> sid;
    }
  };

  struct PFreezeChannel : public sox::Marshallable{
    enum {uri = PFreezeChannelURI };
    uint32_t taskId;
    uint32_t sid;
    uint32_t unfrozenTime; //to uint32 ???

    PFreezeChannel()
      :taskId(0)
      ,sid(0)
      ,unfrozenTime(0)
    {}
    virtual void marshal(sox::Pack &p) const {
      p << taskId << sid << unfrozenTime;
    }
    virtual void unmarshal(const sox::Unpack &p) {
      p >> taskId >> sid >> unfrozenTime;
    }
  };

  struct PRelocateChannel : public sox::Marshallable{
    enum {uri = PRelocateChannelURI };
    uint32_t taskId;
    uint32_t sid;
    uint32_t newSession;
    uint32_t newSessionGroup;

    PRelocateChannel()
      :taskId(0)
      ,sid(0)
      ,newSession(0)
      ,newSessionGroup(0)
    {}
    virtual void marshal(sox::Pack &p) const {
      p << taskId << sid << newSession << newSessionGroup;
    }
    virtual void unmarshal(const sox::Unpack &p) {
      p >> taskId >> sid >> newSession >> newSessionGroup;
    }
  };


  struct PRecyleChannelAsid : public sox::Marshallable{
    enum {uri = PRecyleChannelAsidURI };
    uint32_t taskId;
    uint32_t sid;

    PRecyleChannelAsid()
      :taskId(0)
      ,sid(0)
    {}
    virtual void marshal(sox::Pack &p) const {
      p << taskId << sid;
    }
    virtual void unmarshal(const sox::Unpack &p) {
      p >> taskId >> sid;
    }
  };

//response of maitainence req
  struct PChannelMaintainanceRes : public sox::Marshallable {
    enum {uri = PChannelMaintainanceResURI};
    uint32_t nodeId;  // by Smnode
    uint32_t taskId;  // backfilled by Smnode
    uint32_t sessionId;
    uint32_t sessionGroupId;
    uint16_t controlPort;
    uint32_t ctIP;
    uint32_t cncIP;
    uint32_t resCode;

    PChannelMaintainanceRes()
      :nodeId(0)
      ,taskId(0)
      ,sessionId(0)
      ,sessionGroupId(0)
      ,controlPort(0)
      ,ctIP(0) //china telecom --ctc
      ,cncIP(0)
      ,resCode(0)
    {}
    virtual void marshal(sox::Pack &p) const {
      p << nodeId << taskId << sessionId << sessionGroupId << controlPort;
      p << ctIP << cncIP;
      p << resCode;
    }
    virtual void unmarshal(const sox::Unpack &p) {
      p >> nodeId >> taskId >> sessionId >> sessionGroupId >> controlPort;
      p >> ctIP >> cncIP;
      p >> resCode;
    }
  };

  struct PDismissChannel1 : public sox::Marshallable {
    enum { uri = PDismissChannel1URI };
    uint32_t taskId;
    uint32_t sid;

    PDismissChannel1() : taskId(0), sid(0) {}
    virtual void marshal(sox::Pack &p) const {
      p << taskId << sid;
    }
    virtual void unmarshal(const sox::Unpack &p) {
      p >> taskId >> sid;
    }
  };

  struct PFreezeChannel1 : public sox::Marshallable{
    enum {uri = PFreezeChannel1URI };
    uint32_t taskId;
    uint32_t sid;
    uint32_t unfrozenTime; //to uint32 ???

    PFreezeChannel1()
      :taskId(0)
      ,sid(0)
      ,unfrozenTime(0)
    {}
    virtual void marshal(sox::Pack &p) const {
      p << taskId << sid << unfrozenTime;
    }
    virtual void unmarshal(const sox::Unpack &p) {
      p >> taskId >> sid >> unfrozenTime;
    }
  };

  struct PRelocateChannel1 : public sox::Marshallable{
    enum {uri = PRelocateChannel1URI };
    uint32_t taskId;
    uint32_t sid;
    uint32_t newSession;
    uint32_t newSessionGroup;

    PRelocateChannel1()
      :taskId(0)
      ,sid(0)
      ,newSession(0)
      ,newSessionGroup(0)
    {}
    virtual void marshal(sox::Pack &p) const {
      p << taskId << sid << newSession << newSessionGroup;
    }
    virtual void unmarshal(const sox::Unpack &p) {
      p >> taskId >> sid >> newSession >> newSessionGroup;
    }
  };


  struct PRecyleChannelAsid1 : public sox::Marshallable{
    enum {uri = PRecyleChannelAsid1URI };
    uint32_t taskId;
    uint32_t sid;

    PRecyleChannelAsid1()
      :taskId(0)
      ,sid(0)
    {}
    virtual void marshal(sox::Pack &p) const {
      p << taskId << sid;
    }
    virtual void unmarshal(const sox::Unpack &p) {
      p >> taskId >> sid;
    }
  };

  struct SessionInfoItem : public sox::Marshallable {
    uint32_t sessionId;
    uint32_t sessionGroupId;
    uint16_t controlPort;
    uint32_t ctIP;
    uint32_t cncIP;

    SessionInfoItem()
      :sessionId(0)
      ,sessionGroupId(0)
      ,controlPort(0)
      ,ctIP(0)
      ,cncIP(0)
    {}
    virtual void marshal(sox::Pack &p) const {
      p << sessionId << sessionGroupId << controlPort << ctIP << cncIP;
    }
    virtual void unmarshal(const sox::Unpack &p) {
      p >> sessionId >> sessionGroupId >> controlPort >> ctIP >> cncIP;
    }
  };

  struct PChannelMaintainanceRes1 : public sox::Marshallable {
    enum {uri = PChannelMaintainanceRes1URI};
    uint32_t nodeId;  // by Smnode
    uint32_t taskId;  // backfilled by Smnode
    uint32_t resCode;
    std::vector<SessionInfoItem> sInfo;
    PChannelMaintainanceRes1()
      :nodeId(0)
      ,taskId(0)
      ,resCode(0)
      ,sInfo()
    {}
    virtual void marshal(sox::Pack &p) const {
      p << nodeId << taskId << resCode;
      sox::marshal_container(p, sInfo);
    }
    virtual void unmarshal(const sox::Unpack &p) {
      p >> nodeId >> taskId >> resCode;
      sox::unmarshal_container(p, std::back_inserter(sInfo));
    }
  };

	
  struct PJoinChannelCongest : public sox::Marshallable {
    enum {uri = PJoinChannelCongestURI};
		uint32_t serverId;
		uint32_t lookup;
		uint32_t timestamp;
    PJoinChannelCongest()
      :serverId(0)
      ,lookup(0)
      ,timestamp(0)
    {}
    virtual void marshal(sox::Pack &p) const {
      p << serverId << lookup << timestamp;
    }
    virtual void unmarshal(const sox::Unpack &p) {
      p >> serverId >> lookup >> timestamp;
    }
  };

  struct PJoinChannelUidCongest : public sox::Marshallable {
    enum {uri = PJoinChannelUidCongestURI};
		uint32_t serverId;
		uint32_t uid;
    uint32_t lookup;
		uint32_t timestamp;
    PJoinChannelUidCongest()
      :serverId(0)
      ,uid(0)
      ,lookup(0)
      ,timestamp(0)
    {}
    virtual void marshal(sox::Pack &p) const {
      p << serverId << uid << lookup << timestamp;
    }
    virtual void unmarshal(const sox::Unpack &p) {
      p >> serverId >> uid >> lookup >> timestamp;
    }
  };

  struct PJoinChannelLinkdCongest : public sox::Marshallable {
    enum {uri = PJoinChannelLinkdCongestURI};
		uint32_t serverId;
		uint32_t linkdServerId;
    uint32_t lookup;
		uint32_t timestamp;
    PJoinChannelLinkdCongest()
      :serverId(0)
      ,linkdServerId(0)
      ,lookup(0)
      ,timestamp(0)
    {}
    virtual void marshal(sox::Pack &p) const {
      p << serverId << linkdServerId << lookup << timestamp;
    }
    virtual void unmarshal(const sox::Unpack &p) {
      p >> serverId >> linkdServerId >> lookup >> timestamp;
    }
  };

  struct PAllocateChannelCongest : public sox::Marshallable {
    enum {uri = PAllocateChannelCongestURI};
		uint32_t serverId;
		uint32_t allocate;
		uint32_t timestamp;
    PAllocateChannelCongest()
      :serverId(0)
      ,allocate(0)
      ,timestamp(0)
    {}
    virtual void marshal(sox::Pack &p) const {
      p << serverId << allocate << timestamp;
    }
    virtual void unmarshal(const sox::Unpack &p) {
      p >> serverId >> allocate >> timestamp;
    }
  };

  struct PAllocateNodeChange : public sox::Marshallable {
    enum {uri = PAllocateNodeChangeURI};
		uint32_t serverId;
		uint32_t oldAllocateServerId;
		uint32_t newAllocateServerId;
    uint32_t timestamp;
    PAllocateNodeChange()
      :serverId(0)
      ,oldAllocateServerId(0)
      ,newAllocateServerId(0)
      ,timestamp(0)
    {}
    virtual void marshal(sox::Pack &p) const {
      p << serverId << oldAllocateServerId << newAllocateServerId << timestamp;
    }
    virtual void unmarshal(const sox::Unpack &p) {
      p >> serverId >> oldAllocateServerId >> newAllocateServerId >> timestamp;
    }
  };

  struct PServiceProxyCongest : public sox::Marshallable {
    enum {uri = PServiceProxyCongestURI};
		uint32_t serverId;
		uint32_t query;
		uint32_t timestamp;
    PServiceProxyCongest()
      :serverId(0)
      ,query(0)
      ,timestamp(0)
    {}
    virtual void marshal(sox::Pack &p) const {
      p << serverId << query << timestamp;
    }
    virtual void unmarshal(const sox::Unpack &p) {
      p >> serverId >> query >> timestamp;
    }
  };

  struct PRequestConsolePort : public sox::Marshallable {
    enum {uri = PRequestConsolePortURI};
    uint32_t serverId;
    PRequestConsolePort()
     : serverId(0)
    {}
    virtual void marshal(sox::Pack &p) const {
      p << serverId;
    }
    virtual void unmarshal(const sox::Unpack &p) {
      p >> serverId;
    }
  };

  struct PNotifyConsolePort: public sox::Marshallable {
    enum {uri = PNotifyConsolePortURI};
    uint32_t serverId;
    uint32_t consolePort;
    PNotifyConsolePort()
     : serverId(0)
     , consolePort(0)
    {}
    virtual void marshal(sox::Pack &p) const {
      p << serverId << consolePort;
    }
    virtual void unmarshal(const sox::Unpack &p) {
      p >> serverId >> consolePort;
    }
  };


  struct PSessionLbsRes6: public sox::Marshallable {
    enum {uri = PSessionLbsRes6URI};
    PSessionLbsRes6 () {}
    uint32_t encyptType;
    std::string cipher;
    virtual void marshal(sox::Pack &p) const {
      p << encyptType << cipher;
    }
    virtual void unmarshal(const sox::Unpack &p) {
      p >> encyptType >> cipher;
    }
  };

  struct PSignList: public protocol::session::PListMini{
    std::string sign;
    uint32_t jifen;
    virtual void marshal(sox::Pack &pk) const{
      PListMini::marshal(pk);
      pk << sign << jifen;
    }

    virtual void unmarshal(const sox::Unpack &up){
      PListMini::unmarshal(up);
      up >> sign >> jifen;
    }
  };

  struct PSessUserJF : public PSignList{
    uint32_t smemberJifen;
    uint32_t uinfoJifen;
    std::vector<protocol::session::MRolerAndCh> rolers;
    virtual void marshal(sox::Pack &p) const{
      PSignList::marshal(p);
      p << smemberJifen << uinfoJifen;
      sox::marshal_container(p, rolers);
    }
    virtual void unmarshal(const sox::Unpack &p){
      PSignList::unmarshal(p);
      p >> smemberJifen >> uinfoJifen;
      sox::unmarshal_container(p, std::back_inserter(rolers));
    }
  };

struct PInnerSMRoler : public sox::Marshallable 
{
  enum {uri = (1352 << 8 | protocol::SDB_SVID)};
  uint32_t tid;
  std::string context;
  virtual void marshal(sox::Pack & p) const {
    p <<tid;
    p.push_varstr32(context.data(), context.size());
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >>tid;
    context=p.pop_varstr32();
  }
};

struct SMRolerInfo : public sox::Marshallable 
{
	uint32_t uinfoJifen;
	uint32_t smemberJifen;
	std::vector<protocol::session::MRolerAndCh> rolers; // rolers of all subchannels
	virtual void marshal(sox::Pack &p) const 
	{
		p << uinfoJifen << smemberJifen;
		sox::marshal_container(p, rolers);
	}
	virtual void unmarshal(const sox::Unpack &p) 
	{
		p >> uinfoJifen >> smemberJifen;
		sox::unmarshal_container(p, std::back_inserter(rolers));
	}
};
struct AdmRole : public sox::Marshallable 
{
	uint32_t uid;
	uint32_t role;
	virtual void marshal(sox::Pack &p) const 
	{
		p << uid << role;
	}
	virtual void unmarshal(const sox::Unpack &p) 
	{
		p >> uid >> role;
	}	
};
struct PInnerSMRolerRes : public sox::Marshallable 
{
	enum {uri = (1353 << 8 | protocol::SDB_SVID)};
	uint32_t tid;
	std::string context;
	uint32_t rescode;
	std::map<uint32_t, SMRolerInfo> allrole; // uid 2 roler info
	std::vector<AdmRole> alladm;
	std::vector<uint32_t> settle_guilduid;
	virtual void marshal(sox::Pack & p) const
	{
		p <<tid;
		p.push_varstr32(context.data(), context.size());
		p <<rescode;
		sox::marshal_container(p, allrole);
		sox::marshal_container(p, alladm);
		sox::marshal_container(p, settle_guilduid);
	}
	virtual void unmarshal(const sox::Unpack &p)
	{
		p >>tid;
		context=p.pop_varstr32();
		p >>rescode;
		sox::unmarshal_container(p, std::inserter(allrole, allrole.begin()));
		sox::unmarshal_container(p, std::back_inserter(alladm));
		sox::unmarshal_container(p, std::back_inserter(settle_guilduid));
	}
};

struct PInnerJoin2 : public protocol::session::PSessionSid {
  enum {uri = (129 << 8 | protocol::SDB_SVID)};
  virtual void marshal(sox::Pack & p) const {
    protocol::session::PSessionSid::marshal(p);
  }

  virtual void unmarshal(const sox::Unpack &p) {
    protocol::session::PSessionSid::unmarshal(p);
  }
};

struct PInnerJoinRes2 : public sox::Marshallable {
  enum {uri = (129 << 8 | protocol::SDB_SVID)};
  uint32_t sid;
  protocol::session::SidMapProperties sprops;
  //std::vector<protocol::session::Member> members;
  std::vector<uint32_t> subs;
  std::vector<uint32_t> banIds;
  std::vector<std::string> banIps;
  std::vector<protocol::session::AuthCode> authes;
  std::vector<std::string> banPCs;
  uint32_t jiedai;  //接待频道，如果没有设置过则为顶级频道id

  virtual void marshal(sox::Pack & p) const {
    p << sid << sprops;
    //sox::marshal_container(p, members);
    sox::marshal_container(p, subs);
    sox::marshal_container(p, banIds);
    sox::marshal_container(p, banIps);
    sox::marshal_container(p, authes);
    sox::marshal_container(p, banPCs);
    p << jiedai;
  }

  virtual void unmarshal(const sox::Unpack &p) {
    p >> sid >> sprops;
    //sox::unmarshal_container(p, std::back_inserter(members));
    sox::unmarshal_container(p, std::back_inserter(subs));
    sox::unmarshal_container(p, std::back_inserter(banIds));
    sox::unmarshal_container(p, std::back_inserter(banIps));
    sox::unmarshal_container(p, std::back_inserter(authes));
    sox::unmarshal_container(p, std::back_inserter(banPCs));
    p >> jiedai;
  }
  virtual std::ostream & trace(std::ostream & os) const
  {   os <<"class: PInnerJoinRes2" <<std::endl;
    os <<"sid: " <<sid <<std::endl;
    os <<"jiedai: " <<jiedai <<std::endl;
    return os;
  }
};

struct PInnerJoin3 : public sox::Marshallable 
{
  enum { uri = (171<<8 |protocol::SDB_SVID) };
  uint32_t sid;
  std::string m_contex;

  virtual void marshal(sox::Pack & p) const {
    p << sid << m_contex;
  }

  virtual void unmarshal(const sox::Unpack &p) {
    p >> sid >> m_contex;
  }
  virtual std::ostream & trace(std::ostream & os) const
  {   os <<"class: PInnerJoin3" <<std::endl;
    os <<"sid: " <<sid <<std::endl;
    return os;
  }
};

struct PInnerJoinRes3 : public sox::Marshallable
{
  enum { uri = (172<<8 |protocol::SDB_SVID) };
  uint16_t resCode; //0: ok  -1: db fail  1: not exist
  uint32_t sid;
  std::string m_contex;
  protocol::session::SidMapProperties sprops;
  std::vector<uint32_t> subs;
  std::vector<uint32_t> banIds;
  std::vector<std::string> banIps;
  std::vector<protocol::session::AuthCode> authes;
  std::vector<std::string> banPCs;
  uint32_t jiedai;

  virtual void marshal(sox::Pack & p) const {
    p << resCode << sid << m_contex << sprops;
    sox::marshal_container(p, subs);
    sox::marshal_container(p, banIds);
    sox::marshal_container(p, banIps);
    sox::marshal_container(p, authes);
    sox::marshal_container(p, banPCs);
    p << jiedai;
  }

  virtual void unmarshal(const sox::Unpack &p) {
    p >>resCode >> sid >> m_contex >> sprops;
    sox::unmarshal_container(p, std::back_inserter(subs));
    sox::unmarshal_container(p, std::back_inserter(banIds));
    sox::unmarshal_container(p, std::back_inserter(banIps));
    sox::unmarshal_container(p, std::back_inserter(authes));
    sox::unmarshal_container(p, std::back_inserter(banPCs));
    p >> jiedai;
  }
  virtual std::ostream & trace(std::ostream & os) const
  {   os <<"class: PInnerJoinRes3" <<std::endl;
    os <<"sid: " <<sid <<std::endl;
    return os;
  }
};



//130 jifen
struct PInnerMemberUpdated: public protocol::session::PUpdateChanelMember{
  enum {uri = (131 << 8 | protocol::SDB_SVID)};
  uint16_t resCode;
  uint32_t updator;
  virtual void marshal(sox::Pack &p) const {
    PUpdateChanelMember::marshal(p);
    p << resCode << updator;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    PUpdateChanelMember::unmarshal(p);
    p >> resCode >> updator;
  }
  virtual std::ostream & trace(std::ostream & os) const
  {   os <<"class: PInnerMemberUpdated" <<std::endl;
    os <<"resCode: " <<resCode <<std::endl;
    os <<"updator: " <<updator <<std::endl;
    return os;
  }
};


struct PInnerSinfoUpdated: public protocol::session::POnChanelInfoUpdated{
  enum {uri = (132 << 8 | protocol::SDB_SVID)};
  uint16_t resCode;
  virtual void marshal(sox::Pack &p) const {
    protocol::session::POnChanelInfoUpdated::marshal(p);
    p << resCode;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    protocol::session::POnChanelInfoUpdated::unmarshal(p);
    p >> resCode;
  }
  virtual std::ostream & trace(std::ostream & os) const
  {   os <<"class: PInnerSinfoUpdated" <<std::endl;
    os <<"sid: " <<sid <<std::endl;
    os <<"updator: " <<updator <<std::endl;
    os <<"resCode: " <<resCode <<std::endl;
    return os;
  }
};

struct PInnerChannelCreated: public protocol::session::POnSubChannelAdd{
  enum {uri = (133 << 8 | protocol::SDB_SVID)};
  uint16_t resCode;
  virtual void marshal(sox::Pack &p) const {
    protocol::session::POnSubChannelAdd::marshal(p);
    p << resCode;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    protocol::session::POnSubChannelAdd::unmarshal(p);
    p >> resCode;
  }

  virtual std::ostream & trace(std::ostream & os) const
  {   os << "class: PInnerChannelCreated" <<std::endl;
    protocol::session::POnSubChannelAdd::trace(os);
    os <<"resCode: " <<resCode <<std::endl ;
    return os;
  }
};

struct PInnerBanUser: public protocol::session::PUpdateBanID{
  enum {uri = 135 << 8 | protocol::SDB_SVID};
  uint32_t admin;
  uint32_t sid;
  uint16_t resCode;
  virtual void marshal(sox::Pack &p) const {
    protocol::session::PUpdateBanID::marshal(p);
    p << admin << sid << resCode;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    protocol::session::PUpdateBanID::unmarshal(p);
    p >> admin >> sid >> resCode;
  }
  virtual std::ostream & trace(std::ostream & os) const
  {   os <<"class: PInnerBanUser" <<std::endl;
    os <<"admin: " <<admin <<std::endl;
    os <<"sid: " <<sid <<std::endl;
    os <<"resCode: " <<resCode <<std::endl;
    return os;
  }
};

struct PInnerBanUser2: public PInnerBanUser{
  enum {uri = 151 << 8 | protocol::SDB_SVID};
  std::string reason;
  virtual void marshal(sox::Pack &p) const {
    PInnerBanUser::marshal(p);
    p << reason;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    PInnerBanUser::unmarshal(p);
    p >> reason;
  }
};


struct PInnerSyncManagers: public protocol::session::PSyncManagers{
  enum {uri = (138 << 8) | protocol::SDB_SVID};
  uint32_t who;
  virtual void marshal(sox::Pack &p) const{
    protocol::session::PSyncManagers::marshal(p);
    p << who;
  }
  virtual void unmarshal(const sox::Unpack &p){
    protocol::session::PSyncManagers::unmarshal(p);
    p >> who;
  }
};

struct PInnerSyncManagersRes: public protocol::session::POnSyncManagers{
  enum {uri = (138 << 8) | protocol::SDB_SVID};
  uint32_t who;
  uint16_t resCode;
  virtual void marshal(sox::Pack &p) const{
    protocol::session::POnSyncManagers::marshal(p);
    p << who << resCode;
  }
  virtual void unmarshal(const sox::Unpack &p){
    protocol::session::POnSyncManagers::unmarshal(p);
    p >> who >> resCode;
  }
  virtual std::ostream & trace(std::ostream & os) const
  {   os <<"class: PInnerSyncManagersRes" <<std::endl;
    os <<"who: " <<who <<std::endl;
    os <<"resCode: " <<resCode <<std::endl;
    return os;
  }
};


// get the first 1000 session managers data
// the managers' data sorted in this order:
//     (first) ow > vp > ma > ca1 > ca2 (last)
struct PInnerSyncPartialManagers: public protocol::session::PSyncManagers {
    enum { uri = (165 << 8) | protocol::SDB_SVID };
    uint32_t uid;

    virtual void marshal(sox::Pack &pack) const {
        protocol::session::PSyncManagers::marshal(pack);
        pack << uid;
    }

    virtual void unmarshal(const sox::Unpack &pack) {
        protocol::session::PSyncManagers::unmarshal(pack);
        pack >> uid;
    }
};


struct PManagerRole: public sox::Marshallable {
    uint32_t uid;
    uint32_t imid;       // yy number, or yy id, imid
    std::string nick;
    protocol::uinfo::SEX gender;
    protocol::session::ChannelRoler role;

    virtual void marshal(sox::Pack &pack) const {
        pack << uid << imid << nick;
        pack.push_uint8(gender);
        pack.push_uint16(role);
    }

    virtual void unmarshal(const sox::Unpack &unpack) {
        unpack >> uid >> imid >> nick;
        gender = (protocol::uinfo::SEX)unpack.pop_uint8();
        role = (protocol::session::ChannelRoler)unpack.pop_uint16();
    }
};

// resCode:
//     RES_SUCCESS   -- succeed in getting 1000 session managers
//     RES_ENONEXIST -- invalid input sid
//     RES_EUNKNOWN  -- error happens
struct PInnerSyncPartialManagersRes: public sox::Marshallable {
    enum { uri = (166 << 8) | protocol::SDB_SVID };
    uint32_t uid;
    uint32_t sid;
    uint16_t resCode;
    std::vector<PManagerRole> managers;

    virtual void marshal(sox::Pack &pack) const {
        pack << uid << sid << resCode;
        sox::marshal_container(pack, managers);
    }

    virtual void unmarshal(const sox::Unpack &unpack) {
        unpack >> uid >> sid >> resCode;
        sox::unmarshal_container(unpack, std::back_inserter(managers));
    }
};


struct PInnerSyncBans: public sox::Marshallable{
  enum {uri = (139 << 8) | protocol::SDB_SVID};
  uint32_t sid;
  uint32_t who;
  virtual void marshal(sox::Pack &p) const{
    p << sid << who;
  }
  virtual void unmarshal(const sox::Unpack &p){
    p >> sid >> who;
  }
};

struct PInnerSyncBansRes: public protocol::session::POnGetBans{
  enum {uri = (139 << 8) | protocol::SDB_SVID};
  uint32_t who;
  virtual void marshal(sox::Pack &p) const {
    protocol::session::POnGetBans::marshal(p);
    p << who;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    protocol::session::POnGetBans::unmarshal(p);
    p >> who;
  }
};

///////////////////////////////////////////////////////////////////


struct PInnerUpdateChanelMember: public protocol::session::PUpdateChanelMember{
  enum {uri =(140 << 8 | protocol::SDB_SVID)};
  uint32_t tid;
  uint32_t updator;
  virtual void marshal(sox::Pack &p) const {
    protocol::session::PUpdateChanelMember::marshal(p);
    p << tid << updator;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    protocol::session::PUpdateChanelMember::unmarshal(p);
    p >> tid >> updator;
  }
};

struct PInnerUpdateChanelMember2: public PInnerUpdateChanelMember{
  enum {uri =(149 << 8 | protocol::SDB_SVID)};
};

struct PBatchPaiming: public sox::Marshallable{
  enum {uri =(141 << 8 | protocol::SDB_SVID)};
  std::set<uint32_t> sids;
  virtual void marshal(sox::Pack &p) const {
    sox::marshal_container(p, sids);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    sox::unmarshal_container(p, std::inserter(sids, sids.begin()));
  }
};

struct PBatchPaimingRes: public sox::Marshallable{
  enum {uri =(141 << 8 | protocol::SDB_SVID)};
  std::vector<std::pair<uint32_t, uint32_t> > paimins;
  virtual void marshal(sox::Pack &p) const {
    sox::marshal_container(p, paimins);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    sox::unmarshal_container(p, std::back_inserter(paimins));
  }
  virtual std::ostream & trace(std::ostream & os) const
  { return os << "class: PBatchPaimingRes" <<std::endl; }
};

struct PSyncJifen: public sox::Marshallable{
  enum {uri =(142 << 8 | protocol::SDB_SVID)};
  uint32_t sid;
  virtual void marshal(sox::Pack &p) const {
    p << sid;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> sid;
  }
};

struct POnSyncJifen: public PSyncJifen{
  std::string jifen;
  virtual void marshal(sox::Pack &p) const {
    p << sid << jifen;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> sid >> jifen;
  }
  virtual std::ostream & trace(std::ostream & os) const
  { return os << "class: POnSyncJifen" <<std::endl 
      <<"sid: " <<sid <<std::endl
      <<"jifen: " <<jifen <<std::endl; }
};

struct PInnerCreateSubChannel: public protocol::session::PCreateChanel{
  enum {uri =(143 << 8 | protocol::SDB_SVID)};
  uint32_t creator;
  virtual void marshal(sox::Pack &p) const {
    protocol::session::PCreateChanel::marshal(p);
    p << creator;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    protocol::session::PCreateChanel::unmarshal(p);
    p >> creator;
  }
};

struct PSyncUsers: public sox::Marshallable{
  enum {uri = (144 << 8 | protocol::SDB_SVID)};
  std::vector<uint32_t> uids;
  uint32_t sid;
  virtual void marshal(sox::Pack &p) const {
    sox::marshal_container(p, uids);
    p << sid;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    sox::unmarshal_container(p, std::back_inserter(uids));
    p >> sid;
  }
};

// TODO(lzd): to be deleted
struct PCheckSid: public sox::Marshallable{
  enum {uri = (145 << 8 | protocol::SDB_SVID)};
  uint32_t sid;
  uint16_t type;
  std::string from;
  virtual void marshal(sox::Pack &p) const {
    p << sid << type << from;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> sid >> type >> from;
  }
};

// SmDispatcher, 加taskId
struct PCheckSid2: public sox::Marshallable{
  enum {uri = (146 << 8 | protocol::SDB_SVID)};
  uint32_t sid;
  uint32_t taskId;

  virtual void marshal(sox::Pack &p) const {
    p << sid << taskId;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> sid >> taskId;
  }
};

// TODO(lzd): to be deleted
struct PCheckSidRes: public sox::Marshallable{
  enum {uri = (145 << 8 | protocol::SDB_SVID)};
  uint32_t realSid;
  uint32_t realAsid;
  uint16_t serverType;
  uint16_t userNetType;
  std::string from;
  uint16_t resCode;

  virtual void marshal(sox::Pack &p) const {
    p << realSid << realAsid << serverType << userNetType << from << resCode;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> realSid >> realAsid >> serverType >> userNetType >> from >> resCode;
  }
};

// SmDispatcher, 加taskId
struct PCheckSidRes2: public sox::Marshallable{
  enum {uri = (147 << 8 | protocol::SDB_SVID)};
  uint32_t realSid;
  uint32_t realAsid;
  uint16_t serverType;
  uint32_t taskId;
  uint16_t resCode;
  PCheckSidRes2()
    : realSid(0)
    , realAsid(0)
    , serverType(0)
    , taskId(0)
    , resCode(0)
  {}

  virtual void marshal(sox::Pack &p) const {
    p << realSid << realAsid << serverType << taskId << resCode;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> realSid >> realAsid >> serverType >> taskId >> resCode;
  }
  virtual std::ostream & trace(std::ostream & os) const
  { return os << "class: PCheckSidRes2" <<std::endl 
      <<"realSid: " <<realSid <<std::endl
      <<"realAsid: " <<realAsid <<std::endl
      <<"serverType: " <<serverType <<std::endl
      <<"taskId: " <<taskId <<std::endl
      <<"resCode: " <<resCode <<std::endl; }
};

struct PCheckSidRestored: public sox::Marshallable{
  enum {uri = (148 << 8 | protocol::SDB_SVID)};
  uint32_t realSid;
  uint32_t realAsid;
  uint32_t taskId;
  std::string name;
  PCheckSidRestored()
    :realSid(0)
    ,realAsid(0)
    ,taskId(0)
  {}
  virtual void marshal(sox::Pack &p) const {
    p << realSid << realAsid  << taskId << name;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> realSid >> realAsid >> taskId >> name;
  }
};

struct PCheckASidRecyled: public sox::Marshallable{
         enum {uri = (151 << 8 | protocol::SDB_SVID)};
         uint32_t sid;
         uint32_t oldAsid;
         uint32_t newAsid;
         uint32_t taskId;
         std::string name;

  PCheckASidRecyled()
    :sid(0)
    ,oldAsid(0)
    ,newAsid(0)
    ,taskId(0)
  {}
         virtual void marshal(sox::Pack &p) const {
                   p << sid << oldAsid << newAsid  << taskId << name;
         }
         virtual void unmarshal(const sox::Unpack &p) {
                p >> sid >> oldAsid >> newAsid >> taskId >> name;
         }
};


struct PInnerMemberUpdated2: public PInnerMemberUpdated{
  enum {uri =(150 << 8 | protocol::SDB_SVID)};
  uint32_t jifen;
  virtual void marshal(sox::Pack &p) const {
    PInnerMemberUpdated::marshal(p);
    p << jifen;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    PInnerMemberUpdated::unmarshal(p);
    p >> jifen;
  }
};

//typedef ZipMarshal<PInnerJoinRes, 145 << 8 | protocol::SDB_SVID> PInnerJoinZipRes;
typedef ZipMarshal<PInnerJoinRes2, 146 << 8 | protocol::SDB_SVID> PInnerJoinZipRes2;
typedef ZipMarshal<PInnerJoinRes3, 248 << 8 | protocol::SDB_SVID> PInnerJoinZipRes3;

struct PMSmanagerStart2: public sox::Marshallable {
  enum { uri = PMSmanagerStart2URI };
  uint32_t serverID;
  PMSmanagerStart2() : serverID(0) {}
  void marshal(sox::Pack &pk) const{
    pk << serverID;
  }
  void unmarshal(const sox::Unpack &up){
    up >> serverID;
  }
};

struct PMSmanagerStart3: public sox::Marshallable {
  enum { uri = PMSmanagerStart3URI };
  uint32_t serverID;
  PMSmanagerStart3() : serverID(0) {}
  void marshal(sox::Pack &pk) const{
    pk << serverID;
  }
  void unmarshal(const sox::Unpack &up){
    up >> serverID;
  }
};

struct PMSmanagerStart4: public sox::Marshallable {
  enum { uri = PMSmanagerStart4URI };
  uint32_t sessiongroup;
  PMSmanagerStart4() : sessiongroup(0) {}
  void marshal(sox::Pack &pk) const{
    pk << sessiongroup;
  }
  void unmarshal(const sox::Unpack &up){
    up >> sessiongroup;
  }
};

struct PCheckSGonProxyAdmin: public sox::Marshallable {
  enum { uri = PCheckSGonProxyAdminURI };
  uint32_t serverID;
  PCheckSGonProxyAdmin() : serverID(0) {}
  void marshal(sox::Pack &pk) const{
    pk << serverID;
  }
  void unmarshal(const sox::Unpack &up){
    up >> serverID;
  }
};

struct PNotifySGonProxyAdmin: public sox::Marshallable{
  enum {uri = (PNotifySGonProxyAdminURI)};
  uint32_t serverid;
  std::vector<uint32_t> sessionGroups;
  PNotifySGonProxyAdmin()
    :serverid(0)
  {}
  virtual void marshal(sox::Pack &pk) const {
    pk << serverid;
    sox::marshal_container(pk, sessionGroups);
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> serverid;
    sox::unmarshal_container(up, std::back_inserter(sessionGroups));
  }
};

struct PSessionSyncStart : public sox::Marshallable {
  enum {uri = PSessionSyncStartURI};
  uint32_t serverId;
  virtual void marshal(sox::Pack &p) const {
    p << serverId;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> serverId;
  }
};

struct PSessionSync : public sox::Marshallable {
  enum {uri = PSessionSyncURI};
  uint32_t serverId;
  uint8_t step;
  std::set<uint32_t> channels;
  virtual void marshal(sox::Pack &p) const {
    p << serverId;
    p.push_uint8(step);
    sox::marshal_container(p, channels);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> serverId;
    step = p.pop_uint8();
    sox::unmarshal_container(p, std::inserter(channels, channels.begin()));
  }
};


struct PChannelRemoved: public sox::Marshallable{
  enum {uri = PChannelRemovedURI};
  uint32_t serverId;
  uint32_t channelId;

  virtual void marshal(sox::Pack &p) const {
    p << serverId << channelId;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> serverId >> channelId;
  }
};

struct PChannelRemoved2: public PChannelRemoved {
  enum {uri = PChannelRemoved2URI };
  uint32_t sessionGroupId;

  virtual void marshal(sox::Pack &p) const {
    PChannelRemoved::marshal(p);
    p << sessionGroupId;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    PChannelRemoved::unmarshal(p);
    p >> sessionGroupId;
  }
};

struct PCheckDup: public sox::Marshallable{
  enum {uri = PCheckDupURI };
  uint32_t serverId;
  uint32_t channelId;

  virtual void marshal(sox::Pack &p) const {
    p << serverId << channelId;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> serverId >> channelId;
  }
};

struct PCheckDupRes: public sox::Marshallable{
  enum {uri =PCheckDupResURI };
  uint32_t resCode;
  uint32_t channelId;
  virtual void marshal(sox::Pack &p) const {
    p << resCode << channelId;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> resCode >> channelId;
  }
};

// 带公会logo的smanager上线后，这条指令可删除
struct PSessionStartSync: public protocol::session::PSessionSyncInfo{
  enum {uri = PSessionStartSyncURI };
};

struct PBatchSyncUsers: public sox::Marshallable{
  enum {uri = PBatchSyncUsersURI };
  std::set<uint32_t> sids;
  virtual void marshal(sox::Pack &p) const {
    sox::marshal_container(p, sids);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    sox::unmarshal_container(p, std::inserter(sids, sids.begin()));
  }
};

struct PBatchSyncUserRes: public sox::Marshallable{
  enum {uri = PBatchSyncUserResURI };
  std::vector<std::pair<uint32_t, uint32_t> > sidUsers;
  virtual void marshal(sox::Pack &p) const {
    sox::marshal_container(p, sidUsers);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    sox::unmarshal_container(p, std::back_inserter(sidUsers));
  }
};

struct PNotifySessions: public sox::Marshallable{
  enum {uri = (135 << 8 | protocol::SESSION_SVID)};
  uint32_t type; // 广播类型。0:紧急,1:公告,2:广告,3:注意,4:警告,5:错误
  std::string msg; //广播正文
  std::vector<uint32_t> sids; // 频道列表，仅仅当频道类型为"指定频道"有效
  virtual void marshal(sox::Pack &pk) const {
    pk << type << msg;
    sox::marshal_container(pk, sids);
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> type >> msg;
    sox::unmarshal_container(up, std::back_inserter(sids));
  }
};

struct ChannelTypeStrSet: public sox::Marshallable{
    std::set<std::string> typeStrSet;
    virtual void marshal(sox::Pack &pk) const {
    sox::marshal_container(pk, typeStrSet);
  }
  virtual void unmarshal(const sox::Unpack &up) {
    sox::unmarshal_container(up, std::inserter(typeStrSet, typeStrSet.end()));
  }
};

struct PNotifySessions2: public PNotifySessions{
  enum {uri = (3135 << 8 | protocol::SESSION_SVID)};
  std::map<uint16_t, ChannelTypeStrSet > type2typStrSet;
  uint32_t broadcastType; // 频道类型。0:游戏,1:娱乐,9:指定频道id,-1:所有频道
    virtual void marshal(sox::Pack &pk) const {
    PNotifySessions::marshal(pk);
    sox::marshal_container(pk, type2typStrSet);
    pk << broadcastType;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    PNotifySessions::unmarshal(up);
    sox::unmarshal_container(up, std::inserter(type2typStrSet, type2typStrSet.end()));
    if (!up.empty())
    {
      up >> broadcastType;
    }
  }
};

struct PDupSessions: public sox::Marshallable{
  enum {uri = (136 << 8 | protocol::SESSION_SVID)};
  std::vector<uint32_t> sids;
  virtual void marshal(sox::Pack &pk) const {
    sox::marshal_container(pk, sids);
  }
  virtual void unmarshal(const sox::Unpack &up) {
    sox::unmarshal_container(up, std::back_inserter(sids));
  }
};


struct PMSmanagerStart: public sox::Voidmable{
  enum { uri = (137 << 8) | protocol::SESSION_SVID };
};


struct PGetSessUserInfos: public sox::Marshallable{
  enum { uri = (138 << 8) | protocol::SESSION_SVID };
  uint32_t sid;
  std::vector<uint32_t> uids;
  void marshal(sox::Pack &pk) const{
    pk << sid;
    sox::marshal_container(pk, uids);
  }

  void unmarshal(const sox::Unpack &up){
    up >> sid;
    sox::unmarshal_container(up, std::back_inserter(uids));
  }
};

struct PGetSessUserInfosRes: public sox::Marshallable{
  enum { uri = (138 << 8) | protocol::SESSION_SVID };
  uint32_t sid;
  std::vector<PSessUserJF> infos;
  void marshal(sox::Pack &pk) const{
    pk << sid;
    sox::marshal_container(pk, infos);
  }

  void unmarshal(const sox::Unpack &up){
    up >> sid;
    sox::unmarshal_container(up, std::back_inserter(infos));
  }
};

struct PSessUserJF2: public PSessUserJF{
  PSessUserJF2():bSettle(false){}
  bool bSettle;

  virtual void marshal(sox::Pack &p) const{
    PSessUserJF::marshal(p);
    p.push_uint8(bSettle);
  }
  virtual void unmarshal(const sox::Unpack &p){
    PSessUserJF::unmarshal(p);
    bSettle = p.pop_uint8() != 0;
  }
};

struct PGetSessUserInfos2: public PGetSessUserInfos{
  enum { uri = (139 << 8) | protocol::SESSION_SVID };
  void marshal(sox::Pack &pk) const{
    PGetSessUserInfos::marshal(pk);
  }

  void unmarshal(const sox::Unpack &up){
    PGetSessUserInfos::unmarshal(up);
  }
};


struct PGetSessUserInfosRes2: public PGetSessUserInfosRes{
  enum { uri = (139 << 8) | protocol::SESSION_SVID };
  uint32_t sid;
  std::vector<PSessUserJF2> infos;
  void marshal(sox::Pack &pk) const{
    pk << sid;
    sox::marshal_container(pk, infos);
  }

  void unmarshal(const sox::Unpack &up){
    up >> sid;
    sox::unmarshal_container(up, std::back_inserter(infos));
  }
  virtual std::ostream & trace(std::ostream & os) const
  { return os << "class: PGetSessUserInfosRes2" <<std::endl 
      <<"sid: " <<sid <<std::endl; }
};

/************************************************************************/
/* Add By Neou                                                          */
/************************************************************************/

struct PSessionSnapshot : public sox::Marshallable
{
    enum {uri = (200 << 8) | protocol::SESSION_SVID};
    uint32_t sessionid;
    uint32_t version;
    virtual void marshal(sox::Pack &pk) const {
        pk << sessionid << version;
    }
    virtual void unmarshal(const sox::Unpack &upk) {
        upk  >> sessionid >>  version;
    }
};

struct PSessionSnapRep :  public sox::Marshallable
{
    enum {uri = (201 << 8) | protocol::SESSION_SVID};
    uint32_t    rescode;
    uint32_t    sessionid;
    uint32_t    version;

    std::string   sessdata;

    virtual void marshal(sox::Pack &pk) const
    {
        pk << rescode << sessionid << version << sessdata;
    }
    virtual void unmarshal(const sox::Unpack &upk)
    {
        upk >> rescode >> sessionid >> version >> sessdata;
    }
};

struct PInnerBanPC : public protocol::session::PUpdateBanPC{
  enum {uri = 202 << 8 | protocol::SDB_SVID};

  uint16_t resCode;

  uint32_t admin;
  uint32_t sid;
  std::string pcInfo;
  std::string ip;
  std::string user_nick;


  virtual void marshal(sox::Pack &p) const {
    protocol::session::PUpdateBanPC::marshal(p);
    p << resCode << admin << sid << pcInfo << ip << user_nick;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    protocol::session::PUpdateBanPC::unmarshal(p);
    p >> resCode >> admin >> sid >> pcInfo >> ip >> user_nick;
  }
  virtual std::ostream & trace(std::ostream & os) const
  {   os <<"class: PInnerBanPC" <<std::endl;
    os <<"resCode: " <<resCode <<std::endl;
    os <<"admin: " <<admin <<std::endl;
    os <<"sid: " <<sid <<std::endl;
    os <<"pcInfo: " <<pcInfo <<std::endl;
    os <<"ip: " <<ip <<std::endl;
    os <<"user_nick: " <<user_nick <<std::endl;
    return os;
  }
};

struct PInnerBanPCRes : public PInnerBanPC
{

};

struct PInnerBanPC2 : public PInnerBanPC{
  enum {uri = 199 << 8 | protocol::SDB_SVID};

  std::string reason;
  virtual void marshal(sox::Pack &p) const {
    PInnerBanPC::marshal(p);
    p << reason;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    PInnerBanPC::unmarshal(p);
    p >> reason;
  }
};

struct PInnerBanPCRes2: public PInnerBanPC2{

};



struct PInnerSyncBans_banPC : public sox::Marshallable{
  enum {uri = (203 << 8) | protocol::SDB_SVID};
  uint32_t sid;
  uint32_t who;
  virtual void marshal(sox::Pack &p) const{
    p << sid << who;
  }
  virtual void unmarshal(const sox::Unpack &p){
    p >> sid >> who;
  }
};

struct PInnerSyncBansRes_banPC : public protocol::session::POnGetBans_banPC{
  enum {uri = (203 << 8) | protocol::SDB_SVID};
  uint32_t who;

  virtual void marshal(sox::Pack &p) const {
    protocol::session::POnGetBans_banPC::marshal(p);
    p << who;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    protocol::session::POnGetBans_banPC::unmarshal(p);
    p >> who;
  }
};

struct PInnerSyncBanPCsPer24Hours : public sox::Voidmable
{
  enum { uri = (204 << 8) | protocol::SESSION_SVID };

  uint32_t sid;

  virtual void marshal(sox::Pack & p) const {
    p << sid;
  }

  virtual void unmarshal(const sox::Unpack &p) {
    p >> sid;
  }
};

struct PInnerSyncBanPCsPer24HoursRes : public PInnerSyncBanPCsPer24Hours
{
  enum { uri = (205 << 8) | protocol::SESSION_SVID };

  std::set<std::string> banPCInfo;

  virtual void marshal(sox::Pack & p) const {
    sox::marshal_container(p, banPCInfo);
  }

  virtual void unmarshal(const sox::Unpack &p) {
    sox::unmarshal_container(p, std::inserter(banPCInfo, banPCInfo.begin()));
  }
  virtual std::ostream & trace(std::ostream & os) const
  {   os <<"class: PInnerSyncBanPCsPer24HoursRes" <<std::endl;
    return os;
  }
};

struct PSyncRolers : public sox::Marshallable
{
  enum { uri = (206 << 8) | protocol::SESSION_SVID };
  uint32_t uid;
  uint32_t admin;
  uint32_t sid;
  virtual void marshal(sox::Pack & p) const {
    p << uid << admin << sid;
  }

  virtual void unmarshal(const sox::Unpack &p) {
    p >> uid >> admin >> sid;
  }
};

struct PSyncRolersRes: public protocol::session::PNewRolers{
  enum { uri = (206 << 8) | protocol::SESSION_SVID };
};

struct PSyncRolers2 : public PSyncRolers
{
    enum{uri = (16010 << 8 | protocol::SESSION_SVID)};

    protocol::session::SyncRoleReason reason;
    uint32_t reserver1;

    virtual void marshal(sox::Pack & p) const {
       PSyncRolers::marshal(p);
       p << (uint32_t)reason << reserver1;
    }
    virtual void unmarshal(const sox::Unpack &p) {
       PSyncRolers::unmarshal(p);
       reason = (protocol::session::SyncRoleReason)p.pop_uint32();
       p >> reserver1;
    }
};

struct PSyncRolersRes2: public PSyncRolersRes {
    enum{uri = (16011 << 8 | protocol::SESSION_SVID)};

    protocol::session::SyncRoleReason reason;
    uint32_t reserver1;

    virtual void marshal(sox::Pack & p) const {
        PSyncRolersRes::marshal(p);
        p << (uint32_t)reason << reserver1;
    }

    virtual void unmarshal(const sox::Unpack &p) {
       PSyncRolersRes::unmarshal(p);
       reason = (protocol::session::SyncRoleReason)p.pop_uint32();
       p >> reserver1;
    }
};

struct PSetJiedai: public sox::Marshallable{
  enum { uri = (207 << 8) | protocol::SESSION_SVID };
  bool bset; // true set; false remove
  uint32_t subChannelId;
  uint32_t topChannelId;

  uint32_t admin;
  virtual void marshal(sox::Pack & p) const {
    p << bset << subChannelId << topChannelId << admin;
  }

  virtual void unmarshal(const sox::Unpack &p) {
    p >> bset >> subChannelId >> topChannelId >> admin;
  }
};

struct PSetJiedaiRes: public PSetJiedai{
  //bset 无用， 返回频道当前的接待频道
};

struct PGetChannelProperties: public sox::Marshallable
{
  enum {uri = (208 << 8) | protocol::SESSION_SVID};
  uint32_t ChannelProperty;
  uint32_t sid;
  virtual void marshal(sox::Pack &pak) const {
    pak << sid << ChannelProperty;
  }
  virtual void unmarshal(const sox::Unpack &unpak) {
    unpak >> sid >> ChannelProperty;
  }
};

/**
 * 频道操作日志的操作类型
 *
 * @note(lzd): 跟数据库结构对应，值必须从1开始；类型只能加，不能删、修改。
 */
enum SesisonOPType
{
  //由于传输类型里面约束为1个字节，所以这里的范围为1-255.等以后超出，在改传输协议吧。
  SOP_KICK = 1,     // 踢人
  SOP_BANPC,        // 封机器码
  SOP_BANID,        // 封用户id
  SOP_CHANNELROLER,   // 频道的马甲管理
  SOP_CREATESESSION,    // 创建子频道
  SOP_DELSESSION,     // 删除子频道
  SOP_CHANGESESSION,    // 修改子频道公会信息
  SOP_LEAVEGUILD,     // 离开公会
  SOP_CHANGETOPSESSION,   // 修改顶级频道公会信息
  SOP_REJECTADDGUILD,   //拒绝加入公会申请

  //wuji start
  SOP_LOGINSESSION=136,   // 进入频道（网监需求）
  SOP_LOGOUTSESSION   // 离开频道（网监需求）

  //何丰商量过，由于他们那边的工会系统也会有自己定义的类型，所以约定
  //150-255范围的他们用，150以下（包括150）我们用
  //wuji end
};

// 频道操作日志
struct PMsgToLog :  public sox::Marshallable
{
  enum {uri = (250 << 8) | protocol::SESSION_SVID};

  uint32_t    topmost_sid;  // 顶级频道id
  SesisonOPType type;     // 操作类型
  uint32_t    admin_uid;    // 管理员
  std::string   admin_nick;   // 管理员昵称
  uint32_t    id;       // 被操作对象（user or session）
  std::string   id_nick;    // 被操作对象昵称
  std::string   op_event;   // 事件描述
  std::string   remark;     // 备注

  virtual void marshal(sox::Pack &pk) const
  {
    pk << topmost_sid;
    pk.push_uint8(type);
    pk << admin_uid << admin_nick
       << id << id_nick
       << op_event << remark;
  }
  virtual void unmarshal(const sox::Unpack &upk)
  {
    upk >> topmost_sid;
    type = (SesisonOPType)upk.pop_uint8();
    upk >> admin_uid >> admin_nick
      >> id >> id_nick
      >> op_event >> remark;
  }
};

struct PIpMsgToLog :  public sox::Marshallable
{
    enum {uri = (251 << 8) | protocol::SESSION_SVID};
    uint32_t        sid;
    uint32_t        ownerid;
    std::string     ip;
    virtual void marshal(sox::Pack &pk) const
    {
        pk << sid << ownerid << ip;
    }
    virtual void unmarshal(const sox::Unpack &upk)
    {
        upk >> sid >> ownerid >> ip;
    }
};

struct PPassportMsgToLog :  public sox::Marshallable
{
  enum {uri = (252 << 8) | protocol::SESSION_SVID};
  std::string passport;
  std::string app_type;
  std::string ip;
  virtual void marshal(sox::Pack &pk) const
  {
    pk << passport << app_type << ip;
  }
  virtual void unmarshal(const sox::Unpack &upk)
  {
    upk >> passport >> app_type >> ip;
  }
};

struct PInnerSyncBanIDAndBanPC : public sox::Marshallable
{
  enum { uri = (251 << 8) | protocol::SDB_SVID };

  uint32_t topmostSID;

  virtual void marshal(sox::Pack &pk) const
  {
    pk << topmostSID;
  }

  virtual void unmarshal(const sox::Unpack &upk)
  {
    upk >> topmostSID;
  }
};

struct PInnerSyncBanIDAndBanPCRes : public sox::Marshallable
{
  enum { uri = (252 << 8) | protocol::SDB_SVID };

  std::set<uint32_t>    banIDs;
  std::set<std::string> banPCs;

  virtual void marshal(sox::Pack &p) const {
    sox::marshal_container(p, banIDs);
    sox::marshal_container(p, banPCs);
  }

  virtual void unmarshal(const sox::Unpack &p) {
    sox::unmarshal_container(p, std::inserter(banIDs, banIDs.begin()));
    sox::unmarshal_container(p, std::inserter(banPCs, banPCs.begin()));
  }
  virtual std::ostream & trace(std::ostream & os) const
  {   os <<"class: PInnerSyncBanIDAndBanPCRes" <<std::endl;
    return os;
  }
};

struct PSessionStartSync2 : public protocol::session::PSessionSyncInfo2{
  enum {uri = PSessionStartSync2URI };
};

/**
 * Lbs时SmDispatcher和smanager交互的包格式
 *
 * @note:
    若在数据库中找不到记录，smanager对包不作修改，直接原路返回；
    找到记录，则填充完整包的信息。
 */
 struct PCoSessionLbs : public protocol::session::PSessionSid {
  enum {uri = PCoSessionLbsURI};
  ISPType type;
  std::string from;
  uint32_t taskId;
  virtual void marshal(sox::Pack &p) const {
    p << sid;
    p.push_uint16(type);
    p << from << taskId;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> sid;
    type = (ISPType)p.pop_uint16();
    p >> from >> taskId;
  }

};
struct PCoSessionLbsRes : public sox::Marshallable {
  enum {uri = PCoSessionLbsResURI };
  ISPType type;
  uint32_t sessSrvId;
  uint32_t groupId; // Smdispatcher填
  uint32_t smSrvId; // Smdispatcher填
  std::string from; // Smdispatcher填
  uint32_t taskId;  // Smdispatcher填
//  uint32_t UserNum;
  uint32_t sid;
  uint32_t asid;
  std::string ip;
  std::vector<uint16_t> ports;
  uint32_t resCode;
  virtual void marshal(sox::Pack &p) const {
    p.push_uint16(type);
    p << sessSrvId << groupId << smSrvId << from << taskId/* << UserNum*/ << sid << asid << ip ;
    sox::marshal_container(p, ports);
    p << resCode;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    type = (ISPType)p.pop_uint16();
    p >> sessSrvId >> groupId >>smSrvId >> from >> taskId /*>> UserNum*/ >> sid >> asid >> ip ;
    sox::unmarshal_container(p, std::back_inserter(ports));
    p >> resCode;
  }

};

struct PCoSessionLbsRes2 : public PCoSessionLbsRes {
  enum {uri = PCoSessionLbsRes2URI};
  uint32_t sessionGroupId;
  PCoSessionLbsRes2()
    : PCoSessionLbsRes()
    , sessionGroupId(0)
  {}

  virtual void marshal(sox::Pack &p) const {
    PCoSessionLbsRes::marshal(p);
    p << sessionGroupId;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    PCoSessionLbsRes::unmarshal(p);
    p >> sessionGroupId;
  }
};

struct PUpdateSessionInfo : public sox::Marshallable {
  enum {uri = PUpdateSessionInfoURI};

  uint32_t sid;
  uint32_t asid;
  uint32_t sessSrvId;//正确的sessSrvId

  virtual void marshal(sox::Pack &p) const {
    p << sid << asid << sessSrvId;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> sid >> asid >> sessSrvId;
  }
};
struct PUpdateSessionInfo2 : public PUpdateSessionInfo {
  enum {uri = PUpdateSessionInfo2URI};
  uint32_t sessionGroupId;

  PUpdateSessionInfo2()
    : PUpdateSessionInfo()
    , sessionGroupId(0)
  {}
  virtual void marshal(sox::Pack &p) const {
    PUpdateSessionInfo::marshal(p);
    p << sessionGroupId;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    PUpdateSessionInfo::unmarshal(p);
    p >> sessionGroupId;
  }
};
struct PUpdateSessionInfoRes :public sox::Voidmable {
  enum {uri = PUpdateSessionInfoResURI};
};
struct PInjectSessInfo : public sox::Marshallable {
  enum {uri = PInjectSessInfoURI};
  uint32_t sid;
  uint32_t asid;
  uint32_t serverId;//sessSrvId

  virtual void marshal(sox::Pack &p) const {
    p << sid <<asid  << serverId;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> sid >>asid >> serverId;
  }
};
struct PInjectSessInfo2 : public PInjectSessInfo {
  enum {uri = PInjectSessInfo2URI};
  uint32_t sessionGroupId;

  PInjectSessInfo2()
    : PInjectSessInfo()
    , sessionGroupId(0)
  {}

  virtual void marshal(sox::Pack &p) const {
    PInjectSessInfo::marshal(p);
    p << sessionGroupId;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    PInjectSessInfo::unmarshal(p);
    p >> sessionGroupId;
  }
};

struct PInjectSessInfoRes : public sox::Voidmable {
  enum {uri = PInjectSessInfoResURI};
};
struct PAllocateChanel :public sox::Marshallable {
  enum {uri = PAllocateChanelURI};
  uint32_t taskId;
  uint32_t sid;
  uint32_t asid;
  ISPType cType;
  ISPType sType;

  virtual void marshal(sox::Pack &p) const {
    p.push_uint16(cType);
    p.push_uint16(sType);
    p << sid << asid<< taskId ;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    cType = (ISPType)p.pop_uint16();
    sType = (ISPType)p.pop_uint16();
    p >> sid >> asid >>taskId ;
  }

};

struct PAllocatechannelRes:public PCoSessionLbsRes{
  enum {uri = PAllocatechannelResURI};
};
struct PAllocatechannelRes2 : public PAllocatechannelRes{
  enum {uri = PAllocatechannelRes2URI};
  uint32_t sessionGroupId;
  PAllocatechannelRes2()
    : PAllocatechannelRes()
    , sessionGroupId(0)
  {}

  virtual void marshal(sox::Pack &p) const {
    PAllocatechannelRes::marshal(p);
    p << sessionGroupId;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    PAllocatechannelRes::unmarshal(p);
    p >> sessionGroupId;
  }
};

struct PDetectTask:public sox::Voidmable {
  enum {uri = PDetectTaskURI};
};
struct PDetectTaskRes:public sox::Voidmable {
  enum {uri = PDetectTaskResURI};
  uint32_t smSrvId;
  uint32_t groupId;
  virtual void marshal(sox::Pack &p) const {
    p << smSrvId << groupId;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> smSrvId >> groupId;
  }
};
//冲突时查人数
struct PChkUserNum:public sox::Voidmable{
  enum {uri = PChkUserNumURI};
  uint32_t taskId;
  uint32_t sid;
  virtual void marshal(sox::Pack &p) const {
    p << taskId << sid;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> taskId >> sid;
  }
};
struct PChkUserNumRes:public sox::Voidmable{
  enum {uri = PChkUserNumResURI};
  uint32_t taskId;
  uint32_t serverId;
  uint32_t sid;
  uint32_t usrNum;
  virtual void marshal(sox::Pack &p) const {
    p << taskId << serverId << sid << usrNum;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> taskId >> serverId >> sid >> usrNum;
  }
};

struct PChkUserNumRes2:public sox::Voidmable{
  enum {uri = PChkUserNumRes2URI};
  uint32_t taskId;
  uint32_t resCode;
  uint32_t serverId;
  uint32_t sessionId; 
  uint32_t sgid;
  uint32_t sid;
  uint32_t usrNum;
  uint32_t reserved1;
  
  PChkUserNumRes2()
    : taskId(0)
    , resCode(0)
    , serverId(0)
    , sessionId(0)
    , sgid(0)
    , sid(0)
    , usrNum(0)
    , reserved1(0)
  {}
  
  virtual void marshal(sox::Pack &p) const {
    p << taskId << resCode << serverId << sessionId << sgid << sid << usrNum << reserved1;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> taskId >> resCode >> serverId >> sessionId >> sgid >> sid >> usrNum >> reserved1;
  }
};

//////////////////////////////////////////////////////////////////////////
//hide session for smdispatcher-smanager
struct PCoSessionLbsHS : public protocol::session::PSessionSid {
  enum {uri = PCoSessionLbsHSURI};
  uint32_t taskId;
  virtual void marshal(sox::Pack &p) const {
    p << sid << taskId;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> sid >> taskId;
  }
};

struct PCoSessionLbsHS2 : public PCoSessionLbsHS {
  enum { uri = PCoSessionLbsHS2URI };
  virtual void marshal( sox::Pack & p ) const {
    PCoSessionLbsHS ::marshal(p);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    PCoSessionLbsHS ::unmarshal(p);
  }
};

struct PCoSessionLbsHS3 : public PCoSessionLbsHS2 {
  enum { uri = PCoSessionLbsHS3URI };
  uint32_t sendTimeStamp;
  uint32_t reserved1;
  uint32_t reserved2;  
  PCoSessionLbsHS3()
    :sendTimeStamp(0)
    ,reserved1(0)
    ,reserved2(0)
  {
  }
  virtual void marshal( sox::Pack & p ) const {
    PCoSessionLbsHS2 ::marshal(p);
    p << sendTimeStamp << reserved1 << reserved2;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    PCoSessionLbsHS2 ::unmarshal(p);
    p >> sendTimeStamp >> reserved1 >> reserved2;
  }
};

struct PCoSessionLbsResHS : public sox::Marshallable {
  enum {uri = PCoSessionLbsResHSURI};
  uint32_t groupId; // Smdispatcher填
  uint32_t smSrvId; // Smdispatcher填
  uint32_t taskId;  // Smdispatcher填
  uint32_t sid;
  uint32_t asid;
  uint32_t sessSrvId;
  std::string sessionResPack;
  uint32_t timeStamp;
  uint32_t resCode;

  PCoSessionLbsResHS()
    : groupId(0)
    , smSrvId(0)
    , taskId(0)
    , sid(0)
    , asid(0)
    , sessSrvId(0)
    , sessionResPack()
    , timeStamp(0)
    , resCode(0)
  {}

  virtual void marshal(sox::Pack &p) const {
    p << sessSrvId << groupId << smSrvId << taskId << sid << asid << sessionResPack << timeStamp ;
    p << resCode;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> sessSrvId >> groupId >>smSrvId >> taskId >> sid >> asid >> sessionResPack >> timeStamp;
    p >> resCode;
  }

};

//This is one of possible responses for PAllocateChannelHS2 to notice
//simdispacher that channel is freezed and its defreezed time.
struct PChannelFrozen : public sox::Marshallable {
  enum {uri = PChannelFrozenURI};
  PChannelFrozen()
    : groupId(0)
    , smSrvId(0)
    , taskId(0)
    , sid(0)
    , asid(0)
    , unfrozenTime(0)
    , resCode(0) //0 is not a valid resCode.
  {}

  uint32_t groupId; // Smdispatcher填
  uint32_t smSrvId; // Smdispatcher填
  uint32_t taskId;  // Smdispatcher填
  uint32_t sid;
  uint32_t asid;
  uint32_t unfrozenTime;
  uint32_t resCode;
  virtual void marshal(sox::Pack &p) const {
    p << groupId << smSrvId << taskId << sid << asid << unfrozenTime ;
    p << resCode;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> groupId >>smSrvId >> taskId >> sid >> asid >> unfrozenTime;
    p >> resCode;
  }

};

struct PSManagerUpdate : public sox::Marshallable {
  enum { uri = PSManagerUpdateURI };
  std::string source;
  std::string key;
  std::string value;
  uint32_t    timeStamp;
  PSManagerUpdate()
    : source()
    , key()
    , value()
    , timeStamp(0)
  {}

  virtual void marshal(sox::Pack &p) const {
    p << source << key << value << timeStamp;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> source >> key >> value >> timeStamp;
  }
};

struct PSMProxyPing : public sox::Marshallable {
  enum { uri = PSMProxyPingURI };
  uint32_t    pingTime;
  PSMProxyPing()
    : pingTime(0)
  {}

  virtual void marshal(sox::Pack &p) const {
    p << pingTime;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> pingTime;
  }
};

struct PSMProxyPong : public sox::Marshallable {
  enum { uri = PSMProxyPongURI };
  uint32_t    pingTime;
  PSMProxyPong()
    : pingTime(0)
  {}

  virtual void marshal(sox::Pack &p) const {
    p << pingTime;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> pingTime;
  }
};


struct PSManagerClear : public sox::Marshallable {
  enum { uri = PSManagerClearURI };
  uint32_t    serverId;
  std::string key;
  PSManagerClear()
    : serverId(0)
    , key()
  {}
  virtual void marshal(sox::Pack &p) const {
    p << serverId << key;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> serverId >> key;
  }
};

struct PUpdateSmanagerSgConfig : public sox::Marshallable{
  enum {uri = PUpdateSmanagerSgConfigURI };
  uint32_t sgid;
  std::string config;
  uint32_t reserved1;
  uint32_t reserved2;

  PUpdateSmanagerSgConfig()
    :sgid(0)
    ,config()
    ,reserved1(0)
    ,reserved2(0)
  {}
  virtual void marshal(sox::Pack &p) const {
    p << sgid << config << reserved1 << reserved2;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> sgid >> config >> reserved1 >> reserved2;
  }
};

struct PRemoveSmanagerSgConfig : public sox::Marshallable{
  enum {uri = PRemoveSmanagerSgConfigURI };
  uint32_t sgid;
  uint32_t reserved1;
  uint32_t reserved2;

  PRemoveSmanagerSgConfig()
    :sgid(0)
    ,reserved1(0)
    ,reserved2(0)
  {}
  virtual void marshal(sox::Pack &p) const {
    p << sgid << reserved1 << reserved2;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> sgid >> reserved1 >> reserved2;
  }
};

struct PUpdateSmanagerChannelConfig : public sox::Marshallable{
  enum {uri = PUpdateSmanagerChannelConfigURI };
  uint32_t sid;
  std::string config;
  uint32_t reserved1;
  uint32_t reserved2;

  PUpdateSmanagerChannelConfig()
    :sid(0)
    ,config()
    ,reserved1(0)
    ,reserved2(0)
  {}
  virtual void marshal(sox::Pack &p) const {
    p << sid << config << reserved1 << reserved2;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> sid >> config >> reserved1 >> reserved2;
  }
};

struct PRemoveSmanagerChannelConfig : public sox::Marshallable{
  enum {uri = PRemoveSmanagerChannelConfigURI };
  uint32_t sid;
  uint32_t reserved1;
  uint32_t reserved2;

  PRemoveSmanagerChannelConfig()
    :sid(0)
    ,reserved1(0)
    ,reserved2(0)
  {}
  virtual void marshal(sox::Pack &p) const {
    p << sid << reserved1 << reserved2;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> sid >> reserved1 >> reserved2;
  }
};

struct PCoSessionLbsResHS2 : public PCoSessionLbsResHS {
  enum {uri = PCoSessionLbsResHS2URI};
  uint32_t sessionGroupId;
  PCoSessionLbsResHS2()
    : PCoSessionLbsResHS()
    , sessionGroupId(0)
  {}

  virtual void marshal(sox::Pack &p) const {
    PCoSessionLbsResHS::marshal(p);
    p << sessionGroupId;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    PCoSessionLbsResHS::unmarshal(p);
    p >> sessionGroupId;
  }
};

struct PCoSessionLbsResHS3 : public PCoSessionLbsResHS2 {
	enum {uri = PCoSessionLbsResHS3URI};
	uint32_t sendTimeStamp;
  uint32_t reserved1;
  uint32_t reserved2;
  PCoSessionLbsResHS3()
    : PCoSessionLbsResHS2()
    , sendTimeStamp(0)
    , reserved1(0)
    , reserved2(0)
  {}

	virtual void marshal(sox::Pack &p) const {
    PCoSessionLbsResHS2::marshal(p);
    p << sendTimeStamp << reserved1 << reserved2;
	}
	virtual void unmarshal(const sox::Unpack &p) {
    PCoSessionLbsResHS2::unmarshal(p);
    p >> sendTimeStamp >> reserved1 >> reserved2;
	}
};

struct PAllocateChanelHS :public sox::Marshallable {
  enum {uri = PAllocateChanelHSURI};
  uint32_t taskId;
  uint32_t sid;
  uint32_t asid;
  ISPType sType;

  virtual void marshal(sox::Pack &p) const {
    p.push_uint16(sType);
    p << sid << asid<< taskId ;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    sType = (ISPType)p.pop_uint16();
    p >> sid >> asid >>taskId ;
  }
};

struct PAllocateChannel2 : public sox::Marshallable {
  enum {uri = PAllocateChannel2URI};
  uint32_t taskId;
  uint32_t sid;
  uint32_t asid;    // no use after pre-process
  uint16_t deploy;  // no use after shard
  uint32_t hint;    // for fix channel

  PAllocateChannel2()
    : taskId(0)
    , sid(0)
    , asid(0)
    , deploy(0)
    , hint(0)
  {}

  virtual void marshal(sox::Pack &p) const {
    p << taskId << sid << asid << deploy << hint;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> taskId >> sid >> asid >> deploy >> hint;
  }
};

struct PAllocateChannelHS2 : public PAllocateChanelHS {
  enum { uri = PAllocateChannelHS2URI };
  virtual void marshal(sox::Pack &p) const {
    PAllocateChanelHS::marshal(p);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    PAllocateChanelHS::unmarshal(p);
  }
};
struct PAllocatechannelResHS : public PCoSessionLbsResHS{
  enum {uri = PAllocatechannelResHSURI };
};
struct PAllocatechannelResHS2 : public PAllocatechannelResHS{
  enum {uri = PAllocatechannelResHS2URI };
  uint32_t sessionGroupId;

  PAllocatechannelResHS2()
    : PAllocatechannelResHS()
    , sessionGroupId(0)
  {}

  virtual void marshal(sox::Pack &p) const {
    PAllocatechannelResHS::marshal(p);
    p << sessionGroupId;
  }

  virtual void unmarshal(const sox::Unpack &p) {
    PAllocatechannelResHS::unmarshal(p);
    p >> sessionGroupId;
  }
};

struct PUpdateSessionInfoHS : public sox::Marshallable {
  enum {uri = PUpdateSessionInfoHSURI};

  uint32_t sid;
  uint32_t asid;
  uint32_t sessSrvId;//正确的sessSrvId
  std::string sessionResPack;
  uint32_t timeStamp;
  virtual void marshal(sox::Pack &p) const {
    p << sid << asid << sessSrvId << sessionResPack << timeStamp;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> sid >> asid >> sessSrvId >> sessionResPack >> timeStamp;
  }
};

struct PUpdateSessionInfoHS2 : public PUpdateSessionInfoHS {
  enum {uri = PUpdateSessionInfoHS2URI };
  uint32_t sessionGroupId;
  PUpdateSessionInfoHS2()
    : PUpdateSessionInfoHS()
    , sessionGroupId(0)
  {}

  virtual void marshal(sox::Pack &p) const {
    PUpdateSessionInfoHS::marshal(p);
    p << sessionGroupId;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    PUpdateSessionInfoHS::unmarshal(p);
    p >> sessionGroupId;
  }
};

struct PForceUpdateSessionInfoHS : public PUpdateSessionInfoHS2 {
  enum {uri = PForceUpdateSessionInfoHSURI};
  PForceUpdateSessionInfoHS()
    : PUpdateSessionInfoHS2()
  {}

  virtual void marshal(sox::Pack &p) const {
    PUpdateSessionInfoHS2::marshal(p);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    PUpdateSessionInfoHS2::unmarshal(p);
  }
};

struct PInjectSessInfoHS : public sox::Marshallable {
  enum {uri = PInjectSessInfoHSURI };
  uint32_t sid;
  uint32_t asid;
  uint32_t serverId;//sessSrvId
  std::string sessionResPack;
  uint32_t timeStamp;
  virtual void marshal(sox::Pack &p) const {
    p << sid <<asid  << serverId << sessionResPack << timeStamp;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> sid >>asid >> serverId >> sessionResPack >> timeStamp;
  }
};
struct PInjectSessInfoHS2 : public PInjectSessInfoHS {
  enum {uri = PInjectSessInfoHS2URI };
  uint32_t sessionGroupId;
  PInjectSessInfoHS2()
    : PInjectSessInfoHS()
    , sessionGroupId(0)
  {}

  virtual void marshal(sox::Pack &p) const {
    PInjectSessInfoHS::marshal(p);
    p << sessionGroupId;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    PInjectSessInfoHS::unmarshal(p);
    p >> sessionGroupId;
  }
};
struct PInjectSessInfoHS3 : public PInjectSessInfoHS2 {
  enum {uri = PInjectSessInfoHS3URI };
	uint32_t smSrvId;
	bool isAllocated;
  uint32_t reserved1;
  uint32_t reserved2;
  PInjectSessInfoHS3()
    : PInjectSessInfoHS2()
		, smSrvId(0)
		, isAllocated(0)
    , reserved1(0)
    , reserved2(0)
  {}

  virtual void marshal(sox::Pack &p) const {
    PInjectSessInfoHS2::marshal(p);
    p << smSrvId << isAllocated << reserved1 << reserved2;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    PInjectSessInfoHS2::unmarshal(p);
    p >> smSrvId >> isAllocated >> reserved1 >> reserved2;
  }
};

struct PInjectSessInfoHS4 : public sox::Marshallable {
  enum {uri = PInjectSessInfoHS4URI };
  uint32_t sid;
  uint32_t asid;
  uint32_t serverId;//session serverid
  uint32_t sessionGroupId;
  uint32_t smSrvId; //smnode serverid
  uint32_t timeStamp;
  uint32_t allocateMode; //0, old; 1, new;
  uint32_t reserved2;
  bool isAllocated;//if channel is allocated before

  virtual void marshal(sox::Pack& p) const
  {
    p << sid << asid << serverId << sessionGroupId << smSrvId << timeStamp << allocateMode << reserved2 << isAllocated;
  }

  virtual void unmarshal(const sox::Unpack& up)
  {
    up >> sid >> asid >> serverId >> sessionGroupId >> smSrvId >> timeStamp >> allocateMode >> reserved2 >> isAllocated;
  }
};

struct PCoSessionLbsHS4 : public PCoSessionLbsHS2 {//新消息 local lookup
  enum { uri = PCoSessionLbsHS4URI };
  uint32_t sendTimeStamp;
  uint32_t reserved1;
  uint32_t reserved2;  
  PCoSessionLbsHS4()
    : PCoSessionLbsHS2()
    , sendTimeStamp(0)
    , reserved1(0)
    , reserved2(0)
  {}
  virtual void marshal( sox::Pack & p ) const {
    PCoSessionLbsHS2 ::marshal(p);
    p << sendTimeStamp << reserved1 << reserved2;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    PCoSessionLbsHS2 ::unmarshal(p);
    p >> sendTimeStamp >> reserved1 >> reserved2;
  }
};

struct PCoSessionLbsResHS4 : public PCoSessionLbsResHS2 {//增加频道类型和封锁/冻结频道到期时间 回复local lookup
  enum {uri = PCoSessionLbsResHS4URI};
  uint32_t sendTimeStamp;
  uint32_t channelState;
  uint32_t expiredTime;
  uint32_t reserved1;
  uint32_t reserved2;
  PCoSessionLbsResHS4()
    : PCoSessionLbsResHS2()
    , sendTimeStamp(0)
    , channelState(0)
    , expiredTime(0)
    , reserved1(0)
    , reserved2(0)
  {}

  virtual void marshal(sox::Pack &p) const {
    PCoSessionLbsResHS2::marshal(p);
    p << sendTimeStamp << channelState << expiredTime << reserved1 << reserved2;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    PCoSessionLbsResHS2::unmarshal(p);
    p >> sendTimeStamp >> channelState >> expiredTime >> reserved1 >> reserved2;
  }
};

struct PCoSessionLbsHS5 : public PCoSessionLbsHS {//新消息 global lookup
  enum { uri = PCoSessionLbsHS5URI };
  uint32_t sendTimeStamp;
  uint32_t reserver1;
  uint32_t reserver2;
  PCoSessionLbsHS5()
    : PCoSessionLbsHS()
    , sendTimeStamp(0)
    , reserver1(0)
    , reserver2(0)
  {}
  virtual void marshal( sox::Pack & p ) const {
    PCoSessionLbsHS ::marshal(p);
    p << sendTimeStamp << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    PCoSessionLbsHS ::unmarshal(p);
    p >> sendTimeStamp >> reserver1 >> reserver2;
  }
};

struct PCoSessionLbsResHS5 : public PCoSessionLbsResHS2 {//增加频道类型和封锁/冻结频道到期时间 回复global lookup
  enum {uri = PCoSessionLbsResHS5URI};
  uint32_t sendTimeStamp;
  uint32_t channelState;
  uint32_t expiredTime;
  uint32_t users;
  uint32_t reserver2;
  PCoSessionLbsResHS5()
    : PCoSessionLbsResHS2()
    , sendTimeStamp(0)
    , channelState(0)
    , expiredTime(0)
    , users(0)
    , reserver2(0)
  {}

  virtual void marshal(sox::Pack &p) const {
    PCoSessionLbsResHS2::marshal(p);
    p << sendTimeStamp << channelState << expiredTime << users << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    PCoSessionLbsResHS2::unmarshal(p);
    p >> sendTimeStamp >> channelState >> expiredTime >> users >> reserver2;
  }
};

struct PAllocateChannelHS3 : public sox::Marshallable {//新消息 allocate channel
  enum {uri = PAllocateChannelHS3URI};
  uint32_t taskId;
  uint32_t sid;
  uint16_t deploy;
  uint32_t sendTimeStamp;
  uint32_t reserver1;
  uint32_t reserver2;

  PAllocateChannelHS3()
  : taskId(0)
  , sid(0)
  , deploy(0)
  , sendTimeStamp(0)
  , reserver1(0)
  , reserver2(0)
  {}

  virtual void marshal(sox::Pack &p) const {
    p << sendTimeStamp << taskId << sid << deploy << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> sendTimeStamp >> taskId >> sid >> deploy >> reserver1 >> reserver2;
  }
};

struct PAllocatechannelResHS3 : public PAllocatechannelResHS{
  enum {uri = PAllocatechannelResHS3URI };
  uint32_t sessionGroupId;
  uint32_t sendTimeStamp;
  uint32_t channelState;
  uint32_t expiredTime;
  uint32_t reserved1;
  uint32_t reserved2;

  PAllocatechannelResHS3()
    : PAllocatechannelResHS()
    , sessionGroupId(0)
    , sendTimeStamp(0)
    , channelState(0)
    , expiredTime(0)
    , reserved1(0)
    , reserved2(0)
  {}

  virtual void marshal(sox::Pack &p) const {
    PAllocatechannelResHS::marshal(p);
    p << sendTimeStamp << sessionGroupId << channelState << expiredTime << reserved1 << reserved2;
  }

  virtual void unmarshal(const sox::Unpack &p) {
    PAllocatechannelResHS::unmarshal(p);
    p >> sendTimeStamp >> sessionGroupId >> channelState >> expiredTime >> reserved1 >> reserved2;
  }
};

struct PSyncAllowUid:public sox::Marshallable {
  enum {uri = PSyncAllowUidURI};
  uint32_t targetServerId;
  std::set<uint32_t> allowUid;
  virtual void marshal(sox::Pack &p) const {
    p << targetServerId;
    sox::marshal_container(p, allowUid);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> targetServerId;
    sox::unmarshal_container(p, std::inserter(allowUid, allowUid.begin()));
  }
};
struct PSyncAllowUidAck:public sox::Marshallable {
  enum {uri = PSyncAllowUidAckURI};
  uint32_t targetServerId;
  uint32_t serverId;
  virtual void marshal(sox::Pack &p) const {
    p << targetServerId << serverId;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> targetServerId >> serverId;
  }
};

struct PSyncChannelConfig:public sox::Marshallable {
  enum {uri = PSyncChannelConfigURI};
  uint32_t targetServerId;
  std::map<uint32_t, std::string> channelConfig;
  virtual void marshal(sox::Pack &p) const {
    p << targetServerId;
    sox::marshal_container(p, channelConfig);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> targetServerId;
    sox::unmarshal_container(p, std::inserter(channelConfig, channelConfig.begin()));
  }
};
struct PSyncChannelConfigAck:public sox::Marshallable {
  enum {uri = PSyncChannelConfigAckURI};
  uint32_t targetServerId;
  uint32_t serverId;
  virtual void marshal(sox::Pack &p) const {
    p << targetServerId << serverId;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> targetServerId >> serverId;
  }
};

struct PSyncSgConfig:public sox::Marshallable {
  enum {uri = PSyncSgConfigURI};
  uint32_t targetServerId;
  std::map<uint32_t, std::string> sgConfig;
  virtual void marshal(sox::Pack &p) const {
    p << targetServerId;
    sox::marshal_container(p, sgConfig);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> targetServerId;
    sox::unmarshal_container(p, std::inserter(sgConfig, sgConfig.begin()));
  }
};
struct PSyncSgConfigAck:public sox::Marshallable {
  enum {uri = PSyncSgConfigAckURI};
  uint32_t targetServerId;
  uint32_t serverId;
  virtual void marshal(sox::Pack &p) const {
    p << targetServerId << serverId;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> targetServerId >> serverId;
  }
};
struct PChkUserNumHS:public PChkUserNum{//仅为BEGIN_FORM_MAP兼容
  enum {uri = PChkUserNumHSURI };
};
struct PChkUserNumResHS:public PChkUserNumRes{//仅为BEGIN_FORM_MAP兼容
  enum {uri = PChkUserNumResHSURI };
};

struct PReqRatedSpIps : public sox::Marshallable
{
  enum {uri = PReqRatedSpIpsURI};
  uint32_t version;
  uint32_t uid;
  uint32_t ip;

  PReqRatedSpIps():version(0),uid(0),ip(0){}

  virtual void marshal(sox::Pack &pk) const
  {
    pk << version << uid << ip;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> version >> uid >> ip;
  }
};

struct PReqRatedSpIpsRes : public sox::Marshallable
{
  enum {uri = PReqRatedSpIpsResURI};

  enum AlgorithmType
  {
    // load * score
    Aggressive = 0,
    // load * (1 + score)
    Conservative = 1,
  };

  uint32_t minMultiplexSpIpNum;
  uint32_t minMultiplexSpLoad;
  // refresh time is in second
  uint32_t refreshTime;
  uint32_t multiplexAlgo;
  uint32_t mSAlgo;
  // <Ip, score>, for now score is a percentage
  std::map<uint32_t, uint32_t> spIps;
  uint32_t minMsSpIpNum;
  uint32_t minMsSpLoad;    

  PReqRatedSpIpsRes(): minMultiplexSpIpNum(0xffffffff),  minMultiplexSpLoad(0xffffffff), refreshTime(0xffffffff), multiplexAlgo(Aggressive), mSAlgo(Conservative), minMsSpIpNum(0xffffffff), minMsSpLoad(0xffffffff){}

  virtual void marshal(sox::Pack &pk) const
  {
    pk << minMultiplexSpIpNum << minMultiplexSpLoad << refreshTime << multiplexAlgo << mSAlgo;
    sox::marshal_container(pk, spIps);
    pk << minMsSpIpNum << minMsSpLoad;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> minMultiplexSpIpNum >>  minMultiplexSpLoad >> refreshTime >> multiplexAlgo >> mSAlgo;
    sox::unmarshal_container(up, std::inserter(spIps, spIps.begin()));
    if (!up.empty())
    {
      up >> minMsSpIpNum;
    }
    if (!up.empty())
    {
      up >> minMsSpLoad;
    }
  }
};

struct PDetectTaskDc2:public sox::Voidmable {
  enum {uri = PDetectTaskDc2URI };
  uint32_t nNodeIdSend;
  uint32_t smSrvIdSend;
  uint32_t startTime;
  virtual void marshal(sox::Pack &p) const {
    p << nNodeIdSend << smSrvIdSend << startTime;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> nNodeIdSend >> smSrvIdSend >> startTime;
  }
};
struct PDetectTaskResDc2:public sox::Voidmable {
  enum {uri = PDetectTaskResDc2URI };
  uint32_t nNodeIdSend;
  uint32_t smSrvIdSend;
  uint32_t smSrvIdRecv;
  uint32_t groupId;
  uint32_t startTime;
  virtual void marshal(sox::Pack &p) const {
    p << nNodeIdSend << smSrvIdSend << smSrvIdRecv << groupId << startTime;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> nNodeIdSend >> smSrvIdSend >> smSrvIdRecv >> groupId >> startTime;
  }
};
struct PSmnodePing : public sox::Marshallable {
  enum {uri = PSmnodePingURI };
  uint32_t targetServerId;
  uint32_t startTime;
  PSmnodePing()
    : targetServerId(0)
    , startTime(0)
  {}
  virtual void marshal(sox::Pack &p) const {
    p << targetServerId << startTime;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> targetServerId >> startTime;
  }
};
struct PSmnodePong : public sox::Marshallable {
  enum {uri = PSmnodePongURI };
  uint32_t targetServerId;
  uint32_t serverId;
  uint32_t groupId;
  uint32_t startTime;
  uint16_t shardId;

  PSmnodePong()
    : targetServerId(0)
    , serverId(0)
    , groupId(0)
    , startTime(0)
    , shardId(0)
  {}
  virtual void marshal(sox::Pack &p) const {
    p << targetServerId << serverId << groupId << startTime << shardId;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> targetServerId >> serverId >> groupId >> startTime >> shardId;
  }
};

struct PSmnodeStatusPing : public sox::Marshallable {
  enum {uri = PSmnodeStatusPingURI };
  uint32_t targetServerId;
  uint64_t timeStamp;
  PSmnodeStatusPing()
    : targetServerId(0)
    , timeStamp(0)
  {}
  virtual void marshal(sox::Pack &p) const {
    p << targetServerId << timeStamp;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> targetServerId >> timeStamp;
  }
};

struct PSmnodeStatusPong : public sox::Marshallable {
  enum {uri = PSmnodeStatusPongURI };
  uint32_t targetServerId;
  uint32_t serverId;
  uint32_t groupId;
  uint64_t timeStamp;
  uint32_t capacity;

  PSmnodeStatusPong()
    : targetServerId(0)
    , serverId(0)
    , groupId(0)
    , timeStamp(0)
    , capacity(0)
  {}
  virtual void marshal(sox::Pack &p) const {
    p << targetServerId << serverId << groupId << timeStamp << capacity;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> targetServerId >> serverId >> groupId >> timeStamp >> capacity;
  }
};

struct PSmObserverPong : public sox::Marshallable {
  enum {uri = PSmObserverPongURI };
  uint32_t targetServerId;
  uint32_t serverId;
  uint32_t groupId;
  uint32_t startTime;

  std::map<std::string, uint16_t>   session_deploys;
  std::map<std::string, uint16_t>   session_shards;

  std::map<uint16_t, uint16_t>      channel_deploy_shards;
  std::map<uint16_t, std::pair<uint32_t, uint32_t> >     channel_sid_shards;
  uint16_t                          channel_default_shard;

  PSmObserverPong()
    : targetServerId(0)
    , serverId(0)
    , groupId(0)
    , startTime(0)
    , channel_default_shard(0)
  {}
  virtual void marshal(sox::Pack &p) const {
    p << targetServerId << serverId << groupId << startTime;
    marshal_container( p, session_deploys );
    marshal_container( p, session_shards );
    marshal_container( p, channel_deploy_shards );
    marshal_container( p, channel_sid_shards );
    p << channel_default_shard;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> targetServerId >> serverId >> groupId >> startTime;
    unmarshal_container( p, std::inserter( session_deploys, session_deploys.end() ) );
    unmarshal_container( p, std::inserter( session_shards, session_shards.end() ) );
    unmarshal_container( p, std::inserter( channel_deploy_shards, channel_deploy_shards.end() ) );
    unmarshal_container( p, std::inserter( channel_sid_shards, channel_sid_shards.end() ) );
    p >> channel_default_shard;
  }
};
struct PSmdispatcherPong : public sox::Marshallable {
  enum {uri = PSmdispatcherPongURI };
  uint32_t targetServerId;
  uint32_t serverId;
  uint32_t groupId;
  uint32_t startTime;

  std::map<uint16_t, uint16_t>      channel_deploy_shards;
  std::map<uint16_t, std::pair<uint32_t, uint32_t> >     channel_sid_shards;
  uint16_t                          channel_default_shard;

  std::map<uint16_t, std::pair<uint32_t, uint32_t> >     channel_deploys;

  std::vector<uint32_t>             query_order;
  uint32_t                          query_count;
  std::vector<uint32_t>             allocate_order;

  PSmdispatcherPong()
    : targetServerId(0)
    , serverId(0)
    , groupId(0)
    , startTime(0)
    , channel_default_shard(0)
    , query_count(0)
  {}
  virtual void marshal(sox::Pack &p) const {
    p << targetServerId << serverId << groupId << startTime;
    marshal_container( p, channel_deploy_shards );
    marshal_container( p, channel_sid_shards );
    p << channel_default_shard;
    marshal_container( p, channel_deploys );
    marshal_container( p, query_order );
    p << query_count;
    marshal_container( p, allocate_order );
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> targetServerId >> serverId >> groupId >> startTime;
    unmarshal_container( p, std::inserter( channel_deploy_shards, channel_deploy_shards.end() ) );
    unmarshal_container( p, std::inserter( channel_sid_shards, channel_sid_shards.end() ) );
    p >> channel_default_shard;
    unmarshal_container( p, std::inserter( channel_deploys, channel_deploys.end() ) );
    unmarshal_container( p, std::inserter( query_order, query_order.end() ) );
    p >> query_count;
    unmarshal_container( p, std::inserter( allocate_order, allocate_order.end() ) );
  }
};
struct PCheckSid2HS:public PCheckSid2{//仅为BEGIN_FORM_MAP兼容
  enum {uri = (260 << 8 | protocol::SDB_SVID)};
};
struct PCheckSidRes2HS:public PCheckSidRes2{//仅为BEGIN_FORM_MAP兼容
  enum {uri = (261 << 8 | protocol::SDB_SVID)};
};
struct PCheckSidRestoredHS:public PCheckSidRestored{//仅为BEGIN_FORM_MAP兼容
  enum {uri = (262 << 8 | protocol::SDB_SVID)};
};

//~hide session
//////////////////////////////////////////////////////////////////////////
struct PSetControlPort : public sox::Marshallable
{
  enum {uri = (255 << 8 | protocol::SESSION_SVID)};
  uint32_t serverId;
  uint32_t port;
  virtual void marshal(sox::Pack &pk) const {
    pk << serverId << port;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> serverId >> port;
  }
};

struct PupdateChannelPasswd : public  sox::Marshallable
{
    enum {uri = (256 << 8 | protocol::SESSION_SVID)};
    uint32_t sid;
    virtual void marshal(sox::Pack &pk) const {
        pk << sid;
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >>sid;
    }
};

struct PmodifySoundQuality: public sox::Marshallable
{
    enum {uri = (257 << 8) | protocol::SESSION_SVID};
    uint32_t sid ;
    virtual void marshal(sox::Pack &p) const {
        p << sid;
    }

    virtual void unmarshal(const sox::Unpack &p) {
        p >> sid;
    }

};

struct SInfoItem : public sox::Marshallable {
  uint32_t sid;
  uint32_t users;
  std::string name;
  uint32_t owner;
  uint32_t logoIndex;
  std::string logoUrl;
  uint32_t ipCount;
  uint32_t pcCount;
  uint32_t quality;
  bool bLimit;
  bool bPublic;

  SInfoItem()
    : sid(0)
    , users(0)
    , name()
    , owner(0)
    , logoIndex(0)
    , logoUrl()
    , ipCount(0)
    , pcCount(0)
    , quality(0)
    , bLimit(false)
    , bPublic(true)
  {}
  virtual void marshal(sox::Pack &p) const {
    p << sid << users << name << owner << logoIndex << logoUrl << ipCount << pcCount << quality;
    p.push_uint8(bLimit);
    p.push_uint8(bPublic);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> sid >> users >> name >> owner >> logoIndex >> logoUrl >> ipCount >> pcCount >> quality;
    bLimit = p.pop_uint8() != 0;
    bPublic = p.pop_uint8() != 0;
  }
};

struct ChannelInfoTrimItem : public sox::Marshallable {
  uint32_t sid;
  uint32_t users;

  ChannelInfoTrimItem()
    : sid(0)
    , users(0)
  {}
  ChannelInfoTrimItem(uint32_t channel, uint32_t totalUsers)
    : sid(channel)
    , users(totalUsers)
  {}
  virtual void marshal(sox::Pack &p) const {
    p << sid << users;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> sid >> users;
  }  
};

struct PSessionSyncInfo5: public sox::Marshallable
{
  enum{uri = PSessionSyncInfo5URI};
  uint32_t serverId;
  std::string sessionResPack;
  uint32_t timeStamp;
  uint32_t sessionGroupId;
  uint32_t oldMasterId;
  uint32_t newMasterId;
  std::vector<SInfoItem> infos;

  PSessionSyncInfo5()
    : serverId(0)
    , sessionResPack()
    , timeStamp(0)
    , sessionGroupId(0)
    , oldMasterId(0)
    , newMasterId(0)
    , infos()
  {}
  virtual void marshal(sox::Pack& p) const
  {
    p << serverId << sessionResPack << timeStamp << sessionGroupId << oldMasterId << newMasterId;
    sox::marshal_container(p, infos);
  }
  virtual void unmarshal(const sox::Unpack& p)
  {
    p >> serverId >> sessionResPack >> timeStamp >> sessionGroupId >> oldMasterId >> newMasterId;
    sox::unmarshal_container(p, std::back_inserter(infos));
  }
};

struct PSessionSyncInfo6: public PSessionSyncInfo5
{
  enum{uri = PSessionSyncInfo6URI};
};

struct PSessionSyncInfo7: public sox::Marshallable
{
  enum{uri = PSessionSyncInfo7URI};
  uint32_t sessionGroupId;
  uint32_t serverId; //该serverid为前端管理的serverid
  std::string sessionResPack;
  uint32_t timeStamp;
  uint32_t totalUsers;
  uint32_t session;
  uint32_t reserved2;
  std::vector<ChannelInfoTrimItem> infos;

  PSessionSyncInfo7()
    : sessionGroupId(0)
    , serverId(0)
    , sessionResPack()
    , timeStamp(0)
    , totalUsers(0)
    , session(0)
    , reserved2(0)
    , infos()
  {}
  virtual void marshal(sox::Pack& p) const
  {
    p << sessionGroupId << serverId << sessionResPack << timeStamp << totalUsers << session << reserved2;
    sox::marshal_container(p, infos);
  }
  virtual void unmarshal(const sox::Unpack& p)
  {
    p >> sessionGroupId >> serverId >> sessionResPack >> timeStamp >> totalUsers >> session >> reserved2;
    sox::unmarshal_container(p, std::back_inserter(infos));
  }
};

struct PSessionUpdateInfo: public sox::Marshallable
{
  enum{uri = PSessionUpdateInfoURI};
  uint32_t sessionGroupId;
  uint32_t serverId; //该serverid为前端管理的serverid
  uint32_t timeStamp;
  uint32_t totalUsers;  //仍然为 session总人数
  uint32_t  session;
  uint32_t  reserved2;
  std::vector<ChannelInfoTrimItem> infos;
  std::vector<uint32_t> removedSids;

  PSessionUpdateInfo()
    : sessionGroupId(0)
    , serverId(0)
    , timeStamp(0)
    , totalUsers(0)
    , session(0)
    , reserved2(0)
  {}

  virtual void marshal(sox::Pack& p) const
  {
    p << sessionGroupId << serverId << timeStamp << totalUsers << session << reserved2;
    sox::marshal_container(p, infos);
    sox::marshal_container(p, removedSids);
  }
  virtual void unmarshal(const sox::Unpack& up)
  {
    up >> sessionGroupId >> serverId >> timeStamp >> totalUsers >> session >> reserved2;
    sox::unmarshal_container(up, std::back_inserter(infos));
    if (!up.empty())
    {
      sox::unmarshal_container(up, std::back_inserter(removedSids));
    }
  }
};

struct PSessionUpdateInfo1: public PSessionUpdateInfo
{
  enum{uri = PSessionUpdateInfo1URI};
};

struct PProxyAdminStopSyncSgs: public sox::Marshallable{
  enum {uri = (PProxyAdminStopSyncSgsURI)};
  uint32_t serverId; //该serverid为前端管理的serverid
  uint32_t  reserved1;
  uint32_t  reserved2;
  std::vector<uint32_t> sessionGroups;

  PProxyAdminStopSyncSgs()
    : serverId(0)
    , reserved1(0)
    , reserved2(0)
    , sessionGroups(0)
  {}
  
  virtual void marshal(sox::Pack& p) const
  {
    p << serverId << reserved1 << reserved2;
    sox::marshal_container(p, sessionGroups);
  }
  virtual void unmarshal(const sox::Unpack& p)
  {
    p >> serverId >> reserved1 >> reserved2;
    sox::unmarshal_container(p, std::back_inserter(sessionGroups));
  }

  
};

struct PProxySyncInfo3 : public protocol::session::PProxySyncInfo2 {
  enum {uri = PProxySyncInfo3URI};
};

struct PProxySyncInfo4 : public sox::Marshallable{
  enum {uri = PProxySyncInfo4URI};
  uint32_t sessionGroupId;
  uint32_t serverId; //该serverid为前端管理的serverid
  std::string sessionResPack;
  uint32_t timeStamp;
  uint32_t  reserved1;
  uint32_t  reserved2;

  PProxySyncInfo4()
    : sessionGroupId(0)
    , serverId(0)
    , sessionResPack()
    , timeStamp(0)
    , reserved1(0)
    , reserved2(0)
  {}
  virtual void marshal(sox::Pack& p) const
  {
    p << sessionGroupId << serverId << sessionResPack << timeStamp << reserved1 << reserved2;
  }
  virtual void unmarshal(const sox::Unpack& p)
  {
    p >> sessionGroupId >> serverId >> sessionResPack >> timeStamp >> reserved1 >> reserved2;
  }
};

struct PSessionStartSync6: public sox::Marshallable
{
  enum{uri = PSessionStartSync6URI};
  uint32_t serverId;
  std::string sessionResPack;
  uint32_t timeStamp;
  uint32_t exclude;
  uint32_t sessionGroupId;
  std::vector<SInfoItem> infos;

  PSessionStartSync6()
    : serverId(0)
    , sessionResPack()
    , timeStamp(0)
    , exclude(0)
    , sessionGroupId(0)
    , infos()
  {}

  virtual void marshal(sox::Pack& p) const
  {
    p << serverId << sessionResPack << timeStamp << exclude << sessionGroupId;
    sox::marshal_container(p, infos);
  }
  virtual void unmarshal(const sox::Unpack& p)
  {
    p >> serverId >> sessionResPack >> timeStamp >> exclude >> sessionGroupId;
    sox::unmarshal_container(p, std::back_inserter(infos));
  }
};


struct PSessionStartSync7: public sox::Marshallable
{
  enum{uri = PSessionStartSync7URI};
  uint32_t serverId;
  uint32_t sessionGroupId;
  uint32_t controlPort;
  uint32_t exclude;
  uint32_t masterSession;
  uint32_t timeStamp; //message send timestamp
  std::string sessionResPack;
  uint32_t totalUsers;
  uint32_t  reserved1;
  uint32_t  reserved2;
  std::vector<ChannelInfoTrimItem> infos;

  
  PSessionStartSync7()
    : serverId(0)
    , sessionGroupId(0)
    , controlPort(0)
    , exclude(0)
    , masterSession(0)
    , timeStamp(0)
    , sessionResPack()
    , totalUsers(0)
    , reserved1(0)
    , reserved2(0)
    , infos()
  {}

  virtual void marshal(sox::Pack& p) const
  {
    p << serverId << sessionGroupId << controlPort << exclude << masterSession << timeStamp << sessionResPack << totalUsers << reserved1 << reserved2;
    sox::marshal_container(p, infos);
  }
  virtual void unmarshal(const sox::Unpack& p)
  {
    p >> serverId >> sessionGroupId >> controlPort >> exclude >> masterSession >> timeStamp >> sessionResPack >> totalUsers >> reserved1 >> reserved2;
    sox::unmarshal_container(p, std::back_inserter(infos));
  }

};

struct PCheckSidUsers: public sox::Marshallable{//??BEGIN_FORM_MAP??
  enum {uri = PCheckSidUsersURI };
  uint32_t sid;
  uint32_t sgid; 
  uint32_t reserved;
  uint32_t taskId;
  PCheckSidUsers()
  : sid(0)
  , sgid(0)
  , reserved(0)
  , taskId(0)
  {}
  
  virtual void marshal(sox::Pack &pk) const {
    pk << sid << sgid << reserved << taskId;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> sid >> sgid >> reserved >> taskId;
  }
};

struct PCheckSidUsersRes:public sox::Marshallable{
  enum {uri = PCheckSidUsersResURI };
  uint32_t sid;
  uint32_t sgid; 
  uint32_t realSgid;
  uint32_t realUsers;
  uint32_t taskId;  
  uint16_t resCode;
  uint32_t reserved1;
  uint32_t reserved2;
  bool isMaster;
  PCheckSidUsersRes()
  : sid(0) 
  , sgid(0)
  , realSgid(0)
  , realUsers(0)
  , taskId(0)
  , resCode(0)
  , reserved1(0)
  , reserved2(0)
  , isMaster(true)
  {}
  virtual void marshal(sox::Pack &pk) const {
    pk << sid << sgid << realSgid << realUsers << taskId << resCode << reserved1 << reserved2;
    pk.push_uint32(isMaster);
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> sid >> sgid >> realSgid >> realUsers >> taskId >> resCode >> reserved1 >> reserved2;
    isMaster = up.pop_uint32() != 0;
    
  }
};

struct PSessionStartSync3 : public protocol::session::PSessionSyncInfo3{
  enum {uri = PSessionStartSync3URI };
};

struct PSS_GetSessionInfoBase : public sox::Marshallable
{
  uint32_t m_uChannelID;
  uint32_t m_uUID;
  uint32_t m_uSvrID;
  uint32_t m_uAppURI;
  std::string m_strAppCtx;

  PSS_GetSessionInfoBase ()
  {
    m_uChannelID = 0;
    m_uUID = 0;
    m_uSvrID = 0;
    m_uAppURI = 0;
  }

  virtual void marshal(sox::Pack &p) const
  {
    p << m_uChannelID << m_uUID << m_uSvrID << m_uAppURI << m_strAppCtx;
  }

  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> m_uChannelID >> m_uUID >> m_uSvrID >> m_uAppURI >> m_strAppCtx;
  }
};

struct PSessionStartSync4 : public PSessionStartSync3{
  enum {uri = PSessionStartSync4URI };
  uint32_t exclude;

  virtual void marshal(sox::Pack &p) const {
    PSessionStartSync3::marshal(p);
    p << exclude;
  }

  virtual void unmarshal(const sox::Unpack &p) {
    PSessionStartSync3::unmarshal(p);
    p >> exclude;
  }
};
struct PSessionStartSync5 : public PSessionStartSync4{
  enum {uri = PSessionStartSync5URI };
  uint32_t sessionGroupId;

  virtual void marshal(sox::Pack &p) const {
    PSessionStartSync4::marshal(p);
    p << sessionGroupId;
  }

  virtual void unmarshal(const sox::Unpack &p) {
    PSessionStartSync4::unmarshal(p);
    p >> sessionGroupId;
  }
};

struct PSessionStatUpdate : public sox::Marshallable{
  enum {uri = PSessionStatUpdateURI };
  uint32_t serverId;
  uint32_t exclude;

  virtual void marshal(sox::Pack &p) const {
    p << serverId << exclude;
  }

  virtual void unmarshal(const sox::Unpack &p) {
    p >> serverId >> exclude;
  }
};

struct PSessionGroupStatUpdate : public sox::Marshallable{
  enum {uri = PSessionGroupStatUpdateURI };
  uint32_t sessionGroupId;
  uint32_t serverId;
  uint32_t exclude;  //session group's allocatable flag.
  uint32_t  reserved1;
  uint32_t  reserved2;

  PSessionGroupStatUpdate()
    : sessionGroupId(0)
    , serverId(0)
    , exclude(0)
    , reserved1(0)
    , reserved2(0)
  {}

  virtual void marshal(sox::Pack &p) const {
    p << sessionGroupId << serverId << exclude << reserved1 << reserved2;
  }

  virtual void unmarshal(const sox::Unpack &p) {
    p >> sessionGroupId >> serverId >> exclude >> reserved1 >> reserved2;
  }
};

struct PUpdateChannelOnSession : public sox::Marshallable{
  enum {uri = PUpdateChannelOnSessionURI };
  uint32_t serverId;
  uint32_t to;
  PUpdateChannelOnSession() : serverId(0), to(0) {}
  virtual void marshal(sox::Pack &p) const {
    p << serverId << to;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> serverId >> to;
  }
};

struct PUpdateChannelOnSession1 : public sox::Marshallable{
  enum {uri = PUpdateChannelOnSession1URI };
  uint32_t sessionGroupId;
  uint32_t serverId; //消息来源
  uint32_t newmaster;
  uint32_t oldmaster;
  uint32_t timestamp;
  uint32_t  reserved1;
  uint32_t  reserved2;

  PUpdateChannelOnSession1() 
    : sessionGroupId(0)
    , serverId(0)
    , newmaster(0)
    , oldmaster(0)
    , timestamp(0)
    , reserved1(0)
    , reserved2(0)
    {}
  virtual void marshal(sox::Pack &p) const {
    p << sessionGroupId << serverId << newmaster << oldmaster << timestamp << reserved1 << reserved2;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> sessionGroupId >> serverId >> newmaster >> oldmaster >> timestamp >> reserved1 >> reserved2;
  }
};

struct PGetSessUserNum : public sox::Marshallable{
  enum {uri = PGetSessUserNumURI };
  uint32_t sid;
  virtual void marshal(sox::Pack &p) const {
    p << sid;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> sid;
  }
};

struct PGetSessUserNumRes : public sox::Marshallable{ // UNUSED
  enum {uri = PGetSessUserNumResURI };
  uint32_t sid;
  uint32_t serverId;
  uint32_t userNum;
  virtual void marshal(sox::Pack &p) const {
    p << sid << serverId << userNum;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> sid >> serverId >> userNum;
  }
};

struct PBatchChannelRemoved : public sox::Marshallable{
  enum {uri = PBatchChannelRemovedURI };
  std::vector<std::pair<uint32_t, uint32_t> > sidAndserverId;
  virtual void marshal(sox::Pack &p) const {
    sox::marshal_container(p, sidAndserverId);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    sox::unmarshal_container(p, std::back_inserter(sidAndserverId));
  }
};

struct PRemoveChannel : public sox::Marshallable{
  enum {uri = PRemoveChannelURI };
  enum RemoveType
  {
    NORMAL = 0,
    LOCKED =1,
    FROZEN = 2,
    DISMISS = 3,
    RECYCLE = 4,
  };
  uint32_t sid;
  RemoveType type;
  uint32_t timestmap; // unlocked time or unforzen time (when type=LOCKED|FROZEN)

  PRemoveChannel()
    : sid(0)
    , type(NORMAL)
    , timestmap(0)
  {}

  virtual void marshal(sox::Pack &p) const {
    p << sid;
    p.push_uint32(type);
    p << timestmap;
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> sid;
    if (!p.empty())
    {
      type = (RemoveType)p.pop_uint32();
      p >> timestmap;
    }
  }
};

struct PSessionLbs3 : public protocol::session::PSessionLbs {
  enum {uri = PSessionLbs3URI};
};

struct PSessionLbsRes5 : public sox::Marshallable {
  enum { uri = PSessionLbsRes5URI };
  enum { old_unknown =0, localLookup = 1, localAllocate =2, globalLookup =3, globalAllocate=4, local2GlobalLookup=5, duplicateRequest=6};
  enum { TCP_TYPE = 1, UDP_TYPE = 2};
  uint32_t  code;
  uint32_t  sid;
  uint32_t  asid;
  uint32_t  session;
  uint32_t  sessiongroup;
  uint32_t  lookupstatus;
  uint32_t  linkType;

  std::map<std::string, std::string>  response;
  PSessionLbsRes5()
    : code(0)
    , sid(0)
    , asid(0)
    , session(0)
    , sessiongroup(0)
    , lookupstatus(0)
    , linkType(1)
  {}
  virtual void marshal(sox::Pack &p) const {
    p << code << sid << asid << session << sessiongroup << lookupstatus << linkType;
    marshal_container( p, response );
  }

  virtual void unmarshal(const sox::Unpack &p) {
    p >> code >> sid >> asid >> session >> sessiongroup >> lookupstatus >> linkType;
    unmarshal_container( p, std::inserter(response, response.end()) );
  }
};

typedef ZipMarshal< PSessionLbsRes5, 360 << 8 | protocol::SMANAGER_SVID > PSessionLbsZipRes;

struct PSessionLbsBypassing : public sox::Marshallable
{
  enum {uri = PSessionLbsBypassingURI };
  uint32_t sid; 
  uint32_t protoVer;
  virtual void marshal(sox::Pack &p) const 
  {
    p << sid << protoVer;
  }
  virtual void unmarshal(const sox::Unpack &up) 
  {
    up >> sid >> protoVer;
  }
};

typedef ZipMarshal< PSessionLbsRes5, PSessionLbsBypassingZipResURI > PSessionLbsBypassingZipRes;

struct PLocateSid : public sox::Marshallable
{
  enum { uri = PLocateSidURI };
  uint32_t sid;
  std::string context;

  virtual void marshal(sox::Pack &p) const
  {
    p << sid << context;
  }

  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> sid >> context;
  }
};

struct PLocateSidRes : public sox::Marshallable
{
  enum { uri = PLocateSidResURI };
  PLocateSidRes():sid(0),session(0),sessiongroup(0) {}
  uint32_t sid;
  uint32_t session;
  uint32_t sessiongroup;
  uint32_t code;
  std::string context;
  std::map<uint32_t, std::string> props;

  virtual void marshal(sox::Pack &p) const
  {
    p << sid << session << sessiongroup << code << context;
    sox::marshal_container(p, props);
  }

  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> sid >> session >> sessiongroup >> code >> context;
    sox::unmarshal_container(up, std::inserter(props, props.begin()));
  }
};

//dupsesion_d
//副本清理请求, x->dupsession
struct PDupSessReq : public sox::Marshallable{
  enum {uri = (1 << 8 | protocol::DUPSESSION_SVID)};
  uint32_t sid;
  std::set<uint32_t> seSrvIds;

  virtual void marshal(sox::Pack &p) const {
    p << sid;
    sox::marshal_container(p, seSrvIds);
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> sid;
    sox::unmarshal_container(p, std::inserter(seSrvIds, seSrvIds.begin()));
  }
};
//检查超时请求
struct PDupSessTimeoutChk : public sox::Voidmable{
  enum {uri = (2 << 8 | protocol::DUPSESSION_SVID)};
};


struct PSS_GetSessionChannelRoler : public PSS_GetSessionInfoBase
{
  enum {uri = (204 << 8 | protocol::SDB_SVID)};
};

//需要将请求包原包返回，因此继承请求报文
struct PSS_GetSessionChannelRolerRes : public PSS_GetSessionChannelRoler
{
  enum {uri = (205 << 8 | protocol::SDB_SVID)};

  uint32_t uChannelRoler;

  PSS_GetSessionChannelRolerRes()
  {
    uChannelRoler = 0;
  }

  PSS_GetSessionChannelRolerRes(PSS_GetSessionChannelRoler& objOri):
    PSS_GetSessionChannelRoler(objOri),
    uChannelRoler(0)
  {
  }

  virtual void marshal(sox::Pack &p) const
  {
    PSS_GetSessionChannelRoler::marshal(p);
    p << uChannelRoler;
  }

  virtual void unmarshal(const sox::Unpack &p)
  {
    PSS_GetSessionChannelRoler::unmarshal(p);
    p >> uChannelRoler;
  }
};


struct PSS_GetChannelMembersByRole : public PSS_GetSessionInfoBase
{
  enum {uri = (210 << 8 | protocol::SDB_SVID)};
  
  //指定的马甲列表
  std::set<uint32_t> m_setRole;
    
  virtual void marshal(sox::Pack &p) const
  {
    PSS_GetSessionInfoBase::marshal(p);
    sox::marshal_container(p, m_setRole);
  }

  virtual void unmarshal(const sox::Unpack &p)
  {
    PSS_GetSessionInfoBase::unmarshal(p);
    sox::unmarshal_container(p, std::inserter(m_setRole,
      m_setRole.begin()));
  }
};

//需要将请求包原包返回，因此继承请求报文
struct PSS_GetChannelMembersByRoleRes : public PSS_GetChannelMembersByRole
{
  enum {uri = (211 << 8 | protocol::SDB_SVID)};

  //key: 马甲, value: 马甲对应uid
  //如果请求者(m_uUID)不是频道的OW, 则这个map置为空
  typedef std::map<uint32_t, std::set<uint32_t> > ChUserMapType;
  
  ChUserMapType m_mapUser;
  
  PSS_GetChannelMembersByRoleRes()
  { 
  }

  PSS_GetChannelMembersByRoleRes(PSS_GetChannelMembersByRole& objOri):
  PSS_GetChannelMembersByRole(objOri)
  {
  }

  virtual void marshal(sox::Pack &p) const
  {
    PSS_GetChannelMembersByRole::marshal(p);
    sox::marshal_container(p, m_mapUser);
  }

  virtual void unmarshal(const sox::Unpack &p)
  {
    PSS_GetChannelMembersByRole::unmarshal(p);
    sox::unmarshal_container(p, std::inserter(m_mapUser,
      m_mapUser.begin()));
  }
  
};


        enum EPrivilegeType{CHANNEL_TOP=0, CHANNEL_RED_NAME=1, VIP_FACE=2, LIANG_NUMBER=3, PURPLE_NAME=4};
        struct UserVipInfo : public sox::Marshallable 
        {    
			UserVipInfo():vipLevel(0),vipJifen(0),reserved(0),sort_value_(0),sort_tips_type_(0) {}
    uint8_t vipLevel;
    uint32_t vipJifen;
    uint8_t  reserved;
	uint32_t sort_value_;
	uint32_t sort_tips_type_;
                std::map<uint16_t, std::string> vtPrivilege;  //EPrivilegeType ==> parameter

                virtual void marshal(sox::Pack &p) const
                {    
      p << vipLevel << vipJifen << reserved;
                        marshal_container( p, vtPrivilege );
						p << sort_value_ << sort_tips_type_ ;
                }    

                virtual void unmarshal(const sox::Unpack &p)  
                {    
      p >> vipLevel >> vipJifen >> reserved;
                        unmarshal_container( p, std::inserter(vtPrivilege, vtPrivilege.end()));
						p >> sort_value_ >> sort_tips_type_ ;
                }    
        };   

        struct PGetUserVipInfo : sox::Marshallable {
                enum {uri = (222 << 8 | protocol::SDB_SVID)};
                uint32_t sid; 
                uint32_t uid; 
                std::string m_context;
                virtual void marshal(sox::Pack & p) const {
                        p << sid << uid << m_context;
                }    

                virtual void unmarshal(const sox::Unpack &p) {
                        p >> sid >> uid >> m_context;
                }    
        };   

        struct PGetUserVipInfoRes : sox::Marshallable {
                enum {uri = (223 << 8 | protocol::SDB_SVID)};
				PGetUserVipInfoRes():sid(0),uid(0),m_context(""),isVip(0),rescode(0),ext_info("") {}
                uint32_t sid; 
                uint32_t uid; 
                std::string m_context;
                uint32_t isVip; 
    uint32_t rescode;
                std::string ext_info; 
                virtual void marshal(sox::Pack & p) const {
                        p << sid << uid << m_context << isVip << rescode;
                        p.push_varstr32(ext_info.data(), ext_info.length());
                }    

                virtual void unmarshal(const sox::Unpack &p) {
                        p >> sid >>uid >> m_context >> isVip >> rescode;
                        ext_info = p.pop_varstr32();
                }    
        };
//======================================================================================
// V4 messages
struct PNegotiateConfig: public sox::Marshallable
{
  enum{uri = PNegotiateConfigURI};

  uint64_t proxyId;
  uint32_t reserver1;
  uint32_t reserver2;

  virtual void marshal(sox::Pack &pk) const
  {
    pk << proxyId << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> proxyId >> reserver1 >> reserver2;
  }
};

struct PNegotiateConfigRes: public sox::Marshallable
{
  enum{uri = PNegotiateConfigResURI};
    
  uint64_t smId;
  std::vector<std::pair<uint32_t, uint32_t> > hashRanges;
  uint32_t reserver1;
  uint32_t reserver2;

  virtual void marshal(sox::Pack &pk) const
  {
    sox::marshal_container(pk, hashRanges);
    pk << smId << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    sox::unmarshal_container(up, std::back_inserter(hashRanges));
    up >> smId >> reserver1 >> reserver2;
  }
};

struct PProxyRegister: public sox::Marshallable
{
  enum{uri = PProxyRegisterURI};

  uint32_t ip;
  uint16_t tcp[4];
  uint16_t udp[4];
  uint16_t tcpPort2Prx;
  uint16_t udpPort2Prx;
  uint64_t proxyId;
  uint16_t isp;
  uint16_t sinfoVer; // channels info version
  uint16_t ulistVer; // user list version
  uint32_t proxyType;
  uint32_t groupId;  // jifang id
  uint32_t relayPort;
  uint32_t sgId;     // service group id
  uint32_t sessionId;
  uint32_t reserver1;
  uint32_t reserver2;

  virtual void marshal(sox::Pack &p) const {
    p << ip;
    for(int i=0;i<4;i++)p<<tcp[i];
    for(int i=0;i<4;i++)p<<udp[i];
    p << tcpPort2Prx << udpPort2Prx << proxyId << isp << sinfoVer << ulistVer
      << relayPort << sgId << sessionId << proxyType << groupId << reserver1
      << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> ip;
    for(int i=0;i<4;i++)up>>tcp[i];
    for(int i=0;i<4;i++)up>>udp[i];
    up >> tcpPort2Prx >> udpPort2Prx >> proxyId >> isp >> sinfoVer >> ulistVer 
       >>relayPort >> sgId >> sessionId >> proxyType >> groupId >> reserver1
       >> reserver2;
  }
};

struct PProxyUnregister: public sox::Marshallable
{
  enum{uri = PProxyUnregisterURI};

  uint32_t sgId;     // service group id
  uint64_t proxyId;
  uint32_t reserver1;
  uint32_t reserver2;

  PProxyUnregister() 
    : sgId(0)
    , proxyId(0)
    , reserver1(0)
    , reserver2(0)
  { }

  void marshal(sox::Pack & pk) const{
    pk << sgId << proxyId << reserver1 << reserver2;
  }
  void unmarshal(const sox::Unpack &up){
    up >> sgId >> proxyId >> reserver1 >> reserver2;
  }
};

struct ProxyInformation: public sox::Marshallable
{
  uint64_t proxyId;
  uint32_t ip;
  std::vector<uint16_t> tcp;
  std::vector<uint16_t> udp;
  uint16_t tcpPort2Prx;
  uint16_t udpPort2Prx;
  uint16_t relayPort;
  uint16_t isp;
  uint16_t type;
  uint32_t areaType;
  uint32_t reserver2;

  void marshal(sox::Pack & pk) const{
    pk << proxyId << ip << tcpPort2Prx << udpPort2Prx 
       << relayPort << isp << type << areaType << reserver2;
    sox::marshal_container(pk, tcp);
    sox::marshal_container(pk, udp);
  }
  void unmarshal(const sox::Unpack &up){
    up >> proxyId >> ip >> tcpPort2Prx >> udpPort2Prx 
       >> relayPort >> isp >> type >> areaType >> reserver2;
    sox::unmarshal_container(up, std::back_inserter(tcp));
    sox::unmarshal_container(up, std::back_inserter(udp));
  }
};

struct PProxyRegisterRes: public sox::Marshallable
{
  enum{uri = PProxyRegisterResURI};

  std::vector<ProxyInformation> proxyInfos;
  uint16_t flags;
  uint32_t reserver1;
  uint32_t reserver2;

  PProxyRegisterRes()
    : flags(0)
    , reserver1(0)
    , reserver2(0)
  {}

  virtual void marshal(sox::Pack &pk) const {
    marshal_container(pk, proxyInfos);
    pk << flags << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    unmarshal_container(up, std::back_inserter(proxyInfos));
    up >> flags >> reserver1 >> reserver2;
  }
};

struct PProxyJoin: public sox::Marshallable
{
  enum{uri = PProxyJoinURI};

  std::vector<ProxyInformation> proxyInfos;
  uint32_t sessGroupId;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pk) const {
    marshal_container(pk, proxyInfos);
    pk << sessGroupId << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    unmarshal_container(up, std::back_inserter(proxyInfos));
    up >> sessGroupId >> reserver2;
  }
};

struct PProxyDrop: public sox::Marshallable
{
  enum{ uri = PProxyDropURI };
  uint64_t proxy_id;
  uint32_t sessGroupId;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pk) const {
    pk << proxy_id;
    pk << sessGroupId << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> proxy_id;
    up >> sessGroupId >> reserver2;
  }
};

struct PProxyDropBatch: public sox::Marshallable
{
  enum{ uri = PProxyDropBatchURI };

  PProxyDropBatch() : sessGroupId(0), reserver2(0) {}

  std::set<uint64_t> proxyList;
  uint32_t sessGroupId;
  uint32_t reserver2;

  virtual void marshal(sox::Pack &pk) const 
  {
    sox::marshal_container(pk, proxyList);
    pk << sessGroupId << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) 
  {
    sox::unmarshal_container(up, std::inserter(proxyList, proxyList.begin()));
    up >> sessGroupId >> reserver2;
  }
};

struct PProxyServiceSgInfos: public sox::Marshallable
{
  enum{ uri = PProxyServiceSgInfosURI };

  uint64_t proxyId;
  std::set<uint32_t> sgIds;     // service group id

  PProxyServiceSgInfos() 
    : proxyId(0) { }

  void marshal(sox::Pack & pk) const
  {
    pk << proxyId ;
    marshal_container(pk, sgIds);
  }

  void unmarshal(const sox::Unpack &up)
  {
    up >> proxyId;
    unmarshal_container(up, std::inserter(sgIds, sgIds.end()));
  }
};

struct PProxyInfoSync: public sox::Marshallable
{
  enum{ uri = PProxyInfoSyncURI };
  uint32_t userNum;
  std::vector<SInfoItem> infos;
  uint32_t version;
  uint32_t reserver1;
  uint32_t reserver2;

  virtual void marshal(sox::Pack &pk) const {
    marshal_container(pk, infos);
    pk << userNum << version <<  reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    unmarshal_container(up, std::back_inserter(infos));
    up >> userNum >> version >> reserver1 >> reserver2;
  }
};

struct PProxyInfoUpdate: public PProxyInfoSync
{
  enum{ uri = PProxyInfoUpdateURI };
};

struct PProxyChannelsRemove: public sox::Marshallable
{
  enum{ uri = PProxyChannelsRemoveURI };
  std::set<uint32_t> sidList;
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pk) const {
    marshal_container(pk, sidList);
    pk << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    unmarshal_container(up, std::inserter(sidList, sidList.begin()));
    up >> reserver1 >> reserver2;
  }
};

struct PProxyPing: public sox::Marshallable
{
  enum{ uri = PProxyPingURI };

  uint32_t sgId;      // session group id
  uint32_t userNum;   // total user number in proxy
  uint16_t sinfoVer;  // channel info version
  uint16_t ulistVer;  // user list version
  uint32_t timestamp;
  uint16_t status;    // proxy status
  uint32_t reserver1;
  uint32_t reserver2;

  PProxyPing()
    :sgId(0)
    ,userNum(0)
    ,sinfoVer(0)
    ,ulistVer(0)
    ,timestamp(0)
    ,status(0)
  {}
  virtual void marshal(sox::Pack &pk) const {
    pk << sgId << userNum << sinfoVer << ulistVer << timestamp 
       << status << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> sgId >> userNum >> sinfoVer >> ulistVer >> timestamp
       >> status >> reserver1 >> reserver2;
  }
};

struct PProxyPingRes: public sox::Marshallable
{
  enum{ uri = PProxyPingResURI };
  uint16_t flags;
  uint32_t timestamp;
  uint32_t mediaProxyNum;
  uint32_t reserver1;
  uint32_t reserver2;

  PProxyPingRes()
    :flags(0)
    ,timestamp(0)
    ,mediaProxyNum(0)
    ,reserver1(0)
    ,reserver2(0)
  {}
  virtual void marshal(sox::Pack &pk) const {
    pk << flags << timestamp << mediaProxyNum 
       << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> flags >> timestamp >> mediaProxyNum 
       >> reserver1 >> reserver2;
  }
};

struct PMasterSessionChange : public sox::Marshallable{
  enum {uri = PMasterSessionChangeURI };

  uint32_t oldSessionId; //old master session id
  uint32_t newSessionId; //new master session id
  uint32_t sessionGroup;
  uint32_t reserver1;
  uint32_t reserver2;
  PMasterSessionChange() 
    : oldSessionId(0)
    , newSessionId(0) 
    , sessionGroup(0)
    , reserver1(0)
    , reserver2(0)
  {}
  virtual void marshal(sox::Pack &p) const {
    p << oldSessionId << newSessionId << sessionGroup << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &p) {
    p >> oldSessionId >> newSessionId >> sessionGroup >> reserver1 >> reserver2;
  }
};

struct PProxySyncChannelUsers: public sox::Marshallable
{
  enum{ uri = PProxySyncChannelUsersURI };

  uint16_t mode; // 0: only sync updated channel; 1: sync all channels
  uint16_t version;
  std::map<uint32_t, uint32_t> chUserNum;
  uint32_t reserver1;
  uint32_t reserver2;

  PProxySyncChannelUsers() 
    : mode(0)
    , version(0)
    , reserver1(0)
    , reserver2(0)
  { 
    chUserNum.clear(); 
  }

  virtual void marshal(sox::Pack &pk) const {
    marshal_container(pk, chUserNum);
    pk << mode << version << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    unmarshal_container(up, std::inserter(chUserNum, chUserNum.end()));
    up >> mode >> version >> reserver1 >> reserver2;
  }
};

struct PMediaProxySyncChannelUsers: public PProxySyncChannelUsers
{
  enum{ uri = PMediaProxySyncChannelUsersURI };
  std::set<uint32_t> virtChannels;

  virtual void marshal(sox::Pack &pk) const {
    PProxySyncChannelUsers::marshal(pk);
    marshal_container(pk, virtChannels);
  }
  virtual void unmarshal(const sox::Unpack &up) {
    PProxySyncChannelUsers::unmarshal(up);
    unmarshal_container(up, std::inserter(virtChannels, virtChannels.end()));
  }
};

struct PGetMediaProxyInfoSM: public sox::Marshallable
{
  enum{uri = PGetMediaProxyInfoSMURI};

  uint32_t uid;
  uint32_t sid;
  uint32_t uip;

  PGetMediaProxyInfoSM() 
    : uid(0)
    , sid(0)
    , uip(0)
  { }

  void marshal(sox::Pack & pk) const{
    pk << uid << sid << uip;
  }
  void unmarshal(const sox::Unpack &up){
    up >> uid >> sid >> uip;
  }
};

struct PGetMediaProxyInfoBatch: public sox::Marshallable
{
  enum{ uri = PGetMediaProxyInfoBatchURI };

  uint32_t context;
  uint32_t sid;
  std::set<uint32_t> uids;

  PGetMediaProxyInfoBatch() : sid(0) { }

  void marshal(sox::Pack & pk) const
  {
    pk << context << sid;
    marshal_container(pk, uids);
  }
  void unmarshal(const sox::Unpack &up)
  {
    up >> context >> sid;
    unmarshal_container(up, std::inserter(uids, uids.begin()));
  }
};

enum LoginRetryProc
{
  E_LOGIN_SUCCESS = 0, // 登录成功，不需要重连
  E_DONT_RETRY = 1, // 保留
  E_RETRY_NORMAL = 2, // 按现有机制重连
  E_RETRY_THIS_ONLY = 3, // 指定只重连本前端，不重新进频道
  E_RETRY_OTHER_IMMD = 4, // 指定重连其他前端，不重新进频道
  E_RETRY_PRESERVE
};

struct POnGetMediaProxyInfoSM: public sox::Marshallable
{
  enum{uri = POnGetMediaProxyInfoSMURI};

  std::vector<protocol::session::IpInfo> mediaProxyInfo;
  uint32_t uid;
  uint32_t sid;
  uint32_t mediaProxyCnt;
  uint32_t reserver1;
  LoginRetryProc retryType; // 重连操作控制
  uint16_t retryFreq; // 重连频率控制，单位s

  POnGetMediaProxyInfoSM()
    : mediaProxyCnt(0)
    , reserver1(0)
    , retryType(E_LOGIN_SUCCESS)
    , retryFreq(0)
  { }

  void marshal(sox::Pack & pk) const{
    marshal_container(pk, mediaProxyInfo);
    pk << uid << sid << mediaProxyCnt << reserver1;
    pk.push_uint8(retryType);
    pk << retryFreq;
  }
  void unmarshal(const sox::Unpack &up){
    unmarshal_container(up, std::back_inserter(mediaProxyInfo));
    up >> uid >> sid >> mediaProxyCnt >> reserver1;
    if (!up.empty())
    {
      retryType = (LoginRetryProc)up.pop_uint8();
      up >> retryFreq;
    }
  }
};

struct PGetMediaProxyInfoBatchRes: public sox::Marshallable
{
  enum{ uri = PGetMediaProxyInfoBatchResURI };

  uint32_t context;
  std::vector<protocol::session::IpInfo> mediaProxyInfo;
  std::set<uint32_t> uids;
  uint32_t sid;
  uint32_t mediaProxyCnt;
  uint32_t reserver1;
  LoginRetryProc retryType; // 重连操作控制
  uint16_t retryFreq; // 重连频率控制，单位s

  PGetMediaProxyInfoBatchRes()
    : mediaProxyCnt(0)
    , reserver1(0)
    , retryType(E_LOGIN_SUCCESS)
    , retryFreq(0)
  { }

  void marshal(sox::Pack & pk) const
  {
    pk << context;
    marshal_container(pk, mediaProxyInfo);
    marshal_container(pk, uids);
    pk << sid << mediaProxyCnt << reserver1;
    pk.push_uint8(retryType);
    pk << retryFreq;
  }
  void unmarshal(const sox::Unpack &up)
  {
    up >> context;
    unmarshal_container(up, std::back_inserter(mediaProxyInfo));
    sox::unmarshal_container(up, std::inserter(uids, uids.begin()));
    up >> sid >> mediaProxyCnt >> reserver1;
    retryType = (LoginRetryProc)up.pop_uint8();
    up >> retryFreq;
  }
};

struct PGetSignalProxyInfoSM: public protocol::session::PGetSignalProxyInfo
{
  enum{uri = PGetSignalProxyInfoSMURI};

  uint32_t sid;
  uint32_t reserver1;

  PGetSignalProxyInfoSM(): sid(0),reserver1(0) { }

  void marshal(sox::Pack & pk) const{
    PGetSignalProxyInfo::marshal(pk);
    pk << sid << reserver1;
  }
  void unmarshal(const sox::Unpack &up){
    PGetSignalProxyInfo::unmarshal(up);
    up >> sid >> reserver1;
  }
};
struct POnGetSignalProxyInfoSM: public sox::Marshallable
{
  enum{uri = POnGetSignalProxyInfoSMURI};
  std::vector<protocol::session::IpInfo> signalProxyInfoOld;
  uint32_t uid;
  uint32_t sid;
  uint32_t reserver1;
  std::vector<protocol::session::IpInfo> signalProxyInfoNew; //兼容大频道前端管理

  POnGetSignalProxyInfoSM(): uid(0),sid(0),reserver1(0) { }

  void marshal(sox::Pack & pk) const{
    sox::marshal_container(pk, signalProxyInfoOld);
    pk << uid << sid << reserver1;
    sox::marshal_container(pk, signalProxyInfoNew);
  }
  void unmarshal(const sox::Unpack &up){
    sox::unmarshal_container(up, std::back_inserter(signalProxyInfoOld));
    up >> uid >> sid >> reserver1;
    if(!up.empty())
    {
      sox::unmarshal_container(up, std::back_inserter(signalProxyInfoNew));	
    }
  }
};

struct PSyncProxyInfos : public sox::Marshallable {
  enum {uri = (2119 << 8 | protocol::SESSION_SVID)};
  std::vector<uint64_t> proxyInfos;
  uint32_t reserver1;
  uint32_t reserver2;

  virtual void marshal(sox::Pack &pak) const {
    marshal_container(pak, proxyInfos);
    pak << reserver1 << reserver2; 
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    unmarshal_container(pak, std::back_inserter(proxyInfos));
    pak >> reserver1 >> reserver2;
  }
};

struct PProxyEndPointsChanged : public sox::Marshallable {
  enum{ uri= ( 2070 << 8 | protocol::SESSION_SVID ) };
  uint32_t ip;
  std::vector<uint16_t> tcpPorts;
  std::vector<uint16_t> udpPorts;
  uint64_t proxyId;
  uint32_t reserved1;
  uint32_t reserved2;

  virtual void marshal( sox::Pack & p ) const {
    p << ip;
    marshal_container( p, tcpPorts );
    marshal_container( p, udpPorts );
    p << proxyId << reserved1 << reserved2;
  }

  virtual void unmarshal( const sox::Unpack & up ) {
    up >> ip;
    unmarshal_container( up, std::back_inserter( tcpPorts ) );
    unmarshal_container( up, std::back_inserter( udpPorts ) );
    up >> proxyId >> reserved1 >> reserved2;
  }
};

struct PInitialProxyInfoSyncReq: public sox::Marshallable
{
  enum {uri = PInitialProxyInfoSyncReqURI};

  uint32_t serverId;

  virtual void marshal(sox::Pack& pk) const
  {
    pk << serverId;
  }
  virtual void unmarshal(const sox::Unpack& up)
  {
    up >> serverId;
  }  
};

struct PInitialProxyInfoSyncRes: public sox::Marshallable
{
  enum {uri = PInitialProxyInfoSyncResURI};

  uint32_t groupId;
  //<proxyId, proxyInfo3>
  std::map<uint64_t, protocol::session::proxyInfo3> proxyInfo;
  //<proxyId, <sid> >
  std::map<uint64_t, std::set<uint32_t> > proxyIdToSidMap;
  //<sid, user number in this group>
  std::map<uint32_t, uint32_t> sidToUserNumMap;

  virtual void marshal(sox::Pack& pk) const
  {
    pk << groupId;
    marshal_container(pk, proxyInfo);

    pk << static_cast<uint32_t>(proxyIdToSidMap.size());
    std::map<uint64_t, std::set<uint32_t> >::const_iterator addcit;
    for(addcit = proxyIdToSidMap.begin(); addcit != proxyIdToSidMap.end(); ++addcit)
    {
      pk << addcit->first;
      sox::marshal_container(pk, addcit->second);
    }

    marshal_container(pk, sidToUserNumMap);
  }
  virtual void unmarshal(const sox::Unpack& up)
  {
    up >> groupId;
    unmarshal_container(up, std::inserter(proxyInfo, proxyInfo.begin()));

    uint32_t mapSize;
    up >> mapSize;
    for(uint32_t loop = 0; loop < mapSize; ++loop)
    {
      uint64_t proxyId;
      up >> proxyId;
      std::set<uint32_t>& tmpSet = proxyIdToSidMap[proxyId];
      sox::unmarshal_container(up, std::inserter(tmpSet, tmpSet.begin()));
    }

    unmarshal_container(up, std::inserter(sidToUserNumMap, sidToUserNumMap.begin()));
  }  
};

struct PPeriodicProxyInfoSync: public sox::Marshallable
{
  enum {uri = PPeriodicProxyInfoSyncURI};

  uint32_t groupId;
  //<proxyId, proxyInfo3>
  std::map<uint64_t, protocol::session::proxyInfo3> proxyInfo;
  //<proxyId, <sid> >
  std::map<uint64_t, std::set<uint32_t> > addProxyIdToSidMap;
  std::map<uint64_t, std::set<uint32_t> > delProxyIdToSidMap;

  //<groupId>
  std::set<uint32_t> removedGroupIdSet;
  //<<groupId, proxyId> >
  std::set<std::pair<uint32_t, uint64_t> > removedProxyIdSet;

  virtual void marshal(sox::Pack& pk) const
  {
    pk << groupId;
    marshal_container(pk, proxyInfo);

    pk << static_cast<uint32_t>(addProxyIdToSidMap.size());
    std::map<uint64_t, std::set<uint32_t> >::const_iterator addcit;
    for(addcit = addProxyIdToSidMap.begin(); addcit != addProxyIdToSidMap.end(); ++addcit)
    {
      pk << addcit->first;
      sox::marshal_container(pk, addcit->second);
    }

    pk << static_cast<uint32_t>(delProxyIdToSidMap.size());
    std::map<uint64_t, std::set<uint32_t> >::const_iterator delcit;
    for(delcit = delProxyIdToSidMap.begin(); delcit != delProxyIdToSidMap.end(); ++delcit)
    {
      pk << delcit->first;
      sox::marshal_container(pk, delcit->second);
    }

    marshal_container(pk, removedGroupIdSet);
    marshal_container(pk, removedProxyIdSet);
  }
  virtual void unmarshal(const sox::Unpack& up)
  {
    up >> groupId;
    unmarshal_container(up, std::inserter(proxyInfo, proxyInfo.begin()));

    uint32_t mapSize;
    up >> mapSize;
    for(uint32_t loop = 0; loop < mapSize; ++loop)
    {
      uint64_t proxyId;
      up >> proxyId;
      std::set<uint32_t>& tmpSet = addProxyIdToSidMap[proxyId];
      sox::unmarshal_container(up, std::inserter(tmpSet, tmpSet.begin()));
    }

    up >> mapSize;
    for(uint32_t loop = 0; loop < mapSize; ++loop)
    {
      uint64_t proxyId;
      up >> proxyId;
      std::set<uint32_t>& tmpSet = delProxyIdToSidMap[proxyId];
      sox::unmarshal_container(up, std::inserter(tmpSet, tmpSet.begin()));
    }

    unmarshal_container(up, std::inserter(removedGroupIdSet, removedGroupIdSet.begin()));
    unmarshal_container(up, std::inserter(removedProxyIdSet, removedProxyIdSet.begin()));
  }
};

struct PGroupIdConfig: public sox::Marshallable
{
  enum{uri = PGroupIdConfigURI};
  std::set<uint32_t> groupIdSet;
  virtual void marshal(sox::Pack &pk) const
  {
    marshal_container(pk, groupIdSet);
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    unmarshal_container(up, std::inserter(groupIdSet, groupIdSet.begin()));
  }
};

struct PGetBanChannel : public sox::Marshallable
{
    enum{ uri = PGetBanChannelURI };
    uint32_t serverId;
    virtual void marshal(sox::Pack &pk) const
    {
        pk << serverId;
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        up >> serverId;
    }
};

struct PBanChannelList : public sox::Marshallable
{
    enum{ uri = PBanChannelListURI };
    std::set<uint32_t> sids;
    virtual void marshal(sox::Pack &pk) const
    {
        marshal_container(pk, sids);
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        sox::unmarshal_container(up, std::inserter(sids, sids.begin()));
    }
};

struct PSyncBanChannel : public sox::Marshallable
{
    enum{ uri = PSyncBanChannelUri };
    enum BanType { FROZEN = 1, LOCKED = 2 };
    uint32_t sid;
    uint32_t unBanTime;
    BanType type;

    virtual void marshal(sox::Pack &pk) const
    {
        pk << sid << unBanTime;
        pk.push_uint32(type);
    }
    virtual void unmarshal(const sox::Unpack &up)
    {
        up >> sid >> unBanTime;
        type = (BanType)up.pop_uint32();
    }
};


// 视频前端消息
struct PVideoProxySyncUsers: public sox::Marshallable
{
  enum{ uri = (10057 << 8 | protocol::SESSION_SVID) };
  uint16_t mode; // 0: only sync updated channel; 1: sync all channels
  uint16_t version;
  uint32_t totalnum;
  uint32_t reserver1;
  // app - sid - users
  std::map<uint32_t, std::map<uint32_t, uint32_t> > chUserNum;

  PVideoProxySyncUsers() 
    : mode(0)
    , version(0)
    , totalnum(0)
    , reserver1(0)
  { 
    chUserNum.clear(); 
  }

  virtual void marshal(sox::Pack &pk) const {
    pk << mode << version << totalnum << reserver1;
    pk << chUserNum.size();
    for(std::map<uint32_t, std::map<uint32_t, uint32_t> >::const_iterator it = chUserNum.begin();
      it != chUserNum.end(); ++it)
    {
      pk << it->first;
      sox::marshal_container(pk, it->second);
    }
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> mode >> version >> totalnum >> reserver1;
    uint32_t size = 0;
    up >> size;
    for(uint32_t i=0;i<size;++i)
    {
      uint32_t appid=0;
      std::map<uint32_t, uint32_t> sid_num;
      up >> appid;
      sox::unmarshal_container(up, std::inserter(sid_num, sid_num.begin()));
      chUserNum[appid] = sid_num;
    }
  }
};

struct PGetVideoProxyInfo2: public sox::Marshallable {
  enum{uri = (10035 << 8 | protocol::SESSION_SVID)};
  uint32_t uid;
  uint32_t ip;
  uint32_t appid;
  uint32_t codeRate;
  virtual void marshal(sox::Pack &p) const {
    p << uid << ip << appid << codeRate;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> uid >> ip >> appid >> codeRate;
  }
};

struct PGetVideoProxyInfoFromAdm: public PGetVideoProxyInfo2
{
  enum{ uri = PGetVideoProxyInfoFromAdmURI };

  uint32_t topSid;

  virtual void marshal(sox::Pack &p) const 
  {
    PGetVideoProxyInfo2::marshal(p);
    p << topSid;
  }
  virtual void unmarshal(const sox::Unpack &up) 
  {
    PGetVideoProxyInfo2::unmarshal(up);
    up >> topSid;
  }
};

struct PGetVideoProxyInfoBatch: public sox::Marshallable 
{
  enum{ uri = PGetVideoProxyInfoBatchURI };

  uint32_t context;
  uint32_t topSid;
  uint32_t appid;
  uint32_t codeRate;
  std::set<uint32_t> uids;

  virtual void marshal(sox::Pack &p) const 
  {
    p << context << topSid << appid << codeRate;
    sox::marshal_container(p, uids);
  }
  virtual void unmarshal(const sox::Unpack &up) 
  {
    up >> context >> topSid >> appid >> codeRate;
    sox::unmarshal_container(up, std::inserter(uids, uids.begin()));
  }
};

struct IpInfo: public sox::Marshallable
{
  uint32_t ip;
  std::vector<uint16_t> tcpPorts;
  std::vector<uint16_t> udpPorts;

  void marshal(sox::Pack & pk) const{
    pk << ip;
    sox::marshal_container(pk, tcpPorts);
    sox::marshal_container(pk, udpPorts);
  }
  void unmarshal(const sox::Unpack &up){
    up >> ip;
    sox::unmarshal_container(up, std::back_inserter(tcpPorts));
    sox::unmarshal_container(up, std::back_inserter(udpPorts));
  }
};

struct PGetVideoProxyInfoResFromAdm: public sox::Marshallable
{
  enum { uri = PGetVideoProxyInfoResFromAdmURI };

  uint32_t appid;
  std::vector<IpInfo> videoProxyInfo;
  std::map<uint8_t, uint32_t> configInfo;
  uint32_t uid;
  LoginRetryProc retryType; // 重连操作控制
  uint16_t retryFreq;       // 重连频率控制，单位s

  PGetVideoProxyInfoResFromAdm() 
    : retryType(E_LOGIN_SUCCESS)
    , retryFreq(0) 
  {};

  void marshal(sox::Pack &p) const
  {
    p << appid;
    sox::marshal_container(p, videoProxyInfo);
    sox::marshal_container(p, configInfo);
    p << uid;
    p.push_uint8(retryType);
    p << retryFreq;
  }
  void unmarshal(const sox::Unpack &up)
  {
    up >> appid;
    sox::unmarshal_container(up, std::back_inserter(videoProxyInfo));
    sox::unmarshal_container(up, std::inserter(configInfo, configInfo.begin()));
    up >> uid;
    if (!up.empty())
    {
      retryType = (LoginRetryProc)up.pop_uint8();
      up >> retryFreq;
    }
  }
};

struct PGetVideoProxyInfoBatchRes: public sox::Marshallable
{
  enum { uri = PGetVideoProxyInfoBatchResURI };

  uint32_t context;
  uint32_t appid;
  std::vector<IpInfo> videoProxyInfo;
  std::set<uint32_t>  uids;
  LoginRetryProc retryType; // 重连操作控制
  uint16_t retryFreq;       // 重连频率控制，单位s

  PGetVideoProxyInfoBatchRes() 
    : retryType(E_LOGIN_SUCCESS)
    , retryFreq(0) 
  {};

  void marshal(sox::Pack &p) const
  {
    p << context << appid;
    sox::marshal_container(p, videoProxyInfo);
    sox::marshal_container(p, uids);
    p.push_uint8(retryType);
    p << retryFreq;
  }
  void unmarshal(const sox::Unpack &up)
  {
    up >> context >> appid;
    sox::unmarshal_container(up, std::back_inserter(videoProxyInfo));
    sox::unmarshal_container(up, std::inserter(uids, uids.begin()));
    retryType = (LoginRetryProc)up.pop_uint8();
    up >> retryFreq;
  }
};

struct PPingAppDaemon : public sox::Voidmable {
  enum { uri = PPingAppDaemonURI };
};

struct PPingAppDaemon2 : public sox::Marshallable {
  enum { uri = PPingAppDaemon2URI };

  uint32_t ip;
  uint32_t port;
  uint32_t reserved1;

  PPingAppDaemon2() :  reserved1(0) {}

  virtual void marshal( sox::Pack & p ) const {
    p << ip << port << reserved1;
  }

  virtual void unmarshal( const sox::Unpack & up ) {
    up >> ip >> port >> reserved1;
  }
};

//for local proxy admin
struct PLocalSmproxyAdminPingAppDaemon : public sox::Marshallable 
{
  enum { uri = PLocalSmproxyAdminPingAppDaemonURI };

  uint32_t ip;
  uint32_t port;
  std::set<uint16_t> inChargeAreas;
  uint32_t reserved1;
  uint32_t reserved2;

  PLocalSmproxyAdminPingAppDaemon() 
    :ip(0)
    ,port(0)
    ,reserved1(0) 
    ,reserved2(0)
    {}

  virtual void marshal( sox::Pack & p ) const {
    marshal_container(p, inChargeAreas);
    p << ip << port << reserved1 << reserved2;
  }

  virtual void unmarshal( const sox::Unpack & up ) {
    sox::unmarshal_container(up, std::inserter(inChargeAreas, inChargeAreas.begin()));    
    up >> ip >> port >> reserved1 >> reserved2;
  }
};

struct PProxyRegisterToLocalSmroxyAdmin: public sox::Marshallable
{
  enum{uri = PProxyRegisterToLocalSmroxyAdminURI};

  uint32_t ip;
  uint16_t tcp[4];
  uint16_t udp[4];
  uint16_t tcpPort2Prx;
  uint16_t udpPort2Prx;
  uint64_t proxyId;
  uint16_t isp;
  uint16_t sinfoVer; // channels info version
  uint16_t ulistVer; // user list version
  uint32_t proxyType;
  uint32_t groupId;  // jifang id
  uint32_t relayPort;
  uint32_t sgId;     // service group id
  uint32_t sessionId;
  uint32_t users;
  uint16_t status;    // proxy status  
  uint32_t reserver1;
  uint32_t reserver2;
  
  PProxyRegisterToLocalSmroxyAdmin()
    :ip(0)
    ,tcpPort2Prx(0)
    ,udpPort2Prx(0)
    ,proxyId(0)
    ,isp(0)
    ,sinfoVer(0) // channels info version
    ,ulistVer(0) // user list version
    ,proxyType(0)
    ,groupId(0)  // jifang id
    ,relayPort(0)
    ,sgId(0)     // service group id
    ,sessionId(0)
    ,users(0)
    ,status(0)
    ,reserver1(0)
    ,reserver2(0)
  {}

  virtual void marshal(sox::Pack &p) const {
    p << ip;
    for(int i=0;i<4;i++)p<<tcp[i];
    for(int i=0;i<4;i++)p<<udp[i];
    p << tcpPort2Prx << udpPort2Prx << proxyId << isp << sinfoVer << ulistVer
      << relayPort << sgId << sessionId << proxyType << groupId << users << status << reserver1
      << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> ip;
    for(int i=0;i<4;i++)up>>tcp[i];
    for(int i=0;i<4;i++)up>>udp[i];
    up >> tcpPort2Prx >> udpPort2Prx >> proxyId >> isp >> sinfoVer >> ulistVer 
       >>relayPort >> sgId >> sessionId >> proxyType >> groupId >> users >> status >> reserver1
       >> reserver2;
  }
};

struct PProxyUnregisterToLocalSmproxyAdmin: public sox::Marshallable
{
  enum{uri = PProxyUnregisterToLocalSmproxyAdminURI};

  uint32_t sgId;     // service group id
  uint64_t proxyId;
  uint32_t reserver1;
  uint32_t reserver2;

  PProxyUnregisterToLocalSmproxyAdmin() 
    : sgId(0)
    , proxyId(0)
    , reserver1(0)
    , reserver2(0)
  { }

  void marshal(sox::Pack & pk) const{
    pk << sgId << proxyId << reserver1 << reserver2;
  }
  void unmarshal(const sox::Unpack &up){
    up >> sgId >> proxyId >> reserver1 >> reserver2;
  }
};

struct PProxyRegisterToLocalSmproxyadminRes: public sox::Marshallable
{
  enum{uri = PProxyRegisterToLocalSmproxyadminResURI};
  enum RegResult {ACCEPT, REJECT};

  uint16_t result;
  uint16_t flags;
  uint32_t reserver1;
  uint32_t reserver2;

  PProxyRegisterToLocalSmproxyadminRes()
    : result(ACCEPT)
    , flags(0)
    , reserver1(0)
    , reserver2(0)
  {}

  virtual void marshal(sox::Pack &pk) const {
    
    pk << result << flags << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {

    up >> result >> flags >> reserver1 >> reserver2;
  }
};


struct PForbiddenGroup: public sox::Marshallable
{
  enum{uri = (2090 << 8 | protocol::SESSION_SVID)};
  std::set<uint32_t> forbids;
  virtual void marshal(sox::Pack &pak) const
  {
    marshal_container(pak, forbids);
  }
  virtual void unmarshal(const sox::Unpack &pak)
  {
    unmarshal_container(pak, std::inserter(forbids, forbids.end()));
  }
};

struct PProxySyncChannelUsersToLocalSmproxyAdmin: sox::Marshallable
{
  enum{ uri = PProxySyncChannelUsersToLocalSmproxyAdminURI };

  uint16_t mode; // 0: only sync updated channel; 1: sync all channels
  uint16_t version;
  uint32_t userNum;
  std::map<uint32_t, uint32_t> chUserNum;
  uint32_t reserver1;
  uint32_t reserver2;

  PProxySyncChannelUsersToLocalSmproxyAdmin() 
    : mode(0)
    , version(0)
    , userNum(0)
    , reserver1(0)
    , reserver2(0)
  { 
  }

  virtual void marshal(sox::Pack &pk) const {
    marshal_container(pk, chUserNum);
    pk << mode << version << userNum << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    unmarshal_container(up, std::inserter(chUserNum, chUserNum.end()));
    up >> mode >> version >> userNum >> reserver1 >> reserver2;
  }
};

struct PProxyPingLocalSmproxyAdmin: sox::Marshallable
{
  enum{ uri = PProxyPingLocalSmproxyAdminURI };

  uint32_t sgId;      // session group id
  uint64_t proxyid;
  uint32_t userNum;   // total user number in proxy
  uint16_t ulistVer;  // user list version
  uint32_t timestamp;
  uint16_t status;    // proxy status
  uint32_t reserver1;
  uint32_t reserver2;

  PProxyPingLocalSmproxyAdmin()
    :sgId(0)
    ,proxyid(0)
    ,userNum(0)
    ,ulistVer(0)
    ,timestamp(0)
    ,status(0)
    ,reserver1(0)
    ,reserver2(0)
  {}

  virtual void marshal(sox::Pack &pk) const {
    pk << sgId << proxyid << userNum << ulistVer << timestamp 
       << status << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> sgId >> proxyid >> userNum >> ulistVer >> timestamp
       >> status >> reserver1 >> reserver2;
  }
};

struct PProxyPingLocalSmproxyAdminRes: public sox::Marshallable
{
  enum{ uri = PProxyPingLocalSmproxyAdminResURI };
  uint16_t flags;
  uint32_t timestamp;
  uint32_t reserver1;
  uint32_t reserver2;

  PProxyPingLocalSmproxyAdminRes()
    :flags(0)
    ,timestamp(0)
    ,reserver1(0)
    ,reserver2(0)
  {}
  virtual void marshal(sox::Pack &pk) const {
    pk << flags << timestamp << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> flags >> timestamp >> reserver1 >> reserver2;
  }
};

//to local smproxy admin
struct PProxyChannelsRemove1: sox::Marshallable
{
  enum{ uri = PProxyChannelsRemove1URI };
  std::set<uint32_t> sidList;
  uint32_t reserver1;
  uint32_t reserver2;

  PProxyChannelsRemove1()
    : reserver1(0)
    , reserver2(0)
  {}

  virtual void marshal(sox::Pack &pk) const {
    marshal_container(pk, sidList);
    pk << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    unmarshal_container(up, std::inserter(sidList, sidList.begin()));
    up >> reserver1 >> reserver2;
  }
};

//to local smproxy admin
struct PProxyChannelsCreate: sox::Marshallable
{
  enum{ uri = PProxyChannelsCreateURI };
  std::set<uint32_t> sidList;
  uint32_t reserver1;
  uint32_t reserver2;

  PProxyChannelsCreate()
    : reserver1(0)
    , reserver2(0)
  {}

  virtual void marshal(sox::Pack &pk) const {
    marshal_container(pk, sidList);
    pk << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    unmarshal_container(up, std::inserter(sidList, sidList.begin()));
    up >> reserver1 >> reserver2;
  }
};

//local smproxyadmin to proxy
struct PRequireProxyInfo: sox::Marshallable
{
  enum{ uri = PRequireProxyInfoURI };
  uint32_t reserver1;
  uint32_t reserver2;

  PRequireProxyInfo()
    : reserver1(0)
    , reserver2(0)
  {}

  virtual void marshal(sox::Pack &pk) const {
    pk << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> reserver1 >> reserver2;
  }
};

//proxy to local smproxyadmin
struct PRequireProxyInfoRes: sox::Marshallable
{
  enum{ uri = PRequireProxyInfoResURI };
  uint64_t proxyid;
  std::string proxyVersion;
  uint32_t groupid;
  uint16_t proxyStatus;
  uint32_t reserver1;
  uint32_t reserver2;

  PRequireProxyInfoRes()
    : proxyid(0)
    , groupid(0)
    , proxyStatus(0)
    , reserver1(0)
    , reserver2(0)
  {}

  virtual void marshal(sox::Pack &pk) const {
    pk << proxyid << proxyVersion << groupid << proxyStatus << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> proxyid >> proxyVersion >> groupid >> proxyStatus >> reserver1 >> reserver2;
  }
};

//session -> local smproxyadmin to proxy
struct PRequireInChargeArea: sox::Marshallable
{
  enum{ uri = PRequireInChargeAreaURI };
  uint32_t session;
  uint32_t reserver1;
  uint32_t reserver2;

  PRequireInChargeArea()
    : session(0)
    , reserver1(0)
    , reserver2(0)
  {}

  virtual void marshal(sox::Pack &pk) const {
    pk << session << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> session >> reserver1 >> reserver2;
  }
};

//local smproxyadmin to proxy  ->  session 
struct PRequireInChargeAreaRes: sox::Marshallable
{
  enum{ uri = PRequireInChargeAreaResURI };
  uint32_t localsmproxyadmin;
  std::vector<uint16_t> inChargeAreas;
  uint32_t reserver1;
  uint32_t reserver2;

  PRequireInChargeAreaRes()
    : localsmproxyadmin(0)
    , reserver1(0)
    , reserver2(0)
  {}

  virtual void marshal(sox::Pack &pk) const {
    marshal_container(pk, inChargeAreas);    
    pk << localsmproxyadmin << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    unmarshal_container(up, std::back_inserter(inChargeAreas));
    up >> localsmproxyadmin >> reserver1 >> reserver2;
  }
};

// Session向local smproxy admind 请求前端,分区版本
struct PRequireSignalProxyToLocalSmproxyAdmin : public sox::Marshallable
{
  enum{uri = (PRequireSignalProxyToLocalSmproxyAdminURI)};
  PRequireSignalProxyToLocalSmproxyAdmin()
    :session(0)
    ,sessiongroup(0)
    ,areaType(0)
    ,count(0)
    ,reserver1(0)
    ,reserver2(0)
  {}

  uint16_t ports[4];  // Session服务端口
  uint32_t session;
  uint32_t sessiongroup;
  std::string sessionVersion;
  uint32_t areaType;  // 请求的前端网络分区类型
  uint32_t count;
  uint32_t reserver1;
  uint32_t reserver2;  
  virtual void marshal(sox::Pack &pak) const {
    for(int i=0; i < 4; i++)pak << ports[i];
    pak << session << sessiongroup << sessionVersion << areaType << count << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    for(int i=0; i<4 ; i++)pak >> ports[i];
    pak >> session >> sessiongroup >> sessionVersion >> areaType >> count >> reserver1 >> reserver2;
  }
};

struct PRequireSignalProxyToLocalSmproxyAdminRes : public sox::Marshallable
{
  enum{uri = PRequireSignalProxyToLocalSmproxyAdminResURI};
  PRequireSignalProxyToLocalSmproxyAdminRes()
    :localsmproxyadmin(0)
    ,count(0)
    ,reserver1(0)
    ,reserver2(0)
  {}
  
  uint32_t localsmproxyadmin;
  uint32_t count;   // 通知前端数
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pak) const {
    pak << localsmproxyadmin << count << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    pak >> localsmproxyadmin >> count >> reserver1 >> reserver2;
  }
};


struct PSessionRequireSignalProxy : public sox::Marshallable
{
  enum{uri = (PSessionRequireSignalProxyURI)};
  PSessionRequireSignalProxy()
    :session(0)
    ,ip(0)
    ,reserver1(0)
    ,reserver2(0)
  {}

  uint16_t ports[4];  // Session服务端口
  uint32_t session; //serverid
  uint32_t ip;      // SessionIP
  uint32_t reserver1;
  uint32_t reserver2;
  virtual void marshal(sox::Pack &pak) const {
    for(int i=0; i < 4; i++)pak << ports[i];
    pak << session << ip << reserver1 << reserver2;
  }
  virtual void unmarshal(const sox::Unpack &pak) {
    for(int i=0; i<4 ; i++)pak >> ports[i];
    pak >> session >> ip >> reserver1 >> reserver2;
  }
};


struct PProxyStatusReport: public sox::Marshallable
{
  enum{uri = (15023 << 8 | protocol::SESSION_SVID)};
  std::map<uint64_t, bool> status;
  virtual void marshal(sox::Pack &pk) const
  {
    marshal_container(pk, status);
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    unmarshal_container(up, std::inserter(status, status.end()));
  }
};

struct RelocateChannelInfo: public sox::Marshallable
{
  uint32_t users;
  std::vector<protocol::session::IpInfo> relocateMediaProxyInfo;

  void marshal(sox::Pack &pk) const{
    pk << users;
    marshal_container(pk, relocateMediaProxyInfo);
  }

  void unmarshal(const sox::Unpack &up){
    up >> users;
    unmarshal_container(up, std::back_inserter(relocateMediaProxyInfo));
  }
};

struct PForceConnectMediaProxySM: public sox::Marshallable
{
  enum{ uri = PForceConnectMediaProxySMURI };

  std::map<uint32_t, RelocateChannelInfo>  relocateChannels;
  uint32_t reserved1;

  virtual void marshal(sox::Pack &pk) const
  {
    pk << reserved1;
    marshal_container(pk, relocateChannels);
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> reserved1;
    unmarshal_container(up, std::inserter(relocateChannels, relocateChannels.end()));
  }
};

enum ProxyAdminBcEnum 
{
  BC_TO_TOPSID = 0x1,
  BC_TO_SIDS   = 0x2,
  BC_TO_USERS  = 0x4,
  BC_TO_GROUP  = 0x8,
};

struct PProxyAdminBroadcast: public sox::Marshallable
{
  enum{ uri = PProxyAdminBroadcastURI };

  uint32_t fromServerId;
  ProxyAdminBcEnum bcMode;
  uint32_t sessGroupId;
  uint32_t topSid;
  std::set<uint32_t> sids;
  std::set<uint32_t> users;
  uint32_t seq;
  uint32_t reserved2;
  std::string data;

  PProxyAdminBroadcast() : sessGroupId(0),topSid(0), seq(0),reserved2(0) {}

  virtual void marshal(sox::Pack &pk) const
  {
    pk << fromServerId;
    pk.push_uint16((uint16_t)bcMode);
    pk << sessGroupId << topSid << seq << reserved2;
    marshal_container(pk, sids);
    marshal_container(pk, users);
    pk.push_varstr32(data.data(), data.size());
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> fromServerId;
    bcMode = (ProxyAdminBcEnum)up.pop_uint16();
    up >> sessGroupId >> topSid >> seq >> reserved2;
    unmarshal_container(up, std::inserter(sids, sids.end()));
    unmarshal_container(up, std::inserter(users, users.end()));
    data = up.pop_varstr32();
  }
};

struct PProxyAdminBroadcastRes: public sox::Marshallable
{
  enum{ uri = PProxyAdminBroadcastResURI };

  uint32_t serverid;
  uint16_t resCode;
  uint32_t sessGroupId;
  uint32_t topSid;
  uint16_t bcProxies;
  std::set<uint64_t> failProxies;
  uint32_t bcBw;

  uint32_t seq;
  uint32_t reserved2;

  PProxyAdminBroadcastRes() : resCode(RES_SUCCESS), sessGroupId(0), topSid(0), bcBw(0), seq(0), reserved2(0) {};

  virtual void marshal(sox::Pack &pk) const
  {
    pk << serverid << resCode << sessGroupId << topSid << bcProxies << bcBw << seq << reserved2;
    marshal_container(pk, failProxies);
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> serverid >> resCode >> sessGroupId >> topSid >> bcProxies >> bcBw >> seq >> reserved2;
    unmarshal_container(up, std::inserter(failProxies, failProxies.end()));
  }
};

struct GroupChannels: public sox::Marshallable
{
  std::set<uint32_t> topSids;
  uint32_t reserved;

  GroupChannels(): reserved(0) {}

  virtual void marshal(sox::Pack &pk) const
  {
    marshal_container(pk, topSids);
    pk << reserved;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    unmarshal_container(up, std::inserter(topSids, topSids.end()));
    up >> reserved;
  }
};

struct PProxyAdminBroadcastBatch: public sox::Marshallable
{
  enum{ uri = PProxyAdminBroadcastBatchURI };

  uint32_t fromServerId;
  std::map<uint32_t, GroupChannels> channels;
  uint32_t seq;
  uint32_t reserved1;
  std::string data;

  PProxyAdminBroadcastBatch() : seq(0),reserved1(0) {}

  virtual void marshal(sox::Pack &pk) const
  {
    pk << fromServerId;
    marshal_container(pk, channels);
    pk << seq << reserved1;
    pk.push_varstr32(data.data(), data.size());
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> fromServerId;
    unmarshal_container(up, std::inserter(channels, channels.end()));
    up >> seq >> reserved1;
    data = up.pop_varstr32();
  }
};

struct PProxyAdminBroadcastBatchRes: public sox::Marshallable
{
  enum{ uri = PProxyAdminBroadcastBatchResURI };
  
  uint32_t serverid;
  uint16_t resCode;
  std::set<uint64_t> failProxies;
  uint32_t bcBw;

  uint32_t seq;
  uint32_t reserved2;

  PProxyAdminBroadcastBatchRes() : resCode(RES_SUCCESS), bcBw(0), seq(0), reserved2(0) {};

  virtual void marshal(sox::Pack &pk) const
  {
    pk << serverid << resCode << bcBw << seq << reserved2;
    marshal_container(pk, failProxies);
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> serverid >> resCode >> bcBw >> seq >> reserved2;
    unmarshal_container(up, std::inserter(failProxies, failProxies.end()));
  }
};

enum SessionEvent
{
  EVT_CHANNEL_DESTROY = 1,
  EVT_CHANNEL_CREATE  = 2,
  EVT_SEGROUP_DESTROY = 4,
  EVT_SEGROUP_CREATE  = 8,
};

struct PSubscribeEvent: public sox::Marshallable
{
  enum{ uri = PSubscribeEventURI };

  uint16_t toWatchEvent;
  uint32_t fromServerId;
  uint32_t reserved1;
  uint32_t reserved2;

  PSubscribeEvent() 
    : toWatchEvent(0)
    , fromServerId(0)
    , reserved1(0)
    , reserved2(0)
  {}

  virtual void marshal(sox::Pack &pk) const
  {
    pk << toWatchEvent << fromServerId << reserved1 << reserved2;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> toWatchEvent >> fromServerId >> reserved1 >> reserved2;
  }
};

struct PUnsubscribeEvent: public PSubscribeEvent
{
  enum{ uri = PUnsubscribeEventURI };
};

struct PEventNotify_ChannelDestroy: public sox::Marshallable
{
  enum{ uri = PEventNotify_ChannelDestroyURI };

  uint32_t serverid;
  std::map<uint32_t, GroupChannels> channels;
  uint32_t reserved1;
  uint32_t reserved2;

  PEventNotify_ChannelDestroy() : reserved1(0),reserved2(0) {}

  virtual void marshal(sox::Pack &pk) const
  {
    pk << serverid;
    marshal_container(pk, channels);
    pk << reserved1 << reserved2;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> serverid;
    unmarshal_container(up, std::inserter(channels, channels.end()));
    up >> reserved1 >> reserved2;
  }
};

struct PEventNotify_ChannelCreate: public sox::Marshallable
{
  enum{ uri = PEventNotify_ChannelCreateURI };

  uint32_t serverid;
  std::map<uint32_t, GroupChannels> channels;
  uint32_t reserved1;
  uint32_t reserved2;

  PEventNotify_ChannelCreate() : reserved1(0),reserved2(0) {}

  virtual void marshal(sox::Pack &pk) const
  {
    pk << serverid;
    marshal_container(pk, channels);
    pk << reserved1 << reserved2;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> serverid;
    unmarshal_container(up, std::inserter(channels, channels.end()));
    up >> reserved1 >> reserved2;
  }
};

struct PGetSessionGroupList : public sox::Marshallable
{
  enum { uri = PGetSessionGroupListURI };

  uint32_t fromServerId;

  virtual void marshal(sox::Pack &pk) const
  {
    pk << fromServerId ;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> fromServerId ;
  }
};

struct PGetSessionGroupListRes: public sox::Marshallable
{
  enum{ uri = PGetSessionGroupListResURI };

  uint32_t serverid;
  std::set<uint32_t> sessionGroups;

  virtual void marshal(sox::Pack &pk) const
  {
    pk << serverid;
    marshal_container(pk, sessionGroups);
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> serverid;
    unmarshal_container(up, std::inserter(sessionGroups, sessionGroups.end()));
  }
};

struct PGetSessionGroupChannelList: public sox::Marshallable
{
  enum{ uri = PGetSessionGroupChannelListURI };

  uint32_t fromServerId;
  uint32_t sessionGroup;
  uint32_t reserved;

  PGetSessionGroupChannelList() : reserved(0) {};

  virtual void marshal(sox::Pack &pk) const
  {
    pk << fromServerId << sessionGroup << reserved;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> fromServerId >> sessionGroup >> reserved;
  }
};

struct PGetSessionGroupChannelListRes: public sox::Marshallable
{
  enum{ uri = PGetSessionGroupChannelListResURI };

  uint32_t serverid;
  uint32_t sessionGroup;
  uint32_t reserved;
  std::set<uint32_t> channels;
  
  PGetSessionGroupChannelListRes() : reserved(0) {};

  virtual void marshal(sox::Pack &pk) const
  {
    pk << serverid << sessionGroup << reserved;
    marshal_container(pk, channels);
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> serverid >> sessionGroup >> reserved;
    unmarshal_container(up, std::inserter(channels, channels.end()));
  }
};

// for broadcast
struct PBC_ProxyPing: public sox::Marshallable
{
  enum{ uri = PBC_ProxyPingURI };

  uint64_t pid;
  uint32_t sessionGroup;
  uint32_t proxyType;
  uint32_t chVersion;
  uint32_t reserved;

  PBC_ProxyPing() : reserved(0) {};

  virtual void marshal(sox::Pack &pk) const
  {
    pk << pid << sessionGroup << proxyType << chVersion << reserved;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> pid >> sessionGroup >> proxyType >> chVersion >> reserved;
  }
};

struct PBC_ProxyPingRes: public sox::Marshallable
{
  enum{ uri = PBC_ProxyPingResURI };

  uint64_t pid;
  uint32_t sessionGroup;
  uint32_t cBits;    // control bits
  uint32_t reserved;

  PBC_ProxyPingRes() : cBits(0), reserved(0) {};

  virtual void marshal(sox::Pack &pk) const
  {
    pk << pid << sessionGroup << cBits << reserved;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> pid >> sessionGroup >> cBits >> reserved;
  }
};

struct PBC_ProxyEvent_ChannelCreate: public sox::Marshallable
{
  enum{ uri = PBC_ProxyEvent_ChannelCreateURI };

  uint64_t pid;
  uint32_t sessionGroup;
  uint32_t sid;
  uint32_t chVersion;
  uint32_t reserved;

  PBC_ProxyEvent_ChannelCreate() : reserved(0) {};

  virtual void marshal(sox::Pack &pk) const
  {
    pk << pid << sessionGroup << sid << chVersion << reserved;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> pid >> sessionGroup >> sid >> chVersion >> reserved;
  }
};

struct PBC_ProxyEvent_ChannelRemove: public sox::Marshallable
{
  enum{ uri = PBC_ProxyEvent_ChannelRemoveURI };

  uint64_t pid;
  uint32_t sessionGroup;
  uint32_t sid;
  uint32_t chVersion;
  uint32_t reserved;

  PBC_ProxyEvent_ChannelRemove() : reserved(0) {};

  virtual void marshal(sox::Pack &pk) const
  {
    pk << pid << sessionGroup << sid << chVersion << reserved;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> pid >> sessionGroup >> sid >> chVersion >> reserved;
  }
};

struct PBC_ProxySync: public sox::Marshallable
{
  enum{ uri = PBC_ProxySyncURI };

  uint64_t pid;
  uint32_t sessionGroup;
  uint32_t chVersion;
  uint32_t reserved;
  std::set<uint32_t> channels;

  PBC_ProxySync() : reserved(0) {};

  virtual void marshal(sox::Pack &pk) const
  {
    pk << pid << sessionGroup << chVersion << reserved;
    marshal_container(pk, channels);
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> pid >> sessionGroup >> chVersion >> reserved;
    unmarshal_container(up, std::inserter(channels, channels.end()));
  }
};

struct PBC_Route: public sox::Marshallable
{
  enum{ uri = PBC_RouteURI };

  uint32_t fromServerId;
  uint32_t sessionGroup;
  uint64_t pid;
  std::string key;
  std::string to;
  std::string load;

  PBC_Route() : fromServerId(0),sessionGroup(0),pid(0) {};

  virtual void marshal(sox::Pack &pk) const
  {
    pk << fromServerId << sessionGroup << pid << key << to;
    pk.push_varstr32(load.data(), load.size());
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> fromServerId >> sessionGroup >> pid >> key >> to;
    load = up.pop_varstr32();
  }
};

struct BroadCastDest: public sox::Marshallable
{
  ProxyAdminBcEnum bcMode;
  uint32_t topSid;
  std::set<uint32_t> sids;
  std::set<uint32_t> users;

  BroadCastDest() : bcMode(BC_TO_TOPSID),topSid(0) {}

  virtual void marshal(sox::Pack &pk) const
  {
    pk.push_uint16((uint16_t)bcMode);
    pk << topSid;
    marshal_container(pk, sids);
    marshal_container(pk, users);
  }

  virtual void unmarshal(const sox::Unpack &up)
  {
    bcMode = (ProxyAdminBcEnum)up.pop_uint16();
    up >> topSid;
    unmarshal_container(up, std::inserter(sids, sids.end()));
    unmarshal_container(up, std::inserter(users, users.end()));
  }
};

struct PBC_BroadCastToUser: public sox::Marshallable
{
  enum{ uri = PBC_BroadCastToUserURI };

  uint32_t fromServerId;
  uint32_t proxyType;
  std::vector<BroadCastDest> destList;
  uint32_t seq;
  uint32_t delay;
  uint32_t timestamp;
  uint32_t sessGroupId;
  std::string load;

  PBC_BroadCastToUser() : fromServerId(0),proxyType(0),seq(0),delay(60),timestamp(0),sessGroupId(0) {};

  virtual void marshal(sox::Pack &pk) const
  {
    pk << fromServerId << proxyType;
    marshal_container(pk, destList);
    pk << seq << delay << timestamp << sessGroupId;
    pk.push_varstr32(load.data(), load.size());
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> fromServerId >> proxyType;
    unmarshal_container(up, std::back_inserter(destList));
    up >> seq >> delay >> timestamp >> sessGroupId;
    load = up.pop_varstr32();
  }
};

struct PBC_BroadCastAck: public sox::Marshallable
{
  enum{ uri = PBC_BroadCastAckURI };

  uint32_t serverid;
  uint32_t seq;
  uint32_t reserved;

  PBC_BroadCastAck() : serverid(0),seq(0),reserved(0) {};

  virtual void marshal(sox::Pack &pk) const
  {
    pk << serverid << seq << reserved;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> serverid >> seq >> reserved;
  }
};

struct PBC_BroadCastRes: public sox::Marshallable
{
  enum{ uri = PBC_BroadCastResURI };

  uint32_t serverid;
  uint32_t seq;
  uint32_t proxyNum;
  uint32_t bwUsed;
  uint32_t reserved;

  PBC_BroadCastRes() : serverid(0),seq(0),proxyNum(0),bwUsed(0),reserved(0) {};

  virtual void marshal(sox::Pack &pk) const
  {
    pk << serverid << seq << proxyNum << bwUsed << reserved;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> serverid >> seq >> proxyNum >> bwUsed >> reserved;
  }
};

struct PBC_BroadCastToAllProxy: public sox::Marshallable
{
  enum{ uri = PBC_BroadCastToAllProxyURI };

  uint32_t fromServerId;
  uint32_t proxyType;
  uint32_t includeIdleProxy;
  uint32_t seq;
  uint32_t delay;
  uint32_t timestamp;
  uint32_t reserved;
  std::string load;

  PBC_BroadCastToAllProxy() : fromServerId(0),proxyType(0),includeIdleProxy(0)
    ,seq(0),delay(60),timestamp(0),reserved(0) {};

  virtual void marshal(sox::Pack &pk) const
  {
    pk << fromServerId << proxyType << includeIdleProxy;
    pk << seq << delay << timestamp << reserved;
    pk.push_varstr32(load.data(), load.size());
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> fromServerId >> proxyType >> includeIdleProxy;
    up >> seq >> delay >> timestamp >> reserved;
    load = up.pop_varstr32();
  }
};

struct PBC_BroadCastToProxyByChannel: public sox::Marshallable
{
  enum{ uri = PBC_BroadCastToProxyByChannelURI };

  uint32_t fromServerId;
  uint32_t proxyType;
  uint32_t topSid;
  uint32_t seq;
  uint32_t delay;
  uint32_t timestamp;
  uint32_t reserved;
  std::string load;

  PBC_BroadCastToProxyByChannel() : fromServerId(0),proxyType(0), 
    topSid(0), seq(0), delay(60),timestamp(0),reserved(0) {};

  virtual void marshal(sox::Pack &pk) const
  {
    pk << fromServerId << proxyType << topSid;
    pk << seq << delay << timestamp << reserved;
    pk.push_varstr32(load.data(), load.size());
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> fromServerId >> proxyType >> topSid;
    up >> seq >> delay >> timestamp >> reserved;
    load = up.pop_varstr32();
  }
};

struct PBC_RouteBroadCast: public sox::Marshallable
{
  enum{ uri = PBC_RouteBroadCastURI };

  uint32_t fromServerId;
  uint32_t sessionGroup;
  uint32_t proxyType;
  std::vector<uint64_t> proxyList;
  std::string load;

  virtual void marshal(sox::Pack &pk) const
  {
    pk << fromServerId << sessionGroup << proxyType;
    marshal_container(pk, proxyList);
    pk.push_varstr32(load.data(), load.size());
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> fromServerId >> sessionGroup >> proxyType;
    unmarshal_container(up, std::back_inserter(proxyList));
    load = up.pop_varstr32();
  }
};

struct PBC_ProxyEvent_GroupCreate: public sox::Marshallable
{
  enum{ uri = PBC_ProxyEvent_GroupCreateURI };

  uint32_t serverid;
  std::set<uint32_t> groups;
  uint32_t reserved1;

  PBC_ProxyEvent_GroupCreate() : serverid(0), reserved1(0) {}

  virtual void marshal(sox::Pack &pk) const
  {
    pk << serverid;
    marshal_container(pk, groups);
    pk << reserved1;
  }
  virtual void unmarshal(const sox::Unpack &up)
  {
    up >> serverid;
    unmarshal_container(up, std::inserter(groups, groups.end()));
    up >> reserved1;
  }
};

struct PBC_ProxyEvent_GroupRemove: public PBC_ProxyEvent_GroupCreate
{
  enum{ uri = PBC_ProxyEvent_GroupRemoveURI };
};

struct PBC_Ping: public sox::Marshallable
{
  enum{ uri = PBC_PingURI };

  uint32_t pingTime;

  PBC_Ping(): pingTime(0) {}

  virtual void marshal(sox::Pack &p) const 
  {
    p << pingTime;
  }
  virtual void unmarshal(const sox::Unpack &p) 
  {
    p >> pingTime;
  }
};

struct PBC_PingRes: public sox::Marshallable
{
  enum{ uri = PBC_PingResURI };

  uint32_t pingTime;
  uint32_t isReady;

  PBC_PingRes(): pingTime(0),isReady(0) {}

  virtual void marshal(sox::Pack &p) const 
  {
    p << pingTime << isReady;
  }
  virtual void unmarshal(const sox::Unpack &p) 
  {
    p >> pingTime >> isReady;
  }
};

//smpreprocess manager -> smdispacher
struct PreProcessManagerReq : public sox::Marshallable 
{
   enum {uri = PreProcessManagerReqURI};
   enum RequestType {Register = 10, Check = 20, AddSids = 30, RemoveSids = 40, Unregister = 50};
   uint32_t sequenceid;
   uint32_t requestType;
   uint32_t serverid;       // preprocess manager serverid
   uint32_t group;  // preprocess manager groupid
   std::set<uint32_t> sids;
   uint32_t reserved1;
   uint32_t reserved2;

   PreProcessManagerReq()
    :sequenceid(0)
    ,requestType(0)
    ,serverid(0)
    ,group(0)
    ,reserved1(0)
    ,reserved2(0)
   {}
   
   virtual void marshal(sox::Pack &p) const 
   {
     p << sequenceid << requestType << serverid << group << reserved1 << reserved2;
     sox::marshal_container(p, sids);
   }
   virtual void unmarshal(const sox::Unpack &p) 
   {
     p >> sequenceid >> requestType >> serverid >> group >> reserved1 >> reserved2;
     sox::unmarshal_container(p, std::inserter(sids, sids.end()));
   }
   
};

//smdispacher -> smpreprocess manager 

struct PreProcessManagerRes : public sox::Marshallable 
{
   enum {uri = PreProcessManagerResURI};
   enum {Ok = 200, Failure = 400};
   uint32_t sequenceid;
   uint32_t requestType;
   uint32_t serverid;       // smdispacher serverid
   uint32_t group;  // smdispacher groupid
   uint32_t code;
   uint32_t reserved1;
   uint32_t reserved2;

   PreProcessManagerRes()
    :sequenceid(0)
    ,requestType(0)
    ,serverid(0)
    ,group(0)
    ,code(0)
    ,reserved1(0)
    ,reserved2(0)
   {}
   
   virtual void marshal(sox::Pack &p) const 
   {
     p << sequenceid << requestType << serverid << group << code << reserved1 << reserved2;
   }
   virtual void unmarshal(const sox::Unpack &p) 
   {
     p >> sequenceid >> requestType >> serverid >> group >> code >> reserved1 >> reserved2;
   }
   
};

//smdispacher -> sm pre process mgr
struct PreProcessChannel : public sox::Marshallable {
  enum {uri = PreProcessChannelURI};
  uint32_t requestSid;
  std::string  from;
  uint32_t   sid;
  uint32_t    asid;
  uint32_t   deploy;
  uint32_t   shard;
  uint32_t  reserved1; 
  uint32_t  reserved2;
  PreProcessChannel()
    :requestSid(0)
    ,sid(0)
    ,asid(0)
    ,deploy(0)
    ,shard(0)
    ,reserved1(0)
    ,reserved2(0)
  {}
  
  void marshal(sox::Pack &pk) const{
    pk << requestSid << from << sid << asid << deploy << shard << reserved1 << reserved2;
  }

  void unmarshal(const sox::Unpack &up){
    up >> requestSid >> from >> sid >> asid >> deploy >> shard >> reserved1 >> reserved2;
  }
};


//smpretreat 
struct PpretreatRequest : public sox::Marshallable {
  enum {uri = PpretreatRequestURI};
  enum RequestType {Register = 10, Check=20, AddSids=30, RemoveSids=40, Unregister=50};
  enum ServiceType {Stop = 10, Conditioned=20, Continue=30, FullControl=40};
  uint32_t  requestType;     // 1 is register, 2 is add, 3 is remove, 4 is unregister, unregister ignore the sids
  uint32_t   seqence; 
  uint32_t serverId;             // smpretreat serverid
  uint32_t groupId;        //locataion id
  uint32_t serviceType;      // stop, conditioned or continue lookup channel location
  uint32_t serviceGroupId;  // service groupid
  std::set<uint32_t>  sids;    // unregister ingnore sids;

  PpretreatRequest()
    :requestType(0)
    ,seqence(0)
    ,serverId(0)
    ,groupId(0)
    ,serviceType(0)
    ,serviceGroupId(0)
  {
  }
  void marshal(sox::Pack &pk) const{
    pk  << requestType << seqence << serverId << groupId << serviceType << serviceGroupId;
    sox::marshal_container(pk,sids);
  }

  void unmarshal(const sox::Unpack &up){
    up >> requestType >> seqence >> serverId >> groupId >> serviceType >> serviceGroupId;
    sox::unmarshal_container(up, std::inserter(sids, sids.end()));
  }
};

struct PpretreatRequestRes : public sox::Marshallable {
  enum {uri = PpretreatRequestResURI};
  enum {Ok=200, Failure = 400};
  uint32_t code; // ok, or failure
  uint32_t  requestType; 
  uint32_t  seqence;    
  uint32_t serverId;       // sm pre manager server id
  uint32_t group;        // sm pre manager group
  uint32_t serviceType;    // stop, conditioned or continue lookup channel location
  uint32_t serviceGroupId;  // service groupid
  uint32_t reserved1;
  uint32_t reserved2;

  PpretreatRequestRes()
    :code(0)
    ,requestType(0)
    ,seqence(0)
    ,serverId(0)
    ,group(0)
    ,serviceType(0)
    ,serviceGroupId(0)
    ,reserved1(0)
    ,reserved2(0)
  {
  }
  
  void marshal(sox::Pack &pk) const{
    pk << code << requestType << seqence << serverId  << group << serviceType << serviceGroupId << reserved1 << reserved2;
  }

  void unmarshal(const sox::Unpack &up){
    up >> code >> requestType >> seqence  >> serverId >> group >> serviceType >> serviceGroupId >> reserved1 >> reserved2;
  }
};

struct PpretreatChannel : public sox::Marshallable {
  enum {uri = PpretreatchannelURI};
  uint32_t taskid;
  uint32_t requestSid;
  uint32_t   sid;
  uint32_t    asid;
  uint32_t   deploy;
  uint32_t   shard;
  std::string  from;
  uint32_t    serviceType;   //用来验证预处理类型
  uint32_t    serviceGroupId; //
  uint32_t  reserved1; 
  uint32_t  reserved2;
  std::map<std::string, std::string>  preResponse; 
  PpretreatChannel()
    :taskid(0)
    ,requestSid(0)
    ,sid(0)
    ,asid(0)
    ,deploy(0)
    ,shard(0)
    ,serviceType(0)
    ,serviceGroupId(0)
    ,reserved1(0)
    ,reserved2(0)
  {}
  
  void marshal(sox::Pack &pk) const{
    pk << taskid << requestSid << sid << asid << deploy << shard << from << serviceType << serviceGroupId<< reserved1 << reserved2;
    sox::marshal_container(pk, preResponse);
  }

  void unmarshal(const sox::Unpack &up){
    up >> taskid >> requestSid >> sid >> asid >> deploy >> shard >> from >> serviceType >> serviceGroupId >> reserved1 >> reserved2;
    sox::unmarshal_container(up, std::inserter(preResponse, preResponse.end()) );
  }
};

struct PpretreatChannelRes: public sox::Marshallable {
  enum { uri = PpretreatChannelResURI};
  enum Result { Stop = 10, Continue=20, Timeout =30};  
  uint32_t taskid;
  uint32_t  code;
  uint32_t  sid;
  uint32_t  asid;
  std::string  from;       // uid._r/serverid
  uint32_t  result;       // 1 stop; 2 conditioned; 3 continue , 4 timeout; 
  uint32_t   serviceType;  //
  uint32_t   serviceGroupId;
  uint32_t  errorNo;  
  uint32_t  reserved2;
  std::map<std::string, std::string>  preResponse;
  PpretreatChannelRes()
    :taskid(0)
    ,code(0)
    ,sid(0)
    ,asid(0)
    ,result(0)
    ,serviceType(0)
    ,serviceGroupId(0)
    ,errorNo(0)
    ,reserved2(0)
  {}
  
  void marshal(sox::Pack &pk) const{
    pk  << taskid << code << sid << asid << from << result << serviceType << serviceGroupId<< errorNo << reserved2;
    sox::marshal_container(pk, preResponse);
  }

  void unmarshal(const sox::Unpack &up){
    up >> taskid >> code >> sid >> asid >> from >> result >> serviceType >> serviceGroupId >> errorNo >> reserved2;
    sox::unmarshal_container(up, std::inserter(preResponse, preResponse.end()) );
  }
};

// -> smdispacher to check channel
struct PInqueryChannel : public sox::Marshallable {
  enum {uri = PInqueryChannelURI};
  uint32_t sid;
  uint32_t  reserved1; 
  uint32_t  reserved2;
  bool needPreProcess;
  bool needAllocate;
  PInqueryChannel()
    :sid(0)
    ,reserved1(0)
    ,reserved2(0)
    ,needPreProcess(false)
    ,needAllocate(true)
  {}
  
  void marshal(sox::Pack &pk) const{
    pk << sid << reserved1 << reserved2;
    pk.push_uint32(needPreProcess);
    pk.push_uint32(needAllocate);
  }

  void unmarshal(const sox::Unpack &up){
    up >> sid >> reserved1 >> reserved2;
    needPreProcess = up.pop_uint32() != 0;
    needAllocate = up.pop_uint32() != 0;
  }
};

// -> smdispacher to check channel
struct PInqueryChannel1 : public sox::Marshallable {
  enum {uri = PInqueryChannel1URI};
  uint32_t sid;
  uint32_t  reserved1;
  uint32_t  reserved2;
  uint32_t  reserved3;
  bool needAllocate;
  
  PInqueryChannel1()
    :sid(0)
    ,reserved1(0)
    ,reserved2(0)
    ,reserved3(0)
    ,needAllocate(true)
  {}
  
  void marshal(sox::Pack &pk) const{
    pk << sid << reserved1 << reserved2 << reserved3;
    pk.push_uint32(needAllocate);
  }

  void unmarshal(const sox::Unpack &up){
    up >> sid >> reserved1 >> reserved2 >> reserved3;
    needAllocate = up.pop_uint32() != 0;
  }
};

//smdispacher -> pkgStatic, to report smdispacher's statistic
struct PSmdispatcherStat : public sox::Marshallable
{
    enum {uri = (801 << 8) | protocol::CATALOG_SVID };
    uint32_t serverid;
    uint32_t groupid;    
    uint32_t startTimeStamp;  // send message timeStamp
    uint32_t endTimeStamp;  // send message timeStamp
    uint32_t ip; 
    uint32_t pid;
    //from linkd
    uint32_t linkdLookup;
    uint32_t linkdLookupDrop;
    uint32_t linkdReplyOk;
    uint32_t linkdReplyError;
    uint32_t linkdReplySDBResNoExist;
    uint32_t linkdReplySDBTimeout;
    uint32_t routeToPreTreats;

    //from smnode
    uint32_t smnodeLookupReply;
    uint32_t allocate;
    uint32_t allocateDrop;
    uint32_t allocateReply;    
    //service
    uint32_t smanagerQuery;
    uint32_t smanagerUpdate;
    //sdb
    uint32_t channelInfoReq;
    uint32_t channelInfoReply;
    
    std::vector<uint32_t> stats;

    PSmdispatcherStat()
    : serverid(0)
    , groupid(0) 
    , startTimeStamp(0)
    , endTimeStamp(0)
    , pid()
    , linkdLookup(0)
    , linkdLookupDrop(0)
    , linkdReplyOk(0)
    , linkdReplyError(0)
    , linkdReplySDBResNoExist(0)
    , linkdReplySDBTimeout(0)
    , smnodeLookupReply(0)
    , allocate(0)
    , allocateDrop(0)
    , allocateReply(0)
    , smanagerQuery(0)
    , smanagerUpdate(0)
    , channelInfoReq(0)
    , channelInfoReply(0)
    {}
    
    virtual void marshal(sox::Pack &p) const {
      
        p << serverid<<groupid<<startTimeStamp <<endTimeStamp<<ip<<pid<<linkdLookup
        <<linkdLookupDrop<<linkdReplyOk <<linkdReplyError<<linkdReplySDBResNoExist 
        << linkdReplySDBTimeout<< routeToPreTreats <<smnodeLookupReply<<allocate<<allocateDrop
        <<allocateReply<<smanagerQuery <<smanagerUpdate<<channelInfoReq<<channelInfoReply;
        sox::marshal_container(p, stats);
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> serverid>>groupid>>startTimeStamp>>endTimeStamp>>ip>>pid>>linkdLookup
          >>linkdLookupDrop>>linkdReplyOk>>linkdReplyError>>linkdReplySDBResNoExist
          >>linkdReplySDBTimeout>>routeToPreTreats >>smnodeLookupReply>>allocate>>allocateDrop
          >>allocateReply>>smanagerQuery>>smanagerUpdate>>channelInfoReq>>channelInfoReply;
        sox::unmarshal_container(up, std::back_inserter(stats));        
    }
};


//smnode -> pkgStatic, to report smnode's statistic
struct PSmnodeStat : public sox::Marshallable
{
    enum {uri = (802 << 8) | protocol::CATALOG_SVID };
    uint32_t serverid;
    uint32_t groupid;    
    uint32_t startTimeStamp;  // send message timeStamp
    uint32_t endTimeStamp;  // send message timeStamp
    uint32_t ip; 
    uint32_t pid;
    //from smdispatcher
    uint32_t lookup;
    uint32_t lookupFailed;
    uint32_t allocate;
    uint32_t allocateFailed;
    uint32_t channelUpdate;
    uint32_t channelInject;
    //from session
    uint32_t sessionStart;
    uint32_t sessionSync;
    uint32_t sessionPorxyUpdate;
    //channel maintainence
    uint32_t maintainance;
    uint32_t maintainFailed;
    
    std::vector<uint32_t> stats;

    PSmnodeStat()
    : serverid(0)
    , groupid(0) 
    , startTimeStamp(0)
    , endTimeStamp(0)
    , pid()
    ,lookup(0)
    ,lookupFailed(0)
    ,allocate(0)
    ,allocateFailed(0)
    ,channelUpdate(0)
    ,channelInject(0)
    ,sessionStart(0)
    ,sessionSync(0)
    ,sessionPorxyUpdate(0)
    ,maintainance(0)
    ,maintainFailed(0)
    {}
    
    virtual void marshal(sox::Pack &p) const {
      
        p << serverid<<groupid<<startTimeStamp
          <<endTimeStamp<<ip<<pid<<lookup<<lookupFailed
          <<allocate<<allocateFailed<<channelUpdate<<channelInject
          <<sessionStart<<sessionSync<<sessionPorxyUpdate<<maintainance<<maintainFailed;
        sox::marshal_container(p, stats);
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> serverid>>groupid>>startTimeStamp>>endTimeStamp>>ip>>pid
          >>lookup>>lookupFailed>>allocate>>allocateFailed>>channelUpdate
          >>channelInject>>sessionStart>>sessionSync>>sessionPorxyUpdate
          >>maintainance>>maintainFailed;
        sox::unmarshal_container(up, std::back_inserter(stats));        
    }
};


//LinkD 统计linkd 到smanager的round trip time 报告给 pkgStatic
struct UIDRoundTripTime : public sox::Marshallable
{
    enum { other=0, localLookup = 1, localAllocate =2, globalLookup=3, globalAllocate=4, local2GlobalLookup=5, duplicateRequest=6 };
    uint32_t uid;
    uint32_t sid;   // uid lookup channel id
    uint32_t peerIp;    
    uint32_t type;
    uint32_t time;  // the round trip time (millisecond).

    UIDRoundTripTime()
    : uid(0)
    , sid(0)
    , peerIp(0)    
    , type(0)
    , time(0)
    {}
    
    virtual void marshal(sox::Pack &p) const {
        p <<  uid << sid << peerIp << type << time;
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> uid >> sid >> peerIp >> type >> time;
    }
};

struct PLinkdSmgrRoundtripTime : public sox::Marshallable
{
    enum {uri = (701 << 8) | protocol::CATALOG_SVID };
    uint32_t linkdServerId;
    uint32_t timeStamp;  // send message timeStamp
    uint32_t interval;  //millisecond
    uint32_t linkdIP; 
    std::vector<UIDRoundTripTime> uidRoundTripTime;
    
    PLinkdSmgrRoundtripTime()
    : linkdServerId(0)
    , timeStamp(0)    
    , interval(0)
    , linkdIP(0)
    , uidRoundTripTime()
    {}
    
    virtual void marshal(sox::Pack &p) const {
        p <<  linkdServerId << timeStamp << interval << linkdIP ;
        sox::marshal_container(p, uidRoundTripTime);        
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> linkdServerId >> timeStamp >> interval >> linkdIP ;
        sox::unmarshal_container(up, std::back_inserter(uidRoundTripTime));              
    }
};

struct PLinkdSmgrTimeOut : public sox::Marshallable
{
    enum {uri = (702 << 8) | protocol::CATALOG_SVID };
    uint32_t linkdServerId;
    uint32_t timeStamp;  // send message timeStamp
    uint32_t interval;     //millisecond
    uint32_t linkdIP; 
    std::vector<std::pair<uint32_t, uint32_t> > uidSids; // pair first is uid, pair second is sid

    PLinkdSmgrTimeOut()
    : linkdServerId(0)
    , timeStamp(0) 
    , interval(0)
    , linkdIP(0)
    , uidSids()
    {}
    
    virtual void marshal(sox::Pack &p) const {
        p << linkdServerId << timeStamp  << interval << linkdIP;
        sox::marshal_container(p, uidSids);
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> linkdServerId >> timeStamp >> interval >> linkdIP;
        sox::unmarshal_container(up, std::back_inserter(uidSids));        
    }
};

struct LinkdUIDExceed : public sox::Marshallable
{
    uint32_t uid;
    uint32_t interval;   //second
    uint32_t exceeds;    //exceed the limit conters

    LinkdUIDExceed()
    : uid(0)
    , interval(0) 
    , exceeds(0)
    {}
    
    virtual void marshal(sox::Pack &p) const {
        p << uid << interval << exceeds;
}
    virtual void unmarshal(const sox::Unpack &up) {
        up >> uid >> interval >> exceeds;
    }
};

struct PLinkdMsgUIDExceed : public sox::Marshallable
{
    enum {uri = (703 << 8) | protocol::CATALOG_SVID };
    uint32_t linkdServerId;
    uint32_t msgURI;
    uint32_t timeStamp;  // send message timeStamp
    uint32_t linkdIP; 
    std::vector<LinkdUIDExceed > LinkdUIDExceeds; 

    PLinkdMsgUIDExceed()
    : linkdServerId(0)
    , msgURI(0)
    , timeStamp(0) 
    , linkdIP(0)
    , LinkdUIDExceeds()
    {}
    
    virtual void marshal(sox::Pack &p) const {
        p << linkdServerId << msgURI << timeStamp  << linkdIP;
        sox::marshal_container(p, LinkdUIDExceeds);
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> linkdServerId >> msgURI >> timeStamp >> linkdIP;
        sox::unmarshal_container(up, std::back_inserter(LinkdUIDExceeds));        
    }
};

struct PLinkdSmgrRoundtripTime2 : public sox::Marshallable
{
    enum {uri = (704 << 8) | protocol::CATALOG_SVID };
    uint32_t linkdServerId;
    uint32_t timeStamp;  // send message timeStamp
    uint32_t interval;  //millisecond
    uint32_t linkdIP; 
    bool       sendToLocal;
    std::vector<UIDRoundTripTime> uidRoundTripTime;

    PLinkdSmgrRoundtripTime2()
    : linkdServerId(0)
    , timeStamp(0)    
    , interval(0)
    , linkdIP(0)
    , sendToLocal(false)
    , uidRoundTripTime()
    {}
    
    virtual void marshal(sox::Pack &p) const {
        p <<  linkdServerId << timeStamp << interval << linkdIP << sendToLocal ;
        sox::marshal_container(p, uidRoundTripTime);        
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> linkdServerId >> timeStamp >> interval >> linkdIP >>sendToLocal ;
        sox::unmarshal_container(up, std::back_inserter(uidRoundTripTime));              
    }
};

struct smanagerLookupContext : public sox::Marshallable
{
  uint32_t      uid;
  uint32_t      sid;
  uint32_t      asid;
  uint32_t      sendTimestamp;    
  uint32_t      usedTime;
  uint32_t      lookupStatus;
  uint32_t      smDispatcherIP;

  virtual void marshal(sox::Pack &p) const {
    p <<  uid << sid << asid << sendTimestamp << usedTime << lookupStatus << smDispatcherIP;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >>  uid >> sid >> asid >> sendTimestamp >> usedTime >> lookupStatus >> smDispatcherIP;
  }
};

struct PLinkdSmgrReport : public sox::Marshallable
{
  enum {uri = (705 << 8) | protocol::CATALOG_SVID };
  uint32_t serverId;
  uint32_t timeStamp;  // send message timeStamp
  std::string ip; 
  
  std::vector<smanagerLookupContext> lookupCtx;

  virtual void marshal(sox::Pack &p) const {
    p <<  serverId << timeStamp << ip;
    sox::marshal_container(p, lookupCtx);        
  }

  virtual void unmarshal(const sox::Unpack &up) {
    up >>  serverId >> timeStamp >> ip;
    sox::unmarshal_container(up, std::back_inserter(lookupCtx));              
  }

};

struct smanagerLookupContext2 : public sox::Marshallable
{
  uint32_t      uid;
  uint32_t      sid;
  uint32_t      asid;
  uint32_t      sendTimestamp;    
  uint32_t      usedTime;
  uint32_t      lookupStatus;
  uint32_t      smDispatcherIP;
  uint32_t      packSize;
  uint32_t      res;
  bool          isEncypt;

  virtual void marshal(sox::Pack &p) const {
    p <<  uid << sid << asid << sendTimestamp << usedTime << lookupStatus << smDispatcherIP << packSize << res << isEncypt;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >>  uid >> sid >> asid >> sendTimestamp >> usedTime >> lookupStatus >> smDispatcherIP >> packSize >> res >> isEncypt;
  }
};

struct PLinkdSmgrReportUdp : public sox::Marshallable
{
  enum {uri = (706 << 8) | protocol::CATALOG_SVID };
  uint32_t serverId;
  uint32_t timeStamp;  // send message timeStamp
  std::string ip; 

  std::vector<smanagerLookupContext2> lookupCtx;

  virtual void marshal(sox::Pack &p) const {
    p <<  serverId << timeStamp << ip;
    sox::marshal_container(p, lookupCtx);        
  }

  virtual void unmarshal(const sox::Unpack &up) {
    up >>  serverId >> timeStamp >> ip;
    sox::unmarshal_container(up, std::back_inserter(lookupCtx));
  }

};

struct smanagerLookupContext3 : public sox::Marshallable2
{
  enum RES_TYPE
  {
    USER_NOT_FOUND = 0, //can not find udp user when smanager reply
    ENCYPT_FAIL    = 1, //encypt fail, user TEA algorithm
    ENCYPT_SUCCESS = 2, //encypt success
    NOT_ENCYPT     = 3, //not necessary for tcp
  };

  uint32_t      uid;
  uint32_t      sid;
  uint32_t      asid;
  uint32_t      sendTimestamp;
  uint32_t      usedTime;
  uint32_t      lookupStatus;
  uint32_t      smDispatcherIP;
  uint32_t      packSize;
  uint32_t      res;
  uint32_t      encyptType;
  bool          isTcp;

  virtual void marshal2(sox::Pack &p) const {
    p << uid << sid << asid << sendTimestamp << usedTime << lookupStatus << smDispatcherIP << packSize;
    p << res << encyptType << isTcp;
  }
  virtual void unmarshal2(const sox::Unpack &up) {
    up >> uid >> sid >> asid >> sendTimestamp >> usedTime >> lookupStatus >> smDispatcherIP >> packSize;
    up >> res >> encyptType >> isTcp;
  }
};

struct PLinkdSmgrReportTotal : public sox::Marshallable
{
  enum {uri = (750 << 8) | protocol::CATALOG_SVID };
  uint32_t serverId;
  uint32_t timeStamp;  // send message timeStamp
  std::string ip; 

  std::vector<smanagerLookupContext3> lookupCtx;

  virtual void marshal(sox::Pack &p) const {
    p <<  serverId << timeStamp << ip;
    sox::marshal_container(p, lookupCtx);
  }

  virtual void unmarshal(const sox::Unpack &up) {
    up >>  serverId >> timeStamp >> ip;
    sox::unmarshal_container(up, std::back_inserter(lookupCtx));
  }

};


//statistic to pkgStatic
struct SidSessiongroup : public sox::Marshallable
{
    uint32_t sid;
    uint32_t sessiongroup;
    uint32_t users;    

    SidSessiongroup()
    : sid(0)
    , sessiongroup(0)    
    , users(0)
    {}
    
    virtual void marshal(sox::Pack &p) const {
        p <<  sid << sessiongroup << users;
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> sid >> sessiongroup >> users;
    }
};

struct PDuplicateChannel : public sox::Marshallable
{
    enum {uri = (710 << 8) | protocol::CATALOG_SVID };
    uint32_t timeStamp;  // send message timeStamp
    uint32_t masterChannels;   //频道主本数
    uint32_t duplicateChannels;  //频道副本数
    uint32_t totalChannels;
    std::vector<SidSessiongroup> sidsgs;
    
    PDuplicateChannel()
    : timeStamp(0) 
    , masterChannels(0)
    , duplicateChannels(0)
    , totalChannels(0)
    {}
    
    virtual void marshal(sox::Pack &p) const {
        p << timeStamp  << masterChannels << duplicateChannels << totalChannels;
        sox::marshal_container(p, sidsgs);  
    }
    virtual void unmarshal(const sox::Unpack &up) {
        up >> timeStamp >> masterChannels >> duplicateChannels >> totalChannels;
        sox::unmarshal_container(up, std::back_inserter(sidsgs));      
    }
};

struct PSS_BroadcastMsg : public sox::Marshallable
{
    enum { uri = PSS_BroadcastMsgURI };
	
	uint32_t nBrctType;     // 广播类型。0:紧急,1:公告,2:广告,3:注意,4:警告,5:错误
  std::vector<uint32_t> nChannelTypes;  // 频道类型。0:游戏,1:娱乐,9:指定频道id,-1:所有频道
  std::vector<uint32_t> vecChannels; // 频道列表，仅仅当频道类型为"指定频道"有效
  std::string games; // 游戏列表(以逗号分隔)
  std::string msg;   // 广播正文
        
  PSS_BroadcastMsg()
     :nBrctType(0)
  {}   

  virtual void marshal(sox::Pack &p) const {
    p << nBrctType ;

    sox::marshal_container(p, nChannelTypes);
    sox::marshal_container(p, vecChannels);

    p << games << msg;
  }
  virtual void unmarshal(const sox::Unpack &up) {
    up >> nBrctType;

    sox::unmarshal_container(up, std::back_inserter(nChannelTypes));          
    sox::unmarshal_container(up, std::back_inserter(vecChannels)); 

    up >> games >> msg;
  }

};

struct configSetting : public sox::Marshallable
{
  enum { uri = 0 };    // 不需要配置uri
  std::map<uint32_t, std::string>         dynamicSetting; // 动态加载，预留
  std::map<uint16_t, bool>                options;        // 开关
  std::map<uint16_t, uint32_t>            parameters;     // 参数

  virtual void marshal(sox::Pack &p) const 
  {
    marshal_container(p, dynamicSetting);
    marshal_container(p, options);
    marshal_container(p, parameters);
  }

  virtual void unmarshal(const sox::Unpack &p) 
  {
    unmarshal_container(p, std::inserter(dynamicSetting, dynamicSetting.end()));
    unmarshal_container(p, std::inserter(options, options.end()));
    unmarshal_container(p, std::inserter(parameters, parameters.end()));
  }
};

}

namespace globalBan
{
  // 全局黑名单
  struct PGlobalBan : public sox::Marshallable{
    enum {uri = (253 << 8 | protocol::SESSION_SVID)};

    uint32_t id;
    uint32_t uid;
    uint32_t time_interval; // 封多长时间（以小时为单位）

    PGlobalBan() : id(0) {

    }

    virtual void marshal(sox::Pack &pk) const {
      pk << id << uid << time_interval;
    }
    virtual void unmarshal(const sox::Unpack &up) {
      up >> id >> uid >> time_interval;
    }
  };

  // 全局黑名单(onlined、linkd间)
  struct PGlobalBan2 : public sox::Marshallable{
    enum {uri = (3 << 8 | protocol::ONLINE_SVID)};

    uint32_t id;
    uint32_t uid;
    uint32_t time_interval; // 封多长时间（以小时为单位）
    std::string banDate;
    std::string unBanDate;
    std::string note;

    PGlobalBan2() : id(0) {}

    virtual void marshal(sox::Pack &pk) const {
      pk << id << uid << time_interval << banDate << unBanDate << note;
    }
    virtual void unmarshal(const sox::Unpack &up) {
      up >> id >> uid >> time_interval >> banDate >> unBanDate >> note;
    }
  };

  // linkd将从客户端获得的pcInfo通过这条指令发给onlinedb，并将pcInfo存入数据库。
  struct PRecordPCInfoAfterGlobalBan : public sox::Marshallable{
    enum {uri = (254 << 8 | protocol::SESSION_SVID)};

    uint32_t id;
    std::string pc_info;
    uint32_t ip;      // 被封用户的ip
    virtual void marshal(sox::Pack &pk) const {
      pk << id << pc_info << ip;
    }
    virtual void unmarshal(const sox::Unpack &up) {
      up >> id >> pc_info >> ip;
    }
  };
  
  //kickoff从onlinedb那里拿linkid
  struct PGetLinkid : public sox::Marshallable{
    enum {uri = (111 << 8 | protocol::ONLINE_SVID)};
    
    uint32_t uid;
    uint32_t time_interval;  // 封多长时间（以小时为单位）
    std::string note;
    uint32_t flag;  //0:POnlyKickOff,1:PKickOffAndBanUID,2:PKickOffAndBanUIDPCInfo
    uint32_t ban_type;
    
    PGetLinkid() : ban_type(0) {}

    virtual void marshal(sox::Pack &pk) const {
      pk << uid << time_interval << note << flag << ban_type;
    }
    virtual void unmarshal(const sox::Unpack &up) {
      up >> uid >> time_interval >> note >> flag >> ban_type;
    }
  };

  //仅踢人下线
  struct POnlyKickOff : public sox::Marshallable{
    enum {uri = (1 << 8 | protocol::ONLINE_SVID)};
    
    uint32_t uid;
    std::string note;

    virtual void marshal(sox::Pack &pk) const {
      pk << uid << note;
    }
    virtual void unmarshal(const sox::Unpack &up) {
      up >> uid >> note;
    }
  };
  
  struct POnlyKickOffRes : public sox::Marshallable{
    enum {uri = (11 << 8 | protocol::ONLINE_SVID)};
    uint32_t uid;
    std::string note;
    uint32_t linkid;

    virtual void marshal(sox::Pack &pk) const {
      pk << uid << note << linkid ;
    }
    virtual void unmarshal(const sox::Unpack &up) {
      up >> uid >> note >> linkid ;
    }
  };

  //踢人下线并封UID
  struct PKickOffAndBanUID : public sox::Marshallable{
    enum {uri = (2 << 8 | protocol::ONLINE_SVID)};
    uint32_t uid;
    uint32_t time_interval; // 封多长时间（以小时为单位）
    std::string note;
    uint32_t ban_type;
    
    PKickOffAndBanUID() : ban_type(0) {}  //设置ban_type默认值为0
    
    virtual void marshal(sox::Pack &pk) const {
      pk << uid << time_interval << note << ban_type;
    }
    virtual void unmarshal(const sox::Unpack &up) {
      up >> uid >> time_interval >> note >> ban_type;
    }
  };
  
  //查询和设置登录验证码标志位
  struct PGetAuthCode : public sox::Marshallable{
	  enum {uri = (6 << 8 | protocol::ONLINE_SVID)};
	  uint32_t uid;

	  virtual void marshal(sox::Pack &pk) const {
		  pk << uid;
	  }
	  virtual void unmarshal(const sox::Unpack &up) {
		  up >> uid;
	  }
  };

  struct PGetAuthCodeRes : public sox::Marshallable{
	  enum {uri = (7 << 8 | protocol::ONLINE_SVID)};
	  uint32_t uid;
	  uint32_t stat;

	  virtual void marshal(sox::Pack &pk) const {
		  pk << uid << stat;
	  }
	  virtual void unmarshal(const sox::Unpack &up) {
		  up >> uid >> stat;
	  }
  };

  struct PSetAuthCode : public sox::Marshallable{
	  enum {uri = (8 << 8 | protocol::ONLINE_SVID)};
	  uint32_t uid;
	  uint32_t stat;

	  virtual void marshal(sox::Pack &pk) const {
		  pk << uid << stat;
	  }
	  virtual void unmarshal(const sox::Unpack &up) {
		  up >> uid >> stat;
	  }
  };

  //踢人下线并封UID,不覆盖封
  struct PKickOffAndBanUIDNotCover : public sox::Marshallable{
    enum {uri = (20 << 8 | protocol::ONLINE_SVID)};
    uint32_t uid;
    uint32_t time_interval; // 封多长时间（以小时为单位）
    std::string note;
    uint32_t ban_type;
    
    PKickOffAndBanUIDNotCover() : ban_type(0) {}  //设置ban_type默认值为0
    
    virtual void marshal(sox::Pack &pk) const {
      pk << uid << time_interval << note << ban_type;
    }
    virtual void unmarshal(const sox::Unpack &up) {
      up >> uid >> time_interval >> note >> ban_type;
    }
  };
  
  struct PKickOffAndBanUIDRes : public sox::Marshallable{
    enum {uri = (22 << 8 | protocol::ONLINE_SVID)};
    uint32_t uid;
    uint32_t time_interval; // 封多长时间（以小时为单位）
    std::string note;
    uint32_t linkid;
    uint32_t ban_type;

    PKickOffAndBanUIDRes() : ban_type(0) {}
    
    virtual void marshal(sox::Pack &pk) const {
      pk << uid << time_interval << note << linkid << ban_type;
    }
    virtual void unmarshal(const sox::Unpack &up) {
      up >> uid >> time_interval >> note >> linkid >> ban_type;
    }
  };

  //批量踢人封UID
  struct PKickOffAndBanUIDS : public sox::Marshallable{
    enum {uri = (222 << 8 | protocol::ONLINE_SVID)};
    std::vector<uint32_t> uids;
    uint32_t time_interval; // 封多长时间（以小时为单位）
    std::string note;
    uint32_t ban_type;
    
    PKickOffAndBanUIDS() : ban_type(0) {}  //设置ban_type默认值为0
    
    virtual void marshal(sox::Pack &pk) const {
      sox::marshal_container(pk, uids);
      pk << time_interval << note << ban_type;
    }
    virtual void unmarshal(const sox::Unpack &up) {
      sox::unmarshal_container(up, std::back_inserter(uids));
      up >> time_interval >> note >> ban_type;
    }
  };  

  //踢人下线封UID并封PCInfo
  struct PKickOffAndBanUIDPCInfo : public sox::Marshallable{
    enum {uri = (4 << 8 | protocol::ONLINE_SVID)};
    uint32_t uid;
    uint32_t time_interval; // 封多长时间（以小时为单位）
    std::string note;
    uint32_t ban_type;
    
    PKickOffAndBanUIDPCInfo() : ban_type(0) {}
    
    virtual void marshal(sox::Pack &pk) const {
      pk << uid << time_interval << note << ban_type;
    }
    virtual void unmarshal(const sox::Unpack &up) {
      up >> uid >> time_interval >> note >> ban_type;
    }
  };
  
  struct PKickOffAndBanUIDPCInfoRes : public sox::Marshallable{
    enum {uri = (44 << 8 | protocol::ONLINE_SVID)};
    uint32_t uid;
    uint32_t time_interval; // 封多长时间（以小时为单位）
    std::string note;
    uint32_t linkid;
    uint32_t ban_type;
    
    PKickOffAndBanUIDPCInfoRes() : ban_type(0) {}

    virtual void marshal(sox::Pack &pk) const {
      pk << uid << time_interval << note << linkid << ban_type;
    }
    virtual void unmarshal(const sox::Unpack &up) {
      up >> uid >> time_interval >> note >> linkid >> ban_type;
    }
  };
  
  //批量解封
  struct PUnBanUID : public sox::Marshallable{
    enum {uri = (5 << 8 | protocol::ONLINE_SVID)};
    std::vector<uint32_t> uids;
    virtual void marshal(sox::Pack &pk) const {
      sox::marshal_container(pk, uids);
    }
    virtual void unmarshal(const sox::Unpack &up) {
      sox::unmarshal_container(up, std::back_inserter(uids));
    }
  };

  //查询消息
  struct PCheckUID : public sox::Marshallable{
	enum {uri = (231 << 8 | protocol::ONLINE_SVID)};
    uint32_t uid;
	std::string context; //消息认证文本
	
	PCheckUID()
	: uid(0)
	, context()	
	{}
	
	virtual void marshal(sox::Pack &pk) const {
      pk << uid << context;
    }
    virtual void unmarshal(const sox::Unpack &up) {
      up >> uid >> context;
    }
  };

  struct PCheckUIDRes : public sox::Marshallable{
	enum {uri = (232 << 8 | protocol::ONLINE_SVID)};
	uint32_t uid;
	uint32_t status;  //状态 0:banned, 1:unban, 2:ban time expired, -1:error. 0和2会携带以下封禁信息
	std::string banDate; //封禁时间 "yyyy-MM-dd HH:mm:ss"
	std::string unbanDate; //解封时间 "yyyy-MM-dd HH:mm:ss"
	std::string note; //封禁原因
	std::string pc_info; //封禁机器码
	uint32_t banType; //封禁类型
	std::string context; //消息认证文本
	
	PCheckUIDRes()
	: uid(0)
	, status(-1)
	, banType(0)
	, context()
	{}
		
	virtual void marshal(sox::Pack &pk) const {
      pk << uid << status << banDate << unbanDate << note << pc_info << banType << context;
    }
    virtual void unmarshal(const sox::Unpack &up) {
      up >> uid >> status >> banDate >> unbanDate >> note >> pc_info >> banType >> context;
    }
  };

  struct PUnBanUIDByChangePassword : public sox::Marshallable{
      enum {uri = (233 << 8 | protocol::ONLINE_SVID)};
      std::vector<uint32_t> uids;
      virtual void marshal(sox::Pack &pk) const {
          sox::marshal_container(pk, uids);
      }
      virtual void unmarshal(const sox::Unpack &up) {
          sox::unmarshal_container(up, std::back_inserter(uids));
      }
  };

  // 改密解封请求
  struct PUnBanUIDByChangePassword2 : public sox::Marshallable{
      enum { uri = (234 << 8 | protocol::ONLINE_SVID) };

      string context; // 用于关联请求方的上下文
      std::vector<uint64_t> uids;

      virtual void marshal(sox::Pack &pk) const {
      	  pk << context;
          sox::marshal_container(pk, uids);
      }
      virtual void unmarshal(const sox::Unpack &up) {
          up >> context;
          sox::unmarshal_container(up, std::back_inserter(uids));
      }
  };

  // 改密解封确认
  struct PUnBanUIDByChangePassword2Rsp : public sox::Marshallable{
      enum { uri = (235 << 8 | protocol::ONLINE_SVID) };

      string context; // 用于关联请求方的上下文
      uint32_t unban_count;
      
      virtual void marshal(sox::Pack &pk) const {
      	  pk << context;
      	  pk << unban_count;
      }

      virtual void unmarshal(const sox::Unpack &up) {
          up >> context;
          up >> unban_count;
      }
  }; 
}
}
#endif /*PSESSION_H_*/
