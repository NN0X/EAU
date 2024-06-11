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
    std::vector<unsigned char> mData;
    int mSizeBits;

    File(const std::string &name, const std::string &extension);

    bool load();
    bool save();

    void print(bool hex = false);
};

class Archive
{
private:
    std::string mPath;
    std::vector<std::string> mFiles;
    std::unordered_map<std::string, int> mSizes;
    std::unordered_map<std::string, File *> mLoadedFiles;

    std::vector<unsigned char> mMetadata;

public:
    std::string mName;
    int mSizeBits;

    Archive(const std::string &path, const std::string &archiveName);
    ~Archive();

    void save();

    /*---METADATA STRUCTURE---
    4 bytes: size of metadata in bytes
    for each file:
        4 bytes: size of filename in bytes
        n bytes: filename
        4 bytes: size of file in bytes
        4 bytes: index of first byte of the file in archive
    --------------------------*/
    void generateMetadata();
    bool loadMetadata();

    bool loadFile(const std::string &filename);
    void unloadFile(const std::string &filename);

    void addFile(const std::string &filename);
    void removeFile(const std::string &filename);

    void extractFile(const std::string &filename);
    void extractAllFiles();

    void listFiles();
};

#endif