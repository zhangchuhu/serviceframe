#ifndef _P_SHORT_MESSAGE_SEND_BACK_H_
#define _P_SHORT_MESSAGE_SEND_BACK_H_

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"

namespace server{
    namespace smsbk
    {
        static void coding_vec(sox::Pack &p,const std::vector<std::string>& v)
        {
            uint32_t len=v.size();
            p << len;
            std::vector<std::string>::const_iterator i=v.begin();
            for(;i!=v.end();++i)
            {
                p.push_varstr32(i->data(), i->size());
            }
        }
        static void decode_vec(const sox::Unpack &p,std::vector<std::string>& v)
        {
            uint32_t len=0;
            p >> len;
            v.resize(len);
            std::vector<std::string>::iterator i=v.begin();
            for(;i!=v.end();++i)
            {
                (*i)=p.pop_varstr32();
            }
        }

        struct PSmSend:  public sox::Marshallable{
            enum {uri = (1 << 8) | protocol::SMS_BK_SVID};            

            std::string strSeq; // 
            std::string strPhone;
            std::string strContent; // utf8
            std::string strUserName; // for statistic.
            std::string strType; // default : empty . pdu ? common text message ? wap push?
            std::string strTime; // default : empty.
            
            virtual void marshal(sox::Pack &p) const {                    
                p << strSeq << strPhone << strContent << strUserName << strType << strTime;            
            }

            virtual void unmarshal(const sox::Unpack &p){
                p >> strSeq >> strPhone >> strContent >> strUserName >> strType >> strTime;        
            }
        };            

        struct PSmSendRes:  public sox::Marshallable{
            enum {uri = (2 << 8) | protocol::SMS_BK_SVID};            

            std::string strSeq; // 
            uint32_t uRes; // 0 - success,1 - should retry,2 - system error

            virtual void marshal(sox::Pack &p) const {                    
                p << strSeq << uRes;
            }

            virtual void unmarshal(const sox::Unpack &p){
                p >> strSeq >> uRes;
            }
        };        

        struct PSmSendBatch:  public sox::Marshallable{
            enum {uri = (3 << 8) | protocol::SMS_BK_SVID};                

            std::vector<std::string> vecSmId;
            std::vector<std::string> vecPhone;
                                    
            std::string strContent; // utf8

            virtual void marshal(sox::Pack &p) const {                
                coding_vec(p,vecSmId);
                coding_vec(p,vecPhone);

                p << strContent;    
            }

            virtual void unmarshal(const sox::Unpack &p){                
                decode_vec(p,vecSmId);
                decode_vec(p,vecPhone);

                p >> strContent;

            }
        };            

        struct PSmSendBatchRes:  public sox::Marshallable{
            enum {uri = (4 << 8) | protocol::SMS_BK_SVID};                

            std::string strSmId;
            std::string strPhone;
            uint32_t uRes;            // 0    -    success, others    -    fail.

            virtual void marshal(sox::Pack &p) const {                    
                p << strSmId << strPhone << uRes;
            }

            virtual void unmarshal(const sox::Unpack &p){
                p >> strSmId >> strPhone >> uRes;
            }
        };            

        struct PSmSendBatchAsync:  public sox::Marshallable{
            enum {uri = (5 << 8) | protocol::SMS_BK_SVID};                

            std::string strSequence;
            std::string strSource;
            std::vector<std::string> vecPhone;

            std::string strContent; // utf8

            virtual void marshal(sox::Pack &p) const {                
                p << strSequence;
                p << strSource;
                coding_vec(p,vecPhone);

                p << strContent;    
            }

            virtual void unmarshal(const sox::Unpack &p){                
                p >> strSequence;
                p >> strSource;
                decode_vec(p,vecPhone);
                p >> strContent;
            }
        };            

        struct PSmSendBatchAsyncRes:  public sox::Marshallable{
            enum {uri = (6 << 8) | protocol::SMS_BK_SVID};                

            std::string strSequence;
            std::vector<std::string> vecFailRes;    // 1    -    database error.
                                                    // 2    -    system error.
                                                    // 3    -    illegal phone number.
            std::vector<std::string> vecFailPhone;
    

            virtual void marshal(sox::Pack &p) const {                    
                p << strSequence;
                coding_vec(p,vecFailRes);
                coding_vec(p,vecFailPhone);            
            }

            virtual void unmarshal(const sox::Unpack &p){
                p >> strSequence;
                decode_vec(p,vecFailRes);
                decode_vec(p,vecFailPhone);                
            }
        };
         

        struct PAppSmBatchSendAsync:  public sox::Marshallable{
            enum {uri = (10 << 8) | protocol::SMS_BK_SVID};                

            std::string strSequence;
            std::string strSource;
            std::string strAppid;
            std::vector<std::string> vecPhone;

            std::string strContent; // utf8

            virtual void marshal(sox::Pack &p) const {                
                p << strSequence;
                p << strSource << strAppid;
                coding_vec(p,vecPhone);

                p << strContent;    
            }

            virtual void unmarshal(const sox::Unpack &p){                
                p >> strSequence;
                p >> strSource >> strAppid;
                decode_vec(p,vecPhone);
                p >> strContent;
            }
        };
         
        struct PGetSmsGatewayReq : public sox::Marshallable                                                    
          {                                                                                                   
              enum {uri = (107 << 8) | 120};                                                                  
                      
              std::string strSeq;                                                                                                 
              std::string service_id;                                                                                
              std::string operatorid;                                                                         
                                                                                                                                                      
              virtual void marshal(sox::Pack &pk) const {                                                      
                  pk << strSeq << service_id << operatorid;                  
              }                                                                                               
                                                                                                              
              virtual void unmarshal(const sox::Unpack &up) {                                                  
                  up >> strSeq >> service_id >> operatorid;                  
              }
          };

            struct PGetSmsGatewayRes : public sox::Marshallable                                                    
           {                                                                                                   
               enum {uri = (107 << 8) | 121};                                                                  
                       
               std::string strSeq;                                                                                                
               std::string strSmsGatewayNumber;                                           
                                                                                                                                                       
               virtual void marshal(sox::Pack &pk) const {                                                     
                   pk << strSeq << strSmsGatewayNumber;                  
               }                                                                                               
                                                                                                               
               virtual void unmarshal(const sox::Unpack &up) {                                                 
                   up >> strSeq >> strSmsGatewayNumber;                  
               }
           };          


    }
}


#endif
