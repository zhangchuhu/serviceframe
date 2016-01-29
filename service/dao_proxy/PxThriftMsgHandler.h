#ifndef _DAOPROXYTHRIFTPX_MSG_HANDLER_H_
#define _DAOPROXYTHRIFTPX_MSG_HANDLER_H_

#include "server_common/server-lib3/IServerStategy.h"
#include "server_common/server-lib3/ITask.h"
#include "server_common/server-lib3/interface.h"

#include "common/core/ilink.h"
#include "server_back/thrift-bridge/IThriftBridge.h"
#include <transport/TTransport.h>
#include <protocol/TProtocol.h>
#include <boost/shared_ptr.hpp>
#include "route_types.h"

namespace server
{
    namespace dao_proxy
    {
        using namespace core;
        using apache::thrift::protocol::TProtocol;
        using apache::thrift::protocol::TProtocolFactory;
        using apache::thrift::transport::TTransportFactory;

		class ThriftPxMsgHandler : public server::thrift::IProxyProcessor
        {
		public:
            ThriftPxMsgHandler();
            ~ThriftPxMsgHandler();
			virtual int process(uint32_t cid, const char *buf, size_t len);

		private:
			boost::shared_ptr<TProtocolFactory> m_inputProtocolFactory;
			/////////////
		public:
			int generate_fname_not_found_pack(int32_t seqid,std::string& fname,std::string& out_thrift);
			int generate_fname_unknown_exception_pack(int32_t seqid,std::string& fname,std::string& exception_desc,std::string& out_thrift);

			int generate_routeresp_pack(int32_t seqid,RouteResp* rtresp,std::string& out_thrift);
        };
    }
}
#endif //_DAOPROXYTHRIFTPX_MSG_HANDLER_H_
