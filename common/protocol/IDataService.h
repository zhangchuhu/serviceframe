#pragma once

#include "common/int_types.h"
#include <string>


namespace protocol
{
	namespace imdatasv
	{

		//截图数据信息
		struct ImageDataInfo 
		{
			std::string m_strURL;			//截图的url
			std::string m_strFileName;		//截图的文件名
			std::string m_strData;			//截图文件数据
			std::string m_strContextData;	//客户端上下文
		};

		//截图统计信息
		struct ImageStatInfo
		{
			uint32_t m_uBId;				//好友的uid
			uint32_t m_uFldId;				//组id

			ImageStatInfo() : m_uBId(0), m_uFldId(0)
			{}
		};

		//上传截图统计信息
		struct UploadStatInfo : public ImageStatInfo
		{
		};

		//下载截图统计信息
		struct DownloadStatInfo : public ImageStatInfo
		{
			uint64_t m_uImageAge;			//图龄
			
			DownloadStatInfo() : m_uImageAge(0)
			{}
		};



		class IDataServiceWatcher
		{
		public:
			virtual ~IDataServiceWatcher() {}
			//uRet: 0-上传成功; 1-上传失败   
			virtual void OnUpload(const uint32_t &uRet, const ImageDataInfo &dataInfo) {}
			//uRet: 0-下载成功; 1-下载失败
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

