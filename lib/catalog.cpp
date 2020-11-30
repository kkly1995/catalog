#include "catalog.h"
#include<fstream>
#include<iostream>
#include<ctime>
#include<filesystem>

namespace fs = std::filesystem;

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

bool read_log(std::string fname, std::vector<Entry>& catalog)
{
    std::ifstream infile{fname};
    if (!infile) {
        std::cerr << "failed to read " << fname << std::endl;
        return false;
    }

    //check first line
    std::string marker, time;
    infile >> marker;
    if (!infile || marker != "Catalog") {
        std::cerr << "failed to read first line of " << fname
                << ", expected header\n";
        return false;
    }
    //seems ok, move on
    std::getline(infile, time);

    //before reading into catalog, make sure catalog is empty (?)
    if (catalog.size() > 0) {
        std::cerr << "attempted to read log into nonempty catalog\n";
        return false;
    }

    for (Entry en; infile >> en;) {
        catalog.push_back(en);
    }
    //success!
    return true;
}

void check_log(const std::vector<Entry>& catalog,
        const std::vector<std::string>& directories)
    // go through every entry and see if corresponding file exists
    // note: since directories may not contain the slash / at the end,
    // it is added here for the construction of fullname
{
    std::string fullname;
    int count = 0;
    for (const Entry& en : catalog) {
        fullname = directories[en.dir_id] + "/" + en.filename;
        if (!fs::exists(fullname)) {
            std::cout << "[" << en.id << "]\n";
            std::cout << fullname << " was not found\n\n";
            count += 1;
        }
    }
    if (count == 0) {
        std::cout << "all files were found!\n";
        return;
    }
    std::cout << count << " files were not found!\n";
    return;
}

void lookup_title(std::string keyphrase, const std::vector<Entry>& catalog,
                const std::vector<std::string>& directories)
{
    std::vector<Entry> search_results;
    for (const Entry& en : catalog) {
        if (tolower(en.title).find(tolower(keyphrase)) != std::string::npos) {
            search_results.push_back(en);
        }
    }

    //print search results
    std::cout << "search results for \"" << keyphrase << "\" by title\n\n";
    for (const Entry& en : search_results) {
        print_entry(en, directories);
        std::cout << std::endl;
    }
    std::cout << search_results.size() << " results\n";
}

void lookup_author(std::string keyphrase, const std::vector<Entry>& catalog,
                const std::vector<std::string>& directories)
{
    std::vector<Entry> search_results;
    for (const Entry& en : catalog) {
        if (tolower(en.authors).find(tolower(keyphrase)) != std::string::npos) {
            search_results.push_back(en);
        }
    }

    //print search results
    std::cout << "search results for \"" << keyphrase << "\" by author\n\n";
    for (const Entry& en : search_results) {
        print_entry(en, directories);
        std::cout << std::endl;
    }
    std::cout << search_results.size() << " results\n";
}

void lookup_notes(std::string keyphrase, const std::vector<Entry>& catalog,
                const std::vector<std::string>& directories)
{
    std::vector<Entry> search_results;
    for (const Entry& en : catalog) {
        if (tolower(en.notes).find(tolower(keyphrase)) != std::string::npos) {
            search_results.push_back(en);
        }
    }

    //print search results
    std::cout << "search results for \"" << keyphrase << "\" by notes\n\n";
    for (const Entry& en : search_results) {
        print_entry(en, directories);
        std::cout << std::endl;
    }
    std::cout << search_results.size() << " results\n";
}

void print_titles(const std::vector<Entry>& catalog)
{
    for (const Entry& en : catalog) {
        std::cout << en.id << ". " << en.title << std::endl;
    }
}

void write_log(std::string fname, const std::vector<Entry>& catalog)
    //just dump catalog into file
{
    std::ofstream outfile{fname};
    outfile << "Catalog last modified " << local_time() << std::endl;

    for (const Entry& en : catalog) {
        outfile << en << std::endl;
    }
}
