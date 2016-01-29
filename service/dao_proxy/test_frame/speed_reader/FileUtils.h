#ifndef _FILEUTILS_H_20111027
#define _FILEUTILS_H_20111027
#include <string>


int gl_write_to_file(std::string file_path, const std::string& content);
int gl_read_from_file(std::string file_path, std::string& content);


#endif	// _FILEUTILS_H_20111027

