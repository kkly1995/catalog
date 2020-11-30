#ifndef CATALOG_H
#define CATALOG_H

#include "entry.h"
#include<vector>
//there may be better options than using a vector here

std::string local_time();

bool read_log(std::string fname, std::vector<Entry>& catalog);
void check_log(const std::vector<Entry>& catalog,
        const std::vector<std::string>& directories);
void lookup_title(std::string keyphrase, const std::vector<Entry>& catalog,
                const std::vector<std::string>& directories);
void lookup_author(std::string keyphrase, const std::vector<Entry>& catalog,
                const std::vector<std::string>& directories);
void lookup_notes(std::string keyphrase, const std::vector<Entry>& catalog,
                const std::vector<std::string>& directories);
void print_titles(const std::vector<Entry>& catalog);
void write_log(std::string fname, const std::vector<Entry>& catalog);

#endif
