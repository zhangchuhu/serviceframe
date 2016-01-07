#ifndef _PTY_PROTOCOL_EMAILVERIFY_H_ZHANGXU_20131217
#define _PTY_PROTOCOL_EMAILVERIFY_H_ZHANGXU_20131217

#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"

namespace server{
namespace emailverify{

    // protocol::SERVERLOGIN_SVID = 233

    // uri desc (uint32_t):
    //  [32-24] ... [24-8] ... [8-0] bits
    //    [32-24] :protocol type. 0:normal, 1:proprietary
    //    [24-8]  :service identity.
    //    [8-0]   :globe identity.  protocol::SERVERLOGIN_SVID

    struct RequestEmailCode: public sox::Marshallable
    {
        enum { uri = ((1 << 24) | (91 << 8)) | protocol::SERVERLOGIN_SVID };

        enum VerifyType
        {
            VT_DEF = 0,
            VT_REG = 1,   // 注册, 验证的时候对应的service_id是: "[appid]@reg"
            VT_PWD = 2,   // 改密, 对应: "[appid]@pwd"
			VT_BIND = 3,  // 绑定, 对应: "[appid]@bind"
        };

        // 基本调用参数
        std::string   context;          // 上下文字段，调用方随意使用，会在返回时带上
        std::string   email_address;    //
        std::string   appid;            // 业务appid, 不能为空
        uint32_t      type;             // @see VerifyType , 必须是注册或改密

        // email参数
        uint32_t      expire_time;      // 验证码有效期, 单位秒, 不能为0
        std::string   email_from;       // email 的from字段
        std::string   email_subject;    // email 的主题
        std::string   email_content;    // 短信内容, 用 {{code}} 表示验证码
                                        // 例如: email_content = "验证码:{{code}},请在网页输入进入下一步"
                                        // 用户收到的email内容: 验证码:123456,请在网页输入进入下一步

        // 安全参数, 暂未启用
        uint32_t      user_ip;    // 用户 ip
        uint32_t      caller_ip;  // 调用者的 ip
        std::string   appkey;     // 对应 appid 的验证信息

        RequestEmailCode():
            context(""),email_address(""),appid(""),type(0),
            expire_time(0),email_from(""),email_subject(""),
            email_content(""),user_ip(0),caller_ip(0),appkey(""){}

        virtual void marshal( sox::Pack &pk) const
        {
            pk << context << email_address << appid << type;
            pk << expire_time << email_from;
            pk << email_subject << email_content;
            pk << user_ip << caller_ip << appkey;
        }

        virtual void unmarshal( const sox::Unpack &up)
        {
            up >> context >> email_address >> appid >> type;
            up >> expire_time >> email_from;
            up >> email_subject >> email_content;
            up >> user_ip >> caller_ip >> appkey;
        }
    };

    struct RequestEmailCodeResp: public sox::Marshallable
    {
        enum { uri = ((1 << 24) | (92 << 8)) | protocol::SERVERLOGIN_SVID };

        enum RescodeType
        {
            RT_SUC = 0,           // 成功
            RT_EXISTENCE = 1,     // email已经注册, 对应注册逻辑
            RT_NONEXISTENCE = 2,  // email未注册, 对应改密逻辑
            RT_INVALID_ARGS = 3,  // 存在非法参数
            RT_REJ = 4,           // 拒绝请求.
                                  // 可能是 appkey 不正确、ip 被禁止等原因。暂未启用
            RT_SYSERR = 99        // 系统错误
        };

        //  基本调用信息, 和 RequestEmailCode 一致
        std::string context;
        std::string email_address;
        std::string appid;
        uint32_t type;

        uint32_t rescode;   // @see RescodeType
        std::string errdesc;  // 详细的错误信息描述

        RequestEmailCodeResp():
            context(""),email_address(""),appid(""),type(0),
            rescode(99),errdesc(""){}

        virtual void marshal( sox::Pack &pk) const
        {
            pk << context << email_address << appid << type;
            pk << rescode << errdesc;
        }

        virtual void unmarshal( const sox::Unpack &up)
        {
            up >> context >> email_address >> appid >> type ;
            up >> rescode >> errdesc;
        }
    };


    struct VerifyEmailCode: public sox::Marshallable
    {
        enum { uri = ((1 << 24) | (93 << 8)) | protocol::SERVERLOGIN_SVID };

        std::string context;    // 上下文
        std::string email;        // email
        std::string service_id;    // 业务ID, 如果之前是使用RequestEmailCode来请求验证码的话请注意VerifyType的说明
        std::string code;        // 验证码

        std::string carry_data;     // 会在返回中回传给调用者

        virtual void marshal(sox::Pack &pk) const
        {
            pk << context << email << service_id << code;
            pk << carry_data;
        }

        virtual void unmarshal(const sox::Unpack &up)
        {
            up >> context >> email >> service_id >> code;
            up >> carry_data;
        }
    };

    struct VerifyEmailCodeResp: public sox::Marshallable
    {
        enum { uri = ((1 << 24) | (94 << 8)) | protocol::SERVERLOGIN_SVID };

        std::string context;    // 上下文
        std::string email;        // 手机号
        std::string service_id;    // 业务ID
        uint32_t rescode;        // 结果码，
                                    // 0：成功，
                                    // 1：验证验证码失败，
                                    // 2：参数无效，
                                    // 3：系统错误
                                    
        std::string strerror;    // 结果码描述

        uint32_t fail_desc;     // 描述验证失败的详细原因码
        //    0, 成功
        //    1, 验证码不存在（未下发过验证码）
        //    2, 验证码超时
        //    3, 验证码错误
        //    4, 验证错误次数大于3次
        //    99, 系统错误

        std::string carry_data;

        virtual void marshal(sox::Pack &pk) const
        {
            pk << context << email << service_id;
            pk << rescode << strerror << fail_desc;
            pk << carry_data;
        }

        virtual void unmarshal(const sox::Unpack &up)
        {
            up >> context >> email >> service_id;
            up >> rescode >> strerror >> fail_desc;
            up >> carry_data;
        }
    };

    struct CommRequestEmailCode: public sox::Marshallable
    {
        enum { uri = ((1 << 24) | (95 << 8)) | protocol::SERVERLOGIN_SVID };


        // 基本调用参数
        std::string   context;          // 上下文字段，调用方随意使用，会在返回时带上
        std::string   email;            // 通过email
        uint64_t      yyuid;            // 通过yyuid, email和yyuid不能同时为空
        std::string   appid;            // 业务appid, 不能为空
        std::string   service_id;       //子业务编号

        // email参数
        uint32_t      expire_time;      // 验证码有效期, 单位秒, 不能为0
        std::string   email_from;       // email 的from字段
        std::string   email_subject;    // email 的主题
        std::string   email_content;    // 短信内容, 用 {{code}} 表示验证码
                                        // 例如: email_content = "验证码:{{code}},请在网页输入进入下一步"
                                        // 用户收到的email内容: 验证码:123456,请在网页输入进入下一步


        CommRequestEmailCode():
            context(""),email(""),yyuid(0), appid(""),service_id(""),
            expire_time(0),email_from(""),email_subject(""),
            email_content("") {}

        virtual void marshal( sox::Pack &pk) const
        {
            pk << context << email << yyuid << appid << service_id;
            pk << expire_time << email_from;
            pk << email_subject << email_content;
        }

        virtual void unmarshal( const sox::Unpack &up)
        {
            up >> context >> email >> yyuid >> appid >> service_id;
            up >> expire_time >> email_from;
            up >> email_subject >> email_content;
            
        }
    };

    struct CommRequestEmailCodeResp: public sox::Marshallable
    {
        enum { uri = ((1 << 24) | (96 << 8)) | protocol::SERVERLOGIN_SVID };

        enum RescodeType
        {
            RT_SUC = 0,           // 成功
            RT_PARAM_ERR = 2,     // 参数非法，uid和email同时为空,缺少appid等
            RT_USER_NO_EXIST = 3,   // email或yyuid对应的用户不存在
            RT_USER_NOT_BIND_EMAIL = 4,   // yyuid对应的用户没有绑定email
            RT_REJ = 5,           // 拒绝请求.
            RT_SYSERR = 99        // 系统错误
        };

        //  基本调用信息, 和 RequestEmailCode 一致
        std::string  context;
        std::string  email;
        uint64_t     yyuid;
        std::string  appid;
        std::string  service_id;
        uint32_t     rescode;   // @see RescodeType
        std::string  errdesc;  // 详细的错误信息描述

        CommRequestEmailCodeResp():
            context(""),email(""),yyuid(0),appid(""),service_id(""),
            rescode(99),errdesc(""){}

        virtual void marshal( sox::Pack &pk) const
        {
            pk << context << email << yyuid << appid << service_id ;
            pk << rescode << errdesc;
        }

        virtual void unmarshal( const sox::Unpack &up)
        {
            up >> context >> email >> yyuid >> appid >> service_id;
            up >> rescode >> errdesc;
        }
    };


    struct CommVerifyEmailCode: public sox::Marshallable
    {
        enum { uri = ((1 << 24) | (97 << 8)) | protocol::SERVERLOGIN_SVID };

        std::string context;    // 上下文
        std::string email;        // email
        uint64_t    yyuid;        // email
        std::string appid;    // 业务ID, 如果之前是使用RequestEmailCode来请求验证码的话请注意VerifyType的说明
        std::string service_id; //子业务ID
        std::string code;        // 验证码

        std::string carry_data;     // 会在返回中回传给调用者
        CommVerifyEmailCode():
            context(""),email(""),yyuid(0),appid(""),service_id(""),
            code(""),carry_data(""){}
            
        virtual void marshal(sox::Pack &pk) const
        {
            pk << context << email << yyuid << appid << service_id << code ;
            pk << carry_data;
        }

        virtual void unmarshal(const sox::Unpack &up)
        {
            up >> context >> email >> yyuid >> appid >> service_id >> code;
            up >> carry_data;
        }
    };

    struct CommVerifyEmailCodeResp: public sox::Marshallable
    {
        enum { uri = ((1 << 24) | (98 << 8)) | protocol::SERVERLOGIN_SVID };

        std::string context;    // 上下文
        std::string email;        // 用户邮箱它与yyuid不能同时为空
        uint64_t    yyuid;      //yyuid
        std::string appid;    // 业务ID
        std::string service_id; //子业务ID
        uint32_t  rescode;        // 结果码，
        //    0, 成功
        //    1, 验证码不存在（未下发过验证码）
        //    2, 验证码超时
        //    3, 验证码错误
        //    4, 验证错误次数大于3次
        //    98 参数错误
        //    99 系统错误
                                    
        std::string strerror; // 结果码描述
        std::string carry_data;
        CommVerifyEmailCodeResp():
            context(""),email(""),yyuid(0),appid(""),service_id(""),
            rescode(99),strerror(""),carry_data("") {}

        virtual void marshal(sox::Pack &pk) const
        {
            pk << context << email << yyuid << appid << service_id;
            pk << rescode << strerror << carry_data;
        }

        virtual void unmarshal(const sox::Unpack &up)
        {
            up >> context >> email >> yyuid >> appid >> service_id;
            up >> rescode >> strerror ;
            up >> carry_data;
        }
    };
    
    // PING
    struct PingEmailVerify: public sox::Marshallable
    {
        enum {uri = 50<<8|110};

        uint32_t  u;

        PingEmailVerify():u(0){}
        virtual void marshal(sox::Pack &p) const
        {
            p << u;
        }

        virtual void unmarshal(const sox::Unpack &p)
        {
            p >> u;
        }
    };

    struct PingEmailVerifyResp: public sox::Marshallable
    {
        enum {uri = 51<<8|110};

        uint32_t  u;

        PingEmailVerifyResp():u(0){}
        virtual void marshal(sox::Pack &p) const
        {
            p << u;
        }

        virtual void unmarshal(const sox::Unpack &p)
        {
            p >> u;
        }
    };
}};
#endif
