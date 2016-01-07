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

namespace secuserinfo{
/*
	//SECUSERINFO_SVID = 123;
	enum ResCode
	{
		SUI_SYSTEM_ERROR   = 100, // 其它系统错误
		SUI_VERIFY_SUCCESS = 101, // 票据验证成功
		SUI_VERIFY_FAILURE = 102, // 票据验证失败
		SUI_CODING_ERROR   = 103, // 票据编码错误
		SUI_APPID_ERROR    = 104, // 票据Appid不存在
		SUI_RATE_ERROR     = 111, // 超频访问错误
		SUI_AUTH_ERROR     = 112, // 接入权限错误
		SUI_EXISTENT_DATA  = 121, // 数据存在
		SUI_NEXISTENT_DATA = 122, // 数据不存在
		SUI_MATCH_DATA     = 131, // 数据匹配
		SUI_NMATCH_DATA    = 132, // 数据不匹配
		SUI_BIND_DATA      = 141, // 信息已绑定
		SUI_NBIND_DATA     = 142, // 信息未绑定
	};

	enum TokenEncodingType
	{
		RAW             = 0,   // 无编码
		BASE64          = 1,   // 最外层是base64编码
		BASE64_WITH_URL = 2,   // 最外层是URLs编码,其次是base64编码
	};
*/

	struct PVerifyAppTokenMsgReq: public sox::Marshallable{
		enum {uri = (5 << 8) | protocol::SECUSERINFO_SVID};
		std::string context;    // 上下文信息,会在返回里面带回,由业务系统自由使用
		uint32_t seq;           // seq,在返回里面带回,由业务系统自由使用
		std::string appid;      // 业务id
		uint32_t yyuid;         // yyuid,如果没有,则置为0
		std::string token;      // 票据内容
		uint32_t encoding_type; // 票据编码,类型为TokenEncodingType
		PVerifyAppTokenMsgReq():seq(0),yyuid(0),encoding_type(2)
		{
		}
		virtual void marshal(sox::Pack &p) const {
			p << context << seq << appid << yyuid << token << encoding_type;
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> context >> seq >> appid >> yyuid >> token >> encoding_type;
		}
	};

	struct PVerifyAppTokenMsgRes: public sox::Marshallable{
		enum {uri = (6 << 8) | protocol::SECUSERINFO_SVID};
		std::string context;  // 上下文信息,与PVerifyAppTokenMsgReq中的context值相等
		uint32_t seq;         // seq,与PVerifyAppTokenMsgReq中的seq值相等
		uint32_t rescode;     // 结果标识码,类型为ResCode
		uint32_t yyuid;       // 从票据获得的yyuid
		std::string passport; // 从票据获得的通行证
		PVerifyAppTokenMsgRes()
		{
		}
		virtual void marshal(sox::Pack &p) const {
			p << context << seq << rescode << yyuid << passport;
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> context >> seq >> rescode >> yyuid >> passport;
		}
	};

}
