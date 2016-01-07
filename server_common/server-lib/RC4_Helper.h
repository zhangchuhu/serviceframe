#ifndef _RC4_HELPER_H_
#define _RC4_HELPER_H_

#include <string>

#include "protocol/const.h"
#include "core/ilink.h"

class rc4Helper{
public:
	static std::string genRC4Key();

	static std::string encRC4Key(const std::string &pubKey, const std::string &e, const std::string &rc4);
};

#endif
