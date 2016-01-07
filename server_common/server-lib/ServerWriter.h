#ifndef SERVER_WRITER_H
#define SERVER_WRITER_H
#include "ProtoWriter.h"

namespace server{
	class ServerWriter: public core::ProtoWriter{
	public:
		virtual void answer(uint32_t uri, const sox::Marshallable &obj);

		virtual void answer(uint32_t uri, uint16_t resCode, const sox::Marshallable &obj);
		
		virtual void answerErr(uint32_t uri, uint16_t ResCode);
	};

}
#endif

