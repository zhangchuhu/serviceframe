#pragma once
#include <string>
#include <stdarg.h>

void NotifyAll(const char *szFmt, ...);
    
void NotifyAll(const std::string &str);
    
void NotifyOne(uint32_t uUid, const char *szFmt, ...);
    
void NotifyOne(uint32_t uUid, const std::string &str);

