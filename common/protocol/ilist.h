#ifndef ILIST_H_
#define ILIST_H_
#include <vector>
#include <map> //added by xmp
#include <utility>
#include "common/protocol/iaction.h"
#include "common/protocol/ilist.h"
#include "common/unistr.h"
#include "common/int_types.h"


namespace protocol {

typedef UniString HULI;

struct IListWatcher{//UIʵ�ֵĽӿ�
	virtual void onRefresh() = 0;
	virtual void onChange(const HULI &id, const HULI &p) = 0;
	virtual void onMoved(const HULI &id, const HULI &from, const HULI &to) = 0;
	virtual void onAdd(const HULI &id, const HULI &p) = 0;
	virtual void onRemove(const HULI &id, const HULI &p) = 0;

};



struct IListWatchBase 
{
	virtual void watch(IListWatcher *) = 0;
	virtual void revoke(IListWatcher *) = 0;
};

struct IListWatcherEx
{
	virtual void onRefresh() = 0;
	virtual void onChange(const HULI &id, const HULI &p) = 0;
	virtual void onMoved(const HULI &id, const HULI &from, const HULI &to) = 0;
	virtual void onAdd(const HULI &id, const HULI &p) = 0;
	virtual void onRemove(const HULI &id, const HULI &p) = 0;
	virtual void onAddBatch(const std::vector<std::pair<HULI, HULI> >& ids) = 0;
	virtual void onRemoveBatch(const std::vector<std::pair<HULI, HULI> >& ids) = 0;
	virtual void onChangeBatch(const std::vector<std::pair<HULI, HULI> >& ids) = 0;

	//added by xmp
	//PPushSessData3_00֪ͨ��Ƶ����Աuid�б�
	virtual void onAddBatch_Uids(const std::map<HULI, size_t>& localUids, const std::map<HULI, std::vector<HULI> >& newUids) = 0;
	virtual void onGetAllUserInfo() = 0;
	virtual void onLoadMe() = 0;
};

struct IListWatchBaseEx {
	virtual void watch(IListWatcherEx *) = 0;
	virtual void revoke(IListWatcherEx *) = 0;
};

struct IMiscWatcher
{
	virtual void onPopupPasswordWnd(const HULI &target) = 0;
	virtual void onFailedToAccess() = 0;
};

struct IMiscWatchBaseEx {
	virtual void watch(IMiscWatcher *) = 0;
	virtual void revoke(IMiscWatcher *) = 0;
};


struct IListBase : public virtual IListWatchBase,//�ײ����ʵ�ֵĽӿ�
	 public virtual IOperatorWatchBase
{
	typedef std::vector<std::pair<UniString, UniString> > PlainTree_t;

	virtual bool isFolder(const HULI &hid) const= 0;

	virtual UniString getNick(const HULI &hid) const= 0;

	virtual void getParent(const HULI &hid, std::vector<HULI>& parents) const= 0;

	virtual UniString getAliaseId(const HULI &hid) const = 0;

	//virtual UniString getDisplayName(const HULI &hid) = 0;
	
	virtual PlainTree_t getTreeData() const= 0;

	virtual bool isExisit(const HULI& hid, const HULI& pid) = 0;
};

struct IListBaseEx : public virtual IListWatchBaseEx,
	public virtual IOperatorWatchBase{
		typedef std::vector<std::pair<UniString, UniString> > PlainTree_t;

		virtual bool isFolder(const HULI &hid) const= 0;

		virtual UniString getNick(const HULI &hid) const= 0;

		virtual void getParent(const HULI &hid, std::vector<HULI>& parents) const= 0;

		virtual UniString getAliaseId(const HULI &hid) const = 0;

		virtual PlainTree_t getTreeData() const= 0;

		virtual void getUidsBySid(const HULI& sid, std::vector<HULI>& uids) const = 0;

		virtual bool isExisit(const HULI& hid, const HULI& pid) = 0;
};

namespace im
{	
	
	//wuji start
	//IM������list���ܶ����иýӿ�
	enum E_UI_LIST_CHANGED_OP{CHANGED_BUDDY_REMARK,CHANGED_FOLDER_REMARK,CHANGED_BUDDY_INFO,CHANGED_BUDDY_STATUS,		
		CHANGED_BUDDY_HEADPHOTO,
		CHANGED_BUDDY_GAME,
		CHANGED_BUDDY_CHANEL,
		CHANGED_BUDDY_ALL};

	//�����ƶ����ѵ�ָ������
	struct SMoveToFolder
	{
		uint32_t uBid;
		uint32_t uFolderIdFrom;
		uint32_t uFolderIdTo;

		SMoveToFolder() {}
		SMoveToFolder(uint32_t _uBid, uint32_t _uFolderIdFrom, uint32_t _uFolderIdTo)
		{
			uBid = _uBid;
			uFolderIdFrom = _uFolderIdFrom;
			uFolderIdTo = _uFolderIdTo;
		}
	};

	struct IImListWatcher
	{//UIʵ�ֵĽӿ�

		//3.2 ֪ͨUI����˳��ı�
		virtual void onFolderOrderChange() {};

		virtual void onRefresh() = 0;
		virtual void onRefreshHeadPhotoList() = 0;		//add by myf		��¼�ɹ������ͷ�����ݵ���֪ͨ
		virtual void onLoadHeadBuf() = 0;					//add by myf		��ȡ��ͷ�񱾵ػ����,֪ͨ�ͻ�����ȡ����

		/* ����ͷ�����֪ͨ
		*	@param id: ��bid,���ѵ�uid
		*	@param id: ��pid,���ѵ�folderid
		*	@param strCustomHeadPhotoPath: �Զ���ͷ��·��
		*/
		virtual void onHeadPhotoChange(const HULI &id, const HULI &pid,const UniString &strCustomHeadPhotoPath) = 0;//add by myf		

		virtual void onChange(const HULI &id, const HULI &p,const E_UI_LIST_CHANGED_OP &whatischanged) = 0;
		virtual void onMoved(const HULI &id, const HULI &from, const HULI &to) = 0;
		virtual void onAdd(const HULI &id, const HULI &p) = 0;
		virtual void onRemove(const HULI &id, const HULI &p) = 0;

		//add by kdjie, 2009.11.19
		//onListChange�ı�����
		enum EListChangeType
		{
			ENUM_LIST_NULL = 0,
			ENUM_LIST_CHANNEL = 1,		//����Ƶ���б���
			ENUM_LIST_GAME =2,					//������Ϸ�б���
			ENUM_LIST_HEADPHOTO =3,		//����ͷ���б���
			ENUM_LIST_CUSTOMHEADPHOTO =4,			//�����Զ���ͷ���б���
			ENUM_LIST_IMID = 5,			//IMID���
			ENUM_LIST_IMSTATUS = 6,			//Online״̬���
			ENUM_LIST_IMREMARK = 7,			//���ѱ�ע(remark)���
			ENUM_LIST_UINFO = 8					//�û�������Ϣ
		};
		virtual void onListChange(const std::vector<HULI> &vecBid, const enum EListChangeType &emListType) = 0;

		//֪ͨ(���ε�½ǰ)�к�����ӵ����������
		virtual void onNotifyAddBuddy(const std::vector<HULI> &vecBid) = 0;		

		//֪ͨ(���ε�½ǰ)����Ҫͨ��������֤������.
		virtual void onNotifyWaitCheck(const std::vector<HULI> &vecBid) = 0;
		//add end
	};
	
	
	
	struct IImListWatchBase 
	{
		virtual void watch(IImListWatcher *) = 0;
		virtual void revoke(IImListWatcher *) = 0;
	};
	
	
	struct IImListBase : public virtual IImListWatchBase,//�ײ����ʵ�ֵĽӿ�
		public virtual IOperatorWatchBase
	{
		typedef std::vector<std::pair<UniString, UniString> > PlainTree_t;
		
		virtual bool isFolder(const HULI &hid) const= 0;
		
		virtual UniString getNick(const HULI &hid) const= 0;
		
		virtual void getParent(const HULI &hid, std::vector<HULI>& parents) const= 0;
		
		virtual UniString getAliaseId(const HULI &hid) const = 0;
		
		//virtual UniString getDisplayName(const HULI &hid) = 0;
		
		virtual void getTreeData(PlainTree_t&) const=0;
		virtual PlainTree_t getTreeData() const= 0;
		
		virtual bool isExisit(const HULI& hid, const HULI& pid) = 0;

		//3.2 ��ȡ����˳��UI���ȶ�������ķ����������
		virtual void getFolderOrder(std::vector<uint32_t> &vecFolderOrder) {};
		//3.2 ÿ����ӡ�ɾ�����ƶ��������Ҫ���ñ��ӿ����÷���˳��
		//bNotifyUI��ʾ��������Ӧ������Ƿ���Ҫ�ٴ�֪ͨUI
		virtual void setFolderOrder(std::vector<uint32_t> &vecFolderOrder, bool bNotifyUI = false) {};
	};
	//wuji end
}

/*struct IListRuler{
 virtual void setListBase(IListBase *list);
 };*/
}
#endif /*ILIST_H_*/
