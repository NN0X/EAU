#include "archive.h"

#define NAME "main.exe"
#define VERSION "1.0"
#define LICENSE "See NOTICE.md file for license information"
#define AUTHOR "NoX"

void helpArchive()
{
    std::cout << "Commands:\n";
    std::cout << "\tadd [filename]\n";
    std::cout << "\tremove [filename]\n";
    std::cout << "\tlist\n";
    std::cout << "\tsize [filename]\n";
    std::cout << "\tsize all\n";
    std::cout << "\tadd [filename]\n";
    std::cout << "\tremove [filename]\n";
    std::cout << "\tprint [filename]\n";
    std::cout << "\thex [filename]\n";
    std::cout << "\tbits [filename]\n";
    std::cout << "\textract [filename]\n";
    std::cout << "\textract all\n";
    std::cout << "\tsave\n";
    std::cout << "\texit\n";
}

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

void infoCommands(std::string command)
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
        std::cout << "version: " << VERSION << "\n";
        exit(0);
    }
    else if (command == "--license")
    {
        std::cout << LICENSE << "\n";
        exit(0);
    }
    else if (command == "--author")
    {
        std::cout << "author: " << AUTHOR << "\n";
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
        infoCommands(std::string(argv[1]));
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

    std::ifstream file(path + name + ARCHIVE_EXTENSION, std::ios::binary);
    bool exists = file.good();
    file.close();

    Archive archive(path, name, exists);
    std::string command;
    std::cout << "Enter a command (type 'help' for a list of commands):\n";
    while (true)
    {
        std::cout << "> ";
        std::cin >> command;
        if (command == "help")
        {
            helpArchive();
        }
        else if (command == "add")
        {
            std::string filename;
            std::cin >> filename;
            archive.addFile(filename);
        }
        else if (command == "remove")
        {
            std::string filename;
            std::cin >> filename;
            archive.removeFile(filename);
        }
        else if (command == "list")
        {
            archive.listFiles();
        }
        else if (command == "size")
        {
            std::string filename;
            std::cin >> filename;
            if (filename == "all")
                std::cout << "Size: " << archive.mSizeBytes << " bytes\n";
            else
                std::cout << "Size: " << archive.mMetadata.getFileSize(filename) << " bytes\n";
        }
        else if (command == "add")
        {
            std::string filename;
            std::cin >> filename;
            archive.addFile(filename);
        }
        else if (command == "remove")
        {
            std::string filename;
            std::cin >> filename;
            archive.removeFile(filename);
        }
        else if (command == "print")
        {
            std::string filename;
            std::cin >> filename;
            archive.printFile(filename);
        }
        else if (command == "hex")
        {
            std::string filename;
            std::cin >> filename;
            archive.printFile(filename, HEX);
        }
        else if (command == "bits")
        {
            std::string filename;
            std::cin >> filename;
            archive.printFile(filename, BITS);
        }
        else if (command == "extract")
        {
            std::string filename;
            std::cin >> filename;
            if (filename == "all")
                archive.extractAll();
            else
                archive.extractFile(filename);
        }
        else if (command == "clear")
        {
            std::cout << "\033[2J\033[1;1H";
        }
        else if (command == "exit")
        {
            break;
        }
        else if (command == "erase")
        {
            std::remove((path + name + ARCHIVE_EXTENSION).c_str());
            break;
        }
        else
        {
            std::cerr << "Error: Invalid command\n";
        }
    }
}