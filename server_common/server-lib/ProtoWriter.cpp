#include "ProtoWriter.h"

using namespace core;
int ProtoWriter::flush(IConn *conn){
	int ret;

	try{
		for(std::vector<IPacketBase *>::iterator it = sendBuf.begin(); it != sendBuf.end(); ++it){
			ret = (*it)->emit(conn, sDispatcher);
			if(ret == -1){
				break;
			}
		}
	}
	catch(...)
	{
		if (NULL != pp)
			pp->gcPackets(sendBuf);
		
		throw;
	}

	if (NULL != pp)
		pp->gcPackets(sendBuf);
	return ret;
}
