#ifndef ENTRY_H
#define ENTRY_H

#include<string>
#include<vector>

struct Entry {
    int id, dir_id;
    std::string filename, title, authors, notes;
    //default values of id and dir_id; the metadata can be empty strings
    Entry() :id{-1}, dir_id{-1} {}
};

std::ostream& operator<<(std::ostream& os, const Entry& en);
std::istream& operator>>(std::istream& is, Entry& en);
void print_entry(const Entry& en, const std::vector<std::string>& directories);

#endif
