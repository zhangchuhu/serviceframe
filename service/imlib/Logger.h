#ifndef MEMBER_FUNC_LOGGER_H
#define MEMBER_FUNC_LOGGER_H

#include "core/sox/logger.h"
#include <boost/thread/thread.hpp>

#undef __CLASS__
#define __CLASS__ ""

#define SYSLOG(level, fmt, ...)   log(level, "[%s::%s]: " fmt, __CLASS__, __FUNCTION__, __VA_ARGS__)

#endif
