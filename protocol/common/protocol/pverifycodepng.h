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

		//png����
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
					delete []m_pValuePng;					//���ͷ�֮ǰ���ڴ�,�ٸ�ֵ
					m_pValuePng = NULL;
					m_iValuePngSize = 0;
				}
				m_iValuePngSize = p.pop_uint32();
				if(m_iValuePngSize > 0 )
				{
					char *buf = new char[m_iValuePngSize];
					memcpy(buf, p.pop_fetch_ptr(m_iValuePngSize), m_iValuePngSize);//��֪���������Ƿ����
					m_pValuePng = buf;		//���͹����ͷ�
				}
				else
				{
					m_pValuePng = NULL;
				}
			}

			const char * getPngBin(){return m_pValuePng;}
			std::size_t  getPngBinSize(){return m_iValuePngSize;}
			
			
		private:
			//���п���;��һ��˽�л�,��׼��
			//ImPng();
			ImPng(const ImPng&);
			ImPng& operator =(const ImPng&);

			const char * m_pValuePng;					//pngͼƬָ��
			std::size_t m_iValuePngSize;				//pngͼƬ��ռ�ֽ���const char *const pValuePng;	
			
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
				//pPng������ָ���Զ��ͷ�
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
					pPng = ImPngPrt(new ImPng(NULL, 0));	//����ָ����Զ��ͷ�,�����ϲ����ڴ�й©
				}
				(*pPng).unmarshal(p);
			}
			
		};

		typedef boost::shared_ptr<VerifyCodeObject> VerifyCodeObjectPrt;
		

		struct CommonVerifyCodeOperatorReq : public sox::Marshallable
		{
			//�ݲ�����uri, �ɼ̳���ʵ��֮

			CommonVerifyCodeOperatorReq(const std::string &strKey,const std::string &strValue):
					m_strKey(strKey),m_strValue(strValue) {}

			CommonVerifyCodeOperatorReq():m_strKey(""),m_strValue("") {}

			std::string m_strKey;			//��֤���Key
			std::string m_strValue;		//�û��������֤��

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
			//�ݲ�����uri, �ɼ̳���ʵ��֮

			protocol::im::E_UI_CHANNEL_ADD_BUDDY_RES_TYPE	m_resCode;			//������
			std::string m_strKey;				//��֤���Key
			protocol::verify_code::ImPngPrt m_pPngValue;		//ͼƬ��ʽ����֤��

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
					m_pPngValue = protocol::verify_code::ImPngPrt(new protocol::verify_code::ImPng(NULL, 0));				//����ָ���Զ��ͷ�
				}
				m_pPngValue->unmarshal(p);
			}

			virtual ~CommonVerifyCodeOperatorRes(){}
		};
	}
}


#endif
