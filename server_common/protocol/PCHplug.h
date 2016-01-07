#pragma once

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"
#include <vector>
#include <string>
#include <map>
#include <pthread.h>
#include <semaphore.h>
namespace server{
	namespace plug{
    
    /*ͬ���ӿ�*/
		struct PCHplugaddReq:public sox::Marshallable
		{
			   enum {uri = 1 << 8 | protocol::UDB_SVID};
            
			   uint32_t uid;
			   uint32_t reason;  //ԭ��ö��ֵ
         std::string info; //˵��
         uint32_t exp_time;//�����ù���ʱ��ʱ��0
			
			   virtual void marshal(sox::Pack &pk) const
			   {
				     pk << uid << reason << info << exp_time;
			   }
			   virtual void unmarshal(const sox::Unpack &up)
			   {
				     up >> uid >> reason >> info >> exp_time;
			   }
		};

		struct PCHplugdelReq:public sox::Marshallable
		{
		    enum {uri = 2 << 8 | protocol::UDB_SVID};
        
	      uint32_t uid;
	      std::string user;//�ӿ�ʹ���ߣ����Լ���ģ�����ƾͺ���

			
			  virtual void marshal(sox::Pack &pk) const
			  {
				    pk <<  uid  << user ; 
			  }
			  virtual void unmarshal(const sox::Unpack &up)
			  {
				    up >> uid   >> user;
			  }
		};		
		
		
		/*�����ӿ�*/

		struct PCHplugaddlistReq:public sox::Marshallable
		{
			   enum {uri = 4 << 8 | protocol::UDB_SVID};
            
			   std::vector<uint32_t> uids;
			   std::vector<uint32_t> reasons;
			   std::vector<std::string> infos; //��ʼ���ر�״̬ʱ��0��������״̬��ֵ֮ͬ���ʹ��|�Ÿ���
         std::vector<uint32_t> exp_times; //�����ù���ʱ��ʱ��0
         
			
			   virtual void marshal(sox::Pack &pk) const
			   {
			   	   sox::marshal_container(pk, uids);
			   	   sox::marshal_container(pk, reasons);
			   	   sox::marshal_container(pk, infos);
			   	   sox::marshal_container(pk, exp_times);
				     
			   }
			   virtual void unmarshal(const sox::Unpack &up)
			   {
				     sox::unmarshal_container(up, std::back_inserter(uids));
				     sox::unmarshal_container(up, std::back_inserter(reasons));
                                     sox::unmarshal_container(up, std::back_inserter(infos));
				     sox::unmarshal_container(up, std::back_inserter(exp_times));
			   }
		};

		struct PCHplugdellistReq:public sox::Marshallable
		{
		    enum {uri = 8 << 8 | protocol::UDB_SVID};
        
	      std::vector<uint32_t> uids;
	      std::string user;//�ӿ�ʹ���ߣ����Լ���ģ�����ƾͺ���

			
			  virtual void marshal(sox::Pack &pk) const
			  {
			  	  sox::marshal_container(pk, uids);
				    pk << user ; 
			  }
			  virtual void unmarshal(const sox::Unpack &up)
			  {
			  	  sox::unmarshal_container(up, std::back_inserter(uids));
				    up >> user;
			  }
		};		
		
		
               struct PCHreasonaddReq:public sox::Marshallable
               {
                   enum {uri = 16 << 8 | protocol::UDB_SVID};
                   std::string key;
                   std::string value;

                   virtual void marshal(sox::Pack &pk) const
                   {
                       pk << key << value ;
                   }
                   
                   virtual void unmarshal(const sox::Unpack &up)
                   { 
                       up >> key >> value;
                   }
               };				

                struct PCHIpaddReq:public sox::Marshallable
                {
                           enum {uri = 32 << 8 | protocol::UDB_SVID};
                           
                           uint32_t ip;
                           uint32_t topSid;
                           uint32_t sid;
                           uint32_t exp_time;

                           virtual void marshal(sox::Pack &pk) const
                           {
                                     pk << ip << topSid << sid << exp_time;
                           }
                           virtual void unmarshal(const sox::Unpack &up)
                           {
                                     up >> ip >> topSid >> sid >> exp_time;
                           }
                };

	}
}
