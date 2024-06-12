#include "archive.h"

#define NAME "main.exe"
#define VERSION "1.0"
#define LICENSE "See NOTICE.md file for license information"
#define AUTHOR "NoX"

void help()
{
    std::cout << "Usage: " << NAME << " [path] [name]\n";
    std::cout << "Create or open an archive with the given name in the given path\n";
    std::cout << "If no path is given, the current directory is used\n";

    std::cout << "\n";
    std::cout << " --help\n";
    std::cout << " --examples\n";
    std::cout << " --version\n";
    std::cout << " --license\n";
    std::cout << " --author\n";
}

void commands(std::string command)
{
    if (command == "--help")
    {
        help();
        exit(0);
    }
    else if (command == "--examples")
    {
        std::cout << "Example usage:\n";
        std::cout << NAME << " /data archive\n";
        std::cout << "Creates or opens an archive named 'archive' in the /data directory\n\n";
        std::cout << NAME << " archive\n";
        std::cout << "Creates or opens an archive named 'archive' in the current directory\n";
        exit(0);
    }
    else if (command == "--version")
    {
        std::cout << "Version: " << VERSION << "\n";
        exit(0);
    }
    else if (command == "--license")
    {
        std::cout << LICENSE << "\n";
        exit(0);
    }
    else if (command == "--author")
    {
        std::cout << "Author: " << AUTHOR << "\n";
        exit(0);
    }
}

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
        commands(std::string(argv[1]));
        path = "./";
        name = argv[1];
    }
    else
    {
        std::cerr << "Error: Invalid number of arguments\n";
        std::cout << "Example usage: " << NAME << " [path] [name]\n";
        std::cout << "Use " << NAME << " --help for more information\n";
        return 1;
    }

    Archive archive(path, name);
}