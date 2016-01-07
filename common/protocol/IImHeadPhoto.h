#pragma once

#include <vector>
#include "common/unistr.h"
//#include "common/packet.h"


namespace protocol
{
	namespace im
	{
		struct CImHeadPhotoInfoClientData 
		{
		public:
			CImHeadPhotoInfoClientData()
			{
				m_uId=-1;
				m_uHeadPhoto=-1;
			}

			CImHeadPhotoInfoClientData(uint32_t uId,uint32_t uHeadPhoto,UniString &strCustomHeadPhotoPath)
			{
				m_uId=uId;
				m_uHeadPhoto=uHeadPhoto;
				m_strCustomHeadPhotoPath=strCustomHeadPhotoPath;
			}

			CImHeadPhotoInfoClientData& operator = (const CImHeadPhotoInfoClientData& obj)
			{
				if (this == &obj)
				{
					return *this;
				}
				m_uId = obj.m_uId;
				m_uHeadPhoto = obj.m_uHeadPhoto;
				m_strCustomHeadPhotoPath = obj.m_strCustomHeadPhotoPath;
				return *this;
			}

			uint32_t m_uId;
			uint32_t m_uHeadPhoto;//ͷ��
			UniString m_strCustomHeadPhotoPath;//�Զ���ͷ��·��
		};



		

		struct IImHeadPhotoInfoWatcher
		{
			//UIʵ�ֵĽӿ�
			virtual void OnChange(const std::vector<CImHeadPhotoInfoClientData*>& vecData) = 0;
		};

		struct IImHeadPhotoInfoWatchBase 
		{
			virtual void watch(IImHeadPhotoInfoWatcher *) = 0;
			virtual void revoke(IImHeadPhotoInfoWatcher *) = 0;
		};

		//�ײ�ʵ�ֵĽӿ�
		struct IImHeadPhotoInfo:public virtual IImHeadPhotoInfoWatchBase
		{
			virtual void GetImHeadPhotoInfo(const std::vector<uint32_t/*���鿴�˵�ID�б�*/> &vecUid)=0;
			//ȡȺ�û��б�ͷ��ֻ����ϵͳͷ���Զ���ͷ����ΪϵͳĬ�ϣ�
			virtual void GetImHeadPhotoInfo2(const std::vector<uint32_t/*���鿴�˵�ID�б�*/> &vecUid)=0;
		};


	}
}

