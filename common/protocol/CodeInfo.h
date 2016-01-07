#ifndef CODE_INFO_H
#define CODE_INFO_H

#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/const.h"

namespace server{
	namespace sendcode{

		struct CodeInfo : public sox::Marshallable{
			uint32_t	uCodeID;		//唯一标识
			uint32_t	uCodeType;		//类型，1为 dll 
			uint32_t 	uYYmin;			//匹配的yy最低版本号
			uint32_t 	uYYmax;			//匹配的yy最高版本号
			uint32_t	uExtend;		//当前未使用
			std::string 	md5;		//代码文件md5
			std::string     strFileName;	//文件名

			virtual void marshal(sox::Pack &pk) const {
				pk << uCodeID << uCodeType << uYYmin << uYYmax << uExtend << md5 << strFileName;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> uCodeID >> uCodeType >> uYYmin >> uYYmax >> uExtend >>md5 >> strFileName;
			}
		};


	}
}


#endif
