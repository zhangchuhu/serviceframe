#include "MultiPortServer.h"

using namespace core;
using namespace std;

ServerSocketHelper *MultiPortServer::create(const char* ip, uint16_t p, uint16_t &cur){
	sox::Sockethelper so;
	so.socket();
	so.setreuse();
	so.setblocking(false);
	so.setnodelay();
	so.bind(p, ip);

	return new ServerSocketHelper(this, so.detach());
}



ServerSocketHelper *MultiPortServer::createHelper(const char *ip){
	if(!ports.empty()){
		for(int i = 0; i < 100; ++i){
			if(ports.front() + i == 445)
				continue;
			try{
				uint16_t tmp;
				ServerSocketHelper *ret = create(ip, ports.front() + i, tmp);

				curports.push_back(ports.front() + i);
				log(Info, "create listen port %d success", ports.front() + i);

				ports.erase(ports.begin());
				return ret;
			}catch(sox::socket_error se){
				log(Info, "alloc port conflict port: %u", ports.front() + i);
			}
		}
	}

	return NULL;
}




void MultiPortServer::start(){
	while(!ports.empty())
		helpers.push_back(createHelper(NULL));

	for (std::vector<ServerSocketHelper *>::iterator it = helpers.begin(); it
		!= helpers.end(); ++it) {
			(*it)->start();
		}
}
