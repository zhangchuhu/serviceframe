#include "thrift_watcher.h"
#include <stdio.h>
void ThriftWatcher::onNodeValueChanged(zhandle_t*,const char* path)
{
    printf("DefaultWatcher paht=%s\n", path);

}
