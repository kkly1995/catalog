#ifndef DIRECTORIES_H
#define DIRECTORIES_H

#include<string>
#include<vector>

void create_directories(std::string fname);
bool read_directories(std::string fname, std::vector<std::string>& directories);
void write_directories(std::string fname,
                    const std::vector<std::string>& directories);

void print_directories(const std::vector<std::string>& directories);
void update_directory(std::string& directory, const std::string& new_dir);
void add_directory(std::vector<std::string>& directories,
                const std::string& new_dir);
void delete_directory(std::vector<std::string>& directories,
                    int dir_id_to_del);

int is_in_directories(const std::string& dir,
                    const std::vector<std::string>& directories);

#endif