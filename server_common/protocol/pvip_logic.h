#pragma once

#include "common/packet.h"
#include "common/core/base_svid.h"

#include <iostream>
#include <vector>
#include <string>

namespace protocol
{
	namespace vip
	{
		//rsync文件格式
		/*
			每一行为uid 增加的积分，如
			1001 150
			1002 300
			1003 600
		*/

		//设置会员
		struct PSetVipUser : public sox::Marshallable
		{
			uint32_t uUid;
			std::string strDateFrom;
			std::string strDateTo;

			virtual void marshal(sox::Pack &p) const
			{
				p << uUid << strDateFrom << strDateTo;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uUid >> strDateFrom >> strDateTo;
			}
		};

		struct PSS_SetVipUser : public sox::Marshallable
		{
			enum { uri = (1 << 8) | protocol::VIP_SVID};

			std::vector<PSetVipUser> vecVipUser;

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, vecVipUser);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::back_inserter(vecVipUser));
			}
		};

		//设置会员过期
		struct PSS_UnSetVipUser : public sox::Marshallable
		{
			enum { uri = (2 << 8) | protocol::VIP_SVID};

			std::vector<uint32_t> vecUid;

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, vecUid);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::back_inserter(vecUid));
			}
		};

		//修改用户成长记录
		struct PSetVipScore : public sox::Marshallable
		{
			uint32_t uUid;
			uint32_t uBaseScore;  //基础成长值
			std::string strBaseDate;   //基础日期
			uint32_t uAddValue;   //每天增加的分数，可以为负

			virtual void marshal(sox::Pack &p) const
			{
				p << uUid << uBaseScore << strBaseDate << uAddValue;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uUid >> uBaseScore >> strBaseDate >> uAddValue;
			}
		};
		struct PSS_SetVipScore : public sox::Marshallable
		{
			enum { uri = (3 << 8) | protocol::VIP_SVID};

			std::vector<PSetVipScore> vecVipScore;

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, vecVipScore);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::back_inserter(vecVipScore));
			}
		};

		//////////////////////////////////////////////////////////////////////////
		// vip sms

		enum EVipSmsType
		{
			ENUM_SMS_OFFSITE_LOGIN = 1,     // 异地登陆提醒
			ENUM_SMS_EXPIRE,                // 过期提醒
			ENUM_SMS_AUTOPAY                // 自动续费提醒
		};

		// expire sms
		struct PSS_SmsExpire : public sox::Marshallable
		{
			enum { uri = (4 << 8) | protocol::VIP_SVID};

			std::vector<uint32_t> m_vecUid;

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_vecUid);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::back_inserter(m_vecUid));
			}
		};

		struct PSS_SmsLoginOffSite : public sox::Marshallable
		{
			enum { uri = (5 << 8) | protocol::VIP_SVID};

			uint32_t m_uUid;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid;
			}
		};


		//////////////////////////////////////////////////////////////////////////
		//vip app ...
		//
		struct PSS_AddVip : public sox::Marshallable
		{
			enum { uri = (21 << 8) | protocol::VIP_SVID};

			uint32_t m_uDays;
			uint32_t m_uUid;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uDays;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_uDays;
			}
		};

		//////////////////////////////////////////////////////////////////////////
		// 支付相关

		// 请求开通会员 / 续费

		struct PSS_VipPay : public sox::Marshallable
		{
			enum { uri = (100 << 8) | protocol::VIP_SVID};
			std::string passport;
			uint32_t     m_uUid;
			uint32_t     m_uMoney;        // 单位 分
			uint32_t     m_uDays;         // 开通时间， 单位天
			std::string  m_strOrderId;    // 订单号

			virtual void marshal(sox::Pack &p) const
			{
				p << passport << m_uUid << m_uMoney << m_uDays << m_strOrderId;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> passport >> m_uUid >> m_uMoney >> m_uDays >> m_strOrderId;
			}
		};

		struct PSS_VipPayRes : public sox::Marshallable
		{
			enum { uri = (101 << 8) | protocol::VIP_SVID};

			uint32_t    m_uUid;
			std::string m_strOrderId;
			uint16_t    m_uResCode;     // 操作是否被vip_logic记录

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uResCode << m_strOrderId;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_uResCode >> m_strOrderId;
			}

		};

		// 向sdb 请求

		struct PSS_ApplyVip : public sox::Marshallable
		{
			enum { uri = (102 << 8) | protocol::VIP_SVID};

			uint32_t     m_uUid;
			std::string  m_strTaskId;
			uint32_t     m_uDays;       // 开通时间  天

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_strTaskId << m_uDays;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_strTaskId >> m_uDays;
			}
		};

		struct PSS_ApplyVipRes : public sox::Marshallable
		{
			enum { uri = (103 << 8) | protocol::VIP_SVID};

			uint32_t    m_uUid;
			std::string m_strTaskId;
			uint16_t    m_uResCode;     // 是否充值成功
			uint32_t    m_uStartTime;   // 本次充值开始计费时间
			uint32_t    m_uExpireTime;  // 到期时间

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_strTaskId << m_uResCode << m_uStartTime << m_uExpireTime;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_strTaskId >> m_uResCode >> m_uStartTime >> m_uExpireTime;
			}
		};

		enum EPaySourcePath
		{
			ENUM_ZHUHAI_ORDER = 0,   //珠海系统订单
			ENUM_PLATFORM_ORDER = 1,     //运营系统订单
			ENUM_ACTIVITY_ORDER = 2,     //活动系统订单（包括YY商城订单）
			ENUM_VMALL_KQ_ORDER = 3,     //新支付块钱订单
			ENUM_VMALL_YB_ORDER = 4,     //新支付Y币订单
			ENUM_VCARD_ORDER = 5,        //新支付V卡支付订单
			ENUM_PHONE_ORDER = 6,        //手机支付订单
            ENUM_GOODIMID_ORDER=7,       //YY靓号摊分充值YY会员订单
			ENUM_TAOBAO_ORDER = 8,       // 淘宝订单
			ENUM_EXTENAL_ORDER = 9,     //对外接口订单
			ENUM_AUTH_ORDER = 10,       //激活码付款路径
			ENUM_NEW_PAY_ORDER = 11,   //支付中心迁移后的新支付中心订单
            ENUM_JIAOYOU_ORDER = 12,    // 从交友过来的订单

			ENUM_DEFAULT_ORDER = 255 //默认未知路径，待区分
		};

		enum EOrderType
		{
			ENUM_NORMAL_TYPE = 0,                  //普通订单类型
			ENUM_SON_TYPE = 1,                     //子母单
			ENUM_PURPLE_DIAMOND_TYPE = 2,          // 紫砖会员类型
			ENUM_PURPLE_DIAMOND_SON_TYPE = 3,      // 紫砖会员子单类型
			ENUM_SUPER_DIAMOND_ALL_TYPE = 4,       // 超级紫钻类型
			ENUM_SUPER_DIAMOND_UPGRADE_TYPE = 5,   // 超级紫钻升级类型
			ENUM_OW_VIP_TYPE = 6,                  // OW会员
            ENUM_PINK_DIAMOND_TYPE = 7,            // 粉钻会员类型
            ENUM_SVIP_TYPE = 8,                    // SVIP类型

			ENUM_DEFAULT_TYPE = 255            //默认未知类型，待区分
		};


		// vip pre-expire
		struct PSS_VipSysmPreExpire : public sox::Marshallable
		{
			enum { uri = (201 << 8) | protocol::VIP_SVID};

			uint32_t m_uPreDays;
			std::vector<uint32_t> m_vecUids;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uPreDays;
				sox::marshal_container(p, m_vecUids);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uPreDays;
				sox::unmarshal_container(p, std::back_inserter(m_vecUids));
			}
		};

		// vip upgrade
		struct PSS_VipSysmUpgrade : public sox::Marshallable
		{
			enum { uri = (202 << 8) | protocol::VIP_SVID};

			std::map<uint32_t, uint32_t> m_mapUid2Grade;

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_mapUid2Grade);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::inserter(m_mapUid2Grade, m_mapUid2Grade.begin()));
			}
		};

		// vip expired
		struct PSS_VipSysmExpired : public sox::Marshallable
		{
			enum { uri = (203 << 8) | protocol::VIP_SVID};

			PSS_VipSysmExpired() {}
			PSS_VipSysmExpired(uint32_t uDec, const std::vector<uint32_t>& vecUids)
				: m_uDecScore(uDec), m_vecUids(vecUids)
			{

			}

			uint32_t m_uDecScore;
			std::vector<uint32_t> m_vecUids;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uDecScore;
				sox::marshal_container(p, m_vecUids);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uDecScore;
				sox::unmarshal_container(p, std::back_inserter(m_vecUids));
			}
		};

		// user is the first time to apply vip or is renew
		struct PSS_VipSysmNew : public sox::Marshallable
		{
			enum { uri = (204 << 8) | protocol::VIP_SVID};

			uint32_t m_uApplyType;     // apply type
			bool     m_bIsVip;               // current is vip or not
			uint32_t m_uUid;
			std::string m_strOrderId;
			uint32_t m_uExpireDays;   // the expire_days

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uApplyType;
				p.push_uint32(m_bIsVip);
				p << m_strOrderId << m_uExpireDays;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_uApplyType;
				m_bIsVip = (bool)p.pop_uint32();
				p >> m_strOrderId >> m_uExpireDays;
			}
		};

		// vip user login notify
		struct PSS_VipSysmLogin : public sox::Marshallable
		{
			enum { uri = (205 << 8) | protocol::VIP_SVID};

			uint32_t    m_uUid;
			std::string m_strLoginInfo;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_strLoginInfo;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_strLoginInfo;
			}
		};

		// imonlineinfo --> vip_sysmsg  ----> imonlineinfo
		struct SVipInfo : public sox::Marshallable
		{
			uint32_t m_uIsVip;                       // 0 非会员，1会员， 2过期会员
			uint32_t m_uGrade;
			uint32_t m_uNextGrade;
			std::string m_strMoodInfo;
			std::string m_strActionDesc;             // 会员活动
			std::string m_strActionUrl;              // 活动链接

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uIsVip << m_uGrade << m_uNextGrade << m_strMoodInfo
					<< m_strActionDesc << m_strActionUrl;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uIsVip >> m_uGrade >> m_uNextGrade >> m_strMoodInfo;
				if (!p.empty())
				{
					p >> m_strActionDesc >> m_strActionUrl;
				}
			}
		};

		struct PSS_VipSysLoginRes : public PSS_VipSysmLogin
		{
			enum { uri = (207 << 8) | protocol::VIP_SVID};

			std::string m_strVipInfo;    // marshal SVipInfo

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strVipInfo;
				PSS_VipSysmLogin::marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_strVipInfo;
				PSS_VipSysmLogin::unmarshal(p);
			}
		};

		struct PSS_VipActivityPay : public sox::Marshallable
		{
			enum { uri = (206 <<8 ) | protocol::VIP_SVID};

			uint32_t uid;
			uint32_t previous_vip_type;
			uint32_t renew_vip_type;
			uint32_t vip_begin_time;

			virtual void marshal(sox::Pack &p) const
			{
				p << uid << previous_vip_type << renew_vip_type << vip_begin_time;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uid >> previous_vip_type >> renew_vip_type >> vip_begin_time;
			}
		};

		// notify apply info to vip_app

		struct PSS_NotifyAppInfo : public sox::Marshallable
		{
			enum { uri = (208 << 8) | protocol::VIP_SVID};

			uint32_t m_uUid;
			uint32_t m_uDays;                  // apply days
			std::string m_strBeforeApply;      // VipInfo  before apply
			std::string m_strAfterApply;       // VipInfo  after apply
			std::string m_strOrderId;
			uint32_t m_uChargeType;
			std::string m_from;

			virtual void marshal(sox::Pack &p) const
			{
			  p << m_uUid << m_uDays << m_strBeforeApply << m_strAfterApply << m_strOrderId << m_uChargeType << m_from ;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_uDays >> m_strBeforeApply >> m_strAfterApply >> m_strOrderId;
				if ( !p.empty() ) {
				  p >> m_uChargeType;
				}
				if ( !p.empty() ) {
				  p >> m_from ;
				}
			}
		};

		struct PSS_VipSysmRingSet : public sox::Marshallable
		{
			enum { uri = (209 << 8) | protocol::VIP_SVID};

			uint32_t uid;
			uint32_t ring_type;
			uint32_t total_times;
			uint32_t used_times;
			uint32_t vip_grade;
			std::string end_time;

			virtual void marshal(sox::Pack &p) const
			{
				p << uid << ring_type << total_times << used_times << vip_grade << end_time;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uid >> ring_type >> total_times >> used_times >> vip_grade >> end_time;
			}
		};

		struct PSS_VipStatistics : public sox::Marshallable
		{
			enum { uri = (210 << 8) | protocol::VIP_SVID};

			std::string service_type_;
			std::string ip_;
			std::map<std::string, uint32_t> type_to_uid_;

			virtual void marshal(sox::Pack &p) const
			{
				p << service_type_ << ip_;
				sox::marshal_container(p,type_to_uid_);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> service_type_ >> ip_;
				sox::unmarshal_container(p, std::inserter(type_to_uid_, type_to_uid_.begin()));
			}
		};

		struct PSS_VipSysmTipRingSet : public sox::Marshallable
		{
			enum { uri = (211 << 8) | protocol::VIP_SVID};

			uint32_t uid;
			uint32_t vip_grade;
			virtual void marshal(sox::Pack &p) const
			{
				p << uid << vip_grade;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uid >> vip_grade;
			}
		};

		// used already
		// enum { uri = (212 << 8) | protocol::VIP_SVID};

/////////////////////////////////////////////////////////////
/******重构之后为兼容添加的协议*******/

		struct PSS_VipPayNew : public sox::Marshallable
		{
			enum { uri = (301 << 8) | protocol::VIP_SVID};
			std::string passport;
			uint32_t     m_uUid;
			uint32_t     m_uMoney;        // 单位 分
			uint32_t     m_uDays;         // 开通时间， 单位天
			uint32_t     m_uType;         // 开通会员的类型
			std::string  m_strOrderId;    // 订单号
			uint32_t     m_uPayPath;      // 订单来源路径
			uint32_t     m_uOrderType;    // 订单类型，（普通单，子母单）
			std::string  m_strNotifyUrl;
			std::string  m_context;

			virtual void marshal(sox::Pack &p) const
			{
				p << passport << m_uUid << m_uMoney << m_uDays <<  m_uType
					<< m_strOrderId << m_uPayPath << m_uOrderType << m_strNotifyUrl << m_context;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> passport >> m_uUid >> m_uMoney >> m_uDays >> m_uType >> m_strOrderId >> m_uPayPath >> m_uOrderType;
				if (!p.empty())
				{
					try {
						p >> m_strNotifyUrl >> m_context;
					}catch(const sox::PackError& e)
					{
					}
				}
			}
		};

		struct PSS_VipPayNewRes : public sox::Marshallable
		{
			enum { uri = (302 << 8) | protocol::VIP_SVID};

			uint32_t    m_uUid;
			std::string m_strOrderId;
			uint16_t    m_uResCode;     // 操作是否被vip_logic记录
			uint32_t    m_uPayPath;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uResCode << m_strOrderId << m_uPayPath;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_uResCode >> m_strOrderId >> m_uPayPath;
			}

		};

		struct PSS_ApplyVipNew : public sox::Marshallable
		{
			enum { uri = (303 << 8) | protocol::VIP_SVID};

			uint32_t     m_uUid;
			std::string  m_strTaskId;
			uint32_t     m_uMoney;
			uint32_t     m_uDays;       // 开通时间  天
			uint32_t     m_uType;       // 开通类型
			uint32_t     m_uPayPath;      // 订单来源路径
			uint32_t     m_uOrderType;    // 订单类型，（普通单，子母单）


			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_strTaskId << m_uMoney << m_uDays << m_uType << m_uPayPath << m_uOrderType;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_strTaskId >> m_uMoney >> m_uDays >> m_uType >> m_uPayPath >> m_uOrderType;
			}
		};

		struct PSS_ApplyVipNewRes : public sox::Marshallable
		{
			enum { uri = (304 << 8) | protocol::VIP_SVID};

			uint32_t    m_uUid;
			std::string m_strTaskId;
			uint16_t    m_uResCode;     // 是否充值成功

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_strTaskId << m_uResCode;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_strTaskId >> m_uResCode;
			}
		};

		struct PSS_ApplyMoodImage : public sox::Marshallable
		{
			enum { uri = (305 << 8) | protocol::VIP_SVID};

			uint32_t    m_uUid; //签到的uid
			uint32_t    m_sign_image;//使用的图像编号
			uint32_t    m_sign_rank;//当月签到排名
			uint32_t    m_sing_count;//当月签到次数

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_sign_image << m_sign_rank<<m_sing_count;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_sign_image >> m_sign_rank>>m_sing_count;
			}
		};

		enum InfoType
		{
			TYPE_GUESS_FRIEND_MOOD  = 0,
			TYPE_REMIND_FRIEND_SIGN
		};

		struct PSS_GuessMoodInfo : public sox::Marshallable
		{
			enum { uri = (306 << 8) | protocol::VIP_SVID};

			uint32_t uid;                   // the uid of user
			uint32_t friend_uid;            // the uid of the friend of user
			uint32_t guess_right_result;    // the result of guess_friend_result,0 means true,1 means false
			uint32_t timestamp;             // the time of user guess
			InfoType type;                  // the type of info

			virtual void marshal(sox::Pack &p) const
			{
				p << uid << friend_uid << guess_right_result << timestamp;
				p.push_uint32(static_cast<uint32_t>(type));
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uid >> friend_uid >> guess_right_result >> timestamp;
				type = static_cast<InfoType>(p.pop_uint32());
			}
		};
		struct PSS_GetVipInfoReq : public sox::Marshallable
		{
			enum { uri = (307 << 8) | protocol::VIP_SVINFO};

			std::string from_;   //来源非白名单，不提供服务
			uint32_t    uid_;    //user id
			uint32_t server_id;  //请求的serverid
			std::string extend_; //扩展

			virtual void marshal(sox::Pack &p) const
			{
				p << from_ << uid_ <<server_id<< extend_;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> from_ >> uid_ >>server_id>> extend_;
			}
		};

		struct PSS_GetVipInfoRes : public sox::Marshallable
		{
			enum { uri = (308 << 8) | protocol::VIP_SVINFO};

			uint32_t    uid_;    //user id
			uint32_t    charge_type_; //费用类型
			uint32_t    grade_;  //当前等级,如果用户为非vip返回0
			std::string extend_; //扩展

			virtual void marshal(sox::Pack &p) const
			{
				p << uid_ << charge_type_ << grade_ << extend_;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uid_ >> charge_type_ >> grade_ >> extend_;
			}
		};

		struct PSS_AutoPayTip : public sox::Marshallable
		{
			enum { uri = (309 << 8) | protocol::VIP_SVID };

			uint32_t    uid;        //user id
			uint32_t    pay_result;        //Y币扣费结果
			std::string extend_str; //扩展

			virtual void marshal(sox::Pack &p) const
			{
				p << uid << pay_result << extend_str;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uid >> pay_result >> extend_str;
			}
		};

		struct PSS_AlarmInfo: public sox::Marshallable
		{
			enum { uri = (310 << 8) | protocol::VIP_SVID };

			std::string from_;
			std::string service_type_;
			uint32_t    time_stamp_;

			virtual void marshal(sox::Pack &p) const
			{
				p << from_ << service_type_ << time_stamp_ ;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> from_ >> service_type_ >> time_stamp_;
			}
		};
		struct PSS_SendAlarm: public sox::Marshallable
		{
			enum { uri = (311 << 8) | protocol::VIP_SVID };

			uint32_t uid_;
			std::string msg_;

			virtual void marshal(sox::Pack &p) const
			{
				p << uid_ << msg_ ;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uid_ >> msg_;
			}
		};

		struct PSS_NotifyVipShop: public sox::Marshallable
		{
			enum { uri = (312 << 8) | protocol::VIP_SVID };

			uint32_t uid;
			std::string order_id;

			virtual void marshal(sox::Pack &p) const
			{
				p << uid << order_id ;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uid >> order_id;
			}
		};

		// 通知vip_timer 导入游戏礼包
		struct PSS_InsertGameGift: public sox::Marshallable
		{
			enum { uri = (313 << 8) | protocol::VIP_SVID };

			uint32_t m_giftId;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_giftId;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_giftId;
			}
		};

		struct PSS_NotifyReload : public sox::Marshallable
		{
			enum { uri = (314 << 8) | protocol::VIP_SVID };

			std::vector<std::string> m_vecArgs;

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_vecArgs);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::back_inserter(m_vecArgs));
			}
		};

		struct PSS_LotteryTask: public sox::Marshallable
		{
			enum { uri = (315 << 8) | protocol::VIP_SVID };

			uint32_t op_code_;  //1 run lottery;2 set lottery

			uint32_t term_id_;  //期号

			uint32_t id_;       //任务序号

			uint32_t win_number_;

			uint32_t  group_number_; //匹配多少组

			std::string lottery_;  //中奖号码

			virtual void marshal(sox::Pack &p) const
			{
				p << op_code_ << term_id_ << id_ << win_number_ << group_number_ << lottery_;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> op_code_ >> term_id_ >> id_ >> win_number_ >> group_number_ >> lottery_;
			}
		};

		struct RedisCommand : public sox::Marshallable{

			uint64_t id_;

			std::string redis_cmd_;

			virtual void marshal(sox::Pack &p) const
			{
				p << id_ << redis_cmd_ ;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> id_ >> redis_cmd_ ;
			}
		};

		struct PSS_SyncRedisCmd: public sox::Marshallable
		{
			enum { uri = (316 << 8) | protocol::VIP_SVID };

			std::string name_;

			uint64_t version_;

			uint32_t serverid_;

			virtual void marshal(sox::Pack &p) const
			{
				p << name_ << version_ << serverid_ ;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> name_ >> version_ >> serverid_ ;
			}
		};

		struct PSS_SyncRedisCmdRes: public sox::Marshallable
		{
			enum { uri = (317 << 8) | protocol::VIP_SVID };

			std::string name_;

			uint64_t version_;

			uint32_t server_id_;

			uint32_t res_code_; //0 successed ;other error

			std::vector< RedisCommand > cmd_list_;

			virtual void marshal(sox::Pack &p) const
			{
				p << name_ << version_ << server_id_ << res_code_ ;

				sox::marshal_container(p, cmd_list_);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> name_ >> version_ >> server_id_ >> res_code_ ;

				sox::unmarshal_container(p, std::back_inserter( cmd_list_ ));
			}
		};

		// 周年庆Y秀
		struct PSS_NotifyShowChange : public sox::Marshallable
		{
			enum { uri = (318 << 8) | protocol::VIP_SVID };

			uint32_t m_uid;
			uint32_t m_flag;                 // 1  穿上， 0 脱下

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uid << m_flag;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uid >> m_flag;
			}
		};

		struct PSS_NotifyShowChangeRes : public PSS_NotifyShowChange
		{
			enum { uri = (319 << 8) | protocol::VIP_SVID };
		};

		struct PSS_VipSysmShowPreExpire : public sox::Marshallable
		{
			enum { uri = (320 << 8) | protocol::VIP_SVID};

			//ENUM_SHOW_PRE_EXPIRE_2DAY,                 //Y秀2天后到期
			//ENUM_SHOW_EXPIRE_TODAY,                    //Y秀今天到期
			//ENUM_SHOW_EXPIRED_3_DAYS,                  //Y秀过期了3天
			uint32_t u_ShowMsgType;//the type of uids
			std::vector<uint32_t> m_vecUids;//32_248_98293_ .... _**
			virtual void marshal(sox::Pack &p) const
			{
				p << u_ShowMsgType;
				sox::marshal_container(p, m_vecUids);
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> u_ShowMsgType;
				sox::unmarshal_container(p, std::back_inserter(m_vecUids));
			}
		};

		// 系统消息通用包
		struct PSS_VipCommSysMsg : public sox::Marshallable
		{
			enum { uri = (321 << 8) | protocol::VIP_SVID};

			uint32_t  m_msgType;
			uint32_t  m_uid;

			PSS_VipCommSysMsg() : m_msgType(46), m_uid(0)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_msgType << m_uid;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_msgType >> m_uid;
			}
		};

		// SysMsgUtil 对通用包只能做些简单的和uid无关的解析，注意测试
		struct PSS_VipCommSysMsgBatch : public sox::Marshallable
		{
			enum { uri = (322 << 8) | protocol::VIP_SVID};

			uint32_t              m_msgType;
			uint32_t              m_expireDays;   // 用户未上线，保留消息多少天
			std::vector<uint32_t> m_uids;

			PSS_VipCommSysMsgBatch() : m_msgType(46), m_expireDays(1)
			{}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_msgType << m_expireDays;
				sox::marshal_container(p, m_uids);
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_msgType >> m_expireDays;
				sox::unmarshal_container(p, std::back_inserter(m_uids));
			}
		};

		struct PSS_MsgShowGift : public sox::Marshallable
		{
			enum { uri = (323 << 8) | protocol::VIP_SVID};

			uint32_t  m_showType;
			uint32_t  m_fromUid;
			uint32_t  m_uid;

			PSS_MsgShowGift() : m_fromUid(0), m_uid(0)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_fromUid << m_uid << m_showType;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_fromUid >> m_uid >> m_showType;
			}
		};


		struct PSC_UserSigin: public sox::Marshallable
		{
			enum { uri = (324 << 8) | protocol::VIP_SVID};

			uint32_t  top_sid_;
			uint32_t  sub_sid_;
			uint32_t  uid_;
			uint32_t  service_type_;

			PSC_UserSigin() : top_sid_(0), sub_sid_(0),uid_(0),service_type_(0)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << top_sid_ << sub_sid_ << uid_ << service_type_;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> top_sid_ >> sub_sid_ >> uid_ >> service_type_;
			}
		};

		struct PSC_YShowBroadcast : public sox::Marshallable
		{
			enum { uri = (325 << 8) | protocol::VIP_SVID};

			uint32_t  top_sid_;
			uint32_t  sub_sid_;
			uint32_t  broadcast_type_; //1顶级频道，2子频道广播
			uint32_t  uid_;
			uint32_t  service_type_;
			std::string context_; //[img]http://logoxxxx[img] [0xFF0000]蛋总[0xFF0000] 传着[0xFF0000]5周年YY秀[0xFF0000] 进场了
			std::string url_;
			std::string exten_info_;
			uint32_t  show_time_;           // 展示时间 秒

			PSC_YShowBroadcast() :top_sid_(0),sub_sid_(0),broadcast_type_(0),
			uid_(0),service_type_(0),context_(""),url_(""),exten_info_(""), show_time_(2)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << top_sid_ << sub_sid_ << broadcast_type_ << uid_ << service_type_ << context_ << url_ << exten_info_ << show_time_;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> top_sid_ >> sub_sid_ >> broadcast_type_ >> uid_ >> service_type_ >> context_ >> url_ >> exten_info_;
				if (!p.empty()){
					try{
						p >> show_time_;
					}catch (const sox::UnpackError& e){
					}
				}
			}
		};


		// 这个文件新加协议，我移到vip/protocol目录下了，别说偶没提醒你！！
	}
}
