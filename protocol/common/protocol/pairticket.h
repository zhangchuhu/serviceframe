#ifndef PINVITE_CODE_PROTOCOL_H
#define PINVITE_CODE_PROTOCOL_H
#include "common/core/base_svid.h"
#include "common/iproperty.h"
#include "common/protocol/const.h"
namespace protocol{
        namespace airticket{
		/*
		 * asid and sid thansformat
		 */
		struct PAirTicketGetSid : public sox::Marshallable
		{
			enum { uri = (1 << 8) | AIRTICKETAPP_SVID};

			uint32_t id; // asid or sid

			void marshal(sox::Pack &pk) const 
			{
				pk << id;
			}
			void unmarshal(const sox::Unpack &pk) 
			{
				pk >> id;
			}
		};

		struct PAirTicketGetSidRes: public sox::Marshallable
		{
			enum{uri = 2 << 8| AIRTICKETAPP_SVID};

			uint16_t resCode;  //0:成功  1:没有找到匹配输入id的长号  2:输入id错误

			uint32_t asid;
			uint32_t sid;

			void marshal(sox::Pack &pk) const 
			{
				pk << resCode << sid << asid;
			}
			void unmarshal(const sox::Unpack &pk) 
			{
				pk >> resCode >> sid >> asid;
			}
		};

		struct PAirTicketBatchGetSid: public sox::Marshallable
		{
			enum { uri = (3 << 8) | AIRTICKETAPP_SVID};

			std::vector<uint32_t> asids; // Batch asids, not support sid

			void marshal(sox::Pack &pk) const 
			{
				sox::marshal_container(pk, asids);
			}
			void unmarshal(const sox::Unpack &pk) 
			{
				sox::unmarshal_container(pk, std::inserter(asids, asids.begin()));
			}
		};

		struct PAirTicketBatchGetSidRes: public sox::Marshallable
		{
			enum{uri = 4 << 8| AIRTICKETAPP_SVID};

			uint16_t resCode;  //0:成功  1:没有找到匹配输入asids的长号  2:输入asids错误
			std::map<uint32_t,uint32_t> sids;

			void marshal(sox::Pack &pk) const 
			{
				pk << resCode ;
				sox::marshal_container(pk, sids);
			}
			void unmarshal(const sox::Unpack &pk) 
			{
				pk >> resCode ;
				sox::unmarshal_container(pk, std::inserter(sids, sids.begin()));
			}
		};
	}
}

#endif /*PINVITECODE_*/
