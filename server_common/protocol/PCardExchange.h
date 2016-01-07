#ifndef PCARD_EXCHANGE_H_
#define PCARD_EXCHANGE_H_
#include "common/core/base_svid.h"
#include "common/iproperty.h"
#include "common/res_code.h"
#include <set>

namespace protocol {
namespace card_exchange {

	enum EXTENSION_POS
	{
		EXTENSION_PROPS_NONE	=	0,
		EXTENSION_POS_MOBILE	=	1,
		EXTENSION_POS_EMAIL		=	1 << 1,
		EXTENSION_POS_QQ		=	1 << 2,
		EXTENSION_POS_MSN		=	1 << 3,
		EXTENSION_POS_ADDRESS	=	1 << 4
	};

	enum NAMECARD_PROPS{		
		NAMECARD_PROPS_NAME			= 0x0001,
		NAMECARD_PROPS_PIC_URL		= 0x0002,
		NAMECARD_PROPS_SEX			= 0x0003,
		NAMECARD_PROPS_BIRTHDAY		= 0x0004,
		NAMECARD_PROPS_REGION		= 0x0005,
		NAMECARD_PROPS_SCHOOL		= 0x0006,
		NAMECARD_PROPS_COMPANY		= 0x0007,
		NAMECARD_PROPS_BRIEF_INFO	= 0x0008,
		NAMECARD_PROPS_OTHER		= 0x0009,
		NAMECARD_PROPS_MOBILE		= 0x000A,
		NAMECARD_PROPS_EMAIL		= 0x000B,
		NAMECARD_PROPS_QQ			= 0x000C,
		NAMECARD_PROPS_MSN			= 0x000D,
		NAMECARD_PROPS_ADDRESS		= 0x000E,
		NAMECARD_PROPS_FLAG			= 0x000F //to be del
	};
	typedef std::map<uint32_t, sox::Properties> uid2props_map_t;

	//YYProtocol_map<uid, YYProtocol_property(-> std::map<uint16_t//property_pos, string//property_value>)>
	struct UidMapProperties : public sox::Marshallable {
		uid2props_map_t mapp;

		virtual void marshal(sox::Pack & p) const {
			sox::marshal_container(p, mapp);
		}

		virtual void unmarshal(const sox::Unpack &p) {
			sox::unmarshal_container(p, sox::properties::replace_iterator(mapp));
		}
	};

	struct PSetCardInfo: public sox::Marshallable{
		enum {uri = (1 << 8 | protocol::CARD_EXCHANGE_SVID)};
		sox::Properties prop;

		virtual void marshal(sox::Pack & p) const {
			prop.marshal(p);
		}

		virtual void unmarshal(const sox::Unpack & p) {
			prop.unmarshal(p);
		}

	};

	struct PSetCardInfoRes: public sox::Marshallable{
		enum {uri = (2 << 8 | protocol::CARD_EXCHANGE_SVID)};
		PSetCardInfoRes(){};
		PSetCardInfoRes(uint32_t _src_uid){
			src_uid = _src_uid;
			resCode = RES_SUCCESS;
		};
		uint32_t src_uid;
		uint16_t resCode;//RES_SUCCESS or RES_EDBERROR

		virtual void marshal(sox::Pack & p) const {
			p << src_uid << resCode;
		}

		virtual void unmarshal(const sox::Unpack & p) {
			p >> src_uid >> resCode;
		}
	};
	
	struct PGetCardInfo: public sox::Marshallable{
		enum {uri = (3 << 8 | protocol::CARD_EXCHANGE_SVID)};
		uint32_t uid;

		virtual void marshal(sox::Pack & p) const {
			p << uid;
		}

		virtual void unmarshal(const sox::Unpack & p) {
			p >> uid;
		}
	};

	struct PGetCardInfoRes: public sox::Marshallable{
		enum {uri = (4 << 8 | protocol::CARD_EXCHANGE_SVID)};
		PGetCardInfoRes(){};
		PGetCardInfoRes(uint32_t target_uid){
			uid = target_uid;
			prop.props.clear();
			resCode = RES_SUCCESS;
			target_exchange_flag = EXTENSION_PROPS_NONE;
		};
		sox::Properties prop;
		uint32_t uid;//target_uid
		uint16_t resCode;
		uint32_t target_exchange_flag;
		
		virtual void marshal(sox::Pack & p) const {
			prop.marshal(p);
			p << uid << resCode << target_exchange_flag;
		}

		virtual void unmarshal(const sox::Unpack & p) {
			prop.unmarshal(p);
			p >> uid >> resCode >> target_exchange_flag;
		}
	};

	struct PAddAllow: public sox::Marshallable{
		enum {uri = (5 << 8 | protocol::CARD_EXCHANGE_SVID)};
		
		uint32_t allow_uid;
		uint32_t exchange_flag;//my_exchange_flag

		virtual void marshal(sox::Pack & p) const {
			p << allow_uid << exchange_flag;
		}

		virtual void unmarshal(const sox::Unpack & p) {
			p >> allow_uid >> exchange_flag;
		}
	};

	struct PAddAllowRes: public sox::Marshallable{
		enum {uri = (6 << 8 | protocol::CARD_EXCHANGE_SVID)};
		PAddAllowRes(){
			resCode = RES_SUCCESS;
		};
		uint16_t resCode;//RES_SUCCESS or RES_EDBERROR

		virtual void marshal(sox::Pack & p) const {
			p << resCode;
		}

		virtual void unmarshal(const sox::Unpack & p) {
			p >> resCode;
		}
	};	
	struct PBatchGetNamePic: public sox::Marshallable{
		enum {uri = (9 << 8 | protocol::CARD_EXCHANGE_SVID)};

		std::set<uint32_t> uids;
		uint32_t request_id;

		virtual void marshal(sox::Pack & p) const {
			sox::marshal_container(p, uids);
			p << request_id;
		}

		virtual void unmarshal(const sox::Unpack & p) {
			sox::unmarshal_container(p, std::inserter(uids, uids.begin()));
			p >> request_id;
		}
	};	
	struct PBatchGetNamePicRes: public sox::Marshallable{
		enum {uri = (10 << 8 | protocol::CARD_EXCHANGE_SVID)};
		PBatchGetNamePicRes(){};
		PBatchGetNamePicRes(uint32_t req_id){
			request_id = req_id;
			sprops.mapp.clear();			
		}
		UidMapProperties sprops;//uid-props
		uint32_t request_id;

		virtual void marshal(sox::Pack & p) const {
			sprops.marshal(p);
			p << request_id;
		}

		virtual void unmarshal(const sox::Unpack & p) {
			sprops.unmarshal(p);
			p >> request_id;
		}
	};	

        struct PBatchGetMobile: public sox::Marshallable{
		enum {uri = (11 << 8 | protocol::CARD_EXCHANGE_SVID)};

		std::set<uint32_t> uids;
		uint32_t request_id;

		virtual void marshal(sox::Pack & p) const {
			sox::marshal_container(p, uids);
			p << request_id;
		}

		virtual void unmarshal(const sox::Unpack & p) {
			sox::unmarshal_container(p, std::inserter(uids, uids.begin()));
			p >> request_id;
		}
	};	

	struct PBatchGetMobileRes: public sox::Marshallable{
		enum {uri = (12 << 8 | protocol::CARD_EXCHANGE_SVID)};
		PBatchGetMobileRes(){};
		PBatchGetMobileRes(uint32_t req_id){
			request_id = req_id;
			sprops.mapp.clear();			
		}
		UidMapProperties sprops;//uid-props
		uint32_t request_id;

		virtual void marshal(sox::Pack & p) const {
			sprops.marshal(p);
			p << request_id;
		}

		virtual void unmarshal(const sox::Unpack & p) {
			sprops.unmarshal(p);
			p >> request_id;
		}
	};	


}

}
#endif
