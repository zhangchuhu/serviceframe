#pragma once
#include "core/sox/logger.h"
#ifndef WIN32
#include <pthread.h>
#endif

#ifdef __CLASS__
    #define FUNLOG(level, fmt, ...)   log(level, "[%s::%s]: " fmt, __CLASS__, __FUNCTION__, __VA_ARGS__)
#else
    #define FUNLOG(level, fmt, ...)   log(level, "[%s]: " fmt, __FUNCTION__, __VA_ARGS__)
#endif

