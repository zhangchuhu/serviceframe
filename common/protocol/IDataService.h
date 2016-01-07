#pragma once

#include "common/int_types.h"
#include <string>


namespace protocol
{
	namespace imdatasv
	{

		//��ͼ������Ϣ
		struct ImageDataInfo 
		{
			std::string m_strURL;			//��ͼ��url
			std::string m_strFileName;		//��ͼ���ļ���
			std::string m_strData;			//��ͼ�ļ�����
			std::string m_strContextData;	//�ͻ���������
		};

		//��ͼͳ����Ϣ
		struct ImageStatInfo
		{
			uint32_t m_uBId;				//���ѵ�uid
			uint32_t m_uFldId;				//��id

			ImageStatInfo() : m_uBId(0), m_uFldId(0)
			{}
		};

		//�ϴ���ͼͳ����Ϣ
		struct UploadStatInfo : public ImageStatInfo
		{
		};

		//���ؽ�ͼͳ����Ϣ
		struct DownloadStatInfo : public ImageStatInfo
		{
			uint64_t m_uImageAge;			//ͼ��
			
			DownloadStatInfo() : m_uImageAge(0)
			{}
		};



		class IDataServiceWatcher
		{
		public:
			virtual ~IDataServiceWatcher() {}
			//uRet: 0-�ϴ��ɹ�; 1-�ϴ�ʧ��   
			virtual void OnUpload(const uint32_t &uRet, const ImageDataInfo &dataInfo) {}
			//uRet: 0-���سɹ�; 1-����ʧ��
			virtual void OnDownload(const uint32_t &uRet, const ImageDataInfo &dataInfo) {}
		};

		class IOpDataService
		{
		public:
			virtual ~IOpDataService() {}

			virtual void watch(IDataServiceWatcher *w) = 0;

			virtual void revoke(IDataServiceWatcher *w) = 0;
		};

		class IDataService : public virtual IOpDataService
		{
		public:

			virtual ~IDataService() {}

			virtual void UploadToServer(const ImageDataInfo &dataInfo, const UploadStatInfo &statInfo) = 0;

			virtual void DownloadFromServer(const ImageDataInfo &dataInfo, const DownloadStatInfo &statInfo) = 0;
		};
	}
}

