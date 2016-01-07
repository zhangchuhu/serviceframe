#include "File.h"

//Path
const char *Path::format(const char *fmt, ...) {
    va_list va;
    va_start(va, fmt);
    vsnprintf(buf_, sizeof(buf_), fmt, va);
    va_end(va);
    return buf_;
}

const char *Path::getShortName(const char *filename) {
    int pos = strlen(filename) - 1;
    for (; pos >=0; --pos) {
        if (*(filename + pos) == '/')
            break;
    }
    return filename + pos + 1;
}

//File
File::~File() {
    if (fd_ >= 0) {
        ::close(fd_);
    }
}

int File::close() {
	if (fd_ >= 0) {
		if (::close(fd_) == 0) {
			fd_ = -1;
			return 0;
		} else {
			fd_ = -1;
			return -errno;
		}
	}

	return -EINVAL;
}

int File::seek(off_t where) {
    off_t rt = lseek(fd_, where, SEEK_SET);
    if (rt == (off_t) -1) {
        return -errno;
    } else {
        return 0;
    }
}

int File::append(const char *filename) {
    if ((fd_ = ::open(filename, O_CREAT|O_RDWR|O_APPEND, 0644)) >= 0) {
        name_ = filename;
        return 0;
    }

    return -errno;
}

int File::create(const char *filename) {
  if ((fd_ = ::open(filename, O_CREAT|O_RDWR, 0644)) >= 0) {
    name_ = filename;
    return 0;
  }

  return -errno;
}

int File::sync() {
    if (fsync(fd_) != 0) {
        return -errno;
    } else {
        return 0;
    }
}

int File::open(const char *filename) {
  if ((fd_ = ::open(filename, O_RDONLY, 0644)) >= 0) {
    name_ = filename;
    return 0;
  }

  return -errno;
}

int File::write(const char *data, size_t size) {
	size_t done = 0;
	do {
		int n = ::write(fd_, data + done, size - done);
		if (n >= 0) {
			done += n;
		} else {
			switch (errno) {
				case EINTR:
				case EAGAIN:
				case EINPROGRESS:
					break;
				default:
          WARN("read %s failed: %s", getName(), strerror(errno));
					return -errno;
			}
		}
	} while (done < size);
	return 0;
}

int File::read(char *data, size_t size) {
	int n;
  do {
		n = ::read(fd_, data, size);
		if (n >= 0) {
			return n;
		} else {
			switch (errno) {
				case EINTR:
				case EAGAIN:                                                    
				case EINPROGRESS:       
					break;
				default:
          WARN("write %s failed: %s", getName(), strerror(errno));
					return -errno;
			}  
		}
	} while (1);
	return 0;
}

int loadFile(const char *filename, std::string *buffer) {
    Path path;
	File file;
    int err;
    const char *ptr = filename;

    if ((err = file.open(ptr)) < 0) {
        //try to load from old backup file
        ptr = path.format("%s.bak", filename);
        if (errno == ENOENT) {
			if ((err = file.open(ptr)) < 0) {
				return err; //should handle ENOENT outside;
			}
		}
	}

	char buf[4096];
	while (1) {
		int n = file.read(buf, sizeof(buf));
		if (n > 0) {
			buffer->append(buf, n);
		} else if (n == 0) {
			break;
		} else {
			WARN("read %s failed: %s", filename, strerror(errno));
			return n;
		}
	}

	return 0;
}

int saveFile(const char *filename, const char *data, size_t size) {
	Path path;
    path.format("%s.bak", filename);

	if (::access(filename, F_OK) == 0) {
		if (::rename(filename, path.getName()) != 0) {
			ERROR("rename %s to %s failed: %s", filename, path.getName(), strerror(errno));
			return -errno;
		}
	}

    int err;
    File file;

    if ((err = file.create(filename)) < 0) {
		WARN("create %s failed: %s", filename, strerror(errno));
		return err;
	}
	if ((err = file.write(data, size)) < 0) {
	  return err;
    }
	return 0;
}


//FileLock
FileLock::FileLock(): fd_(-1) {}

FileLock::~FileLock() {
	if (fd_ >= 0)
		::close(fd_);
}

bool FileLock::lock(const char *path) {
	fd_ = ::open(path, O_RDWR | O_CREAT | O_TRUNC, 0600);
	if (fd_ < 0)
		return false;

	struct flock f;
	memset(&f, 0, sizeof(f));
	f.l_type = F_WRLCK;
	f.l_whence = SEEK_SET;
	f.l_start = 0;
	f.l_len = 0;        // Lock/unlock entire file

	if (fcntl(fd_, F_SETLK, &f) == 0) {
		return true;
	} else {
		return false;
	}
}

FileLock *lockFile(const char *path) {
	FileLock *l = new FileLock();
	if (l->lock(path)) {
		return l;
	} else {
		delete l;
		return NULL;
	}
}

