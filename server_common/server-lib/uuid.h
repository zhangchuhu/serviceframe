#ifndef YY_UUID
#define YY_UUID

#ifndef WIN32
#include <uuid/uuid.h>
#else
#include "common/uuid/uuid.h"
#endif
#include "common/packet.h"

namespace core{
struct MUUID: public sox::Marshallable{
	uuid_t id;
	MUUID(uuid_t u){
		uuid_copy(id, u);
	}
	MUUID(){
#ifndef WIN32
		memset(id, 0, sizeof(uuid_t));
#endif
	}

	bool operator== (const MUUID &u2) const{
		return uuid_compare(id, u2.id) == 0;
	}

	bool operator< (const MUUID &u2) const{
		return uuid_compare(id, u2.id) < 0;
	}

	MUUID &operator = (const MUUID &u2){
		uuid_copy(id, u2.id);
		return *this;
	}

	void marshal(sox::Pack &pk) const{
		pk << toString();
	}

	void unmarshal(const sox::Unpack &up){
		std::string str;
		up >> str;
		uuid_parse(str.data(), id);
	}

	std::string toString() const{
		char up[37];
		uuid_unparse(id, up);
		return std::string(up, strlen(up));
	}
};
}

#endif
