#ifndef YYPROTOCOL_H
#define YYPROTOCOL_H

#include <iostream>

#include "common/packet.h"
#include "protocol/const.h"

#define URI_VC_VALUE(mud_id, mud_uri, svid) ((mud_uri & 0xFF) << 24) | ((mud_id & 0xFFFF) << 8) | (svid & 0xFF)
#define ERR_VC_VALUE(mud_id, errno) ((mud_id & 0xFFFF) << 16) | errno

enum VCRES_ERRCODE
{
		VCRSE_SUCCESS  = 0,  // 成功
		// 1 - 100 段分配给系统错误
		VCRSE_EXIST_ERR = ERR_VC_VALUE(116, 1),     // 账号不存在
		VCRSE_BIND_ERR = ERR_VC_VALUE(116, 2),      // 账号无绑定
		VCRSE_INTERNAL_ERR = ERR_VC_VALUE(116, 3),  // 标识错误
		VCRSE_CODE_ERR = ERR_VC_VALUE(116, 4),      // 验证码错误
		VCRSE_OVERDUE_ERR = ERR_VC_VALUE(116, 5),   // 验证码已失效（包括时间过期和验证次数达到上限，需要重新获取）
		VCRSE_LIMIT_ERR = ERR_VC_VALUE(116, 6),     // 下发次数上限
		VCRSE_AUTH_ERR = ERR_VC_VALUE(116, 7),      // 授权信息错误
		VCRSE_PARAM_ERR = ERR_VC_VALUE(116, 8),     // 参数错误
		VCRSE_SYSTEM_ERR = ERR_VC_VALUE(116, 99),   // 系统错误，需要重试
};

enum AccountType
{
		AT_YYUID = 0,    // 如果使用yyuid，服务会去查询账号所绑定的邮箱或者密保手机
		AT_EMAIL = 1,
		AT_MOBILE = 2,
};

struct RequestEmailCode: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 1, 233)};
		
		std::string _context;     // 上下文
		std::string _appid;       // 业务标识
		std::string _service_id;  // 业务子标识
		uint8_t     _type;        // AT_YYUID/AT_EMAIL
		std::string _account;     // yyuid/email
		uint32_t    _expire_time; // 有效时间（单位：秒，不能超过24小时，建议不超过10分钟）
		std::string _email_info;  // 下发文案，内容为json格式：{"from":"","subject":"","content":""}，content中，使用{code}来标识生成的验证码,使用{passport}来标识通行证掩码；
		std::string _appkey;      // 验证信息
		std::string _extra;       // 预留字段
		
		RequestEmailCode(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _appid << _service_id << _type << _account << _expire_time << _email_info << _appkey << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _appid >> _service_id >> _type >> _account >> _expire_time >> _email_info >> _appkey >> _extra;
		}
};

struct RequestEmailCodeResp: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 2, 233)};
		
		std::string _context;     // 上下文
		uint32_t    _rescode;     // 结果码，参考VCRES_ERRCODE
		std::string _errinfo;     // 结果描述
		std::string _extra;       // 预留字段
		
		RequestEmailCodeResp(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _rescode << _errinfo << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _rescode >> _errinfo >> _extra;
		}
};

struct VerifyEmailCode: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 3, 233)};
		
		std::string _context;     // 上下文
		std::string _appid;       // 业务标识
		std::string _service_id;  // 业务子标识
		uint8_t     _type;        // AT_YYUID/AT_EMAIL
		std::string _account;     // yyuid/email
		std::string _code;        // 验证码
		std::string _appkey;      // 验证信息
		std::string _extra;       // 预留字段
		
		VerifyEmailCode(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _appid << _service_id << _type << _account << _code << _appkey << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _appid >> _service_id >> _type >> _account >> _code >> _appkey >> _extra;
		}
};

struct VerifyEmailCodeResp: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 4, 233)};
		
		std::string _context;     // 上下文
		uint32_t    _rescode;     // 结果码，参考VCRES_ERRCODE
		std::string _errinfo;     // 结果描述
		std::string _extra;       // 预留字段
		
		VerifyEmailCodeResp(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _rescode << _errinfo << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _rescode >> _errinfo >> _extra;
		}
};

struct RequestSmsCode: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 11, 233)};
		
		std::string _context;     // 上下文
		std::string _appid;       // 业务标识
		std::string _service_id;  // 业务子标识
		uint8_t     _type;        // AT_YYUID/AT_MOBILE
		std::string _account;     // yyuid/mobile
		uint32_t    _expire_time; // 有效时间（单位：秒，不能超过24小时，建议不超过10分钟）	
		std::string _sms_info;    // 下发文案，content中，使用{code}来标识生成的验证码,使用{passport}来标识通行证掩码；如果值为"[template]"字符串，则为定制文案，定制文案需提前跟服务方定制
		std::string _appkey;      // 验证信息
		std::string _extra;       // 预留字段
		
		RequestSmsCode(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _appid << _service_id << _type << _account << _expire_time << _sms_info << _appkey << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _appid >> _service_id >> _type >> _account >> _expire_time >> _sms_info >> _appkey >> _extra;
		}
};

struct RequestSmsCodeResp: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 12, 233)};
		
		std::string _context;     // 上下文
		uint32_t    _rescode;     // 结果码，参考VCRES_ERRCODE
		std::string _errinfo;     // 结果描述
		std::string _extra;       // 预留字段
		
		RequestSmsCodeResp(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _rescode << _errinfo << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _rescode >> _errinfo >> _extra;
		}
};

struct VerifySmsCode: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 13, 233)};
		
		std::string _context;     // 上下文
		std::string _appid;       // 业务标识
		std::string _service_id;  // 业务子标识
		uint8_t     _type;        // AT_YYUID/AT_MOBILE
		std::string _account;     // yyuid/mobile
		std::string _code;        // 验证码
		std::string _appkey;      // 验证信息
		std::string _extra;       // 预留字段
		
		VerifySmsCode(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _appid << _service_id << _type << _account << _code << _appkey << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _appid >> _service_id >> _type >> _account >> _code >> _appkey >> _extra;
		}
};

struct VerifySmsCodeResp: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 14, 233)};
		
		std::string _context;     // 上下文
		uint32_t    _rescode;     // 结果码，参考VCRES_ERRCODE
		std::string _errinfo;     // 结果描述
		std::string _extra;       // 预留字段
		
		VerifySmsCodeResp(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _rescode << _errinfo << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _rescode >> _errinfo >> _extra;
		}
};

struct RequestImageCode: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 21, 233)};
		
		std::string _context;     // 上下文
		std::string _appid;       // 业务标识
		std::string _service_id;  // 业务子标识
		uint8_t     _image_type;  // 图片验证码类型（0：英文，1：中文）
		uint32_t    _expire_time; // 有效时间（单位：秒，不能超过24小时，建议不超过10分钟）
		std::string _appkey;      // 验证信息
		std::string _extra;       // 预留字段
		
		RequestImageCode(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _appid << _service_id << _image_type << _expire_time << _appkey << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _appid >> _service_id >> _image_type >> _expire_time >> _appkey >> _extra;
		}
};

struct RequestImageCodeResp: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 22, 233)};
		
		std::string _context;     // 上下文
		uint32_t    _rescode;     // 结果码，参考VCRES_ERRCODE
		std::string _internal_id; // 验证码唯一标识
		std::string _codeimage;   // 验证码图片
		std::string _errinfo;     // 结果描述
		std::string _extra;       // 预留字段
		
		RequestImageCodeResp(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _rescode << _internal_id << _codeimage << _errinfo << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _rescode >> _internal_id >> _codeimage >> _errinfo >> _extra;
		}
};

struct VerifyImageCode: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 23, 233)};
		
		std::string _context;     // 上下文
		std::string _appid;       // 业务标识
		std::string _service_id;  // 业务子标识
		std::string _internal_id; // 验证码唯一标识
		std::string _code;        // 验证码图片
		uint8_t     _image_type;  // 验证失败后，重新获取图片验证码类型（0：英文，1：中文）
		std::string _appkey;      // 验证信息
		std::string _extra;       // 预留字段
		
		VerifyImageCode(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _appid << _service_id << _internal_id << _code << _image_type << _appkey << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _appid >> _service_id >> _internal_id >> _code >> _image_type >> _appkey >> _extra;
		}
};

struct VerifyImageCodeResp: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 24, 233)};
		
		std::string _context;     // 上下文
		uint32_t    _rescode;     // 结果码，参考VCRES_ERRCODE
		std::string _internal_id; // 验证码唯一标识（验证不成功的情况下使用）
		std::string _codeimage;   // 验证码图片（验证不成功的情况下使用）
		std::string _errinfo;     // 结果描述
		std::string _extra;       // 预留字段
		
		VerifyImageCodeResp(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _rescode << _internal_id << _codeimage << _errinfo << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _rescode >> _internal_id >> _codeimage >> _errinfo >> _extra;
		}
};

struct RequestImageCode2: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 31, 233)};
		
		std::string _context;     // 上下文
		std::string _appid;       // 业务标识
		std::string _service_id;  // 业务子标识
		uint64_t    _yyuid;       // 用户yyuid
		uint8_t     _image_type;  // 图片验证码类型（0：英文，1：中文）
		uint32_t    _expire_time; // 有效时间（单位：秒，不能超过24小时，建议不超过10分钟）
		std::string _appkey;      // 验证信息
		std::string _extra;       // 预留字段
		
		RequestImageCode2(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _appid << _service_id << _yyuid << _image_type << _expire_time << _appkey << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _appid >> _service_id >> _yyuid >> _image_type >> _expire_time >> _appkey >> _extra;
		}
};

struct RequestImageCodeResp2: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 32, 233)};
		
		std::string _context;     // 上下文
		uint32_t    _rescode;     // 结果码，参考VCRES_ERRCODE
		std::string _codeimage;   // 验证码图片
		std::string _errinfo;     // 结果描述
		std::string _extra;       // 预留字段
		
		RequestImageCodeResp2(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _rescode << _codeimage << _errinfo << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _rescode >> _codeimage >> _errinfo >> _extra;
		}
};

struct VerifyImageCode2: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 33, 233)};
		
		std::string _context;     // 上下文
		std::string _appid;       // 业务标识
		std::string _service_id;  // 业务子标识
		uint64_t    _yyuid;       // 用户yyuid
		std::string _code;        // 验证码图片
		uint8_t     _image_type;  // 验证失败后，重新获取图片验证码类型（0：英文，1：中文）
		std::string _appkey;      // 验证信息
		std::string _extra;       // 预留字段
		
		VerifyImageCode2(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _appid << _service_id << _yyuid << _code << _image_type << _appkey << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _appid >> _service_id >> _yyuid >> _code >> _image_type >> _appkey >> _extra;
		}
};

struct VerifyImageCodeResp2: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 34, 233)};
		
		std::string _context;     // 上下文
		uint32_t    _rescode;     // 结果码，参考VCRES_ERRCODE
		std::string _codeimage;   // 验证码图片（验证不成功的情况下使用）
		std::string _errinfo;     // 结果描述
		std::string _extra;       // 预留字段
		
		VerifyImageCodeResp2(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _rescode << _codeimage << _errinfo << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _rescode >> _codeimage >> _errinfo >> _extra;
		}
};

#endif
