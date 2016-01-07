#pragma once
#include "common/protocol/const.h"
#include "common/int_types.h"
#include "common/packet.h"

namespace protocol
{
	namespace vip
	{
		// 漫游表情信息
		struct VipExpInfo : public sox::Marshallable
		{
			uint32_t    m_uGrpId;         // 所属分组
			std::string m_strUrl;         // URL
			std::string m_strEx;          // 附加信息，快捷键、表情名

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uGrpId << m_strUrl << m_strEx;
			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGrpId >> m_strUrl >> m_strEx;
			}
		};

		// 表情推广
		struct VipExpPopInfo : public sox::Marshallable
		{
			uint32_t     m_uGrpId;         // 表情组件ID
			std::string  m_strKey;         // 表情key

			VipExpPopInfo() : m_uGrpId(0) {}
			VipExpPopInfo(uint32_t uid, const std::string& key) :
				m_uGrpId(uid), m_strKey(key) 
			{}

			bool operator==(const VipExpPopInfo& t) const
			{
				return (m_uGrpId == t.m_uGrpId) && (m_strKey == t.m_strKey);
			}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uGrpId << m_strKey;
			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGrpId >> m_strKey;
			}
		};

		enum EVipExpressionResCode
		{
			ENUM_VIP_EXP_RES_OK = 0,
			ENUM_VIP_EXP_RES_NOTVIP,           // 用户不是vip
			ENUM_VIP_EXP_RES_OVER_LIMIT,       // 超过等级限制
			ENUM_VIP_EXP_RES_BADCACHE          // 上传操作中出现不存在的分组或其他情况时
			                                   // 返回码标识本地缓存可能错误
		};

		class IVipExpressionWatcher
		{
		public:

			virtual ~IVipExpressionWatcher() {}

			virtual void onAddExpGrpRes(const uint32_t& uNewVersion, 
				const uint32_t& uGrpId, 
				const std::string& grpName,
				const EVipExpressionResCode& resCode) = 0;

			virtual void onDelExpGrpRes(const uint32_t& uNewVersion, 
				const uint32_t& uGrpId,
				const EVipExpressionResCode& resCode) = 0;

			virtual void onRenameExpGrpRes(const uint32_t& uNewVersion, 
				const uint32_t& uGrpId, 
				const std::string& grpName,
				const EVipExpressionResCode& resCode) = 0;

			// 批量操作的响应
			virtual void onBatchAddExpRes(const std::vector<VipExpInfo>& expInfos, const EVipExpressionResCode& resCode, const uint32_t& uNewVersion) = 0;
			virtual void onBatchDelExpRes(const std::vector<VipExpInfo>& expInfos, const EVipExpressionResCode& resCode, const uint32_t& uNewVersion) = 0;
			virtual void onBatchUpdateExpRes(const std::vector<VipExpInfo>& expInfos, const EVipExpressionResCode& resCode, const uint32_t& uNewVersion) = 0;

			// 表情的版本信息
			virtual void getExpVersionRes(const uint32_t& uNewVersion, const uint32_t& uLimit) = 0;

			// 同步漫游表情的响应
			virtual void syncExpressionRes(const uint32_t& uNewVersion, 
				const std::map<uint32_t, std::string>& mapId2Name, const std::vector<VipExpInfo>& vecExpInfo) = 0;
		};

		class IVipExpressionWatcherBase
		{
		public:

			virtual ~IVipExpressionWatcherBase() {}

			virtual void watch(IVipExpressionWatcher* p) = 0;
			virtual void revoke(IVipExpressionWatcher* p) = 0;
		};

		class IVipExpression : public virtual IVipExpressionWatcherBase
		{
		public:

			virtual ~IVipExpression() {}

			// 添加分组
			virtual void addExpGroup(const std::string& strName) = 0;

			// 取消分组表情漫游
			virtual void delExpGroup(uint32_t uGroupId) = 0;

			// 更改分组名
			virtual void renameExpGroup(uint32_t uGroupId, const std::string& strNewName) = 0;

			// 添加、删除、更改漫游表情
			virtual void batchAddExp(const std::vector<VipExpInfo>& expInfos) = 0;
			virtual void batchDelExp(const std::vector<VipExpInfo>& expInfos) = 0;
			virtual void batchUpdateExp(const std::vector<VipExpInfo>& expInfos) = 0;

			// 确定用户是vip后，获取漫游表情的版本信息
			// 无缓存时，uCurVersion 为0
			virtual void getExpVersion(uint32_t uCurVersion) = 0;

			// 同步漫游表情
			virtual void syncExpression() = 0;
		};
	}
}


