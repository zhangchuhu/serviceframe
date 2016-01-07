#ifndef _P_LG_PIC_CODE_H_
#define _P_LG_PIC_CODE_H_

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"

namespace protocol {
static const unsigned int LG_PIC_CODE = 181;
}

namespace server {
	namespace lg_pic_code {		

			// ����ͼƬ��֤��
			struct PGetPicCodeReq: public sox::Marshallable {
				enum {
					uri = (1 << 8) | protocol::LG_PIC_CODE
				};

				std::string strSeq;		// �첽���������ģ�������ͻظ���Ӧ������

				uint32_t uYyuid;		// yyuid�ţ���ʶ�û���
				std::string strExtraData;	// �û��Զ������ݣ��ɲ��

				virtual void marshal(sox::Pack &p) const {
					p << strSeq << uYyuid << strExtraData;
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> strSeq >> uYyuid >> strExtraData;
				}
			};

			// ����ͼƬ��֤��Ļظ� 
			
			struct PGetPicCodeRep: public sox::Marshallable {
				enum {
					uri = (2 << 8) | protocol::LG_PIC_CODE
				};

				std::string strSeq;		// �첽���������ģ�������ͻظ���Ӧ������

				uint32_t uRes; 		// result. 0		-	success.	
                           //		1		-	something wrong like system errors.
				std::string strInternalId;	// ��֤��Ψһ��ʶ����ͨ��PVerifyPicCodeReq����֤ͼƬ��֤��ʱ���븳ֵ��PVerifyPicCodeReq::strInternalId
				std::string strImg;		// png ͼƬ�ļ�	,avaiable only when uRes is 0 .

				virtual void marshal(sox::Pack &p) const {
					p << strSeq << uRes << strInternalId << strImg;
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> strSeq >> uRes >> strInternalId >> strImg;			
				}
			};

			// ��֤�û��������֤��
			struct PVerifyPicCodeReq: public sox::Marshallable {
				enum {
					uri = (3 << 8) | protocol::LG_PIC_CODE
				};

				std::string strSeq;		// �첽���������ģ�������ͻظ���Ӧ������


				uint32_t uYyuid;		// yyuid�š�
				std::string strInternalId;	// ��֤��Ψһ��ʶ��ָ��������֤�ĸ���֤�룬��PGetPicCodeRep���ء�
				std::string strPicCode;	// �û��������֤�룬6���ַ���

				virtual void marshal(sox::Pack &p) const {
					p << strSeq << uYyuid << strInternalId << strPicCode;
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> strSeq >> uYyuid >> strInternalId >> strPicCode;
				}

			};


			// ��֤����ظ�
			struct PVerifyPicCodeRep: public sox::Marshallable {
				enum {
					uri = (4 << 8) | protocol::LG_PIC_CODE
				};

				std::string strSeq;		// �첽���������ģ�������ͻظ���Ӧ������


				uint32_t uRes;		//	0		-	succeed and the code is correct
				//	1		-	something wrong like system errors.
				//	2		-	the code is not correct									
				std::string strExtraData;	// the user-defined data for the custom-made intention. only available if uRes is 0.

				std::string strInternalId;	// including group-id , 32-bit-rand ,only avaiable if uRes is not 0.
				std::string strImg;			// a new png image,for the new image if uRes is not 0.
				virtual void marshal(sox::Pack &p) const {
					p << strSeq << uRes << strExtraData << strInternalId << strImg;		
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> strSeq >> uRes >> strExtraData >> strInternalId >> strImg;				
				}
			};		

			// ��֤��ɾ��״̬����ȫ�Բ��ߣ��������á�
			struct PVerifyPicCodeReqNoDel: public sox::Marshallable {
				enum {
					uri = (5 << 8) | protocol::LG_PIC_CODE
				};

				std::string strSeq;

				uint32_t uYyuid;
				std::string strInternalId;
				std::string strPicCode;		// code to verify ,should be 6 in size.

				virtual void marshal(sox::Pack &p) const {
					p << strSeq << uYyuid << strInternalId << strPicCode;
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> strSeq >> uYyuid >> strInternalId >> strPicCode;
				}

			};

			struct PAlivePing : public sox::Marshallable {			
				enum {
					uri = (6 << 8) | protocol::LG_PIC_CODE
				};

				std::string strReserver;				

				virtual void marshal(sox::Pack &p) const {
					p << strReserver ;
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> strReserver;
				}
			};

			//ͨ��ͼƬ��֤���ȡ�ӿ�:��֧��Ӣ�ģ�������֤�룬��
			struct PCommGetPicCodeReq: public sox::Marshallable {
				enum {
					uri = (7 << 8) | protocol::LG_PIC_CODE
				};

				std::string strSeq;		             //�첽���������ģ�������ͻظ���Ӧ������
				std::string strAppid;                 //������ҵ����
				uint32_t    uPicCodeType;           //��������ͼ��֤�������ͣ�Ŀǰ��֧������ 01 ��ĸ��֤�� 2 ������֤�� (����)
				uint32_t    uYyuid;		             //yyuid�ţ����û���ʾ��(����)
				std::string strExtraData;	         // �û��Զ������ݣ��ɲ��


				virtual void marshal(sox::Pack &p) const {
					p << strSeq << strAppid << uPicCodeType << uYyuid << strExtraData;
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> strSeq >> strAppid >> uPicCodeType >> uYyuid >> strExtraData;
				}
			};
			
			// ��֤�û��������֤��
			struct PCommVerifyPicCodeReq: public sox::Marshallable {
				enum {
					uri = (8 << 8) | protocol::LG_PIC_CODE
				};

				std::string strSeq;		// �첽���������ģ�������ͻظ���Ӧ������
				uint32_t uYyuid;		// yyuid�š�
				std::string strAppid;  ///������ҵ����
				std::string strInternalId;	// ��֤��Ψһ��ʶ��ָ��������֤�ĸ���֤�룬��PGetPicCodeRep���ء�
				uint32_t    uNextPicCodeType; //��֤ʧ��ʱ,��һ��ͼƬ��֤������: 1 ��ĸ��֤�� 2 ������֤��
				std::string strPicCode;	// �û��������֤�룬6���ַ���

				virtual void marshal(sox::Pack &p) const {
					p << strSeq << uYyuid << strAppid << strInternalId << uNextPicCodeType <<strPicCode;
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> strSeq >> uYyuid >> strAppid >> strInternalId >> uNextPicCodeType >> strPicCode;
				}

			};

			struct PVerifyPicCodeReqNoDel2: public sox::Marshallable {
				enum {
					uri = (9 << 8) | protocol::LG_PIC_CODE
				};

				std::string strSeq;

				uint32_t uYyuid;
				std::string strInternalId;
				std::string strPicCode;		// code to verify ,should be 6 in size.
				uint32_t uPicCodeType;      // 1:English, 2:Chinese

				virtual void marshal(sox::Pack &p) const {
					p << strSeq << uYyuid << strInternalId << strPicCode << uPicCodeType;
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> strSeq >> uYyuid >> strInternalId >> strPicCode >> uPicCodeType;
				}
			};

			struct PRuleGetPicCodeReq: public sox::Marshallable {
				enum {
					uri = (10 << 8) | protocol::LG_PIC_CODE
				};

				std::string strSeq;		             // context
				std::string strAppid;                // appid
				std::string strIp;                   // server ip
				uint32_t    uPicCodeType;            // 1:English, 2:Chinese
				uint32_t    uYyuid;		             // yyuid
				std::string strExtraData;	         // extra data


				virtual void marshal(sox::Pack &p) const {
					p << strSeq << strAppid << strIp << uPicCodeType << uYyuid << strExtraData;
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> strSeq >> strAppid >> strIp >> uPicCodeType >> uYyuid >> strExtraData;
				}
			};

			struct PRuleGetPicCodeRes: public sox::Marshallable {
				enum {
					uri = (11 << 8) | protocol::LG_PIC_CODE
				};

				std::string strSeq;		// context
				uint32_t uRes; 		// result,0:success,1:something wrong like system errors,999:rate error	
				std::string strInternalId;	// piccode id 
				std::string strImg;		// pic file

				virtual void marshal(sox::Pack &p) const {
					p << strSeq << uRes << strInternalId << strImg;
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> strSeq >> uRes >> strInternalId >> strImg;			
				}
			};

			struct PRuleVerifyPicCodeReq: public sox::Marshallable {
				enum {
					uri = (12 << 8) | protocol::LG_PIC_CODE
				};

				std::string strSeq;		// context
				std::string strAppid;   // appid
				std::string strIp;      // server ip
				std::string strInternalId;	// piccode id 
				uint32_t    uNextPicCodeType; // next piccode type,1:English, 2:Chinese 
				uint32_t 	uYyuid;		// yyuid
				std::string strPicCode;	// code

				virtual void marshal(sox::Pack &p) const {
					p << strSeq << strAppid << strIp << strInternalId << uNextPicCodeType << uYyuid << strPicCode;
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> strSeq >> strAppid >> strIp >> strInternalId >> uNextPicCodeType >> uYyuid >> strPicCode;
				}

			};

			struct PRuleVerifyPicCodeRes: public sox::Marshallable {
				enum {
					uri = (13 << 8) | protocol::LG_PIC_CODE
				};

				std::string strSeq;		// context
				uint32_t uRes;		// result,0:succeed and the code is correct,1:something wrong like system errors,2:the code is not correct,999:rate error 	
				std::string strExtraData;	// the user-defined data for the custom-made intention. only available if uRes is 0.
				std::string strInternalId;	// including group-id , 32-bit-rand ,only avaiable if uRes is not 0.
				std::string strImg;			// a new png image,for the new image if uRes is not 0.
				virtual void marshal(sox::Pack &p) const {
					p << strSeq << uRes << strExtraData << strInternalId << strImg;		
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> strSeq >> uRes >> strExtraData >> strInternalId >> strImg;				
				}
			};		
	}
}


#endif

