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

			// 生成图片验证码
			struct PGetPicCodeReq: public sox::Marshallable {
				enum {
					uri = (1 << 8) | protocol::LG_PIC_CODE
				};

				std::string strSeq;		// 异步调用上下文，把请求和回复对应起来。

				uint32_t uYyuid;		// yyuid号，标识用户。
				std::string strExtraData;	// 用户自定义数据，可不填。

				virtual void marshal(sox::Pack &p) const {
					p << strSeq << uYyuid << strExtraData;
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> strSeq >> uYyuid >> strExtraData;
				}
			};

			// 生成图片验证码的回复 
			
			struct PGetPicCodeRep: public sox::Marshallable {
				enum {
					uri = (2 << 8) | protocol::LG_PIC_CODE
				};

				std::string strSeq;		// 异步调用上下文，把请求和回复对应起来。

				uint32_t uRes; 		// result. 0		-	success.	
                           //		1		-	something wrong like system errors.
				std::string strInternalId;	// 验证码唯一标识，在通过PVerifyPicCodeReq来验证图片验证码时必须赋值给PVerifyPicCodeReq::strInternalId
				std::string strImg;		// png 图片文件	,avaiable only when uRes is 0 .

				virtual void marshal(sox::Pack &p) const {
					p << strSeq << uRes << strInternalId << strImg;
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> strSeq >> uRes >> strInternalId >> strImg;			
				}
			};

			// 验证用户输入的验证码
			struct PVerifyPicCodeReq: public sox::Marshallable {
				enum {
					uri = (3 << 8) | protocol::LG_PIC_CODE
				};

				std::string strSeq;		// 异步调用上下文，把请求和回复对应起来。


				uint32_t uYyuid;		// yyuid号。
				std::string strInternalId;	// 验证码唯一标识，指定本次验证哪个验证码，由PGetPicCodeRep返回。
				std::string strPicCode;	// 用户输入的验证码，6个字符。

				virtual void marshal(sox::Pack &p) const {
					p << strSeq << uYyuid << strInternalId << strPicCode;
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> strSeq >> uYyuid >> strInternalId >> strPicCode;
				}

			};


			// 验证结果回复
			struct PVerifyPicCodeRep: public sox::Marshallable {
				enum {
					uri = (4 << 8) | protocol::LG_PIC_CODE
				};

				std::string strSeq;		// 异步调用上下文，把请求和回复对应起来。


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

			// 验证后不删除状态，安全性不高，不建议用。
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

			//通用图片验证码获取接口:可支持英文，中文验证码，或
			struct PCommGetPicCodeReq: public sox::Marshallable {
				enum {
					uri = (7 << 8) | protocol::LG_PIC_CODE
				};

				std::string strSeq;		             //异步调用上下文，把请求和回复对应起来。
				std::string strAppid;                 //请求者业务编号
				uint32_t    uPicCodeType;           //请求生成图验证验码类型，目前仅支持两种 01 字母验证码 2 中文验证码 (必填)
				uint32_t    uYyuid;		             //yyuid号，或用户标示。(必填)
				std::string strExtraData;	         // 用户自定义数据，可不填。


				virtual void marshal(sox::Pack &p) const {
					p << strSeq << strAppid << uPicCodeType << uYyuid << strExtraData;
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> strSeq >> strAppid >> uPicCodeType >> uYyuid >> strExtraData;
				}
			};
			
			// 验证用户输入的验证码
			struct PCommVerifyPicCodeReq: public sox::Marshallable {
				enum {
					uri = (8 << 8) | protocol::LG_PIC_CODE
				};

				std::string strSeq;		// 异步调用上下文，把请求和回复对应起来。
				uint32_t uYyuid;		// yyuid号。
				std::string strAppid;  ///请求者业务编号
				std::string strInternalId;	// 验证码唯一标识，指定本次验证哪个验证码，由PGetPicCodeRep返回。
				uint32_t    uNextPicCodeType; //验证失败时,下一个图片验证码类型: 1 字母验证码 2 中文验证码
				std::string strPicCode;	// 用户输入的验证码，6个字符。

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

