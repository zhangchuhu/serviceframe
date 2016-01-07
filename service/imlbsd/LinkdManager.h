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

		// 就近接入方式linkd info
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
			uint32_t m_uCounter;     // 目前ping入的人数，每次ping入自加1
			uint32_t m_uIp;
			std::vector<uint16_t>  m_vecPorts;
		};

		// 强制登陆方式linkd Info
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
			VecLinkd_T m_validLinkd;      // 就近接入方式，可用imlinkd
			VecLinkd_T m_allLinkd;        // 本网的所有imlinkd，用于强制登陆方式

			uint32_t  m_uIndex;

			ImLbsRequest* m_pLbsRequest;

		public:

			CLinkdManager();

			// 更新imlinkd在线人数
			void SetImLinkdInfo(uint32_t uLinkdId, uint32_t uOnline);

			// imlinkd初始化报给同组的imlbsd
			void SetImLinkdInfoByGroup(uint32_t uLinkdId, uint32_t uOnline, uint32_t uIp, const std::vector<uint16_t>& vecPorts);

			// imlinkd初始化时报给同ISP的imlbsd
			void SetImLinkdInfoByISP(uint32_t uLinkdId, const std::string& strIp, const std::vector<uint16_t>& vecPorts);

			void DelImLinkdByGroup(uint32_t uLinkdId);

			void DelImLinkdByISP(uint32_t uLinkdId);

			// client 请求imlinkd信息， 新协议（判断imlinkd是否达到承载上限）
			bool GetImlinkInfo2(uint32_t& uGroupId, uint32_t &ip, std::vector<uint16_t> &ports);
		
			// client 请求imlinkd信息， 旧协议（不判断imlinkd是否达到承载上限）
			bool GetImlinkInfo(std::string &ip, std::vector<uint16_t> &ports);

			void SetImLbsRequest(ImLbsRequest* p) { m_pLbsRequest = p; }

			void Trace();


			// map< linkdId, Info >
			std::map<uint32_t, SLinkdInfo> m_mapLinkdInfo;

			// 旧协议，强制登陆方式imlinkd 容器。 和上面的容器分开，主要是为了方便统计该机房imlinkd在线人数
			// 非本组的imlinkd在线人数不会更新
			std::map<uint32_t, SLinkdInfoSameISP> m_mapLinkdInfoSameISP;

		private:

			void __AdjustLinkd(uint32_t uLindId, SLinkdInfo& res);
			void __RemoveValue(VecLinkd_T& vec, uint32_t value);

			uint32_t m_uNotifyCounter;

		};
			
	}
}

#endif
