#ifndef  __IHeaderBuf_h__
#define __IHeaderBuf_h__

#include "common/protocol/iImV2.h"

namespace protocol
{
	namespace im
	{
		//头像logo数据
		struct HeaderData
		{
			HULI id;												//用户ID
			UniString CustomHeadPhotoPath;		//头像路径	
			const char *buf;												//图像buf头地址
			uint32_t size;										//buf的size
			uint32_t nIndex;									//系统头像的index

			HeaderData():buf(NULL), size(0), nIndex(IImChanelSet::DEFAULT_LOGO_NUMBER)
			{
			}
		};
		typedef std::map<HULI, HeaderData> MapHeaderData;


		class IHeaderBuf
		{
		public:

			//保存个人头像信息到缓存里
			virtual void putCustomHeaderPhoto(const HULI &id, const UniString &CustomHeadPhotoPath, char *buf, const uint32_t size) = 0;

			virtual void deleteHeader(const HULI &id) = 0;

			//把缓存里的个人头像信息保存到本地文件
			virtual void saveHeader() = 0;

			//获取自定义头像数据
			//这里取得的图片数据,理论上是到deleteHeader或者ProtocolManagerImp::releaseProtocol()时才会释放,保险起见,最好收到后自己copy一份使用
			virtual void getCustomHeaderPhoto(MapHeaderData & dataset) = 0;
			
			//获取自定义头像数据
			//这里取得的图片数据,理论上是到deleteHeader或者ProtocolManagerImp::releaseProtocol()时才会释放,保险起见,最好收到后自己copy一份使用
			virtual void getCustomHeaderPhoto(const UniString &id, HeaderData & data) = 0;
		};

	}


}

#endif	//__IHeaderBuf_h__

