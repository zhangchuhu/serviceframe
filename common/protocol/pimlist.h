#ifndef P_IM_LIST_H
#define  P_IM_LIST_H



//以下3个文件是必须的文件
// #include "common/core/base_svid.h"///*protocol::WUJI_SIMPLE_SVID*/定义放的地方
// #include "common/packet.h"//sox::Marshallable的定义
// #include "common/protocol/const.h"

#include "core/base_svid.h"///*protocol::WUJI_SIMPLE_SVID*/定义放的地方
#include "packet.h"//sox::Marshallable的定义
#include "protocol/const.h"

#define PROTOCOL_IM_LIST_NS_BEGIN	namespace protocol{ namespace imlist{
#define PROTOCOL_IM_LIST_NS_END	} }

#define USING_PROTOCOL_IM_LIST_NS using namespace protocol::imlist;


//如果接口文件放在server\protocol目录下，和放在server\我的目录下不知道有什么区别
PROTOCOL_IM_LIST_NS_BEGIN
#define  WUJI_SIMPLE_SVID 24




PROTOCOL_IM_LIST_NS_END

#endif

