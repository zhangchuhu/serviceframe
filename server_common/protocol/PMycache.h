#ifndef PMYCACHE_H
#define PMYCACHE_H
#include "core/base_svid.h"
#include "packet.h"
#include "protocol/const.h"
#include <string.h>
#include <vector>

using namespace std;
using namespace sox;

namespace protocol { namespace mycache {

	struct PStorageLogin : public sox::Marshallable {
		enum { uri = (3000 << 8) | protocol::SESSCACHE_SVID };

		string name;
		string password;
		vector<uint32_t> tables;
		virtual void marshal(Pack & p) const {
			p << name << password;
			sox::marshal_container(p, tables);
		}

		virtual void unmarshal(const Unpack & p) {
			p >> name >> password;
			tables.clear();
			sox::unmarshal_container(p, std::back_inserter(tables));
		}
	};
	struct PStorageLoginRes : public sox::Marshallable {
		enum { uri = (2001 << 8) | protocol::SESSCACHE_SVID };
		uint16_t rescode;
		string content;
		virtual void marshal(Pack & p) const {
			p << rescode << content;
		}

		virtual void unmarshal(const Unpack & p) {
			p >> rescode >> content;
		}
	};


	struct storage_range : public sox::Marshallable {
		string db_name;
		string key_start;
		string key_stop;
		uint32_t offset;
		uint16_t limit;
		virtual void marshal(Pack & p) const {
			p << db_name << key_start << key_stop << offset << limit;
		}

		virtual void unmarshal(const Unpack & p) {
			p >> db_name >> key_start >> key_stop >> offset >> limit;
		}
	};
	struct struct_db_keys : public sox::Marshallable {
		vector<string> key_request;        
		virtual void marshal(Pack & p) const {
			sox::marshal_container(p, key_request);
		}

		virtual void unmarshal(const Unpack & p) {
			key_request.clear();
			sox::unmarshal_container(p, std::back_inserter(key_request));
		}
	};
	struct PStorageGet : public sox::Marshallable {
		enum { uri = (2002 << 8) | protocol::SESSCACHE_SVID };
		uint32_t context_id;
		bool  bGetDel;
		map<string, struct_db_keys> map_db_keys;   
		vector<storage_range>  range_request;
		virtual void marshal(Pack & p) const {
			p << context_id << bGetDel;
			sox::marshal_container(p, map_db_keys);
			sox::marshal_container(p, range_request);
		}

		virtual void unmarshal(const Unpack & p) {
			p >> context_id >> bGetDel;
			map_db_keys.clear();
			range_request.clear();
			sox::unmarshal_container(p, std::inserter(map_db_keys, map_db_keys.begin()));
			sox::unmarshal_container(p, std::back_inserter(range_request));
		}
	};
	struct storage_data : public sox::Marshallable {
		string key;
		string value;
		uint64_t timeStamp;
		bool isDel;
		virtual void marshal(Pack & p) const {
			p << key << value << timeStamp << isDel;
		}

		virtual void unmarshal(const Unpack & p) {
			p >> key >> value >> timeStamp >> isDel;
		}
	};
	struct vector_datas : public sox::Marshallable {
		vector<storage_data> data;
		virtual void marshal(Pack & p) const {
			sox::marshal_container(p, data);
		}

		virtual void unmarshal(const Unpack & p) {
			data.clear();
			sox::unmarshal_container(p, std::back_inserter(data));
		}
	};
	struct PStorageGetRes : public sox::Marshallable {
		enum { uri = (2003 << 8) | protocol::SESSCACHE_SVID };
		uint32_t context_id;
		bool bContinued;
		string error_string;
		map<string, vector_datas> db_data;
		virtual void marshal(Pack & p) const {
			p << context_id << bContinued << error_string;
			sox::marshal_container(p, db_data);
		}

		virtual void unmarshal(const Unpack & p) {
			p >> context_id >> bContinued >> error_string;
			db_data.clear();
			sox::unmarshal_container(p, std::inserter(db_data, db_data.begin()));
		}
	};
	struct PStorageSetOrDel : public sox::Marshallable {
		enum { uri = (2004 << 8) | protocol::SESSCACHE_SVID };
		uint32_t context_id;
		map<string, vector_datas> db_data;
		virtual void marshal(Pack & p) const {
			p << context_id;
			sox::marshal_container(p, db_data);
		}

		virtual void unmarshal(const Unpack & p) {
			p >> context_id;
			db_data.clear();
			sox::unmarshal_container(p, std::inserter(db_data, db_data.begin()));
		}
	};
	struct PStorageSetOrDelRes : public sox::Marshallable {
		enum { uri = (2005 << 8) | protocol::SESSCACHE_SVID };
		uint32_t context_id;
		uint16_t affected_rows;
		uint16_t timeold_rows;
		string error_string;
		virtual void marshal(Pack & p) const {
			p << context_id << affected_rows << timeold_rows << error_string;
		}

		virtual void unmarshal(const Unpack & p) {
			p >> context_id >> affected_rows >> timeold_rows >> error_string;
		}
	};
	struct PStorageGetDbInfos : public sox::Marshallable {
		enum { uri = (2006 << 8) | protocol::SESSCACHE_SVID };
		uint32_t context_id;
		virtual void marshal(Pack & p) const {
			p << context_id;
		}

		virtual void unmarshal(const Unpack & p) {
			p >> context_id;
		}
	};

	struct database_info : public sox::Marshallable {
		string db_name;
		uint64_t size;
		virtual void marshal(Pack & p) const {
			p << db_name << size;
		}

		virtual void unmarshal(const Unpack & p) {
			p >> db_name >> size;
		}
	};
	struct PStorageGetDbInfosRes : public sox::Marshallable {
		enum { uri = (2007 << 8) | protocol::SESSCACHE_SVID };
		uint32_t context_id;
		map<string, database_info> db_infos;
		virtual void marshal(Pack & p) const {
			p << context_id;
			sox::marshal_container(p, db_infos);
		}

		virtual void unmarshal(const Unpack & p) {
			p >> context_id;
			db_infos.clear();
			sox::unmarshal_container(p, std::inserter(db_infos, db_infos.begin()));
		}
	};
	struct PStorageCreateDB : public sox::Marshallable {
		enum { uri = (2008 << 8) | protocol::SESSCACHE_SVID };
		uint32_t context_id;
		string db_name;
		virtual void marshal(Pack & p) const {
			p << context_id << db_name;
		}

		virtual void unmarshal(const Unpack & p) {
			p >> context_id >> db_name;
		}
	};
	struct PStorageCreateDBRes : public sox::Marshallable {
		enum { uri = (2009 << 8) | protocol::SESSCACHE_SVID };
		uint32_t context_id;
		uint16_t resCode;
		string error_info;
		virtual void marshal(Pack & p) const {
			p << context_id << resCode << error_info;
		}

		virtual void unmarshal(const Unpack & p) {
			p >> context_id >> resCode >> error_info;
		}
	};




	struct manager_range : public sox::Marshallable {
		string key_start;
		string key_stop;
		uint32_t offset;
		uint16_t limit;
		virtual void marshal(Pack & p) const {
			p << key_start << key_stop << offset << limit;
		}

		virtual void unmarshal(const Unpack & p) {
			p >> key_start >> key_stop >> offset >> limit;
		}
	};
	struct PManagerGet : public sox::Marshallable {
		enum { uri = (3002 << 8) | protocol::SESSCACHE_SVID };
		uint32_t context_id;
		bool  bGetDel;
		vector<string> key_request;          
		vector<manager_range>  range_request;
		virtual void marshal(Pack & p) const {
			p << context_id << bGetDel;
			sox::marshal_container(p, key_request);
			sox::marshal_container(p, range_request);
		}

		virtual void unmarshal(const Unpack & p) {
			p >> context_id >> bGetDel;
			key_request.clear();
			range_request.clear();
			sox::unmarshal_container(p, std::back_inserter(key_request));
			sox::unmarshal_container(p, std::back_inserter(range_request));
		}
	};
	struct manager_data : public sox::Marshallable {
		string key;
		string value;
		uint64_t timeStamp;
		uint8_t isDel;
		virtual void marshal(Pack & p) const {
			p << key << value << timeStamp << isDel;
		}

		virtual void unmarshal(const Unpack & p) {
			p >> key >> value >> timeStamp >> isDel;
		}
	};
	struct PManagerGetRes : public sox::Marshallable {
		enum { uri = (3003 << 8) | protocol::SESSCACHE_SVID };
		uint32_t context_id;
		bool bContinued;
		string error_string;
		vector<manager_data> data;
		virtual void marshal(Pack & p) const {
			p << context_id << bContinued << error_string;
			sox::marshal_container(p, data);
		}

		virtual void unmarshal(const Unpack & p) {
			p >> context_id >> bContinued >> error_string;
			data.clear();
			sox::unmarshal_container(p, std::back_inserter(data));
		}
	};
	struct PManagerSetOrDel : public sox::Marshallable {
		enum { uri = (3004 << 8) | protocol::SESSCACHE_SVID };
		uint32_t context_id;
		vector<manager_data> data;
		virtual void marshal(Pack & p) const {
			p << context_id;
			sox::marshal_container(p, data);
		}

		virtual void unmarshal(const Unpack & p) {
			p >> context_id;
			data.clear();
			sox::unmarshal_container(p, std::back_inserter(data));
		}
	};
	struct PManagerSetOrDelRes : public sox::Marshallable {
		enum { uri = (3005 << 8) | protocol::SESSCACHE_SVID };
		uint32_t context_id;
		uint16_t affected_rows;
		uint16_t timeold_rows;
		string error_string;
		virtual void marshal(Pack & p) const {
			p << context_id << affected_rows << timeold_rows << error_string;
		}

		virtual void unmarshal(const Unpack & p) {
			p >> context_id >> affected_rows >> timeold_rows >> error_string;
		}
	};



}}
#endif

