// Copyright 2024 NoX

#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <unordered_map>

#define ARCHIVE_EXTENSION ".eau"

template <typename T>
std::vector<char> convertToBytes(T value)
{
    int size = sizeof(T);
    std::vector<char> bytes(size);
    for (int i = 0; i < size; i++)
    {
        bytes[i] = (value >> (i * 8)) & 0xFF;
    }
    return bytes;
}

void appendBytes(std::vector<char> &data, std::vector<char> bytes);

struct File
{
    std::string mName;
    std::string mExtension;
    std::vector<char> mData;
    int mSizeBits;

    File(const std::string &name, const std::string &extension, const std::vector<char> &data);

    void save(int offset);

    void print(bool hex = false);
};

/*---METADATA STRUCTURE---
    4 bytes: size of metadata in bytes (including this size field)
    for each file:
        4 bytes: size of filename in bytes
        n bytes: filename
        4 bytes: size of file in bytes
        4 bytes: index of first byte of the file in archive (offset from the end of metadata)
    --------------------------*/
struct Metadata
{
    int mSizeBytes;
    std::unordered_map<std::string, int> mOffsets;
    std::unordered_map<std::string, int> mMetadataOffsets;
    std::vector<char> mData;
    std::string mLastFilename;

    Metadata();
    Metadata(std::vector<char> data);

    void addFile(const std::string &filename, int size);
    // void removeFile(const std::string &filename);
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
    std::string mPath;
    std::vector<std::string> mFiles;
    std::unordered_map<std::string, int> mSizes;
    Metadata mMetadata;
    std::unordered_map<std::string, File *> mLoadedFiles;

public:
    std::string mName;
    int mSizeBits;

    Archive(const std::string &path, const std::string &archiveName);
    ~Archive();

    void loadMetadata();

    void save();

    void loadFile(const std::string &name, const std::string &extension);
    void unloadFile(const std::string &name, const std::string &extension);

    void addFile(const std::string &name, const std::string &extension);
    void removeFile(const std::string &name, const std::string &extension);

    void extractFile(const std::string &name, const std::string &extension);
    void extractAll();

    void listFiles();

    void printFile(const std::string &filename, bool hex = false);
};

#endif