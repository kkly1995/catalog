#include "catalog.h"
#include<fstream>
#include<iostream>
#include<ctime>

std::string local_time()
    //example copied from cplusplus reference
{
    std::time_t rawtime;
    struct tm * timeinfo;
    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);
    return std::asctime(timeinfo);
}

std::string tolower(std::string s)
    //derived from s11.6 in Stroustrup
{
    std::string lowered{s};
    for (char& x : lowered) x = std::tolower(x);
    return lowered;
}

void create_log(std::string fname)
{
    //check first if log exists
    std::ifstream infile{fname};
    if (infile) {
        //file was successfully read, i.e. it exists
        std::cout << "tried to create " << fname
                << " but the file already exists\n";
        return;
    }

    //create the log
    std::ofstream outfile{fname};
    outfile << "Last modified: " << local_time();
}

bool read_log(std::string fname, std::vector<Entry>& catalog)
{
    std::ifstream infile{fname};
    if (!infile) {
        std::cout << "failed to read " << fname << std::endl;
        return false;
    }

    //check first line
    std::string marker, time;
    infile >> marker;
    if (!infile || marker != "Last") {
        std::cout << "failed to read first line of " << fname
                << ", expected header\n";
        return false;
    }
    //seems ok, move on
    std::getline(infile, time);

    //before reading into catalog, make sure catalog is empty (?)
    if (catalog.size() > 0) {
        std::cout << "attempted to read log into nonempty catalog\n";
        return false;
    }

    for (Entry en; infile >> en;) {
        catalog.push_back(en);
    }
    //success!
    return true;
}

void validate(const std::vector<Entry>& catalog, int num_dir)
    //ideally i think this should also verify that the files exist
{
    for (int i = 0; i < catalog.size(); i++) {
        if (i != catalog[i].id) {
            std::cout << "invalid id encountered, expected " << i << std::endl
                    << "entry in question:\n" << catalog[i] << std::endl;
            return;
        }
        if (catalog[i].dir_id >= num_dir) {
            std::cout << "out of range directory id encountered, "
                        << "there should only be " << num_dir
                        << " directories\n"
                        << "entry in question:\n" << catalog[i] << std::endl;
            return;
        }
    }
    //ids seem to be ok
    std::cout << "catalog verified!\n"
            << "number of entries: " << catalog.size() << std::endl
            << "number of directories: " << num_dir << std::endl;
}

void lookup_title(std::string keyphrase, const std::vector<Entry>& catalog,
                const std::vector<std::string>& directories)
{
    std::vector<Entry> search_results;
    for (Entry en : catalog) {
        if (tolower(en.title).find(tolower(keyphrase)) != std::string::npos) {
            search_results.push_back(en);
        }
    }

    //print search results
    std::cout << "search results for \"" << keyphrase << "\" by title\n\n";
    for (Entry en : search_results) {
        print_entry(en, directories);
        std::cout << std::endl;
    }
    std::cout << search_results.size() << " results\n";
}

void lookup_author(std::string keyphrase, const std::vector<Entry>& catalog,
                const std::vector<std::string>& directories)
{
    std::vector<Entry> search_results;
    for (Entry en : catalog) {
        if (tolower(en.authors).find(tolower(keyphrase)) != std::string::npos) {
            search_results.push_back(en);
        }
    }

    //print search results
    std::cout << "search results for \"" << keyphrase << "\" by author\n\n";
    for (Entry en : search_results) {
        print_entry(en, directories);
        std::cout << std::endl;
    }
    std::cout << search_results.size() << " results\n";
}

void lookup_notes(std::string keyphrase, const std::vector<Entry>& catalog,
                const std::vector<std::string>& directories)
{
    std::vector<Entry> search_results;
    for (Entry en : catalog) {
        if (tolower(en.notes).find(tolower(keyphrase)) != std::string::npos) {
            search_results.push_back(en);
        }
    }

    //print search results
    std::cout << "search results for \"" << keyphrase << "\" by notes\n\n";
    for (Entry en : search_results) {
        print_entry(en, directories);
        std::cout << std::endl;
    }
    std::cout << search_results.size() << " results\n";
}

void print_titles(const std::vector<Entry>& catalog)
{
    for (Entry en : catalog) {
        std::cout << en.id << ". " << en.title << std::endl;
    }
}

void write_log(std::string fname, const std::vector<Entry>& catalog)
    //just dump catalog into file
{
    std::ofstream outfile{fname};
    outfile << "Last modified: " << local_time() << std::endl;

    for (Entry en : catalog) {
        outfile << en << std::endl;
    }
}
