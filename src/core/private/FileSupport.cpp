#include "public/FileSupport.h"
#include <fstream>

void FileFromString (const std::string& filepath, std::string& refStringPath)
{
    std::ifstream file(filepath, std::ios::in | std::ios::binary);
    if(!file)
    {
        throw std::runtime_error("can't open file : " + filepath);
    }

    std::ostringstream filestring;
    filestring << file.rdbuf();
    if(!filestring)
    {
        throw std::runtime_error("can't string stream file : " + filestring.str());
    }

    std::string str = filestring.str();
    refStringPath = str;
}
