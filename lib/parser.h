#ifndef PARSER_H
#define ENTRY_H

#include<string>

enum class Mode {
    check_log,
    list_titles,
    list_directories,
    add_entry,
    add_directory,
    ID,
    update_title,
    update_author,
    update_notes,
    update_filename,
    change_directory,
    search_title,
    search_author,
    search_notes,
    delete_entry,
    delete_directory,
    fail
};

Mode parse_option(const std::string& arg);

#endif
