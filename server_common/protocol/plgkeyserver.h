#ifndef _P_LG_KEY_SERVER_H_
#define _P_LG_KEY_SERVER_H_

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"

namespace protocol {
static const unsigned int LG_KEY_SERVER = 179;
}

namespace server {
	namespace lg_key_server {		

		struct PGetKeysReqData: public sox::Marshallable {					
				
				std::string strAppid;		// grasp the type of the server keys.adding this variable to avoid masquerade.
				uint32_t uLocalTimestamp; // the unix time of the local keys .
				std::string strLocalMd5; //  the md5 sum of the local keys.
				uint32_t uLocalIp;		// local ip.
				uint32_t uServerId;		// local server id;
				std::string strLocalServerType; // _lk ? imlinkd ? mostly for statistic.

				virtual void marshal(sox::Pack &p) const {
					p << strAppid << uLocalTimestamp << strLocalMd5 << uLocalIp << uServerId << strLocalServerType;
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> strAppid >> uLocalTimestamp >> strLocalMd5 >> uLocalIp >> uServerId >> strLocalServerType;
				}				
			};

			struct PGetKeysReq: public sox::Marshallable {
				enum {
					uri = (1 << 8) | protocol::LG_KEY_SERVER
				};

				std::string strAppid;		// grasp the type of the server keys.
				uint32_t uLocalTimestamp; // the unix time of the local keys .

				std::string strEncryptedData;	// encrypted data,the exact structure is PGetKeysReqData,  
				std::string strSignature;			
				std::string strSequence; // context.

				virtual void marshal(sox::Pack &p) const {
					p << strAppid << uLocalTimestamp << strEncryptedData << strSignature << strSequence;
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> strAppid >> uLocalTimestamp >> strEncryptedData >> strSignature;
					
					if ( p.size() )
					{
						try
						{
							p >> strSequence;
						}
						catch(...)
						{
							strSequence.clear();
						}
					}
				}

			};

			struct PGetKeysRepData: public sox::Marshallable {

				struct KEY_ITEM
				{
					uint32_t uKeyVersion;
					std::string strKey;
					uint32_t uSkew;	// for linkd krb5int_check_clockskew					
				};

				std::string strAppid;
				uint32_t uNewTimestamp; // unix time .
				std::string strNewMd5; //  the md5 sum of the keys from the key server.

				std::vector< KEY_ITEM > vecKeys;

				virtual void marshal(sox::Pack &p) const {
					p << strAppid << uNewTimestamp << strNewMd5;

					uint32_t idsCnt = vecKeys.size();
					p << idsCnt;

					for (std::vector< KEY_ITEM >::const_iterator i = vecKeys.begin(); i != vecKeys.end(); ++i) {
						p << i->uKeyVersion;
						p << i->strKey;
						p << i->uSkew;						
					}
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> strAppid >> uNewTimestamp >> strNewMd5;

					uint32_t idsCnt;
					p >> idsCnt;

					vecKeys.clear();
					for( uint32_t i = 0 ; i != idsCnt ; ++i ) {
						KEY_ITEM item;
						p >> item.uKeyVersion >> item.strKey >> item.uSkew;
						vecKeys.push_back( item );
					}
				}

			};

			struct PGetKeysRep: public sox::Marshallable {
				enum {
					uri = (2 << 8) | protocol::LG_KEY_SERVER
				};

				uint32_t uRes; // result. 0		-	success.
				std::string strAppid;	// copy from the request.
				uint32_t uNewTimestamp; // unix time .
				std::string strNewMd5; //  the md5 sum of the keys from the key server.

				std::string strEncryptedData;	// encrypted data,the exact structure is PGetKeysRepData
				std::string strSignature;		
				std::string strSequence; // context.
				
				std::string strExtension10;				

				virtual void marshal(sox::Pack &p) const {
					p << uRes << strAppid << uNewTimestamp << strNewMd5 << strEncryptedData << strSignature << strSequence << strExtension10;			
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> uRes >> strAppid >> uNewTimestamp >> strNewMd5 >> strEncryptedData >> strSignature;				

					if ( p.size() )
					{
						try
						{
							p >> strSequence;
						}
						catch(...)
						{
							strSequence.clear();
						}						
					}
					
					if ( p.size() )
					{
						try
						{
							p >> strExtension10;
						}
						catch(...)
						{
							strExtension10.clear();
						}
					}
				}
			};

			struct PGetKeysRepExtension10: public sox::Marshallable {	

				std::vector< uint32_t > vecVerifyType;

				virtual void marshal(sox::Pack &p) const {
					uint32_t idsCnt = vecVerifyType.size();
					p << idsCnt;

					for (std::vector< uint32_t >::const_iterator i = vecVerifyType.begin(); i != vecVerifyType.end(); ++i) {
						p << *i;									
					}
				}

				virtual void unmarshal(const sox::Unpack &p) {
					uint32_t idsCnt;
					p >> idsCnt;

					vecVerifyType.clear();
					for( uint32_t i = 0 ; i != idsCnt ; ++i ) {
						uint32_t uVerify;
						p >> uVerify;
						vecVerifyType.push_back( uVerify );
					}
				}

			};
			

			// ------------------------------------------------------------

			struct PGetAllKeysReq: public sox::Marshallable {
				enum {
					uri = (3 << 8) | protocol::LG_KEY_SERVER
				};

				uint32_t uLocalTimestamp; // the unix time of the local keys .
				std::string strLocalMd5; //  the md5 sum of the local keys.
				uint32_t uLocalIp;		// local ip.
				uint32_t uServerId;		// local server id;

				virtual void marshal(sox::Pack &p) const {
					p << uLocalTimestamp << strLocalMd5 << uLocalIp << uServerId;
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> uLocalTimestamp >> strLocalMd5 >> uLocalIp >> uServerId;
				}

			};

			

			// -----------------not in use currently---------------------------------
			struct PGetAllKeysRep: public sox::Marshallable {
				enum {
					uri = (4 << 8) | protocol::LG_KEY_SERVER
				};

				struct KEY_ITEM
				{
					std::string strAppid;
					uint32_t uKeyVersion;
					std::string strKey;
					uint32_t uExpire;
				};

				uint32_t uRes; // result. 0		-	success.
				uint32_t uNewTimestamp; // unix time .
				std::string strNewMd5; //  the md5 sum of the keys from the key server.

				std::vector< KEY_ITEM > vecKeys;

				virtual void marshal(sox::Pack &p) const {
					p << uRes << uNewTimestamp << strNewMd5;

					uint32_t idsCnt = vecKeys.size();
					p << idsCnt;

					for (std::vector< KEY_ITEM >::const_iterator i = vecKeys.begin(); i != vecKeys.end(); ++i) {
						p << i->strAppid;
						p << i->uKeyVersion;
						p << i->strKey;
						p << i->uExpire;
					}
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> uRes >> uNewTimestamp >> strNewMd5;

					uint32_t idsCnt;
					p >> idsCnt;

					vecKeys.clear();
					for( uint32_t i = 0 ; i != idsCnt ; ++i ) {
						KEY_ITEM item;
						p >> item.strAppid >> item.uKeyVersion >> item.strKey >> item.uExpire;
						vecKeys.push_back( item );
					}
				}
			};

			struct PGetActivesReq: public sox::Marshallable {
				enum {
					uri = (5 << 8) | protocol::LG_KEY_SERVER
				};

				uint32_t uLocalTimestamp; // the unix time of the local keys .
				std::string strLocalMd5; // the md5 sum of the local keys.
				uint32_t uLocalIp;		// local ip.
				uint32_t uServerId;		// local server id;

				virtual void marshal(sox::Pack &p) const {
					p << uLocalTimestamp << strLocalMd5 << uLocalIp << uServerId;
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> uLocalTimestamp >> strLocalMd5 >> uLocalIp >> uServerId;
				}

			};

			struct PGetActivesRep: public sox::Marshallable {
				enum {
					uri = (6 << 8) | protocol::LG_KEY_SERVER
				};

				struct KEY_ITEM
				{
					std::string strAppid;
					uint32_t uKeyVersion;			
					std::string strKey;		// in theory,KDC has the key in memory,but what if KDC doesn't has the keys on some rare occasion.
				};

				uint32_t uRes; // result. 0		-	success.
				uint32_t uNewTimestamp; // unix time .
				std::string strNewMd5; //  the md5 sum of the keys from the key server.

				std::vector< KEY_ITEM > vecKeys;

				virtual void marshal(sox::Pack &p) const {
					p << uRes << uNewTimestamp << strNewMd5;

					uint32_t idsCnt = vecKeys.size();
					p << idsCnt;

					for (std::vector< KEY_ITEM >::const_iterator i = vecKeys.begin(); i != vecKeys.end(); ++i) {
						p << i->strAppid;
						p << i->uKeyVersion;
						p << i->strKey;
					}
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> uRes >> uNewTimestamp >> strNewMd5;

					uint32_t idsCnt;
					p >> idsCnt;

					vecKeys.clear();
					for( uint32_t i = 0 ; i != idsCnt ; ++i ) {
						KEY_ITEM item;
						p >> item.strAppid >> item.uKeyVersion >> item.strKey;
						vecKeys.push_back( item );
					}
				}
			};

			struct PGetStatisticReq: public sox::Marshallable {
				enum {
					uri = (7 << 8) | protocol::LG_KEY_SERVER
				};

				std::string strAppid;

				virtual void marshal(sox::Pack &p) const {
					p << strAppid;
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> strAppid;
				}

			};

			struct PGetStatisticRep: public sox::Marshallable {
				enum {
					uri = (8 << 8) | protocol::LG_KEY_SERVER
				};

				uint32_t uRes;
				std::string strResult;

				virtual void marshal(sox::Pack &p) const {
					p << uRes << strResult;
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> uRes >> strResult;
				}

			};

			struct PClearStatisticReq: public sox::Marshallable {
				enum {
					uri = (9 << 8) | protocol::LG_KEY_SERVER
				};

				std::string strSource;

				virtual void marshal(sox::Pack &p) const {
					p << strSource;
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> strSource;
				}

			};

			struct PClearStatisticRep: public sox::Marshallable {
				enum {
					uri = (10 << 8) | protocol::LG_KEY_SERVER
				};

				uint32_t uRes;

				virtual void marshal(sox::Pack &p) const {
					p << uRes;
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> uRes;
				}

			};

		}
	}

#endif
