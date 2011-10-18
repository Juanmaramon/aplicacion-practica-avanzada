// Class to obtain data of resource files

#ifndef FILE_UTILS_H
#define FILE_UTILS_H
#include <string>

class cFileUtils
{
	public:
		static std::string GetDirectory(const std::string lacFile);
		static std::string GetFilename(const std::string lacFile);
};

#endif