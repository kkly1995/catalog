#include "directories.h"
#include<fstream>
#include<iostream>

//for local_time()
#include "catalog.h"

void create_directories(std::string fname)
{
    //check first if file exists
    std::ifstream infile{fname};
    if (infile) {
        //file was successfully read, i.e. it exists
        std::cout << "tried to create " << fname
                << " but the file already exists\n";
        return;
    }

    //create the log
    std::ofstream outfile{fname};
    outfile << "Directories last modified: " << local_time();
}

bool read_directories(std::string fname, std::vector<std::string>& directories)
{
    std::ifstream infile{fname};
    if (!infile) {
        std::cout << "failed to read " << fname << std::endl;
        return false;
    }

    //check first line
    std::string marker;
    infile >> marker;
    if (!infile || marker != "Directories") {
        std::cout << "failed to read first line of " << fname
                << ", expected header\n";
        return false;
    }
    //seems ok, move past the remainder of the line
    std::getline(infile, marker);

    for (std::string dir; infile >> dir;) {
        directories.push_back(dir);
    }
    //success!
    return true;
}

void write_directories(std::string fname,
                    const std::vector<std::string>& directories)
{
    std::ofstream outfile{fname};
    outfile << "Directories last modified: " << local_time() << std::endl;

    for (std::string dir : directories) {
        outfile << dir << std::endl;
    }
}

void print_directories(const std::vector<std::string>& directories)
{
    for (int i = 0; i < directories.size(); i++) {
        std::cout << i << ". " << directories[i] << std::endl;
    }
}

void update_directory(std::string& directory, const std::string& new_dir)
{
    std::cout << "[old] " << directory << std::endl;
    directory = new_dir;
    std::cout << "[new] " << directory << std::endl
            << "[directory changed]\n";
}

void add_directory(std::vector<std::string>& directories,
                const std::string& new_dir)
{
    directories.push_back(new_dir);
    std::cout << "added new directory " << new_dir
            << " (dir_id = " << directories.size() - 1 << ")\n";
}

void delete_directory(std::vector<std::string>& directories,
                    int dir_id_to_del)
{
    std::cout << "removing directory " << directories[dir_id_to_del]
            << std::endl;
    directories.erase(directories.begin()+dir_id_to_del);
}

int is_in_directories(const std::string& dir,
                    const std::vector<std::string>& directories)
{
    for (int i = 0; i < directories.size(); i++) {
        if (dir == directories[i]) return i;
    }
    std::cout << dir << " was not found in available directories\n";
    return -1;
}