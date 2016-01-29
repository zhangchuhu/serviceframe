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

#include <arpa/inet.h>  // for htonl
#include <memory>
#include <assert.h>

#include <zookeeper.h>
#include <proto.h>

#include "zk_watcher.h"

using namespace std;
using namespace libzk;

// *****************************************************************************
// watcher action implementation
// *****************************************************************************
// a set of async completion signatures
void asyncCompletion(int rc, ACL_vector *acl,Stat *stat, const void *data){
    assert("Completion data is NULL"&&data);
    static_cast<AsyncCompletion*>((void*)data)->aclCompl(rc,acl,stat);
}
void asyncCompletion(int rc, const char *value, int len, const Stat *stat, 
        const void *data){    
    assert("Completion data is NULL"&&data);
    static_cast<AsyncCompletion*>((void*)data)->dataCompl(rc,value,len,stat);    
}
void asyncCompletion(int rc, const Stat *stat, const void *data){    
    assert("Completion data is NULL"&&data);
    static_cast<AsyncCompletion*>((void*)data)->statCompl(rc,stat);
}
void asyncCompletion(int rc, const char *value, const void *data){
    assert("Completion data is NULL"&&data);
    static_cast<AsyncCompletion*>((void*)data)->stringCompl(rc,value);
}
void asyncCompletion(int rc,const String_vector *strings, const void *data){    
    assert("Completion data is NULL"&&data);
    static_cast<AsyncCompletion*>((void*)data)->stringsCompl(rc,strings);
}
void asyncCompletion(int rc, const void *data){    
    assert("Completion data is NULL"&&data);
    static_cast<AsyncCompletion*>((void*)data)->voidCompl(rc);
}

