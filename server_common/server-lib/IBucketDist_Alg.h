#ifndef _I_BUCKETDIST_ALG_H_
#define _I_BUCKETDIST_ALG_H_

#include <vector>

#include "int_types.h"

struct IBucketDist_Alg{
	virtual ~IBucketDist_Alg(){}
	virtual void Add_Server(uint32_t sid) = 0;
	virtual void Remove_Server(uint32_t sid) = 0;
	virtual uint32_t Dispatch_User(uint32_t uid, uint32_t& sid) = 0;
	virtual bool bucket_to_sid(uint32_t bucket, uint32_t& sid) = 0;
};

class BucketDist_Alg_Aware{
public:
	virtual void Set_BucketDist_Alg(IBucketDist_Alg *alg){
		bucketdist_alg = alg;
	}
protected:
	IBucketDist_Alg *bucketdist_alg;
};

#endif
