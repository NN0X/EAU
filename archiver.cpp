#include "archiver.h"

File::File(std::string name, std::string extension) : name(name), extension(extension) {}

Archive::Archive(std::string path, std::string name) : path(path), name(name) {}

void Archive::addFile(File file)
{
    files.push_back(file);
}