#include "archive.h"

void Metadata::generate()
{
    for (std::string filename : mFiles)
    {
        mData.push_back(filename.size());
        for (char c : filename)
            mData.push_back(c);
        mData.push_back(mSizes[filename]);
        mData.push_back(mOffsets[filename]);
    }

    mSizeBytes = mData.size() + sizeof(int);
    mData.insert(mData.begin(), (char *)&mSizeBytes, (char *)&mSizeBytes + sizeof(int));
}

void Metadata::clear()
{
    mSizeBytes = 0;
    mFiles.clear();
    mSizes.clear();
    mOffsets.clear();

    mData.clear();
}