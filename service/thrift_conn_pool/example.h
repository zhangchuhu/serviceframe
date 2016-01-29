/*
 * @brief:  example code
 * @author: Rowan Zhu <rowanzhu@yeah.net>
 */
#ifndef _ZK_CPP_INTERFACE_EXAMPLE_
#define _ZK_CPP_INTERFACE_EXAMPLE_

#include "zk_client.h"

using namespace libzk;

class MyZkClient: public ZkClient
{
    protected:
        virtual void onDataCompletion(int ret, const char *data,int len, void * user_data);
};

#endif
