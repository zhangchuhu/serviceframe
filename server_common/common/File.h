#ifndef FILE_IO_H
#define FILE_IO_H

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdarg.h>
#include <string>
#include <errno.h>
#include "Logging.h"

class Path {
  char buf_[PATH_MAX];

public:
  const char *format(const char *fmt, ...);

  const char *getName() const { return buf_; }

  static const char *getShortName(const char *name);
};  //Path

class File {
  DECLARE_CLASS_NAME(File)

  int fd_;
  std::string name_;

public:
  File(): fd_(-1) {}

  ~File();

  int close();

  int getDescriptor() const { return fd_; }

  int create(const char *filename);

  int open(const char *filename);

  int append(const char *filename);

  int turncate(const char *filename);
  
  int write(const char *data, size_t size);

  int read(char *data, size_t size);

  const char *getName() const { return name_.c_str(); }

  int sync();

  int seek(off_t where);
};  //File

int loadFile(const char *path, std::string *buffer);

int saveFile(const char *path, const char *data, size_t size);

class FileLock {
public:
	FileLock();

	~FileLock();

	bool lock(const char *path);

private:
	FileLock(const FileLock &);	//non-copyable

private:
	int fd_;
};

FileLock *lockFile(const char *path);

#endif
