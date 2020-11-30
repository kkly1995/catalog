#include "update.h"
#include<iostream>

void update_title(Entry& en, const std::string& new_title)
{
    std::cout << "[old]\n" << en << std::endl;
    en.title = new_title;
    std::cout << "[new]\n" << en << std::endl
            << "[title changed]\n";
}

void update_authors(Entry& en, const std::string& new_authors)
{
    std::cout << "[old]\n" << en << std::endl;
    en.authors = new_authors;
    std::cout << "[new]\n" << en << std::endl
            << "[authors changed]\n";
}

void update_notes(Entry& en, const std::string& new_notes)
{
    std::cout << "[old]\n" << en << std::endl;
    en.notes = new_notes;
    std::cout << "[new]\n" << en << std::endl
            << "[notes changed]\n";
}

void update_filename(Entry& en, const std::string& new_filename)
{
    std::cout << "[old]\n" << en << std::endl;
    en.filename = new_filename;
    std::cout << "[new]\n" << en << std::endl
            << "[filename changed]\n";
}

void change_dir(Entry& en, int new_dir_id)
    //eventually want print actual directory instead of dir_id
{
    std::cout << "entry " << en.id << " moved from "
            << en.dir_id << " to " << new_dir_id << std::endl;
    en.dir_id = new_dir_id;
}

void add_entry(std::vector<Entry>& catalog,
        const std::vector<std::string>& directories,
        int dir_id, const std::string& filename)
{
    Entry new_entry;
    new_entry.id = catalog.size();
    new_entry.dir_id = dir_id;
    new_entry.filename = filename;
    catalog.push_back(new_entry);
    std::cout << "added " << directories[dir_id]
            << "/" << filename << " (id = " << new_entry.id
            << ")\n";
}

void delete_entry(std::vector<Entry>& catalog, int id_to_del)
{
    std::cout << "removing entry\n" << catalog[id_to_del];
    catalog.erase(catalog.begin()+id_to_del);
    //shift all subsequent ids down
    for (int i = id_to_del; i < catalog.size(); i++) {
        catalog[i].id = i;
    }
}
