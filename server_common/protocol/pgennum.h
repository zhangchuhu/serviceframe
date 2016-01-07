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

// \brief: ��������(һ������һ����Դ, ����ָ���ĸ���)
struct PRequestNum: public sox::Marshallable 
{
	public:
		enum { uri = (1 << 8) | protocol::LG_GEN_NUM_SVID };
	public:
		string strSeq; // �첽���ñ��(���÷�����,gennum����Ӧ����ԭ������)
		string strResourceName; // ��Դ����, ����: /yyid,/yyuid,...
		uint32_t uRequestAmount; // ÿ����Դ��Ҫ����ĸ���

	public:
		virtual void marshal(sox::Pack &p) const {
			p << strSeq << strResourceName << uRequestAmount;
		}

		virtual void unmarshal(const sox::Unpack &p) {
			p >> strSeq >> strResourceName >> uRequestAmount;
		}
};

// \brief: Ӧ���������(����һ����Դ�Ķ�Ӧ�Ŷ�)
struct PRequestNumRes: public sox::Marshallable 
{
	public:
		enum { uri = (2 << 8) | protocol::LG_GEN_NUM_SVID };

	public:
		string strSeq; // ����ʱ���͵��첽����Ƿ���
		string strResourceName; // ��Դ��, �᷵����
		uint32_t uRes; // �Ƿ�ɹ� { 0 - success; others - system error. }
		vector<pair<uint64_t, uint64_t> > ids; // ���ص���Դ�ĺŶ���ʼֵ: [iStart, iEnd] ��: [1:2] => 1 �� 2

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
// \brief : һ�����������Դ, һ����Դ����һ����
struct PRequestNumSingleBatch: public sox::Marshallable 
{
	public:
		enum { uri = (5 << 8) | protocol::LG_GEN_NUM_SVID };

	public:

		std::string strSeq; // ���ڱ�ʶ�첽���õİ����
		std::string strResourceNames; // ��Դ������ϴ�. ��";"�ָ�.��: "/uid;/yyuid;/yyid", ������ͬʱ����3����Դ

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

// \brief: Ӧ�� PRequestNumSingleBatch ����
struct PRequestNumSingleBatchRes: public sox::Marshallable 
{
	public:
		enum { uri = (6 << 8) | protocol::LG_GEN_NUM_SVID };

	public:
		string strSeq; // �����첽���õİ����
		string strResourceNames; // ��Դ����, ������һ��
		uint32_t uRes; // �������Ƿ�ɹ�: { 0 - success, others - system error. }
		vector<uint64_t> ids; // ����Դ��˳���Ӧ�ĺ��뷵��ֵ ����:  ids = vector{ yyuid, yyid, uid };

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
