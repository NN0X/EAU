/*
Copyright 2024 NoX

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

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

struct Metadata
{
    int mSizeBytes;
    std::vector<std::string> mFiles;
    std::unordered_map<std::string, int> mSizes;
    std::unordered_map<std::string, int> mOffsets;

    std::vector<unsigned char> mData;

    void generate();
    void clear();
};

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
        4 bytes: index of first byte of the file in archive (offset)
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