// Copyright 2024 NoX

#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <unordered_map>
#include <algorithm>

#define ARCHIVE_EXTENSION ".eau"
#define FILE_PARTITION_SIZE 1000000000 // 1 GB

enum PrintMode
{
    ASCII,
    HEX,
    BITS
};

namespace Vector
{
    template <typename T>
    void append(std::vector<T> &data, const std::vector<T> &dataToAppend)
    {
        data.insert(data.end(), dataToAppend.begin(), dataToAppend.end());
    }

    template <typename T>
    void replace(std::vector<T> &data, const std::vector<T> &newData, int index)
    {
        data.erase(data.begin() + index, data.begin() + index + newData.size());
        data.insert(data.begin() + index, newData.begin(), newData.end());
    }
}

namespace Byte
{
    std::vector<char> intToBytes(int value);
    std::vector<int> bytesToInts(const std::vector<char> &bytes);
    std::vector<bool> bytesToBits(const std::vector<char> &bytes);
}

struct File
{
    std::string mName;
    std::vector<char> mData;
    int mSizeBytes;

    File(const std::string &name, const std::vector<char> &data);

    void save(const std::string &path);

    void print(int mode = 0);
};

/*---METADATA STRUCTURE---
    4 bytes: size of metadata in bytes (including this size field)
    for each file:
        4 bytes: size of filename in bytes
        n bytes: filename
        4 bytes: size of file in bytes
        4 bytes: index of first byte of the file in archive (offset from the end of metadata)
    --------------------------*/
class Metadata
{
private:
    std::unordered_map<std::string, int> mSizes;
    std::unordered_map<std::string, int> mOffsets;
    std::unordered_map<std::string, int> mMetadataOffsets;

public:
    std::vector<std::string> mFilenames;
    int mSizeBytes;
    std::vector<char> mData;

    Metadata();

    void load(const std::string &path);
    void toData();
    void serialize();

    void addFile(const std::string &filename, int size);
    void removeFile(const std::string &filename);

    int getFileSize(const std::string &filename);
    int getFileOffset(const std::string &filename);
};

/*---ARCHIVE STRUCTURE---
n bytes: metadata
for each file:
    n bytes: file data
--------------------------
*/
class Archive
{
private:
    std::vector<char> mData;
    std::unordered_map<std::string, File *> mLoadedFiles;

public:
    Metadata mMetadata;
    std::string mPath;
    std::string mName;
    int mSizeBytes;

    Archive(const std::string &path, const std::string &archiveName, bool exists = false);
    ~Archive();

    void load();
    void save();

    // void partitionFile(const std::string &filename);

    void loadOutsideFile(const std::string &filename);
    void loadArchiveFile(const std::string &filename);
    void unloadFile(const std::string &filename);

    void addFile(const std::string &filename);
    void removeFile(const std::string &filename);

    void extractFile(const std::string &filename);
    void extractAll();

    void listFiles();

    void printFile(const std::string &filename, int mode = 0);
};

#endif