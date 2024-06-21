#include "archive.h"

File::File(const std::string &name, const std::vector<char> &data)
    : mName(name), mData(data), mSizeBytes(data.size())
{
}

void File::save(const std::string &path)
{
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << path << "\n";
        return;
    }

    file.seekp(0);
    file.write(&mData[0], mData.size());
    file.close();
}

void File::print(int mode)
{
    std::vector<bool> bits;
    std::cout << "File '" << mName << "'\n";
    std::cout << "Size: " << mSizeBytes << " bytes\n";
    std::cout << "--------------------------------\n";
    switch (mode)
    {
    case ASCII:
        for (size_t i = 0; i < mData.size(); i++)
        {
            std::cout << mData[i];
        }
        break;
    case HEX:
        for (size_t i = 0; i < mData.size(); i++)
        {
            std::cout << std::hex << (int)mData[i] << " ";
        }
        break;
    case BITS:
        bits = Byte::bytesToBits(mData);
        for (size_t i = 0; i < mData.size(); i++)
        {
            std::cout << bits[i];
            if ((i + 1) % 8 == 0)
            {
                std::cout << " ";
            }
        }
        break;
    default:
        std::cerr << "Error: Invalid print mode\n";
        break;
    }
    std::cout << "\n--------------------------------\n";
}