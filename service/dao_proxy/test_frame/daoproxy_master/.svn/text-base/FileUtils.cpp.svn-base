#include "FileUtils.h"
#include <fstream>

using namespace std;


int gl_write_to_file(std::string file_path, const std::string& content)
{
	ofstream fs(file_path.c_str(), ios_base::out|ios_base::binary|ios_base::trunc);
	if(!fs)
	{
		return -1;
	}
	if (!fs.is_open())
	{
		fs.close();
		return -1;
	}
	fs.write(content.c_str(), content.size());
	if (fs.fail())
	{
		fs.close();
		return -1;
	}
	fs.close();
	return 0;
}
int gl_read_from_file(std::string file_path, std::string& content)
{
	ifstream fs(file_path.c_str(), fstream::in | fstream::binary);
	if(!fs)
	{
		return -1;
	}
	if (!fs.is_open())
	{
		fs.close();
		return -1;
	}

	content.clear();
	char buffer[1024];
	for(;;)
	{
		unsigned long ret=fs.tellg();
		fs.read(buffer,1024);
		if(fs.eof())
		{
			fs.clear();
		}
		else if(fs.fail())
		{
			return -2;
		}
		ret=(unsigned long)fs.tellg()-ret;
		content.append(buffer,buffer+ret);
		if(ret!=1024)
		{
			break;
		}
	}
	fs.close();
	return 0;
}



