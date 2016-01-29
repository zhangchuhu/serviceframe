#ifndef __DEFAULT_WATCHER__
#define __DEFAULT_WATCHER__
#include "zk_watcher.h"
#include "example.h"
class DefaultWatcher: public WatcherAction
{
    void onNodeValueChanged(zhandle_t*,const char* path);
};
#endif
