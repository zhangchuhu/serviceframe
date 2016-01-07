#ifndef _PONLINEDB_H_
#define _PONLINEDB_H_

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"

#include <vector>

namespace server{
	namespace onlinedb{

		struct PCheckRegisterFrom : public sox::Marshallable{
			enum {uri = (23 << 8)|protocol::ONLINEDB_SVID};
			uint32_t uid;
			std::string passport;
			std::string passwd;

			virtual void marshal(sox::Pack &pk) const {
				pk << uid << passport << passwd;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> uid >> passport >> passwd;
			}
		};

		struct PCheckRegisterFromRes : public sox::Marshallable{
			enum {uri = (24 << 8)|protocol::ONLINEDB_SVID};
			uint32_t uid;
			uint32_t registerFrom;

			virtual void marshal(sox::Pack &pk) const {
				pk << uid << registerFrom;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> uid >> registerFrom;
			}
		};

		struct PGetRegisterFrom : public sox::Voidmable{
			enum {uri = (25 << 8)|protocol::ONLINEDB_SVID};
		};

		struct POpenIDSetRegisterFrom : public sox::Marshallable{
			enum {uri = (27 << 8)|protocol::ONLINEDB_SVID};
			uint32_t uid;
			uint32_t appid;

			virtual void marshal(sox::Pack &pk) const {
				pk << uid << appid;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> uid >> appid;
			}
		};

		struct RegisterInfo: public sox::Marshallable{
			RegisterInfo(){}
			RegisterInfo(uint32_t code, const std::string& from, uint32_t u):reg_code(code), reg_from(from), users(u){}
			uint32_t reg_code;
			std::string reg_from;
			uint32_t users;

			virtual void marshal(sox::Pack &p) const{
				p << reg_code << reg_from << users;
			}

			virtual void unmarshal(const sox::Unpack &p){
				p >> reg_code >> reg_from >> users;
			}
		};

		struct PGetRegisterFromRes : public sox::Marshallable{
			enum {uri = (26 << 8)|protocol::ONLINEDB_SVID};

			uint32_t linkd_id;
			std::vector<RegisterInfo> registerInfos;

			virtual void marshal(sox::Pack &p) const {
				p << linkd_id;
				sox::marshal_container(p, registerInfos);
			}

			virtual void unmarshal(const sox::Unpack &p){
				p >> linkd_id;
				sox::unmarshal_container(p, std::inserter(registerInfos, registerInfos.begin()));
			}
		};

		struct POnlinedbSelfNotification: public sox::Marshallable {
			enum { uri = (50 << 8) | protocol::ONLINEDB_SVID};

			uint32_t flag;
			std::vector<uint32_t> linkdids;

			virtual void marshal(sox::Pack &p) const{
				p << flag;
				sox::marshal_container(p, linkdids);
			}
			virtual void unmarshal(const sox::Unpack &p){
				p >> flag;
				sox::unmarshal_container(p, std::back_inserter(linkdids));
			}
		};

		struct PUserStatChanged2 : public sox::Marshallable{
			enum {uri = (4 << 8 | protocol::ONLINEDB_SVID)};

			PUserStatChanged2(){}
			PUserStatChanged2(uint32_t id, uint32_t linkid, std::string cip, protocol::E_ONLINESTAT st) 
				: uid(id), linkd_id(linkid), client_ip(cip), stat(st) {}

			uint32_t uid;
			uint32_t linkd_id;
			std::string client_ip;
			protocol::E_ONLINESTAT stat;

			virtual void marshal(sox::Pack &p) const{
				p << uid << linkd_id << client_ip;
				p.push_uint8(stat);
			}
			virtual void unmarshal(const sox::Unpack &p){
				p >> uid >> linkd_id >> client_ip;
				stat = (protocol::E_ONLINESTAT)p.pop_uint8();
			}
		};

		struct PUserStatChanged3 : public sox::Marshallable{
			enum {uri = (5 << 8 | protocol::ONLINEDB_SVID)};

			PUserStatChanged3(){}
			PUserStatChanged3(uint32_t id, uint32_t linkid, uint32_t cip, std::string cfrom, protocol::E_ONLINESTAT st) 
				: uid(id), linkd_id(linkid), client_ip(cip), client_from(cfrom), stat(st) {}

			uint32_t uid;
			uint32_t linkd_id;
			uint32_t client_ip;
			std::string client_from;
			protocol::E_ONLINESTAT stat;

			virtual void marshal(sox::Pack &p) const{
				p << uid << linkd_id << client_ip << client_from;
				p.push_uint8(stat);
			}
			virtual void unmarshal(const sox::Unpack &p){
				p >> uid >> linkd_id >> client_ip >> client_from;
				stat = (protocol::E_ONLINESTAT)p.pop_uint8();
			}
		};
		
		struct PUserStatChanged4 : public sox::Marshallable{
			enum {uri = (6 << 8 | protocol::ONLINEDB_SVID)};

			PUserStatChanged4(){}
			PUserStatChanged4(uint32_t id, uint32_t linkid, uint32_t cip, std::string cfrom, std::string cVer, protocol::E_ONLINESTAT st) 
				: uid(id), linkd_id(linkid), client_ip(cip), client_from(cfrom), client_ver(cVer), stat(st) {}

			uint32_t uid;
			uint32_t linkd_id;
			uint32_t client_ip;
			std::string client_from;
			std::string client_ver;
			protocol::E_ONLINESTAT stat;

			virtual void marshal(sox::Pack &p) const{
				p << uid << linkd_id << client_ip << client_from << client_ver;
				p.push_uint8(stat);
			}
			virtual void unmarshal(const sox::Unpack &p){
				p >> uid >> linkd_id >> client_ip >> client_from >> client_ver;
				stat = (protocol::E_ONLINESTAT)p.pop_uint8();
			}
		};

        struct PUserStatChanged5 : public sox::Marshallable{
            enum {uri = (7 << 8 | protocol::ONLINEDB_SVID)};

            PUserStatChanged5(){}
            PUserStatChanged5(uint32_t id, uint32_t linkid, uint32_t cip, std::string cfrom, std::string cVer, protocol::E_ONLINESTAT st, uint32_t sec, uint32_t usec)
                : uid(id), linkd_id(linkid), client_ip(cip), client_from(cfrom), client_ver(cVer), stat(st), tv_sec(sec), tv_usec(usec) {}

            uint32_t uid;
            uint32_t linkd_id;
            uint32_t client_ip;
            std::string client_from;
            std::string client_ver;
            protocol::E_ONLINESTAT stat;
            uint32_t tv_sec;
            uint32_t tv_usec;

            virtual void marshal(sox::Pack &p) const{
                p << uid << linkd_id << client_ip << client_from << client_ver;
                p.push_uint8(stat);
                p << tv_sec << tv_usec ;
            }
            virtual void unmarshal(const sox::Unpack &p){
                p >> uid >> linkd_id >> client_ip >> client_from >> client_ver;
                stat = (protocol::E_ONLINESTAT)p.pop_uint8();
                p >> tv_sec >> tv_usec ;
            }
        };

        struct PUserStatChanged6 : public sox::Marshallable{
            enum {uri = (8 << 8 | protocol::ONLINEDB_SVID)};

            PUserStatChanged6(){}
            PUserStatChanged6(uint32_t id, uint32_t linkid, uint32_t cip, std::string cfrom, std::string cVer, protocol::E_ONLINESTAT st, uint32_t sec, uint32_t usec, uint32_t tktime)
                : uid(id), linkd_id(linkid), client_ip(cip), client_from(cfrom), client_ver(cVer), stat(st), tv_sec(sec), tv_usec(usec), tk_timestamp(tktime) {}

            uint32_t uid;
            uint32_t linkd_id;
            uint32_t client_ip;
            std::string client_from;
            std::string client_ver;
            protocol::E_ONLINESTAT stat;
            uint32_t tv_sec;
            uint32_t tv_usec;
            uint32_t tk_timestamp;

            virtual void marshal(sox::Pack &p) const{
                p << uid << linkd_id << client_ip << client_from << client_ver;
                p.push_uint8(stat);
                p << tv_sec << tv_usec << tk_timestamp;
            }
            virtual void unmarshal(const sox::Unpack &p){
                p >> uid >> linkd_id >> client_ip >> client_from >> client_ver;
                stat = (protocol::E_ONLINESTAT)p.pop_uint8();
                p >> tv_sec >> tv_usec >> tk_timestamp;
            }
        };

		struct PUserStatToMQ : public sox::Marshallable{
			enum {uri = (100 << 8 | protocol::ONLINEDB_SVID)};

			PUserStatToMQ(){}
			PUserStatToMQ(uint32_t id, uint32_t linkid, std::string cip, uint32_t tm, protocol::E_ONLINESTAT st) 
				: uid(id), linkd_id(linkid), client_ip(cip), time(tm), stat(st) {}

			uint32_t uid;
			uint32_t linkd_id;
			std::string client_ip;
			uint32_t time;
			protocol::E_ONLINESTAT stat;

			virtual void marshal(sox::Pack &p) const{
				p << uid << linkd_id << client_ip << time;
				p.push_uint8(stat);
			}
			virtual void unmarshal(const sox::Unpack &p){
				p >> uid >> linkd_id >> client_ip >> time;
				stat = (protocol::E_ONLINESTAT)p.pop_uint8();
			}
		};


		struct PQueryOnlineSt : public sox::Marshallable{
			enum {uri = (2 << 8 | protocol::ONLINEDB_SVID)};

			PQueryOnlineSt(){}
			PQueryOnlineSt(uint16_t se, uint32_t id) : seq(se), uid(id) {}

			uint16_t seq;
			uint32_t uid;
			
			virtual void marshal(sox::Pack &p) const{
				p << seq << uid;
			}
			virtual void unmarshal(const sox::Unpack &p){
				p >> seq >> uid;
			}
		};

		struct PQueryOnlineSt_Res : public sox::Marshallable{
			enum {uri = (2 << 8 | protocol::ONLINEDB_SVID)};

			PQueryOnlineSt_Res(){}
			PQueryOnlineSt_Res(uint16_t se, uint32_t id, protocol::E_ONLINESTAT st) : seq(se), uid(id), stat(st) {}

			uint16_t seq;
			uint32_t uid;
			protocol::E_ONLINESTAT stat;

			virtual void marshal(sox::Pack &p) const{
				p << seq << uid;
				p.push_uint8(stat);
			}
			virtual void unmarshal(const sox::Unpack &p){
				p >> seq >> uid;
				stat = (protocol::E_ONLINESTAT)p.pop_uint8();
			}
		};

		struct PQueryOnlineSt_Vec : public sox::Marshallable{
			enum {uri = (3 << 8 | protocol::ONLINEDB_SVID)};

			PQueryOnlineSt_Vec(){}

			std::vector<uint32_t> uids;

			virtual void marshal(sox::Pack &p) const{
				sox::marshal_container(p, uids);
			}
			virtual void unmarshal(const sox::Unpack &p){
				sox::unmarshal_container(p, std::back_inserter(uids));
			}
		};


		struct PQueryOnlineSt_Vec_Res : public sox::Marshallable{
			enum {uri = (3 << 8 | protocol::ONLINEDB_SVID)};

			PQueryOnlineSt_Vec_Res(){}

			std::map<uint32_t, uint16_t> uid_stat_map;

			virtual void marshal(sox::Pack &p) const{
				sox::marshal_container(p, uid_stat_map);
			}
			virtual void unmarshal(const sox::Unpack &p){
				sox::unmarshal_container(p, std::inserter(uid_stat_map, uid_stat_map.begin()));
			}
		};
		
		struct S_Recover_OnlineStat2 : public sox::Marshallable{
			uint32_t uid;
			uint32_t client_ip;
			protocol::E_ONLINESTAT stat;

			virtual void marshal(sox::Pack &p) const {
				p << uid << client_ip;
				p.push_uint8(stat);
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> uid >> client_ip;
				stat = (protocol::E_ONLINESTAT)p.pop_uint8();
			}
		};

		//LINK_D to ONLINEDB
		struct PSS_Recover_Online2 : public sox::Marshallable{
			enum {uri = (19 << 8) | protocol::ONLINEDB_SVID};
			PSS_Recover_Online2(){}
			PSS_Recover_Online2(uint32_t id) : linkd_id(id) {}

			uint32_t linkd_id;
			std::vector<S_Recover_OnlineStat2> buddies_stat;

			virtual void marshal(sox::Pack &p) const {
				p << linkd_id;
				marshal_container(p, buddies_stat);
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> linkd_id;
				sox::unmarshal_container(p, std::back_inserter(buddies_stat));
			}
		};
		
	   struct S_Recover_OnlineStat3 : public sox::Marshallable{
			uint32_t uid;
			uint32_t client_ip;
			std::string client_from;
			std::string client_ver;
			protocol::E_ONLINESTAT stat;

			virtual void marshal(sox::Pack &p) const {
				 p << uid << client_ip << client_from << client_ver;
				 p.push_uint8(stat);
			}

			virtual void unmarshal(const sox::Unpack &p) {
				 p >> uid >> client_ip >> client_from >> client_ver;
				 stat = (protocol::E_ONLINESTAT)p.pop_uint8();
			}
	   };
	   
	   struct PSS_Recover_Online3 : public sox::Marshallable{
			enum {uri = (24 << 8) | protocol::ONLINEDB_SVID};
			PSS_Recover_Online3(){}
			PSS_Recover_Online3(uint32_t id) : linkd_id(id) {}

			uint32_t linkd_id;
			std::vector<S_Recover_OnlineStat3> buddies_stat;

			virtual void marshal(sox::Pack &p) const {
				 p << linkd_id;
				 marshal_container(p, buddies_stat);
			}

			virtual void unmarshal(const sox::Unpack &p) {
				 p >> linkd_id;
				 sox::unmarshal_container(p, std::back_inserter(buddies_stat));
			}
	   };      

	    struct S_Recover_OnlineStat4: public sox::Marshallable
      {
          uint32_t uid;
          uint32_t client_ip;
          std::string client_from;
          std::string client_ver;
          protocol::E_ONLINESTAT stat;
          uint32_t tv_sec;
          uint32_t tv_usec;

          virtual void marshal(sox::Pack &p) const
          {
              p << uid << client_ip << client_from << client_ver;
              p.push_uint8(stat);
              p << tv_sec << tv_usec;
          }

          virtual void unmarshal(const sox::Unpack &p)
          {
              p >> uid >> client_ip >> client_from >> client_ver;
              stat = (protocol::E_ONLINESTAT) p.pop_uint8();
              p >> tv_sec >> tv_usec;
          }
      };

      struct PSS_Recover_Online4: public sox::Marshallable
      {
        enum
        {
            uri = (25 << 8) | protocol::ONLINEDB_SVID
        };
        PSS_Recover_Online4()
        {
        }
        PSS_Recover_Online4(uint32_t id)
                : linkd_id(id)
        {
        }

        uint32_t linkd_id;
        std::vector<S_Recover_OnlineStat4> buddies_stat;

        virtual void marshal(sox::Pack &p) const
        {
            p << linkd_id;
            marshal_container(p, buddies_stat);
        }

        virtual void unmarshal(const sox::Unpack &p)
        {
            p >> linkd_id;
            sox::unmarshal_container(p, std::back_inserter(buddies_stat));
        }
    };
  
    struct S_Recover_OnlineStat5 : public sox::Marshallable
    {
      uint32_t uid;
      uint32_t client_ip;
      std::string client_from;
      std::string client_ver;
      protocol::E_ONLINESTAT stat;
      uint32_t tv_sec;
      uint32_t tv_usec;
      uint32_t ticketTimestamp;

      virtual void marshal(sox::Pack &p) const
      {
          p << uid << client_ip << client_from << client_ver;
          p.push_uint8(stat);
          p << tv_sec << tv_usec << ticketTimestamp;
      }

      virtual void unmarshal(const sox::Unpack &p)
      {
          p >> uid >> client_ip >> client_from >> client_ver;
          stat = (protocol::E_ONLINESTAT) p.pop_uint8();
          p >> tv_sec >> tv_usec >> ticketTimestamp;
      }

    };

    struct S_Recover_OnlineStat6 : public sox::Marshallable
    {
      uint32_t uid;
      uint32_t client_ip;
      std::string client_from;
      std::string client_ver;
      protocol::E_ONLINESTAT stat;
      uint32_t tv_sec;
      uint32_t tv_usec;
      uint32_t ticketTimestamp;
      std::string pcinfo;

      virtual void marshal(sox::Pack &p) const
      {
          p << uid << client_ip << client_from << client_ver;
          p.push_uint8(stat);
          p << tv_sec << tv_usec << ticketTimestamp << pcinfo;
      }

      virtual void unmarshal(const sox::Unpack &p)
      {
          p >> uid >> client_ip >> client_from >> client_ver;
          stat = (protocol::E_ONLINESTAT) p.pop_uint8();
          p >> tv_sec >> tv_usec >> ticketTimestamp >> pcinfo;
      }

    };
    
    struct PSS_Recover_Online5 : public sox::Marshallable{
     enum {uri = (60 << 8) | protocol::ONLINEDB_SVID};
     PSS_Recover_Online5(){}
     PSS_Recover_Online5(uint32_t id) : linkd_id(id) {}
    
     uint32_t linkd_id;
     std::vector<S_Recover_OnlineStat5> buddies_stat;
    
     virtual void marshal(sox::Pack &p) const {
        p << linkd_id;
        marshal_container(p, buddies_stat);
     }
    
     virtual void unmarshal(const sox::Unpack &p) {
        p >> linkd_id;
        sox::unmarshal_container(p, std::back_inserter(buddies_stat));
     }
    };   

    struct PSS_Recover_Online6 : public sox::Marshallable{
     enum {uri = (61 << 8) | protocol::ONLINEDB_SVID};
     PSS_Recover_Online6(){}
     PSS_Recover_Online6(uint32_t id) : linkd_id(id) {}

     uint32_t linkd_id;
     std::vector<S_Recover_OnlineStat6> buddies_stat;

     virtual void marshal(sox::Pack &p) const {
        p << linkd_id;
        marshal_container(p, buddies_stat);
     }

     virtual void unmarshal(const sox::Unpack &p) {
        p >> linkd_id;
        sox::unmarshal_container(p, std::back_inserter(buddies_stat));
     }
    };

		struct PReqRecoverOnline : public sox::Voidmable{
			enum {uri = (134 << 8) | protocol::LINKD_SVID};
		};

        //linkd recv PReqRecoverOnline2, and than reply PSS_Recover_Online4 to online_user
        struct PReqRecoverOnline2 : public sox::Voidmable{
            enum {uri = (135 << 8) | protocol::ONLINEDB_SVID};
        };

		//ONLINEDB to ONLINEDB
		struct PBroadcastGlobalBan : public sox::Marshallable{
			enum {uri = (30 << 8) | protocol::ONLINEDB_SVID};

			uint32_t uid;

			virtual void marshal(sox::Pack &p) const {
				p << uid;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> uid;
			}
		};

		//LINK_D to ONLINEDB
		struct PCheckGlobalBan : public sox::Marshallable{
			enum {uri = (20 << 8) | protocol::ONLINEDB_SVID};

			uint32_t uid;
			std::string pc_info;

			virtual void marshal(sox::Pack &p) const {
				p << uid << pc_info;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> uid >> pc_info;
			}
		};
		struct PSessionFreeze: public sox::Marshallable{
			enum {uri = (21 << 8) | protocol::ONLINEDB_SVID};
			uint32_t sid;
			std::vector<uint32_t> uids;
			uint32_t freeSeconds;
			PSessionFreeze(){
				freeSeconds = 4 * 60 * 60;
			};

			virtual void marshal(sox::Pack &p) const {
				p << sid << freeSeconds;
				sox::marshal_container(p, uids);
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> sid >> freeSeconds;
				sox::unmarshal_container(p, std::back_inserter(uids));
			}
		};
		
		struct PLoginedClientsToAux: public sox::Marshallable{
			enum {uri = (22 << 8) | protocol::ONLINEDB_SVID};
			std::map<uint32_t, uint32_t> logined_clients;

			virtual void marshal(sox::Pack &p) const {
				sox::marshal_container(p, logined_clients);
            }

            virtual void unmarshal(const sox::Unpack &p) {
				sox::unmarshal_container(p, std::inserter(logined_clients, logined_clients.begin()));
            }
			
		};

		struct POnlineStdbSelfNotification: public sox::Marshallable {
			enum { uri = (23 << 8) | protocol::ONLINEDB_SVID};
			
			virtual void marshal(sox::Pack &p) const {
			}
			virtual void unmarshal(const sox::Unpack &p) {
			}
		};
		
		//获取在线状态
		struct PGetOnlineSt: public sox::Marshallable {
			enum { uri = (141 << 8) | protocol::ONLINEDB_SVID}; //ONLINEDB_SVID值为22

			std::string client_from; //发出请求的客户端来源，后台统计使用

			virtual void marshal(sox::Pack &p) const {
				p << client_from;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> client_from;
			}
		};
		
		//返回在线状态
		struct PGetOnlineStRes: public sox::Marshallable {
			enum { uri = (142 << 8) | protocol::ONLINEDB_SVID}; //ONLINEDB_SVID值为22
			
			std::vector<uint32_t> type_list;  //客户端类型列表，0/1/2依次代表pc/手机/pad
			std::vector<uint32_t> lg_time_list; //各类型的登陆时间戳，个数顺序与type_List一致
			std::vector<std::string> desc_list; //类型的具体描述，个数顺序与type_List一致，值为iPhone/Android之类，可以为空
			std::vector<uint32_t> pic_type_list; //该类型用于显示客户端图标
			std::vector<uint32_t> pic2_type_list;			

			virtual void marshal(sox::Pack &p) const {
				sox::marshal_container(p, type_list);
				sox::marshal_container(p, lg_time_list);
				sox::marshal_container(p, desc_list);
				sox::marshal_container(p, pic_type_list);
				sox::marshal_container(p, pic2_type_list);
			}

			virtual void unmarshal(const sox::Unpack &p) {
				sox::unmarshal_container(p, std::back_inserter(type_list));
				sox::unmarshal_container(p, std::back_inserter(lg_time_list));
				sox::unmarshal_container(p, std::back_inserter(desc_list));
				sox::unmarshal_container(p, std::back_inserter(pic_type_list));
				sox::unmarshal_container(p, std::back_inserter(pic2_type_list));
			}
		};
	








		//获取在线状态
		struct PGetOnlineSt2: public sox::Marshallable {
			enum { uri = (161 << 8) | protocol::ONLINEDB_SVID}; //ONLINEDB_SVID值为22

			std::string client_from; //发出请求的客户端来源，后台统计使用

			virtual void marshal(sox::Pack &p) const {
				p << client_from;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> client_from;
			}
		};
		
		//返回在线状态
		struct PGetOnlineStRes2: public sox::Marshallable {
			enum { uri = (162 << 8) | protocol::ONLINEDB_SVID}; //ONLINEDB_SVID值为22
			
			std::vector<uint32_t> type_list;  //客户端类型列表，0/1/2依次代表pc/手机/pad
			std::vector<uint32_t> lg_time_list; //各类型的登陆时间戳，个数顺序与type_List一致
			std::vector<std::string> desc_list; //类型的具体描述，个数顺序与type_List一致，值为iPhone/Android之类，可以为空
			std::vector<uint32_t> pic_client_type_list; //以下三个类型用于标识客户端图标类型
			std::vector<uint32_t> pic_mobile_type_list;
			std::vector<uint32_t> pic_app_type_list;			

			virtual void marshal(sox::Pack &p) const {
				sox::marshal_container(p, type_list);
				sox::marshal_container(p, lg_time_list);
				sox::marshal_container(p, desc_list);
				sox::marshal_container(p, pic_client_type_list);
				sox::marshal_container(p, pic_mobile_type_list);
				sox::marshal_container(p, pic_app_type_list);
			}

			virtual void unmarshal(const sox::Unpack &p) {
				sox::unmarshal_container(p, std::back_inserter(type_list));
				sox::unmarshal_container(p, std::back_inserter(lg_time_list));
				sox::unmarshal_container(p, std::back_inserter(desc_list));
				sox::unmarshal_container(p, std::back_inserter(pic_client_type_list));
				sox::unmarshal_container(p, std::back_inserter(pic_mobile_type_list));
				sox::unmarshal_container(p, std::back_inserter(pic_app_type_list));
			}
		};





		
		//踢除某客户端
		struct PKickSomeClient: public sox::Marshallable {
			enum { uri = (143 << 8) | protocol::ONLINEDB_SVID}; //ONLINEDB_SVID值为22
			
			std::string client_from; //发出请求的客户端来源
            uint32_t target_client_type;  //指定要踢的端，不允许是当前客户端
			
			virtual void marshal(sox::Pack &p) const {
				p << client_from << target_client_type;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> client_from >> target_client_type;
			}
		};
		
		//返回踢除结果
		struct PKickSomeClientRes: public sox::Marshallable {
			enum { uri = (144 << 8) | protocol::ONLINEDB_SVID}; //ONLINEDB_SVID值为22
			
			uint32_t kick_res_code; //0 - 成功, 1 - 那端早已下线， 2 - 是当前端， 3 - 失败
			
			virtual void marshal(sox::Pack &p) const {
				p << kick_res_code;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> kick_res_code;
			}
		};



        //获取在线状态
        struct PGetOnlineSt3: public sox::Marshallable {
            enum { uri = (181 << 8) | protocol::ONLINEDB_SVID}; //ONLINEDB_SVID值为22

            uint64_t    uid;
            std::string client_from; //发出请求的客户端来源

            virtual void marshal(sox::Pack &p) const {
                p << uid << client_from;
            }

            virtual void unmarshal(const sox::Unpack &p) {
                p >> uid >> client_from;
            }
        };

        //返回在线状态
        struct PGetOnlineStRes3: public sox::Marshallable {
            enum { uri = (182 << 8) | protocol::ONLINEDB_SVID}; //ONLINEDB_SVID值为22

            std::vector<std::string> client_list;
            std::vector<uint32_t>    lg_time_list;

            virtual void marshal(sox::Pack &p) const {
                sox::marshal_container(p, client_list);
                sox::marshal_container(p, lg_time_list);
            }

            virtual void unmarshal(const sox::Unpack &p) {
                sox::unmarshal_container(p, std::back_inserter(client_list));
                sox::unmarshal_container(p, std::back_inserter(lg_time_list));
            }
        };


        //踢除某客户端
        struct PKickSomeClient2: public sox::Marshallable {
            enum { uri = (183 << 8) | protocol::ONLINEDB_SVID}; //ONLINEDB_SVID值为22

            uint64_t    uid;
            std::string client_from;        //发出请求的客户端来源
            std::string target_client_type;  //指定要踢的端，不允许是当前客户端

            virtual void marshal(sox::Pack &p) const {
                p << uid << client_from << target_client_type;
            }

            virtual void unmarshal(const sox::Unpack &p) {
                p >> uid >> client_from >> target_client_type;
            }
        };

        //返回踢除结果
        struct PKickSomeClientRes2: public sox::Marshallable {
            enum { uri = (184 << 8) | protocol::ONLINEDB_SVID}; //ONLINEDB_SVID值为22

            uint32_t kick_res_code; //0 - 成功, 1 - 那端早已下线， 2 - 是当前端， 3 - 失败

            virtual void marshal(sox::Pack &p) const {
                p << kick_res_code;
            }

            virtual void unmarshal(const sox::Unpack &p) {
                p >> kick_res_code;
            }
        };
	}
}


#endif
