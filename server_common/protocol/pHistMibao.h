#ifndef _PHISTMIBAO_H_
#define _PHISTMIBAO_H_

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"
#include <string>
#include <vector>

namespace server{
	namespace hist_mibao{
	
		struct PCheckHistQuest : public sox::Marshallable{
			enum {uri = (1 << 8 | protocol::MQ_SVID)};

			uint32_t uid;
			std::string question;
			std::string answer;
			uint32_t days;
			
			PCheckHistQuest() : uid(0) {}

			virtual void marshal(sox::Pack &p) const{
				p << uid << days << question << answer;
			}
			virtual void unmarshal(const sox::Unpack &p){
				p >> uid >> days >> question >> answer;
			}
		};

		struct PCheckHistEmail : public sox::Marshallable{
			enum {uri = (2 << 8 | protocol::MQ_SVID)};

			uint32_t uid;
			std::string email;
			uint32_t days;
			
			PCheckHistEmail() : uid(0) {}

			virtual void marshal(sox::Pack &p) const{
				p << uid << days << email;
			}
			virtual void unmarshal(const sox::Unpack &p){
				p >> uid >> days >> email;
			}
		};
		
		struct PCheckHistMobile : public sox::Marshallable{
			enum {uri = (3 << 8 | protocol::MQ_SVID)};

			uint32_t uid;
			std::string mobile;
			uint32_t days;
			
			PCheckHistMobile() : uid(0) {}

			virtual void marshal(sox::Pack &p) const{
				p << uid << days << mobile;
			}
			virtual void unmarshal(const sox::Unpack &p){
				p >> uid >> days >> mobile;
			}
		};
		
		struct PCheckHistCertific : public sox::Marshallable{
			enum {uri = (4 << 8 | protocol::MQ_SVID)};

			uint32_t uid;
			uint32_t type;
			std::string cert_no;
			uint32_t days;
			
			PCheckHistCertific() : uid(0) {}

			virtual void marshal(sox::Pack &p) const{
				p << uid << days << type << cert_no;
			}
			virtual void unmarshal(const sox::Unpack &p){
				p >> uid >> days >> type >> cert_no;
			}
		};
		
		//共用一个回消息
		struct PCheckHistRes : public sox::Marshallable{
			enum {uri = (5 << 8 | protocol::MQ_SVID)};

			uint32_t uid;
			uint32_t flag; //匹配的个数，-1=查询失败
			std::vector<uint32_t> match_time; //依次列出匹配的时间顺序
			
			PCheckHistRes() : uid(0), flag(0) {}

			virtual void marshal(sox::Pack &p) const{
				p << uid << flag;
				sox::marshal_container(p, match_time);
			}
			virtual void unmarshal(const sox::Unpack &p){
				p >> uid >> flag;
				sox::unmarshal_container(p, std::back_inserter(match_time));
			}
		};

		enum HISTMIBAO_TYPE
		{
			QUESTION = 1,
			EMAIL = 2,
			MOBILE = 3,
			ID = 4,
		};

        struct PGetHistInfoModReq : public sox::Marshallable{
             enum {uri = (6 << 8 | protocol::MQ_SVID)};

             uint32_t uid;
			 uint32_t context;
             uint32_t type;	// 1 = Question(Not supported), 2 = Email, 3 = Mobile, 4 = ID

             PGetHistInfoModReq() : uid(0),context(0),type(0) {}

             virtual void marshal(sox::Pack &p) const{
                     p << uid << context << type;
             }
             virtual void unmarshal(const sox::Unpack &p){
                     p >> uid >> context >> type;
             }
        };

        struct PGetHistInfoModRes : public sox::Marshallable{
             enum {uri = (7 << 8 | protocol::MQ_SVID)};

             uint32_t uid;
             uint32_t context;
             std::vector<uint32_t> mod_times;

             PGetHistInfoModRes() : uid(0),context(0) {}

             virtual void marshal(sox::Pack &p) const{
                     p << uid << context;
                     sox::marshal_container(p,mod_times);
             }
             virtual void unmarshal(const sox::Unpack &p){
                     p >> uid >> context;
                     sox::unmarshal_container(p, std::back_inserter(mod_times));
             }
        };

	}
}

#endif
