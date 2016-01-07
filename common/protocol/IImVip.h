#pragma once

#include "const.h"
#include "common/int_types.h"
#include "common/protocol/PVipRing.h"

namespace protocol
{
	namespace imvip
	{
		struct PCS_VipInfo : public sox::Marshallable
		{
			PCS_VipInfo():grade_(0),vip_type_(0) {}
			bool isVip() { return grade_ > 0; }

			uint32_t grade_;
			uint32_t vip_type_;
			virtual void marshal(sox::Pack &p) const
			{
				p << grade_<<vip_type_;
			}

			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> grade_ >> vip_type_;
			}
		};

		// 紫钻会员信息
		enum EPlDiamonPrivi 
		{
			ENUM_PLDIAMOND_NICK = 0,       // 紫色昵称
            ENUM_PLDIAMOND_SUPER,          // 超级紫钻
            ENUM_PLDIAMOND_EXVIP,          // 至尊会员
			ENUM_PLDIAMOND_GAME,           // 游戏紫钻     
			ENUM_SUPER_VIP_GRADE,          // 超凡会员等级   0 表示不是超凡会员
		};

		struct PlDiamondInfo : public sox::Marshallable
		{
			uint8_t                      m_grade;                // 0 非紫钻会员  1 ~ 7
			uint8_t                      m_type;                 // 0 月费  1 年费
			uint32_t                     m_upgradeTime;          // 升级时间
			std::map<uint32_t, uint8_t>  m_privi;                // 特权字段

			PlDiamondInfo() : m_grade(0), m_type(0), m_upgradeTime(0) {}

			virtual void marshal(sox::Pack& p) const
			{
				sox::PackBuffer buffer;
				sox::Pack pack(buffer);
				pack << m_grade << m_type << m_upgradeTime;
				sox::marshal_container(pack, m_privi);
				p.push_varstr(pack.data(), pack.size());
			}

			virtual void unmarshal(const sox::Unpack& p)
			{
				try{
					sox::Varstr vs = p.pop_varstr_ptr();
					sox::Unpack un(vs.data(), vs.size());
					un >> m_grade >> m_type >> m_upgradeTime;
					sox::unmarshal_container(un, std::inserter(m_privi, m_privi.begin()));
				}
				catch (const sox::UnpackError& )
				{
				}
			}
		};
	
		// ow 会员信息
		struct OwVipInfo : public sox::Marshallable
		{
			uint8_t  m_isVip;             // 1 yes, 0 not

			OwVipInfo() : m_isVip(0) {}
			virtual void marshal(sox::Pack& p) const
			{
				sox::PackBuffer buffer;
				sox::Pack pack(buffer);
				pack << m_isVip;
				p.push_varstr(pack.data(), pack.size());
			}

			virtual void unmarshal(const sox::Unpack& p)
			{
				try{
					sox::Varstr vs = p.pop_varstr_ptr();
					sox::Unpack un(vs.data(), vs.size());
					un >> m_isVip;
				}
				catch (const sox::UnpackError& )
				{
				}
			}
		};

		class IImVipWatcher
		{
		public:
			virtual ~IImVipWatcher() {}
			
			virtual void OnGetBuddyVipInfoRes(const std::map<uint32_t, PCS_VipInfo>& info) = 0;    // for im
			virtual void OnGetGrpUserVipInfoRes(const std::map<uint32_t, PCS_VipInfo>& info) = 0;  // for group

			//  if ( uGrade == 0 )      user is not vip         
			virtual void OnVipInfoChange(const uint32_t& uUid, const PCS_VipInfo& info) = 0;             // for im
			virtual void OnGrpVipInfoChange(const uint32_t& uUid, const PCS_VipInfo& info) = 0;          // for group

			// for single user
			virtual void OnGetVipInfoRes(const uint32_t& uUid, const PCS_VipInfo& info) = 0;

			virtual void OnGetBatchColorRingInfoRes(const std::map<uint32_t, std::vector<PCS_ColorRingInfo> >& infos) = 0;

			/*	
			* 炫铃设置变更的通知
			*   bIsSetRing: 是否设置了新铃声
			*   info: 如果bIsSetRing==true, 则info为新设置的铃声信息.
			*         如果bIsSetRing==false, info.idx的值没有意义,可以忽略
			*/
			virtual void OnColorRingInfoChange(const uint32_t& uUid, const bool& bIsSetRing, const PCS_ColorRingInfo &info) = 0;

			/*
			* 设置用户上限提醒服务器返回处理结果
			* @param uid设置的提醒用户uid
			* @param return_code设置结果
			*/
			virtual void OnSetWatchUserRes(const uint32_t& uid,const uint32_t& return_code) = 0;

			/*
			* 取消用户上限提醒服务器返回处理结果
			* @param uid设置的提醒用户uid
			* @param return_code设置结果
			*/
			virtual void OnCancelWatchUserRes(const uint32_t& uid,const uint32_t& return_code) = 0;

			/*
			* 用户登录时获取上限提醒列表
			* @param uid_ringId 提醒用户uid对应的铃声id列表,ringId == 0 表示无效id
			*/
			virtual void OnGetWatcherListRes(const std::map<uint32_t,uint32_t>& uid_ringId) = 0;

			/*
			* web设置上线提醒列表
			* @param uid_ringId 提醒用户uid对应的铃声id列表,ringId == 0 表示无效id
			*/
			virtual void OnWebSetWatcherListRes(const std::map<uint32_t,uint32_t>& uid_ringId) = 0;

            /*
             *web设置心情签到同步客户端
             *@param vip_mood_string 同步到客户端的心情字符串
             *@param uid，用户的uid
             */
            virtual void OnWebVipMoodStringRes(const uint32_t& uid, const std::string& vip_mood_string) = 0;

			/*
			* 获取会员可以使用的表情组件白名单，白名单中的用户才能使用该表情
			*/
			virtual void OnVipExpWhiteList(const std::map< uint32_t, std::vector<uint32_t> >& mapUid2Lists) = 0;

			// 上传自定义动态头像鉴权响应
			virtual void OnVipUpLogoAuthRes(const std::string& strContext, const std::string& strFileName, const std::string& strCookie) = 0;

			// im离线积分回调函数
			virtual void OnGetUserOffLineScoreForIm(const uint32_t& uid,const uint32_t& score,const uint32_t& add_score) = 0;

			// 群离线积分回调函数
			virtual void OnGetUserOffLineScoreForGrp(const uint32_t& uid,const uint32_t& score,const uint32_t& add_score) = 0;

			// 频道离线积分回调函数
			virtual void OnGetUserOffLineScoreForChannel(const uint32_t& uid,const uint32_t& score,const uint32_t& add_score) = 0;

			/* 用户登录获取v图标活动数,客户端回调函数
			 * @param vec_activity: 与用户相关的活动的id列表
			 */
			virtual void OnGetVipActivityRes(const std::vector<uint32_t>& m_vec_activity) = 0;
		   /* 用户点击v图标,返回的结果
			* @param uid: 用户的uid
			* @param return_code设置结果,1:成功;0:失败
			*/
			virtual void OnCancelActivityRes(const uint32_t& uid,const uint32_t& return_code) = 0;

			// 会员通知消息
			//   @param id:  消息类型ID，供以后可能的统计需求使用（暂时没用）
			virtual void OnVipMsg(const uint32_t& id, const std::string& strMsg) = 0;			

			// 获取紫钻信息返回
			// @param  type  1 好友及自己  2 单个陌生人
			virtual void onGetPlDiamondInfoRes(const uint32_t& type, const std::map<uint32_t, PlDiamondInfo>& uid2Info) {}

			// 或者紫钻离线积分
			// @param  type  1 好友列表   2 群成员   3 频道列表
			virtual void onGetPlDiamondOffScoreRes(const uint32_t& uid, const uint32_t& score, const uint32_t& addScore, const uint32_t& type) {}

			virtual void onGetOwVipInfoRes(const uint32_t& type, const std::map<uint32_t, OwVipInfo>& uid2Info) {}
		};

		class IImVipWatcherBase
		{
		public:
			virtual ~IImVipWatcherBase() {}

			virtual void watch(IImVipWatcher *w) = 0;
			virtual void revoke(IImVipWatcher *w) = 0;
		};

		class IImVip : public IImVipWatcherBase
		{
		public:
			virtual ~IImVip() {}

			// 获取好友的会员信息
			virtual void GetBuddyVipInfo(const std::vector<uint32_t>& vecUids) = 0;

			// 获取群成员的会员信息
			virtual void GetGrpUserVipInfo(const std::vector<uint32_t>& vecUids) = 0;

			// 获取陌生人的会员信息
			virtual void GetSingleVipInfo(uint32_t uPeerId) = 0;

			virtual void GetBatchColorRingInfo(const std::vector<uint32_t>& vecUids) = 0;

			/*
			* vip用户设置好友为上线提醒对象
			* @param uid 上线提醒用户的uid
			*/
			virtual void SetWatchUser(uint32_t uid) = 0;

			/*
			* vip用户取消好友为上线提醒对象
			* @param uid 上线提醒用户的uid
			*/
			virtual void CancleWatchUser(uint32_t uid) = 0;

			/*
			* vip用户登录时获取上线提醒列表
			*/
			virtual void GetWatcherList() = 0;

			/*
			* 获取会员可以使用的表情组件白名单，在确认自己是vip后调用
			*/
			virtual void GetVipExpWhiteList(const std::vector<uint32_t>& vecUids) = 0;

			// 动态头像
			// strFileName  上传图片的文件名，  strContext  客户端上下文
			virtual void UpLogoAuth(const std::string& strFileName, const std::string& strContext) = 0;
			virtual void ChangeSysLogo(const uint32_t uLogoId) = 0;

			// 请求离线积分ui主动调用函数
			// uid 被请求者uid
			// 请求来源 1 im  2 group  3 channel
			virtual void GetUserOffLineScore(const uint32_t uid,const uint32_t from) = 0;
			
			/* 用户登录请求获取v图标活动数
			 * param uid 用户的uid
			 */
			virtual void GetVipActivity(const uint32_t& uid) = 0;
			/* 用户点击v图表,取消v图标的活动数
			 * param uid 用户的uid
			 * param m_vec_activity 用户相关的活动id
			 */
			virtual void CancleActivity(uint32_t uid,std::vector<uint32_t> m_vec_activity) = 0;

			// 获取紫钻会员信息，好友紫钻信息登陆的时候，protocol发起
			// @param  type   1 好友及自己， 2 单个陌生人
			virtual void GetPlDiamondInfo(uint32_t type, const std::vector<uint32_t>& vecUids) = 0;

			// 获取紫钻会员离线积分
			// @param  type   1 好友列表   2 群成员   3 频道列表
			virtual void GetPlDiamondOffScore(uint32_t type, uint32_t uid) = 0;

			// 获取紫钻会员信息
			// @param  type   预留
			virtual void GetOwVipInfo(uint32_t type, const std::vector<uint32_t>& vecUids) = 0;
		};
	}
}
