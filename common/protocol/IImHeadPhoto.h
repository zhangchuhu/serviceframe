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
			uint32_t m_uHeadPhoto;//头像
			UniString m_strCustomHeadPhotoPath;//自定义头像路径
		};



		

		struct IImHeadPhotoInfoWatcher
		{
			//UI实现的接口
			virtual void OnChange(const std::vector<CImHeadPhotoInfoClientData*>& vecData) = 0;
		};

		struct IImHeadPhotoInfoWatchBase 
		{
			virtual void watch(IImHeadPhotoInfoWatcher *) = 0;
			virtual void revoke(IImHeadPhotoInfoWatcher *) = 0;
		};

		//底层实现的接口
		struct IImHeadPhotoInfo:public virtual IImHeadPhotoInfoWatchBase
		{
			virtual void GetImHeadPhotoInfo(const std::vector<uint32_t/*被查看人的ID列表*/> &vecUid)=0;
			//取群用户列表头像（只包含系统头像，自定义头像设为系统默认）
			virtual void GetImHeadPhotoInfo2(const std::vector<uint32_t/*被查看人的ID列表*/> &vecUid)=0;
		};


	}
}

