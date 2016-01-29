#ifndef _KATAMA_HASH_RING_H_20111202
#define _KATAMA_HASH_RING_H_20111202
#include <map>
#include <vector>
#include <string>
#include <set>

namespace server
{
	namespace dao_proxy
	{
		class KTMHashRing
		{
		public:
			KTMHashRing();
			~KTMHashRing();

		public:
			int init(std::map<int,int>& groupid_weight);
			//return 0:ok 1:can't find -1:hashring error
			int get_groupid(const std::string& hash_str,std::set<int>* p_groupid_mask,int& out_groupid);
			//return 0:ok 1:can't find -1:hashring error
			int get_groupid(uint64_t hash,std::set<int>* p_groupid_mask,int& out_groupid);
		private:
			std::map<uint64_t ,int> hash_groupid;
			std::set<int> groupids;

		};

	}
}

#endif	// _KATAMA_HASH_RING_H_20111202

