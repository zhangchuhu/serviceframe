#pragma once

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"
#include <vector>
#include <string>
#include <map>
#include <pthread.h>
#include <semaphore.h>
namespace server{
	namespace transinferface{

		struct NotifyMsg:public sox::Marshallable
		{
			enum {uri = 7 << 8 | 23};
			unsigned int MsgType;
			std::string MsgContent;
			virtual void marshal(sox::Pack &pk) const
			{
				pk << MsgType << MsgContent;
			}
			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> MsgType >> MsgContent;
			}
		};
		
		struct PImidToUidMsgReq: public sox::Marshallable{
			enum {uri = (5 << 8) | protocol::THRIFTPROXY_SVID};
			std::vector<uint32_t> vecimids;
			uint32_t seq;
			PImidToUidMsgReq()
			{
				vecimids.clear();
			}
			virtual void marshal(sox::Pack &p) const {
				marshal_container(p, vecimids);
				p << seq;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				unmarshal_container(p, std::back_inserter(vecimids));
				p >> seq;
			}
		};

		struct PImidToUidMsgRes: public sox::Marshallable{
			enum {uri = (6 << 8) | protocol::THRIFTPROXY_SVID};
			std::map<uint32_t,uint32_t> mapuids;
			uint32_t seq;
			PImidToUidMsgRes()
			{
				mapuids.clear();
			}
			virtual void marshal(sox::Pack &p) const {
				sox::marshal_container(p, mapuids);
				p << seq;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				sox::unmarshal_container(p, std::inserter(mapuids, mapuids.begin()));
				p >> seq;
			}
		};

		struct PUidToImidMsgReq: public sox::Marshallable{
			enum {uri = (7 << 8) | protocol::THRIFTPROXY_SVID};
			std::vector<uint32_t> vecuids;
			uint32_t seq;
			PUidToImidMsgReq()
			{
				vecuids.clear();
			}
			virtual void marshal(sox::Pack &p) const {
				marshal_container(p, vecuids);
				p << seq;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				unmarshal_container(p, std::back_inserter(vecuids));
				p >> seq;
			}
		};

		struct PUidToImidMsgRes: public sox::Marshallable{
			enum {uri = (8 << 8) | protocol::THRIFTPROXY_SVID};
			std::map<uint32_t,uint32_t> mapimids;
			uint32_t seq;
			PUidToImidMsgRes()
			{
				mapimids.clear();
			}
			virtual void marshal(sox::Pack &p) const {
				sox::marshal_container(p, mapimids);
				p << seq;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				sox::unmarshal_container(p, std::inserter(mapimids, mapimids.begin()));
				p >> seq;
			}
		};

		struct PUidToStatusMsgReq: public sox::Marshallable{
			enum {uri = (9 << 8) | protocol::THRIFTPROXY_SVID};
			std::vector<uint32_t> vecuids;
			uint32_t seq;
			PUidToStatusMsgReq()
			{
				vecuids.clear();
			}
			virtual void marshal(sox::Pack &p) const {
				marshal_container(p, vecuids);
				p << seq;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				unmarshal_container(p, std::back_inserter(vecuids));
				p >> seq;
			}
		};

		struct PUidToStatusMsgRes: public sox::Marshallable{
			enum {uri = (10 << 8) | protocol::THRIFTPROXY_SVID};
			std::map<uint32_t,uint32_t> mapstatus;
			uint32_t seq;
			PUidToStatusMsgRes()
			{
				mapstatus.clear();
			}
			virtual void marshal(sox::Pack &p) const {
				sox::marshal_container(p, mapstatus);
				p << seq;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				sox::unmarshal_container(p, std::inserter(mapstatus, mapstatus.begin()));
				p >> seq;
			}
		};

		struct PUnameToUidMsgReq: public sox::Marshallable{
			enum {uri = (11 << 8) | protocol::THRIFTPROXY_SVID};
			std::vector<std::string> vecunames;
			uint32_t seq;
			PUnameToUidMsgReq()
			{
				vecunames.clear();
			}
			virtual void marshal(sox::Pack &p) const {
				marshal_container(p, vecunames);
				p << seq;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				unmarshal_container(p, std::back_inserter(vecunames));
				p >> seq;
			}
		};

		struct PUnameToUidMsgRes: public sox::Marshallable{
			enum {uri = (12 << 8) | protocol::THRIFTPROXY_SVID};
			std::map<std::string,uint32_t> mapuids;
			uint32_t seq;
			PUnameToUidMsgRes()
			{
				mapuids.clear();
			}
			virtual void marshal(sox::Pack &p) const {
				marshal_container(p, mapuids);
				p << seq;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				unmarshal_container(p, std::inserter(mapuids, mapuids.begin()));
				p >> seq;
			}
		};

		struct PYynoToUidThriftReq: public sox::Marshallable{
			enum {uri = (21 << 8) | protocol::THRIFTPROXY_SVID};
			uint32_t context_id;
			std::string auth_info;
			PYynoToUidThriftReq()
			{
				context_id = 0;
				auth_info = "";
			}
			PYynoToUidThriftReq(uint32_t id, std::string info)
			{
				context_id = id;
				auth_info = info;
			}
			virtual void marshal(sox::Pack &p) const {
				p << context_id << auth_info;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> context_id >> auth_info;
			}
		};

		struct PUidToYynoThriftReq: public sox::Marshallable{
			enum {uri = (22 << 8) | protocol::THRIFTPROXY_SVID};
			uint32_t context_id;
			std::string auth_info;
			PUidToYynoThriftReq()
			{
				context_id = 0;
				auth_info = "";
			}
			PUidToYynoThriftReq(uint32_t id, std::string info)
			{
				context_id = id;
				auth_info = info;
			}
			virtual void marshal(sox::Pack &p) const {
				p << context_id << auth_info;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> context_id >> auth_info;
			}
		};

		struct PUidToPpsActsThriftReq: public sox::Marshallable{
			enum {uri = (23 << 8) | protocol::THRIFTPROXY_SVID};
			uint32_t context_id;
			std::string auth_info;
			PUidToPpsActsThriftReq()
			{
				context_id = 0;
				auth_info = "";
			}
			PUidToPpsActsThriftReq(uint32_t id, std::string info)
			{
				context_id = id;
				auth_info = info;
			}
			virtual void marshal(sox::Pack &p) const {
				p << context_id << auth_info;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> context_id >> auth_info;
			}
		};

		struct PImidToImidInfoThriftReq: public sox::Marshallable{
			enum {uri = (24 << 8) | protocol::THRIFTPROXY_SVID};
			uint32_t context_id;
			std::string auth_info;
			PImidToImidInfoThriftReq()
			{
				context_id = 0;
				auth_info = "";
			}
			PImidToImidInfoThriftReq(uint32_t id, std::string info)
			{
				context_id = id;
				auth_info = info;
			}
			virtual void marshal(sox::Pack &p) const {
				p << context_id << auth_info;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> context_id >> auth_info;
			}
		};

		struct PUidToImidInfoThriftReq: public sox::Marshallable{
			enum {uri = (25 << 8) | protocol::THRIFTPROXY_SVID};
			uint32_t context_id;
			std::string auth_info;
			PUidToImidInfoThriftReq()
			{
				context_id = 0;
				auth_info = "";
			}
			PUidToImidInfoThriftReq(uint32_t id, std::string info)
			{
				context_id = id;
				auth_info = info;
			}
			virtual void marshal(sox::Pack &p) const {
				p << context_id << auth_info;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> context_id >> auth_info;
			}
		};

		struct PUsernfoAllotIpReq: public sox::Marshallable{
			enum {uri = (31 << 8) | protocol::THRIFTPROXY_SVID};
			std::set<uint32_t> m_allowIps;
			std::set<std::string> m_freeAccess;
			std::multimap<uint32_t,std::string> m_allowAccess;
			uint32_t seq;
			PUsernfoAllotIpReq()
			{
				m_allowIps.clear();
				m_freeAccess.clear();
				m_allowAccess.clear();
			}
			virtual void marshal(sox::Pack &p) const {
				sox::marshal_container(p, m_allowIps);
				sox::marshal_container(p, m_freeAccess);
				sox::marshal_container(p, m_allowAccess);
				p << seq;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				sox::unmarshal_container(p, std::inserter(m_allowIps, m_allowIps.begin()));
				sox::unmarshal_container(p, std::inserter(m_freeAccess, m_freeAccess.begin()));
				sox::unmarshal_container(p, std::inserter(m_allowAccess, m_allowAccess.begin()));
				p >> seq;
			}
		};

		struct PUsernfoAllotIpRes: public sox::Marshallable{
			enum {uri = (32 << 8) | protocol::THRIFTPROXY_SVID};
			std::string strResponse;
			uint32_t seq;
			PUsernfoAllotIpRes()
			{
			}
			virtual void marshal(sox::Pack &p) const {
				p << strResponse << seq;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> strResponse >> seq;
			}
		};

		struct PCheckMobileBindByUidReq: public sox::Marshallable{
			enum {uri = (41 << 8) | protocol::THRIFTPROXY_SVID};
			uint32_t uSeq;
			std::string sContext;
			std::string sYyuid;
			virtual void marshal(sox::Pack &p) const {
				p << uSeq << sContext << sYyuid;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> uSeq >> sContext >> sYyuid;
			}
		};

		struct PCheckMobileBindByUidRes: public sox::Marshallable{
			enum {uri = (42 << 8) | protocol::THRIFTPROXY_SVID};
			uint32_t uSeq;
			std::string sContext;
			std::string sRescode;
			virtual void marshal(sox::Pack &p) const {
				p << uSeq << sContext << sRescode;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> uSeq >> sContext >> sRescode;
			}
		};

		struct PGetMibaoInfoBindByUidReq: public sox::Marshallable{
			enum {uri = (43 << 8) | protocol::THRIFTPROXY_SVID};
			std::string sContext;
			uint32_t uSeq;
			uint32_t uYyuid;
			virtual void marshal(sox::Pack &p) const {
				p << sContext << uSeq << uYyuid;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> sContext >> uSeq >> uYyuid;
			}
		};

		struct PGetMibaoInfoBindByUidRes: public sox::Marshallable{
			enum {uri = (44 << 8) | protocol::THRIFTPROXY_SVID};
			std::string sContext;
			uint32_t uSeq;
			uint32_t uRescode;
			virtual void marshal(sox::Pack &p) const {
				p << sContext << uSeq << uRescode;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> sContext >> uSeq >> uRescode;
			}
		};

		struct PGetMibaoQuestionByUidReq: public sox::Marshallable{
			enum {uri = (45 << 8) | protocol::THRIFTPROXY_SVID};
			std::string sContext;
			uint32_t uSeq;
			uint32_t uYyuid;
			virtual void marshal(sox::Pack &p) const {
				p << sContext << uSeq << uYyuid;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> sContext >> uSeq >> uYyuid;
			}
		};

		struct PGetMibaoQuestionByUidRes: public sox::Marshallable{
			enum {uri = (46 << 8) | protocol::THRIFTPROXY_SVID};
			std::string sContext;
			uint32_t uSeq;
			uint32_t uRescode;
			std::vector<std::string> vecQues;
			virtual void marshal(sox::Pack &p) const {
				p << sContext << uSeq << uRescode;
				marshal_container(p, vecQues);
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> sContext >> uSeq >> uRescode;
				unmarshal_container(p, std::back_inserter(vecQues));
			}
		};

		struct PCheckMibaoQAByUidReq: public sox::Marshallable{
			enum {uri = (47 << 8) | protocol::THRIFTPROXY_SVID};
			std::string sContext;
			uint32_t uSeq;
			uint32_t uYyuid;
			std::string sQuestione1;
			std::string sAnswer1;
			std::string sQuestione2;
			std::string sAnswer2;
			std::string sQuestione3;
			std::string sAnswer3;
			PCheckMibaoQAByUidReq(std::string context = "", uint32_t seq = 0) : sContext(context), uSeq(seq) {}
			virtual void marshal(sox::Pack &p) const {
				p << sContext << uSeq << uYyuid << sQuestione1 << sAnswer1 << sQuestione2 << sAnswer2 << sQuestione3 << sAnswer3;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> sContext >> uSeq >> uYyuid >> sQuestione1 >> sAnswer1 >> sQuestione2 >> sAnswer2 >> sQuestione3 >> sAnswer3;
			}
		};

		struct PCheckMibaoQAByUidRes: public sox::Marshallable{
			enum {uri = (48 << 8) | protocol::THRIFTPROXY_SVID};
			std::string sContext;
			uint32_t uSeq;
			uint32_t uRescode;
			uint32_t cntQA;
			uint32_t flagQA1;
			uint32_t flagQA2;
			uint32_t flagQA3;
			PCheckMibaoQAByUidRes(std::string context = "", uint32_t seq = 0) : sContext(context), uSeq(seq) {}
			virtual void marshal(sox::Pack &p) const {
				p << sContext << uSeq << uRescode << cntQA << flagQA1 << flagQA2 << flagQA3;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> sContext >> uSeq >> uRescode >> cntQA >> flagQA1 >> flagQA2 >> flagQA3;
			};
		};

		struct PCheckUidPhoneBindReq: public sox::Marshallable{
			enum {uri = (49 << 8) | protocol::THRIFTPROXY_SVID};
			std::string sContext;
			uint32_t uSeq;
			uint32_t uYyuid;
			std::string sPhone;
			virtual void marshal(sox::Pack &p) const {
				p << sContext << uSeq << uYyuid << sPhone;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> sContext >> uSeq >> uYyuid >> sPhone;
			}
		};

		struct PCheckUidPhoneBindRes: public sox::Marshallable{
			enum {uri = (50 << 8) | protocol::THRIFTPROXY_SVID};
			std::string sContext;
			uint32_t uSeq;
			uint32_t uRescode;
			virtual void marshal(sox::Pack &p) const {
				p << sContext << uSeq << uRescode;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> sContext >> uSeq >> uRescode;
			}
		};

		struct PCheckMibaoQAByUidExReq: public sox::Marshallable{
			enum {uri = (51 << 8) | protocol::THRIFTPROXY_SVID};
			std::string sContext;
			uint32_t uSeq;
			uint32_t uYyuid;
			std::map<std::string, std::string> mibaoQA;
			PCheckMibaoQAByUidExReq(std::string context = "", uint32_t seq = 0) : sContext(context), uSeq(seq) {}
			virtual void marshal(sox::Pack &p) const {
				p << sContext << uSeq << uYyuid;
				sox::marshal_container(p, mibaoQA);
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> sContext >> uSeq >> uYyuid;
				sox::unmarshal_container(p, std::inserter(mibaoQA, mibaoQA.begin()));
			}
		};

		struct PCheckMibaoQAByUidExRes: public sox::Marshallable{
			enum {uri = (52 << 8) | protocol::THRIFTPROXY_SVID};
			std::string sContext;
			uint32_t uSeq;
			uint32_t uRescode;
			uint32_t uResult;
			PCheckMibaoQAByUidExRes(std::string context = "", uint32_t seq = 0) : sContext(context), uSeq(seq) {}
			virtual void marshal(sox::Pack &p) const {
				p << sContext << uSeq << uRescode << uResult;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> sContext >> uSeq >> uRescode >> uResult;
			};
		};
	}
}
