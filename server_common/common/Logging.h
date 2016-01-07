#ifndef COMMON_LOGGING_H
#define COMMON_LOGGING_H

#include <syslog.h>
#include <syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>


#if __WORDSIZE == 64
#define PRI64 "l"
#else
#define PRI64 "ll"
#endif

class Logging {
public:
  static void setLogWithThread(bool b);

  static void setLogWithClassName(bool b);

  static void setLogLevel(int level);

  static void log(int level, const char *clz, const char *func, const char *fmt, ...);
};

inline const char *getClassName() { return ""; }

#define DECLARE_CLASS_NAME(Name) static const char *getClassName() { return #Name; }
#define DECLARE_NONCOPYABLE(Name) private: Name(const Name &)

#define DEBUG(...)    Logging::log(LOG_DEBUG, getClassName(), __FUNCTION__, __VA_ARGS__)
#define INFO(...)     Logging::log(LOG_INFO, getClassName(), __FUNCTION__, __VA_ARGS__)
#define NOTICE(...)   Logging::log(LOG_NOTICE, getClassName(), __FUNCTION__, __VA_ARGS__)
#define WARN(...)     Logging::log(LOG_WARNING, getClassName(), __FUNCTION__, __VA_ARGS__)
#define ERROR(...)    Logging::log(LOG_ERR, getClassName(), __FUNCTION__, __VA_ARGS__)
#define FATAL(...)    { Logging::log(LOG_CRIT, getClassName(), __FUNCTION__, __VA_ARGS__); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); exit(-1); }

#endif  //COMMON_LOGGING_H
