#ifndef P_IM_LIST_H
#define  P_IM_LIST_H



//����3���ļ��Ǳ�����ļ�
// #include "common/core/base_svid.h"///*protocol::WUJI_SIMPLE_SVID*/����ŵĵط�
// #include "common/packet.h"//sox::Marshallable�Ķ���
// #include "common/protocol/const.h"

#include "core/base_svid.h"///*protocol::WUJI_SIMPLE_SVID*/����ŵĵط�
#include "packet.h"//sox::Marshallable�Ķ���
#include "protocol/const.h"

#define PROTOCOL_IM_LIST_NS_BEGIN	namespace protocol{ namespace imlist{
#define PROTOCOL_IM_LIST_NS_END	} }

#define USING_PROTOCOL_IM_LIST_NS using namespace protocol::imlist;


//����ӿ��ļ�����server\protocolĿ¼�£��ͷ���server\�ҵ�Ŀ¼�²�֪����ʲô����
PROTOCOL_IM_LIST_NS_BEGIN
#define  WUJI_SIMPLE_SVID 24




PROTOCOL_IM_LIST_NS_END

#endif

