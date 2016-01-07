#ifndef PROTOWRITER_H_
#define PROTOWRITER_H_
#include "IPacket.h"
#include "common/core/iserver.h"
#include "core/corelib/WriterImp.h"

namespace core{
class ProtoWriter: public AbstractWriter,
				   public IPacketProtoAware,
				   public IServerIdDispatcherAware
{
public:
	virtual int flush(IConn *ansConn = NULL);
};
}

#endif

