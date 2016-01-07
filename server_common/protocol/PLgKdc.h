#ifndef _P_LG_KDC_H_
#define _P_LG_KDC_H_

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"

namespace server{
	namespace lgkdc
	{
		struct PAsRequest:  public sox::Marshallable{
			enum {uri = (103 << 8) | protocol::LG_KDC_SVID };			

			std::string strSeq;			// 异步调用时使用的上下文，PAsResponse中会回传
			uint32_t uVersion;			// 版本
			uint32_t uIp;				// 客户端IP
			std::string strUid;			// 用户uid，目的是为了方便调试
			std::string strAsn;			// AS-REQ ASN数据包。
			
			virtual void marshal(sox::Pack &p) const {					
				p << strSeq << uVersion << uIp << strUid << strAsn;
			}

			virtual void unmarshal(const sox::Unpack &p){			
				p >> strSeq >> uVersion >> uIp >> strUid >> strAsn;
			}
		};			

		struct PAsResponse:  public sox::Marshallable{
			enum {uri = (104 << 8) | protocol::LG_KDC_SVID };			

			enum AS_REP_RESULT
			{
				AS_REP_RESULT_SUCCESS,		// succeed.
				AS_REP_RESULT_NOT_EXIST,	// the user is not existing in db.
				AS_REP_RESULT_DB_FAIL,		// db failure.
				AS_REP_RESULT_SYSTEM_ERROR	// such as : the username's too long.			
			};

			std::string strSeq;			// 回传PAsRequest的strReq，异步调用时使用的上下文。
			uint32_t uResult;			// 0	-	success. others		-	fail.
			uint32_t uVersion;			// 版本
			std::string strAsn;			// AS-RES ASN数据包.

			virtual void marshal(sox::Pack &p) const {					
				p << strSeq << uResult << uVersion << strAsn;
			}

			virtual void unmarshal(const sox::Unpack &p){			
				p >> strSeq >> uResult >> uVersion >> strAsn;
			}
		};				

		struct PAsRequest20:  public sox::Marshallable{
			enum {uri = (105 << 8) | protocol::LG_KDC_SVID };			

			std::string strSeq;			// 异步调用时使用的上下文，PAsResponse中会回传
			uint32_t uVersion;			// 版本
			uint32_t uIp;				// 客户端IP
			std::string strUid;			// 用户uid，目的是为了方便调试
			std::string strAsn;			// AS-REQ ASN数据包。
			std::string strClientVersion;	//	客户端版本号,kdc根据它来版本返回as_response的个数。
			std::string strAccessToken;		// face book access token
			std::string strAppid;		// special id for some application except YY client.for instance : mobile app game.

			virtual void marshal(sox::Pack &p) const {					
				p << strSeq << uVersion << uIp << strUid << strAsn << strClientVersion << strAccessToken << strAppid;
			}

			virtual void unmarshal(const sox::Unpack &p){			
				p >> strSeq >> uVersion >> uIp >> strUid >> strAsn >> strClientVersion >> strAccessToken;

					if ( false == p.empty() ){
						try{
							p >> strAppid;
						}
						catch(...){
						}
					}
				
			}

		};			

		struct PAsRequest30:  public sox::Marshallable{
			enum {uri = (107 << 8) | protocol::LG_KDC_SVID };			

			std::string strSeq;			// 异步调用时使用的上下文，PAsResponse中会回传
			uint32_t uVersion;			// 版本
			uint32_t uIp;				// 客户端IP
			std::string strUid;			// 用户uid，目的是为了方便调试
			std::string strAsn;			// AS-REQ ASN数据包。
//			std::string strClientVersion;	//	客户端版本号,kdc根据它来版本返回as_response的个数。
			uint16_t    _app_type;
			std::string strAccessToken;		// face book access token
			std::string strAppid;		// special id for some application except YY client.for instance : mobile app game.

			virtual void marshal(sox::Pack &p) const {					
				p << strSeq << uVersion << uIp << strUid << strAsn << _app_type << strAccessToken << strAppid;
			}

			virtual void unmarshal(const sox::Unpack &p){			
				p >> strSeq >> uVersion >> uIp >> strUid >> strAsn >> _app_type >> strAccessToken;

					if ( false == p.empty() ){
						try{
							p >> strAppid;
						}
						catch(...){
						}
					}
				
			}

		};			
		
	}
}


#endif
