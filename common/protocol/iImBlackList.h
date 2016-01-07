#pragma once
#include <list>
#include "common/int_types.h"
#include "common/protocol/ilist.h"
#include "common/unistr.h"

#include "common/protocol/IImUInfo.h"

namespace protocol
{
	namespace im
	{

		//��������UIʵ�ֵĽӿ���ʹ��IImListWatcher��IImBlackListUInfoWatcher

		//�������ӿ�
		struct IBlackList:public virtual IImListWatchBase//�ײ�ʵ�ֵĽӿ�
		{
			//����������������
			virtual void Ansy_MoveToBlackList(const HULI& strBuddyId ) = 0;
			//�����ѴӺ�����ɾ��
			virtual void Ansy_DeleteFromBlackList(const HULI& strBuddyId) = 0;
			virtual void Ansy_GetBackList()=0;//ȡ�غ������б�

			virtual void GetBackList(std::list<uint32_t>& lstData)=0;//ȡ�غ������б�
			virtual void GetBackList(std::list<HULI>& lstData)=0;//ȡ�غ������б�
			virtual bool IsInBlackList(const HULI& strBuddyId ) = 0;//true������

			//IM 2.2�¼��빦��
			//����������������
			virtual void MoveToBlackList(uint32_t uBid ) = 0;
			//�����ѴӺ�����ɾ��
			virtual void DeleteFromBlackList(uint32_t uBid,bool bIsOnlyDel/*true�����ں������мӺ��ѣ�false����ɾ��������*/)=0;
		};
		
		struct IImBlackListUInfoWatcher
		{
			//UIʵ�ֵĽӿ�
			virtual void OnSimpleChange(const std::vector<CImUserInfoSimpleClientData>& vecData) = 0;
		};

		struct IImBlackListUInfoWatchBase 
		{
			virtual void watch(IImBlackListUInfoWatcher *) = 0;
			virtual void revoke(IImBlackListUInfoWatcher *) = 0;
		};

		//�ײ�ʵ�ֵĽӿ�
		struct IImBlackListUInfo:public virtual IImBlackListUInfoWatchBase 
		{
				virtual void GetUinfo()=0;
		};


	}

}
