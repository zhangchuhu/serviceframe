#ifndef _PSMSCODE_H_ZHANGXU_12_19_2012
#define _PSMSCODE_H_ZHANGXU_12_19_2012

#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"

namespace server{
	namespace smscode{
	
		// protocol::SERVERLOGIN_SVID = 233
		
		struct SendMsgReq: public sox::Marshallable
		{
			enum {uri = (81 << 8) | protocol::SERVERLOGIN_SVID};			

			std::string context;	// 上下文
			std::string mobile;		// 手机号,需要使用uid的话这个置空就行了。
			std::string service_id;	// 业务ID
			uint32_t valid_time;	// 有效期,单位秒
			std::string tag;		// 短信定制内容，见注1
			std::string	data;		// 回调数据
			std::string extra;		// 目前无用
			
			std::string carry_data; // 携带数据,会在返回中带上,和context类似,但是不会打印到日志
            
            uint32_t uid;   // uid，如果mobile为空，则使用uid作为入参
			
			SendMsgReq():mobile(""),uid(0){}

			virtual void marshal(sox::Pack &pk) const {					
				pk << context << mobile << service_id << valid_time << tag << data << extra << carry_data << uid;
			}
			
			

			virtual void unmarshal(const sox::Unpack &up){
				up >> context >> mobile >> service_id >> valid_time >> tag >> data >> extra >> carry_data;
				if( !up.empty())
				{
				    up >> uid;
				}
			}
		};
		// 注1：如果tag参数非空，那么用户收到的短信内容就是tag中的内容，如果tag中有通配符"{code}"，"xxxx{code}xxxx"，用户收到的就是"xxxx{123456}xxxx"；
		//如果tag中无通配符{code}，如tag="您的验证码为:" 那用户将收到短信内容为"您的验证码为:123456"，即系统会将验证码追加在末尾
        
        //SendMsgReq接口的响应报文
		struct SendMsgRes: public sox::Marshallable
		{
			enum {uri = (82 << 8) | protocol::SERVERLOGIN_SVID};			

			std::string context;	// 上下文
			std::string mobile;		// 手机号
			uint32_t rescode;		// 结果码，0：成功，1：参数无效，2：系统错误，3：请求下发短信失败
			std::string strerror;	// 结果码描述
			
			std::string carry_data; // 携带数据,见SendMsgReq

            uint32_t uid;	
            uint32_t fail_desc;     // 描述失败的详细原因码
									//	0, 成功
									//	1, 用户不存在
									//	2, 系统内没有记录该用户的手机号
									//	3, 特定业务下，每个手机下发的短信数量超过了限制(如10条)

			virtual void marshal(sox::Pack &pk) const {					
				pk << context << mobile << rescode << strerror << carry_data << uid << fail_desc;
			}

			virtual void unmarshal(const sox::Unpack &up){
				up >> context >> mobile >> rescode >> strerror >> carry_data; 
				if( !up.empty())
				{
                    up >> uid;
				}
				if( !up.empty())
				{
                    up >> fail_desc;
				}
			}
		};
        //请求验证短信验证码
		struct VerifyCodeReq: public sox::Marshallable
		{
			enum {uri = (83 << 8) | protocol::SERVERLOGIN_SVID};			

			std::string context;	// 上下文
			std::string mobile;		// 手机号,需要使用uid的话这个置空就行了。
			std::string service_id;	// 业务ID
			std::string code;		// 验证码
			
			std::string carry_data; // 携带数据,会在返回中带上,和context类似,但是不会打印到日志

            uint32_t uid;  // uid，如果mobile为空，则使用uid作为入参

			VerifyCodeReq():mobile(""),uid(0){}
			
			virtual void marshal(sox::Pack &pk) const {					
				pk << context << mobile << service_id << code << carry_data << uid;
			}

			virtual void unmarshal(const sox::Unpack &up){
				up >> context >> mobile >> service_id >> code >> carry_data; 
				if( !up.empty())
				{
                    up >> uid;
				}
			}
		};
        //请求验证短信验证码(VerifyCodeReq)的响应报文
		struct VerifyCodeRes: public sox::Marshallable
		{
			enum {uri = (84 << 8) | protocol::SERVERLOGIN_SVID};			

			std::string context;	// 上下文
			std::string mobile;		// 手机号
			std::string service_id;	// 业务ID
			std::string data;		// 回调数据
			uint32_t rescode;		// 结果码，0：成功，1：验证验证码失败，2：参数无效，3：系统错误
			                        // 
			std::string strerror;	// 结果码描述

			std::string carry_data; // 携带数据,VerifyCodeReq

            uint32_t fail_desc;     // 描述验证失败的详细原因码 两个错误码，仅会出现在手机安全中心。其它业务中不会出现。
									//	0, 成功
									//	1, 验证码不存在（未下发过验证码）
									//	2, 验证码超时(过期失效)
									//	3, 验证码错误
									//	4, 用户不存在
									//	5, 系统内没有记录该用户的手机号
									//  6, 验证码验证次数超过限制
									//	99, 系统错误

            uint32_t uid;
			
			virtual void marshal(sox::Pack &pk) const {					
				pk << context << mobile << service_id << data << rescode << strerror << carry_data << fail_desc << uid;
			}

			virtual void unmarshal(const sox::Unpack &up){
				up >> context >> mobile >> service_id >> data >> rescode >> strerror >> carry_data >> fail_desc; 
				if( !up.empty())
				{
				    up >> uid;
				}
			}
		};
		//申请产生短信验证码，由调用者自行下发短信。验证码服务将保存回调数据，验证成功后将返回
		struct UserSendMsgReq: public sox::Marshallable
		{
			enum {uri = (85 << 8) | protocol::SERVERLOGIN_SVID};			

			std::string context;		// 上下文
			std::string mobile;			// 手机号
			std::string service_id;		// 业务ID
			uint32_t valid_time;		// 有效期,单位秒
			std::string callback_svid;	// 服务名sid,用于发送回调消息
			uint32_t callback_groupid;		// 服务对应的groupid,填0的话则是全网
			std::string	callback_data;	// 回调数据,会在用户成功之后回调给业务系统
			std::string extra;
			
			std::string carry_data; // 携带数据,会在返回中带上,和context类似,但是不会打印到日志

			virtual void marshal(sox::Pack &pk) const {					
				pk << context << mobile << service_id << valid_time << callback_svid << callback_groupid << callback_data << extra;
			}

			virtual void unmarshal(const sox::Unpack &up){
				up >> context >> mobile >> service_id >> valid_time >> callback_svid >> callback_groupid >> callback_data >> extra;
			}
		};
		//UserSendMsgReq的响应报文
		struct UserSendMsgRes: public sox::Marshallable
		{
			enum {uri = (86 << 8) | protocol::SERVERLOGIN_SVID};			

			std::string context;	// 上下文
			std::string mobile;		// 手机号
			std::string smscode;	// 验证码，需要用户发送的
			uint32_t rescode;		// 结果码: 0(成功),1(系统错误)
			std::string strerror;
			
			std::string carry_data; // 携带数据,见UserSendMsgReq

			virtual void marshal(sox::Pack &pk) const {					
				pk << context << mobile << smscode << rescode << strerror;
			}

			virtual void unmarshal(const sox::Unpack &up){
				up >> context >> mobile >> smscode >> rescode >> strerror; 
			}
		};
		//从MQ中获取需要验证的验证码，并且在验证成功时,返回UserSendMsgReq中提交的回调数据
		struct UserSendMsgCallback: public sox::Marshallable
		{
			enum {uri = (87 << 8) | protocol::SERVERLOGIN_SVID};			

			std::string context;	// 上下文
			std::string mobile;		// 手机号
			uint32_t rescode;		// 验证结果码：0(成功),1(验证码错误),2(系统错误)
			std::string strerror;
			
			std::string callback_data; // 回调数据,会在用户成功之后回调给业务系统

			virtual void marshal(sox::Pack &pk) const {					
				pk << context << mobile << callback_data << rescode << strerror;
			}

			virtual void unmarshal(const sox::Unpack &up){
				up >> context >> mobile >> callback_data >> rescode >> strerror; 
			}
		};
		
		// 由短信网关从MQ获取用户的上行短信后,发送此请求给smscode_d来生成验证码验证请求
		struct SimpleSmsReq: public sox::Marshallable
		{
			enum {uri = (88 << 8) | protocol::SERVERLOGIN_SVID};			

			std::string text;		// MQ短信内容,Json

			virtual void marshal(sox::Pack &pk) const {					
				pk << text;
			}

			virtual void unmarshal(const sox::Unpack &up){
				up >> text; 
			}
		};
        //预验证接口，可用于测试性验证，验证成功不会销毁数据，验证失败会报错
    	struct TestVerifyCodeReq: public sox::Marshallable
    	{
    		enum {uri = (89 << 8) | protocol::SERVERLOGIN_SVID};			
    
    		std::string context;	// 上下文
    		std::string mobile;		// 手机号,需要使用uid的话这个置空就行了。
    		std::string service_id;	// 子业务ID
    		std::string code;		// 验证码
    		
    		std::string carry_data; // 携带数据,会在返回中带上,和context类似,但是不会打印到日志
    
            uint32_t uid;  // uid，如果mobile为空，则使用uid作为入参
    
    		TestVerifyCodeReq():mobile(""),uid(0){}
    		
    		virtual void marshal(sox::Pack &pk) const {					
    			pk << context  << mobile << service_id << code << carry_data << uid;
    		}
    
    		virtual void unmarshal(const sox::Unpack &up){
    			up >> context >> mobile >> service_id >> code >> carry_data; 
    			if( !up.empty())
    			{
                    up >> uid;
    			}
    		}
    		
    	};
	}
	
};

#endif
