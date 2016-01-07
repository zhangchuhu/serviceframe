#ifndef STATIC_SENDER_H
#define STATIC_SENDER_H
#include "common/core/sender.h"
namespace core{
	class StaticSender: public Sender{
	public:
		Sender &pack(uint32_t uri, sox::Marshallable &obj){
			clear();
			marshall(uri, obj);
			return *this;
		}
	};
}

#endif

