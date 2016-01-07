#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/const.h"
#include "common/protocol/zip_tpl.h"
#include "protocol/CodeInfo.h"
#include "common/res_code.h"
#include "server_common/protocol/login_udb_v2_i.h"

using namespace sox;

// 组外使用，需将以下宏:_INTERFACE_USER定义打开
// #define _INTERFACE_USER
#ifndef _INTERFACE_USER
#include "server_login/common/resource/login_uri.h"
#endif // _INTERFACE_USER

using namespace std;

#ifndef _login_rules_v2_2015_6_3
#define _login_rules_v2_2015_6_3

#ifdef _INTERFACE_USER
#endif // _INTERFACE_USER

// 以下取值请不要随意改动，需要与udb保持一致
#define URI_VALUE(mud_id, mud_uri, svid) ((mud_uri&0xFF) << 24)|((mud_id&0xFFFF) << 8)|(svid&0xFF)

#define LM_LOGIN_POLICY 102

enum LOGIN_RULES_URI
{
    LG_URI_RULES2_RULES_REQ = URI_VALUE(LM_LOGIN_POLICY, 3, protocol::SERVERLOGIN_SVID),
    LG_URI_RULES2_RULES_RES = URI_VALUE(LM_LOGIN_POLICY, 4, protocol::SERVERLOGIN_SVID),
    LG_URI_RULES2_LOGIN_RESULT = URI_VALUE(LM_LOGIN_POLICY, 5, protocol::SERVERLOGIN_SVID),
};


namespace server{namespace rules2{

    struct proto_userinfo
    {
        uint64_t     _yyuid;
        uint64_t     _yyid;
        std::string  _passport;
        std::string  _mobile;
        std::string  _email;
        uint32_t     _login_name_type;
        uint32_t     _user_status;
        uint32_t     _token_type;    // 1 - 凭证, 2 - 帐号/密码, 3 - ticket票据, 4 - acctinfo
        std::string  _token;
        bool         _passwd_ok;
    };
    inline Pack & operator << (Pack & p, const proto_userinfo& r)
    {
        sox::PackBuffer pb;
        sox::Pack pk(pb);
        pk << r._yyuid << r._yyid << r._passport << r._mobile << r._email << r._login_name_type << r._user_status << r._token_type << r._token << r._passwd_ok;
        p.push_varstr(pk.data(), pk.size());
        return p;
    }
    inline const Unpack & operator >> (const Unpack & p, proto_userinfo& r)
    {
        std::string str = p.pop_varstr();
        if (str.empty())
        {
            return p;
        }
        sox::Unpack up(str.c_str(), str.size());
        up >> r._yyuid >> r._yyid >> r._passport >>  r._mobile >> r._email >> r._login_name_type >> r._user_status >> r._token_type >> r._token >> r._passwd_ok;
        return p;
    }

    /*查询验证策略:包括静态和动态*/
    struct proto_rules_req:public sox::Marshallable
    {
        enum{uri=LG_URI_RULES2_RULES_REQ};
        uint32_t       _proto_ver;     // 协议版本号，方便协议升级
        std::string    _context;       // 消息序列号
        server::loginUDB::proto_header   _proto_hdr;     // 公用协议头
        proto_userinfo _proto_userinfo;   // 用户登陆信息

        proto_rules_req():_proto_ver(0){}
        virtual void marshal( sox::Pack &pk )const
        {
            pk << _proto_ver << _context << _proto_hdr << _proto_userinfo ;
        }
        virtual void unmarshal(const sox::Unpack &up)
        {
            up >> _proto_ver >> _context >> _proto_hdr >> _proto_userinfo ;
        }
    };

    enum NEXT_VERIFY_STRATEGY
    {
        RULES_NONE     = 0x0,      // 无动态策略
        RULES_PICCODE  = 0x01,     // 图片验证码
        RULES_MOBTOKEN = 0x02,     // 手机令牌
        RULES_HWTOKEN  = 0x04,     // 硬件令牌
        RULES_SMSCODE  = 0x08,     // 短信验证码
    };

    enum REJECT_STRATEGY
    {
        RULES_REJECT_NONE     = 0,      // 无
        RULES_FREEZE   = 1,      // 临时冻结
    };

    struct proto_rules_res: public sox::Marshallable
    {
        enum{uri=LG_URI_RULES2_RULES_RES};
        enum ENU_RESCODE
        {
            RULES_SUCCESS                = 0,       // 没有策略
            RULES_NEED_NEXT_VERIFY       = 1,       // 需要进一步验证（可能包括：短信验证码、手机令牌、硬件令牌）
            RULES_REJECT_LOGIN           = 2,       // 拒绝登陆
            RULES_SYSTEM_ERR             = 99,      // 重试
            RULES_PARAM_ERR              = 100      // 参数错误
        };

        uint32_t     _proto_ver;          // 协议版本号，方便协议升级
        std::string  _context;            // 消息序列号
        uint32_t     _rescode;            // 返回错误码，见ENU_RESCODE枚举值
        std::string  _errmsg;             // 错误信息
        std::string  _reason;             // jason格式，包括了每个策略掩码的具体原因{dynamic_strategy:{"smscode":"asdfad","mobtoken":"asdf"}, static_strategy:{"piccode":"adsfads" "reject":"adfad"}}
        uint32_t     _strategy;           // 当rescode = RULES_NEED_NEXT_VERIFY（需要进一步验证）的登录策略选项，bit位组合，枚举值见：NEXT_VERIFY_STRATEGY
                                          // 当rescode = RULES_REJECT_LOGIN禁止登陆 ，禁止原因值见：REJECT_STRATEGY
        proto_rules_res():_proto_ver(0), _rescode(0),_strategy(0){}
        virtual void marshal( sox::Pack &pk )const
        {
            pk << _proto_ver << _context << _rescode << _errmsg << _reason << _strategy;
        }
        virtual void unmarshal(const sox::Unpack &up)
        {
            up >> _proto_ver >> _context >> _rescode >> _errmsg >> _reason >> _strategy;
        }
    };

    /*验证失败通知消息*/
    struct proto_login_result: public sox::Marshallable
    {
        enum{uri=LG_URI_RULES2_LOGIN_RESULT};
        uint32_t        _proto_ver;     // 协议版本号，方便协议升级
        std::string     _context;       // 消息序列号
        server::loginUDB::proto_header   _proto_hdr;        // 公用协议头
        proto_userinfo  _proto_userinfo;   // 用户登陆信息
        uint32_t        _login_rescode;   // 登陆结果
        uint32_t        _rules_rescode;   // 策略返回结果
        uint32_t        _strategy;        // 策略内容

        proto_login_result():_proto_ver(),_login_rescode(0),_rules_rescode(0),_strategy(0){}
        virtual void marshal(sox::Pack &pk) const
        {
            pk << _proto_ver << _context << _proto_hdr << _proto_userinfo << _login_rescode << _rules_rescode << _strategy;
        }
        virtual void unmarshal(const sox::Unpack &up)
        {
            up >> _proto_ver >> _context >> _proto_hdr >> _proto_userinfo >> _login_rescode >> _rules_rescode >> _strategy;
        }
    };


}}

#endif //_verificationStrategy_i_H_2014_3_5
