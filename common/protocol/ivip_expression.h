#pragma once
#include "common/protocol/const.h"
#include "common/int_types.h"
#include "common/packet.h"

namespace protocol
{
	namespace vip
	{
		// ���α�����Ϣ
		struct VipExpInfo : public sox::Marshallable
		{
			uint32_t    m_uGrpId;         // ��������
			std::string m_strUrl;         // URL
			std::string m_strEx;          // ������Ϣ����ݼ���������

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uGrpId << m_strUrl << m_strEx;
			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uGrpId >> m_strUrl >> m_strEx;
			}
		};

		// �����ƹ�
		struct VipExpPopInfo : public sox::Marshallable
		{
			uint32_t     m_uGrpId;         // �������ID
			std::string  m_strKey;         // ����key

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
			ENUM_VIP_EXP_RES_NOTVIP,           // �û�����vip
			ENUM_VIP_EXP_RES_OVER_LIMIT,       // �����ȼ�����
			ENUM_VIP_EXP_RES_BADCACHE          // �ϴ������г��ֲ����ڵķ�����������ʱ
			                                   // �������ʶ���ػ�����ܴ���
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

			// ������������Ӧ
			virtual void onBatchAddExpRes(const std::vector<VipExpInfo>& expInfos, const EVipExpressionResCode& resCode, const uint32_t& uNewVersion) = 0;
			virtual void onBatchDelExpRes(const std::vector<VipExpInfo>& expInfos, const EVipExpressionResCode& resCode, const uint32_t& uNewVersion) = 0;
			virtual void onBatchUpdateExpRes(const std::vector<VipExpInfo>& expInfos, const EVipExpressionResCode& resCode, const uint32_t& uNewVersion) = 0;

			// ����İ汾��Ϣ
			virtual void getExpVersionRes(const uint32_t& uNewVersion, const uint32_t& uLimit) = 0;

			// ͬ�����α������Ӧ
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

			// ��ӷ���
			virtual void addExpGroup(const std::string& strName) = 0;

			// ȡ�������������
			virtual void delExpGroup(uint32_t uGroupId) = 0;

			// ���ķ�����
			virtual void renameExpGroup(uint32_t uGroupId, const std::string& strNewName) = 0;

			// ��ӡ�ɾ�����������α���
			virtual void batchAddExp(const std::vector<VipExpInfo>& expInfos) = 0;
			virtual void batchDelExp(const std::vector<VipExpInfo>& expInfos) = 0;
			virtual void batchUpdateExp(const std::vector<VipExpInfo>& expInfos) = 0;

			// ȷ���û���vip�󣬻�ȡ���α���İ汾��Ϣ
			// �޻���ʱ��uCurVersion Ϊ0
			virtual void getExpVersion(uint32_t uCurVersion) = 0;

			// ͬ�����α���
			virtual void syncExpression() = 0;
		};
	}
}


