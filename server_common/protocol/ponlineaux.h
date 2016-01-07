#ifndef _PONLINEAUX_H_
#define _PONLINEAUX_H_

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"

#include <vector>

namespace server{
	namespace online
	{
		struct PGetLinkdCurUsers : public sox::Voidmable{
			enum {uri = (138 << 8) | protocol::LINKD_SVID};
		};

		struct PGetLinkdCurUsersRes : public sox::Marshallable{
			enum {uri = (1 << 8) | protocol::ONLINE_ST_SVID};

			uint32_t linkd_id;
			std::map<std::string, uint32_t> users;	// <user_type, users>

			virtual void marshal(sox::Pack &p) const {
				p << linkd_id;
				sox::marshal_container(p, users);
			}

			virtual void unmarshal(const sox::Unpack &p){
				p >> linkd_id;
				sox::unmarshal_container(p, std::inserter(users, users.begin()));
			}
		};

                struct PGetLinkdCurUsersRes2 : public sox::Marshallable{
                        enum {uri = (4 << 8) | protocol::ONLINE_ST_SVID};

                        uint32_t linkd_id;
                        std::map<uint32_t, std::map<std::string, uint32_t> > hist_users;  // <user_type, users>

                        virtual void marshal(sox::Pack &p) const {
                                p << linkd_id;

				p << hist_users.size();
				for(std::map<uint32_t, std::map<std::string, uint32_t> >::const_iterator i = hist_users.begin(); i != hist_users.end(); ++i) {
					p << i->first;
					sox::marshal_container(p, i->second);
				}
                        }

                        virtual void unmarshal(const sox::Unpack &p){
                                p >> linkd_id;

				uint32_t size = 0;
				p >> size;

				for(uint32_t i=0;i<size;++i) {
					uint32_t time=0;
					std::map<std::string, uint32_t> users_snapshot;

					p >> time;
					sox::unmarshal_container(p, std::inserter(users_snapshot, users_snapshot.begin()));
					hist_users[time] = users_snapshot;
				}
                        }
                };

		enum LoginStatus{
			NORMAL_LOGIN = 0,
			USER_NOT_EXIST,
			ACCOUNT_FROZEN,
			PASSWD_FAILED,
			RANDOM_LINKD_FAIL,
			FAILED_UNKNOWN,
			FAILED_TOKEN_FAIL, // 6,  硬件令牌/手机令牌 失败
			FAILED_TOKEN_TIMEOUT, // 7, 令牌超时
			FAILED_VCODE_FAIL, // 服务器验证码失败
			FAILED_VCODE_TIMEOUT, // 服务器验证码超时
		};

		struct LbsLoginInfo: public sox::Marshallable{

			LbsLoginInfo(){}
			LbsLoginInfo(uint32_t uid_, uint32_t login_time_, const std::string& user_ip_, const std::string& login_type_, 
				const std::string& register_from_, uint32_t login_status_, const std::string& account_)
				: uid(uid_), login_time(login_time_), user_ip(user_ip_), login_type(login_type_), register_from(register_from_),
					login_status(login_status_), account(account_) {}
			uint32_t uid;
			uint32_t login_time;
			std::string user_ip;
			std::string login_type;
			std::string register_from;
			uint32_t login_status;
			std::string account;

			virtual void marshal(sox::Pack &p) const{
				p << uid;
				p << login_time;
				p << user_ip;
				p << login_type;
				p << register_from;
				p << login_status;
				p << account;
			}

			virtual void unmarshal(const sox::Unpack &p){
				p >> uid;
				p >> login_time;
				p >> user_ip;
				p >> login_type;
				p >> register_from;
				p >> login_status;
				p >> account;
			}
		};

    struct LbsLoginInfo2: public LbsLoginInfo {

      LbsLoginInfo2(){}
      LbsLoginInfo2(uint32_t uid_, uint32_t login_time_, const std::string& user_ip_, const std::string& login_type_, 
				const std::string& register_from_, uint32_t login_status_, const std::string& account_)
        : LbsLoginInfo(uid_, login_time_, user_ip_, login_type_, register_from_, login_status_, account_)
      {}

      LbsLoginInfo2(uint32_t uid_, uint32_t login_time_, const std::string& user_ip_, const std::string& login_type_, 
				const std::string& register_from_, uint32_t login_status_, const std::string& account_,
        const std::string& vStr_, const std::string& mac_)
        : LbsLoginInfo(uid_, login_time_, user_ip_, login_type_, register_from_, login_status_, account_)
        , verStr(vStr_), macAddr(mac_)
      {}

      std::string verStr;
      std::string macAddr;

      virtual void marshal(sox::Pack &p) const{
        LbsLoginInfo::marshal(p);
        p << verStr << macAddr;
      }

      virtual void unmarshal(const sox::Unpack &p){
        LbsLoginInfo::unmarshal(p);
        p >> verStr >> macAddr;
      }
    };

    struct LbsLoginInfo3: public LbsLoginInfo2 {

      LbsLoginInfo3(){}
      LbsLoginInfo3(uint32_t uid_, uint32_t login_time_, const std::string& user_ip_, const std::string& login_type_, 
				const std::string& register_from_, uint32_t login_status_, const std::string& account_)
        : LbsLoginInfo2(uid_, login_time_, user_ip_, login_type_, register_from_, login_status_, account_)
        , user_port(0)
      {}

      LbsLoginInfo3(uint32_t uid_, uint32_t login_time_, const std::string& user_ip_, uint16_t user_port_, const std::string& login_type_, 
				const std::string& register_from_, uint32_t login_status_, const std::string& account_,
        const std::string& vStr_, const std::string& mac_)
        : LbsLoginInfo2(uid_, login_time_, user_ip_, login_type_, register_from_, login_status_, account_, vStr_, mac_)
        , user_port(user_port_)
      {}

      uint16_t user_port;

      virtual void marshal(sox::Pack &p) const{
        LbsLoginInfo2::marshal(p);
        p << user_port;
      }

      virtual void unmarshal(const sox::Unpack &p){
        LbsLoginInfo2::unmarshal(p);
        p >> user_port;
      }
    };

		struct PGetLbsLoginInfo : public sox::Voidmable{
			enum {uri = (107 << 8) | protocol::LBS_SVID};
		};

		struct PGetLbsLoginInfoRes : public sox::Marshallable{
			enum {uri = (2 << 8) | protocol::ONLINE_ST_SVID};

			uint32_t lbsd_id;
			std::vector<LbsLoginInfo> loginInfos;

			virtual void marshal(sox::Pack &p) const {
				p << lbsd_id;
				sox::marshal_container(p, loginInfos);
			}

			virtual void unmarshal(const sox::Unpack &p){
				p >> lbsd_id;
				sox::unmarshal_container(p, std::inserter(loginInfos, loginInfos.begin()));
			}
		};

    struct PGetLbsLoginInfoRes2 : public sox::Marshallable{
			enum {uri = (3 << 8) | protocol::ONLINE_ST_SVID};

			uint32_t lbsd_id;
			std::vector<LbsLoginInfo2> loginInfos;

			virtual void marshal(sox::Pack &p) const {
				p << lbsd_id;
				sox::marshal_container(p, loginInfos);
			}

			virtual void unmarshal(const sox::Unpack &p){
				p >> lbsd_id;
				sox::unmarshal_container(p, std::inserter(loginInfos, loginInfos.begin()));
			}
		};
	}

	namespace onlineaux{
		

		struct PUpdateUserIp : public sox::Marshallable{
			enum {uri = (1 << 8) | protocol::ONLINEAUX_SVID};
			uint32_t uid;
			uint64_t uip;
			uint32_t linkd_id;
			uint32_t login_time;
			std::string strPcInfo;

			virtual void marshal(sox::Pack &p) const {
				p << uid;
				p << uip;
				p << linkd_id;
				p << login_time;
				p << strPcInfo;
			}

			virtual void unmarshal(const sox::Unpack &p){
				p >> uid;
				p >> uip;
				p >> linkd_id;
				p >> login_time;
				p >> strPcInfo;
			}
		};

		struct PUserStatChangedAux : public sox::Marshallable{
			enum {uri = (2 << 8 | protocol::ONLINEAUX_SVID)};

			PUserStatChangedAux(){}
			PUserStatChangedAux(uint32_t id, uint32_t linkid, protocol::E_ONLINESTAT st) 
				: uid(id), linkd_id(linkid), stat(st) {}

			uint32_t uid;
			uint32_t linkd_id;
			protocol::E_ONLINESTAT stat;

			virtual void marshal(sox::Pack &p) const{
				p << uid << linkd_id;
				p.push_uint8(stat);
			}
			virtual void unmarshal(const sox::Unpack &p){
				p >> uid >> linkd_id;
				stat = (protocol::E_ONLINESTAT)p.pop_uint8();
			}
		};


		struct PCheckLinkdAlive: public sox::Marshallable{
			enum {uri = (126 << 8)|protocol::LINKD_SVID};
			uint32_t uid;
			uint64_t uip;

			virtual void marshal(sox::Pack &p) const {
				p << uid;
				p << uip;
			}

			virtual void unmarshal(const sox::Unpack &p){
				p >> uid;
				p >> uip;
			}
		};

		struct PSyncLinkdToOnline: public sox::Voidmable{
			enum {uri = (129 << 8)|protocol::LINKD_SVID};
		};

		struct UserInfoAux: public sox::Marshallable{
			uint64_t uip;
			uint32_t login_time;
			std::string strPcInfo;
			virtual void marshal(sox::Pack &p) const{
				p << uip;
				p << login_time;
				p << strPcInfo;
			}

			virtual void unmarshal(const sox::Unpack &p){
				p >> uip;
				p >> login_time;
				p >> strPcInfo;
			}
		};

		struct POnlineUsersInfo: public sox::Marshallable{
			enum {uri = (139 << 8)|protocol::LINKD_SVID};
			uint32_t linkd_id;
			std::map<uint32_t, UserInfoAux> uinf;

			virtual void marshal(sox::Pack &p) const {
				p << linkd_id;
				sox::marshal_container(p, uinf);
			}

			virtual void unmarshal(const sox::Unpack &p){
				p >> linkd_id;
				sox::unmarshal_container(p, std::inserter(uinf,uinf.begin()));
			}
		};

		// 2011-08-08 add by zxz
		struct PIPUsualLoginInfo:public sox::Marshallable{
			enum {uri = (3 << 8)| protocol::ONLINEAUX_SVID};
			uint32_t _uid;
			uint64_t _uip;
			uint32_t _linkd_id;
			uint32_t _login_time;
			uint32_t _logout_time;
			PIPUsualLoginInfo():_uid(0),_uip(0),_linkd_id(0),_login_time(0),_logout_time(0){}
			virtual void marshal(sox::Pack &p) const {
				p << _uid << _uip << _linkd_id << _login_time << _logout_time;
			}

			virtual void unmarshal(const sox::Unpack &p){
				p >> _uid >> _uip >> _linkd_id >> _login_time >> _logout_time;
			}
		};
	}

	namespace onlinemisc
	{
		struct PCheckLinkdAliveRes: public sox::Marshallable{
			enum {uri = (1 << 8)|protocol::ONLINEMISC_SVID};

			uint32_t uid;
			uint64_t uip;

			virtual void marshal(sox::Pack &p) const {
				p << uid;
				p << uip;
			}

			virtual void unmarshal(const sox::Unpack &p){
				p >> uid;
				p >> uip;
			}
		};
		
		struct PStartProcess: public sox::Marshallable{
			enum {uri = (2 << 8)|protocol::ONLINEMISC_SVID};

			virtual void marshal(sox::Pack &p) const {
			}

			virtual void unmarshal(const sox::Unpack &p){
			}
		};
	}
}


#endif
