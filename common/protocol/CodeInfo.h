#ifndef CODE_INFO_H
#define CODE_INFO_H

#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/const.h"

namespace server{
	namespace sendcode{

		struct CodeInfo : public sox::Marshallable{
			uint32_t	uCodeID;		//Ψһ��ʶ
			uint32_t	uCodeType;		//���ͣ�1Ϊ dll 
			uint32_t 	uYYmin;			//ƥ���yy��Ͱ汾��
			uint32_t 	uYYmax;			//ƥ���yy��߰汾��
			uint32_t	uExtend;		//��ǰδʹ��
			std::string 	md5;		//�����ļ�md5
			std::string     strFileName;	//�ļ���

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
