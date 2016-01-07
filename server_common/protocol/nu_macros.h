#ifndef __NU_MACROS_H__
#define __NU_MACROS_H__
 


#define  SOX_BEGIN_REQUEST_MAP(theClass, connType)\
		virtual void  process_request(uint32_t uri, const char* data, uint32_t len, connType* c)\
		 { 


#define  SOX_REQUEST_HANDLER(Proto, func) \
		if(uri == Proto::uri){\
		Proto pro; sox::Unpack upk(data,len);	\
		try{pro.unpack(upk);}catch(sox::UnpackError& err){ \
		log(Info, "unpack request error , uri: 0x%x", uri);\
		}\
		return func(pro, c);\
		}

#define  SOX_END_REQUEST_MAP()\
		log(Info, "this uri has not been processed, uri : 0x%x", uri);\
		 }

#define  CHAIN_REQUEST_MAP_MEMBER(theChainMember) \
		return theChainMember.process_request(uri,data,len,c);

#endif
