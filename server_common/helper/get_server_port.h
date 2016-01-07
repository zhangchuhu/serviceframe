#ifndef __GET_SERVER_PORT_H__
#define __GET_SERVER_PORT_H__

#include <nsox/nu_types.h>
#include <vector>
#include "common/protocol/const.h"
#include "nproto/proto_cast.h"
#include "server_common/protocol/pheartd.h"

bool get_server_ports(uint16_t svid, int count, 
					  std::vector<uint16_t>& ports)
{
		typedef sox::BlockBuffer<sox::def_block_alloc_1k, 1> Buffer1k; // 1M WOW!
		typedef nsox::SockBuffer<Buffer1k> InputBuffer;

		bool success = false;
		nproto::sync_proto_client client;
		try{
				client.create();
				client.set_recv_timeout(5000);
				client.connect(nsox::address(server::heartd::heartd_listen_port,"127.0.0.1"), 500);

			
				nproto::proto_cast<server::heartd::PAllocPort> papcmd;
				papcmd.number = count;
				papcmd.svid   = svid;
				client.write_request(papcmd);

							
				nproto::proto_cast<server::heartd::PAllocPortRes> papres;
				if(client.read_request(papres))
				{
						success = true;
						std::copy(papres.ports.begin(),papres.ports.end(),std::back_inserter(ports));
				}
				client.close();
				return success;
				
		}catch (nsox::socket_error& err) {
				nsox::xlog_err(NsoxDebug,err.what_errno(), "get_server_ports error , svid: %d", svid);
				client.close();
		}
		return false;
}
uint16_t get_server_ports(uint16_t svid)
{
		std::vector<uint16_t> ports;
		if(get_server_ports(svid, 1, ports))
		{
				return ports.empty() ? 0 : ports[0];
		}
		return 0;
}

#endif