#ifndef _BUCKETDIST_ALG_H_
#define _BUCKETDIST_ALG_H_

#include <map>
#include <set>
#include <vector>

#include "int_types.h"
#include "IBucketDist_Alg.h"
#include "ILocateUser.h"


class BucketDist_Alg : public IBucketDist_Alg, public ILocate_User_Aware{
public:
	virtual void Add_Server(uint32_t sid);
	virtual void Remove_Server(uint32_t sid);
	virtual uint32_t Dispatch_User(uint32_t uid, uint32_t& sid);
	virtual bool bucket_to_sid(uint32_t bucket, uint32_t& sid);

	void clear_all();
	bool check_alg() const;

private:
	uint32_t hashUid(uint32_t uid);

	typedef std::map<uint32_t, std::set<uint32_t> > sid_bucks_t;  //sid ---> buckets_set
	sid_bucks_t _sid_bucks_map;

	typedef std::map<uint32_t, uint32_t> buck_sid_t;  //bucket ---> sid
	buck_sid_t _buck_sid_map;
};
#endif
