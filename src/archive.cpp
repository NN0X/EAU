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
    mMetadata.clear();

    // <--continue here (gen offsets + sizes)
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

    int sizeOfMetadataBytes;
    file.read((char *)(&sizeOfMetadataBytes), sizeof(int));

    std::vector<char> metadata(sizeOfMetadataBytes);
    file.read(&metadata[0], sizeOfMetadataBytes);

    for (int i = 0; i < sizeOfMetadataBytes;)
    {
        int sizeOfFilenameBytes = metadata[i]; // <--temp
        i += sizeof(int);

        std::string filename(sizeOfFilenameBytes, '\0');
        for (int j = 0; j < sizeOfFilenameBytes; ++j)
        {
            filename[j] = metadata[i++];
        }

        int sizeOfFileBytes = metadata[i]; // <--temp
        i += sizeof(int);

        int offset = metadata[i]; // <--temp
        i += sizeof(int);

        mMetadata.mFiles.push_back(filename);
        mMetadata.mSizes[filename] = sizeOfFileBytes;
        mMetadata.mOffsets[filename] = offset;
    }

    return true;
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
