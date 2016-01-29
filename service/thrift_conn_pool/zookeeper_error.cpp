#include "zookeeper_error.h"
#include <cstring>
#include <zookeeper.h>

ZooException::ZooException(int code)
: std::runtime_error(zerror(code)), zoo_error_code_(code) {
}

ZooException::ZooException(int code, const char* what)
: std::runtime_error(what), zoo_error_code_(code) {
}