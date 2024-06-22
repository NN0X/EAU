#include "archive.h"

Metadata::Metadata()
    : mSizeBytes(4)
{
    mData = Byte::intToBytes(mSizeBytes);
}

void Metadata::load(const std::string &path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << path << "\n";
        return;
    }

    file.seekg(0);
    std::vector<char> sizeBytes(4);
    file.read(&sizeBytes[0], 4);
    mSizeBytes = Byte::bytesToInts(sizeBytes)[0];
    mData = std::vector<char>(mSizeBytes);
    file.seekg(0);
    file.read(&mData[0], mSizeBytes);

    file.close();

    serialize();
}

void Metadata::toData()
{
    mData = Byte::intToBytes(mSizeBytes);
    for (size_t i = 0; i < mFilenames.size(); i++)
    {
        std::vector<char> filenameBytes = Byte::intToBytes(mFilenames[i].size());
        filenameBytes.insert(filenameBytes.end(), mFilenames[i].begin(), mFilenames[i].end());
        std::vector<char> sizeBytes = Byte::intToBytes(mSizes[mFilenames[i]]);
        std::vector<char> offsetBytes = Byte::intToBytes(mOffsets[mFilenames[i]]);

        Vector::append(mData, filenameBytes);
        Vector::append(mData, sizeBytes);
        Vector::append(mData, offsetBytes);
    }

    mSizeBytes = mData.size();
    Vector::replace(mData, Byte::intToBytes(mSizeBytes), 0);

    serialize();
}

void Metadata::serialize()
{
    mFilenames.clear();
    mSizes.clear();
    mOffsets.clear();
    mMetadataOffsets.clear();

    int index = 4;
    while (index < mSizeBytes)
    {
        int filenameSize = Byte::bytesToInts(std::vector<char>(mData.begin() + index, mData.begin() + index + 4))[0];
        std::string filename(mData.begin() + index + 4, mData.begin() + index + 4 + filenameSize);
        mFilenames.push_back(filename);

        int size = Byte::bytesToInts(std::vector<char>(mData.begin() + index + 4 + filenameSize, mData.begin() + index + 8 + filenameSize))[0];
        mSizes[filename] = size;

        int offset = Byte::bytesToInts(std::vector<char>(mData.begin() + index + 8 + filenameSize, mData.begin() + index + 12 + filenameSize))[0];
        mOffsets[filename] = offset;

        mMetadataOffsets[filename] = index;

        index += 12 + filenameSize;
    }

    if (index != mSizeBytes)
    {
        std::cerr << "Error: Metadata size mismatch\n";
        std::cerr << "Expected: " << mSizeBytes << " bytes\n";
        std::cerr << "Actual: " << index << " bytes\n";
    }
}

// test
void Metadata::addFile(const std::string &filename, int size)
{
    if (mFilenames.size() == 0)
    {
        mOffsets[filename] = 0;
    }
    else
    {
        mOffsets[filename] = mOffsets[mFilenames.back()] + mSizes[mFilenames.back()];
    }
    mMetadataOffsets[filename] = mSizeBytes;
    mFilenames.push_back(filename);
    mSizes[filename] = size;

    std::vector<char> filenameBytes = Byte::intToBytes(filename.size());
    filenameBytes.insert(filenameBytes.end(), filename.begin(), filename.end());
    std::vector<char> sizeBytes = Byte::intToBytes(size);
    std::vector<char> offsetBytes = Byte::intToBytes(mOffsets[filename]);

    Vector::append(mData, filenameBytes);
    Vector::append(mData, sizeBytes);
    Vector::append(mData, offsetBytes);

    mSizeBytes += 12 + filename.size();

    Vector::replace(mData, Byte::intToBytes(mSizeBytes), 0);
}

// test
void Metadata::removeFile(const std::string &filename)
{
    if (mMetadataOffsets.find(filename) == mMetadataOffsets.end())
    {
        std::cerr << "Error: File " << filename << " is not in the metadata\n";
        return;
    }

    int index = mMetadataOffsets[filename];

    int filenameSize = Byte::bytesToInts(std::vector<char>(mData.begin() + index, mData.begin() + index + 4))[0];
    int size = Byte::bytesToInts(std::vector<char>(mData.begin() + index + 4 + filenameSize, mData.begin() + index + 8 + filenameSize))[0];

    mData.erase(mData.begin() + index, mData.begin() + index + 12 + filenameSize);
    mSizeBytes -= 12 + filenameSize;

    Vector::replace(mData, Byte::intToBytes(mSizeBytes), 0);

    for (auto &offset : mMetadataOffsets)
    {
        if (offset.second > index)
        {
            offset.second -= 12 + filenameSize;
        }
    }

    for (auto &offset : mOffsets)
    {
        if (offset.second > mOffsets[filename])
        {
            offset.second -= size;
        }
    }

    mFilenames.erase(std::find(mFilenames.begin(), mFilenames.end(), filename));
    mSizes.erase(filename);
    mOffsets.erase(filename);
    mMetadataOffsets.erase(filename);

    toData();
}

int Metadata::getFileSize(const std::string &filename)
{
    return mSizes[filename];
}

int Metadata::getFileOffset(const std::string &filename)
{
    return mOffsets[filename];
}