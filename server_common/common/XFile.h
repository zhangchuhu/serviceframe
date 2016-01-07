#ifndef SERVER_COMMON___COMMON___FILE_H
#define SERVER_COMMON___COMMON___FILE_H
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <fstream>
#include <iostream>
//#include <string>
#include <string.h>

namespace server{ namespace xlinux{

	class File
	{
	public:
		File(const std::string& strFileName, std::ios_base::openmode mode = std::fstream::in | std::fstream::out /* | std::fstream::app*/)
			: m_strFileName(strFileName), m_mode(mode)
		{}

		~File()
		{
			m_fstream.close();
		}

		int ensureOpen()
		{
			if (!m_fstream.is_open())
			{
				m_fstream.close();
				m_fstream.open(m_strFileName.c_str(), m_mode);
				if (m_fstream.fail())
					return -1;
			}

			return 0;
		}

		/**
		* @return	
		0	success
		-1	open file fail
		*/
		int getFileSize(uint64_t& u64FileSize)
		{
			if (0 != ensureOpen())
				return -1;

			m_fstream.seekg(0, std::ios::end);
			u64FileSize = m_fstream.tellg();

			return 0;
		}

		/**
		* @return	
		1	strOut.size() < u64Counts
		0	success
		-1	open file fail
		-2	read fail
		*/
		int read(uint64_t u64FileOffset, uint64_t u64Counts, std::string& strOutBuffer)
		{
			if (strOutBuffer.capacity() < u64Counts)
				return 1;

			if (0 != ensureOpen())
				return -1;

			char* buffer = new char[u64Counts];
			memset(buffer, 0, sizeof(char) * u64Counts);
			m_fstream.seekg(u64FileOffset, std::ios::beg);
			if (m_fstream.fail())
			{
				m_fstream.close();
                                delete [] buffer;
                                return -2;
			}

			m_fstream.read(buffer, u64Counts);
			if (m_fstream.fail())
			{
				m_fstream.close();
				delete [] buffer;
				return -2;
			}
			strOutBuffer.assign(buffer, u64Counts);
			delete [] buffer;
			return 0;
		}

		/**
		* @return	
		1	strContent.size() > u64MaxCounts
		0	success
		-1	open file fail
		-2	write fail
		*/
		int write(uint64_t u64FileOffset, uint64_t u64MaxCounts, const std::string& strContent)
		{
			if (strContent.size() > u64MaxCounts)
				return 1;

			if (0 != ensureOpen())
				return -1;

			m_fstream.seekg (u64FileOffset, std::ios::beg);
			m_fstream.write(strContent.c_str(), strContent.size());
			if (m_fstream.fail())
			{
				return -2;
			}

			m_fstream.flush();
			if (m_fstream.fail())
			{
				return -2;
			}

			return 0;
		}

	private:
		std::fstream m_fstream;
		std::string m_strFileName;
		std::ios_base::openmode m_mode;
	};

}}


#endif	// SERVER_COMMON___COMMON___FILE_H

