#include "WrapServerStart.h"
#include "core/sox/tcpsock.h"
#include "core/sox/selsel.h"
#include "core/sox/logger.h"
using namespace core;
using namespace sox;

//sox_env_help WrapServerStart::soxEnv;

void WrapServerStart::init(){
	static sox_env_help soxEnv;
}
void WrapServerStart::run() {
	log(Info, "server start, epoll_prt=%p", sox::env::selector());
	sox::env::selector()->mainloop();
	log(Info, "server end");

}

void WrapServerStart::stop(){
	sox::env::selector()->stop();
}
