#include "archive.h"

File::File(const std::string &name, const std::string &extension, const std::vector<char> &data)
    : mName(name), mExtension(extension), mData(data)
{
    mSizeBits = mData.size() * 8;
}

bool File::save(int offset)
{
    std::string filename = mName + "." + mExtension;
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return false;
    }

    file.seekp(offset);
    file.write(&mData[0], mData.size());
    file.close();

    return true;
}

void File::print(bool hex)
{
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
}