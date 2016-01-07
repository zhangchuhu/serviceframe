#ifndef SERVER_ROUTER_H_
#define SERVER_ROUTER_H_

#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/res_code.h"
#include "common/protocol/prouter.h"
#include <vector>
#include "core/corelib/RouterBase.h"
#include "common/core/sender.h"

namespace core{
	struct IPacketGc;
}

namespace server{
	namespace router{
		struct PAnswer: public core::IPacketBase{
			core::Sender ss;

			virtual int emit(core::IConn *conn, core::IServerIdDispatcher* );
			virtual void gc(core::IPacketGc *);
		};

		struct PRouter: public core::RouterBase{
			enum {uri = 1 << 8 | protocol::ROUTE_SVID};
			std::string to;

			virtual void marshal(sox::Pack &pk) const{
				core::RouterBase::marshal(pk);
				pk << to;
			}
			virtual void unmarshal(const sox::Unpack &up){
				core::RouterBase::unmarshal(up);
				up >> to;
			}

			virtual int emit(core::IConn *conn, core::IServerIdDispatcher* );
			virtual void gc(core::IPacketGc *);
		};

		struct PICMP: public PRouter{
			enum {uri = 2 << 8 | protocol::ROUTE_SVID};
			enum ICMPType{
				not_in_mine,
				not_online
			} type;

		};

		// add by Grj
		struct PToServersRoute: public core::RouterBase
		{
			enum {uri = 136 << 8 | protocol::ROUTE_SVID};
			std::string to;
	
			virtual void marshal(sox::Pack &pk) const{
				core::RouterBase::marshal(pk);
				pk << to;
			}
			virtual void unmarshal(const sox::Unpack &up){
				core::RouterBase::unmarshal(up);
				up >> to;
			}

			virtual int emit(core::IConn *conn, core::IServerIdDispatcher* );
			virtual void gc(core::IPacketGc *);
		};

		// 轮询方式发包
		struct PRoundRobinRouter : public core::IPacketBase
		{
			enum {uri = 146 << 8 | protocol::ROUTE_SVID};
			
			PRouter  m_router;

			virtual void marshal(sox::Pack &pk) const
			{
				m_router.marshal(pk);
			}
			virtual void unmarshal(const sox::Unpack &up)
			{
				m_router.unmarshal(up);
			}

			virtual int emit(core::IConn *conn, core::IServerIdDispatcher* );
			virtual void gc(core::IPacketGc *);
		};

		// end

		// add by long, support proxy
        struct CProxyInfo: public sox::Marshallable
        {
            std::vector<uint32_t> m_vecProxyId;
            std::vector<uint32_t> m_vecKey;

            virtual void marshal(sox::Pack &p) const
            {
                marshal_container(p, m_vecProxyId);
                marshal_container(p, m_vecKey);
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                unmarshal_container(p, std::back_inserter(m_vecProxyId));
                unmarshal_container(p, std::back_inserter(m_vecKey));
            }
        };

		struct PProxyRouter: public core::RouterBase
		{
			enum {uri = (137 << 8) | protocol::ROUTE_SVID};
            uint32_t m_uServerId;
            CProxyInfo m_objProxyInfo;

			virtual void marshal(sox::Pack &p) const 
			{
                core::RouterBase::marshal(p);
                p << m_uServerId << m_objProxyInfo;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
                core::RouterBase::unmarshal(p);
                p >> m_uServerId >> m_objProxyInfo;
			}
            
            virtual int emit(core::IConn *conn, core::IServerIdDispatcher* );
            virtual void gc(core::IPacketGc *);

		};
		// end

		struct PServerIdRouter: public core::RouterBase{
			enum {uri = 130 << 8 | protocol::ROUTE_SVID};
			uint32_t serverId;

			virtual void marshal(sox::Pack &pk) const{
				core::RouterBase::marshal(pk);
				pk << serverId;
			}
			virtual void unmarshal(const sox::Unpack &up){
				core::RouterBase::unmarshal(up);
				up >> serverId;
			}

			virtual int emit(core::IConn *conn, core::IServerIdDispatcher* );

			virtual void gc(core::IPacketGc *);
		};

		struct PUidsRouter: public core::RouterBase{
			enum {uri = 133 << 8 | protocol::ROUTE_SVID};
			std::vector<uint32_t> uids;
			virtual void marshal(sox::Pack &pk) const{
				core::RouterBase::marshal(pk);
				sox::marshal_container(pk, uids);
			}
			virtual void unmarshal(const sox::Unpack &up){
				core::RouterBase::unmarshal(up);
				sox::unmarshal_container(up, std::back_inserter(uids));
			}

			virtual int emit(core::IConn *conn, core::IServerIdDispatcher* );

			virtual void gc(core::IPacketGc *);
		};

		struct PUidTarget: public core::RouterBase{
			enum {uri = 134 << 8 | protocol::ROUTE_SVID};
			uint32_t uid;
			virtual void marshal(sox::Pack &pk) const{
				core::RouterBase::marshal(pk);
				pk << uid;
			}
			virtual void unmarshal(const sox::Unpack &up){
				core::RouterBase::unmarshal(up);
				up >> uid;
			}

			virtual int emit(core::IConn */*conn*/, core::IServerIdDispatcher* ){ return 0;}

			virtual void gc(core::IPacketGc *){}
		};

		/* new to uid packet, add uRouter field */
		struct PUidTarget2: public core::RouterBase{
			enum {uri = 129 << 8 | protocol::ROUTE_SVID};
			uint32_t uid;
			uint32_t uRouter; //0 for tcp 1 not use 2 for udp
			virtual void marshal(sox::Pack &pk) const{
				core::RouterBase::marshal(pk);
				pk << uid << uRouter;
			}
			virtual void unmarshal(const sox::Unpack &up){
				core::RouterBase::unmarshal(up);
				up >> uid >> uRouter;
			}

			virtual int emit(core::IConn* /*conn*/, core::IServerIdDispatcher* ){ return 0;}

			virtual void gc(core::IPacketGc *){}
		};

		struct PBrachRoute: public sox::Marshallable{
			enum {uri = 135 << 8 | protocol::ROUTE_SVID};
			std::string load;
			virtual void marshal(sox::Pack &pk) const{
				pk << load;
			}
			virtual void unmarshal(const sox::Unpack &up){
				up >> load;
			}
		};

		struct PUidListTarget: public core::RouterBase{
			enum {uri = 136 << 8 | protocol::ROUTE_SVID};
			std::vector<uint32_t> vecUidList;
			virtual void marshal(sox::Pack &pk) const{
				core::RouterBase::marshal(pk);
				sox::marshal_container(pk, vecUidList);
			}
			virtual void unmarshal(const sox::Unpack &up){
				core::RouterBase::unmarshal(up);				
				sox::unmarshal_container(up, std::back_inserter(vecUidList));			
			}
			virtual int emit(core::IConn */*conn*/, core::IServerIdDispatcher* ){ return 0;}

			virtual void gc(core::IPacketGc *){}
		};

        enum ENUM_RELIABLE_TYPE
        {
            ENUM_RELIABLE_TYPE_DATA = 1,
            ENUM_RELIABLE_TYPE_RESET = 2,
            ENUM_RELIABLE_TYPE_PING = 3,
			ENUM_RELIABLE_TYPE_DATA_MULTI_ROUTE = 4,
			ENUM_RELIABLE_TYPE_SHUTDOWN = 5
        };

        struct PSS_SyncRouter: public core::RouterBase
        {
            enum {uri = (138 << 8) | protocol::ROUTE_SVID};
            ENUM_RELIABLE_TYPE m_enType;
            uint32_t m_uSrcId;
            uint32_t m_uDstId;
            uint32_t m_uSeqId;

            uint64_t m_ullSendTime;
            uint32_t m_uBridgeId;

            PSS_SyncRouter()
                : m_enType(ENUM_RELIABLE_TYPE_DATA), m_ullSendTime(0), m_uBridgeId(0)
            {
            }

            PSS_SyncRouter(ENUM_RELIABLE_TYPE enType)
                : m_enType(enType), m_ullSendTime(0), m_uBridgeId(0)
            {
            }

            PSS_SyncRouter(ENUM_RELIABLE_TYPE enType, uint32_t uUri, const sox::Marshallable &objMsg)
                : m_enType(enType), m_ullSendTime(0), m_uBridgeId(0)
            {
                ruri = uUri;
                packLoad(objMsg);
            }

            virtual void marshal(sox::Pack &p) const
            {
                core::RouterBase::marshal(p);
                p.push_uint8((uint8_t) m_enType);
                p << m_uSrcId << m_uDstId << m_uSeqId << m_ullSendTime << m_uBridgeId;
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                core::RouterBase::unmarshal(p);
                m_enType = (ENUM_RELIABLE_TYPE) p.pop_uint8();
                p >> m_uSrcId >> m_uDstId >> m_uSeqId;
				if (p.size() >= 8)
				{
					p >> m_ullSendTime;

					if (p.size() >= 4)
					{
						p >> m_uBridgeId;
					}
				}
            }

            virtual int emit(core::IConn *conn, core::IServerIdDispatcher *);

            virtual void gc(core::IPacketGc *pg);
        };

        struct PSS_AckRouter: public core::IPacketBase, sox::Marshallable
        {
            enum {uri = (139 << 8) | protocol::ROUTE_SVID};
            ENUM_RELIABLE_TYPE m_enType;
            uint32_t m_uSrcId;
            uint32_t m_uDstId;
            uint32_t m_uSeqId;
            uint32_t m_uExpectId;
            core::Sender m_objSender;
            uint32_t m_uBridgeId;

            PSS_AckRouter()
            	: m_enType(ENUM_RELIABLE_TYPE_DATA), m_uExpectId(0)
            {
            }

            PSS_AckRouter(PSS_SyncRouter *pSync)
                : m_enType(pSync->m_enType), m_uSrcId(pSync->m_uDstId), m_uDstId(pSync->m_uSrcId), m_uSeqId(pSync->m_uSeqId), m_uExpectId(0)
            {
            }

            virtual void marshal(sox::Pack &p) const
            {
                p.push_uint8((uint8_t) m_enType);
                p << m_uSrcId << m_uDstId << m_uSeqId << m_uExpectId;
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                m_enType = (ENUM_RELIABLE_TYPE) p.pop_uint8();
                p >> m_uSrcId >> m_uDstId >> m_uSeqId;
                if (p.size() > 0)
                {
					p >> m_uExpectId;
                }
            }

            virtual int emit(core::IConn *conn, core::IServerIdDispatcher *);

            virtual void gc(core::IPacketGc *pg);
        };
        
        struct PSS_PingRouter: public sox::Voidmable
        {
            enum {uri = (140 << 8) | protocol::ROUTE_SVID};
        };

        struct PSS_PingRouterRes: public sox::Voidmable
        {
            enum {uri = (141 << 8) | protocol::ROUTE_SVID};
        };

		struct PThriftPackge: public sox::Marshallable{
			enum{uri = 50 << 8 | protocol::ROUTE_SVID};

			std::string key;
			uint32_t ctxId;
			std::string thriftCall;

			virtual void marshal(sox::Pack &pk) const{
				pk << key << ctxId;
				pk.push_varstr32(thriftCall.data(), thriftCall.size());
			}
			virtual void unmarshal(const sox::Unpack &up){
				up >> key >> ctxId;
				thriftCall = up.pop_varstr32();
			}
		};

		struct PThriftPackgeRes: public PThriftPackge{
			enum{uri = 51 << 8 | protocol::ROUTE_SVID};
		};

        // add by tanshaoxi for dbd_proxy
        struct PRouterLayer: public core::RouterBase{
            enum {uri = 150 << 8 | protocol::ROUTE_SVID};
            std::string to;
            std::string routeContext;

            virtual void marshal(sox::Pack &pk) const{
                core::RouterBase::marshal(pk);
                pk << to;
                pk << routeContext;
            }
            virtual void unmarshal(const sox::Unpack &up){
                core::RouterBase::unmarshal(up);
                up >> to;
                up >> routeContext;
            }

            virtual int emit(core::IConn */*conn*/, core::IServerIdDispatcher* ){return 0;}
            virtual void gc(core::IPacketGc *){}

            void fillPack(const std::string& from_i, 
                                  const std::string& to_i, 
                                  const std::string& routeContext_i, 
                                  uint16_t resCode_i, 
                                  uint32_t uri_i, 
                                  const sox::Marshallable & obj_i)
            {
                from = from_i;
                to = to_i;
                routeContext = routeContext_i;
                ruri = uri_i;
                ttl = 1;
                resCode = resCode_i;
                packLoad(obj_i);
            }
        };

        struct StringMarshalable : public sox::Marshallable {
            std::string load;

            StringMarshalable() {}
            StringMarshalable(const std::string& p):load(p) {}

            virtual void marshal(sox::Pack &pk) const{
                pk.push(load.data(),load.size());
            }
            virtual void unmarshal(const sox::Unpack &/*up*/){
                assert(false);
            }
        };
		struct PProxyDaoRequest: public sox::Marshallable{
			enum{uri = 151 << 8 | protocol::ROUTE_SVID};
			std::string context;
			std::string caller_name;
			std::string authorize_token;
			std::string route_option;
			uint32_t priority_hint;
			std::string trace_debug;
			uint32_t coding;
			std::string upper_layer;

			virtual void marshal(sox::Pack &pk) const{
				pk <<context <<caller_name <<authorize_token <<route_option
					<< priority_hint <<trace_debug <<coding;
				pk.push_varstr32(upper_layer.data(), upper_layer.size());
			}
			virtual void unmarshal(const sox::Unpack &up){
				up >>context >>caller_name >>authorize_token >>route_option
					>> priority_hint >>trace_debug >>coding;
				upper_layer=up.pop_varstr32();
			}
		};
		struct PProxyDaoResponse: public sox::Marshallable{
			enum{uri = 152 << 8 | protocol::ROUTE_SVID};
			std::string context;
			uint32_t result_code;
			std::string trace_debug;
			uint32_t coding;
			std::string upper_layer;

			uint32_t current_free;
			virtual void marshal(sox::Pack &pk) const{
				pk <<context <<result_code <<trace_debug <<coding;
				pk.push_varstr32(upper_layer.data(), upper_layer.size());
				pk <<current_free;
			}
			virtual void unmarshal(const sox::Unpack &up){
				up >>context >>result_code >>trace_debug >>coding;
				upper_layer=up.pop_varstr32();
				up >>current_free;
			}
		};
		struct PProxyDaoPing: public sox::Marshallable{
			enum{uri = 153 << 8 | protocol::ROUTE_SVID};
			uint32_t type;
			std::string extra_info;

			virtual void marshal(sox::Pack &pk) const{
				pk <<type <<extra_info;
			}
			virtual void unmarshal(const sox::Unpack &up){
				up >>type >>extra_info;
			}
		};
		struct DAOLOAD: public sox::Marshallable{
			std::string dao_name;
			uint32_t load;
			uint32_t free;
			uint32_t db_type;////1:master db  2:slave db
			std::string extra_info;
			DAOLOAD(){}
			~DAOLOAD(){}
			DAOLOAD(const DAOLOAD& o)
			{
				dao_name=o.dao_name;
				load=o.load;
				free=o.free;
				db_type = o.db_type;
				extra_info=o.extra_info;
			}
			DAOLOAD& operator = (const DAOLOAD& o)
			{
				dao_name=o.dao_name;
				load=o.load;
				free=o.free;
				db_type = o.db_type;
				extra_info=o.extra_info;
				return *this;
			}
			virtual void marshal(sox::Pack &pk) const{
				pk <<dao_name <<load <<free <<db_type<<extra_info;
			}
			virtual void unmarshal(const sox::Unpack &up){
				up >>dao_name >>load >>free >>db_type>>extra_info;
			}
		};
		struct PProxyDaoPingResp: public sox::Marshallable{
			enum{uri = 154 << 8 | protocol::ROUTE_SVID};
			std::string me;
			uint64_t timestamp;
			std::vector<DAOLOAD> daoinfos;
			std::string extra_info;

			virtual void marshal(sox::Pack &pk) const{
				pk <<me <<timestamp;
				uint32_t len=daoinfos.size();
				pk << len;
				for(uint32_t i=0;i<len;++i)
				{
					daoinfos[i].marshal(pk);
				}
				pk <<extra_info;
			}
			virtual void unmarshal(const sox::Unpack &up){
				up >>me >>timestamp;
				uint32_t len=0;
				up >> len;
				daoinfos.resize(len);
				for(uint32_t i=0;i<len;++i)
				{
					daoinfos[i].unmarshal(up);
				}
				up >>extra_info;
			}
		};

		struct PRouterCTXReq: public core::RouterBase{
			enum {uri = 155 << 8 | protocol::ROUTE_SVID};
			std::string context;
			std::string caller_name;
			std::string route_option;
			std::string trace_debug;
			std::string to;

			virtual void marshal(sox::Pack &pk) const{
				pk.push_varstr32(context.data(), context.size());
				pk.push_varstr32(caller_name.data(), caller_name.size());
				pk.push_varstr32(route_option.data(), route_option.size());
				pk.push_varstr32(trace_debug.data(), trace_debug.size());
				core::RouterBase::marshal(pk);
				pk << to;
			}
			virtual void unmarshal(const sox::Unpack &up){
				context=up.pop_varstr32();
				caller_name=up.pop_varstr32();
				route_option=up.pop_varstr32();
				trace_debug=up.pop_varstr32();
				core::RouterBase::unmarshal(up);
				up >> to;
			}

			virtual int emit(core::IConn* /*conn*/, core::IServerIdDispatcher* ){return 0;}
			virtual void gc(core::IPacketGc *){}
		};
		struct PRouterCTXResp: public core::RouterBase{
			enum {uri = 156 << 8 | protocol::ROUTE_SVID};
			std::string context;
			uint32_t result_code;
			std::string trace_debug;
			std::string to;

			virtual void marshal(sox::Pack &pk) const{
				pk.push_varstr32(context.data(), context.size());
				pk << result_code;
				pk.push_varstr32(trace_debug.data(), trace_debug.size());
				core::RouterBase::marshal(pk);
				pk << to;
			}
			virtual void unmarshal(const sox::Unpack &up){
				context=up.pop_varstr32();
				up >> result_code;
				trace_debug=up.pop_varstr32();
				core::RouterBase::unmarshal(up);
				up >> to;
			}

			virtual int emit(core::IConn* /*conn*/, core::IServerIdDispatcher* ){return 0;}
			virtual void gc(core::IPacketGc *){}
		};

		struct PSimplePing: public sox::Marshallable{
			enum{uri = 200 << 8 | protocol::ROUTE_SVID};
			uint64_t sn;
			uint64_t destserverid;
			uint64_t timestamp;
			uint32_t srcserverid;

			virtual void marshal(sox::Pack &pk) const{
				pk << sn << destserverid << timestamp << srcserverid;
			}
			virtual void unmarshal(const sox::Unpack &up){
				up >> sn >> destserverid >> timestamp >> srcserverid;
			}
		};
		struct PSimplePingAck: public sox::Marshallable{
			enum{uri = 201 << 8 | protocol::ROUTE_SVID};
			uint64_t sn;
			uint64_t destserverid;
			uint64_t timestamp;
			virtual void marshal(sox::Pack &pk) const{
				pk << sn << destserverid << timestamp;
			}
			virtual void unmarshal(const sox::Unpack &up){
				up >> sn >> destserverid >> timestamp;
			}
		};

		struct PSRouterStatus: public sox::Marshallable{
			enum{uri = 202 << 8 | protocol::ROUTE_SVID};
			uint64_t sn;
			uint64_t srcserverid;
			uint64_t destserverid;
			uint64_t rtt;
			uint64_t timestamp;
			virtual void marshal(sox::Pack &pk) const{
				pk << sn << srcserverid << destserverid << rtt << timestamp;
			}
			virtual void unmarshal(const sox::Unpack &up){
				up >> sn >> srcserverid >> destserverid >> rtt >> timestamp;
			}
		};
		struct PSRouterStatusAck: public sox::Marshallable{
			enum{uri = 203 << 8 | protocol::ROUTE_SVID};
			uint64_t sn;
			uint32_t rescode;
			virtual void marshal(sox::Pack &pk) const{
				pk << sn << rescode;
			}
			virtual void unmarshal(const sox::Unpack &up){
				up >> sn >> rescode;
			}
		};

		struct PSRouterAppData: public sox::Marshallable{
			enum{uri = 204 << 8 | protocol::ROUTE_SVID};

			uint32_t togroupId;
            uint32_t toserverId;
			uint32_t fromserverId;
			uint32_t fromgroupId;
			uint32_t timestamp;
			uint32_t uritype;
			std::string loadData; 
			

			virtual void marshal(sox::Pack &pk) const{
				pk << togroupId << toserverId << fromserverId<<fromgroupId<<timestamp<<uritype;
				pk.push_varstr32(loadData.data(), loadData.size());
				
			}
			virtual void unmarshal(const sox::Unpack &up){
				up >> togroupId >> toserverId >> fromserverId >>fromgroupId>>timestamp>>uritype;
				loadData = up.pop_varstr32();
			}
		};

		struct PSRouterSendMessageImmediately: public sox::Marshallable{
			enum{uri = 205 << 8 | protocol::ROUTE_SVID};

			uint64_t togroupId;
            uint64_t toServerId;
			uint64_t fromRouterId;
			std::vector<uint64_t> routerid;
			PSRouterAppData appdata;

			virtual void marshal(sox::Pack &pk) const{
				pk << togroupId << toServerId << fromRouterId;
				marshal_container(pk, routerid);
				appdata.marshal(pk);
			}
			virtual void unmarshal(const sox::Unpack &up){
				up >> togroupId >> toServerId >> fromRouterId;
				unmarshal_container(up, std::back_inserter(routerid));
				appdata.unmarshal(up);
			}
		};
		struct PSRouterPathInfo: public sox::Marshallable{
			enum{uri = 206 << 8 | protocol::ROUTE_SVID};

			uint64_t destSRouterId;
			uint64_t rtt;	
			uint64_t totalPackageSize; //包总流量
			uint32_t totalPackageCount;//包个数
			uint32_t totalPingCount;
			uint32_t totalPingAckCount;
			
			virtual void marshal(sox::Pack &pk) const{
				pk << destSRouterId << rtt << totalPackageSize << totalPackageCount << totalPingCount << totalPingAckCount;
			}
			virtual void unmarshal(const sox::Unpack &up){
				up >> destSRouterId >> rtt >> totalPackageSize >> totalPackageCount >> totalPingCount >> totalPingAckCount;
			}
		};

		//任意两SRouter之间的直连路径实时更新
		struct PSRouterPathReport : public sox::Marshallable
		{
			enum{uri = 207 << 8 | protocol::ROUTE_SVID};

			uint64_t sn;
			uint64_t srcSRouterId;
			uint64_t timestamp;
			std::vector<PSRouterPathInfo> path_vec;
			virtual void marshal(sox::Pack &pk) const{
				pk << sn << srcSRouterId << timestamp;
				marshal_container(pk, path_vec);
			}
			virtual void unmarshal(const sox::Unpack &up){
				up >> sn >> srcSRouterId >> timestamp;
				unmarshal_container(up, std::back_inserter(path_vec));
			}		
		};
		struct PSRouterGroupRouterList : public sox::Marshallable 
		{
			enum{uri = 208 << 8 | protocol::ROUTE_SVID};

			uint64_t sn;
			uint64_t groupId;
			uint64_t timestamp;
			std::vector<uint64_t> router_vec;
			virtual void marshal(sox::Pack &pk) const{
				pk << sn << groupId << timestamp;
				marshal_container(pk, router_vec);
			}
			virtual void unmarshal(const sox::Unpack &up){
				up >> sn >> groupId >> timestamp;
				unmarshal_container(up, std::back_inserter(router_vec));
				}
			};
		struct PSRouterPathInfoVec: public sox::Marshallable 
		{
			enum{uri = 209 << 8 | protocol::ROUTE_SVID};
			uint64_t path_id;
			uint64_t rtt;	
			std::vector<uint32_t> path_vec;
			
			virtual void marshal(sox::Pack &pk) const{
				pk << path_id << rtt;
				marshal_container(pk, path_vec);
			}
			virtual void unmarshal(const sox::Unpack &up){
				up >> path_id >> rtt;
				unmarshal_container(up, std::back_inserter(path_vec));
			}
		  
		};
		struct PSRouter2SRouterPathVec : public sox::Marshallable
		{
			enum{uri = 210 << 8 | protocol::ROUTE_SVID};

			uint32_t destSRouterId;
			uint32_t selectedPathId;
			std::vector<PSRouterPathInfoVec> path_vecs;
			
			virtual void marshal(sox::Pack &pk) const{
				pk <<  destSRouterId << selectedPathId ;
				marshal_container(pk, path_vecs);
			}
			virtual void unmarshal(const sox::Unpack &up){
				up >>  destSRouterId >> selectedPathId ;
				unmarshal_container(up, std::back_inserter(path_vecs));
			}
		};
		//实时路径计算报告
		struct PSRouter2SRouterListPathVec : public sox::Marshallable
		{
			enum{uri = 211 << 8 | protocol::ROUTE_SVID};

			uint32_t srcSRouterId;
			uint64_t reportTimeStamp;
			std::vector<PSRouter2SRouterPathVec> path_vecs;
			
			virtual void marshal(sox::Pack &pk) const{
				pk << srcSRouterId << reportTimeStamp;
				marshal_container(pk, path_vecs);
			}
			virtual void unmarshal(const sox::Unpack &up){
				up >> srcSRouterId >>  reportTimeStamp;
				unmarshal_container(up, std::back_inserter(path_vecs));
			}
		};
		//SRouter数据包发送记录
		struct PSRouterSendMessageStatus : public sox::Marshallable
		{
			enum{uri = 212 << 8 | protocol::ROUTE_SVID};

			uint64_t sn;
			uint32_t packagesize;
			uint64_t sendtimestamp;
			uint32_t srcServerId;
			uint32_t destServerId;
			uint32_t reportSRouterId;
			uint32_t pathId;
			uint32_t sendtype;   //发送方式:1:重新选举路径 2:直接发送目的地 3:使用缓冲路径
			std::vector<PSRouterPathInfoVec> path_vecs;
			
			virtual void marshal(sox::Pack &pk) const{
				pk << sn << packagesize << sendtimestamp << srcServerId << destServerId << reportSRouterId << pathId << sendtype;
				marshal_container(pk, path_vecs);
			}
			virtual void unmarshal(const sox::Unpack &up){
				up >> sn >> packagesize >> sendtimestamp >> srcServerId >> destServerId >> reportSRouterId >> pathId >> sendtype;
				unmarshal_container(up, std::back_inserter(path_vecs));
			}
		};
		struct PSRouterSendMessageStatusList : public sox::Marshallable
		{
			enum{uri = 213 << 8 | protocol::ROUTE_SVID};

			uint64_t sn;
			uint64_t sendtimestamp;
			std::vector<PSRouterSendMessageStatus> sendmsg_record_vecs;
			
			virtual void marshal(sox::Pack &pk) const{
				pk << sn << sendtimestamp;
				marshal_container(pk, sendmsg_record_vecs);
			}
			virtual void unmarshal(const sox::Unpack &up){
				up >> sn >> sendtimestamp;
				unmarshal_container(up, std::back_inserter(sendmsg_record_vecs));
			}
		};

		//应用层测试数据包发送统计
		struct PSRouterAppDataStatus: public sox::Marshallable
		{
			enum{uri = 214 << 8 | protocol::ROUTE_SVID};
			uint64_t sn;
			uint32_t fromGroupId;
			uint32_t fromServerId;
			uint32_t toGroupId;
			uint32_t toServerId;
			uint32_t selectedSRouterId;
			uint64_t sendTimeStamp;
			uint64_t recvTimeStamp;
			uint64_t packageSize;
			virtual void marshal(sox::Pack &pk) const{
				pk << sn << fromGroupId << fromServerId << toGroupId << toServerId << selectedSRouterId << sendTimeStamp << recvTimeStamp << packageSize ;
			}
			virtual void unmarshal(const sox::Unpack &up){
				up >> sn >> fromGroupId >> fromServerId >> toGroupId >> toServerId >> selectedSRouterId >> sendTimeStamp >> recvTimeStamp >> packageSize ;
			}	
		};
		//单个SRouter的流量统计
		struct PSRouterFlowStatus : public sox::Marshallable
		{
			enum{uri = 215 << 8 | protocol::ROUTE_SVID};
			uint32_t SRouterID;
			uint32_t SendPackageCount;
			uint32_t SendPackageSize;
			uint32_t RelayPackageCount;
			uint32_t RelayPackageSize;
			uint64_t LastUpdateTime;
			virtual void marshal(sox::Pack &pk) const{
				pk << SRouterID << SendPackageCount << SendPackageSize << RelayPackageCount << RelayPackageSize << LastUpdateTime ;
			}
			virtual void unmarshal(const sox::Unpack &up){
				up >> SRouterID >> SendPackageCount >> SendPackageSize >> RelayPackageCount >> RelayPackageSize >> LastUpdateTime ;
			}	
		};

		//模拟丢包，在SRouterID丢弃来自DSRouterID的ping包
		//
		struct PSRouterDiscardPingMsg : public sox::Marshallable
		{
			enum{uri = 216 << 8 | protocol::ROUTE_SVID};
			uint32_t SRouterID;
			uint32_t DSRouterID;
			uint32_t HandleType; // 0 -- 禁止 1 -- 允许
			virtual void marshal(sox::Pack &pk) const{
				pk << SRouterID << DSRouterID << HandleType ;
			}
			virtual void unmarshal(const sox::Unpack &up){
				up >> SRouterID >> DSRouterID >> HandleType;
			}	
		};
		struct PSRouterAppTestData : public sox::Marshallable
		{
			enum{uri = 217 << 8 | protocol::ROUTE_SVID};
			uint32_t testid;
			uint32_t timestamp;
			std::string testData;
			virtual void marshal(sox::Pack &pk) const{
				pk << testid << timestamp;
				pk.push_varstr32(testData.data(), testData.size());
			}
			virtual void unmarshal(const sox::Unpack &up){
				up >> testid >> timestamp;
				testData = up.pop_varstr32();
			}	
		};
		
    }
}
#endif

