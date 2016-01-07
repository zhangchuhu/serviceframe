#include "BucketDist_Alg.h"
#include "core/sox/logger.h"
#include "res_code.h"

#define USER_BUCKET_NUM 9182

void BucketDist_Alg::Add_Server(uint32_t sid)
{
	log(Debug, "BucketDist_Alg::Add_Server: %d ", sid);
	if(_buck_sid_map.empty()){
		_sid_bucks_map[sid];
		return;
	}

	sid_bucks_t::iterator it1 = _sid_bucks_map.find(sid);
	if (it1 == _sid_bucks_map.end()){
		sid_bucks_t::iterator it2 = _sid_bucks_map.begin();
		uint32_t base_sid = it2->first;
		uint32_t base_bucks_num = it2->second.size();
		std::set<uint32_t>& move_in_buckets = _sid_bucks_map[sid];
		bool need_more = true;
		for (; it2 != _sid_bucks_map.end(); ++it2){
			if (it2->first != sid){
				if (it2->second.size() > base_bucks_num){
					if (it2->second.size() <= 1)
						continue;

					std::set<uint32_t>& move_out_buckets = _sid_bucks_map[it2->first];
					uint32_t delta = (it2->second.size() - base_bucks_num);
					if(delta > 0){
						uint32_t i = 0;
						for(; i < delta; ++i){
							std::set<uint32_t>::iterator it3 = move_out_buckets.begin();
							uint32_t transfer_bucket = *it3;
							move_in_buckets.insert(*it3);
							//_buck_sid_map.erase(*it3);
							_buck_sid_map[*it3] = sid;
							move_out_buckets.erase(it3);

							//notify this linkd(it2->first) to force users in bucket(*it3) to redirect to linkd(sid)
							_locate_user->RedirectUser(transfer_bucket, it2->first);
						}
					}

					if(move_in_buckets.size() >= move_out_buckets.size()){
						need_more = false;
						break;
					}
				}
				else if (it2->second.size() < base_bucks_num){
					if (base_bucks_num <= 1)
						continue;

					std::set<uint32_t>& move_out_buckets = _sid_bucks_map[base_sid];
					uint32_t delta = (base_bucks_num - it2->second.size());
					if(delta > 0){
						uint32_t i = 0;
						for(; i < delta; ++i){
							std::set<uint32_t>::iterator it3 = move_out_buckets.begin();
							uint32_t transfer_bucket = *it3;
							move_in_buckets.insert(*it3);
							_buck_sid_map[*it3] = sid;
							move_out_buckets.erase(it3);

							//notify this linkd(base_sid) to force users in bucket(*it3) to redirect to linkd(fs.serverId)
							_locate_user->RedirectUser(transfer_bucket, base_sid);
						}
					}

					base_bucks_num = move_out_buckets.size();
					if(move_in_buckets.size() >= move_out_buckets.size()){
						need_more = false;
						break;
					}
				}
				else
					continue;
			}

		}

		if(need_more){
			it2 = _sid_bucks_map.begin();
			while(it2 != _sid_bucks_map.end()){
				if(it2->first != sid){
					if (it2->second.size() <= 1)
						break;

					std::set<uint32_t>& move_out_buckets = _sid_bucks_map[it2->first];
					std::set<uint32_t>::iterator it3 = move_out_buckets.begin();
					uint32_t transfer_bucket = *it3;
					move_in_buckets.insert(*it3);
					//_buck_sid_map.erase(*it3);
					_buck_sid_map[*it3] = sid;
					move_out_buckets.erase(it3);

					//notify this linkd(it2->first) to force users in bucket(*it3) to redirect to linkd(fs.serverId)
					_locate_user->RedirectUser(transfer_bucket, it2->first);

					if(move_in_buckets.size() >= move_out_buckets.size())
						break;
				}

				if(++it2 == _sid_bucks_map.end())
					it2 = _sid_bucks_map.begin();
			}
		}	
	}
	else{
		log(Error, "BucketDist_Alg::Add_Server: linkd: %u already exisit.", sid);
	}
}
	
void BucketDist_Alg::Remove_Server(uint32_t sid)
{
	log(Debug, "BucketDist_Alg::Remove_Server: %u", sid);
	sid_bucks_t::iterator it = _sid_bucks_map.find(sid);
	if (it != _sid_bucks_map.end()){
		std::set<uint32_t>& buckets = it->second;
		std::set<uint32_t>::iterator it2 = buckets.begin();
		for (; it2 != buckets.end(); ++it2){
			buck_sid_t::iterator it3 = _buck_sid_map.find(*it2);
			if (it3 != _buck_sid_map.end()){
				_buck_sid_map.erase(it3);
			}
		}
		_sid_bucks_map.erase(it);
	}
	else{
		log(Error, "BucketDist_Alg::Remove_Server(): cannot find sid: %u", sid);
	}
}
	
	
uint32_t BucketDist_Alg::Dispatch_User(uint32_t uid, uint32_t& linkdid)
{
	log(Debug, "BucketDist_Alg::Dispatch_User: %u", uid);
	uint32_t bucket = hashUid(uid);
	buck_sid_t::iterator it1 = _buck_sid_map.find(bucket);
	if (it1 != _buck_sid_map.end()){ //the bucket is already in use
		linkdid = (*it1).second;
		log(Debug, "BucketDist_Alg::Dispatch_User: alloc bucket: %u to sid:%u", bucket,  linkdid);
		_locate_user->AllocBuck(linkdid, bucket);
		log(Debug, "BucketDist_Alg::Dispatch_User: uid: %u, bucket %u, login sid: %u", uid,  bucket,  linkdid);
		return RES_SUCCESS;
	}
	else{ //the bucket is not in use
		if(!_sid_bucks_map.empty()){
			sid_bucks_t::iterator it2 = _sid_bucks_map.begin();
			uint32_t sid_with_min_buc = (*it2).first;
			uint32_t min_buc = (*it2).second.size();
			for (; it2 != _sid_bucks_map.end(); ++it2){
				if((*it2).second.size() < min_buc)
					sid_with_min_buc = (*it2).first;
			}

			std::set<uint32_t>& buckets = _sid_bucks_map[sid_with_min_buc];
			std::set<uint32_t>::iterator it3 = buckets.find(bucket);
			if (it3 == buckets.end()){
				buckets.insert(bucket);
				_buck_sid_map[bucket] = sid_with_min_buc;
			}

			linkdid = sid_with_min_buc;
			log(Debug, "BucketDist_Alg::Dispatch_User: alloc bucket: %u to sid: %u", bucket, linkdid);
			_locate_user->AllocBuck(linkdid, bucket);
			log(Debug, "BucketDist_Alg::Dispatch_User: uid: %u, bucket: %u, login sid: %u", uid,  bucket,  linkdid);
			return RES_SUCCESS;
		}else{ //no linkd
			log(Warn, "BucketDist_Alg::Dispatch_User: uid: %u no linkd now.", uid);
			return RES_EUNKNOWN;
		}
	}
}


bool BucketDist_Alg::bucket_to_sid(uint32_t bucket, uint32_t& linkd)
{
	buck_sid_t::iterator it = _buck_sid_map.find(bucket);
	if (it != _buck_sid_map.end()){
		log(Debug, "bucket_to_sid: bucket: %u sid: %u ", bucket, it->second);
		linkd = it->second;
		return true;
	}
	else{ //the bucket is not in use
		if(!_sid_bucks_map.empty()){
			sid_bucks_t::iterator it2 = _sid_bucks_map.begin();
			uint32_t sid_with_min_buc = (*it2).first;
			uint32_t min_buc = (*it2).second.size();
			for (; it2 != _sid_bucks_map.end(); ++it2){
				if((*it2).second.size() < min_buc)
					sid_with_min_buc = (*it2).first;
			}

			std::set<uint32_t>& buckets = _sid_bucks_map[sid_with_min_buc];
			std::set<uint32_t>::iterator it3 = buckets.find(bucket);
			if (it3 == buckets.end()){
				buckets.insert(bucket);
				_buck_sid_map[bucket] = sid_with_min_buc;
			}

			linkd = sid_with_min_buc;
			log(Debug, "BucketDist_Alg: bucket: %u to sid: %u", bucket, linkd);
			//_locate_user->AllocBuck(linkd, bucket);
			return true;
		}else{ //no linkd
			log(Warn, "BucketDist_Alg: no server now.");
			return false;
		}
	}
}

void BucketDist_Alg::clear_all()
{
	_sid_bucks_map.clear();
	_buck_sid_map.clear();
}


uint32_t BucketDist_Alg::hashUid(uint32_t uid){
	return uid%USER_BUCKET_NUM + 1;
}

bool BucketDist_Alg::check_alg() const
{	
	if(!_sid_bucks_map.empty()){
		sid_bucks_t::const_iterator it = _sid_bucks_map.begin();
		uint32_t max_bucks = (*it).second.size();
		uint32_t min_bucks = (*it).second.size();
		for (; it != _sid_bucks_map.end(); ++it){
			if ((*it).second.size() > max_bucks){
				max_bucks = ((*it).second.size());
			}
			else
				min_bucks = ((*it).second.size());
		}

		if((max_bucks - min_bucks) <= 1)
			return true;
		else
			return false;
	}else
		return true;
}
