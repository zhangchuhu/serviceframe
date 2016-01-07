#ifndef _P_LG_GEN_NUM_H_
#define _P_LG_GEN_NUM_H_

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"
#include <string>
#include <iostream>
#include <list>

using namespace std;

namespace protocol {
static const unsigned int LG_GEN_NUM_SVID = 177;
}

namespace server {
namespace ha_num_gen {

// \brief: 产号请求(一次申请一种资源, 申请指定的个数)
struct PRequestNum: public sox::Marshallable 
{
	public:
		enum { uri = (1 << 8) | protocol::LG_GEN_NUM_SVID };
	public:
		string strSeq; // 异步调用标记(调用方编码,gennum会在应答中原样返回)
		string strResourceName; // 资源名称, 比如: /yyid,/yyuid,...
		uint32_t uRequestAmount; // 每种资源所要申请的个数

	public:
		virtual void marshal(sox::Pack &p) const {
			p << strSeq << strResourceName << uRequestAmount;
		}

		virtual void unmarshal(const sox::Unpack &p) {
			p >> strSeq >> strResourceName >> uRequestAmount;
		}
};

// \brief: 应答产号请求(返回一种资源的对应号段)
struct PRequestNumRes: public sox::Marshallable 
{
	public:
		enum { uri = (2 << 8) | protocol::LG_GEN_NUM_SVID };

	public:
		string strSeq; // 请求时发送的异步包标记符号
		string strResourceName; // 资源名, 会返回来
		uint32_t uRes; // 是否成功 { 0 - success; others - system error. }
		vector<pair<uint64_t, uint64_t> > ids; // 返回的资源的号段起始值: [iStart, iEnd] 如: [1:2] => 1 和 2

	public:
		virtual void marshal(sox::Pack &p) const 
		{
			p << strSeq << strResourceName << uRes;

			uint32_t idsCnt = ids.size();
			p << idsCnt;

			for (std::vector<std::pair<uint64_t, uint64_t> >::const_iterator i =
					ids.begin(); i != ids.end(); ++i) {
				p << i->first;
				p << i->second;
			}

		}

		virtual void unmarshal(const sox::Unpack &p) 
		{
			p >> strSeq >> strResourceName >> uRes;

			uint32_t num = 0;
			uint64_t key = 0;
			uint64_t val = 0;
			p >> num;
			for (size_t i = 0; i < num; ++i) {
				p >> key >> val;
				ids.push_back(std::pair<uint64_t, uint64_t>(key, val));
			}
		}

};

// generates a series of numbers of different resources at the same time.
//			but only one number can be generated each time for one resource.
//			specifying too many resource names will result in slow response at the first time when gen_num_d is launched.
// \brief : 一次申请多种资源, 一种资源返回一个号
struct PRequestNumSingleBatch: public sox::Marshallable 
{
	public:
		enum { uri = (5 << 8) | protocol::LG_GEN_NUM_SVID };

	public:

		std::string strSeq; // 用于标识异步调用的包序号
		std::string strResourceNames; // 资源名的组合串. 用";"分割.如: "/uid;/yyuid;/yyid", 这样就同时返回3钟资源

	public:
		PRequestNumSingleBatch() {};
		PRequestNumSingleBatch(string &sSeq, string &sResourceNames)
		{
			strSeq = sSeq;
			strResourceNames = sResourceNames;
		}
	public:
		virtual void marshal(sox::Pack &p) const 
		{
			p << strSeq << strResourceNames;
		}

		virtual void unmarshal(const sox::Unpack &p) 
		{
			p >> strSeq >> strResourceNames;
		}
};

// \brief: 应答 PRequestNumSingleBatch 请求
struct PRequestNumSingleBatchRes: public sox::Marshallable 
{
	public:
		enum { uri = (6 << 8) | protocol::LG_GEN_NUM_SVID };

	public:
		string strSeq; // 用于异步调用的包序号
		string strResourceNames; // 资源名称, 和请求一样
		uint32_t uRes; // 申请结果是否成功: { 0 - success, others - system error. }
		vector<uint64_t> ids; // 和资源名顺序对应的号码返回值 比如:  ids = vector{ yyuid, yyid, uid };

	public:
		virtual void marshal(sox::Pack &p) const 
		{
			p << strSeq << strResourceNames << uRes;

			uint32_t idsCnt = ids.size();
			p << idsCnt;

			for (std::vector<uint64_t>::const_iterator i =
					ids.begin(); i != ids.end(); ++i) {
				p << *i;
			}
		}

		virtual void unmarshal(const sox::Unpack &p) 
		{
			p >> strSeq >> strResourceNames >> uRes;

			uint32_t num = 0;
			uint64_t val = 0;
			p >> num;
			for (size_t i = 0; i < num; ++i) {
				p >> val;
				ids.push_back(val);
			}
		}
};


struct PGennumDumpInfoReq: public sox::Voidmable {
	enum {
		uri = (3 << 8) | protocol::LG_GEN_NUM_SVID
	};
};

struct PGennumDumpInfoRes: public sox::Marshallable {
	enum {
		uri = (4 << 8) | protocol::LG_GEN_NUM_SVID
	};

	struct range {
		std::string resource;
		uint64_t begin;
		uint64_t end; // Excluded
	};

	uint32_t serverId;
	std::map<std::string, std::list<range> > infos;

	virtual void marshal(sox::Pack &p) const 
	{
		p << serverId;

		p << infos.size();
		for (std::map<std::string, std::list<range> >::const_iterator i =
				infos.begin(); i != infos.end(); ++i) {
			p << i->first;

			p << i->second.size();
			for (std::list<range>::const_iterator j = i->second.begin(); j!=i->second.end(); ++j) {
				p << j->resource;
				p << j->begin;
				p << j->end;
			}
		}
	}

	virtual void unmarshal(const sox::Unpack &p) 
	{
		p >> serverId;

		uint32_t mapSize = 0;
		p >> mapSize;

		for (; mapSize >0; --mapSize) {
			std::string key;
			uint32_t listSize = 0;
			std::list<range> list;

			p >> key;
			p >> listSize;

			for(; listSize>0; --listSize) {
				range r;
				p >> r.resource >> r.begin >> r.end;
				list.push_back(r);
			}
			infos[key] = list;
		}
	}

};

}
}

#endif
