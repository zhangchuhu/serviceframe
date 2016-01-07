#ifndef IM_ONLINE_HASH_HELPER_H_INCLUDE
#define  IM_ONLINE_HASH_HELPER_H_INCLUDE

#include <map>
#include <set>
#include <vector>
#include "server_common/server-lib/iclient.h"
#include "common/protocol/const.h"
#include "server_common/server-lib/ConsistentHash.h"
#include "server_common/server-lib/id_def.h"


namespace server
{
	class CImOnHashHelper :
		public core::IDaemonClientAwareW
	{
	public:

		CImOnHashHelper() 
		{
			m_ISP = CTL;
			m_strSuffix = SUFFIX_IMONLINED;
		}
		CImOnHashHelper(ISPType netType, const std::string& strSuf) 
		{
			m_ISP = CTL;
			m_strSuffix = SUFFIX_IMONLINED;
		}
		~CImOnHashHelper();

		// 先hash imonline组，再hash 组内imonline
		uint32_t DoubleHash(uint32_t uUid);

		// 对imonline组进行hash
		uint32_t HashBySrvGrp(uint32_t key);

		// 对组内imonline进行hash
		uint32_t HashInOneSrvGrp(uint32_t uUid, uint32_t uGroupId);

		// 根据uid, 对uGroupId组内的imonline进行hash
		void BatchHashInOneSrvGrp(uint32_t uGroupId, const std::set<uint32_t>& setUid, std::map< uint32_t, std::set<uint32_t> >& mapSrvUids);

		// 跟据uid，对每个online组的imonline进行hash，uGroupId组除外
		void BatchHashInAllSrvGrpEx(uint32_t uSrvGrp, const std::set<uint32_t>& setUid, std::map<uint32_t, std::set<uint32_t> >& mapSrv2Uids);

		void BatchHashInAllSrvGrpEx(uint32_t uSrvGrp, uint32_t uUid, std::set<uint32_t>& setSrvs);

		void AddHashNode(uint32_t uServerId, uint32_t uGroupId, ISPType net = CTL);

		void RemoveHashNode(uint32_t uServerId, uint32_t uGroupId);

	public:
		virtual void onRefreshSV();
		virtual void onRecoverRefreshSV();
		virtual void onAddSV(const sdaemon::FavorStrSV & fs);
		virtual void onServerRemovedSV(const sdaemon::FavorStrSV & fs);
		virtual void onServerStatusChangeSV(const sdaemon::FavorStrSV & fs);

	private:

		typedef std::map<uint32_t, uint32_t> MapHashCircle_T;

		struct PairHashMap_T
		{
			MapHashCircle_T second;
			CConsistentHash first;

			PairHashMap_T() :  first(second, ENUM_FUNC_NEWHASH)
			{
			}
			PairHashMap_T& operator=(const PairHashMap_T& t)
			{
				second = t.second;
				return *this;
			}
			PairHashMap_T(const PairHashMap_T& t) : second(t.second), first(second, ENUM_FUNC_NEWHASH)
			{
			}
		};

		PairHashMap_T       m_hashGroup;
		std::set<uint32_t>  m_setGrpId;               // 只是为了不遍历，不重复添加group
		
		// map<group, CConsistentHash-online>
		std::map<uint32_t, PairHashMap_T*> m_mapGrpConsisHash;

		ISPType           m_ISP;
		std::string       m_strSuffix;
	};

	class CImOnHashHelperAware
	{
	public:
		void SetOnlineHashHelper(CImOnHashHelper* p)
		{
			m_pImOnlineHashHelper = p;
		}

		CImOnHashHelper* m_pImOnlineHashHelper;
	};
}

#endif
