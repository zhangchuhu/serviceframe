#ifndef _SWIFTZIP_H_
#define _SWIFTZIP_H_

#include "zlib.h"
#include "common/packet.h"
#include <unistd.h> 


template<class M >
class SwiftZipMarshal: public M
{
public:
	enum {uri = M::uri};
	mutable uint32_t zSize;
	mutable std::string zipData;

	void marshal(sox::Pack & p) const
	{
		sox::PackBuffer pb;
		sox::Pack tmp(pb);
		M::marshal(tmp);
		int comressItor = 3;
 		zSize = (uint32_t)tmp.size();

		if(zSize<128)///too little no compress
		{
			zipData.assign(tmp.data(), tmp.size());
		}
		else
		{
			uLong dlength = compressBound(zSize);
			Bytef *buf = new Bytef[dlength];
			memset(buf, 0, dlength);
			int ret = compress2(buf, &dlength, (const Bytef *)tmp.data(), (uLong)tmp.size(), comressItor);

			if(ret == Z_OK)
			{
				if(dlength<zSize)
				{
					zipData.assign((const char *)buf, dlength);
				}
				else
				{
					zipData.assign(tmp.data(), tmp.size());
				}
			}
			delete[] buf;
		}
		p << zSize;
		p.push_varstr32(zipData.data(), zipData.length());
	}

	void unmarshal(const sox::Unpack &p){
		p >> zSize;
		zipData = p.pop_varstr32();
		if(zipData.size() == 0)
		{
			throw sox::UnpackError("zip data is zero");
			return;
		}
		if(zSize==zipData.size())
		{
			sox::Unpack up2(zipData.data(), zipData.size());
			M::unmarshal(up2);
			return;
		}
		Bytef *buf = new Bytef[zSize];
		uLong dLength = zSize;

		int ret = uncompress(buf, &dLength, (const Bytef *)zipData.data(), (uLong)zipData.length());
		if(ret != Z_OK)
		{
			delete[] buf;
			throw sox::UnpackError("unzip error");
		}
		else
		{
			sox::Unpack up2(buf, dLength);
			M::unmarshal(up2);
			delete[] buf;
		}

	}

};

#endif //_SWIFTZIP_H_

