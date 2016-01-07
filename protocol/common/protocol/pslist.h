#ifndef PSLIST_H_
#define PSLIST_H_
#include "common/packet.h"
#include <vector>
#include <iterator>
#include "common/core/base_svid.h"
#include "common/protocol/const.h"
#include "common/protocol/ChannelRoler.h"

//#define FOLDER_SUFFIX "@f"
namespace protocol {
namespace slist {
struct helper{
	static bool isFolder(const std::string &id){
		return atoi(id.data()) < 1000000;
	}
};

// 客户端都采用SListItem2，SListItem将被删除。
struct SListItem : public sox::Marshallable {
	enum ItemType{
		chanel,
		folder
	};
	uint32_t sid;
	uint32_t asid;
	uint32_t pid;
	uint32_t users;
	std::string snick;
	uint32_t owner;
	ItemType type;
	bool bLimit;
	bool bPublic;

	SListItem():users(0), type(chanel){}
	SListItem(int s, int as, int p, const std::string &n, uint32_t o, ItemType t, bool l) :
		sid(s), asid(as), pid(p), users(0), snick(n), owner(o), type(t), bLimit(l) {
	}
	virtual void marshal(sox::Pack &p) const {
		p << sid << asid << pid << users << snick << owner;
		p.push_uint8(type);
		p.push_uint8(bLimit);
		p.push_uint8(bPublic);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> sid >> asid >> pid >> users >> snick >> owner;
		type = (ItemType)p.pop_uint8();
		bLimit = p.pop_uint8() != 0;
		bPublic = p.pop_uint8() != 0;
	}
};


//lzd: 客户端都采用SListItem2，SListItem将被删除。fxw: 记着SListItem要删除
struct SListItem2 : public sox::Marshallable {
	enum ItemType{
		chanel,
		folder
	};
	uint32_t sid;
	uint32_t asid;
	uint32_t pid;
	uint32_t users;
	std::string snick;
	uint32_t owner;
	ItemType type;
	bool bLimit;
	bool bPublic;
	uint32_t logoIndex;
	std::string logoUrl;

	SListItem2():users(0), type(chanel){}
	SListItem2(int s, int as, int p, const std::string &n, uint32_t o, ItemType t
				, bool l, uint32_t nLogoIndex, const std::string& url) 
		: sid(s), asid(as), pid(p), users(0), snick(n), owner(o), type(t), bLimit(l)
		, logoIndex(nLogoIndex), logoUrl(url) {
	}
	virtual void marshal(sox::Pack &p) const {
		p << sid << asid << pid << users << snick << owner;
		p.push_uint8(type);
		p.push_uint8(bLimit);
		p.push_uint8(bPublic);
		p << logoIndex << logoUrl;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> sid >> asid >> pid >> users >> snick >> owner;
		type = (ItemType)p.pop_uint8();
		bLimit = p.pop_uint8() != 0;
		bPublic = p.pop_uint8() != 0;
		p >> logoIndex >> logoUrl;
	}
};


//增加了收藏夹备注名称
struct SListItem3 : public sox::Marshallable
{
	enum ItemType{
		chanel,
		folder
	};
	uint32_t sid;
	uint32_t asid;
	uint32_t pid;
	uint32_t users;
	std::string snick;
	uint32_t owner;
	ItemType type;
	bool bLimit;
	bool bPublic;
	uint32_t logoIndex;
	std::string logoUrl;
    std::string sidNameRemark;   //备注收藏夹名称

	SListItem3():users(0), type(chanel){}
	SListItem3(int s, int as, int p, const std::string &n, uint32_t o, ItemType t
				, bool l, uint32_t nLogoIndex, const std::string& url,std::string& channelName) 
		: sid(s), asid(as), pid(p), users(0), snick(n), owner(o), type(t), bLimit(l)
		, logoIndex(nLogoIndex), logoUrl(url),sidNameRemark(channelName) {
	}
	virtual void marshal(sox::Pack &p) const {
		p << sid << asid << pid << users << snick << owner;
		p.push_uint8(type);
		p.push_uint8(bLimit);
		p.push_uint8(bPublic);
		p << logoIndex << logoUrl;
		p << sidNameRemark;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> sid >> asid >> pid >> users >> snick >> owner;
		type = (ItemType)p.pop_uint8();
		bLimit = p.pop_uint8() != 0;
		bPublic = p.pop_uint8() != 0;
		p >> logoIndex >> logoUrl;
		p >> sidNameRemark;
	}
	
};

struct PSyncSList : public sox::Voidmable {
	enum {uri = (1 << 8 | protocol::SLIST_SVID)};

};

struct PSyncSListRes : public sox::Marshallable {
	enum {uri = (1 << 8 | protocol::SLIST_SVID)};
	std::vector<SListItem> items;
	uint32_t resCode;
	virtual void marshal(sox::Pack &p) const {
		marshal_container(p, items);
		p << resCode;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		unmarshal_container(p, std::back_inserter(items));
		p >> resCode;
	}
};

struct PAddFolder : public sox::Marshallable {
	enum {uri = (2 << 8 | protocol::SLIST_SVID)};
	std::string nick;
	uint32_t pid;
	virtual void marshal(sox::Pack &p) const {
		p << pid << nick;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> pid >>nick;
	}
};

struct PAddFolderRes : public sox::Marshallable {
	enum {uri = (2 << 8 | protocol::SLIST_SVID)};
	std::string nick;
	uint32_t fid;
	uint32_t pid;
	virtual void marshal(sox::Pack &p) const {
		p << nick << fid << pid;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> nick >> fid >> pid;
	}
};

struct PRemoveFolder : public sox::Marshallable {
	enum {uri = (3 << 8 | protocol::SLIST_SVID)};
	uint32_t fid;
	uint32_t pid;
	virtual void marshal(sox::Pack &p) const {
		p << fid << pid;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> fid >> pid;
	}
};

struct PRemoveFolderRes : public PRemoveFolder {
};

struct PAddSess: public sox::Marshallable {
	enum {uri = (4 << 8 | protocol::SLIST_SVID)};
	uint32_t sid;
	uint32_t asid;
	uint32_t pid;
	//todo
	std::string nick;
	uint32_t ownerid;
	bool bLimit;

	virtual void marshal(sox::Pack &p) const {
		p << sid << asid << pid << nick << ownerid;
		p.push_uint8(bLimit);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> sid >> asid >> pid >> nick >> ownerid;
		bLimit = p.pop_uint8() != 0;
	}
};

struct PAddSessRes: public PAddSess {
	//if pid has child more them one, onchange
};


struct PRemoveSess: public sox::Marshallable {
	enum {uri = (5 << 8 | protocol::SLIST_SVID)};
	uint32_t sid;
	uint32_t pid;
	virtual void marshal(sox::Pack &p) const {
		p << sid << pid;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> sid >> pid;
	}
};

struct PRemoveSessRes: public PRemoveSess {
	//if pid is empty remove else onchange happen
};

// 加入logoIndex 和 logoUrl
struct PSyncSList2 : public sox::Voidmable {
	enum {uri = (6 << 8 | protocol::SLIST_SVID)};
};

struct PSyncSListRes2 : public sox::Marshallable {
	enum {uri = (7 << 8 | protocol::SLIST_SVID)};
	std::vector<SListItem2> items;
	uint32_t resCode;
	virtual void marshal(sox::Pack &p) const {
		marshal_container(p, items);
		p << resCode;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		unmarshal_container(p, std::back_inserter(items));
		p >> resCode;
	}
};

struct PResyncSList: public sox::Voidmable{
	enum {uri = (8 << 8 | protocol::SLIST_SVID)};
	uint32_t waitSyncSid;
	PResyncSList():waitSyncSid(0){}
};

struct PSyncMyGuildList : public sox::Marshallable
{
	enum {uri = (9 << 8 | protocol::SDB_SVID)};
	uint32_t uid;
	PSyncMyGuildList(){}
	PSyncMyGuildList(uint32_t id) : uid(id)
	{
	}
	virtual void marshal(sox::Pack &p) const {
		p << uid;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> uid;
	}
};

struct PMListItem : public sox::Marshallable {
	enum ItemType{
		chanel,
		folder
	};
	uint32_t sid;
	uint32_t asid;
	uint32_t pid;
	uint32_t users;
	std::string snick;
	uint32_t owner;
	ItemType type;
	bool bLimit;
	bool bPublic;
	bool isSettle;
	protocol::uinfo::SEX sex;
	uint32_t logoIndex;
	std::string logoUrl;
	protocol::session::ChannelRoler roler;

	PMListItem():users(0), type(chanel){}
	PMListItem(int s, int as, int p, const std::string &n, uint32_t o, ItemType t
		, bool l, uint32_t nLogoIndex, const std::string& url) 
		: sid(s), asid(as), pid(p), users(0), snick(n), owner(o), type(t), bLimit(l)
		, logoIndex(nLogoIndex), logoUrl(url), roler(protocol::session::NORMAL) {
	}
	virtual void marshal(sox::Pack &p) const {
		p << sid << asid << pid << users << snick << owner;
		p.push_uint8(type);
		p.push_uint8(bLimit);
		p.push_uint8(bPublic);
		p.push_uint8(isSettle);
		p.push_uint8(sex);
		p << logoIndex << logoUrl;
		p.push_uint16((uint16_t)roler);

	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> sid >> asid >> pid >> users >> snick >> owner;
		type = (ItemType)p.pop_uint8();
		bLimit = p.pop_uint8() != 0;
		bPublic = p.pop_uint8() != 0;
		isSettle = p.pop_uint8() != 0;
		sex = (protocol::uinfo::SEX)p.pop_uint8();
		p >> logoIndex >> logoUrl;
		roler = (protocol::session::ChannelRoler)p.pop_uint16();
	}
};

struct PSyncMyListRes : public sox::Marshallable {
	enum {uri = (10 << 8 | protocol::SDB_SVID)};
	uint32_t uid;
	uint32_t resCode;
	std::vector<PMListItem> items;
	virtual void marshal(sox::Pack &p) const {
		p << uid << resCode;
		marshal_container(p, items);
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> uid >> resCode;
		unmarshal_container(p, std::back_inserter(items));
	}
};

// 同步收藏夹指令
// 与PSyncSList2差别：需要返回收藏夹频道备注字段
struct PSyncSList3 : public sox::Voidmable {
	enum {uri = (14 << 8 | protocol::SLIST_SVID)};
};


// 同步收藏夹指令返回
// 与PSyncSListRes2差别：SListItem2->SListItem3
struct PSyncSListRes3 : public sox::Marshallable {
	enum {uri = (15 << 8 | protocol::SLIST_SVID)};
	std::vector<SListItem3> items;
	uint32_t resCode;
	virtual void marshal(sox::Pack &p) const {
		marshal_container(p, items);
		p << resCode;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		unmarshal_container(p, std::back_inserter(items));
		p >> resCode;
	}
};

  /**
   * 更新收藏夹频道备注.
   * @param uid sid sidremark.
   */
struct PRemarkSListSidName : public sox::Voidmable {
	enum {uri = (16 << 8 | protocol::SLIST_SVID)};
	uint32_t uid;
	uint32_t sid;
	std::string sidremark;   //备注名称
	virtual void marshal(sox::Pack &p) const {
		p << uid << sid << sidremark;
	}
	virtual void unmarshal(const sox::Unpack &p) {
		p >> uid >> sid >> sidremark;
	}
};
    /**
	 * 更新收藏夹频道备注返回.
	 * @param uid sid sidremark.
	 */
struct PRemarkSListSidNameRes : public sox::Marshallable {
  enum {uri = (17 << 8 | protocol::SLIST_SVID)};
  uint32_t resCode;   //0 -- 成功1 -- 失败
  uint32_t sid;          //频道号
  std::string sidremark; //备注名称
  virtual void marshal(sox::Pack &p) const {
	  p << resCode << sid << sidremark;
  }
  virtual void unmarshal(const sox::Unpack &p) {
	  p >> resCode >> sid >> sidremark;
  }
};
}
}


#endif /*PSLIST_H_*/
