#include "default_watcher.h"
#include <stdio.h>
void DefaultWatcher::onNodeValueChanged(zhandle_t*,const char* path)
{
    printf("DefaultWatcher paht=%s\n", path);
}
