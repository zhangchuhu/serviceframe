#ifndef _P_LG_KDC_CLIENT_H_
#define _P_LG_KDC_CLIENT_H_

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"

namespace protocol
{
//	static const unsigned int LG_KDC_SVID =  116;
};


namespace server{
	namespace lgkdc
	{
		//activeX触发linkd透传的协议
		struct PTGSReq: public sox::Marshallable {
			enum {uri = (101 << 8) | protocol::LG_KDC_SVID};

			PTGSReq() {};
			PTGSReq(uint32_t id, uint32_t ver,std::string exten)
				: uid(id), version(ver),extension(exten) {};

			uint32_t uid;
			uint32_t version;
			std::string extension; //activeX透传的身份票据
			uint32_t ip;		

			virtual void marshal(sox::Pack &pk) const {
				pk << uid << version << extension;
				pk << ip;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> uid >> version >> extension;
				up >> ip;
			}
		};

		struct PTGSRes: public sox::Marshallable {
			enum {uri = (102 << 8) | protocol::LG_KDC_SVID};
			PTGSRes(){};
			PTGSRes(uint32_t version,std::string extension):version(version),extension(extension){};
			uint32_t version;
			std::string extension; //activeX透传的身份票据
			uint32_t result;
			virtual void marshal(sox::Pack &pk) const {
				pk << version << extension << result;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> version >> extension >> result;
			}
		};

		//activeX触发linkd透传的协议
		struct PTGSReq20: public sox::Marshallable {
			enum {uri = (201 << 8) | protocol::LG_KDC_SVID};

			PTGSReq20() {};
			PTGSReq20(uint32_t id, uint32_t ver,std::string exten)
				: uid(id), version(ver),extension(exten) {};

			uint32_t uid;
			uint32_t version;
			std::string extension; //activeX透传的身份票据
			uint32_t ip;		
			uint32_t serverid;		//	回传，只有client protocol用到。
			uint32_t windowid;		//	回传，只有client protocol用到。

			virtual void marshal(sox::Pack &pk) const {
				pk << uid << version << extension;
				pk << ip << serverid << windowid;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> uid >> version >> extension;
				up >> ip >> serverid >> windowid;
			}
		};

		struct PTGSRes20: public sox::Marshallable {
			enum {uri = (202 << 8) | protocol::LG_KDC_SVID};
			PTGSRes20(){};
			PTGSRes20(uint32_t version,std::string extension):version(version),extension(extension){};
			uint32_t version;
			std::string extension; //activeX透传的身份票据
			uint32_t result;
			uint32_t serverid;		//	回传，只有client protocol用到。
			uint32_t windowid;		//	回传，只有client protocol用到。

			virtual void marshal(sox::Pack &pk) const {
				pk << version << extension << result << serverid << windowid;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> version >> extension >> result >> serverid >> windowid;
			}
		};

		// 包含了两个as response的extension：tgt、linkd。
		struct PAsRepExtension : public sox::Marshallable
		{
			PAsRepExtension() : flag(0) {};

			uint64_t	flag;			// 必须为零，表示该extension为组合extension，以区分旧的extension.
			std::string tgtExtension;
			std::string linkdExtension;
			std::string web5060Extension;
			
		   virtual void marshal(sox::Pack &pk) const {
					   pk << flag << tgtExtension << linkdExtension << web5060Extension;
		   }
			
		   virtual void unmarshal(const sox::Unpack &up) {
					   up >> flag >> tgtExtension >> linkdExtension;
			
					   if ( false == up.empty() ){
						   try{
								   up >> web5060Extension;
							   }
						   catch(...){
							   }
					   }
		   }
							   

		};

		struct PTGSReqMulti: public sox::Marshallable {
			enum {uri = (110 << 8) | protocol::LG_KDC_SVID};

			PTGSReqMulti() {};		

			std::string strSequence; 
			std::string strApp;			// the app type of the client.
			std::string strUname;
			std::string strTgsReqCombine;	// include a series of encoded tgs requests.

			virtual void marshal(sox::Pack &pk) const {
				pk << strSequence << strApp << strUname << strTgsReqCombine;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSequence >> strApp >> strUname >> strTgsReqCombine;
			}
		};

		struct PTGSResMulti: public sox::Marshallable {
			enum {uri = (111 << 8) | protocol::LG_KDC_SVID};

			PTGSResMulti() {};		

			std::string strSequence; 			
			uint32_t 	uRes; 				// 0 for success. others for fail.
			uint32_t	uUid; 				// udb id.not yyuid.
			uint32_t	uYyUid; 			// yyUid.
			std::string strTgsRepCombine;	// include a series of encoded tgs response.		
			

			virtual void marshal(sox::Pack &pk) const {
				pk << strSequence << uRes << uUid << uYyUid << strTgsRepCombine;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSequence >> uRes >> uUid >> uYyUid >> strTgsRepCombine;
			}
		};


#if defined(__i386__)||defined(WIN32)||defined(__x86_64__)
#define XHTONS
#else /* big end */
		inline uint16_t XHTONS(uint16_t i16) {
			return ((i16 << 8) | (i16 >> 8));
		}
#endif

#define XNTOHS XHTONS


		class CEntensionEncoder
		{	
		public:

			// encode.
			//		each string in lsItems must be less than 65535.
			// return	:	void
			// params	:	lsItems		-	a series of string to be encoded.
			//				strCombine	-	to retrieve a combined string which could be empty if lsItems is empty.
			static void extensionEncode( const std::vector<std::string>& vecItems,std::string& strCombine )
			{
				strCombine.clear();

				for( std::vector<std::string>::const_iterator i = vecItems.begin() ; i != vecItems.end() ; ++i )
				{
					unsigned short uLen = i->size();
					uLen = XHTONS( uLen );
					std::string strLen;
					strLen.assign( (char*)&uLen,sizeof(uLen));
					strCombine += strLen;

					if ( i->size() )
					{
						strCombine += *i;
					}		
				}
			}

			// decode
			// return		:		0			-	success
			//						-1			-	fail
			// params		:		strCombine	-	a combined and encoded string.
			//						lsItems		-	to retrieve the decoded strings.could be empty if strCombine is empty.
			static int extensionDecode( const std::string& strCombine,std::vector<std::string>& vecItems )
			{
				vecItems.clear();

				if ( strCombine.empty() )
					return 0;

				const char* pData = strCombine.data();	
				unsigned int uLeft = strCombine.size();

				while ( uLeft > 0 )
				{
					if ( uLeft < sizeof(unsigned short) )
					{	
						return -1;
					}			

					unsigned short uLen = *( (unsigned short*) pData );
					uLen = XHTONS( uLen );
					uLeft -= sizeof(unsigned short);
					pData += sizeof(unsigned short);
					if ( uLen > uLeft )
						return -1;

					std::string strOne;
					if ( uLen )
						strOne.assign( pData,uLen );
					vecItems.push_back( strOne );
					pData += uLen;
					uLeft -= uLen;		
				}

				return 0;	
			}
		};


	}
}

#endif
