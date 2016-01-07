#ifndef PCANODED_H_
#define PCANODED_H_

#include <map>
#include "server_common/protocol/daemon.h"
//#include "common/core/ilink.h"
//#include "server_common/protocol/psession.h"
#include "common/core/base_svid.h"
//#include "server_common/protocol/plogin.h"

using namespace core;
//using namespace server::session;
using namespace protocol;
//using namespace protocol::session;
//using namespace protocol::uinfo;
//using namespace server::lbs;
using namespace sox;

namespace server {	namespace canode {
	struct user_info : public sox::Marshallable {
		uint32_t id;
		std::vector<std::string> data;

		virtual void marshal(sox::Pack & p) const {
			p << id;
			sox::marshal_container(p, data);
		}

		virtual void unmarshal(const sox::Unpack & p) {
			p >> id;
			sox::unmarshal_container(p, std::back_inserter(data));
		}
	};

	struct PGetCaUserInfo : public sox::Marshallable {

		enum {
			uri = 14 << 8 | CANODE_SVID
		};

		uint64_t transId;
		std::vector<uint32_t>  ids;
		std::vector<std::string>  params;
		uint8_t conLevel;

		virtual void marshal(sox::Pack & p) const {
			p << transId;
			sox::marshal_container(p, ids);
			sox::marshal_container(p, params);
			p << conLevel;
		}

		virtual void unmarshal(const sox::Unpack & p) {
			p >> transId;
			sox::unmarshal_container(p, std::back_inserter(ids));
			sox::unmarshal_container(p, std::back_inserter(params));
			p >> conLevel;
		}
	};

	struct PGetCaUserInfoRes : public sox::Marshallable {

		enum {
			uri = 15 << 8 | CANODE_SVID
		};

		uint64_t transId;
		std::vector<std::string> params;
		std::vector<user_info> data;
		std::vector<uint32_t> failsIds;

		virtual void marshal(sox::Pack & p) const {
			p << transId;
			sox::marshal_container(p, params);
			sox::marshal_container(p, data);
			sox::marshal_container(p, failsIds);
		}

		virtual void unmarshal(const sox::Unpack & p) {
			p >> transId;
			sox::unmarshal_container(p, std::back_inserter(params));
			sox::unmarshal_container(p, std::back_inserter(data));
			sox::unmarshal_container(p, std::back_inserter(failsIds));
		}
	};

	struct PSetCaUserInfo : public sox::Marshallable {

		enum {
			uri = 16 << 8 | CANODE_SVID
		};

		uint32_t transId;
		std::vector<user_info>  data;
		std::vector<std::string>  params;
		uint8_t conLevel;

		virtual void marshal(sox::Pack & p) const {
			p << transId;
			sox::marshal_container(p, data);
			sox::marshal_container(p, params);
			p << conLevel;
		}

		virtual void unmarshal(const sox::Unpack & p) {
			p >> transId;
			sox::unmarshal_container(p, std::back_inserter(data));
			sox::unmarshal_container(p, std::back_inserter(params));
			p >> conLevel;
		}
	};

	struct PSetCaUserInfoRes : public sox::Marshallable {

		enum {
			uri = 17 << 8 | CANODE_SVID
		};

		uint32_t transId;
		std::vector<uint32_t> successIds;
		std::vector<uint32_t> failsIds;

		virtual void marshal(sox::Pack & p) const {
			p << transId;
			sox::marshal_container(p, successIds);
			sox::marshal_container(p, failsIds);
		}

		virtual void unmarshal(const sox::Unpack & p) {
			p >> transId;
			sox::unmarshal_container(p, std::back_inserter(successIds));
			sox::unmarshal_container(p, std::back_inserter(failsIds));
		}
	};

	//add by ouyangqing for test use. date : 2010-08-05
	struct PRemoveCaUserInfo : public sox::Marshallable {

		enum {
			uri = 20 << 8 | CANODE_SVID
		};

		uint64_t transId;
		std::vector<uint32_t>  ids;
		std::vector<std::string>  params;
		uint8_t conLevel;

		virtual void marshal(sox::Pack & p) const {
			p << transId;
			sox::marshal_container(p, ids);
			sox::marshal_container(p, params);
			p << conLevel;
		}

		virtual void unmarshal(const sox::Unpack & p) {
			p >> transId;
			sox::unmarshal_container(p, std::back_inserter(ids));
			sox::unmarshal_container(p, std::back_inserter(params));
			p >> conLevel;
		}
	};
	//add by ouyangqing for test use. date : 2010-08-05
	struct PRemoveCaUserInfoRes : public sox::Marshallable {

		enum {
			uri = 22 << 8 | CANODE_SVID
		};

		uint64_t transId;
		std::vector<std::string> params;
		std::vector<uint32_t> successIds;
		std::vector<uint32_t> failsIds;

		virtual void marshal(sox::Pack & p) const {
			p << transId;
			sox::marshal_container(p, params);
			sox::marshal_container(p, successIds);
			sox::marshal_container(p, failsIds);
		}

		virtual void unmarshal(const sox::Unpack & p) {
			p >> transId;
			sox::unmarshal_container(p, std::back_inserter(params));
			sox::unmarshal_container(p, std::back_inserter(successIds));
			sox::unmarshal_container(p, std::back_inserter(failsIds));
		}
	};

	typedef std::pair<uint32_t, uint32_t> pairSidKey;
	struct sub_channel_info : public sox::Marshallable {
		uint32_t top_sid;
		uint32_t sub_sid;
		std::vector<std::string> data;

		virtual void marshal(Pack & pk) const {
			pk << top_sid << sub_sid;
			marshal_container(pk, data);
		}

		virtual void unmarshal(const Unpack & up) {
			up >> top_sid >> sub_sid;
			unmarshal_container(up, std::back_inserter(data));
		}
	};

	struct PGetCaSubChannelInfo : public sox::Marshallable {

		enum { uri = 30 << 8 | CANODE_SVID };

		uint64_t transId;
		std::vector<pairSidKey > sid_key_list;
		std::vector<std::string> params;
		uint8_t conLevel;

		virtual void marshal(sox::Pack & p) const {
			p << transId;
			sox::marshal_container(p, sid_key_list);
			sox::marshal_container(p, params);
			p.push_uint8(conLevel);
		}

		virtual void unmarshal(const sox::Unpack & p) {
			p >> transId;
			sox::unmarshal_container(p, std::back_inserter(sid_key_list));
			sox::unmarshal_container(p, std::back_inserter(params));
			conLevel = p.pop_uint8();
		}
	};

	struct PGetCaSubChannelInfoRes : public sox::Marshallable {

		enum {
			uri = 31 << 8 | CANODE_SVID
		};

		uint64_t transId;
		std::vector<std::string> params;
		std::vector<sub_channel_info> data;
		std::vector<pairSidKey > fail_sids;

		virtual void marshal(sox::Pack & p) const {
			p << transId;
			sox::marshal_container(p, params);
			sox::marshal_container(p, data);
			sox::marshal_container(p, fail_sids);
		}

		virtual void unmarshal(const sox::Unpack & p) {
			p >> transId;
			sox::unmarshal_container(p, std::back_inserter(params));
			sox::unmarshal_container(p, std::back_inserter(data));
			sox::unmarshal_container(p, std::back_inserter(fail_sids));
		}
	};

	struct PSetCaSubChannelInfo : public sox::Marshallable {

		enum {
			uri = 32 << 8 | CANODE_SVID
		};

		uint64_t transId;
		std::vector<sub_channel_info> data;
		std::vector<std::string>  params;
		uint8_t conLevel;

		virtual void marshal(sox::Pack & p) const {
			p << transId;
			sox::marshal_container(p, data);
			sox::marshal_container(p, params);
			p.push_uint8(conLevel);
		}

		virtual void unmarshal(const sox::Unpack & p) {
			p >> transId;
			sox::unmarshal_container(p, std::back_inserter(data));
			sox::unmarshal_container(p, std::back_inserter(params));
			conLevel = p.pop_uint8();
		}
	};

	struct PSetCaSubChannelInfoRes : public sox::Marshallable {

		enum {
			uri = 33 << 8 | CANODE_SVID
		};

		uint64_t transId;
		std::vector<pairSidKey> success_sids;
		std::vector<pairSidKey> fail_sids;

		virtual void marshal(sox::Pack & p) const {
			p << transId;
			sox::marshal_container(p, success_sids);
			sox::marshal_container(p, fail_sids);
		}

		virtual void unmarshal(const sox::Unpack & p) {
			p >> transId;
			sox::unmarshal_container(p, std::back_inserter(success_sids));
			sox::unmarshal_container(p, std::back_inserter(fail_sids));
		}
	};

	struct PRemoveCaSubChannelInfo : public sox::Marshallable {

		enum {
			uri = 34 << 8 | CANODE_SVID
		};

		uint64_t transId;
		std::vector< pairSidKey > key_list; 
		std::vector<std::string> params;
		uint8_t conLevel;

		virtual void marshal(sox::Pack & p) const {
			p << transId;
			sox::marshal_container(p, key_list);
			sox::marshal_container(p, params);
			p.push_uint8(conLevel);
		}

		virtual void unmarshal(const sox::Unpack & p) {
			p >> transId;
			sox::unmarshal_container(p, std::back_inserter(key_list));
			sox::unmarshal_container(p, std::back_inserter(params));
			conLevel = p.pop_uint8();
		}
	};

	struct PRemoveCaSubChannelInfoRes : public sox::Marshallable {

		enum {
			uri = 35 << 8 | CANODE_SVID
		};

		uint64_t transId;
		std::vector<pairSidKey> success_sids;
		std::vector<pairSidKey> fail_sids;

		virtual void marshal(sox::Pack & p) const {
			p << transId;
			sox::marshal_container(p, success_sids);
			sox::marshal_container(p, fail_sids);
		}

		virtual void unmarshal(const sox::Unpack & p) {
			p >> transId;
			sox::unmarshal_container(p, std::back_inserter(success_sids));
			sox::unmarshal_container(p, std::back_inserter(fail_sids));
		}
	};


	struct PGetBulltin : public sox::Marshallable {

		enum { uri = 40 << 8 | CANODE_SVID };

		std::string strReserved;

		uint32_t tid;	// 顶级id
		uint32_t sid;	// 子频道id

		PGetBulltin() : tid(0), sid(0) {}

		virtual void marshal(sox::Pack & p) const {
			p << strReserved << tid << sid;
		}

		virtual void unmarshal(const sox::Unpack & p) {
			p >> strReserved >> tid >> sid;
		}
	};


	struct PGetBulltinRes : public sox::Marshallable {

		enum { uri = 41 << 8 | CANODE_SVID };

		std::string strReserved;

		uint32_t tid;
		uint32_t sid;

		bool bExist;	// true exist, false not exist

		std::string strBlt;
		uint32_t u32BltLen;			// 压缩前的长度

		uint32_t u32BltTimeStamp;	// unix time stamp

		PGetBulltinRes() : tid(0), sid(0), bExist(true), u32BltLen(0), u32BltTimeStamp(0) {}

		virtual void marshal(sox::Pack & p) const {
			p << strReserved << tid << sid << bExist << strBlt << u32BltLen << u32BltTimeStamp;
		}

		virtual void unmarshal(const sox::Unpack & p) {
			p >> strReserved >> tid >> sid >> bExist >> strBlt >> u32BltLen >> u32BltTimeStamp;
		}
	};


	struct PSetBulltin : public sox::Marshallable {

		enum { uri = 42 << 8 | CANODE_SVID };

		std::string strReserved;

		uint32_t tid;
		uint32_t sid;

		std::string strBlt;
		uint32_t u32BltLen;

		uint32_t u32BltTimeStamp;	// unix time stamp

		PSetBulltin() : tid(0), sid(0), u32BltLen(0), u32BltTimeStamp(0) {}

		virtual void marshal(sox::Pack & p) const {
			p << strReserved << tid << sid << strBlt << u32BltLen << u32BltTimeStamp;
		}

		virtual void unmarshal(const sox::Unpack & p) {
			p >> strReserved >> tid >> sid >> strBlt >> u32BltLen >> u32BltTimeStamp;
		}
	};

	struct PSetBulltinRes : public sox::Marshallable {

		enum { uri = 43 << 8 | CANODE_SVID };

		std::string strReserved;

		uint32_t tid;
		uint32_t sid;

		bool bSuccess;	// true success, false fail


		PSetBulltinRes() : tid(0), sid(0), bSuccess(true) {}

		virtual void marshal(sox::Pack & p) const {
			p << strReserved << tid << sid << bSuccess;
		}

		virtual void unmarshal(const sox::Unpack & p) {
			p >> strReserved >> tid >> sid >> bSuccess;
		}
	};

	struct PDelBulltin : public sox::Marshallable 
	{
		enum { uri = 44 << 8 | CANODE_SVID };
		std::string strReserved;
		uint32_t tid;
		std::vector<uint32_t> m_vecSubSid;//要删除的子频道列表

		virtual void marshal(sox::Pack & p) const 
		{
			p << strReserved << tid ;
			marshal_container(p, m_vecSubSid);
		}

		virtual void unmarshal(const sox::Unpack & p) 
		{
			p >> strReserved >> tid ;
			sox::unmarshal_container(p, std::inserter(m_vecSubSid, m_vecSubSid.begin()));
		}
	};

}}


#endif /*PCANODED_H_*/
