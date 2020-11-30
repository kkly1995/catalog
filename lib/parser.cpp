#include "parser.h"
#include<iostream>

Mode parse_option(const std::string& arg)
{
    if (arg == "-ch") return Mode::check_log;
    if (arg == "-lt") return Mode::list_titles;
    if (arg == "-ld") return Mode::list_directories;
    if (arg == "-ae") return Mode::add_entry;
    if (arg == "-ad") return Mode::add_directory;
    if (arg == "-id") return Mode::ID;
    if (arg == "-ut") return Mode::update_title;
    if (arg == "-ua") return Mode::update_author;
    if (arg == "-un") return Mode::update_notes;
    if (arg == "-uf") return Mode::update_filename;
    if (arg == "-cd") return Mode::change_directory;
    if (arg == "-st") return Mode::search_title;
    if (arg == "-sa") return Mode::search_author;
    if (arg == "-sn") return Mode::search_notes;
    if (arg == "-de") return Mode::delete_entry;
    if (arg == "-dd") return Mode::delete_directory;
    // didn't find any valid input
    std::cout << "input not recognized!\n";
    return Mode::fail;
}
