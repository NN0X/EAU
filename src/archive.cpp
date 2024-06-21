#include "archive.h"

std::vector<char> Byte::intToBytes(int value)
{
    int size = sizeof(int);
    std::vector<char> bytes(size);

    for (int i = 0; i < size; i++)
    {
        bytes[i] = (value >> (i * 8));
    }

    bytes = std::vector<char>(bytes.rbegin(), bytes.rend());

    return bytes;
}

std::vector<int> Byte::bytesToInts(const std::vector<char> &bytes)
{
    std::vector<int> values;
    for (size_t i = 0; i < bytes.size(); i += sizeof(int))
    {
        int value = 0;
        value = ((value | bytes[0]) << 8 | bytes[1]) << 8 | bytes[2] << 8 | (bytes[3] & 0xFF);
        values.push_back(value);
    }

    return values;
}

std::vector<bool> Byte::bytesToBits(const std::vector<char> &bytes)
{
    std::vector<bool> bits;
    for (size_t i = 0; i < bytes.size(); i++)
    {
        char byte = bytes[i];
        std::vector<bool> bitsOfByte(8);
        for (size_t j = 0; j < 8; j++)
        {
            bool bit = (byte >> j) & 1;
            bitsOfByte[j] = bit;
        }

        bitsOfByte = std::vector<bool>(bitsOfByte.rbegin(), bitsOfByte.rend());
        bits.insert(bits.end(), bitsOfByte.begin(), bitsOfByte.end());
    }

    return bits;
}

Archive::Archive(const std::string &path, const std::string &archiveName, bool exists)
    : mPath(path), mName(archiveName)
{
    if (exists)
        load();
}

Archive::~Archive()
{
    for (auto &file : mLoadedFiles)
    {
        unloadFile(file.first);
    }

    mLoadedFiles.clear();

    std::cout << "Archive '" << mName << "' unloaded\n";
}

void Archive::load()
{
    mMetadata.load(mPath + mName + ARCHIVE_EXTENSION);
}

void Archive::save()
{
    std::ofstream file(mPath + mName + ARCHIVE_EXTENSION, std::ios::binary);
    file.write(&mMetadata.mData[0], mMetadata.mSizeBytes);
    for (std::string filename : mMetadata.mFilenames)
    {
        loadFile(filename);
        file.seekp(mMetadata.getFileOffset(filename));
        file.write(mLoadedFiles[filename]->mData.data(), mLoadedFiles[filename]->mSizeBytes);
        unloadFile(filename);
    }

    file.close();
}

// void Archive::partitionFile(const std::string &filename)
// {
// }

void Archive::loadFile(const std::string &filename)
{
    if (mLoadedFiles.find(filename) != mLoadedFiles.end())
    {
        std::cerr << "Error: File '" << filename << "' already loaded\n";
        return;
    }

    int size = mMetadata.getFileSize(filename);
    if (size > FILE_PARTITION_SIZE)
    {
        // partitionFile(filename);
        return;
    }

    int offset = mMetadata.getFileOffset(filename);
    std::ifstream file(mPath + mName + ARCHIVE_EXTENSION, std::ios::binary);
    file.seekg(offset);
    std::vector<char> data(size);
    file.read(&data[0], size);
    file.close();

    mLoadedFiles[filename] = new File(filename, data);
}

void Archive::unloadFile(const std::string &filename)
{
    if (mLoadedFiles.find(filename) == mLoadedFiles.end())
    {
        std::cerr << "Error: File '" << filename << "' not loaded\n";
        return;
    }

    delete mLoadedFiles[filename];
    mLoadedFiles.erase(filename);
}

void Archive::addFile(const std::string &filename)
{
    std::ifstream file(mPath + filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return;
    }

    file.seekg(0, std::ios::end);
    int size = file.tellg();
    file.close();

    mMetadata.addFile(filename, size);
    save();
}

void Archive::removeFile(const std::string &filename)
{
    mMetadata.removeFile(filename);
    save();

    if (mLoadedFiles.find(filename) != mLoadedFiles.end())
    {
        unloadFile(filename);
    }
}

void Archive::extractFile(const std::string &filename)
{
    std::ofstream file(mPath + filename, std::ios::binary);
    loadFile(filename);
    file.write(mLoadedFiles[filename]->mData.data(), mLoadedFiles[filename]->mSizeBytes);
    file.close();
}

void Archive::extractAll()
{
    for (std::string filename : mMetadata.mFilenames)
    {
        extractFile(filename);
    }
}

void Archive::listFiles()
{
    for (std::string filename : mMetadata.mFilenames)
    {
        std::cout << filename << "\n";
    }
}

void Archive::printFile(const std::string &filename, int mode)
{
    if (mLoadedFiles.find(filename) == mLoadedFiles.end())
    {
        loadFile(filename);
    }

    mLoadedFiles[filename]->print(mode);
}
