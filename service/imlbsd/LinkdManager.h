#ifndef IM_LBS_IMLINKD_INFO_MANAGER
#define IM_LBS_IMLINKD_INFO_MANAGER

#include "common/core/ibase.h"
#include "server_common/server-lib/RouteTarget.h"
#include "common/protocol/PImLbs.h"
#include "ImLbsRequest.h"
#include <list>

namespace server
{
	namespace imlbs
	{
		const uint32_t MAX_NUM_ONLINE_LINKD = 30000;

		// �ͽ����뷽ʽlinkd info
		struct SLinkdInfo
		{
			SLinkdInfo()
			{
				m_uCounter = MAX_NUM_ONLINE_LINKD;
				m_bValid = false;
			}
			SLinkdInfo(bool flag, uint32_t uCount, uint32_t uIp, const std::vector<uint16_t>& vecPorts)
				: m_bValid(flag), m_uCounter(uCount), m_uIp(uIp), m_vecPorts(vecPorts)
			{
			}

			bool m_bValid;
			uint32_t m_uCounter;     // Ŀǰping���������ÿ��ping���Լ�1
			uint32_t m_uIp;
			std::vector<uint16_t>  m_vecPorts;
		};

		// ǿ�Ƶ�½��ʽlinkd Info
		struct SLinkdInfoSameISP
		{
			SLinkdInfoSameISP() {}
			SLinkdInfoSameISP(const std::string& str, const std::vector<uint16_t>& vec)
				: m_strIp(str), m_vecPorts(vec)
			{
			}

			std::string            m_strIp;
			std::vector<uint16_t>  m_vecPorts;
		};


		class CLinkdManager :
			public core::PHClass,
			public core::IRouteTarget
		{
			typedef std::vector<uint32_t> VecLinkd_T;
			VecLinkd_T m_validLinkd;      // �ͽ����뷽ʽ������imlinkd
			VecLinkd_T m_allLinkd;        // ����������imlinkd������ǿ�Ƶ�½��ʽ

			uint32_t  m_uIndex;

			ImLbsRequest* m_pLbsRequest;

		public:

			CLinkdManager();

			// ����imlinkd��������
			void SetImLinkdInfo(uint32_t uLinkdId, uint32_t uOnline);

			// imlinkd��ʼ������ͬ���imlbsd
			void SetImLinkdInfoByGroup(uint32_t uLinkdId, uint32_t uOnline, uint32_t uIp, const std::vector<uint16_t>& vecPorts);

			// imlinkd��ʼ��ʱ����ͬISP��imlbsd
			void SetImLinkdInfoByISP(uint32_t uLinkdId, const std::string& strIp, const std::vector<uint16_t>& vecPorts);

			void DelImLinkdByGroup(uint32_t uLinkdId);

			void DelImLinkdByISP(uint32_t uLinkdId);

			// client ����imlinkd��Ϣ�� ��Э�飨�ж�imlinkd�Ƿ�ﵽ�������ޣ�
			bool GetImlinkInfo2(uint32_t& uGroupId, uint32_t &ip, std::vector<uint16_t> &ports);
		
			// client ����imlinkd��Ϣ�� ��Э�飨���ж�imlinkd�Ƿ�ﵽ�������ޣ�
			bool GetImlinkInfo(std::string &ip, std::vector<uint16_t> &ports);

			void SetImLbsRequest(ImLbsRequest* p) { m_pLbsRequest = p; }

			void Trace();


			// map< linkdId, Info >
			std::map<uint32_t, SLinkdInfo> m_mapLinkdInfo;

			// ��Э�飬ǿ�Ƶ�½��ʽimlinkd ������ ������������ֿ�����Ҫ��Ϊ�˷���ͳ�Ƹû���imlinkd��������
			// �Ǳ����imlinkd���������������
			std::map<uint32_t, SLinkdInfoSameISP> m_mapLinkdInfoSameISP;

		private:

			void __AdjustLinkd(uint32_t uLindId, SLinkdInfo& res);
			void __RemoveValue(VecLinkd_T& vec, uint32_t value);

			uint32_t m_uNotifyCounter;

		};
			
	}
}

#endif
