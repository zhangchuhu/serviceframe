#ifndef  __IHeaderBuf_h__
#define __IHeaderBuf_h__

#include "common/protocol/iImV2.h"

namespace protocol
{
	namespace im
	{
		//ͷ��logo����
		struct HeaderData
		{
			HULI id;												//�û�ID
			UniString CustomHeadPhotoPath;		//ͷ��·��	
			const char *buf;												//ͼ��bufͷ��ַ
			uint32_t size;										//buf��size
			uint32_t nIndex;									//ϵͳͷ���index

			HeaderData():buf(NULL), size(0), nIndex(IImChanelSet::DEFAULT_LOGO_NUMBER)
			{
			}
		};
		typedef std::map<HULI, HeaderData> MapHeaderData;


		class IHeaderBuf
		{
		public:

			//�������ͷ����Ϣ��������
			virtual void putCustomHeaderPhoto(const HULI &id, const UniString &CustomHeadPhotoPath, char *buf, const uint32_t size) = 0;

			virtual void deleteHeader(const HULI &id) = 0;

			//�ѻ�����ĸ���ͷ����Ϣ���浽�����ļ�
			virtual void saveHeader() = 0;

			//��ȡ�Զ���ͷ������
			//����ȡ�õ�ͼƬ����,�������ǵ�deleteHeader����ProtocolManagerImp::releaseProtocol()ʱ�Ż��ͷ�,�������,����յ����Լ�copyһ��ʹ��
			virtual void getCustomHeaderPhoto(MapHeaderData & dataset) = 0;
			
			//��ȡ�Զ���ͷ������
			//����ȡ�õ�ͼƬ����,�������ǵ�deleteHeader����ProtocolManagerImp::releaseProtocol()ʱ�Ż��ͷ�,�������,����յ����Լ�copyһ��ʹ��
			virtual void getCustomHeaderPhoto(const UniString &id, HeaderData & data) = 0;
		};

	}


}

#endif	//__IHeaderBuf_h__

