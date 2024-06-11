#include "archive.h"

Archive::Archive(const std::string &path, const std::string &archiveName)
{
    mPath = path;
    mName = archiveName;
}

Archive::~Archive()
{
    for (auto &file : mLoadedFiles)
    {
        delete file.second;
    }

    mLoadedFiles.clear();

    std::cout << "Archive '" << mName << "' unloaded\n";
}

void Archive::generateMetadata()
{
}

bool Archive::loadMetadata()
{
    std::string filename = mPath + mName + ARCHIVE_EXTENSION;
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return false;
    }

    file.seekg(0, std::ios::end);
    mSizeBits = file.tellg() * 8;
    file.seekg(0, std::ios::beg);

    int sizeOfMetadataBytes;
    file.read(reinterpret_cast<char *>(&sizeOfMetadataBytes), sizeof(int));

    std::vector<unsigned char> metadata(sizeOfMetadataBytes);
    file.read(reinterpret_cast<char *>(&metadata[0]), sizeOfMetadataBytes);

    // <---- continue here

    return true;
}
