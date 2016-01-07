#ifndef PINVITE_CODE_PROTOCOL_H
#define PINVITE_CODE_PROTOCOL_H
#include "common/core/base_svid.h"
#include "common/iproperty.h"
#include "common/protocol/const.h"
#include "common/protocol/ChannelRoler.h"
namespace protocol{
        namespace invitecode{
		struct PInviteCodeApplyRes: public sox::Marshallable
		{
			enum{uri = 400 << 8| INVITEDB_SVID};
			uint32_t sid;
			uint32_t res;  //0:成功  1:邀请码过期  2:邀请码已使用   3:提交不成功 4:邀请码错误
			uint16_t role;
			std::string corpName;
			void marshal(sox::Pack &pk) const
			{
				pk << sid << res << role;
				pk << corpName;
			}
			void unmarshal(const sox::Unpack &up)
			{
				up >> sid >> res >> role;
				up >> corpName;
			}
		};
		
		struct PInviteCodeApply: public sox::Marshallable{
			enum{uri = 309 << 8| INVITEDB_SVID};
			uint32_t sid;
			std::string inviteCode;
			void marshal(sox::Pack &pk) const{
				pk << sid << inviteCode;
			}
			void unmarshal(const sox::Unpack &up){
				up >> sid >> inviteCode;
			}
		};


                struct PInviteCorpsApplyRes: public sox::Marshallable
                {
                        enum{uri = 1 << 8| INVITEDB_SVID};
                        uint32_t sid;
			uint32_t uid;
                        uint32_t res;  //0:加入成功  1:加入失败 
			//uint16_t role;
                        std::string corpsName;
			std::string from;
                        void marshal(sox::Pack &pk) const
                        {
                                pk << sid <<uid << res  << corpsName << from;
                        }
                        void unmarshal(const sox::Unpack &up)
                        {
                                up >> sid  >> uid >> res  >> corpsName >> from;
                        }
                };

                struct PInviteCorpsApply: public sox::Marshallable{
                        enum{uri = 2 << 8| INVITEDB_SVID};
                        uint32_t sid;
			uint32_t uid;
			//uint16_t role;
                        std::string corpsID;
			std::string from;
                        void marshal(sox::Pack &pk) const{
                                pk << sid << uid <<corpsID << from;
                        }
                        void unmarshal(const sox::Unpack &up){
                                up >> sid >>uid >> corpsID >> from;
                        }
                };
	}
}

#endif /*PINVITECODE_*/
