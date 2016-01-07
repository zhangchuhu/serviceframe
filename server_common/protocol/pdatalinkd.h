#ifndef pdatalinkd_h__
#define pdatalinkd_h__

#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/const.h"
#include "common/protocol/PP2PLinkd.h"
#include <vector>

namespace protocol {
	namespace p2p {
		struct PSS_TcpDataUpload : public PCS_TcpDataUpload
		{
			enum {uri = (109 << 8) | P2PLINKD_SVID};
		};
		
		struct PSS_TcpDataUploadRes : public PCS_TcpDataUploadRes 
		{
			enum {uri = (110 << 8) | P2PLINKD_SVID};

			PSS_TcpDataUploadRes()
			{}
			PSS_TcpDataUploadRes(const PSS_TcpDataUpload &objReq) : PCS_TcpDataUploadRes(objReq)
			{}
		};
		
		struct PSS_TcpDataDownload : public PCS_TcpDataDownload
		{
			enum {uri = (111 << 8) | P2PLINKD_SVID};
		};
		
		struct PSS_TcpDataDownloadRes : public PCS_TcpDataDownloadRes
		{
			enum {uri = (112 << 8) | P2PLINKD_SVID};

			PSS_TcpDataDownloadRes()
			{}
			PSS_TcpDataDownloadRes(const PSS_TcpDataDownload &objReq) : PCS_TcpDataDownloadRes(objReq)
			{}
		};

		struct PSS_TcpDataS2S : public PCS_TcpDataUpload
		{
			enum {uri = (113 << 8) | P2PLINKD_SVID};

			PSS_TcpDataS2S()
			{}
			PSS_TcpDataS2S(const PSS_TcpDataUpload &objReq) : PCS_TcpDataUpload( static_cast<PCS_TcpDataUpload>(objReq) )
			{}
		};

		struct PSS_TcpDataS2SRes : public PCS_TcpDataUploadRes
		{
			enum {uri = (114 << 8) | P2PLINKD_SVID};

			PSS_TcpDataS2SRes()
			{}
			PSS_TcpDataS2SRes(const PSS_TcpDataS2S &objReq) : PCS_TcpDataUploadRes(objReq)
			{}
		};

	}
}
#endif // pdatalinkd_h__
