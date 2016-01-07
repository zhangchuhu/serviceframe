#ifndef PROTOCOL_PVIP_IM_USER_H_INCLUDE
#define PROTOCOL_PVIP_IM_USER_H_INCLUDE

#include "common/core/base_svid.h"
#include "common/packet.h"
#include "IImVip.h"

namespace protocol
{
	namespace imvip
	{
		// require to get buddy's vip info
		struct PCS_GetBuddyVipInfo : public sox::Marshallable
		{
			enum {uri = (1 << 8) | protocol::IM_VIP_SRVID}; 

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

		// response
		struct PCS_GetBuddyVipInfoRes : public sox::Marshallable
		{
			enum {uri = (2 << 8) | protocol::IM_VIP_SRVID}; 

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

		struct PCS_UserVipInfoChange : public sox::Marshallable
		{
			enum {uri = (3 << 8) | protocol::IM_VIP_SRVID};

			uint32_t m_uUid;
			uint32_t m_uGrade;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uGrade;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_uGrade;
			}
		};

		struct PCS_GrpUserVipInfoChange : public PCS_UserVipInfoChange
		{
			enum {uri = (4 << 8) | protocol::IM_VIP_SRVID};
		};


		// get group user vip info

		struct PCS_GetGrpUserVipInfo : public PCS_GetBuddyVipInfo
		{
			enum {uri = (5 << 8) | protocol::IM_VIP_SRVID};
		};

		struct PCS_GetGrpUserVipInfoRes : public PCS_GetBuddyVipInfoRes
		{
			enum {uri = (6 << 8) | protocol::IM_VIP_SRVID};
		};

		// get single user vip info
		struct PCS_GetPeerVipInfo : public sox::Marshallable
		{
			enum {uri = (7 << 8) | protocol::IM_VIP_SRVID};

			uint32_t m_uUid;
			uint32_t m_uPeerId;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uPeerId;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_uPeerId;
			}
		};

		struct PCS_GetPeerVipInfoRes : public sox::Marshallable
		{
			enum {uri = (8 << 8) | protocol::IM_VIP_SRVID};

			uint32_t m_uUid;
			uint32_t m_uGrade;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uGrade;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_uGrade;
			}
		};

		//-------------------------------------------------------------------------------------
		//--add new protocol 
		//--request protocol is Inherit from old protocol
		//--reponse protocol is new protocol which are scalability
		//-------------------------------------------------------------------------------------
		struct PCS_GetBuddyVipInfoEx : public PCS_GetBuddyVipInfo
		{
			enum {uri = (9 << 8) | protocol::IM_VIP_SRVID};
		};

		// response
		struct PCS_GetBuddyVipInfoResEx : public sox::Marshallable
		{
			enum {uri = (10 << 8) | protocol::IM_VIP_SRVID};

			std::map<uint32_t, std::string> m_mapUid2Info;

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_mapUid2Info);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::inserter(m_mapUid2Info, m_mapUid2Info.begin()));
			}
		};

		struct PCS_UserVipInfoChangeEx : public sox::Marshallable
		{
			enum {uri = (11 << 8) | protocol::IM_VIP_SRVID};

			uint32_t m_uUid;
			PCS_VipInfo m_vip_info;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_vip_info;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_vip_info;
			}
		};

		struct PCS_GrpUserVipInfoChangeEx : public PCS_UserVipInfoChangeEx
		{
			enum {uri = (12 << 8) | protocol::IM_VIP_SRVID};
		};


		// get group user vip info

		struct PCS_GetGrpUserVipInfoEx : public PCS_GetBuddyVipInfo
		{
			enum {uri = (13 << 8) | protocol::IM_VIP_SRVID};
		};

		struct PCS_GetGrpUserVipInfoResEx : public PCS_GetBuddyVipInfoResEx
		{
			enum {uri = (14 << 8) | protocol::IM_VIP_SRVID};
		};

		// get single user vip info
		struct PCS_GetPeerVipInfoEx : public PCS_GetPeerVipInfo
		{
			enum {uri = (15 << 8) | protocol::IM_VIP_SRVID};
		};

		struct PCS_GetPeerVipInfoResEx : public sox::Marshallable
		{
			enum {uri = (16 << 8) | protocol::IM_VIP_SRVID};

			uint32_t m_uUid;
			PCS_VipInfo m_vip_info;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_vip_info;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_vip_info;
			}
		};

		struct PCS_WebVipMoodSyncRes : public sox::Marshallable
		{
			enum {uri = (17 << 8) | protocol::IM_VIP_SRVID};
			uint32_t m_uUid;
			std::string vip_mood_string;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << vip_mood_string;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> vip_mood_string;
			}
		};
		//end new protocol
		//-------------------------------------------------------------------------------------

		/*
		* 获取表情组件白名单
		*/
		struct PCS_GetVipExpWhiteList : public sox::Marshallable
		{
			enum {uri = (18 << 8) | protocol::IM_VIP_SRVID};

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

		struct PCS_GetVipExpWhiteListRes : public sox::Marshallable
		{
			enum {uri = (19 << 8) | protocol::IM_VIP_SRVID};

			std::map< uint32_t, std::vector<uint32_t> > m_mapUid2Lists;

			virtual void marshal(sox::Pack &p) const
			{
				p.push_uint32(uint32_t(m_mapUid2Lists.size()));
				for (std::map<uint32_t, std::vector<uint32_t> >::const_iterator it = m_mapUid2Lists.begin(); it != m_mapUid2Lists.end(); ++it)
				{
					p.push_uint32(uint32_t(it->first));
					sox::marshal_container(p, it->second);
				}
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				uint32_t uSize = p.pop_uint32();
				for (uint32_t i = 0; i < uSize; ++i)
				{
					uint32_t uUid = p.pop_uint32();
					sox::unmarshal_container(p, std::back_inserter(m_mapUid2Lists[uUid]));
				}
			}
		};
		struct OffLineScore : public sox::Marshallable
		{
			uint32_t uid_;
			uint32_t score_;
			uint32_t add_score_;

			virtual void marshal(sox::Pack &p) const
			{
				p << uid_ << score_ << add_score_ ;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uid_ >> score_ >> add_score_;
			}
		};

		// require offline score info
		struct PCS_GetUserOffLineScore : public sox::Marshallable
		{
			enum {uri = (20 << 8) | protocol::IM_VIP_SRVID}; 
			uint32_t from_;
			std::vector<uint32_t> users_;

			virtual void marshal(sox::Pack &p) const
			{
				p << from_ ;
				sox::marshal_container(p, users_);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> from_;
				sox::unmarshal_container(p, std::back_inserter(users_));
			}
		};

		// response offline score info
		struct PCS_GetUserOffLineScoreRes : public sox::Marshallable
		{
			enum {uri = (21 << 8) | protocol::IM_VIP_SRVID}; 
			uint32_t to_;
			std::vector<OffLineScore> users_score_;

			virtual void marshal(sox::Pack &p) const
			{
				p << to_ ;
				sox::marshal_container(p, users_score_);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> to_;
				sox::unmarshal_container(p, std::back_inserter(users_score_));
			}
		};

		struct PCS_GetVipActivity : public sox::Marshallable
		{
			enum {uri = (22 << 8) | protocol::VIP_VICON_SVID};

			uint32_t uid_;

			virtual void marshal(sox::Pack &p) const
			{
				p << uid_;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uid_;
			}
		};
		struct PCS_GetVipActivityRes : public sox::Marshallable
		{
			enum {uri = (23 << 8) | protocol::VIP_VICON_SVID};
			std::vector<uint32_t> m_vec_activity;

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_vec_activity);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::back_inserter(m_vec_activity));
			}

		};
		struct PCS_CancelVipActivity : public sox::Marshallable
		{
			enum {uri = (24 << 8) | protocol::VIP_VICON_SVID};

			uint32_t uid_;
			std::vector<uint32_t> m_vec_activity;

			virtual void marshal(sox::Pack &p) const
			{
				p << uid_;
				sox::marshal_container(p, m_vec_activity);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uid_;
				sox::unmarshal_container(p, std::back_inserter(m_vec_activity));
			}
		};
		struct PCS_CancelVipActivityRes : public sox::Marshallable
		{
			enum {uri = (25 << 8) | protocol::VIP_VICON_SVID};

			uint32_t uid_;
			uint32_t return_code_;

			virtual void marshal(sox::Pack &p) const
			{
				p << uid_<<return_code_;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uid_>>return_code_;
			}
		};

		// 会员通知消息
		struct PCS_VipSysMsg : public sox::Marshallable
		{
			enum {uri = (26 << 8) | protocol::VIP_VICON_SVID};

			uint32_t    m_uMsgId;               // 消息类型ID，供可能的统计需求
			std::string m_strMsg;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uMsgId << m_strMsg;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uMsgId >> m_strMsg;
			}
		};
		
		// 紫钻离线积分
		struct DiamondScore : public sox::Marshallable 
		{
			uint32_t uid_;
			uint32_t score_;
			uint32_t add_score_;

			virtual void marshal(sox::Pack &p) const 
			{
				p << uid_ << score_ << add_score_ ;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> uid_ >> score_ >> add_score_;
			}
		};

		// require offline score info                                                                                                                                                           
		struct PCS_GetDiamondOffLineScore : public sox::Marshallable 
		{
			enum {uri = (22 << 8) | protocol::IM_VIP_SRVID}; 
			uint32_t from_;
			std::vector<uint32_t> users_;

			virtual void marshal(sox::Pack &p) const 
			{                        
				p << from_ ;
				sox::marshal_container(p, users_);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> from_;
				sox::unmarshal_container(p, std::back_inserter(users_));
			}
		};

		// response offline score info                                                                                                                                                          
		struct PCS_GetDiamondOffLineScoreRes : public sox::Marshallable 
		{
			enum {uri = (23 << 8) | protocol::IM_VIP_SRVID};
			uint32_t to_;
			std::vector<DiamondScore> users_score_;

			virtual void marshal(sox::Pack &p) const 
			{
				p << to_ ;
				sox::marshal_container(p, users_score_);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> to_;
				sox::unmarshal_container(p, std::back_inserter(users_score_));
			}
		};

		// 获取好友列表紫钻信息
		struct PCS_GetDiamondInfo : public sox::Marshallable
		{
			enum {uri = (24 << 8) | protocol::IM_VIP_SRVID};

			uint32_t               m_type;       // 0 获取好友， 1 获取单个人 
			std::vector<uint32_t>  m_vecUid;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_type;
				sox::marshal_container(p, m_vecUid);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_type;
				sox::unmarshal_container(p, std::back_inserter(m_vecUid));
			}
		};

		struct PCS_GetDiamonInfoRes : public sox::Marshallable
		{
			enum {uri = (25 << 8) | protocol::IM_VIP_SRVID};

			uint32_t                          m_type;       // 0 获取好友， 1 获取单个人 
			std::map<uint32_t, PlDiamondInfo> m_uid2Info;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_type;
				sox::marshal_container(p, m_uid2Info);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_type;
				sox::unmarshal_container(p, std::inserter(m_uid2Info, m_uid2Info.begin()));
			}
		};

		// ow 会员
		struct PCS_GetOwVipInfo : public sox::Marshallable
		{
			enum {uri = (26 << 8) | protocol::IM_VIP_SRVID};

			uint32_t               m_type;       // 0 获取好友， 1 获取单个人 
			std::vector<uint32_t>  m_vecUid;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_type;
				sox::marshal_container(p, m_vecUid);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_type;
				sox::unmarshal_container(p, std::back_inserter(m_vecUid));
			}
		};

		struct PCS_GetOwVipInfoRes : public sox::Marshallable
		{
			enum {uri = (27 << 8) | protocol::IM_VIP_SRVID};

			uint32_t                          m_type;       // 0 获取好友， 1 获取单个人 
			std::map<uint32_t, OwVipInfo>     m_uid2Info;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_type;
				sox::marshal_container(p, m_uid2Info);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_type;
				sox::unmarshal_container(p, std::inserter(m_uid2Info, m_uid2Info.begin()));
			}
		};
	}
}

#endif
