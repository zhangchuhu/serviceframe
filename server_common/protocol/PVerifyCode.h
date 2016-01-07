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
            VT_REG = 1,   // 注册
            VT_PWD = 2,   // 改密
        };

        // 基本调用参数
        std::string   context;          // 上下文字段，调用方随意使用，会在返回时带上
        std::string   email_address;    //
        std::string   appid;            // 业务appid, 不能为空
        uint32_t      type;             // @see VerifyType , 必须是注册或改密

        // 短信参数
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
            expire_time(0),email_content(""),user_ip(0),
            caller_ip(0),appkey(""){}

        virtual void marshal( sox::Pack &pk) const
        {
            pk << context << email_address << appid << type;
            pk << expire_time << email_from << email_subject << email_content;
            pk << user_ip << caller_ip << appkey;
        }

        virtual void unmarshal( const sox::Unpack &up)
        {
            up >> context >> email_address >> appid >> type;
            up >> expire_time >> email_from >> email_subject >> email_content;
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
        std::string service_id;    // 业务ID
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
}};

namespace server{
namespace smscode{

    // protocol::SERVERLOGIN_SVID = 233

    // uri desc (uint32_t):
    //  [32-24] ... [24-8] ... [8-0] bits
    //    [32-24] :protocol type. 0:normal, 1:proprietary
    //    [24-8]  :service identity.
    //    [8-0]   :globe identity.  protocol::SERVERLOGIN_SVID

    struct RequestSmscode: public sox::Marshallable
    {
        enum { uri = ((1 << 24) | (81 << 8)) | protocol::SERVERLOGIN_SVID };

        enum SmscodeType
        {
            ST_DEF = 0,
            ST_REG = 1,   // 注册
            ST_PWD = 2,   // 改密
        };

        // 基本调用参数
        std::string   context;  // 上下文字段，调用方随意使用，会在返回时带上
        std::string   mobile;   // 用户手机号, 不能为空
        std::string   appid;    // 业务appid, 不能为空
        uint32_t      type;     // @see SmscodeType, 必须是注册或改密

        // 短信参数
        uint32_t      expire_time;  // 验证码有效期, 单位秒, 不能为0
        std::string   sms_content;  // 短信内容, 用 {{code}} 表示验证码
                                    // 例如: sms_content = "验证码:{{code}},请在网页输入进入下一步"
                                    // 用户收到的短信内容: 验证码:123456,请在网页输入进入下一步

        // 安全参数, 暂未启用
        uint32_t      user_ip;    // 用户 ip
        uint32_t      caller_ip;  // 调用者的 ip
        std::string   appkey;     // 对应 appid 的验证信息

        RequestSmscode():
            context(""),mobile(""),appid(""),type(0),
            expire_time(0),sms_content(""),
            user_ip(0),caller_ip(0),appkey(""){}

        virtual void marshal( sox::Pack &pk) const
        {
            pk << context << mobile << appid << type;
            pk << expire_time << sms_content;
            pk << user_ip << caller_ip << appkey;
        }

        virtual void unmarshal( const sox::Unpack &up)
        {
            up >> context >> mobile >> appid >> type;
            up >> expire_time >> sms_content;
            up >> user_ip >> caller_ip >> appkey;
        }
    };

    struct RequestSmscodeResp: public sox::Marshallable
    {
        enum { uri = ((1 << 24) | (82 << 8)) | protocol::SERVERLOGIN_SVID };

        enum RescodeType
        {
            RT_SUC = 0,           // 成功
            RT_EXISTENCE = 1,     // 手机号已经注册, 对应注册逻辑
            RT_NONEXISTENCE = 2,  // 手机号未注册, 对应改密逻辑
            RT_INVALID_ARGS = 3,  // 存在非法参数
            RT_REJ = 4,           // 拒绝请求.
                                  // 可能是 appkey 不正确、ip 被禁止等原因。暂未启用
            RT_SYSERR = 99        // 系统错误
        };

        //  基本调用信息, 和 RequestSmscode 一致
        std::string context;
        std::string mobile;
        std::string appid;
        uint32_t type;

        uint32_t rescode;   // @see RescodeType
        std::string errdesc;  // 详细的错误信息描述

        RequestSmscodeResp():
            context(""),mobile(""),appid(""),type(0),
            rescode(99),errdesc(""){}

        virtual void marshal( sox::Pack &pk) const
        {
            pk << context << mobile << appid << type;
            pk << rescode << errdesc;
        }

        virtual void unmarshal( const sox::Unpack &up)
        {
            up >> context >> mobile >> appid >> type ;
            up >> rescode >> errdesc;
        }
    };
}};
#endif
