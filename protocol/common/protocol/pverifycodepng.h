#ifndef __pverifycodepng_h__
#define __pverifycodepng_h__


#include "core/base_svid.h"
#include "packet.h"
#include <string>
#include "boost/shared_ptr.hpp"
#include <stdlib.h>
#include "common/protocol/iImV2.h"
#include "core/sox/logger.h"

#define __DEBUG__

namespace protocol{
	namespace verify_code{

		//png对象
		struct ImPng : public sox::Marshallable
		{
		public:
			enum { uri = (1 << 8) | VERIFYCODE_SVID};

			ImPng(const char * pValuePng = NULL, std::size_t iValuePngSize = 0):m_pValuePng(pValuePng),m_iValuePngSize(iValuePngSize)
			{
				//assert(valuePng);
				//this->pValuePng = valuePng;
				//this->iValuePngSize = ValuePngSize;
				#ifdef __DEBUG__
					static int counter = 0;
					++counter;
					log(Debug, "ImPng counter: %d\n", counter);
				#endif
			}

			virtual  ~ImPng()
			{
				
				if(m_pValuePng != NULL)
				{
					delete [] m_pValuePng;
				}
				#ifdef __DEBUG__
					static int counter = 0;
					++counter;
					log(Debug, "~ImPng counter: %d\n", counter);
				#endif
			}

			virtual void marshal(sox::Pack &p) const 
			{
				p.push_uint32(m_iValuePngSize);
				if(m_iValuePngSize != 0)
				{
					p.push(m_pValuePng, m_iValuePngSize);
				}
			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
				if(m_pValuePng != NULL)
				{
					delete []m_pValuePng;					//先释放之前的内存,再赋值
					m_pValuePng = NULL;
					m_iValuePngSize = 0;
				}
				m_iValuePngSize = p.pop_uint32();
				if(m_iValuePngSize > 0 )
				{
					char *buf = new char[m_iValuePngSize];
					memcpy(buf, p.pop_fetch_ptr(m_iValuePngSize), m_iValuePngSize);//不知道这样用是否可以
					m_pValuePng = buf;		//由释构函释放
				}
				else
				{
					m_pValuePng = NULL;
				}
			}

			const char * getPngBin(){return m_pValuePng;}
			std::size_t  getPngBinSize(){return m_iValuePngSize;}
			
			
		private:
			//所有拷贝途径一律私有化,不准用
			//ImPng();
			ImPng(const ImPng&);
			ImPng& operator =(const ImPng&);

			const char * m_pValuePng;					//png图片指针
			std::size_t m_iValuePngSize;				//png图片所占字节数const char *const pValuePng;	
			
		};

		typedef boost::shared_ptr<ImPng> ImPngPrt;
		
		struct VerifyCodeObject : public sox::Marshallable
		{
			enum { uri = (2 << 8) | VERIFYCODE_SVID};
			ImPngPrt 	pPng;
			std::string strKey;
			std::string strValue;

			VerifyCodeObject(ImPngPrt ptmpPng) : pPng(ptmpPng)
			{
				//assert(valuePng);
				//this->pValuePng = valuePng;
				//this->iValuePngSize = ValuePngSize;				
			}
			
			VerifyCodeObject() : pPng()
			{
					//assert(valuePng);
					//this->pValuePng = valuePng;
					//this->iValuePngSize = ValuePngSize;
					//                 
			}
			virtual ~VerifyCodeObject()
			{
				//pPng由智能指针自动释放
			}

			virtual void marshal(sox::Pack &p) const 
			{
				p << strKey << strValue;
				if(pPng == NULL)
				{
					ImPng(NULL, 0).marshal(p);
				}
				else
				{
					(*pPng).marshal(p);
				}
			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> strKey >> strValue;
				if(pPng == NULL)
				{
					pPng = ImPngPrt(new ImPng(NULL, 0));	//智能指针会自动释放,理论上不会内存泄漏
				}
				(*pPng).unmarshal(p);
			}
			
		};

		typedef boost::shared_ptr<VerifyCodeObject> VerifyCodeObjectPrt;
		

		struct CommonVerifyCodeOperatorReq : public sox::Marshallable
		{
			//暂不定义uri, 由继承者实现之

			CommonVerifyCodeOperatorReq(const std::string &strKey,const std::string &strValue):
					m_strKey(strKey),m_strValue(strValue) {}

			CommonVerifyCodeOperatorReq():m_strKey(""),m_strValue("") {}

			std::string m_strKey;			//验证码的Key
			std::string m_strValue;		//用户输入的验证码

			virtual void marshal(sox::Pack &p) const {
				p << m_strKey << m_strValue;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> m_strKey >> m_strValue;
			}

			virtual ~CommonVerifyCodeOperatorReq(){}
		};

		struct CommonVerifyCodeOperatorRes : public sox::Marshallable
		{
			//暂不定义uri, 由继承者实现之

			protocol::im::E_UI_CHANNEL_ADD_BUDDY_RES_TYPE	m_resCode;			//返回码
			std::string m_strKey;				//验证码的Key
			protocol::verify_code::ImPngPrt m_pPngValue;		//图片格式的验证码

			virtual void marshal(sox::Pack &p) const 
			{
				p.push_uint32(m_resCode);
				p << m_strKey ;
				if(m_pPngValue != NULL)
				{
					m_pPngValue->marshal(p);
				}
				else
				{
					protocol::verify_code::ImPng(NULL, 0).marshal(p);
				}
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				m_resCode = (protocol::im::E_UI_CHANNEL_ADD_BUDDY_RES_TYPE)p.pop_uint32();
				p >> m_strKey;
				if(m_pPngValue == NULL)
				{
					m_pPngValue = protocol::verify_code::ImPngPrt(new protocol::verify_code::ImPng(NULL, 0));				//智能指针自动释放
				}
				m_pPngValue->unmarshal(p);
			}

			virtual ~CommonVerifyCodeOperatorRes(){}
		};
	}
}


#endif
