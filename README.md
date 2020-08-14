# catalog
 
## installation

At the moment setup is DIY. Two logs must be created: one which contains "entries" and one which contains directories. For example:
```
mkdir logs
touch logs/cata.log logs/dirs.log
echo Catalog >> logs/cata.log
echo Directories >> logs/dirs.log
```
The first word in the entries log file must be "Catalog," and the first word in the directories log file must be "Directories." The path to these must be included in main.cpp, in the lines
```
const std::string catalog_name{"/path/to/catalog/logs/cata.log"};
const std::string directories_name{"/path/to/catalog/logs/dirs.log"};
```
This means that the path to these log files is set at compile time. In order to move the logs elsewhere, the program must be recompiled.

C++17 is required because the program uses the filesystem namespace of the standard library. There are no other dependencies. For example, using g++:
```
mkdir bin
g++ -std=c++17 -o bin/bs lib/*cpp
```
Of course, you can call it anything you like. I like `bs`, which is short for bookshelf.

## example

Example log files and dummy papers/books are provided in the example dir. In order to use these log files, they must be known in main.cpp:
```
const std::string catalog_name{"/path/to/catalog/example/cata.log"};
const std::string directories_name{"/path/to/catalog/example/dirs.log"};
```
Then compile an example version with `g++ -std=c++17 -o bin/examplebs lib/*cpp`. For convenience `bin` should be added to your `PATH`.

First, look at `cata.log`. It is meant to be human-readable and can be manually modified. In fact, when adding many entries to it (as you might when you first begin), you will probably want to write in authors and titles by hand. Each field should occupy only one line, so long titles / author lists will appear ugly in a text editor. Each entry has a unique id, and these ids appear in increasing order. Currently this format for ids must be followed strictly; that is, if there are 4 entries, the last entry must have id 3 and the first must have id 0, etc. Adding entries via the program will follow this format.

Next, look at `dirs.log`. This should list the directories containing the files you want to catalog. Currently it is not checked that the directories even exist. However, adding directories via the program should only add real directories, as will be shown momentarily. The idea behind having this directories log is that you have a finite number of directories that you catalog, and if you ever want to move these directories (say to another machine), you just have to change the path in this log file, assuming you didn't move any files among the directories themselves.

If all is well, then `examplebs -sa alice` should yield
```
search results for "alice" by author

[0]
Bob and Alice
First paper
/path/to/catalog/example/papers/paper1

[3]
Alice
Fourth paper
/path/to/catalog/example/papers/paper4

2 results
```
This is the result of searching "alice" by authors. The search is case insensitive. The path is (probably) not real, unless you changed it in the directories log. Of course, for this to be of any use, you'd want to correct the path in `dirs.log`. Now, if I was looking for the paper by Bob and Alice, I can see where it is. For example, on macOS, I can just use the magic `open`:
```
open /path/to/catalog/example/papers/paper1
```
The number which appears above each result is the entry id. For example, `examplebs -id 0 3` should yield
```
[0]
Bob and Alice
First paper
/path/to/catalog/example/papers/paper1

[3]
Alice
Fourth paper
/path/to/catalog/example/papers/paper4
```
The id is used to update or delete entries. If I realize that Bob didn't actually author First paper, I can update
```
$ examplebs -ua 0 "Alice"
[old]
(id) 0
(authors) Bob and Alice
(title) First paper
(notes)
(dir_id) 0
(filename) paper1

[new]
(id) 0
(authors) Alice
(title) First paper
(notes)
(dir_id) 0
(filename) paper1

[authors changed]
```
Now, searching `examplebs -sa bob` yields no results. As previously mentioned, these entries can also be modified by hand, i.e. manually editing `cata.log`.

Navigate to the `example/books`. There are three empty files here, meant to represent books that you might want to include in your catalog. To add them:
```
$ examplebs -ae *
/home/kkly2/catalog/example/books was not found in available directories
```
This fails because I haven't added the current working directory to `dirs.log`. Note now that this is a real path on this particular machine. In order to add an entry, you must always be in same directory as the entry, and this directory must be known in `dirs.log`. The directory can be added with
```
$ examplebs -ad
added new directory /home/kkly2/catalog/example/books (dir_id = 1)
```
and now we can add these books
```
$ examplebs -ae *
adding /home/kkly2/catalog/example/books/book1 (id = 4)
adding /home/kkly2/catalog/example/books/book2 (id = 5)
adding /home/kkly2/catalog/example/books/book3 (id = 6)
```
If I decide I don't want book2 in there, I can remove it with
```
$ examplebs -de 5
removing entry
(id) 5
(authors)
(title)
(notes)
(dir_id) 1
(filename) book2
```
Currently only one entry can be removed at a time. Recall that the id numbers follow a strict order; doing this removal will shift all id numbers after the deleted entry down so as to maintain this order.

## manual

Here it is assumed the program is named `bs`.

`bs -lt`
List all titles in catalog, numbered by id

`bs -ld`
List all available directories, numbered by dir_id

`bs -ae [file1] [file2] ...`
Adds files to catalog. These files must be in the current working directory. Each filename should be a single "word"

`bs -ad`
Add current working directory to available directories

`bs -id [id1] [id2] ...`
Print entries associated with specified ids

`bs -ut [id] [new title]`
Change title of entry associated with specified id. If it has multiple words, enclose it in quotes

`bs -ua [id] [new authors]`
Change authors of entry associated with specified id. If it has multiple words, enclose it in quotes

`bs -un [id] [new notes]`
Change notes of entry associated with specified id. If it has multiple words, enclose it in quotes

`bs -uf [id] [new filename]`
Change filename of entry associated with specified id. Filename must be a single "word"

`bs -cd [id] [new dir_id]`
Change dir_id of entry associated with specified id

`bs -st [keyphrase]`
Search by title, case-insensitive. If keyphrase has multiple words, enclose it quotes

`bs -sa [keyphrase]`
Search by authors, case-insensitive. If keyphrase has multiple words, enclose it quotes

`bs -sn [keyphrase]`
Search by notes, case-insensitive. If keyphrase has multiple words, enclose it quotes

`bs -de [id]`
Delete entry associated with specified id. Deleting multiple entries is currently not supported

`bs -dd [dir_id]`
Remove directory associated with specified dir_id from available directories. Removing multiple directories is currently not supported
