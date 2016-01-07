#include "MultiConnManagerImp.h"
#include "common/core/ibase.h"
#include "core/sox/logger.h"

using namespace core;

MultiConnManagerImp::MultiConnManagerImp():cid(0){
	//sox::env::selector()->set_interrupt_handler(this);
}

MultiConnManagerImp::~MultiConnManagerImp(){
	for (connect_t::iterator it = connects.begin(); it != connects.end(); ++it) {
		delete (*it).second;
	}
}

void MultiConnManagerImp::eraseConnect(IConn *conn) {
	connect_t::size_type sz =connects.erase(conn->getConnId());
	if(sz != 0)
		delete conn;
}

void MultiConnManagerImp::eraseConnectById(uint32_t id) {
	connect_t::iterator it = connects.find(id);
	if (it != connects.end()) {
		eraseConnect(it->second);
	}
}

IConn *MultiConnManagerImp::getConnectById(uint32_t id) {
	connect_t::iterator it = connects.find(id);
	return it == connects.end() ? NULL : it->second;
}


bool MultiConnManagerImp::dispatchByIds(const std::set<uint32_t> &ids, Sender &resp,  uint32_t exp){
/*
#ifndef WIN32
  struct timeval enter_time;  
  struct timeval middle_time;
  struct timeval leave_time;
  gettimeofday(&enter_time, NULL); 
#endif
*/
	resp.endPack();
/*
#ifndef WIN32
  gettimeofday(&middle_time, NULL);
  int diff_tick = (middle_time.tv_sec - enter_time.tv_sec)*1000000 + (middle_time.tv_usec - enter_time.tv_usec);
  m_send_tick1 += diff_tick;
#endif
*/

	if(exp != NONEEXP_CID){
		for (std::set<uint32_t>::const_iterator it = ids.begin(); it!= ids.end(); ++it) {
			if(exp != *it){
				IConn *conn = *it < connects.size() ? connects[*it] : NULL;
				if(conn){
					try{
						conn->sendBin(resp.header(), resp.headerSize() + resp.bodySize(), resp.getUri());
					}catch(std::exception &se) {
						conn->setEnable(false);
						toDelete.insert(*it);
						log(Error, "Sox SocketError in multi dispatch ids, err: %s", se.what());
						//			mexp.exps.push_back((*it_c).second);
					}
				}else{
					//xxx todo
					//log(Info, "broadcast connect absence: ");
				}
			}
		}
	}else{
		for (std::set<uint32_t>::const_iterator it = ids.begin(); it!= ids.end(); ++it) {
			IConn *conn = *it < connects.size() ? connects[*it] : NULL;
			if(conn){
				try{
						conn->sendBin(resp.header(), resp.headerSize() + resp.bodySize(), resp.getUri());
				}catch(std::exception &se) {
					conn->setEnable(false);
					toDelete.insert(*it);
					log(Error, "Sox SocketError in multi dispatch ids, err: %s", se.what());
					//			mexp.exps.push_back((*it_c).second);
				}
			}else{
				//xxx todo
				//log(Info, "broadcast connect absence: ");
			}
		}
	}
//#ifndef WIN32
//  gettimeofday(&leave_time, NULL);
//  int diff_tick2 = (leave_time.tv_sec - enter_time.tv_sec)*1000000 + (leave_time.tv_usec - enter_time.tv_usec);
//  m_send_tick2 += diff_tick2;
//#endif
	return true;
}

int inc = 1;
bool MultiConnManagerImp::dispatchById(uint32_t cid, Sender &request) {
	connect_t::iterator it = connects.find(cid);
	if (it != connects.end()) {
		//for(int i = 0; i < 20; ++i)
		try{
			(*it).second->send(request);
			return true;
		}catch(std::exception &se){
			log(Error, "dispatch by cid:%u err: %s uri:%02x, size:%u", cid, se.what(), request.getUri(), request.bodySize());
			(*it).second->setEnable(false);
			toDelete.insert(cid);
			//onError(12345, "dispatchById", it->second);
		}
	} else {
		log(Info, "couldnot dispatch response: %u", cid);
	}
	return false;
}

void MultiConnManagerImp::onConnCreate(IConn *conn){
	conn->setConnId(++cid);
	conn->setSerialId(cid);
	connects[conn->getConnId()] = conn;
	conn->setLinkEvent(this);
}
