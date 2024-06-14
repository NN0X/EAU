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

void Archive::loadMetadata()
{
}

void Archive::save()
{
    Archive::generateMetadata();

    std::string filename = mPath + mName + ARCHIVE_EXTENSION;
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return;
    }

    file.write((char *)(&mMetadata.mSizeBytes), sizeof(int));
    file.write(&mMetadata.mData[0], mMetadata.mSizeBytes);

    // for (std::string &filename : mMetadata.mFiles)
    // {
    //     Archive::loadFile(filename);
    //     <--continue here
    //     Archive::unloadFile(filename);
    // }

    std::cout << "Archive '" << mName << "' saved\n";
    file.close();
}

void Archive::printFile(const std::string &filename, bool hex)
{
    if (mLoadedFiles.find(filename) == mLoadedFiles.end())
    {
        Archive::loadFile(filename);
    }

    File *file = mLoadedFiles[filename];

    std::cout << "File: " << file->mName << "." << file->mExtension << "\n";
    std::cout << "Size: " << file->mSizeBits << " bits\n";

    std::cout << "Data:\n";
    std::cout << "------------------------------------------\n";
    file->print(hex);
    std::cout << "\n";
    std::cout << "------------------------------------------\n";
}
