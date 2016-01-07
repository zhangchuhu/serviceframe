#include "BranchesFilter.h"
#include "server_common/protocol/prouter.h"

using namespace core;
using namespace server::filter;


bool BranchesFilter::process(core::Request &request){
	if((request.getUri() && 0xff) != protocol::DAEMON_SVID){
		sox::PackBuffer pb;
		sox::Pack pk(pb);
		request.packOrgin(pk);
		
		server::router::PBrachRoute br;
		br.load.assign(pk.data(), pk.size());

		writer->route(target, br.uri, br);
		
	}
	return true;
}
