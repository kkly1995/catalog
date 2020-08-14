#include "catalog.h"
#include "update.h"
#include "directories.h"
#include<fstream>
#include<iostream>
#include<filesystem>

namespace fs = std::filesystem;

const std::string catalog_name{"/Users/valletta/catalog/example/cata.log"};
const std::string directories_name{"/Users/valletta/catalog/example/dirs.log"};
const int fail = -1;

int parse_option(const std::string& arg);

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "no input!\n";
        return 1;
    }
    int mode = parse_option(argv[1]);
    if (mode == fail) return 1;

    std::vector<Entry> catalog;
    std::vector<std::string> directories;
    if (!read_log(catalog_name, catalog)) return 1;
    if (!read_directories(directories_name, directories)) return 1;

    switch (mode) {
        case 0:
            std::cout << "catalog:\n";
            print_titles(catalog);
            break;
        case 1:
            std::cout << "directories:\n";
            print_directories(directories);
            break;
        case 2:
        {
            std::string dir = fs::current_path().string();
            int dir_id = is_in_directories(dir, directories);
            if (dir_id == fail) return 1;
            if (argc == 2) {
                std::cout << "no filenames input!\n";
                return 1;
            }
            for (int i = 2; i < argc; i++) {
                std::cout << "adding " << directories[dir_id]
                        << "/" << argv[i] << " (id = " << catalog.size()
                        << ")\n";
                Entry new_entry;
                new_entry.id = catalog.size();
                new_entry.dir_id = dir_id;
                new_entry.filename = argv[i];
                catalog.push_back(new_entry);
            }
            write_log(catalog_name, catalog);
            break;
        }
        case 3:
        {
            std::string dir = fs::current_path().string();
            if (argc > 2) {
                std::cout << "this adds the current working directory "
                        << "and takes no inputs. exiting because an input "
                        << "was detected\n";
                return 1;
            }
            add_directory(directories, dir);
            write_directories(directories_name, directories);
            break;
        }
        case 4:
            if (argc == 2) {
                std::cout << "no ids input!\n";
                return 1;
            }
            for (int i = 2; i < argc; i++) {
                int id = std::stoi(argv[i]); //should catch exception here
                print_entry(catalog[id], directories);
                std::cout << std::endl; //separate multiple entries
            }
            break;
        case 5:
            if (argc != 4) {
                std::cout << "input not recognized. should be [id] [new title]. "
                        << "if it has multiple words, enclose them in quotes\n";
                return 1;
            }
            {
                int id = std::stoi(argv[2]); //should catch exception here
                update_title(catalog[id], argv[3]);
                write_log(catalog_name, catalog);
            }
            break;
        case 6:
            if (argc != 4) {
                std::cout << "input not recognized. should be [id] [new authors]. "
                        << "if it has multiple words, enclose them in quotes\n";
                return 1;
            }
            {
                int id = std::stoi(argv[2]); //should catch exception here
                update_authors(catalog[id], argv[3]);
                write_log(catalog_name, catalog);
            }
            break;
        case 7:
            if (argc != 4) {
                std::cout << "input not recognized. should be [id] [new notes]. "
                        << "if it has multiple words, enclose them in quotes\n";
                return 1;
            }
            {
                int id = std::stoi(argv[2]); //should catch exception here
                update_notes(catalog[id], argv[3]);
                write_log(catalog_name, catalog);
            }
            break;
        case 8:
            if (argc != 4) {
                std::cout << "input not recognized. should be [id] [new filename]\n";
                return 1;
            }
            {
                int id = std::stoi(argv[2]); //should catch exception here
                update_filename(catalog[id], argv[3]);
                write_log(catalog_name, catalog);
            }
            break;
        case 9:
            if (argc != 4) {
                std::cout << "input not recognized. should be [id] [new dir_id]\n";
                return 1;
            }
            {
                int id = std::stoi(argv[2]); //should catch exception here
                int dir_id = std::stoi(argv[3]);
                change_dir(catalog[id], dir_id);
                write_log(catalog_name, catalog);
            }
            break;
        case 10:
            if (argc != 3) {
                std::cout << "input not recognized. should be [keyphrase]. "
                        << "if it has multiple words, enclose them in quotes\n";
                return 1;
            }
            lookup_title(argv[2], catalog, directories);
            break;
        case 11:
            if (argc != 3) {
                std::cout << "input not recognized. should be [keyphrase]. "
                        << "if it has multiple words, enclose them in quotes\n";
                return 1;
            }
            lookup_author(argv[2], catalog, directories);
            break;
        case 12:
            if (argc != 3) {
                std::cout << "input not recognized. should be [keyphrase]. "
                        << "if it has multiple words, enclose them in quotes\n";
                return 1;
            }
            lookup_notes(argv[2], catalog, directories);
            break;
        case 13:
        {
            if (argc == 2) {
                std::cout << "no ids input!\n";
                return 1;
            }
            if (argc > 3) {
                std::cout << "removing multiple ids "
                        << "is not currently supported\n";
                return 1;
            }
            int id = std::stoi(argv[2]); //should catch exception here
            delete_entry(catalog, id);
            write_log(catalog_name, catalog);
            break;
        }
        case 14:
        {
            if (argc == 2) {
                std::cout << "no dir_ids input!\n";
                return 1;
            }
            if (argc > 3) {
                std::cout << "removing multiple ids "
                        << "is not currently supported\n";
                return 1;
            }
            int id = std::stoi(argv[2]); //should catch exception here
            delete_directory(directories, id);
            write_directories(directories_name, directories);
            break;
        }
        default:
            std::cout << "nothing done\n";
    }
}

int parse_option(const std::string& arg)
{
    if (arg == "-lt") return 0;
    if (arg == "-ld") return 1;
    if (arg == "-ae") return 2;
    if (arg == "-ad") return 3;
    if (arg == "-id") return 4;
    if (arg == "-ut") return 5;
    if (arg == "-ua") return 6;
    if (arg == "-un") return 7;
    if (arg == "-uf") return 8;
    if (arg == "-cd") return 9;
    if (arg == "-st") return 10;
    if (arg == "-sa") return 11;
    if (arg == "-sn") return 12;
    if (arg == "-de") return 13;
    if (arg == "-dd") return 14;
    //didn't find any valid input
    std::cout << "input not recognized!\n";
    return fail;
}
