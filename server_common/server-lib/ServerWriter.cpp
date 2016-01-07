#include "ServerWriter.h"
#include "common/core/ilink.h"
#include "core/sox/logger.h"
using namespace core;
using namespace server;

void ServerWriter::answer(uint32_t uri, const sox::Marshallable &obj){
	send(pp->getAnswerPacket(uri, RES_SUCCESS, obj));
}


void ServerWriter::answer(uint32_t uri, uint16_t resCode, const sox::Marshallable &obj){
	send(pp->getAnswerPacket(uri, resCode, obj));
}

void ServerWriter::answerErr(uint32_t uri, uint16_t ResCode){
	send(pp->getAnswerPacket(uri, ResCode, sox::Voidmable()));
}
