#include "entry.h"
#include<vector>

//entries to be modified are assumed to be fetched by id
void update_title(Entry& en, const std::string& new_title);
void update_authors(Entry& en, const std::string& new_authors);
void update_notes(Entry& en, const std::string& new_notes);
void update_filename(Entry& en, const std::string& new_filename);
void change_dir(Entry& en, int new_dir_id);

void add_entry(std::vector<Entry>& catalog,
        const std::vector<std::string>& directories,
        int dir_id, const std::string& filename);
void delete_entry(std::vector<Entry>& catalog, int id_to_del);
