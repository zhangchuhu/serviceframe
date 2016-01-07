#ifndef __pgverifypng_h__
#define __pgverifypng_h__


#include "core/base_svid.h"
#include "packet.h"
#include <string>
#include "boost/shared_ptr.hpp"
#include <stdlib.h>
#include "common/protocol/iImV2.h"
#include "core/sox/logger.h"

#define __DEBUG__

namespace protocol{
	namespace gverifycode{

		//png����
		struct ImPng : public sox::Marshallable
		{
		public:
			enum { uri = (1 << 8) | GVERIFYCODE_SVID};

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

		struct CommonGVerifyCodeOperatorReq : public sox::Marshallable
		{
			//�ݲ�����uri, �ɼ̳���ʵ��֮
			uint64_t uKey;			//��֤���Key
			uint32_t timestamp;     //����������ʱ���
			uint64_t factor;        //����
			std::string m_strValue;		//�û��������֤��

			virtual void marshal(sox::Pack &p) const {
				p << uKey << timestamp << factor << m_strValue;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> uKey >> timestamp >> factor >> m_strValue;
			}

			virtual ~CommonGVerifyCodeOperatorReq(){}
		};

		struct CommonGVerifyCodeOperatorRes : public sox::Marshallable
		{
			//�ݲ�����uri, �ɼ̳���ʵ��֮
			uint32_t m_resCode;			//������
			uint64_t uKey;				//��֤���Key
			uint32_t timestamp;         //���������ص�ʱ���
			protocol::gverifycode::ImPngPrt m_pPngValue;		//ͼƬ��ʽ����֤��

			virtual void marshal(sox::Pack &p) const 
			{
				p.push_uint32(m_resCode);
				p << uKey << timestamp ;
				if(m_pPngValue != NULL)
				{
					m_pPngValue->marshal(p);
				}
				else
				{
					protocol::gverifycode::ImPng(NULL, 0).marshal(p);
				}
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				m_resCode = (protocol::im::E_UI_CHANNEL_ADD_BUDDY_RES_TYPE)p.pop_uint32();
				p >> uKey >> timestamp;
				if(m_pPngValue == NULL)
				{
					m_pPngValue = protocol::gverifycode::ImPngPrt(new protocol::gverifycode::ImPng(NULL, 0));				//����ָ���Զ��ͷ�
				}
				m_pPngValue->unmarshal(p);
			}

			virtual ~CommonGVerifyCodeOperatorRes(){}
		};
		
		struct PCS_GVerifyCode :
			public protocol::gverifycode::CommonGVerifyCodeOperatorReq
		{
			enum {uri = (3 << 8 | protocol::GVERIFYCODE_SVID)};

			uint32_t m_uReqUID; // will be filled
			uint64_t m_uFactor;

			virtual void marshal(sox::Pack &p) const
			{
				CommonGVerifyCodeOperatorReq::marshal(p);
				p << m_uReqUID << m_uFactor;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CommonGVerifyCodeOperatorReq::unmarshal(p);
				p >> m_uReqUID >> m_uFactor;
			}
		};

		struct PCS_GVerifyCodeRes : 
			public protocol::gverifycode::CommonGVerifyCodeOperatorRes

		{
			enum {uri = (4 << 8 | protocol::GVERIFYCODE_SVID)};

			uint64_t uOldKey;
			uint64_t uFactor;
			PCS_GVerifyCodeRes() {}
			virtual void marshal(sox::Pack &p) const
			{
				CommonGVerifyCodeOperatorRes::marshal(p);
				p<<uOldKey<<uFactor;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CommonGVerifyCodeOperatorRes::unmarshal(p); 
				p>>uOldKey>>uFactor;
			}
		};

		struct PCS_JoinGroupWithVerifyCode :
			public protocol::gverifycode::CommonGVerifyCodeOperatorReq
		{
			enum {uri = (5 << 8 | protocol::GVERIFYCODE_SVID)};

			//Authentication message could be empty if it is not required
			// by the group
			uint32_t m_uGrpID;
			std::string m_strAuthMsg;

			uint32_t m_uReqUID; // will be filled

			virtual void marshal(sox::Pack &p) const
			{
				CommonGVerifyCodeOperatorReq::marshal(p);
				p <<m_uGrpID << m_strAuthMsg << m_uReqUID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CommonGVerifyCodeOperatorReq::unmarshal(p);
				p >>m_uGrpID >> m_strAuthMsg >> m_uReqUID;
			}
		};

		// User joined
		struct PCS_JoinGroupWithVerifyCodeRes : 
			public protocol::gverifycode::CommonGVerifyCodeOperatorRes

		{
			enum {uri = (6 << 8 | protocol::GVERIFYCODE_SVID)};

			uint64_t uOldKey;
			uint32_t uGid;
			PCS_JoinGroupWithVerifyCodeRes() {}
			virtual void marshal(sox::Pack &p) const
			{
				CommonGVerifyCodeOperatorRes::marshal(p);
				p<<uOldKey<<uGid;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CommonGVerifyCodeOperatorRes::unmarshal(p); 
				p>>uOldKey>>uGid;
			}
		};

		struct PCS_SayToGrpPeerVerifyCode :
			public protocol::gverifycode::CommonGVerifyCodeOperatorReq
		{
			enum {uri = (7 << 8 | protocol::GVERIFYCODE_SVID)};

			uint32_t m_uReqUID; // will be filled
			uint64_t m_uFactor;

			virtual void marshal(sox::Pack &p) const
			{
				CommonGVerifyCodeOperatorReq::marshal(p);
				p << m_uReqUID << m_uFactor;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CommonGVerifyCodeOperatorReq::unmarshal(p);
				p >> m_uReqUID >> m_uFactor;
			}
		};

		struct PCS_SayToGrpPeerVerifyCodeRes : 
			public protocol::gverifycode::CommonGVerifyCodeOperatorRes

		{
			enum {uri = (8 << 8 | protocol::GVERIFYCODE_SVID)};

			uint64_t uOldKey;
			uint64_t uFactor;
			uint32_t m_uSeqId;
			uint32_t id;
			std::string strText;
			PCS_SayToGrpPeerVerifyCodeRes() {}
			virtual void marshal(sox::Pack &p) const
			{
				CommonGVerifyCodeOperatorRes::marshal(p);
				p<<uOldKey<<uFactor<<m_uSeqId<<id<<strText;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				CommonGVerifyCodeOperatorRes::unmarshal(p); 
				p>>uOldKey>>uFactor>>m_uSeqId>>id>>strText;
			}
		};
		
	}
}


#endif
