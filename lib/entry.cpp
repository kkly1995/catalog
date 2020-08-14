#include "entry.h"
#include<iostream>

//used for ltrim
#include<algorithm>
#include<cctype>
#include<locale>

static inline void ltrim(std::string& s)
    /* trim s in place starting from the left
     * copied from the stackoverflow thread at
     * https://stackoverflow.com/q/216823
     * */
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch){
        return !std::isspace(ch);
    }));
}

std::ostream& operator<<(std::ostream& os, const Entry& en)
{
    return os << "(id) " << en.id << std::endl
            << "(authors) " << en.authors << std::endl
            << "(title) " << en.title << std::endl
            << "(notes) " << en.notes << std::endl
            << "(dir_id) " << en.dir_id << std::endl
            << "(filename) " << en.filename << std::endl;
}

std::istream& operator>>(std::istream& is, Entry& en)
    //could potentially be improved using string streams?
{
    std::string marker;
    int id;
    is >> marker >> id;
    if (!is || marker != "(id)") return is;

    std::string authors, title, notes;

    is >> marker;
    if (!is || marker != "(authors)") {
        std::cout << "failed to read authors for id" << id << std::endl;
        return is;
    }
    std::getline(is, authors);
    ltrim(authors);

    is >> marker;
    if (!is || marker != "(title)") {
        std::cout << "failed to read title for id" << id << std::endl;
        return is;
    }
    std::getline(is, title);
    ltrim(title);

    is >> marker;
    if (!is || marker != "(notes)") {
        std::cout << "failed to read notes for id" << id << std::endl;
        return is;
    }
    std::getline(is, notes);
    ltrim(notes);

    int dir_id;
    is >> marker >> dir_id;
    if (!is || marker != "(dir_id)") {
        std::cout << "failed to read dir_id for id" << id << std::endl;
        return is;
    }

    std::string filename;
    is >> marker;
    if (!is || marker != "(filename)") {
        std::cout << "failed to read filename for id" << id << std::endl;
        return is;
    }
    std::getline(is, filename);
    ltrim(filename);

    //successful read?
    en.id = id;
    en.dir_id = dir_id;
    en.authors = authors;
    en.title = title;
    en.notes = notes;
    en.filename = filename;
    return is;
}

void print_entry(const Entry& en, const std::vector<std::string>& directories)
    /* separate from << since this is intended as a 'pretty print'
     * whereas << is intended for writing to catalog
     * notably this provides the full path to the file targeted by entry
     * */
{
    std::cout << "[" << en.id << "]\n"
            << en.authors << std::endl
            << en.title << std::endl
            << directories[en.dir_id] << "/"
            << en.filename << std::endl;
}
