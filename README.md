# About

These are header-only libraries.

- `file_list.h` - make a list of file names
    - file names are the file path relative to the application folder
- `path_utils.h` - typical path utilities

I use header-only libraries to write applications on Windows in
MinGW.

# Examples

All of the examples are in `file_list.c`.

There is also an example header-lib, `file_list-tree.h`, for
printing a `tree-like` file list where the caller picks the
recursion depth. The purpose is to show a way to add recursive
functionality to the `file_list` lib.

Build the examples:

```bash
make file_list
```

Run the example **from the Vim cmdline.**

```vim
: !./file_list.exe
```

*Do not run from a bash terminal!*

*Calls to `read()` or `fgetc()` only work if I run from the Vim
cmdline. The application hangs at a bash terminal because I
cannot read from stdin. TODO: fix this.*

# `file_list.h`

A `FileList` is a type:

```c
typedef struct _filelist__def                       // <----- DEFINE FileList
{
    int num_items;                                  // Count the number of files and folders
    int num_files;                                  // Count the number of files only
    int num_folders;                                // Count the number of folders only
    char **names;                                   // List of file names
    char **first_name;                              // Remember first address in this list
    int _idx;                                       // Index file number (used by _next and _prev)
} FileList;
```

A `FileList` has these functions:

Create a new FileList:

```c
FileList* FileList_new_from_path(const char *path)              // <----- NEW FileList
```

Destroy the FileList:

```c
void FileList_free(FileList *fList)                         // <----- DESTROY FileList
```

Work with the FileList:

```c
int FileList_filecount(FileList *fList)
int FileList_foldercount(FileList *fList)
const char* FileList_next(FileList *fList)
const char* FileList_prev(FileList *fList)
```

`file_list.h` uses system lib `dirent` to get file and folder
names.

# `path_utils.h`

`path_utils.h` is used by `file_list.h` (so including
`file_list.h` includes `path_utils.h`). I also use functions in
`path_utils.h` directly in my application.

Typical path utilities:

```c
// Is this a directory?
bool PathUtils_is_dir(const char *path)
// Get the file extension
const char* PathUtils_get_extension(const char *name)
// Get the filename (strip the folders from the path)
const char* PathUtils_tail(const char *path, const char sep)
```

Utilities to match for strings in the path:

```c
// Does this path end with this extension?
bool PathUtils_has_extension(const char *name, const char *ext)
// Does this path contain this separator (`/` or `\\`)?
bool PathUtils_has_path_sep(const char *path, const char sep)
```

`path_utils.h` uses `sys/stat.h` to check if a path is a
directory.

# Tags recipes

Make a tags file:

```bash
make tags
```

Print human-readable tags to the screen:

```bash
make show-tags
```

Make another tags file for symbols in the system-headers:

```bash
make lib-tags
```
