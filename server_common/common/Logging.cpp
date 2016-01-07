#include "Logging.h"
#include <stdarg.h>

static int gettid() {
  return (int)syscall(SYS_gettid);
}

static bool _logWithThreadId = false;
static bool _logWithClassName = true;
static int _logLevel = LOG_INFO;

void Logging::setLogWithThread(bool b) {
  _logWithThreadId = b;
}

void Logging::setLogWithClassName(bool b) {
  _logWithClassName = b;
}

void Logging::setLogLevel(int level) {
  _logLevel = level;
}

void Logging::log(int level, const char *clz, const char *func, const char *fmt, ...) {
	if (level > _logLevel)
		return;

    va_list va;
    char buf[1024];
    int len = 0;

    if (_logWithThreadId) {
      len += snprintf(buf + len, sizeof(buf) - len, "[tid:%d] ", gettid());
    }
    if (_logWithClassName) {
      len += snprintf(buf + len, sizeof(buf) - len, "[%s::%s] ", clz, func); 
    } else {
      len += snprintf(buf + len, sizeof(buf) - len, "[%s] ", func);
    }
    
    va_start(va, fmt);
    len += vsnprintf(buf + len, sizeof(buf) - len,  fmt, va);
    va_end(va);
    syslog(level, "%s", buf);
}

