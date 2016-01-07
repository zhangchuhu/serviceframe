#ifndef IGSEARCH_H 
#define IGSEARCH_H

#include <vector>
#include "IGSearchRecord.h"

namespace protocol
{
	namespace gsearch
	{

		struct IGSearchWatcher
		{
			virtual ~IGSearchWatcher(){};
			
			// for UI, take action when PCS_GetGrpByIdRes is received
			virtual void OnGetGrpByIdRes(const uint32_t &retCode, const protocol::gsearch::CGSearchRecordData &objRecord) = 0;

			// for UI, take action when PCS_GetGrpByNameRes is received
			virtual void OnGetGrpByNameRes(const uint32_t &retCode, const std::vector<protocol::gsearch::CGSearchRecordData> &vecRecords) = 0;
		};

		struct IOpGSearchWatcher//底层需要实现的接口，供UI调用
		{
			virtual ~IOpGSearchWatcher(){};
			virtual void watch(IGSearchWatcher *w) = 0;
			virtual void revoke(IGSearchWatcher *w) = 0;
		};

		struct IGSearch : public virtual IOpGSearchWatcher
		{
			virtual ~IGSearch(){};
			
			//for UI, send PCS_GetGrpById message
			virtual void GetGrpById(uint32_t uGID) = 0;	

			//for UI, send PCS_GetGrpById message
			virtual void GetGrpByName(std::string strGName) = 0;
		};

	}
}
#endif
