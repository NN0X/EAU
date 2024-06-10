#ifndef ARCHIVER_H
#define ARCHIVER_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

struct File
{
    std::string name;
    std::string extension;
    int sizeBits;
    std::string data;
    std::string metadata;

    File(std::string name, std::string extension);
};

class Archive
{
private:
    std::string path;
    std::string name;

    std::vector<File> files;

public:
    Archive(std::string path, std::string name);

    void addFile(File file);
    void removeFile(std::string name);
    void listFiles();
    void extractFile(std::string name);
    void extractAll();

    void save();
    void load();

    void appendMetadata(std::string metadata);
    void resetMetadata();

    // void compress();
    // void decompress();
};

#endif