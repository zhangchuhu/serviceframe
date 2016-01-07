#ifndef PSENDCODE_H_
#define PSENDCODE_H_

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"
#include "protocol/common/protocol/plogin.h"
#include "common/protocol/CodeInfo.h"

namespace server{
	namespace sendcode{
		struct PGetCodeContent : public sox::Voidmable{
			enum {uri = (1 << 8) | protocol::SENDCODE_SVID};
		};

		struct PGetCodeContentRes : public sox::Marshallable{
			enum {uri = (2 << 8) | protocol::SENDCODE_SVID};
			std::vector<server::sendcode::CodeInfo> codeInfoSet;

			virtual void marshal(sox::Pack &pk) const {
				sox::marshal_container(pk, codeInfoSet);
			}

			virtual void unmarshal(const sox::Unpack &up){
				sox::unmarshal_container(up, std::back_inserter(codeInfoSet));
			}
		};

		struct PGetCodeFile : public sox::Marshallable{
			enum {uri = (3 << 8) | protocol::SENDCODE_SVID};
			std::string strFileName;

			virtual void marshal(sox::Pack &pk) const {
				pk << strFileName;
			}

			virtual void unmarshal(const sox::Unpack &up){
				up >> strFileName;
			}
		};

		struct PGetCodeFileRes : public sox::Marshallable{
			enum {uri = (4 << 8) | protocol::SENDCODE_SVID};
			std::string strFileName;
			std::string strFileContent;

			virtual void marshal(sox::Pack &pk) const {
				pk << strFileName << strFileContent;
			}

			virtual void unmarshal(const sox::Unpack &up){
				up >> strFileName >> strFileContent;
			}
		};

	}
}
 

#endif
