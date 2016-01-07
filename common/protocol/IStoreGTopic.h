/*
Note: In protocol, all data with std::string type must be coded with UTF8
*/
#ifndef ISTOREGTOPIC_H 
#define ISTOREGTOPIC_H

#include <vector>
#include <map>
#include "pgtopicbase.h"

using namespace protocol::gtopic;

namespace protocol
{
	namespace store
	{

		struct IStoreGTopicWatcher
		{
			virtual ~IStoreGTopicWatcher(){};
			
			// for UI, take action when PCS_UserGTopicCountRes is received
			virtual void OnUserGTopicCountRes(const uint32_t& uGroupId, const uint32_t& uFolderId, const std::map<uint32_t, uint32_t>& vecTopicCount) = 0;

			// for UI, take action when PCS_TokenRes is received
			virtual void OnGetTokenRes(const uint32_t &bChecked) = 0;

			// for UI, take action when PCS_UserGTopicCountAllRes is received
			virtual void OnGTopicCountAllRes(const std::map<uint32_t, protocol::gtopic::STopicReadInfoA>&  mapTopicCount) = 0;
		};

		struct IOpStoreGTopicWatcher//底层需要实现的接口，供UI调用
		{
			virtual ~IOpStoreGTopicWatcher(){};
			virtual void watch(IStoreGTopicWatcher *w) = 0;
			virtual void revoke(IStoreGTopicWatcher *w) = 0;
		};

		struct IStoreGTopic : public virtual IOpStoreGTopicWatcher
		{
			virtual ~IStoreGTopic(){};

			//for UI, send PCS_UserGTopicCountReq message
			virtual void DoUserGTopicCountReq(uint32_t uGroupId, 
														 uint32_t uFolderId, 
														 const std::vector<uint32_t> &vecTopicId) = 0;
			
			// for UI, send PCS_UserGTopicsReadInfo message
			virtual void DoSendTopicsReadInfo(const std::vector<protocol::gtopic::STopicReadInfo>& vecTopics) = 0;

			//for UI, send PCS_TokenReq message
			virtual void DoGetToken(uint32_t uToken) = 0;

			// for UI, send PCS_UserGTopicCountAllReq message
			virtual void DoGetGTopicCountAll(uint32_t uLastTime, uint32_t uVersion) = 0;
		};
	}
}
#endif


