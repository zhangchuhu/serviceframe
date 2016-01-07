#ifndef _PGLOBALBAN_H_
#define _PGLOBALBAN_H_

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"

#include <vector>

namespace server{
	namespace globalBan{

		struct PHeartBeatReq: public sox::Marshallable
		{
			enum {uri = 50<<8|110};	

			uint32_t  uVal;					

			PHeartBeatReq():uVal(0){}
			virtual void marshal(sox::Pack &p) const 
			{
				p << uVal;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> uVal;
			}
		};

		struct PHeartBeatRes: public sox::Marshallable
		{				
			enum {uri = 51<<8|110};	

			uint32_t uVal;	

			PHeartBeatRes():uVal(0){}
			virtual void marshal(sox::Pack &p) const 
			{
				p << uVal;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> uVal;
			}
		};


		//UDB to GlobalBan_d
		struct PUDBCheckGlobalBan: public sox::Marshallable{
			enum {uri = (5 << 8) | protocol::ONLINEDB_SVID};

			uint32_t uid;
			uint32_t ip;

			virtual void marshal(sox::Pack &p) const {
				p << ip << uid;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> ip >> uid;
			}
		};

		struct UDBCheckGlobalBanRes: public sox::Marshallable{
			enum {uri = (7 << 8) | protocol::ONLINEDB_SVID};	

			bool isBan;
			uint32_t uid;
			uint32_t banType;
			std::string banDate;	
			std::string unbanDate;
			std::string note;

			UDBCheckGlobalBanRes():isBan(false),uid(0),banType(0),banDate(""),unbanDate(""),note(""){}


			virtual void marshal(sox::Pack &pk) const {
			pk << isBan << uid << banType << banDate << unbanDate << note;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> isBan >> uid >> banType >> banDate >> unbanDate >> note;	
			}
		};

		struct PCheckGlobalBan2 : public sox::Marshallable{
			enum {uri = (6 << 8) | protocol::ONLINEDB_SVID};

			uint32_t uid;
			std::string pc_info;
			
			uint32_t context_int;
			std::string context_str;
			
			PCheckGlobalBan2()
				: uid(0)
				, pc_info()
				{}

			virtual void marshal(sox::Pack &p) const {
				p << uid << pc_info << context_int << context_str;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> uid  >> pc_info >> context_int >> context_str;
			}
		};
		
		struct PCheckGlobalBan2Res: public sox::Marshallable {
			enum {uri = (107 << 8 | protocol::ONLINEDB_SVID)};
			uint32_t uid;
			std::string banDate;	// è¢«å°æ—¶é—´
			std::string unbanDate;	// è§£å°æ—¶é—´
			std::string note;		// å°ç¦åŽŸå› 
			
			uint32_t context_int;
			std::string context_str;

			virtual void marshal(sox::Pack &pk) const {
				pk << uid << banDate << unbanDate << note << context_int << context_str;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> uid >> banDate >> unbanDate >> note >> context_int >> context_str;
			}
		};



		struct PCheckGlobalBan3 : public sox::Marshallable
		{
			enum {uri = (9 << 8) | protocol::ONLINEDB_SVID};

		 	enum ENewPlatformType
			{
				ENUM_NEWPLATFORM_UNKNOWN = 0,
				ENUM_NEWPLATFORM_MOBILE = 1,
				ENUM_NEWPLATFORM_WEB = 2,
				ENUM_NEWPLATFORM_PC = 3,
				ENUM_NEWPLATFORM_PAD = 4,
				ENUM_NEWPLATFORM_HD = 5,
				ENUM_NEWPLATFORM_MAX = 65536
			};

			uint64_t uid;

			// 平台类型(ENewPlatformType)
			uint16_t platform;
			// 客户端版本标识(如: yymand3.9.0、yymip_3.9.0 )
			std::string clientVer;
			// 客户端类型标识(如: yymand/yymip/yym08and/yym08ip)
			std::string clientFrom;
			// 设备标识
			std::string pcInfo;
			
			// 发送方用于关联请求的上下文
			std::string context;
			
			PCheckGlobalBan3() : uid(0), platform(0) {}

			virtual void marshal(sox::Pack &p) const
			{
				p << uid << platform << clientVer << clientFrom << pcInfo << context;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uid  >> platform >> clientVer >> clientFrom >> pcInfo >> context;
			}
		};

		struct PCheckGlobalBan3Res: public sox::Marshallable 
		{
			enum {uri = (10 << 8 | protocol::ONLINEDB_SVID)};

			uint64_t uid;

			// 封禁原因码（通常为13)
			uint32_t uReason;

			// 包括封禁的类型识别码、封禁日期、解封时间、封禁原因
			// 如果是手机 yy 娱乐3.9 之前的版本，将封禁库中的原因内容直接透传(可能会出现 html 内容)
			// 如果是手机 yy 娱乐3.9 或之后的版本，发送 json 格式的字符串，
			// 如: {"type": 0, ban":"2015-04-10 12:00:00", "unban":"2015-04-11 13:00:00", "reason":"涉嫌参与频道[75752651]违规活动", "comment": "请联系客服<a href=\"xxxxx\">了解</a>被封禁的原因"})
			// comment 字段内可能会出现 html 内容
			// PC和其他移动客户端直接透传封禁库中的原因内容(可能会出现 html 内容)

			std::string comment;
			
			// 发送方用于关联请求的上下文
			std::string context;

			virtual void marshal(sox::Pack &pk) const 
			{
				pk << uid << uReason << comment << context;
			}
			virtual void unmarshal(const sox::Unpack &up) 
			{
				up >> uid >> uReason >> comment >> context;
			}
		};

		struct PCheckSoftBan : public sox::Marshallable{
			enum {uri = (16 << 8) | protocol::ONLINEDB_SVID};
			uint32_t uid;

			uint32_t context_int;  //æ¶ˆæ¯é‰´å®šå­—æ®µ
			std::string context_str; //æ¶ˆæ¯é‰´å®šå­—æ®µï¼Œå»ºè®®å¡«å†™ â€œUDB_CangePWDâ€ï¼Œç”¨æ¥æ ‡è®°æ”¹æ¶ˆæ¯

			PCheckSoftBan()
				: uid(0)
			{}

			virtual void marshal(sox::Pack &p) const {
				p << uid << context_int << context_str;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> uid >> context_int >> context_str;
			}
		};

		struct PCheckSoftBanRes: public sox::Marshallable {
			enum {uri = (17 << 8 | protocol::ONLINEDB_SVID)};
			uint32_t uid;
			uint32_t flag;    // 0:è½¯å°ï¼Œ1ï¼šéžè½¯å°ï¼Œ 2ï¼šé”™è¯¯
			uint32_t context_int;
			std::string context_str;

			virtual void marshal(sox::Pack &pk) const {
				pk << uid << flag << context_int << context_str;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> uid >> flag >> context_int >> context_str;
			}
		};

	}
}


#endif
