#ifndef IQLIST_H_
#define IQLIST_H_

#include "common/unistr.h"
#include <list>

namespace protocol{
namespace qlist{

	struct QItem{
		UniString asid;
		UniString sid;
		UniString tid;
		UniString name;
		uint32_t resCode;
		bool operator<(const QItem &right)
		{
			return (name < right.name);
		}
		bool operator==(const QItem &right)
		{
			return ((wcstoul(sid.data(), NULL, 10) == wcstoul(right.sid.data(), NULL, 10)) &&
					(wcstoul(tid.data(), NULL, 10) == wcstoul(right.tid.data(), NULL, 10)));
		}
	};

	struct ISessionQListWatcher{
		//通知add是否成功，返回值在QItem的resCode里面
		virtual void onAdd(const QItem &item) = 0;
		//通知remove是否成功，返回值在QItem的resCode里面
		virtual void onRemove(const QItem &item) = 0;
	};

	struct ISessionQListWatchBase{
		virtual void watch(ISessionQListWatcher *) = 0;
		virtual void revoke(ISessionQListWatcher *) = 0;
	};

	struct ISessionQList : public ISessionQListWatchBase {
		//获得所有的QItem，并以名字(string)从小到大排序，返回false的时候表示没有从服务器得到列表，getQList失败
		virtual bool getQList(std::list<QItem> &items) = 0;
		//通知服务器添加一个QItem
		virtual void addQList(const UniString &sid, const UniString &tid) = 0;
		//通知服务器删除一个QItem
		virtual void removeQList(const UniString &sid, const UniString &tid) = 0;
		//询问(sid, tid)的频道是否在本地有记录，true表示已经在本地有记录了，false表示没添加
		virtual bool isQItemExist(const UniString &sid, const UniString &tid) = 0;
	};


}
}
#endif /* IQLIST_H_ */