#ifndef PSESSCACHE_H_
#define PSESSCACHE_H_

#include <map>
#include "server_common/protocol/daemon.h"
#include "common/core/ilink.h"
#include "server_common/protocol/psession.h"
#include "common/core/base_svid.h"
#include "server_common/protocol/plogin.h"
#include "core/sox/snox.h"
//#include "server_session/new_cache/SessCacheUtility.h"

using namespace core;
using namespace server::session;
using namespace protocol;
using namespace protocol::session;
using namespace protocol::uinfo;
using namespace server::lbs;
using namespace sox;


namespace server {
    namespace cache {
        /*********************for 2.1********************************************************/
        typedef ZipMarshal<PInnerJoinRes2, 20 << 8 | SESSCACHE_SVID> PStoreCacheChannelInfo2;

        struct PCacheSinfo : public Marshallable {

            enum {
                uri = (20 << 8) | SESSCACHE_SVID
            };
            uint32_t zSize;
            std::string zipData;

            virtual void marshal(sox::Pack & p) const {
                p << zSize;
                p.push_varstr32(zipData.data(), zipData.length());
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> zSize;
                zipData = p.pop_varstr32();
            }
        };

        struct PInnerCacheSinfo : public PCacheSinfo {

            enum {
                uri = (21 << 8) | SESSCACHE_SVID
            };
        };

        struct PCacheRoute : public Marshallable {

            enum {
                uri = (145 << 8) | SESSCACHE_SVID
            };
            uint32_t cid;
            std::string reqData;
            uint64_t serverId;
            uint64_t transId;

            virtual void marshal(sox::Pack & p) const {
                p << cid;
                p.push_varstr32(reqData.data(), reqData.size());
                p << serverId;
                p << transId;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> cid;
                reqData = p.pop_varstr32();
                p >> serverId;
                p >> transId;
            }
        };

        struct PCacheRouteAnswer : public Marshallable {

            enum {
                uri = (146 << 8) | SESSCACHE_SVID
            };
            uint32_t cid;
            uint16_t res;
            std::string retData;
            uint64_t transId;
            uint64_t serverId;

            virtual void marshal(sox::Pack & p) const {
                p << cid;
                p << res;
                p.push_varstr32(retData.data(), retData.size());
                p << transId;
                p << serverId;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> cid;
                p >> res;
                retData = p.pop_varstr32();
                p >> transId;
                p >> serverId;
            }
        };


        struct PRetrieveCacheChannelInfo2 : public PInnerJoin2 {

            enum {
                uri = (30 << 8) | SESSCACHE_SVID
            };
        };

        struct PRetrieveCacheChannelInfoRes0_1 : public PInnerJoinRes2 {

            enum {
                uri = (40 << 8) | SESSCACHE_SVID
            };
            uint32_t res;

            virtual void marshal(sox::Pack & p) const {
                PInnerJoinRes2::marshal(p);
                p << res;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                PInnerJoinRes2::unmarshal(p);
                p >> res;
            }
        };


        typedef ZipMarshal<PRetrieveCacheChannelInfoRes0_1, 50 << 8 | SESSCACHE_SVID> PRetrieveCacheChannelInfoRes2;

        struct PUpdateJiedaiChannel : public PSetJiedaiRes {

            enum {
                uri = (60 << 8) | SESSCACHE_SVID
            };
        };

        struct PRetrieveCacheUserInfo : public PGetSessUserInfos2 {

            enum {
                uri = (135 << 8) | SESSCACHE_SVID
            };
        };

        struct PRetrieveCacheUserInfoRes : public PGetSessUserInfosRes2 {

            enum {
                uri = (138 << 8) | SESSCACHE_SVID
            };
            std::vector<uint32_t> uids;

            virtual void marshal(sox::Pack & p) const {
                PGetSessUserInfosRes2::marshal(p);
                sox::marshal_container(p, uids);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                PGetSessUserInfosRes2::unmarshal(p);
                sox::unmarshal_container(p, std::back_inserter(uids));
            }
        };
		//////////////////////////////////////////////////////////////////////////
		//se ctxid
		struct PRetrieveCacheUserInfoCtx : public PRetrieveCacheUserInfo {
			enum {
				uri = (160 << 8) | SESSCACHE_SVID
			};
			std::string context;
			virtual void marshal(sox::Pack & p) const {
				PRetrieveCacheUserInfo::marshal(p);
				p.push_varstr32(context.data(), context.size());
			}

			virtual void unmarshal(const sox::Unpack & p) {
				PRetrieveCacheUserInfo::unmarshal(p);
				context=p.pop_varstr32();
			}
		};

		struct PRetrieveCacheUserInfoCtxRes : public PRetrieveCacheUserInfoRes {

			enum {
				uri = (161 << 8) | SESSCACHE_SVID
			};
			std::string context;

			virtual void marshal(sox::Pack & p) const {
				PRetrieveCacheUserInfoRes::marshal(p);
				p.push_varstr32(context.data(), context.size());
			}

			virtual void unmarshal(const sox::Unpack & p) {
				PRetrieveCacheUserInfoRes::unmarshal(p);
				context=p.pop_varstr32();
			}
		};
		//////////////////////////////////////////////////////////////////////////
		
        struct PUpdateCacheUserInfo : public PPInfoChanged {

            enum {
                uri = (141 << 8) | SESSCACHE_SVID
            };
        };

        struct PInnerUpdateCacheUserInfo : public PPInfoChanged {

            enum {
                uri = (140 << 8) | SESSCACHE_SVID
            };
        };

        struct PUpdateCacheChanelMember : public sox::Marshallable {

            enum {
                uri = (142 << 8) | SESSCACHE_SVID
            };
            uint32_t tid;
            PInnerMemberUpdated info;

            virtual void marshal(sox::Pack & p) const {
                p << tid;
                info.marshal(p);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> tid;
                info.unmarshal(p);
            }
        };

        struct PUpdateCacheChanelMember2 : public sox::Marshallable {

            enum {
                uri = (143 << 8) | SESSCACHE_SVID
            };
            uint32_t tid;
            PInnerMemberUpdated2 info;

            virtual void marshal(sox::Pack & p) const {
                p << tid;
                info.marshal(p);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> tid;
                info.unmarshal(p);
            }
        };

        struct PInnerUpdateSmember3 : public PUpdateCacheChanelMember2 {
            enum { uri = (144 << 8) | SESSCACHE_SVID };
        };
        
        struct PStoreCacheUserInfo : public PGetSessUserInfosRes2 {

            enum {
                uri = (147 << 8) | SESSCACHE_SVID
            };
        };

        struct PUpdateCacheChanelInfo : public sox::Marshallable {

            enum {
                uri = (170 << 8) | SESSCACHE_SVID
            };
            uint32_t tid;
            PInnerSinfoUpdated info;

            virtual void marshal(sox::Pack & p) const {
                p << tid;
                info.marshal(p);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> tid;
                info.unmarshal(p);
            }
        };

        struct PCreateCacheSubChannel : public sox::Marshallable {

            enum {
                uri = (171 << 8) | SESSCACHE_SVID
            };
            uint32_t tid;
            PInnerChannelCreated info;

            virtual void marshal(sox::Pack & p) const {
                p << tid;
                info.marshal(p);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> tid;
                info.unmarshal(p);
            }
        };

        struct PDismissCacheSubChannel : public sox::Marshallable {

            enum {
                uri = (172 << 8) | SESSCACHE_SVID
            };
            uint32_t sid;
            uint32_t tid;

            virtual void marshal(sox::Pack & p) const {
                p << sid << tid;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> sid >> tid;
            }
        };

        struct PCacheAck : public sox::Marshallable {

            enum {
                uri = (173 << 8) | SESSCACHE_SVID
            };

            enum tables {
                SINFO, UINFO, SMEMBER, SUBSESSIONS
            };
            tables table;
            uint32_t id;

            virtual void marshal(sox::Pack & p) const {
                p.push_uint8(table);
                p << id;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                table = (tables) p.pop_uint8();
                p >> id;
            }
        };

        struct PSyncWebNotify : public sox::Marshallable {

            enum {
                uri = (200 << 8) | SESSCACHE_SVID
            };

            enum tables {
                SINFO, UINFO, SMEMBER, SUBSESSIONS
            };
            tables table;
            uint32_t uid;
            uint32_t sid;
            uint32_t tid; // tid always fill with 0

            virtual void marshal(sox::Pack & p) const {
                p.push_uint32(table);
                p << uid << sid << tid;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                table = (tables) p.pop_uint32();
                p >> uid >> sid >> tid;
            }
        };

        // Notify other sesscache to clear smember from memory

        struct PClearSmemberNotify : public sox::Marshallable {

            enum {
                uri = (201 << 8) | SESSCACHE_SVID
            };
            uint32_t tid;

            virtual void marshal(sox::Pack & p) const {
                p << tid;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> tid;
            }
        };
		
        struct PClearSinfoNotify : public sox::Marshallable {

            enum {
                uri = (203 << 8) | SESSCACHE_SVID
            };
            uint32_t tid;

            virtual void marshal(sox::Pack & p) const {
                p << tid;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> tid;
            }
        };

        //  resever 202 

        struct PWebUpdateSmember : public sox::Marshallable {

            enum {
                uri = (202 << 8) | SESSCACHE_SVID
            };

            uint32_t tid;
            uint32_t uid;
            ChannelRoler role;
            protocol::session::PUpdateChanelMember::RolerOp op;

            virtual void marshal(sox::Pack & p) const {
                p << tid << uid;
                p.push_uint32(role).push_uint32(op);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> tid >> uid;
                role = (ChannelRoler) p.pop_uint32();
                op = (protocol::session::PUpdateChanelMember::RolerOp)p.pop_uint32();
            }
        };
        //  this protocol for restore role send from web

        struct PWebUpdateSmember2 : public sox::Marshallable {

            enum {
                uri = (205 << 8) | SESSCACHE_SVID
            };

            uint32_t tid;
            uint32_t uid;
            ChannelRoler role;
            protocol::session::PUpdateChanelMember::RolerOp op;
            uint32_t jifen;

            virtual void marshal(sox::Pack & p) const {
                p << tid << uid;
                p.push_uint32(role).push_uint32(op);
                p << jifen;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> tid >> uid;
                role = (ChannelRoler) p.pop_uint32();
                op = (protocol::session::PUpdateChanelMember::RolerOp)p.pop_uint32();
                p >> jifen;
            }
        };

        struct PInnerUpdateSmember : public PWebUpdateSmember {

            enum {
                uri = (204 << 8) | SESSCACHE_SVID
            };
        };

        struct PInnerUpdateSmember2 : public PWebUpdateSmember2 {

            enum {
                uri = (206 << 8) | SESSCACHE_SVID
            };
        };

        struct SmemberInfo : public sox::Marshallable {
            uint32_t smemberJifen;
            bool bSettle;
            std::vector<protocol::session::MRolerAndCh> rolers;

            virtual void marshal(sox::Pack & p) const {
                p << smemberJifen << bSettle;
                sox::marshal_container(p, rolers);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> smemberJifen >> bSettle;
                sox::unmarshal_container(p, std::back_inserter(rolers));
            }
        };

        struct SyncSmember : public SmemberInfo {
            uint32_t sid;
            uint32_t uid;

            virtual void marshal(sox::Pack & p) const {
                SmemberInfo::marshal(p);
                p << sid << uid;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                SmemberInfo::unmarshal(p);
                p >> sid >> uid;
            }
        };

		struct PSQSInfoSync : public sox::Marshallable {
			enum { uri = 264 << 8 | SESSCACHE_SVID };
			uint32_t tid;

			uint32_t u32SQId;
			uint64_t u64SeqId;
			uint32_t u32PrxySQId;
			uint32_t u32SubSQCId;

			std::map<std::string, std::string> mapMsg;

			PSQSInfoSync() : tid(0), u32SQId(-1), u64SeqId(-1), u32PrxySQId(-1), u32SubSQCId(-1) {}	

			virtual void marshal(sox::Pack & p) const {
				p << tid << u32SQId << u64SeqId << u32PrxySQId << u32SubSQCId;	

				p.push_uint32(mapMsg.size());
				for (std::map<std::string, std::string>::const_iterator it = mapMsg.begin(); it != mapMsg.end(); ++it)
				{
					std::string strFirst = it->first;
					std::string strSecond = it->second;
					p << strFirst << strSecond;
				}
			}

			virtual void unmarshal(const sox::Unpack & p) {
				p >> tid >> u32SQId >> u64SeqId >> u32PrxySQId >> u32SubSQCId;

				for (uint32_t i = p.pop_uint32(); i > 0; --i)
				{
					std::string strKey;
					std::string strValue;
					p >> strKey >>strValue;
					mapMsg[strKey] = strValue;
				}
			}
		};

		struct PSQSInfoSyncRes : public sox::Marshallable {
			enum { uri = 265 << 8 | SESSCACHE_SVID };

			uint32_t u32SQId;
			uint64_t u64SeqId;
			uint32_t u32PrxySQId;
			uint32_t u32SubSQCId;
			PSQSInfoSyncRes() : u32SQId(-1), u64SeqId(-1), u32PrxySQId(-1), u32SubSQCId(-1) {}

			virtual void marshal(sox::Pack & p) const {
				p << u32SQId << u64SeqId << u32PrxySQId << u32SubSQCId;
			}                  

			virtual void unmarshal(const sox::Unpack & p) {
				p >> u32SQId >> u64SeqId >> u32PrxySQId >> u32SubSQCId;
			}
		};      

        struct PSessCacheSync : public sox::Marshallable {

            enum {
                uri = 266 << 8 | SESSCACHE_SVID
            };

            uint32_t toGroup; // receiver's group id
            uint32_t fromGroup; // sender's group id
            std::vector<uint32_t> sids;
            std::vector<uint32_t> uids;
            std::vector< std::pair<uint32_t, uint32_t> > smembers;

            virtual void marshal(sox::Pack & p) const {
                p << toGroup << fromGroup;
                sox::marshal_container(p, sids);
                sox::marshal_container(p, uids);
                sox::marshal_container(p, smembers);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> toGroup >> fromGroup;
                sox::unmarshal_container(p, std::back_inserter(sids));
                sox::unmarshal_container(p, std::back_inserter(uids));
                sox::unmarshal_container(p, std::back_inserter(smembers));
            }
        };

        struct PSessCacheSyncRes : public sox::Marshallable {

            enum {
                uri = 267 << 8 | SESSCACHE_SVID
            };
            uint32_t groupId;

            virtual void marshal(sox::Pack & p) const {
                p << groupId;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> groupId;
            }
        };

        struct PSessCacheSync2 : public PSessCacheSync {

            enum {
                uri = 268 << 8 | SESSCACHE_SVID
            };

            uint32_t seqNum;
            uint32_t toGroup; // receiver's group id
            uint32_t fromGroup; // sender's group id
            std::vector<uint32_t> sids;
            std::vector<uint32_t> uids;
            std::vector<uint32_t> smembers;

            virtual void marshal(sox::Pack & p) const {
                p << seqNum << toGroup << fromGroup;
                sox::marshal_container(p, sids);
                sox::marshal_container(p, uids);
                sox::marshal_container(p, smembers);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> seqNum >> toGroup >> fromGroup;
                sox::unmarshal_container(p, std::back_inserter(sids));
                sox::unmarshal_container(p, std::back_inserter(uids));
                sox::unmarshal_container(p, std::back_inserter(smembers));
            }
        };

        struct PSessCacheSyncRes2 : public sox::Marshallable {

            enum {
                uri = 269 << 8 | SESSCACHE_SVID
            };
            uint32_t seqNum;
            uint32_t groupId;

            virtual void marshal(sox::Pack & p) const {
                p << seqNum << groupId;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> seqNum >> groupId;
            }
        };

        struct PSessCacheFullSync : public sox::Marshallable {

            enum {
                uri = 270 << 8 | SESSCACHE_SVID
            };
            uint32_t toGroup;
            uint32_t fromGroup;
            std::vector<PSignList> userInfos;
            std::vector<SyncSmember> smemberInfo;
            std::vector<PSetLoginCache> lbsInfos;

            virtual void marshal(sox::Pack & p) const {
                p << toGroup << fromGroup;
                sox::marshal_container(p, userInfos);
                sox::marshal_container(p, smemberInfo);
                sox::marshal_container(p, lbsInfos);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> toGroup >> fromGroup;
                sox::unmarshal_container(p, std::back_inserter(userInfos));
                sox::unmarshal_container(p, std::back_inserter(smemberInfo));
                sox::unmarshal_container(p, std::back_inserter(lbsInfos));
            }
        };

        struct PSessCacheFullSyncRes : public sox::Marshallable {

            enum {
                uri = 271 << 8 | SESSCACHE_SVID
            };
            uint32_t groupId;

            virtual void marshal(sox::Pack & p) const {
                p << groupId;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> groupId;
            }
        };

        struct PAlivePing : public sox::Marshallable {

            enum {
                uri = (289 << 8) | SESSCACHE_SVID
            };

            virtual void marshal(sox::Pack & p) const {
            }

            virtual void unmarshal(const sox::Unpack & p) {
            }
        };

        struct PAlivePing2 : public sox::Marshallable {

            enum {
                uri = (1 << 8) | SESSCACHE_SVID
            };
            std::string x;

            virtual void marshal(sox::Pack & p) const {
                p << x;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> x;
            }
        };

        struct PUpdateJifen : public sox::Marshallable {

            enum {
                uri = (290 << 8) | SESSCACHE_SVID
            };
            std::string uinfoFileName;
            std::string sinfoFileName;
            std::string smemberFileName;

            virtual void marshal(sox::Pack & p) const {
                p << uinfoFileName << sinfoFileName << smemberFileName;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> uinfoFileName >> sinfoFileName >> smemberFileName;
            }
        };

        struct PUpdateJifen2 : public PUpdateJifen {

            enum {
                uri = (293 << 8) | SESSCACHE_SVID
            };
        };

        // TODO: remove
        struct PUpdateCacheSrvStatus : public sox::Marshallable {

            enum {
                uri = (291 << 8) | SESSCACHE_SVID
            };

            virtual void marshal(sox::Pack & p) const {
            }

            virtual void unmarshal(const sox::Unpack & p) {
            }
        };
        // TODO: remove

        struct PUpdateCacheSrvStatus2 : public sox::Marshallable {

            enum {
                uri = (292 << 8) | SESSCACHE_SVID
            };
            uint32_t status;

            virtual void marshal(sox::Pack & p) const {
                p << status;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> status;
            }
        };

        // Message from ??????

        struct PQueryCacheInfo : public sox::Marshallable {

            enum {
                uri = (301 << 8) | SESSCACHE_SVID
            };

            enum tables {
                SINFO, UINFO, SMEMBER
            };
            tables table;
            uint32_t debugLevel;
            uint32_t uid;
            uint32_t sid;

            virtual void marshal(sox::Pack & p) const {
                p.push_uint32(table);
                p << uid << sid << debugLevel;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                table = (tables) p.pop_uint32();
                p >> uid >> sid >> debugLevel;
            }
        };

        struct PQueryCacheInfoRes : public sox::Marshallable {

            enum {
                uri = (302 << 8) | SESSCACHE_SVID
            };
            std::string info;

            virtual void marshal(sox::Pack & p) const {
                p << info;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> info;
            }
        };

        struct PReqStatisticReport : public sox::Marshallable {

            enum {
                uri = (305 << 8) | SESSCACHE_SVID
            };

            virtual void marshal(sox::Pack & p) const {
            }

            virtual void unmarshal(const sox::Unpack & p) {
            }
        };

        struct PReqStatisticReportRes : public sox::Marshallable {

            enum {
                uri = (306 << 8) | SESSCACHE_SVID
            };

            uint64_t time;
            uint32_t groupId;

            uint64_t sinfoBytes;
            uint64_t uinfoBytes;
            uint64_t smemberBytes;

            uint32_t sinfoCurrentItems;
            uint32_t uinfoCurrentItems;
            uint32_t smemberCurrentItems;
            uint32_t sinfoTotalItems;
            uint32_t uinfoTotalItems;
            uint32_t smemberTotalItems;

            uint32_t sinfoRequest;
            uint32_t sinfoHit;
            uint32_t uinfoRequest;
            uint32_t uinfoHit;

            uint32_t sinfoSyncReq;
            uint32_t uinfoSyncReq;
            uint32_t smemberSyncReq;

            uint32_t cmdGet;
            uint32_t cmdSet;
            uint32_t cmdDel;

            virtual void marshal(sox::Pack & p) const {
                p << time << groupId;
                p << sinfoBytes << uinfoBytes << smemberBytes;
                p << sinfoCurrentItems << uinfoCurrentItems << smemberCurrentItems;
                p << sinfoTotalItems << uinfoTotalItems << smemberTotalItems;
                p << sinfoRequest << sinfoHit << uinfoRequest << uinfoHit;
                p << sinfoSyncReq << uinfoSyncReq << smemberSyncReq;
                p << cmdGet << cmdSet << cmdDel;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> time >> groupId;
                p >> sinfoBytes >> uinfoBytes >> smemberBytes;
                p >> sinfoCurrentItems >> uinfoCurrentItems >> smemberCurrentItems;
                p >> sinfoTotalItems >> uinfoTotalItems >> smemberTotalItems;
                p >> sinfoRequest >> sinfoHit >> uinfoRequest >> uinfoHit;
                p >> sinfoSyncReq >> uinfoSyncReq >> smemberSyncReq;
                p >> cmdGet >> cmdSet >> cmdDel;

            }

        };

        struct PUpdateCacheReceptionSid : public sox::Marshallable {

            enum {
                uri = (298 << 8) | SESSCACHE_SVID
            };

            PClientSetJiedai info;
            uint32_t tid;

            virtual void marshal(sox::Pack & p) const {
                p << tid;
                info.marshal(p);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> tid;
                info.unmarshal(p);
            }
        };

        struct syncPackBuffer : public sox::Marshallable {
            uint16_t type;
            uint32_t version;
            uint16_t reserve1;
            uint32_t reserve2;
            std::string key;
            std::string value;

            syncPackBuffer() {
                type = 0;
                version = 0;
                reserve1 = 0;
                reserve2 = 0;
            };

            virtual void marshal(sox::Pack & p) const {
                p << type << version << reserve1 << reserve2 << key;
                p.push_varstr32(value.data(), value.size());
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> type >> version >> reserve1 >> reserve2 >> key;
                value = p.pop_varstr32();
            }
        };

        struct PCacheFullSync : public sox::Marshallable {

            enum {
                uri = (310 << 8) | SESSCACHE_SVID
            };

            uint32_t to; // group Id
            uint32_t from; // group Id
            std::vector<syncPackBuffer> syncQueue;

            virtual void marshal(sox::Pack & p) const {
                p << to << from;
                sox::marshal_container(p, syncQueue);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> to >> from;
                sox::unmarshal_container(p, std::back_inserter(syncQueue));
            }
        };

        struct PCacheFullSyncRes : public sox::Marshallable {

            enum {
                uri = 314 << 8 | SESSCACHE_SVID
            };
            uint32_t groupId;

            virtual void marshal(sox::Pack & p) const {
                p << groupId;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> groupId;
            }
        };

        struct PGetUserRolers : public sox::Marshallable {

            enum {
                uri = 360 << 8 | SESSCACHE_SVID
            };
            uint32_t uid;

            virtual void marshal(sox::Pack & p) const {
                p << uid;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> uid;
            }
        };

        struct MarSMember : public sox::Marshallable
        , public protocol::session::SMember {

            virtual void marshal(sox::Pack & p) const {
                p << tid << sid << uid << (uint16_t) roler << jifen << (uint32_t) addtime;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> tid >> sid >> uid;
                roler = (protocol::session::ChannelRoler)p.pop_uint16();
                p >> jifen;
                addtime = (uint32_t) p.pop_uint32();
            }

            MarSMember() {
            }

            MarSMember(const protocol::session::SMember & sm) {
                SMember &ti = *this;
                ti = sm;
                ti.jifen = sm.getCalcedJifen();
            }
        };

        struct MarSMember2 : public sox::Marshallable
        , public protocol::session::SMember {

            virtual void marshal(sox::Pack & p) const {
                p << tid << sid << uid << (uint16_t) roler << jifen << strAddTime;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> tid >> sid >> uid;
                roler = (protocol::session::ChannelRoler)p.pop_uint16();
                p >> jifen >> strAddTime;
            }

            MarSMember2() {
            }

            MarSMember2(const protocol::session::SMember & sm) {
                SMember &ti = *this;
                ti = sm;
                ti.jifen = sm.getCalcedJifen();
            }
        };

        struct PGetUserRolersRes : public sox::Marshallable {

            enum {
                uri = 362 << 8 | SDB_SVID
            };
            uint32_t uid;
            std::vector<MarSMember> members;

            virtual void marshal(sox::Pack & p) const {
                p << uid;
                sox::marshal_container(p, members);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> uid;
                sox::unmarshal_container(p, std::back_inserter(members));
            }
        };

        struct PGetUserRolersRes2 : public PGetUserRolersRes {

            enum {
                uri = 364 << 8 | SDB_SVID
            };
            uint32_t settleGuildSid;

            virtual void marshal(sox::Pack & p) const {
                p << settleGuildSid;
                PGetUserRolersRes::marshal(p);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> settleGuildSid;
                PGetUserRolersRes::unmarshal(p);
            }
        };

        struct sidMapRoler : public sox::Marshallable {
            uint32_t jifen;
            uint32_t version;
            std::map<uint32_t, uint32_t> rolers; // sid --> roler

            sidMapRoler() {
                jifen = 0;
                version = 0;
            }

            sidMapRoler(const sidMapRoler & role) {
                jifen = role.jifen;
                version = role.version;
                rolers = role.rolers;
            }

            virtual void marshal(sox::Pack & p) const {
                p << jifen << version;
                sox::marshal_container(p, rolers);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> jifen >> version;
                sox::unmarshal_container(p, std::inserter(rolers, rolers.begin()));
            }
        };

        struct PStoreRunTimeChannelInfo : public sox::Marshallable {

            enum {
                uri = (400 << 8) | SESSCACHE_SVID
            };

            uint32_t sid;
            PPushSessData displayUserlist;
            channelqueuemar_map_t maixuQueue;

            std::set<uint32_t> disableTextChs;
            std::set<uint32_t> disableVisitorsTextChs;

            std::set<uint32_t> disableVoiceChs;

            ChannelUserMap disabletext;
            ChannelUserMap disablevoice;

            virtual void marshal(sox::Pack & p) const {
                p << sid;
                displayUserlist.marshal(p);
                sox::marshal_container(p, maixuQueue);

                sox::marshal_container(p, disableTextChs);
                sox::marshal_container(p, disableVisitorsTextChs);
                sox::marshal_container(p, disableVoiceChs);

                disabletext.marshal(p);
                disablevoice.marshal(p);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> sid;
                displayUserlist.unmarshal(p);
                sox::unmarshal_container(p, std::inserter(maixuQueue, maixuQueue.begin()));

                sox::unmarshal_container(p, std::inserter(disableTextChs, disableTextChs.begin()));
                sox::unmarshal_container(p, std::inserter(disableVisitorsTextChs, disableVisitorsTextChs.begin()));
                sox::unmarshal_container(p, std::inserter(disableVoiceChs, disableVoiceChs.begin()));

                disabletext.unmarshal(p);
                disablevoice.unmarshal(p);
            }
        };

        struct PRetrieveRunTimeChannelInfo : public sox::Marshallable {

            enum {
                uri = (405 << 8) | SESSCACHE_SVID
            };

            uint32_t sid;

            virtual void marshal(sox::Pack & p) const {
                p << sid;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> sid;
            }
        };

        struct PRetrieveRunTimeChannelInfoRes : public PStoreRunTimeChannelInfo {

            enum {
                uri = (410 << 8) | SESSCACHE_SVID
            };
        };

        struct PAddChannelOW : public sox::Marshallable {

            enum {
                uri = (450 << 8) | SESSCACHE_SVID
            };

            uint32_t uid;
            uint32_t sid;

            virtual void marshal(sox::Pack & p) const {
                p << uid << sid;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> uid >> sid;
            }
        };

        struct StoreJifen : public sox::Marshallable {
            uint32_t id;
            uint32_t jifen;

            StoreJifen() {
                id = 0;
                jifen = 0;
            }

            virtual void marshal(sox::Pack & p) const {
                p << id << jifen;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> id >> jifen;
            }
        };

        struct PGetChannelRolers : public sox::Marshallable {

            enum {
                uri = (500 << 8) | SESSCACHE_SVID
            };

            uint32_t sid;

            virtual void marshal(sox::Pack & p) const {
                p << sid;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> sid;
            }
        };

        struct PGetChannelRolers2 : public sox::Marshallable {

            enum {
                uri = (501 << 8) | SESSCACHE_SVID
            };

            uint32_t sid;

            virtual void marshal(sox::Pack & p) const {
                p << sid;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> sid;
            }
        };

        struct PGetChannelRolers3 : public sox::Marshallable {

            enum {
                uri = (502 << 8) | SESSCACHE_SVID
            };

            uint32_t sid;

            virtual void marshal(sox::Pack & p) const {
                p << sid;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> sid;
            }
        };

        struct PGetChannelRolersRes2 : public sox::Marshallable {

            enum {
                uri = (506 << 8) | SESSCACHE_SVID
            };

            uint32_t sid;
            std::vector<MarSMember> chnMembers;
            std::vector<uint32_t> settles;

            virtual void marshal(sox::Pack & p) const {
                p << sid;
                sox::marshal_container(p, chnMembers);
                sox::marshal_container(p, settles);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> sid;
                sox::unmarshal_container(p, std::back_inserter(chnMembers));
                sox::unmarshal_container(p, std::back_inserter(settles));
            }
        };

        //TODO: remove

        struct PGetChannelRolersRes3 : public sox::Marshallable {

            enum {
                uri = (507 << 8) | SESSCACHE_SVID
            };

            uint32_t sid;
            std::vector<MarSMember2> chnMembers;
            std::vector<uint32_t> settles;

            virtual void marshal(sox::Pack & p) const {
                p << sid;
                sox::marshal_container(p, chnMembers);
                sox::marshal_container(p, settles);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> sid;
                sox::unmarshal_container(p, std::back_inserter(chnMembers));
                sox::unmarshal_container(p, std::back_inserter(settles));
            }
        };

        struct RolerRecord : public sox::Marshallable {
            ChannelRoler role;
            //std::string                         addTime;
            uint32_t addTime;

            RolerRecord() {
                addTime = 0;
            }

            virtual void marshal(sox::Pack & p) const {
                p.push_uint16(role);
                p << addTime;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                role = (ChannelRoler) p.pop_uint16();
                p >> addTime;
            }
        };

        struct SidMapUserRoler : public sox::Marshallable {
            uint32_t jifen; // smember jifen
            uint32_t lastModify;
            std::map<uint32_t, RolerRecord> rolers; // sid --> role

            SidMapUserRoler() {
                jifen = 0;
                lastModify = 0;
            }

            virtual void marshal(sox::Pack & p) const {
                p << jifen << lastModify;
                sox::marshal_container(p, rolers);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> jifen >> lastModify;
                sox::unmarshal_container(p, std::inserter(rolers, rolers.begin()));
            }
        };

        struct PStoreChnRolers : public sox::Marshallable {

            enum {
                uri = (510 << 8) | SESSCACHE_SVID
            };
            uint32_t sid;
            uint32_t version;
            std::map<uint32_t, SidMapUserRoler> chnMemberRolers; // uid -- > user's role
            std::set<uint32_t> settleMembers; // uid set

            virtual void marshal(sox::Pack & p) const {
                p << sid << version;
                sox::marshal_container(p, chnMemberRolers);
                sox::marshal_container(p, settleMembers);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> sid >> version;
                sox::unmarshal_container(p, std::inserter(chnMemberRolers, chnMemberRolers.begin()));
                sox::unmarshal_container(p, std::inserter(settleMembers, settleMembers.begin()));
            }
        };

        struct PChangeSettleGuild : public sox::Marshallable {

            enum {
                uri = (530 << 8) | SESSCACHE_SVID
            };

            uint32_t uid;
            uint32_t oldSid;
            uint32_t newSid;

            virtual void marshal(sox::Pack & p) const {
                p << uid << oldSid << newSid;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> uid >> oldSid >> newSid;
            }
        };

        struct PInnerChangeSettleGuild : public PChangeSettleGuild {

            enum {
                uri = (535 << 8) | SESSCACHE_SVID
            };
        };

        struct PLeaveGuild3 : public sox::Marshallable {

            enum {
                uri = (540 << 8) | SESSCACHE_SVID
            };

            uint32_t uid;
            uint32_t sid;

            virtual void marshal(sox::Pack & p) const {
                p << uid << sid;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> uid >> sid;
            }
        };

        struct PInnerLeaveGuild : public PLeaveGuild3 {

            enum {
                uri = (545 << 8) | SESSCACHE_SVID
            };
        };

        /***************************************************************************************************/
        struct PGetBriefChannelRolers : public Marshallable {

            enum {
                uri = (2000 << 8) | SESSCACHE_SVID
            };
            uint32_t sid;

            virtual void marshal(sox::Pack & p) const {
                p << sid;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> sid;
            }
        };

        struct PGetBriefChannelRolersRes : public Marshallable {

            enum {
                uri = (2001 << 8) | SESSCACHE_SVID
            };
            uint32_t sid;
            uint32_t resCode;
            std::vector< std::pair<uint32_t, uint32_t> > infos;

            virtual void marshal(sox::Pack & p) const {
                p << sid << resCode;
                sox::marshal_container(p, infos);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> sid >> resCode;
                sox::unmarshal_container(p, std::back_inserter(infos));
            }
        };

        struct PGetChannelUsers : public Marshallable {

            enum {
                uri = (2010 << 8) | SESSCACHE_SVID
            };
            uint32_t sid;
            std::vector<uint32_t> uids;

            virtual void marshal(sox::Pack & p) const {
                p << sid;
                sox::marshal_container(p, uids);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> sid;
                sox::unmarshal_container(p, back_inserter(uids));
            }

        };

        struct UserGuildInfo : public Marshallable {
            uint32_t uid;
            uint8_t sex;
            uint32_t uinfoJifen;
            uint32_t smemberJifen;
            uint32_t addTime;
            uint32_t role;
            std::string nick;

            virtual void marshal(sox::Pack & p) const {
                p << uid;
                p.push_uint8(sex);
                p << uinfoJifen << smemberJifen << addTime << role << nick;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> uid;
                sex = p.pop_uint8();
                p >> uinfoJifen >> smemberJifen >> addTime >> role >> nick;
            }
        };

        struct PGetChannelUsersRes : public Marshallable {

            enum {
                uri = (2011 << 8) | SESSCACHE_SVID
            };
            uint32_t sid;
            uint32_t resCode;
            std::vector<UserGuildInfo> infos;

            virtual void marshal(sox::Pack & p) const {
                p << sid << resCode;
                sox::marshal_container(p, infos);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> sid >> resCode;
                sox::unmarshal_container(p, back_inserter(infos));
            }
        };

        struct PGetChannelRolerPage : public Marshallable {

            enum {
                uri = (2020 << 8) | SESSCACHE_SVID
            };
            uint32_t sid;
            uint32_t role;
            uint32_t pageNum;
            uint32_t pageSize;

            virtual void marshal(sox::Pack & p) const {
                p << sid << role << pageNum << pageSize;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> sid >> role >> pageNum >> pageSize;
            }

        };

        struct PGetChannelRolerPageRes : public Marshallable {

            enum {
                uri = (2021 << 8) | SESSCACHE_SVID
            };
            uint32_t sid;
            uint32_t resCode;
            uint32_t pageNum;
            uint32_t totalRecords;
            std::vector<UserGuildInfo> infos;

            virtual void marshal(sox::Pack & p) const {
                p << sid << resCode << pageNum << totalRecords;
                sox::marshal_container(p, infos);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> sid >> resCode >> pageNum >> totalRecords;
                sox::unmarshal_container(p, std::back_inserter(infos));
            }
        };

        struct PCheckChnRolerStatus : public Marshallable {

            enum {
                uri = (2030 << 8) | SESSCACHE_SVID
            };
            uint32_t sid;
            uint32_t type;

            virtual void marshal(sox::Pack & p) const {
                p << sid << type;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> sid >> type;
            }
        };

        struct PCheckChnRolerStatusRes : public Marshallable {

            enum {
                uri = (2031 << 8) | SESSCACHE_SVID
            };
            uint32_t sid;
            uint32_t type;
            uint32_t resCode;

            virtual void marshal(sox::Pack & p) const {
                p << sid << type << resCode;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> sid >> type >> resCode;
            }
        };
        /***************************************************************************************************/

        /****************************************************************************************************************************/

        struct memberRolers : public sox::Marshallable {
            uint32_t uid;
            uint32_t jifen;
            uint32_t addTime;
            std::vector<protocol::session::MRolerAndCh> rolers;

            memberRolers() {
                jifen = 0;
                addTime = sox::env::now;
            }

            virtual void marshal(sox::Pack & p) const {
                p << uid << jifen << addTime;
                sox::marshal_container(p, rolers);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> uid >> jifen >> addTime;
                sox::unmarshal_container(p, std::back_inserter(rolers));
            }
        };

        struct PStoreChnRolers2 : public sox::Marshallable {

            enum {
                uri = (3100 << 8) | SESSCACHE_SVID
            };
            uint32_t sid;
            uint32_t version;
            std::vector<memberRolers> chnRolers;
            std::vector<uint32_t> settleMembers; // uid set

            virtual void marshal(sox::Pack & p) const {
                p << sid << version;
                sox::marshal_container(p, chnRolers);
                sox::marshal_container(p, settleMembers);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                //timeMeasure t;
                p >> sid >> version;
                //sox::unmarshal_container(p, std::back_inserter(chnRolers));
                //sox::unmarshal_container(p, std::inserter(settleMembers, settleMembers.begin()));

                uint32_t count = p.pop_uint32();
                chnRolers.reserve(count);
                for (; count > 0; --count) {
                    memberRolers tmp;
                    p >> tmp;
                    chnRolers.push_back(tmp);
                }

                count = p.pop_uint32();
                settleMembers.reserve(count);
                for (; count > 0; --count) {
                    uint32_t tmp;
                    p >> tmp;
                    settleMembers.push_back(tmp);
                }

                //log(Info, "unmarshal used time = %lu", t.getTime());
            }
        };


        /****************************************************************************************************************************/

        /*********************************************internal performancetest******************************************************/
        struct PStoreJifen : public sox::Marshallable {

            enum {
                uri = (5000 << 8) | SESSCACHE_SVID
            };
            uint32_t id;
            uint32_t jifen;

            virtual void marshal(sox::Pack & p) const {
                p << id << jifen;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> id >> jifen;
            }
        };

        struct PCheckData : public sox::Marshallable {

            enum {
                uri = (5010 << 8) | SESSCACHE_SVID
            };
            uint32_t range;

            virtual void marshal(sox::Pack & p) const {
                p << range;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> range;
            }
        };

        struct PStoreText : public PStoreJifen {

            enum {
                uri = (5100 << 8) | SESSCACHE_SVID
            };
        };

        struct PCheckText : public PCheckData {

            enum {
                uri = (5110 << 8) | SESSCACHE_SVID
            };
        };

        struct PTriggerEvent : public sox::Marshallable {

            enum {
                uri = (5210 << 8) | SESSCACHE_SVID
            };
            uint32_t id;

            virtual void marshal(sox::Pack & p) const {
                p << id;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> id;
            }
        };

        /****************************************************************************************************************************/
        struct PGetChannelIDs : public Marshallable {

            enum {
                uri = (3008 << 8) | SESSCACHE_SVID
            };
            uint32_t count;

            virtual void marshal(sox::Pack & p) const {
                p << count;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> count;
            }

        };

        struct PGetChannelIDsRes : public Marshallable {

            enum {
                uri = (3009 << 8) | SESSCACHE_SVID
            };
            std::vector<uint32_t> sids;

            virtual void marshal(sox::Pack & p) const {
                sox::marshal_container(p, sids);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                sox::unmarshal_container(p, std::inserter(sids, sids.begin()));
            }

        };

        struct PGetChannelAllUserRolers : public Marshallable {

            enum {
                uri = (3010 << 8) | SESSCACHE_SVID
            };
            uint32_t sid;

            virtual void marshal(sox::Pack & p) const {
                p << sid;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> sid;
            }

        };

        struct PGetChannelAllUserRolersRes : public Marshallable {

            enum {
                uri = (3011 << 8) | SESSCACHE_SVID
            };
            uint32_t sid;
            //std::map<uint32_t, uint32_t> urolers; //uid->roler
            std::map<uint32_t, SidMapUserRoler> chnMemberRolers;

            virtual void marshal(sox::Pack & p) const {
                p << sid;
                //sox::marshal_container(p, urolers);
                sox::marshal_container(p, chnMemberRolers);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> sid;
                //sox::unmarshal_container(p, std::inserter(urolers, urolers.begin()));
                sox::unmarshal_container(p, std::inserter(chnMemberRolers, chnMemberRolers.begin()));
            }
        };

        struct checkMemRoler : public sox::Marshallable {
            uint32_t uid;
            uint32_t sid;
            uint32_t roler;

            bool operator>(const checkMemRoler & roler) const {
                if (uid < roler.uid) {
                    return true;
                } else if (uid == roler.uid) {
                    return sid < roler.sid;
                } else {
                    return false;
                }
            }

            virtual void marshal(sox::Pack & p) const {
                p << uid << sid << roler;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> uid >> sid >> roler;
            }
        };

        struct PCheckChnAllRolers : public sox::Marshallable {

            enum {
                uri = 3014 << 8 | SESSCACHE_SVID
            };

            uint32_t sid;
            std::vector<checkMemRoler> rolers;

            virtual void marshal(sox::Pack & p) const {
                p << sid;
                sox::marshal_container(p, rolers);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> sid;
                sox::unmarshal_container(p, std::back_inserter(rolers));
            }
        };

        struct PCheckChnAllRolersRes : public sox::Marshallable {

            enum {
                uri = 3015 << 8 | SESSCACHE_SVID
            };

            uint32_t sid;

            virtual void marshal(sox::Pack & p) const {
                p << sid;
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> sid;
            }
        };

        struct PRepairChnRolers : public sox::Marshallable {

            enum {
                uri = (508 << 8) | SESSCACHE_SVID
            };

            uint32_t sid;
            std::vector<MarSMember> chnMembers;
            std::vector<uint32_t> settles;

            virtual void marshal(sox::Pack & p) const {
                p << sid;
                sox::marshal_container(p, chnMembers);
                sox::marshal_container(p, settles);
            }

            virtual void unmarshal(const sox::Unpack & p) {
                p >> sid;
                sox::unmarshal_container(p, std::back_inserter(chnMembers));
                sox::unmarshal_container(p, std::back_inserter(settles));
            }
        };

		struct PGetChannelRolersRangeReq : public sox::Marshallable
		{
			enum {uri=(600 << 8) | SESSCACHE_SVID};
			std::string    context;
			uint32_t  ses_id;
			uint32_t  begin_uid;
			uint32_t  end_uid;
			uint32_t  request_len;
			virtual void marshal(sox::Pack &p) const
			{
				p.push_varstr32(context.data(), context.size());
				p << ses_id;
				p << begin_uid;
				p << end_uid;
				p << request_len;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				context=p.pop_varstr32();
                                p >> ses_id;
				p >> begin_uid;
				p >> end_uid;
				p >> request_len;
            }
		};
		struct UserRolerFull : public sox::Marshallable
		{
			uint32_t                            user_id;
			uint64_t                            jifen;     
			uint32_t                            addtime;
			uint32_t                            roler;
			std::map<uint32_t, RolerRecord>     sub_rolers;     // subses_id --> role    
			virtual void marshal(sox::Pack &p) const
			{
				p << user_id;
				p << jifen;
				p << addtime;
				p << roler;
				sox::marshal_container(p, sub_rolers);
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
                p >> user_id;
				p >> jifen;
				p >> addtime;
				p >> roler;
				sox::unmarshal_container(p, std::inserter(sub_rolers, sub_rolers.begin()));
            }
		};
		struct PGetChannelRolersRangeRes : public sox::Marshallable
		{
			enum {uri=(606 << 8) | SESSCACHE_SVID};
			std::string   context;
			uint8_t           return_value;
			uint32_t           next_uid;  ///0 means no more
			uint32_t                        ses_id;
			std::vector<UserRolerFull>      chnMembers; 
			std::vector<uint32_t>           settles; 
			virtual void marshal(sox::Pack &p) const
			{
				p.push_varstr32(context.data(), context.size());
				p << return_value;
				p << next_uid;
				p << ses_id;
				sox::marshal_container(p, chnMembers);
				sox::marshal_container(p, settles);
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				context=p.pop_varstr32();
				p >> return_value;
		                p >> next_uid;
				p >> ses_id;
				sox::unmarshal_container(p, std::back_inserter(chnMembers));
				sox::unmarshal_container(p, std::back_inserter(settles));
            		}
		};

		//临时协议, sc向ssc查询用户的个人信息, 频道角色
		struct PGetSessMemberInfoViaSSC: public sox::Marshallable
		{
			enum {uri=(607 << 8) | SESSCACHE_SVID};
			uint32_t org_serverid;
			uint32_t org_cid;
			uint32_t tid;
			std::vector<uint32_t> uids;

			virtual void marshal(sox::Pack &p) const 
			{
				p << org_serverid << org_cid << tid << uids;
			}
			
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> org_serverid >> org_cid >> tid >> uids;
			}
		};

		struct PGetSessMemberInfoViaSSCRes: public sox::Marshallable
	    	{
			typedef std::vector<protocol::session::MRolerAndCh> RolerList;
			typedef std::map<uint32_t, RolerList> RolersMap;

			enum {uri=(608 << 8) | SESSCACHE_SVID};
			uint32_t org_serverid;
			uint32_t org_cid;
			uint32_t tid;
			std::vector<uint32_t> uids;	//users no role
			RolersMap rolers;	//users has role

			virtual void marshal(sox::Pack &p) const 
			{
				p << org_serverid << org_cid << tid << uids;
				p.push_uint32(rolers.size());
				for (RolersMap::const_iterator it = rolers.begin(); it != rolers.end(); ++it) 
				{
					p << it->first << it->second;
				}
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> org_serverid >> org_cid >> tid  >> uids;
				uint32_t l = p.pop_uint32();
				for (uint32_t i = 0; i < l; ++i) {
					uint32_t k;
					RolerList v;
					p >> k >> v;
					rolers.insert(std::pair<uint32_t, RolerList>(k, v));
				}
			}
	    	};
		struct  PRetrieveCacheUserInfoViaHSC: public PRetrieveCacheUserInfo {
			enum {
				uri = (609 << 8) | SESSCACHE_SVID
			};
			uint32_t org_serverid;
			uint32_t org_cid;
			virtual void marshal(sox::Pack &p) const 
			{
				PRetrieveCacheUserInfo::marshal(p);
				p << org_serverid << org_cid;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PRetrieveCacheUserInfo::unmarshal(p);
				p >> org_serverid >> org_cid;
			}
		};
		struct  PRetrieveCacheUserInfoViaHSCRes: public PRetrieveCacheUserInfoRes {
			enum {
				uri = (610 << 8) | SESSCACHE_SVID
			};
			uint32_t org_serverid;
			uint32_t org_cid;
			virtual void marshal(sox::Pack &p) const 
			{
				PRetrieveCacheUserInfoRes::marshal(p);
				p << org_serverid << org_cid;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				PRetrieveCacheUserInfoRes::unmarshal(p);
				p >> org_serverid >> org_cid;
			}
		};
		//mon stat
		struct  PRetrieveCacheUserInfoViaHSC3: public PRetrieveCacheUserInfoViaHSC{
			enum {
				uri = (611 << 8) | SESSCACHE_SVID
			};
			uint32_t org_serverid;
			uint32_t org_cid;
			uint64_t start_time;
			virtual void marshal(sox::Pack &p) const 
			{
				PRetrieveCacheUserInfoViaHSC::marshal(p);
				p << org_serverid << org_cid << start_time;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PRetrieveCacheUserInfoViaHSC::unmarshal(p);
				p >> org_serverid >> org_cid >> start_time;
			}
		};
		struct  PRetrieveCacheUserInfoViaHSCRes3: public PRetrieveCacheUserInfoViaHSCRes {
			enum {
				uri = (612 << 8) | SESSCACHE_SVID
			};

			PRetrieveCacheUserInfoViaHSCRes3(){
				cnt_err_sys = cnt_err_busi = 0;
			}

			uint32_t org_serverid;
			uint32_t org_cid;
			uint64_t start_time;//fill back
			uint32_t cnt_err_sys;
			uint32_t cnt_err_busi;
			
			virtual void marshal(sox::Pack &p) const 
			{
				PRetrieveCacheUserInfoViaHSCRes::marshal(p);
				p << org_serverid << org_cid << start_time << cnt_err_sys << cnt_err_busi;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				PRetrieveCacheUserInfoViaHSCRes::unmarshal(p);
				p >> org_serverid >> org_cid >> start_time >> cnt_err_sys >> cnt_err_busi;
			}
		};
		//ctxid
		struct PRetrieveCacheUserInfoViaHSC3Ctx : public PRetrieveCacheUserInfoViaHSC3 {
			enum {
				uri = (162 << 8) | SESSCACHE_SVID
			};
			std::string context;
			virtual void marshal(sox::Pack & p) const {
				PRetrieveCacheUserInfoViaHSC3::marshal(p);
				p.push_varstr32(context.data(), context.size());
			}

			virtual void unmarshal(const sox::Unpack & p) {
				PRetrieveCacheUserInfoViaHSC3::unmarshal(p);
				context=p.pop_varstr32();
			}
		};
		struct PRetrieveCacheUserInfoViaHSCRes3Ctx : public PRetrieveCacheUserInfoViaHSCRes3 {

			enum {
				uri = (163 << 8) | SESSCACHE_SVID
			};
			std::string context;

			virtual void marshal(sox::Pack & p) const {
				PRetrieveCacheUserInfoViaHSCRes3::marshal(p);
				p.push_varstr32(context.data(), context.size());
			}

			virtual void unmarshal(const sox::Unpack & p) {
				PRetrieveCacheUserInfoViaHSCRes3::unmarshal(p);
				context=p.pop_varstr32();
			}
		};
		//~
		//~
		struct PSS_SqcChanelInfoNotify : public sox::Marshallable
		{
			enum {uri = ( 347 << 8 | protocol::SERVICE_SVID )};
			//enum {uri = PSS_SqcChanelInfoNotify_uri};
			uint32_t m_uSid;//顶级频道
			std::string m_strData;//这个是保留字段，暂时不填
			virtual void marshal(Pack &p) const 
			{
				p<<m_uSid<<m_strData;
			}
			virtual void unmarshal(const Unpack &p) 

			{
				p>>m_uSid>>m_strData;
			}
		};


		//redis project, add by yongjian.lian
		enum {
			PSS_GetChinfoLikeSessionCache_uri=( 348 << 8 | protocol::SERVICE_SVID ),
			PSS_GetChinfoLikeSessionCacheRes_uri=( 349 << 8 | protocol::SERVICE_SVID ),
			PSS_GetChinfoLikeSessionCacheZipRes_uri=( 351 << 8 | protocol::SERVICE_SVID ),
			PSS_GetChinfoOnlyMapAndList_uri=( 361 << 8 | protocol::SERVICE_SVID ),
			PSS_GetChinfoOnlyMapAndListRes_uri=( 352 << 8 | protocol::SERVICE_SVID ),
			PSS_GetChinfoOnlyMapAndListZipRes_uri=( 353 << 8 | protocol::SERVICE_SVID ),
		};

		struct PSS_GetChinfoLikeSessionCache : public sox::Marshallable
	    	{
		    enum {uri = PSS_GetChinfoLikeSessionCache_uri};
		    uint32_t m_uSid;
		    std::string m_strContext;
		    virtual void marshal(Pack &p) const 
		    {
			    p<<m_uSid<<m_strContext;
		    }
		    virtual void unmarshal(const Unpack &p)
		    {
			    p>>m_uSid>>m_strContext;
		    }
		};

		struct PSS_GetChinfoLikeSessionCacheRes : public PInnerJoinRes2
	    	{
			enum {uri = PSS_GetChinfoLikeSessionCacheRes_uri};
			uint32_t res;
			std::string m_strContext;
			PSS_GetChinfoLikeSessionCacheRes()
			{

			}
			PSS_GetChinfoLikeSessionCacheRes(PSS_GetChinfoLikeSessionCache *pObj)
			{
				sid=pObj->m_uSid;
				m_strContext=pObj->m_strContext;
			}
			virtual void marshal(sox::Pack & p) const
			{
				PInnerJoinRes2::marshal(p);
				p << res<<m_strContext;
			}
			virtual void unmarshal(const sox::Unpack & p)
			{
				PInnerJoinRes2::unmarshal(p);
				p >> res>>m_strContext;
			}
		};

		typedef ZipMarshal<PSS_GetChinfoLikeSessionCacheRes, PSS_GetChinfoLikeSessionCacheZipRes_uri> PSS_GetChinfoLikeSessionCacheZipRes;

		struct PSS_GetChinfoOnlyMapAndList : public sox::Marshallable
	    	{

		    enum {uri = PSS_GetChinfoOnlyMapAndList_uri};
		    uint32_t m_uSid;
		    std::string m_strContext;
		    virtual void marshal(Pack &p) const 
		    {
			    p<<m_uSid<<m_strContext;
		    }
		    virtual void unmarshal(const Unpack &p) 
		    {
			    p>>m_uSid>>m_strContext;
		    }
		};

		struct PSS_GetChinfoOnlyMapAndListRes : public sox::Marshallable 
	    	{
		    enum {uri = PSS_GetChinfoOnlyMapAndListRes_uri};
		    uint32_t res;
		    std::string m_strContext;
		    uint32_t sid;
		    protocol::session::SidMapProperties sprops;
		    std::vector<uint32_t> subs;
		    PSS_GetChinfoOnlyMapAndListRes()
		    {

		    }
		    PSS_GetChinfoOnlyMapAndListRes(PSS_GetChinfoOnlyMapAndList *pObj)
		    {
			    sid=pObj->m_uSid;
			    m_strContext=pObj->m_strContext;
		    }

		    virtual void marshal(sox::Pack & p) const 
		    {

			    p << res<<m_strContext;
			    p << sid << sprops;
			    sox::marshal_container(p, subs);
		    }

		    virtual void unmarshal(const sox::Unpack & p) 
		    {

			    p >> res>>m_strContext;
			    p >> sid >> sprops;
			    sox::unmarshal_container(p, std::back_inserter(subs));
		    }
		};

		typedef ZipMarshal<PSS_GetChinfoOnlyMapAndListRes, PSS_GetChinfoOnlyMapAndListZipRes_uri> PSS_GetChinfoOnlyMapAndListZipRes;


	}

}

#endif /*PSESSCACHE_H_*/


