#pragma once

#include "common/protocol/ivip_online_privilege.h"
#include "common/protocol/IVipSkin.h"
#include "common/core/base_svid.h"
#include "common/packet.h"

namespace protocol
{
	namespace vip
	{
		// get online privilege
		struct PCS_GetOnlinePrivilege : public sox::Voidmable
		{
			enum {uri = (1 << 8) | protocol::VIP_PRIVILEGE_SVID};
		};

		struct PCS_GetOnlinePrivilegeRes : public sox::Marshallable
		{
			enum {uri = (2 << 8) | protocol::VIP_PRIVILEGE_SVID};

			std::set<uint32_t> m_beHide;        // buddy set show hide to me while online
			std::set<uint32_t> m_beUnHide;
			std::set<uint32_t> m_hideTo;        // show hide to buddy while i am online
			std::set<uint32_t> m_unHideTo;

			uint32_t m_uMaxNum;                 // how many buddy you can set on your vip-grade

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_beHide);
				sox::marshal_container(p, m_beUnHide);
				sox::marshal_container(p, m_hideTo);
				sox::marshal_container(p, m_unHideTo);
				p << m_uMaxNum;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::inserter(m_beHide, m_beHide.begin()));
				sox::unmarshal_container(p, std::inserter(m_beUnHide, m_beUnHide.begin()));
				sox::unmarshal_container(p, std::inserter(m_hideTo, m_hideTo.begin()));
				sox::unmarshal_container(p, std::inserter(m_unHideTo, m_unHideTo.begin()));
				p >> m_uMaxNum;
			}
		};

		// set online privilege

		struct PCS_SetOnlinePrivilege : public sox::Marshallable
		{
			enum {uri = (3 << 8) | protocol::VIP_PRIVILEGE_SVID};

			uint32_t m_uUid;
			uint32_t m_uBuddyId;
			EOnlinePriOp m_emOp;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uBuddyId;
				p.push_uint32(m_emOp);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_uBuddyId;
				m_emOp = (EOnlinePriOp)p.pop_uint32();
			}
		};

		// set online privilege res

		struct PCS_SetOnlinePriRes : public PCS_SetOnlinePrivilege
		{
			enum {uri = (4 << 8) | protocol::VIP_PRIVILEGE_SVID};

			EOnlinePriResCode m_emResCode;

			virtual void marshal(sox::Pack &p) const
			{
				PCS_SetOnlinePrivilege::marshal(p);
				p.push_uint32(m_emResCode);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_SetOnlinePrivilege::unmarshal(p);
				m_emResCode = (EOnlinePriResCode)p.pop_uint32();
			}
		};

		// notify be set online privilege

		struct PCS_BeSetOnlinePri : public PCS_SetOnlinePrivilege
		{
			enum {uri = (5 << 8) | protocol::VIP_PRIVILEGE_SVID};
		};

		struct PCS_NotifyBuddyDel : public sox::Marshallable
		{
			enum {uri = (6 << 8) | protocol::VIP_PRIVILEGE_SVID};

			uint32_t m_uUid;
			uint32_t m_uBid;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uBid;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_uBid;
			}
		};

		// in use
		//enum {uri = (7 << 8) | protocol::VIP_EXPRESSION_SVID};
		//enum {uri = (8 << 8) | protocol::VIP_EXPRESSION_SVID};

		// 动态头像

		// 自定义上传动态头像，请求鉴权
		struct PCS_UpLogoAuth : public sox::Marshallable
		{
			enum {uri = (101 << 8) | protocol::VIP_PRIVILEGE_SVID};

			uint32_t    m_uUid;
			std::string m_strFileName;
			std::string m_strContext;      // 客户端上下文
		
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_strFileName << m_strContext;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_strFileName >> m_strContext;
			}
		};

		// 鉴权响应
		struct PCS_UpLogoAuthRes : public PCS_UpLogoAuth
		{
			enum {uri = (102 << 8) | protocol::VIP_PRIVILEGE_SVID};

			std::string m_strCookie;

			virtual void marshal(sox::Pack &p) const
			{
				PCS_UpLogoAuth::marshal(p);
				p << m_strCookie;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_UpLogoAuth::unmarshal(p);
				p >> m_strCookie;
			}
		};

		// 改变系统动态头像
		struct PCS_ChangeVipLogo : public sox::Marshallable
		{
			enum {uri = (103 << 8) | protocol::VIP_PRIVILEGE_SVID};

			uint32_t  m_uUid;
			uint32_t  m_uLogoID;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uLogoID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_uLogoID;
			}
		};

		// 通知IM会员动态头像变更
		struct PSS_NotifyVipLogoChange : public sox::Marshallable
		{
			enum {uri = (104 << 8) | protocol::VIP_PRIVILEGE_SVID};

			uint32_t    m_uUid;
			std::string m_strPng;          // 第一帧图片url
			std::string m_strGif;          // 完整gif图片url
			std::string m_strFrom;      // imlinkd信息from

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_strPng << m_strGif << m_strFrom;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_strPng >> m_strGif >> m_strFrom;
			}
		};

		// 会员皮肤

		// 设置会员皮肤
		struct PCS_VipSetSkin : public sox::Marshallable
		{
			enum {uri = (150 << 8) | protocol::VIP_PRIVILEGE_SVID};

			bool         m_bShow;
			uint32_t     m_uRgb;
			std::string  m_strUrl;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_bShow << m_uRgb << m_strUrl;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_bShow >> m_uRgb >> m_strUrl;
			}
		};

		// 设置皮肤响应
		struct PCS_VipSetSkinRes : public PCS_VipSetSkin
		{
			enum {uri = (151 << 8) | protocol::VIP_PRIVILEGE_SVID};
		};

		// 设置是否向好友show
		struct PCS_VipSkinShow : public sox::Marshallable
		{
			enum {uri = (152 << 8) | protocol::VIP_PRIVILEGE_SVID};
			
			bool     m_bShow;
			virtual void marshal(sox::Pack &p) const
			{
				p.push_uint8(m_bShow);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				m_bShow = (bool)p.pop_uint8();
			}
		};

		// 设置show的响应
		struct PCS_VipSkinShowRes : public PCS_VipSkinShow
		{
			enum {uri = (153 << 8) | protocol::VIP_PRIVILEGE_SVID};
		};

		// 好友皮肤变更通知
		struct PCS_VipSkinChange : public sox::Marshallable
		{
			enum {uri = (154 << 8) | protocol::VIP_PRIVILEGE_SVID};

			bool         m_bShow;
			uint32_t     m_uUid;
			uint32_t     m_uRgb;
			std::string  m_strUrl;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_bShow << m_uUid << m_uRgb << m_strUrl;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_bShow >> m_uUid >> m_uRgb >> m_strUrl;
			}
		};

		// 好友show信息变更通知
		struct PCS_VipSkinShowChange : public sox::Marshallable
		{
			enum {uri = (155 << 8) | protocol::VIP_PRIVILEGE_SVID};

			uint32_t     m_uUid;
			bool         m_bShow;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid;
				p.push_uint8(m_bShow);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid;
				m_bShow = (bool)p.pop_uint8();
			}
		};

		// 获取自己、好友皮肤信息
		struct PCS_VipGetSkin : public sox::Marshallable
		{
			enum {uri = (156 << 8) | protocol::VIP_PRIVILEGE_SVID};
	
			std::vector<uint32_t> m_vecUids;

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_vecUids);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::back_inserter(m_vecUids));
			}
		};

		// 获取皮肤信息响应
		struct PCS_VipGetSkinRes : public sox::Marshallable
		{
			enum {uri = (157 << 8) | protocol::VIP_PRIVILEGE_SVID};

			std::map<uint32_t, VipSkinInfo> m_mapUid2Info;

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_mapUid2Info);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::inserter(m_mapUid2Info, m_mapUid2Info.begin()));
			}
		};

        //请求支付的协议
        struct PCS_VipPay : public sox::Marshallable
        {
            enum {uri = (158 << 8) | protocol::VIP_PRIVILEGE_SVID};

            uint32_t m_extend_type;//用作扩展, 暂时想不到有什么用, 先全置0吧
            std::string m_message;   //如果message里含有用户的uid, 不能相信, 一定要校验一下. 前端发过来的协议能通过函数检验是哪个用户发过来的消息


            virtual void marshal(sox::Pack &p) const
            {
                p << m_extend_type << m_message;
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                p >> m_extend_type >> m_message;
            }
        };

        //支付的响应协议
        struct PCS_VipPayRes : public sox::Marshallable
        {
            enum {uri = (159 << 8) | protocol::VIP_PRIVILEGE_SVID};

            uint32_t m_result_code;
            uint32_t m_extend_type;//用作扩展, 暂时想不到有什么用, 先全置0吧
            std::string m_message;


            virtual void marshal(sox::Pack &p) const
            {
                p << m_result_code << m_extend_type << m_message;
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                p >> m_result_code >> m_extend_type >> m_message;
            }
        };
	}
}

