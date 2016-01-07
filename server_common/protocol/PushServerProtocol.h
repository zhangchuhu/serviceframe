#ifndef _YY_PUSH_SERVER_PROTOCOL_H__
#define _YY_PUSH_SERVER_PROTOCOL_H__

#include <string>
#include <stdint.h>
#include <sstream>
#include <ostream>

#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/res_code.h"

namespace protocol
{
namespace pushserver
{
	// 协议编号
	const uint32_t APP_PUSH_SERVER          = 178;

	// 服务名称
	const char     PUSH_SERVER_NAME[32]     = "push_server";

	// 消息推送返回码
	const uint32_t YYPUSH_SUCCESS           = 0;         // 发布成功
	const uint32_t YYPUSH_ERR_ACCESS_REJECT = 1;         // 无权限,拒绝发布
	const uint32_t YYPUSH_ERR_USER_OFFLINE  = 2;         // 用户处于离线状态
	const uint32_t YYPUSH_ERR_COMMAND_REJECT = 3;        // 无效的消息推送类型
	const uint32_t YYPUSH_ERR_WHITELILST_DB  = 4;        // 白名单校验失败
	const uint32_t YYPUSH_ERR_DB             = 5;        // 数据库操作失败
	const uint32_t YYPUSH_ERR_PROTOCOL       = 6;        // 协议解析/组包失败
	const uint32_t YYPUSH_ERR_RESINIT        = 7;        // 资源初始化失败
	const uint32_t YYPUSH_ERR_MDB            = 8;        // 内存数据库操作失败
	const uint32_t YYPUSH_ERR_GENMSGID       = 9;        // 生成消息编号失败
	const uint32_t YYPUSH_ERR_DISPATCH       = 10;       // 消息路由失败
	const uint32_t YYPUSH_ERR_NETCONNED      = 11;       // 用户登录状态异常
	const uint32_t YYPUSH_ERR_USERNOTEXISTS  = 15;       // 用户不存在
	const uint32_t YYPUSH_ERR_QUERYUSER      = 17;       // 查询用户异常，用户不存在或数据异常
	const uint32_t YYPUSH_ERR_APNSSEND       = 29;       // 发送apns消息失败
	const uint32_t YYPUSH_ERR_APNSPAYLOADMAX = 30;       // apns消息包大小达到限制
	const uint32_t YYPUSH_ERR_USERINVALID    = 36;       // 用户已失效
	const uint32_t YYPUSH_ERR_SENDBYAPPVER   = 38;       // 按版本号推送失败
	const uint32_t YYPUSH_ERR_NOPERMISSION   = 39;       // 无权限

	// 在线用户消息发布请求
	struct PushPublishOnline: public sox::Marshallable
	{
	public :
		PushPublishOnline()
			:	yyuid_(0),
			 	pushType_(1),
			 	payloadLen_(0),
			 	expiredTime_(0),
			 	platform_(0),
			 	isBadge_(0)
		{

		}
		~PushPublishOnline()
		{

		}

	public :

		enum {uri = APP_PUSH_SERVER | 12 << 8};

		// 消息序列号
		std::string seq_;

		// yy账号
		uint64_t   yyuid_;

		// 消息订阅主题，topic用于区分应用的消息类型的，例如：yyedu/app/news，不支持中文和特殊字符
		std::string topic_;

		// 推送类型, 0 非登陆广播, 1 单点，2 群发，3 登陆广播；目前只支持1 单点；
		uint8_t     pushType_;

		// 推送消息长度
		uint64_t    payloadLen_;

		// 推送消息内容，可自定义消息内容格式，可采用json进行内容定制，例如："data":{"title":"课后作业APP", "content":"您有新的作业"}
		std::string payload_;

		// 失效时间
		uint64_t    expiredTime_;

		// 推送平台类型, 0 全平台，1 android，2 ios
		uint8_t     platform_;

		// 推送消息的app应用编号
		std::string sappid_;

		// 推送消息的app密钥
		std::string sappkey_;

		// 消息数是否体现在应用端图标上, 0 否，1 是，用于苹果IOS的应用图标消息数展示，非IOS应用填0
		uint32_t    isBadge_;

		// 消息提醒内容，用于苹果IOS的推送消息展示内容，此时payload_携带的是服务端推给苹果IOS应用的相关业务数据（不做提醒栏展示用）,非IOS应用填空
		std::string alertMsg_;

		// 推送消息的app应用版本号，版本号格式为“[0~9].[0~9].[0~9]”，例如：1.0.1
		std::string sappVersion_;

        // app应用版本号推送策略，设置该推送策略后，没有符合策略的用户则返回推送失败
		// “>”表示大于该版本号才进行推送；
		// “>=”表示大于等于该版本号才进行推送；
		// “<”表示小于该版本号才进行推送；
		// “<=”表示小于该版本号才进行推送；
		// “==”表示等于该版本号才进行推送；
		// 为空，则该参数不生效，所有版本都推送
		std::string sappStrategy_;


		virtual void marshal(sox::Pack &p) const
		{
			p << seq_ << yyuid_ << topic_ << pushType_ << payload_ << expiredTime_ << platform_ << sappid_ << sappkey_
					<< isBadge_ << alertMsg_ << sappVersion_ << sappStrategy_;
		}

		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> seq_ >> yyuid_ >> topic_ >> pushType_ >> payload_ >> expiredTime_ >> platform_ >> sappid_ >> sappkey_;

			if(p.empty() == false)
			{
				p >> isBadge_;
			}

			if(p.empty() == false)
			{
				p >> alertMsg_;
			}

			if(p.empty() == false)
			{
				p >> sappVersion_;
			}

			if(p.empty() == false)
			{
				p >> sappStrategy_;
			}

			payloadLen_ = payload_.size();
		}

		std::string dump()
		{
			std::ostringstream os;

			os<<"PushPublishOnline{"<<"seq("<<seq_<<"),yyuid("<<yyuid_<<"),topic("<<topic_<<"),pushtype("<<(int)pushType_<<"),payload("<<payload_
					<<"),expire("<<expiredTime_<<"),platform("<<(int)platform_<<"),appid("<<sappid_<<"),appkey("<<sappkey_<<"),isBadge_("<<isBadge_
					<<"),alertMsg_("<<alertMsg_<<"),sappVersion_("<<sappVersion_<<"),sappStrategy_("<<sappStrategy_<<")}";

			return os.str();
		}
	};

	// 在线用户消息发布应答
	struct PushPublishOnlineAck: public sox::Marshallable
	{
		enum {uri = APP_PUSH_SERVER | 13 << 8};

		// 消息序列号
		std::string   seq_;

		// 推送服务分配的消息唯一标识
		uint64_t      msgId_;

		// 推送返回码
		uint32_t      rc_;

		// 推送返回消息
		std::string   errmsg_;

		// 推送平台类型, 0 全平台，1 android，2 ios
		uint8_t     platform_;

		virtual void marshal(sox::Pack &p) const
		{
			p << seq_ << msgId_ << rc_ << errmsg_ << platform_;
		}

		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> seq_ >> msgId_ >> rc_ >> errmsg_;

			if(p.empty() == false)
			{
				p >> platform_;
			}
		}

		std::string dump()
		{
			std::ostringstream os;

			os<<"PushPublishOnlineAck{"<<"seq("<<seq_<<"),msgid("<<msgId_<<"),rc("<<rc_<<"),errmsg("<<errmsg_<<"),platform("<<platform_<<")}";

			return os.str();
		}
	};

	struct PushPublish: public sox::Marshallable
	{
	public :
		PushPublish()
			:	yyuid_(0),
			 	pushType_(1),
			 	payloadLen_(0),
			 	expiredTime_(0),
			 	platform_(0),
			 	isBadge_(0)
		{

		}

		~PushPublish()
		{

		}

	public :
		enum {uri = APP_PUSH_SERVER | 8 << 8};

		// 请求序列号
		std::string seq_;

		// yy账号
		uint64_t   yyuid_;

		// 消息订阅主题，topic用于区分应用的消息类型的，例如：yyedu/app/news，不支持中文和特殊字符
		std::string topic_;

		// 推送类型, 0 非登陆广播, 1 单点，2 群发，3 登陆广播；目前只支持1 单点；
		uint8_t     pushType_;

		// 推送消息长度
		uint64_t    payloadLen_;

		// 推送消息内容，可自定义消息内容格式，可采用json进行内容定制，例如："data":{"title":"课后作业APP", "content":"您有新的作业"}
		std::string payload_;

		// 失效时间，以1970-1-1 00:00:00为基准的秒数
		uint64_t    expiredTime_;

		// 推送平台类型, 0 全平台，1 android，2 ios
		uint8_t     platform_;

		// 推送消息的app应用编号
		std::string sappid_;

		// 推送消息的app密钥
		std::string sappkey_;

		// 消息数是否体现在应用端图标上, 0 否，1 是，用于苹果IOS的应用图标消息数展示，非IOS应用填0
		uint32_t    isBadge_;

		// 消息提醒内容，用于苹果IOS的推送消息展示内容，此时payload_携带的是服务端推给苹果IOS应用的相关业务数据（不做提醒栏展示用）,非IOS应用填空
		std::string alertMsg_;

		// 推送消息的app应用版本号，版本号格式为“[0~9].[0~9].[0~9]”，例如：1.0.1
		std::string sappVersion_;

        // app应用版本号推送策略
		// “>”表示大于该版本号才进行推送；
		// “>=”表示大于等于该版本号才进行推送；
		// “<”表示小于该版本号才进行推送；
		// “<=”表示小于该版本号才进行推送；
		// “==”表示等于该版本号才进行推送；
		// 为空，则该参数不生效，所有版本都推送
		std::string sappStrategy_;

		virtual void marshal(sox::Pack &p) const
		{
			p << seq_ << yyuid_ << topic_ << pushType_ << payload_ << expiredTime_ << platform_
					<< sappid_ << sappkey_ << isBadge_ << alertMsg_ << sappVersion_ << sappStrategy_;
		}

		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> seq_ >> yyuid_ >> topic_ >> pushType_ >> payload_ >> expiredTime_ >> platform_
				>> sappid_ >> sappkey_ ;

			if(p.empty() == false)
			{
				p >> isBadge_;
			}

			if(p.empty() == false)
			{
				p >> alertMsg_;
			}

			if(p.empty() == false)
			{
				p >> sappVersion_;
			}

			if(p.empty() == false)
			{
				p >> sappStrategy_;
			}

			payloadLen_ = payload_.length();
		}

		std::string dump()
		{
			std::ostringstream os;

			os<<"PushPublish{"<<"seq("<<seq_<<"),yyuid("<<yyuid_<<"),topic("<<topic_<<"),pushtype("<<(int)pushType_<<"),payload("<<payload_
					<<"),expire("<<expiredTime_<<"),platform("<<(int)platform_<<"),appid("<<sappid_<<"),appkey("<<sappkey_<<"),isBadge_("<<isBadge_
					<<"),alertMsg_("<<alertMsg_<<"),sappVersion_("<<sappVersion_<<"),sappStrategy_("<<sappStrategy_<<")}";

			return os.str();
		}
	};

	// 消息发布应答
	struct PushPublishAck: public sox::Marshallable
	{
		enum {uri = APP_PUSH_SERVER | 9 << 8};

		// 请求序列号, 透传
		std::string  seq_;

		// 推送服务分配的消息唯一标识
		uint64_t      msgId_;

		// 推送返回码
		uint32_t      rc_;

		// 推送返回消息
		std::string   errmsg_;

		// 推送平台类型, 0 全平台，1 android，2 ios
		uint8_t     platform_;

		virtual void marshal(sox::Pack &p) const
		{
			p << seq_ << msgId_ << rc_ << errmsg_ << platform_;
		}

		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> seq_ >> msgId_ >> rc_ >> errmsg_ ;

			if(p.empty() == false)
			{
				p >> platform_;
			}
		}

		std::string dump()
		{
			std::ostringstream os;

			os<<"PushPublishAck{"<<"seq("<<seq_<<"),msgid("<<msgId_<<"),rc("<<rc_<<"),errmsg("<<errmsg_<<"),platform("<<platform_<<")}";

			return os.str();
		}
	};

	// 查询用户是否在线(请求)
	struct PushIsOnLine: public sox::Marshallable
	{
		enum {uri = APP_PUSH_SERVER | 10 << 8};

		// 请求序列号
		std::string seq_;

		// yy账号
		uint64_t   yyuid_;

		// 客户端应用编号
		std::string  appid_;

		// 推送平台类型, 0 全平台，1 android，2 ios
		uint8_t     platform_;

		virtual void marshal(sox::Pack &p) const
		{
			p << seq_ << yyuid_<<appid_ << platform_;
		}

		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> seq_ >> yyuid_>>appid_ >> platform_;
		}

		std::string dump()
		{
			std::ostringstream os;

			os<<"PushIsOnLine{"<<"seq("<<seq_<<"),yyuid("<<yyuid_<<"),appid("<<appid_<<"),plat("<<(int)platform_<<")}";

			return os.str();
		}
	};

	// 查询用户是否在线(应答)
	struct PushIsOnLineAck: public sox::Marshallable
	{
		enum {uri = APP_PUSH_SERVER | 11 << 8};

		// 请求序列号
		std::string seq_;

		// 客户端是否在线
		uint8_t isOnLine_;

		// 推送返回码
		uint32_t   rc_;

		// 推送返回消息
		std::string errmsg_;

		virtual void marshal(sox::Pack &p) const
		{
			p << seq_ << isOnLine_<<rc_<<errmsg_ ;
		}

		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> seq_ >> isOnLine_ >> rc_ >> errmsg_ ;
		}

		std::string dump()
		{
			std::ostringstream os;

			os<<"PushIsOnLineAck {"<<"seq("<<seq_<<"),online("<<(int)isOnLine_<<"),rc("<<rc_<<"),errmsg("<<errmsg_<<")}";

			return os.str();
		}
	};
}
}

#endif
