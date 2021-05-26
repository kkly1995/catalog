#include "catalog.h"
#include "update.h"
#include "directories.h"
#include "parser.h"
#include<fstream>
#include<iostream>
#include<filesystem>

// this header contains the names of the log files
// see example for how to set it up
#include "names.h"

namespace fs = std::filesystem;

const int fail = -1; // currently used by is_in_directories()

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "no input!\n";
        return 1;
    }
    Mode m = parse_option(argv[1]);
    if (m == Mode::fail) return 1;

    std::vector<Entry> catalog;
    std::vector<std::string> directories;
    if (!read_log(catalog_name, catalog)) return 1;
    if (!read_directories(directories_name, directories)) return 1;
    
    // begin
    std::cout << "-----\n";

    switch (m) {
        case Mode::check_log:
            std::cout << "checking log file...\n\n";
            check_log(catalog, directories);
            break;
        case Mode::list_titles:
            std::cout << "catalog:\n";
            print_titles(catalog);
            break;
        case Mode::list_directories:
            std::cout << "directories:\n";
            print_directories(directories);
            break;
        case Mode::add_entry:
        {
            std::string dir = fs::current_path().string();
            int dir_id = is_in_directories(dir, directories);
            if (dir_id == fail) return 1;
            if (argc == 2) {
                std::cerr << "no filenames input!\n";
                return 1;
            }
            for (int i = 2; i < argc; i++) {
                add_entry(catalog, directories, dir_id, argv[i]);
            }
            write_log(catalog_name, catalog);
            break;
        }
        case Mode::add_directory:
        {
            std::string dir = fs::current_path().string();
            if (argc > 2) {
                std::cerr << "this adds the current working directory "
                        << "and takes no inputs. exiting because an input "
                        << "was detected\n";
                return 1;
            }
            add_directory(directories, dir);
            write_directories(directories_name, directories);
            break;
        }
        case Mode::ID:
            if (argc == 2) {
                std::cerr << "no ids input!\n";
                return 1;
            }
            for (int i = 2; i < argc; i++) {
                int id = std::stoi(argv[i]); // should catch exception here
                print_entry(catalog.at(id), directories);
                std::cout << std::endl; // separate multiple entries
            }
            break;
        case Mode::update_title:
            if (argc != 4) {
                std::cerr << "input not recognized. should be [id] [new title]. "
                        << "if it has multiple words, enclose them in quotes\n";
                return 1;
            }
            {
                int id = std::stoi(argv[2]); //should catch exception here
                update_title(catalog.at(id), argv[3]);
                write_log(catalog_name, catalog);
            }
            break;
        case Mode::update_author:
            if (argc != 4) {
                std::cerr << "input not recognized. should be [id] [new authors]. "
                        << "if it has multiple words, enclose them in quotes\n";
                return 1;
            }
            {
                int id = std::stoi(argv[2]); //should catch exception here
                update_authors(catalog.at(id), argv[3]);
                write_log(catalog_name, catalog);
            }
            break;
        case Mode::update_notes:
            if (argc != 4) {
                std::cerr << "input not recognized. should be [id] [new notes]. "
                        << "if it has multiple words, enclose them in quotes\n";
                return 1;
            }
            {
                int id = std::stoi(argv[2]); //should catch exception here
                update_notes(catalog.at(id), argv[3]);
                write_log(catalog_name, catalog);
            }
            break;
        case Mode::update_filename:
            if (argc != 4) {
                std::cerr << "input not recognized. should be [id] [new filename]\n";
                return 1;
            }
            {
                int id = std::stoi(argv[2]); //should catch exception here
                update_filename(catalog.at(id), argv[3]);
                write_log(catalog_name, catalog);
            }
            break;
        case Mode::change_directory:
            if (argc != 4) {
                std::cerr << "input not recognized. should be [id] [new dir_id]\n";
                return 1;
            }
            {
                int id = std::stoi(argv[2]); //should catch exception here
                int dir_id = std::stoi(argv[3]);
                change_dir(catalog.at(id), dir_id);
                write_log(catalog_name, catalog);
            }
            break;
        case Mode::search_title:
            if (argc != 3) {
                std::cerr << "input not recognized. should be [keyphrase]. "
                        << "if it has multiple words, enclose them in quotes\n";
                return 1;
            }
            lookup_title(argv[2], catalog, directories);
            break;
        case Mode::search_author:
            if (argc != 3) {
                std::cerr << "input not recognized. should be [keyphrase]. "
                        << "if it has multiple words, enclose them in quotes\n";
                return 1;
            }
            lookup_author(argv[2], catalog, directories);
            break;
        case Mode::search_notes:
            if (argc != 3) {
                std::cerr << "input not recognized. should be [keyphrase]. "
                        << "if it has multiple words, enclose them in quotes\n";
                return 1;
            }
            lookup_notes(argv[2], catalog, directories);
            break;
        case Mode::delete_entry:
        {
            if (argc == 2) {
                std::cerr << "no ids input!\n";
                return 1;
            }
            if (argc > 3) {
                std::cerr << "removing multiple ids "
                        << "is not currently supported\n";
                return 1;
            }
            int id = std::stoi(argv[2]); //should catch exception here
            delete_entry(catalog, id);
            write_log(catalog_name, catalog);
            break;
        }
        case Mode::delete_directory:
        {
            if (argc == 2) {
                std::cerr << "no dir_ids input!\n";
                return 1;
            }
            if (argc > 3) {
                std::cerr << "removing multiple ids "
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
    
    // end
    std::cout << "-----\n";
    return 0;
}
