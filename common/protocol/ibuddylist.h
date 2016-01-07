#ifndef __IBUDDYLIST_H__
#define __IBUDDYLIST_H__


#include <map>
#include <vector>
#include <string>
#include <list>

#include "common/protocol/ilist.h"
#include "common/unistr.h"
#include "common/protocol/const.h"
#include "common/protocol/Im_Const.h"
#include "common/protocol/iheaderbuf.h"
#include "common/int_types.h"

namespace protocol{
	namespace im{

		enum E_BUDDYLIST_OP{BUDDYLIST_SYNC, BUDDYLIST_ADDFOLDER, BUDDYLIST_RMFOLDER, BUDDYLIST_MOVETO, BUDDYLIST_DELBUDDY
							, WAITCHECKLIST_SYNC, WAITCHECKLIST_OPWCBUDDY
							, SEARCH_BUDDY, REQADD_BUDDY, CHANGE_REMARK
		//wuji start
		,HEADPHOTOLIST_SYNC,GAMELIST_SYNC,CHANELLIST_SYNC
		,GET_BUDDYCELUE_RES};
		//wuji end

		struct IBuddyList: public protocol::im::IImListBase{//�ײ������Ҫʵ�ֵĽӿ�

			virtual ~IBuddyList(){}
			virtual const HULI& getDefaultGrp() const = 0;
			virtual int			getJiFen(const HULI &id)		const = 0;
			virtual const UniString	getSign(const HULI &id)		const = 0;
			virtual protocol::uinfo::SEX	getSex(const HULI &id)		const = 0;
			virtual const UniString	getRemark(const HULI &id)		const = 0;
			virtual	protocol::E_IMSTATUS getOnline(const HULI &id)		const = 0;
			virtual	void		addFolder(const UniString &fdNick)		= 0;
			virtual void		removeFolder(const HULI &bid)	= 0;
			virtual bool		canRemove(const HULI &id)		= 0;
			virtual bool		canRename(const HULI& id) = 0;
			virtual void		moveTo(const HULI &buddy_id, const HULI &src_gid, const HULI &dest_gid)		= 0;
			virtual void		deleteBuddy(const HULI &bid, const HULI&pid)	= 0;
			virtual void		ownStatusChanged(protocol::E_IMSTATUS stat) = 0;
			virtual void changeRemark(const HULI& id, const UniString& remark) = 0;
			virtual void RenameFolder(const HULI& id, const UniString& name) = 0;
			virtual bool		isDefaultGroup(const HULI& gid) = 0;
			virtual bool isBuddy(const HULI& hid) = 0;
			virtual UniString getNick(const HULI &hid) const = 0;
			virtual UniString get_imid(const HULI& hid) const = 0;
			//virtual IHeaderBuf *getHeaderBuf() = 0;

			//wuji start
			/*
			* �����ӿ�
			* ��ȡ��ǰ��ʲô��Ϸ�������ӿڣ�protocol ʵ��
			* @param hid: �û���UID
			*/
			virtual int GetCurrentPlayGame(const HULI& hid,UniString &strData) = 0	;//-1����û������Ϸ

			/*
			*  �����ӿ�
			*  �û����ڵ�Ƶ���� protocol ʵ��
			*  @param hid : �û���UID
			*/
			//virtual int GetCurrentChanel(const HULI& hid)=0;//-1����û����Ƶ��
			virtual void GetCurrentChannel(const HULI& hid,HULI& strChanelId,UniString& strData)=0;


			/*
			* �����ӿ�
			* ��ȡ�û�IM��ͷ��protocol ʵ��
			* @param hid : �û���UID
			*/
			virtual uint32_t GetUserLogo(const HULI& hid, UniString &CustomHeadPhotoPath)=0;//modify by myf  �����CustomHeadPhotoPath,�Զ���ͷ����


			virtual void GetShenQinResultList(std::list<HULI>& lstData)=0;

			virtual uint32_t GetFolderListCount()=0;//�Լ��ж��ٸ���

			/*
			*  ��ȡ�û���½���ͣ� PC��Mobile
			*/
			virtual EClientType GetClientType(const HULI& hid) = 0;

			//wuji end

			//3.4 �����ӿڣ����������ƶ�����
			virtual void BatchMoveToFolder(const std::vector<SMoveToFolder> &vecMoveToFolder) = 0;

			//��ȡָ������ĺ�������
			virtual uint32_t GetFolderUserCount(uint32_t uFolderId) = 0;
		};

	}
}
#endif /*__IBUDDYLIST_H__*/
