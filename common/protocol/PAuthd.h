#pragma once

#include <vector>
#include <string>
#include "protocol/const.h"
#include "core/base_svid.h"

namespace protocol {
namespace yauthd {

struct PAuthTokenReq: public sox::Marshallable {
	enum {
		uri = (1 << 8) | protocol::YAUTHD_SVID
	};

	std::string requestText;

	virtual void marshal(sox::Pack &p) const {
		p << requestText;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> requestText;
	}
};

struct PAuthTokenRes: public sox::Marshallable {
	enum {
		uri = (2 << 8) | protocol::YAUTHD_SVID
	};

	std::string autToken;

	virtual void marshal(sox::Pack &p) const {
		p << autToken;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> autToken;
	}
};

struct PGetInfoGenericReq: public sox::Marshallable {
	enum {
		uri = (3 << 8) | protocol::YAUTHD_SVID
	};

	std::string requestText;

	virtual void marshal(sox::Pack &p) const {
		p << requestText;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> requestText;
	}
};

struct PGetInfoGenericRes: public sox::Marshallable {
	enum {
		uri = (4 << 8) | protocol::YAUTHD_SVID
	};

	std::string infoString;

	virtual void marshal(sox::Pack &p) const {
		p << infoString;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> infoString;
	}
};

struct PSetInfoGenericReq: public sox::Marshallable {
	enum {
		uri = (5 << 8) | protocol::YAUTHD_SVID
	};

	std::string requestText;

	virtual void marshal(sox::Pack &p) const {
		p << requestText;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> requestText;
	}
};

struct PSetInfoGenericRes: public sox::Marshallable {
	enum {
		uri = (6 << 8) | protocol::YAUTHD_SVID
	};

	std::string successList;	// 成功设置的属性列表，例如 IDCardNumber&IDCardName

	virtual void marshal(sox::Pack &p) const {
		p << successList;
	}

	virtual void unmarshal(const sox::Unpack &p) {
		p >> successList;
	}
};
}
}