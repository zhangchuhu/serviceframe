#ifndef __VerifyCodeHelper__
#define __VerifyCodeHelper__

#include <string>
#include <assert.h>
#include "boost/shared_ptr.hpp"
#include "server_common/dblib/Memcached.h"
#include "server_common/dblib/IMemcachedFactory.h"
#include "protocol/common/protocol/pverifycodepng.h"
#include <libmemcached/memcached.h>


namespace server
{
	namespace verify_code
	{
		static const std::string VERIFY_CODE_MEMCACHED = "verify_code";
		static const time_t EXPIRATION_TIME = 1200;			//MEMCACHE ��ʱʱ��(20����)
		static const std::string KEY_PREFIXAL = "VCKEY";
		static const std::string VALUE_CHAR = "34578acdefghjkmnprstuvwxv";  //value�ı�ѡ��
		static const std::string KEY_CHAR = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";  //key�ı�ѡ��
		static const std::string DEFAULT_FONT_LIB_PATH = "../bin/font_lib/"	;
		class VerifyCodeHelper :public mc::MemcachedFactoryAware
		{
		public:

			
			VerifyCodeHelper(const std::size_t width = 120, const std::size_t height = 40, const std::size_t charCount = 4, const std::string &strFontLibPath = DEFAULT_FONT_LIB_PATH);

			virtual ~VerifyCodeHelper();

		

			//ȡ��֤��ͼƬ����Ӧ��Key, ע��:Ϊ��ȫ���,���ص�ValueΪ��
			void getVerifyCodeAndKey(protocol::verify_code::VerifyCodeObject & pvcObj);	

			//����û��������֤���Ƿ���ȷ
			//strKey: ��֤���Ӧ��Key
			//strUserValue: �û��������֤�� 
			bool checkVerifyCode(const std::string &strKey, const std::string &strUserValue);

			//static VerifyCodeHelper* getInst();
		private:
			std::size_t m_iWidth;				//��֤��ͼƬ����
			std::size_t m_iHeight;				//��֤��ͼƬ���
			std::size_t m_iCharCount;			//��֤��ͼƬ�ַ�����
			std::string m_strFontLibPath;			//�ֿ�·��

			//���������
			static std::size_t myRand(const std::size_t min = 0, const std::size_t max = 100);

			//����ΨһKey(64λ)
			std::string getUniqueKey();

			//�������Value
			std::string getRandValue();

			//����ͼƬ
			protocol::verify_code::ImPngPrt getImage(const std::string& strValue);

			//�����ȡһ�������ļ���
			std::string getRandTTFFileName();
				
			
				
		};
	}
}


#endif   //end of __VerifyCodeHelper__

