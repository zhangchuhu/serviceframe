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
		static const time_t EXPIRATION_TIME = 1200;			//MEMCACHE 超时时间(20分钟)
		static const std::string KEY_PREFIXAL = "VCKEY";
		static const std::string VALUE_CHAR = "34578acdefghjkmnprstuvwxv";  //value的备选项
		static const std::string KEY_CHAR = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";  //key的备选项
		static const std::string DEFAULT_FONT_LIB_PATH = "../bin/font_lib/"	;
		class VerifyCodeHelper :public mc::MemcachedFactoryAware
		{
		public:

			
			VerifyCodeHelper(const std::size_t width = 120, const std::size_t height = 40, const std::size_t charCount = 4, const std::string &strFontLibPath = DEFAULT_FONT_LIB_PATH);

			virtual ~VerifyCodeHelper();

		

			//取验证码图片及对应的Key, 注意:为安全起见,返回的Value为空
			void getVerifyCodeAndKey(protocol::verify_code::VerifyCodeObject & pvcObj);	

			//检查用户输入的验证码是否正确
			//strKey: 验证码对应的Key
			//strUserValue: 用户输入的验证码 
			bool checkVerifyCode(const std::string &strKey, const std::string &strUserValue);

			//static VerifyCodeHelper* getInst();
		private:
			std::size_t m_iWidth;				//验证码图片长度
			std::size_t m_iHeight;				//验证码图片宽度
			std::size_t m_iCharCount;			//验证码图片字符数量
			std::string m_strFontLibPath;			//字库路径

			//生成随机数
			static std::size_t myRand(const std::size_t min = 0, const std::size_t max = 100);

			//生成唯一Key(64位)
			std::string getUniqueKey();

			//生成随机Value
			std::string getRandValue();

			//生成图片
			protocol::verify_code::ImPngPrt getImage(const std::string& strValue);

			//随机获取一个字体文件名
			std::string getRandTTFFileName();
				
			
				
		};
	}
}


#endif   //end of __VerifyCodeHelper__

