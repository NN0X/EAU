#include "archive.h"

Metadata::Metadata()
    : mSizeBytes(4), mLastFilename("")
{
    mData.resize(mSizeBytes);
    appendBytes(mData, convertToBytes<int>(mSizeBytes));
}

Metadata::Metadata(std::vector<char> data)
    : mData(data)
{
    mSizeBytes = mData.size();
    mData.resize(mSizeBytes);
    appendBytes(mData, convertToBytes<int>(mSizeBytes));

    int pos = 4;
    mSizeBytes = mData.size();

    int filenameSize;
    std::string filename;
    int fileSize;
    int offset;
    while (pos < mSizeBytes)
    {
        mMetadataOffsets[mLastFilename] = pos;

        filenameSize = mData[pos] | (mData[pos + 1] << 8) | (mData[pos + 2] << 16) | (mData[pos + 3] << 24);
        pos += 4;

        filename = std::string(mData.begin() + pos, mData.begin() + pos + filenameSize);
        pos += filenameSize;

        fileSize = mData[pos] | (mData[pos + 1] << 8) | (mData[pos + 2] << 16) | (mData[pos + 3] << 24);
        pos += 4;

        offset = mData[pos] | (mData[pos + 1] << 8) | (mData[pos + 2] << 16) | (mData[pos + 3] << 24);
        pos += 4;
        mOffsets[filename] = offset;

        mLastFilename = filename;
    }
}

void Metadata::addFile(const std::string &filename, int size)
{
    mMetadataOffsets[mLastFilename] = mSizeBytes;
    if (mOffsets.empty())
        mOffsets[filename] = 0;
    else
        mOffsets[filename] = mOffsets[mLastFilename] + size;
    mSizeBytes += 4 + filename.size() + 4 + 4;
    mData.resize(mSizeBytes);

    appendBytes(mData, convertToBytes<int>(filename.size()));                // size of filename
    appendBytes(mData, std::vector<char>(filename.begin(), filename.end())); // filename
    appendBytes(mData, convertToBytes<int>(size));                           // size of file
    appendBytes(mData, convertToBytes<int>(mOffsets[filename]));             // offset of file

    mLastFilename = filename;
}

void Metadata::removeFile(const std::string &filename)
{
}