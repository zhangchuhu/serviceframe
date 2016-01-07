#ifndef _PDBNODE_PACKAGE_DEF_H_
#define _PDBNODE_PACKAGE_DEF_H_

#include "packet.h"
#include <vector>
#include <map>
#include <string>
#include "common/core/base_svid.h"
#include <stdint.h>

namespace server{
namespace db_node{
	struct coding_util
	{
		static void coding_vec(sox::Pack &p,const std::vector<std::string>& v)
		{
			uint32_t len=v.size();
			p << len;
			std::vector<std::string>::const_iterator i=v.begin();
			for(;i!=v.end();++i)
			{
				p.push_varstr32(i->data(), i->size());
			}
		}
		static void decode_vec(const sox::Unpack &p,std::vector<std::string>& v)
		{
			uint32_t len=0;
			p >> len;
			v.resize(len);
			std::vector<std::string>::iterator i=v.begin();
			for(;i!=v.end();++i)
			{
				(*i)=p.pop_varstr32();
			}
		}
	};
	
	struct DBOperation : public sox::Marshallable
	{
		DBOperation(){}
		~DBOperation(){}
		DBOperation(const DBOperation& o)
		{
			op=o.op;
			table_name=o.table_name;
			key=o.key;
			value=o.value;
			reserved=o.reserved;
		}
		DBOperation& operator = (const DBOperation& o)
		{
			op=o.op;
			table_name=o.table_name;
			key=o.key;
			value=o.value;
			reserved=o.reserved;
			return *this;
		}
		uint32_t op; //0:set 1:del
		std::string table_name;
		std::map<std::string,std::string> key;
		std::map<std::string,std::string> value;///empty when op is del
		std::string reserved;
		virtual void marshal(sox::Pack &p) const{
			 p << op;
			 p.push_varstr32(table_name.data(), table_name.size());
			 coding_map(p,key);
			 coding_map(p,value);
			 p.push_varstr32(reserved.data(), reserved.size());
		}
		virtual void unmarshal(const sox::Unpack &p) {
			 p >> op;
			 table_name=p.pop_varstr32();
			 decode_map(p,key);
			 decode_map(p,value);
			 reserved=p.pop_varstr32();
		}
		void coding_map(sox::Pack &p,const std::map<std::string,std::string>& m) const
		{
			uint32_t len=m.size();
			p << len;
			std::map<std::string,std::string>::const_iterator i=m.begin();
			for(;i!=m.end();++i)
			{
				p.push_varstr32(i->first.data(), i->first.size());
				p.push_varstr32(i->second.data(), i->second.size());
			}
		}
		void decode_map(const sox::Unpack &p,std::map<std::string,std::string>& m)
		{
			uint32_t len=0;
			p >> len;
			m.clear();
			std::string temk;
			std::string temv;
			for(uint32_t i=0;i<len;++i)
			{
				temk=p.pop_varstr32();
				temv=p.pop_varstr32();
				m[temk]=temv;
			}
		}
	};
	struct DBPackOperationCore : public sox::Marshallable
	{
		DBPackOperationCore(){}
		~DBPackOperationCore(){}
		DBPackOperationCore(const DBPackOperationCore& o)
		{
			time_stamp=o.time_stamp;
			_ops=o._ops;
			reserved=o.reserved;
		}
		DBPackOperationCore& operator = (const DBPackOperationCore& o)
		{
			time_stamp=o.time_stamp;
			_ops=o._ops;
			reserved=o.reserved;
			return *this;
		}
		uint64_t time_stamp;
		std::vector<DBOperation> _ops;
		std::string reserved;
		virtual void marshal(sox::Pack &p) const{
			p << time_stamp;
			uint32_t len=_ops.size();
			p << len;
			for(uint32_t i=0;i<len;++i)
			{
				_ops[i].marshal(p);
			}
			p.push_varstr32(reserved.data(), reserved.size());
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> time_stamp;
			uint32_t len=0;
			p >> len;
			_ops.resize(len);
			for(uint32_t i=0;i<len;++i)
			{
				_ops[i].unmarshal(p);
			}
			reserved=p.pop_varstr32();
		}
	};
	struct DBPackOperation : public sox::Marshallable
	{
		enum {uri = (1 << 8) | ::protocol::DBNODE_SVID};
		uint64_t mode;         ////strong write or weak write
		DBPackOperationCore core_pack;
		std::string reserved;  ////use for contex
		virtual void marshal(sox::Pack &p) const{
			p << mode;
			core_pack.marshal(p);
			p.push_varstr32(reserved.data(), reserved.size());
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> mode;
			core_pack.unmarshal(p);
			reserved=p.pop_varstr32();
		}
	};
	struct DBPackOperationResp : public sox::Marshallable
	{
		enum {uri = (129 << 8) | ::protocol::DBNODE_SVID};
		uint32_t ret_code; //0:succeed  1: disk_error  2: net_error 
		std::string reserved;  ////copy DBPackOperation reserved
		virtual void marshal(sox::Pack &p) const{
			p << ret_code;
			p.push_varstr32(reserved.data(), reserved.size());
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> ret_code;
			reserved=p.pop_varstr32();	
		}
	};
	struct DBOpQueueReq : public sox::Marshallable
	{
		enum {uri = (2 << 8) | ::protocol::DBNODE_SVID};
		std::string source_name;
		std::string target_name;
		uint64_t state;//0:normal  1: start_up
		uint64_t req_lsn;
		uint64_t req_op_number_hint;
		std::string reserved;
		virtual void marshal(sox::Pack &p) const{
			p.push_varstr32(source_name.data(), source_name.size());
			p.push_varstr32(target_name.data(), target_name.size());
			p << state;
			p << req_lsn;
			p << req_op_number_hint;
			p.push_varstr32(reserved.data(), reserved.size());
		}
		virtual void unmarshal(const sox::Unpack &p) {
			source_name=p.pop_varstr32();
			target_name=p.pop_varstr32();
			p >> state;
			p >> req_lsn;
			p >> req_op_number_hint;
			reserved=p.pop_varstr32();	
		}
	};
	struct DBOpQueueResp : public sox::Marshallable
	{
		enum {uri = (130 << 8) | ::protocol::DBNODE_SVID};
		std::string source_name;
		std::string target_name;
		uint64_t ret_code;//0:ok  1:not find
		uint64_t state;//0:normal  1: init set  2: active push
		uint64_t resp_begin_lsn;
		uint64_t commit_time_stamp;
		std::vector<std::string> _packs;
		std::string reserved;
		virtual void marshal(sox::Pack &p) const{
			p.push_varstr32(source_name.data(), source_name.size());
			p.push_varstr32(target_name.data(), target_name.size());
			p << ret_code;
			p << state;
			p << resp_begin_lsn;
			p << commit_time_stamp;
			coding_util::coding_vec(p,_packs);
			p.push_varstr32(reserved.data(), reserved.size());
		}
		virtual void unmarshal(const sox::Unpack &p) {
			source_name=p.pop_varstr32();
			target_name=p.pop_varstr32();
			p >> ret_code;
			p >> state;
			p >> resp_begin_lsn;
			p >> commit_time_stamp;
			coding_util::decode_vec(p,_packs);
			reserved=p.pop_varstr32();
		}
	};
	struct DBRemoteSQLReq : public sox::Marshallable
	{
		enum {uri = (3 << 8) | ::protocol::DBNODE_SVID};
		std::string source_name;
		std::string target_name;
		std::string SQL;
		uint64_t req_id;
		std::string reserved;
		virtual void marshal(sox::Pack &p) const{
			p.push_varstr32(source_name.data(), source_name.size());
			p.push_varstr32(target_name.data(), target_name.size());
			p.push_varstr32(SQL.data(), SQL.size());
			p << req_id;
			p.push_varstr32(reserved.data(), reserved.size());
		}
		virtual void unmarshal(const sox::Unpack &p) {
			source_name=p.pop_varstr32();
			target_name=p.pop_varstr32();
			SQL=p.pop_varstr32();
			p >> req_id;
			reserved=p.pop_varstr32();
		}
	};
	struct DBRemoteSQLResp : public sox::Marshallable
	{
		enum {uri = (131 << 8) | ::protocol::DBNODE_SVID};
		std::string source_name;
		std::string target_name;
		uint64_t req_id;
		uint32_t db_node_ret;//0:ok 1:disk fail 2:other fail
		uint32_t sql_ret; //return from sqlfunction
		std::vector<std::string> result;
		std::string reserved;
		virtual void marshal(sox::Pack &p) const{
			p.push_varstr32(source_name.data(), source_name.size());
			p.push_varstr32(target_name.data(), target_name.size());
			p << req_id;
			p << db_node_ret;
			p << sql_ret;
			coding_util::coding_vec(p,result);
			p.push_varstr32(reserved.data(), reserved.size());
		}
		virtual void unmarshal(const sox::Unpack &p) {
			source_name=p.pop_varstr32();
			target_name=p.pop_varstr32();
			p >> req_id;
			p >> db_node_ret;
			p >> sql_ret;
			coding_util::decode_vec(p,result);
			reserved=p.pop_varstr32();
		}
	};
	struct DBStateReq : public sox::Marshallable
	{
		enum {uri = (4 << 8) | ::protocol::DBNODE_SVID};
		uint32_t which;
		std::string reserved;
		virtual void marshal(sox::Pack &p) const{
			p << which;
			p.push_varstr32(reserved.data(), reserved.size());
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> which;
			reserved=p.pop_varstr32();
		}
	};
	struct DBStateResp : public sox::Marshallable
	{
		enum {uri = (132 << 8) | ::protocol::DBNODE_SVID};
		uint32_t which;
		std::string result;
		std::string reserved;
		virtual void marshal(sox::Pack &p) const{
			p << which;
			p.push_varstr32(result.data(), result.size());
			p.push_varstr32(reserved.data(), reserved.size());
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> which;
			result=p.pop_varstr32();
			reserved=p.pop_varstr32();
		}
	};
	struct DBAdminReq : public sox::Marshallable
	{
		enum {uri = (5 << 8) | ::protocol::DBNODE_SVID};
		uint64_t req_id;
		uint32_t option;
		std::string command;
		std::string reserved;
		virtual void marshal(sox::Pack &p) const{
			p << req_id;
			p << option;
			p.push_varstr32(command.data(), command.size());
			p.push_varstr32(reserved.data(), reserved.size());
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> req_id;
			p >> option;
			command=p.pop_varstr32();
			reserved=p.pop_varstr32();
		}
	};
	struct DBAdminResp : public sox::Marshallable
	{
		enum {uri = (133 << 8) | ::protocol::DBNODE_SVID};
		uint64_t req_id;
		uint32_t option;
		uint32_t result;
		std::string reserved;
		virtual void marshal(sox::Pack &p) const{
			p << req_id;
			p << option;
			p << result;
			p.push_varstr32(reserved.data(), reserved.size());
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> req_id;
			p >> option;
			p >> result;
			reserved=p.pop_varstr32();
		}
	};
	struct DBPackOperationSync : public sox::Marshallable
	{
		enum {uri = (6 << 8) | ::protocol::DBNODE_SVID};
		std::string source_name;
		std::string target_name;
		uint64_t lsn;
		DBPackOperationCore core_pack;
		std::string reserved;  ////unused
		virtual void marshal(sox::Pack &p) const{
			p.push_varstr32(source_name.data(), source_name.size());
			p.push_varstr32(target_name.data(), target_name.size());
			p << lsn;
			core_pack.marshal(p);
			p.push_varstr32(reserved.data(), reserved.size());
		}
		virtual void unmarshal(const sox::Unpack &p) {
			source_name=p.pop_varstr32();
			target_name=p.pop_varstr32();
			p >> lsn;
			core_pack.unmarshal(p);
			reserved=p.pop_varstr32();
		}
	};
	struct DBPackOperationSyncPermit : public sox::Marshallable
	{
		enum {uri = (134 << 8) | ::protocol::DBNODE_SVID};
		std::string source_name;
		std::string target_name;
		uint32_t permit_num;
		std::string reserved;  ////unused
		virtual void marshal(sox::Pack &p) const{
			p.push_varstr32(source_name.data(), source_name.size());
			p.push_varstr32(target_name.data(), target_name.size());
			p << permit_num;
			p.push_varstr32(reserved.data(), reserved.size());
		}
		virtual void unmarshal(const sox::Unpack &p) {
			source_name=p.pop_varstr32();
			target_name=p.pop_varstr32();
			p >> permit_num;
			reserved=p.pop_varstr32();
		}
	};

	///////////////////
	struct SQPushQueueRequest : public sox::Marshallable
	{
		enum {uri = (7 << 8) | ::protocol::DBNODE_SVID};
		std::string m_contex;
		uint64_t mode;         ////strong write or weak write
		DBPackOperationCore core_pack;
		std::string reserved;  ////no use
		virtual void marshal(sox::Pack &p) const
		{
			p.push_varstr32(m_contex.data(), m_contex.size());
			p << mode;
			core_pack.marshal(p);
			p.push_varstr32(reserved.data(), reserved.size());
		}
		virtual void unmarshal(const sox::Unpack &p)
		{
			m_contex=p.pop_varstr32();
			p >> mode;
			core_pack.unmarshal(p);
			reserved=p.pop_varstr32();
		}
	};
	struct SQPushQueueResp : public sox::Marshallable
	{
		enum {uri = (135 << 8) | ::protocol::DBNODE_SVID};
		std::string m_contex;
		uint32_t ret_code; //0:succeed  1: disk_error  2: net_error     257:same key same time  258:too big packet
		uint64_t op_serial_code;
		std::string syncnode_name;
		std::string reserved;  ////if size==sizeof(uint64_t) then is time_stamp
		virtual void marshal(sox::Pack &p) const
		{
			p.push_varstr32(m_contex.data(), m_contex.size());
			p << ret_code;
			p << op_serial_code;
			p.push_varstr32(syncnode_name.data(), syncnode_name.size());
			p.push_varstr32(reserved.data(), reserved.size());
		}
		virtual void unmarshal(const sox::Unpack &p)
		{
			m_contex=p.pop_varstr32();
			p >> ret_code;
			p >> op_serial_code;
			syncnode_name=p.pop_varstr32();
			reserved=p.pop_varstr32();
		}
	};
	struct SQSetOperation : public sox::Marshallable
	{
		enum {uri = (8 << 8) | ::protocol::DBNODE_SVID};
		uint32_t do_code; ////0 default
		uint64_t op_serial_code;
		std::string syncnode_name;
		DBPackOperationCore core_pack;
		std::string reserved;  ////copy DBPackOperation reserved
		virtual void marshal(sox::Pack &p) const
		{
			p << do_code;
			p << op_serial_code;
			p.push_varstr32(syncnode_name.data(), syncnode_name.size());
			core_pack.marshal(p);
			p.push_varstr32(reserved.data(), reserved.size());
		}
		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> do_code;
			p >> op_serial_code;
			syncnode_name=p.pop_varstr32();
			core_pack.unmarshal(p);
			reserved=p.pop_varstr32();
		}
	};
	struct SQLsnCommited : public sox::Marshallable
	{
		enum {uri = (9 << 8) | ::protocol::DBNODE_SVID};
		uint64_t op_serial_code;
		std::string syncnode_name;
		std::string reserved;  ////copy DBPackOperation reserved
		virtual void marshal(sox::Pack &p) const
		{
			p << op_serial_code;
			p.push_varstr32(syncnode_name.data(), syncnode_name.size());
			p.push_varstr32(reserved.data(), reserved.size());
		}
		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> op_serial_code;
			syncnode_name=p.pop_varstr32();
			reserved=p.pop_varstr32();
		}
	};
	struct SQDeleteTimeStamp : public sox::Marshallable
	{
		enum {uri = (10 << 8) | ::protocol::DBNODE_SVID};
		uint64_t mini_useful_time_stamp;
		uint32_t max_delete_hint;
		std::string sync_name;
		std::string reserved;  ////copy DBPackOperation reserved
		virtual void marshal(sox::Pack &p) const{
			p << mini_useful_time_stamp;
			p << max_delete_hint;
			p.push_varstr32(sync_name.data(), sync_name.size());
			p.push_varstr32(reserved.data(), reserved.size());
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> mini_useful_time_stamp;
			p >> max_delete_hint;
			sync_name=p.pop_varstr32();
			reserved=p.pop_varstr32();
		}
	};


	struct PDBCmd : public sox::Marshallable {
		enum {uri = (140 << 8) | ::protocol::DBNODE_SVID};

		std::string strCmd;	// sync, del
		std::string strTableName;
		std::map<std::string, std::string> mapKeys;
		std::map<std::string, std::string> mapNonKeys;

		virtual void marshal(sox::Pack &p) const{
			p << strCmd << strTableName;

			p.push_uint32(mapKeys.size());
			for (std::map<std::string, std::string>::const_iterator it = mapKeys.begin(); it != mapKeys.end(); ++it)
			{
				p << it->first;
				p.push_varstr32(it->second.c_str(), it->second.size());
			}

			p.push_uint32(mapNonKeys.size());
			for (std::map<std::string, std::string>::const_iterator it = mapNonKeys.begin(); it != mapNonKeys.end(); ++it)
			{
				p << it->first;
				p.push_varstr32(it->second.c_str(), it->second.size());
			}
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> strCmd >> strTableName;

			for (uint32_t i = p.pop_uint32(); i > 0; --i)
			{
				std::string strFirst;
				p >> strFirst;
				std::string strSecond = p.pop_varstr32();
				mapKeys[strFirst] = strSecond;
			}

			for (uint32_t i = p.pop_uint32(); i > 0; --i)
			{
				std::string strFirst;
				p >> strFirst;
				std::string strSecond = p.pop_varstr32();
				mapNonKeys[strFirst] = strSecond;
			}
		}
	};

	struct PDBCmdRes : public sox::Marshallable {
		enum {uri = (141 << 8) | ::protocol::DBNODE_SVID};
		
		std::string strRes;

		virtual void marshal(sox::Pack &p) const{
			p << strRes;
		}

		virtual void unmarshal(const sox::Unpack &p) {
			p >> strRes;
		}
	};
}
}

#endif

