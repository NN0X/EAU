#include "archive.h"

std::vector<char> Byte::intToBytes(int value)
{
    int size = sizeof(int);
    std::vector<char> bytes(size);

    for (size_t i = 0; i < size; i++)
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

Archive::Archive(const std::string &path, const std::string &archiveName)
{
    mPath = path;
    mName = archiveName;

    loadMetadata();
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

void Archive::save()
{
    std::string filename = mPath + mName + ARCHIVE_EXTENSION;
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return;
    }

    file.write((char *)(&mMetadata.mSizeBytes), sizeof(int));
    file.write(&mMetadata.mData[0], mMetadata.mSizeBytes);

    for (std::string &filename : mFiles)
    {
        std::string name = filename.substr(0, filename.find_last_of('.'));
        std::string extension = filename.substr(filename.find_last_of('.') + 1);
        Archive::loadFile(name, extension);
        file.write(mLoadedFiles[filename]->mData.data(), mLoadedFiles[filename]->mData.size());
        Archive::unloadFile(name, extension);
    }

    std::cout << "Archive '" << mName << "' saved\n";
    file.close();
}

// change to work for big files
void Archive::loadFile(const std::string &name, const std::string &extension)
{
    std::string filename = mPath + name + "." + extension;
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return;
    }

    file.seekg(0, std::ios::end);
    int size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> data(size);
    file.read(&data[0], size);

    File *fileObj = new File(name, extension, data);
    mLoadedFiles[name] = fileObj;

    file.close();
}

void Archive::unloadFile(const std::string &name, const std::string &extension)
{
    if (mLoadedFiles.find(name) == mLoadedFiles.end())
    {
        std::cerr << "Error: File " << name << "." << extension << " is not loaded\n";
        return;
    }

    delete mLoadedFiles[name];
    mLoadedFiles.erase(name);
}

void Archive::addFile(const std::string &name, const std::string &extension)
{
    std::ifstream file(mPath + name + "." + extension, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << name << "." << extension << "\n";
        return;
    }

    file.seekg(0, std::ios::end);
    int size = file.tellg();
    file.close();

    mFiles.push_back(name);
    mSizes[name] = size;
    mMetadata.addFile(name + "." + extension, size);
}

void Archive::removeFile(const std::string &name, const std::string &extension)
{
    if (mMetadata.mOffsets.find(name + "." + extension) == mMetadata.mOffsets.end())
    {
        std::cerr << "Error: File " << name << "." << extension << " is not in the archive\n";
        return;
    }

    // mMetadata.removeFile(name + "." + extension);
    for (size_t i = 0; i < mFiles.size(); i++)
    {
        if (mFiles[i] == name)
        {
            mFiles.erase(mFiles.begin() + i);
            break;
        }
    }
    mSizes.erase(name);

    std::string filename = mPath + name + "." + extension;
    if (mLoadedFiles.find(name) != mLoadedFiles.end())
    {
        delete mLoadedFiles[name];
        mLoadedFiles.erase(name);
    }
}

void Archive::extractFile(const std::string &name, const std::string &extension)
{
    if (mMetadata.mOffsets.find(name + "." + extension) == mMetadata.mOffsets.end())
    {
        std::cerr << "Error: File " << name << "." << extension << " is not in the archive\n";
        return;
    }

    Archive::loadFile(name, extension);
    mLoadedFiles[name]->save(mMetadata.mOffsets[name + "." + extension]);
    Archive::unloadFile(name, extension);
}

void Archive::extractAll()
{
    for (std::string &filename : mFiles)
    {
        std::string name = filename.substr(0, filename.find_last_of('.'));
        std::string extension = filename.substr(filename.find_last_of('.') + 1);
        Archive::extractFile(name, extension);
    }
}

void Archive::listFiles()
{
    std::cout << "Files in archive '" << mName << "':\n";
    for (std::string &filename : mFiles)
    {
        std::cout << filename << "\n";
    }
}

void Archive::printFile(const std::string &filename, bool hex)
{
    if (mLoadedFiles.find(filename) == mLoadedFiles.end())
    {
        std::string name = filename.substr(0, filename.find_last_of('.'));
        std::string extension = filename.substr(filename.find_last_of('.') + 1);
        Archive::loadFile(filename, extension);
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
