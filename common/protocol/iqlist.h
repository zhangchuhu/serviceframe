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
		//֪ͨadd�Ƿ�ɹ�������ֵ��QItem��resCode����
		virtual void onAdd(const QItem &item) = 0;
		//֪ͨremove�Ƿ�ɹ�������ֵ��QItem��resCode����
		virtual void onRemove(const QItem &item) = 0;
	};

	struct ISessionQListWatchBase{
		virtual void watch(ISessionQListWatcher *) = 0;
		virtual void revoke(ISessionQListWatcher *) = 0;
	};

	struct ISessionQList : public ISessionQListWatchBase {
		//������е�QItem����������(string)��С�������򣬷���false��ʱ���ʾû�дӷ������õ��б�getQListʧ��
		virtual bool getQList(std::list<QItem> &items) = 0;
		//֪ͨ���������һ��QItem
		virtual void addQList(const UniString &sid, const UniString &tid) = 0;
		//֪ͨ������ɾ��һ��QItem
		virtual void removeQList(const UniString &sid, const UniString &tid) = 0;
		//ѯ��(sid, tid)��Ƶ���Ƿ��ڱ����м�¼��true��ʾ�Ѿ��ڱ����м�¼�ˣ�false��ʾû���
		virtual bool isQItemExist(const UniString &sid, const UniString &tid) = 0;
	};


}
}
#endif /* IQLIST_H_ */