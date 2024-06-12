#include "archive.h"

File::File(const std::string &name, const std::string &extension)
{
    mName = name;
    mExtension = extension;

    if (!load())
    {
        return;
    }

    mSizeBits = mData.size() * 8;

    std::cout << "File '" << mName << "." << mExtension << "' loaded with size " << mSizeBits << " bits\n";
}

bool File::load()
{
    std::string filename = mName + "." + mExtension;
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return false;
    }

    file.seekg(0, std::ios::end);

    mData.resize(file.tellg());
    file.read(&mData[0], mData.size());
    file.close();

    return true;
}

bool File::save()
{
    std::string filename = mName + "." + mExtension;
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return false;
    }

    file.write(&mData[0], mData.size());
    file.close();

    return true;
}

void File::print(bool hex)
{
    std::cout << "File: " << mName << "." << mExtension << "\n";
    std::cout << "Size: " << mSizeBits << " bits\n";

    std::cout << "Data:\n";
    std::cout << "------------------------------------------\n";
    if (hex)
    {
        for (char byte : mData)
        {
            std::cout << std::hex << int(byte) << " ";
        }
    }
    else
    {
        for (char byte : mData)
        {
            std::cout << byte;
        }
    }
    std::cout << "\n";
    std::cout << "------------------------------------------\n";
}