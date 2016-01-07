#ifndef  __VIP_COMMON_H_INCLUDE__
#define  __VIP_COMMON_H_INCLUDE__

namespace protocol{ namespace vip

enum EVipLogoResCode
{
	VIP_DLOGO_UNKNOW = 0,            // 未知错误
	VIP_DLOGO_SUCCESS,               // 成功
	VIP_DLOGO_NOT_VIP,               // 非VIP
	VIP_DLOGO_DISSATISFY_GRADE       // 等级不符合条件
};

}}

#endif 
