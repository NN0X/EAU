#include "archive.h"

int main(int argc, char **argv)
{
    std::string path;
    std::string name;
    if (argc == 3)
    {
        path = argv[1];
        name = argv[2];
    }
    else if (argc == 2)
    {
        path = "./";
        name = argv[1];
    }
    else
    {
        std::cerr << "Error: Invalid number of arguments\n";
        std::cout << "Usage: " << argv[0] << " [path] [name]\n";
        return 1;
    }

    Archive archive(path, name);
}