#ifndef STRING_UTIL_H
#define STRING_UTIL_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string>

inline std::string &operator<<(std::string &dst, uint32_t src) {
    char buf[32];
    snprintf(buf, sizeof(buf), "%u", src);
    dst.append(buf);
    return dst;
}

inline std::string &operator<<(std::string &dst, int64_t src) {
	char buf[32];
#if __WORDSIZE == 64
	snprintf(buf, sizeof(buf), "%ld", src);
#else
	snprintf(buf, sizeof(buf), "%lld", src);
#endif
	dst.append(buf);
	return dst;
}

inline std::string &operator<<(std::string &dst, uint64_t src) {
	char buf[32];
#if __WORDSIZE == 64
	snprintf(buf, sizeof(buf), "%lu", src);
#else
	snprintf(buf, sizeof(buf), "%llu", src);
#endif
	dst.append(buf);
	return dst;
}

inline std::string &operator<<(std::string &dst, const char *src) {
	dst.append(src);
	return dst;
}

inline std::string &operator<<(std::string &dst, const std::string &src) {
	dst.append(src);
	return dst;
}

inline char *strip(char *line) {
	int len = strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	if (len > 1 && line[len - 2] == '\r')
		line[len - 2] = '\0';
	return line;
}

inline bool startsWith(const char *target, const char *prefix) {
	int l1 = strlen(target);
	int l2 = strlen(prefix);

	if (l1 < l2) {
		return false;
	} else {
		return strncmp(target, prefix, l2) == 0;
	}
}

inline bool endsWith(const char *target, const char *suffix) {
	int l1 = strlen(target);
	int l2 = strlen(suffix);

	if (l1 < l2) {
		return false;
	} else {
		return strncmp(target + l1 - l2, suffix, l2) == 0;
	}
}

#endif	//STRING_UTIL_H
