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

		// ��hash imonline�飬��hash ����imonline
		uint32_t DoubleHash(uint32_t uUid);

		// ��imonline�����hash
		uint32_t HashBySrvGrp(uint32_t key);

		// ������imonline����hash
		uint32_t HashInOneSrvGrp(uint32_t uUid, uint32_t uGroupId);

		// ����uid, ��uGroupId���ڵ�imonline����hash
		void BatchHashInOneSrvGrp(uint32_t uGroupId, const std::set<uint32_t>& setUid, std::map< uint32_t, std::set<uint32_t> >& mapSrvUids);

		// ����uid����ÿ��online���imonline����hash��uGroupId�����
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
		std::set<uint32_t>  m_setGrpId;               // ֻ��Ϊ�˲����������ظ����group
		
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
