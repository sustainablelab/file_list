#ifndef __PATH_UTILS_H__
#define __PATH_UTILS_H__

#include <sys/stat.h>
#include "bool.h"

bool PathUtils_is_dir(const char *path)
{ // Return true if path is a directory
    struct stat st;
    stat(path, &st);
    return S_ISDIR(st.st_mode);
}

const char* PathUtils_get_extension(const char *name)
{ // Return file extension of file "name", return '\0' if "name" has no extension

    // Advance name to its file extension
    while(  (*name!='\0')&&(*name!='.')  ) {name++;}// Walk to extension '.'
    if (*name!='\0') name++;                        // File name has extension, walk past the '.'
    return name;
}

bool PathUtils_has_extension(const char *name, const char *ext)
{ // Return true if file "name" has extension "ext"

    const char* fx = PathUtils_get_extension(name); // Get file extension

    // Check file has an extension
    if(*fx=='\0') return false;                     // File name has no extension.

    // Check length of file extension matches length of ext
    int len_fx=0; {const char *c=fx; while(*c++!='\0') len_fx++;}   // Measure length of file extension
    int len_mx=0; {const char *c=ext;  while(*c++!='\0') len_mx++;} // Measure length of ext
    if(len_fx != len_mx) return false;              // File name extension and ext are different lengths

    // Check every character matches
    for(int i=0; i<len_fx; i++) { if(fx[i]!=ext[i]) return false; }
    return true;
}

bool PathUtils_has_path_sep(const char *path, const char sep)
{
    bool has_path_sep = false;                  // This is the return value
    while(*path!='\0')                          // Walk the path
    {
        if(*path==sep)                          // Is this '/' or '\\' or whatever
        {
            has_path_sep = true;                // If so, we're done here.
            break;
        }
        else path++;                            // Otherwise, keep walking
    }
    return has_path_sep;
}

const char* PathUtils_tail(const char *path, const char sep)
{ // Return last path component
    const char *tail = path;                        // Return path if sep not in path
    if(  PathUtils_has_path_sep(path,sep)  )
    {  // Walk to end of path, then walk back to last separator
        while(*tail!='\0'){tail++;} while(*tail!=sep){tail--;}tail++;
    }
    return tail;
}

#endif // __PATH_UTILS_H__
