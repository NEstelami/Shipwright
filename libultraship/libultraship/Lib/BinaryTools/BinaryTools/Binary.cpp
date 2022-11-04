#include "Binary.h"
#include <fstream>

Span<char> ReadAllBytes(const std::string& filePath)
{
    std::ifstream file(filePath, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file!"); //Todo: Note file name/path in error. Maybe better to just return an optional
    }

    size_t fileSize = (size_t)file.tellg();
    char* buffer = new char[fileSize];

    file.seekg(0);
    file.read(buffer, fileSize);
    file.close();

    

    return Span<char>(buffer, fileSize);
}

bool FileExists(const std::string& filePath)
{
    std::ifstream file(filePath, std::ios::in | std::ios::binary | std::ios::ate);
    return file.good();
}