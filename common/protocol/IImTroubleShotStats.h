#pragma once

#include "common/protocol/PImreqtimeoutType.h"
#include "common/protocol/IPImStatistics.h"

using namespace std;
using namespace protocol::imstatistics;
using namespace protocol::imreqtimeout;

namespace protocol
{
	namespace imtroubleshotstats
	{

		struct IImTroubleShot
		{

			/**
			 * IM screen shot statics.
			 **/
			virtual void DoImageStaticsShotNotify(const CImageStaticsShotFinalSuccess &imageStaticsObj){};//TODO: pure virual !!!
			virtual void DoImageStaticsShotNotify(const CImageStaticsShotSingle &imageStaticsObj){};//TODO: pure virual !!!
			virtual void DoImageStaticsShotNotify(const CImageStaticsShotFinalFail &imageStaticsObj){};//TODO: pure virual !!!

			/**
			 * Screen shot trouble stats.
			 **/
			virtual void DoImageTroubleShotNotify(const CImageTroubleShot &ImageTroubleObj) = 0;

			/**
			 * KPI - GrpBaseInfo, GrpMemberInfo
			 **/
			 virtual void DoSetTimeout(E_KPI_EVENT_TYPE eKpiType, const std::string& strKey) = 0;
			 virtual void DoSetExpectCount(E_KPI_EVENT_TYPE eKpiType, const std::string& strKey, const uint32_t uExpCount) = 0;
			 virtual void DoUnSetTimeout(E_KPI_EVENT_TYPE eKpiType, const std::string& strKey, const uint32_t uResCount) = 0;
			 virtual string DoGenFSimPropsKey(const uint32_t uUid) = 0;
			 virtual string DoGenGSimPropsKey(const uint32_t uUid) = 0;
			 virtual string DoGenGMemOnlineStateKey(const uint32_t uMyUid, const uint32_t uGFid) = 0;
			 virtual string DoGenGMemSimInfoKey(const uint32_t uMyUid, const uint32_t uGFid) = 0;
				 
			/**
			 * KPI - Action Stats
			 *	1 - P2P Transmist
			 *	2 - IM Voice.
			 *	3 - Image
			 **/
			 virtual void DoActionStatsFailNotify(E_KPI_EVENT_TYPE eKpiType, const std::string& strData) = 0;
			 virtual void DoActionStatsSuccessNotify(void) = 0;
		};

	}
}

