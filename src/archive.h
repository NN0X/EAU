// Copyright 2024 NoX

#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <unordered_map>

#define ARCHIVE_EXTENSION ".eau"

struct File
{
    std::string mName;
    std::string mExtension;
    std::vector<char> mData;
    int mSizeBits;

    File(const std::string &name, const std::string &extension);

    bool load();
    bool save();

    void print(bool hex = false);
};

struct Metadata
{
    int mSizeBytes;
    std::vector<std::string> mFiles;
    std::unordered_map<std::string, int> mSizes;
    std::unordered_map<std::string, int> mOffsets;

    std::vector<char> mData;

    void generate();
    void clear();
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
    Metadata mMetadata;
    std::unordered_map<std::string, File *> mLoadedFiles;

public:
    std::string mName;
    int mSizeBits;

    Archive(const std::string &path, const std::string &archiveName);
    ~Archive();

    /*---METADATA STRUCTURE---
    4 bytes: size of metadata in bytes (including this size field)
    for each file:
        4 bytes: size of filename in bytes
        n bytes: filename
        4 bytes: size of file in bytes
        4 bytes: index of first byte of the file in archive (offset from the end of metadata)
    --------------------------*/
    void generateMetadata();
    bool loadMetadata();

    void save();

    bool loadFile(const std::string &filename);
    void unloadFile(const std::string &filename);

    void addFile(const std::string &filename);
    void removeFile(const std::string &filename);

    void extractFile(const std::string &filename);
    void extractAllFiles();

    void listFiles();
};

#endif