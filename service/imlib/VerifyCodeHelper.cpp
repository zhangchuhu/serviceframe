#include "VerifyCodeHelper.h"
#include <stdlib.h>
#include <time.h>
#include "boost/thread/tss.hpp"
#include "boost/thread/mutex.hpp"
#include <sys/types.h>
#include <dirent.h>
#include <gd.h>
#include <sstream>
#include <vector>
#include <string>
#include <pthread.h> 
#include "core/sox/logger.h"

using namespace std;
using namespace server::verify_code;

VerifyCodeHelper::VerifyCodeHelper(
	const std::size_t width, const std::size_t height, const std::size_t charCount, const std::string &strFontLibPath)
	: m_iWidth(width), m_iHeight(height), m_iCharCount(charCount), m_strFontLibPath(strFontLibPath)
{
	if(m_strFontLibPath[m_strFontLibPath.size() - 1] != '/')
	{
		m_strFontLibPath+= '/';
	}
}

VerifyCodeHelper::~VerifyCodeHelper()
{
}

/*
VerifyCodeHelper* VerifyCodeHelper::getInst()
{
	static server::verify_code::VerifyCodeHelper vchelper;
	
    return &vchelper;
}*/

//ȡջ�ڴ��е������ַ
std::size_t getStackAddrRnd()
{
	int t = 0;
	t = 1;
	return (std::size_t)(&t) ;
}

//���������
std::size_t VerifyCodeHelper::myRand(const std::size_t min, const std::size_t max )
{
	static bool bSetSeek = false;
	static std::size_t iTimes = 0;
	static unsigned int seed = 0;

	static boost::mutex Lock;
	boost::mutex::scoped_lock sl(Lock);
	
	if(!bSetSeek || (iTimes > 1000000))
	{
		iTimes = 0;
		seed = time(NULL);
		bSetSeek = true;
	}
	++iTimes;

	std::size_t rand0 =  rand_r(&seed);			
	std::size_t rand1 = getStackAddrRnd();
	std::size_t rand2 = getpid();
	std::size_t rand3 =	pthread_self();
	std::size_t rand4 = iTimes * iTimes / 7;
	std::size_t result = (rand0 /3) + (rand1 /4) + rand2 +rand3+ rand4;
	return ( (result % (max - min + 1)) + min);
}

//����ΨһKey(64λ)
std::string VerifyCodeHelper::getUniqueKey()
{
	static const size_t MIN_COUNT =  100000000;
	static const size_t MAX_COUNT =  1000000000;

	static size_t iCounter = MIN_COUNT; 	//��֤iCounterռ9λ
	
	++iCounter;
	if(iCounter >= MAX_COUNT)
	{
		iCounter = MIN_COUNT; 
	}
	
	//ȡʱ��t,��֤t������9λ
	time_t t = time(NULL);
	while(t >= static_cast<time_t>(MAX_COUNT))
	{
		t  -= MAX_COUNT;
	}
	if(t < static_cast<time_t>(MIN_COUNT))
	{
		t += MIN_COUNT;
	}



	std::stringstream ss;
	ss << KEY_PREFIXAL << iCounter << t;

	//41λ�����
	for(size_t i = 0; i < 46 - KEY_PREFIXAL.size(); ++i)
	{
		size_t iPos = myRand(0, KEY_CHAR.size() - 1);
		ss << KEY_CHAR[iPos];
	}	

	return ss.str();
}

//�������Value
std::string VerifyCodeHelper::getRandValue()
{
	std::string result = "";
	for(std::size_t i = 0; i < m_iCharCount; ++i)
	{
		size_t iPos = myRand(0, VALUE_CHAR.size() - 1);
		const char tchar = VALUE_CHAR[iPos];
		if(!isalpha(tchar))
		{
			result += tchar;
		}
		else
		{
			//�����һ��,�Ƿ�ת��Ϊ��д��ĸ
			result += ( (myRand(0, 1) == 1) ? tolower(tchar) : toupper(tchar) );
		}
			
	}
	return result;
}


//�����ȡһ�������ļ���
std::string VerifyCodeHelper::getRandTTFFileName()
{
	static std::vector<std::string> vtTTFSet;
	static boost::mutex TTFSetLock;
	static std::size_t iTimes = 0;
	boost::mutex::scoped_lock sl(TTFSetLock);
	++iTimes;
	if(vtTTFSet.empty() || iTimes > 10000)
	{
		vtTTFSet.clear();
		iTimes = 0;
		//��ȡ����ttf�ļ�
		DIR* pdir = opendir(m_strFontLibPath.c_str());
		if(pdir == NULL)
		{
			return "";
		}

		struct dirent * dent = NULL;
		while((dent = readdir(pdir)) != NULL)
		{
			if(dent->d_type == DT_REG)
			{
				std::string strFileName =  dent->d_name;
				std::string strExt = (strFileName.size() <=4)? strFileName : strFileName.substr(strFileName.size() - 4, 4);
				if(strcasecmp(strExt.c_str(), ".ttf") ==0)
				{
					vtTTFSet.push_back(m_strFontLibPath + strFileName);
				}				
			}
		}
		closedir(pdir);
		
	}

	if(vtTTFSet.empty())
	{
		return "";
	}

	std::size_t iPos = myRand(0, vtTTFSet.size() - 1);
	
	return vtTTFSet[iPos];
	
	
}

//���ݸ�����Value�ַ�������ͼƬ
protocol::verify_code::ImPngPrt VerifyCodeHelper::getImage(const std::string& strValue)
{
	//��ʼ��ͼ��
	static const std::size_t COLOR_COUNT = 5;
	gdImagePtr im = gdImageCreate(m_iWidth, m_iHeight);
	int im_white, im_black;
	im_white = gdImageColorAllocate(im,255,255,255);	
	im_black = gdImageColorAllocate(im,0,0,0);
	int imcolor[COLOR_COUNT];
	imcolor[0] = gdImageColorAllocate(im,102,64,149);	//�Ϻ�
	imcolor[1] = im_black;								//��
	imcolor[2] = gdImageColorAllocate(im,255,0,0);		//��
	imcolor[3] = gdImageColorAllocate(im,0,191,0);		//��
	imcolor[4] = gdImageColorAllocate(im,0,0,255);		//��

	gdImageFill(im, 0,0, im_white);					//��䱳��Ϊ��ɫ

	//��Ӹ���
	for(int i = 0; i < 5; ++i)
	{
		int x1 = (int)myRand(0, m_iWidth - 5);
		int y1 = (int)myRand(0, m_iHeight - 3);
		gdImageLine(im, x1, y1, (x1 +  (int)myRand(3, 20)), (y1 + (int)myRand(2, 10)), imcolor[myRand(0, COLOR_COUNT - 1)]);	
	}
	for(int i = 0; i < 5; ++i)
	{
		int x1 = (int)myRand(0, m_iWidth- 5);
		int y1 = (int)myRand(0, m_iHeight);
		gdImageLine(im, x1, y1, (x1 + (int)myRand(3, 20)), (y1 - (int)myRand(2, 10)), imcolor[myRand(0, COLOR_COUNT - 1)]);	
	}
	for(int i = 0; i < 5; ++i)
	{
		int x1 = (int)myRand(0, m_iWidth - 5);
		int y1 = (int)myRand(0, m_iHeight);
		gdImageLine(im, x1, y1, (x1 +  (int)myRand(3, 20)), y1, imcolor[myRand(0, COLOR_COUNT - 1)]);	
	}
	for(int i = 0; i < 5; ++i)
	{
		int x1 = (int)myRand(0, m_iWidth);
		int y1 = (int)myRand(0, m_iHeight - 3);
		gdImageLine(im, x1, y1, (x1), (y1 + (int)myRand(2, 10)), imcolor[myRand(0, COLOR_COUNT - 1)]);	
	}

	
	//������ֵ�ͼƬ��
	int iStep = static_cast<int> (m_iWidth * 0.06);
	int brect[8];
	char strNum[2];
	strNum[1] = 0;
	for(std::size_t iCounter = 0; iCounter < m_iCharCount; ++iCounter, iStep+= static_cast<int> (m_iWidth * 0.9 /m_iCharCount))
	{
		strNum[0] = strValue[iCounter];
		char *sNum = strNum;
		
		std::string TTFFileName = getRandTTFFileName();//ȡ�������
		char strTTFPath[2048];
		strcpy(strTTFPath, TTFFileName.c_str());		

		gdImageStringFT(
			im, 
			&brect[0],
			imcolor[myRand(0, COLOR_COUNT - 1)],
			strTTFPath,
			static_cast<double>(m_iHeight * 0.6),
			0.0,//static_cast<double>(myRand(0, 30)) - 15.0,
			iStep,
			static_cast<int>(m_iHeight * 0.75),
			sNum);
	}
	

	//���pngͼƬ���ݵ��������ڴ���
	int size = 0;
	void *buf = (char *)gdImagePngPtr(im, &size);
	char *buf2 = new char [size];	//����pObj�ͷ�
	memcpy(buf2, buf, size);
	protocol::verify_code::ImPngPrt pObj(new protocol::verify_code::ImPng(buf2, static_cast<std::size_t>(size)));

	gdFree(buf);
 	gdImageDestroy(im);	
	return pObj;
}

//ȡ��֤��ͼƬ����Ӧ��Key, ע��:Ϊ��ȫ���,���ص�ValueΪ��
void VerifyCodeHelper::getVerifyCodeAndKey(protocol::verify_code::VerifyCodeObject & pvcObj)
{
	pvcObj.strValue = getRandValue();
	pvcObj.pPng = getImage(pvcObj.strValue);
	pvcObj.strKey = getUniqueKey();
	
	log(Info, "generate new verifycode.  key=%s  value=%s", pvcObj.strKey.c_str(), pvcObj.strValue.c_str());
	
	//���뵽memcache��
	server::mc::Memcached* mc = getMemcachedFactory()->getMemcached(VERIFY_CODE_MEMCACHED);
	mc->mcSet(pvcObj.strKey, pvcObj.strValue,EXPIRATION_TIME );

	return ;
}

//����û��������֤���Ƿ���ȷ
//strKey: ��֤���Ӧ��Key
//strUserValue: �û��������֤�� 
bool VerifyCodeHelper::checkVerifyCode(const std::string &strKey, const std::string &strUserValue)
{
	//ȡmemcache
	server::mc::Memcached* mc = getMemcachedFactory()->getMemcached(VERIFY_CODE_MEMCACHED);

	std::string strValue = "";
	static boost::mutex Lock;
	{
		//boost::mutex::scoped_lock sl(Lock);
		//�Ƚ�����ֵ��Ԥ��ֵ
		mc->mcGet(strKey, strValue);
		mc->mcDel(strKey);
	}
	log(Info, "[VerifyCodeHelper::checkVerifyCode]: strKey:%s     value:%s     user_value:%s", strKey.c_str(), strValue.c_str(), strUserValue.c_str());
	return (strcasecmp(strUserValue.c_str() ,strValue.c_str()) == 0);
}


