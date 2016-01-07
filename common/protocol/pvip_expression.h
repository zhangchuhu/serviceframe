#pragma once

#include "common/protocol/const.h"
#include "common/int_types.h"
#include "common/core/base_svid.h"
#include "common/protocol/ivip_expression.h"

namespace protocol
{
	namespace vip
	{
		// 添加漫游表情分组
		struct PCS_AddVipExpGrp : public sox::Marshallable
		{
			enum {uri = (1 << 8) | protocol::VIP_EXPRESSION_SVID};

			std::string m_strName;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strName;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_strName;
			}
		};

		struct PCS_AddVipExpGrpRes : public sox::Marshallable
		{
			enum {uri = (2 << 8) | protocol::VIP_EXPRESSION_SVID};

			uint32_t    m_uVersion;
			uint32_t    m_uGrpId;
			std::string m_strName;
			EVipExpressionResCode m_resCode;
		
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uVersion << m_uGrpId << m_strName;
				p.push_uint32(m_resCode);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uVersion >> m_uGrpId >> m_strName;
				m_resCode = (EVipExpressionResCode)p.pop_uint32();
			}
		};

		// 取消分组表情漫游

		struct PCS_DelVipExpGrp : public sox::Marshallable
		{
			enum {uri = (3 << 8) | protocol::VIP_EXPRESSION_SVID};

			uint32_t m_uGrpId;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpId;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpId;
			}
		};

		struct PCS_DelVipExpGrpRes : public sox::Marshallable
		{
			enum {uri = (4 << 8) | protocol::VIP_EXPRESSION_SVID};

			uint32_t    m_uVersion;
			uint32_t    m_uGrpId;
			EVipExpressionResCode m_resCode;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uVersion << m_uGrpId;
				p.push_uint32(m_resCode);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uVersion >> m_uGrpId;
				m_resCode = (EVipExpressionResCode)p.pop_uint32();
			}
		};

		// 重命名分组

		struct PCS_RenameVipExpGrp : public sox::Marshallable
		{
			enum {uri = (5 << 8) | protocol::VIP_EXPRESSION_SVID};

			uint32_t    m_uGrpId;
			std::string m_strName;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpId << m_strName;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpId >> m_strName;
			}
		};

		struct PCS_RenameVipExpGrpRes : public sox::Marshallable
		{
			enum {uri = (6 << 8) | protocol::VIP_EXPRESSION_SVID};

			uint32_t    m_uGrpId;
			uint32_t    m_uVersion;
			std::string m_strName;
			EVipExpressionResCode m_resCode;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uGrpId << m_strName << m_uVersion;
				p.push_uint32(m_resCode);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uGrpId >> m_strName >> m_uVersion;
				m_resCode = (EVipExpressionResCode)p.pop_uint32();
			}
		};

		// 添加表情

		struct PCS_AddVipExp : public sox::Marshallable
		{
			enum {uri = (7 << 8) | protocol::VIP_EXPRESSION_SVID};

			std::vector<VipExpInfo> m_vecExpInfo;

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_vecExpInfo);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::back_inserter(m_vecExpInfo));
			}
		};

		struct PCS_AddVipExpRes : public sox::Marshallable
		{
			enum {uri = (8 << 8) | protocol::VIP_EXPRESSION_SVID};

			uint32_t                m_uVersion;
			EVipExpressionResCode   m_resCode;
			std::vector<VipExpInfo> m_vecExpInfo;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uVersion;
				p.push_uint32(m_resCode);
				sox::marshal_container(p, m_vecExpInfo);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uVersion;
				m_resCode = (EVipExpressionResCode)p.pop_uint32();
				sox::unmarshal_container(p, std::back_inserter(m_vecExpInfo));
			}
		};

		// 删除表情

		struct PCS_DelVipExp : public PCS_AddVipExp
		{
			enum {uri = (9 << 8) | protocol::VIP_EXPRESSION_SVID};
		};

		struct PCS_DelVipExpRes : public PCS_AddVipExpRes
		{
			enum {uri = (10 << 8) | protocol::VIP_EXPRESSION_SVID};
		};

		// 更新表情

		struct PCS_UpdateVipExp : public PCS_AddVipExp
		{
			enum {uri = (11 << 8) | protocol::VIP_EXPRESSION_SVID};
		};

		struct PCS_UpdateVipExpRes : public PCS_AddVipExpRes
		{
			enum {uri = (12 << 8) | protocol::VIP_EXPRESSION_SVID};
		};

		// 获取最新的表情版本

		struct PCS_GetVipExpVersion : sox::Voidmable
		{
			enum {uri = (13 << 8) | protocol::VIP_EXPRESSION_SVID};
		};

		struct PCS_GetVipExpVersionRes : public sox::Marshallable
		{
			enum {uri = (14 << 8) | protocol::VIP_EXPRESSION_SVID};

			uint32_t m_uVersion;
			uint32_t m_uLimit;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uVersion << m_uLimit;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uVersion >> m_uLimit;
			}
		};

		// 同步漫游表情信息

		struct PCS_SyncVipExp : public sox::Voidmable
		{
			enum {uri = (15 << 8) | protocol::VIP_EXPRESSION_SVID};
		};

		struct PCS_SyncVipExpRes : public sox::Marshallable
		{
			enum {uri = (16 << 8) | protocol::VIP_EXPRESSION_SVID};

			uint32_t m_uVersion;
			std::map<uint32_t, std::string>  m_mapGrpId2Name;
			std::vector<VipExpInfo>          m_vecExpInfos;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uVersion;
				sox::marshal_container(p, m_vecExpInfos);
				sox::marshal_container(p, m_mapGrpId2Name);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uVersion;
				sox::unmarshal_container(p, std::back_inserter(m_vecExpInfos));
				sox::unmarshal_container(p, std::inserter(m_mapGrpId2Name, m_mapGrpId2Name.begin()));
			}
		};

		// 获取表情推广信息
		
		struct PCS_GetExpPopu : public sox::Voidmable
		{
			enum {uri = (17 << 8) | protocol::VIP_EXPRESSION_SVID};
		};

		struct PCS_GetExpPopuRes : public sox::Marshallable
		{
			enum {uri = (18 << 8) | protocol::VIP_EXPRESSION_SVID};

			std::vector<VipExpPopInfo> m_vecExpInfo;

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_vecExpInfo);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::back_inserter(m_vecExpInfo));
			}
		};

		// 表情配置文件的版本号

		struct PCS_GetExpConfVersion : public sox::Voidmable
		{
			enum {uri = (19 << 8) | protocol::VIP_EXPRESSION_SVID};
		};

		struct PCS_GetExpConfVersionRes : public sox::Marshallable
		{
			enum {uri = (20 << 8) | protocol::VIP_EXPRESSION_SVID};

			uint32_t m_uVersion;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uVersion;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uVersion;
			}
		};

		// 签名表情配置文件
		struct PCS_GetSignExpConf : public sox::Voidmable
		{
			enum {uri = (21 << 8) | protocol::VIP_EXPRESSION_SVID};
		};

		struct PCS_GetSignExpConfRes : public sox::Marshallable
		{
			enum {uri = (22 << 8) | protocol::VIP_EXPRESSION_SVID};
			uint32_t m_uVersion;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uVersion;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uVersion;
			}
		};
	}
}
