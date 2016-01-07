#ifndef _PLINKDMQ_H_
#define _PLINKDMQ_H_

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"
#include <string>
#include <vector>

namespace server{
	namespace linkd_mq{
	
		struct PUserStatToMQ2 : public sox::Marshallable{
			enum {uri = (5 << 8 | protocol::ONLINEDB_SVID)};

			uint32_t uid;
			uint32_t linkd_id;
			uint32_t client_ip;
			uint32_t time;
			std::string client_from;
			protocol::E_ONLINESTAT stat;
			
			uint32_t language_id;
			uint32_t extend_int;
			std::string extend_str;
			
			PUserStatToMQ2()
				: uid(0)
				, linkd_id(0)
				, client_ip(0)
				, time(0)
				, client_from()
				, language_id(0)
				, extend_int(0)
				, extend_str()
				{}

			virtual void marshal(sox::Pack &p) const{
				p << uid << linkd_id << client_ip << time << client_from << language_id << extend_int << extend_str;
				p.push_uint8(stat);
			}
			virtual void unmarshal(const sox::Unpack &p){
				p >> uid >> linkd_id >> client_ip >> time >> client_from >> language_id >> extend_int >> extend_str;
				stat = (protocol::E_ONLINESTAT)p.pop_uint8();
			}
		};
		
		struct PSessionUpload : public sox::Marshallable{
			enum {uri = (61 << 8 | 9)};

			uint32_t uid;
			uint32_t ip;
			uint32_t time;
			uint32_t first_sid;
			uint32_t second_sid;
			std::string mac;
			
			PSessionUpload()
				: uid(0)
				, ip(0)
				, time(0)
				, first_sid(0)
				, second_sid(0)
				, mac()
				{}

			virtual void marshal(sox::Pack &p) const{
				p << uid << ip << time << first_sid << second_sid << mac;
			}
			virtual void unmarshal(const sox::Unpack &p){
				p >> uid >> ip >> time >> first_sid >> second_sid >> mac;
			}
		};
		
			
		class AntiClient_Login : public ::sox::Marshallable
        {
        public:
            enum{ uri = (1 << 8 | ::protocol::COMPONENT_SVID ) };
            
			uint32_t uid;
            uint32_t type; //0-login in;1-login out
            uint32_t ip;
            uint32_t lkId;
            uint32_t time;
			std::string client_from;

			AntiClient_Login()
				: uid(0)
				, type(-1)
				, ip(0)
				, lkId(0)
				, time(0)
				, client_from()
			{}
			
            void marshal( ::sox::Pack& pk ) const
            {
                pk << uid << type << ip << lkId << time << client_from;
            }

            void unmarshal( const ::sox::Unpack& pk )
            {
                pk >> uid >> type >> ip >> lkId >> time >> client_from;
            }
        };
		
		class PCS_UserLogoutNotice: public ::sox::Marshallable
		{
			public:
				static const uint32_t kUri =
					( 1 << 8 | 200 );
				enum { uri = kUri };

			public:
				PCS_UserLogoutNotice() 
					: uid(0)
				{}

				void marshal( ::sox::Pack& pk ) const
				{
					pk << uid;
				}

				void unmarshal( const ::sox::Unpack& pk )
				{
					pk >> uid;
				}

			public:
				uint32_t    uid;     // user id  
		};


		class Anti_UidStatusQuery : public ::sox::Marshallable
		{
			public:
				enum
				{
					uri = (40 << 8 | ::protocol::COMPONENT_SVID ) 
				};

			public:
				void marshal( ::sox::Pack& pk ) const
				{
					pk << uid;
				}
    
      void unmarshal( const ::sox::Unpack& pk )
      {
        pk >> uid;
      }
    public:
      uint32_t uid;
    };
    
    class Anti_UidStatusQueryRsp : public ::sox::Marshallable
    {
    public:
      enum
      {
        uri = (41 << 8 | ::protocol::COMPONENT_SVID ) 
      };
    
    public:
      void marshal( ::sox::Pack& pk ) const
      {
        pk << uid << online << lastpingtime;
      }
    
      void unmarshal( const ::sox::Unpack& pk )
      {
        pk >> uid >> online >> lastpingtime;
      }
    public:
      uint32_t uid;
      uint32_t online;
      uint32_t lastpingtime;
    };
    
	}
}

#endif
