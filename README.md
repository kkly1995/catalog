# catalog
 
## installation

All the program really does is read and write two particular log files: one which contains "entries," and another which contains a list of directories. The intended purpose is that each entry corresponds to a document of some sort, and contains at minimum the file name and location of the file. The reason for keeping the location of the file and the name of the file separate is to allow one to move files from one directory to another, as one might do when moving files from one machine to another. In this case, assuming one puts their files in a finite number of directories and does not change the names of the files, one only needs to to modify the directories log, and the catalog remains intact.

When starting anew, logs can be created like
```
mkdir logs
touch logs/cata.log logs/dirs.log
echo Catalog >> logs/cata.log
echo Directories >> logs/dirs.log
```
The first word in the entries log file must be "Catalog," and the first word in the directories log file must be "Directories." The location of the log files must be known at compile time. The way this is done is with a header `names.h`. An example is provided in `example/names.h`:
```
#include<string>

const std::string catalog_name{"/path/to/catalog/logs/cata.log"};
const std::string directories_name{"/path/to/catalog/logs/dirs.log"};
```
The location of this header must, of course, be included when compiling. In order to move the logs elsewhere, the program must be recompiled.

C++17 is required because the program uses the filesystem namespace of the standard library. There are no other dependencies. For example,
```
mkdir bin
g++ -std=c++17 -o bin/bs -I logs/ lib/*cpp
```
where I've assumed that the `names.h` header was put in `logs/`. Of course, you can call it anything you like. I like `bs`, which is short for bookshelf.

## example

An example with some dummy papers and books is provided. One can try to barrel straight in and do
```
g++ -std=c++17 -o bin/examplebs -I example/ lib/*cpp
```
From hereon I will assume that `bin/` has been added to `PATH`, for convenience. The problem, of course, is that the paths in `example/names.h` are (likely) not real. The program should (hopefully) compile, but will not be able to find the example logs:
```
$ examplebs -lt
failed to read /path/to/catalog/example/cata.log
```
After fixing the paths in `example/names.h` and compiling again, it should now be able to find the logs:
```
$ examplebs -lt
-----
catalog:
0. First paper
1. Second paper
2. Third paper
3. Fourth paper
-----
```

First, look at `cata.log`. It is meant to be human-readable and can be manually modified. In fact, when adding many entries to it (as you might when you first begin), you will probably want to write in authors and titles by hand. Each field should occupy only one line, so long titles / author lists will appear ugly in a text editor. Each entry has a unique id, and these ids appear in increasing order. Currently this format for ids must be followed strictly; that is, if there are 4 entries, the last entry must have id 3 and the first must have id 0, etc. Adding entries via the program will follow this format.

Next, look at `dirs.log`. This should list the directories containing the files you want to catalog. There should only be one directory here, and it is (likely) incorrect. Not all features of the program require that the directories be correct. Of course, for this to be of any use, the logs should be accurate, and one can quickly check if the files contained in the logs actually exist:
```
$ examplebs -ch
-----
checking log file...

[0]
/path/to/catalog/example/papers/paper1 was not found

[1]
/path/to/catalog/example/papers/paper2 was not found

[2]
/path/to/catalog/example/papers/paper3 was not found

[3]
/path/to/catalog/example/papers/paper4 was not found

4 files were not found!
-----
```
Upon fixing the path in the `dirs.log`, `examplebs -ch` should successfully find all the files.

If all is well, then try
```
$ examplebs -sa alice
-----
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
-----
```
This is the result of searching "alice" by authors. The search is case insensitive. Here I did not fix the path in `dirs.log`, so it still shows a fictituous path. Now, if I was looking for the paper by Bob and Alice, I can see where it is. For example, on macOS, I can just use the magic `open`:
```
open /path/to/catalog/example/papers/paper1
```
The number which appears above each result is the entry id. For example, `examplebs -id 0 3` should yield
```
$ examplebs -id 0 3
-----
[0]
Bob and Alice
First paper
/path/to/catalog/example/papers/paper1

[3]
Alice
Fourth paper
/path/to/catalog/example/papers/paper4

-----
```
The id is used to update or delete entries. If I realize that Bob didn't actually author First paper, I can update
```
$ examplebs -ua 0 "Alice"
-----
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
-----
```
Now, searching `examplebs -sa bob` yields no results. As previously mentioned, these entries can also be modified by hand, i.e. manually editing `cata.log`.

Navigate to the `example/books`. There are three empty files here, meant to represent books that you might want to include in your catalog. To add them:
```
$ examplebs -ae *
-----
/home/kkly2/catalog/example/books was not found in available directories
```
This fails because I haven't added the current working directory to `dirs.log`. Note now that this is a real path on this particular machine. In order to add an entry, you must always be in same directory as the entry, and this directory must be known in `dirs.log`. The directory can be added with
```
$ examplebs -ad
-----
added new directory /home/kkly2/catalog/example/books (dir_id = 1)
-----
```
and now we can add these books
```
$ examplebs -ae *
-----
added /home/kkly2/catalog/example/books/book1 (id = 4)
added /home/kkly2/catalog/example/books/book2 (id = 5)
added /home/kkly2/catalog/example/books/book3 (id = 6)
-----
```
If I decide I don't want book2 in there, I can remove it with
```
$ examplebs -de 5
-----
removing entry
(id) 5
(authors)
(title)
(notes)
(dir_id) 1
(filename) book2
-----
```
Currently only one entry can be removed at a time. Recall that the id numbers follow a strict order; doing this removal will shift all id numbers after the deleted entry down so as to maintain this order.

## manual

Here it is assumed the program is named `bs`.

`bs -ch`
Checks every entry in catalog to see if all the files logged there actually exist

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
