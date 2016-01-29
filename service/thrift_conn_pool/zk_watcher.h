/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ZKMOCKS_H_
#define ZKMOCKS_H_

#include "zk_client.h"
#include <zookeeper.h>

namespace libzk{

class WatcherAction {
public:
    WatcherAction():triggered_(false){}
    virtual ~WatcherAction(){}
    
    virtual void onSessionExpired(zhandle_t*){}
    virtual void onConnectionEstablished(zhandle_t*){}
    virtual void onConnectionLost(zhandle_t*){}
    virtual void onNodeValueChanged(zhandle_t*,const char* path){}
    virtual void onNodeDeleted(zhandle_t*,const char* path){}
    virtual void onChildChanged(zhandle_t*,const char* path){}
    
    void setWatcherTriggered(){
        triggered_=true;
    }

protected:
    bool triggered_;
};
// zh->context is a pointer to a WatcherAction instance
// based on the event type and state, the watcher calls a specific watcher 
// action method
void activeWatcher(zhandle_t *zh, int type, int state, const char *path,void* ctx);

// *****************************************************************************
// a set of async completion signatures
class AsyncCompletion{
public:
    virtual ~AsyncCompletion(){}
    virtual void aclCompl(int rc, ACL_vector *acl,Stat *stat){}
    virtual void dataCompl(int rc, const char *value, int len, const Stat *stat){}
    virtual void statCompl(int rc, const Stat *stat){}
    virtual void stringCompl(int rc, const char *value){}
    virtual void stringsCompl(int rc,const String_vector *strings){}
    virtual void voidCompl(int rc){}
};
void asyncCompletion(int rc, ACL_vector *acl,Stat *stat, const void *data);
void asyncCompletion(int rc, const char *value, int len, const Stat *stat, 
        const void *data);
void asyncCompletion(int rc, const Stat *stat, const void *data);
void asyncCompletion(int rc, const char *value, const void *data);
void asyncCompletion(int rc,const String_vector *strings, const void *data);
void asyncCompletion(int rc, const void *data);
}
#endif /*ZKMOCKS_H_*/
