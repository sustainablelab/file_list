#ifndef __FILE_LIST_TREE_H__
#define __FILE_LIST_TREE_H__
#include "file_list.h"

// file_list.h is not setup for recursion natively.
// Here is an example showing how to do recursion with FileList types.
// This example is a mediocre version of the GNU "tree" utility.
/* *************Example Tree Artwork***************
 * img/ (3 files, 3 folders)
 *   |                                      see macro INDENT
 *   |- a
 *   |- b
 *   |- c
 *   |- blah_GS/ -- (3 files, 0 folders)    see macro BRANCH
 *   |   |
 *   |   |- _GS/d
 *   |   |- _GS/e
 *   |   |- _GS/f
 * *******************************/
#define INDENT_AMOUNT 4
#define INDENT(indent) \
    for (int i=0; i<(indent); i++) {putchar((i%INDENT_AMOUNT)==0?'|':' ');}
#define BRANCH(indent) \
    for (int i=0; i<(indent-1); i++) {putchar((i%INDENT_AMOUNT)==0?'|':'-');} putchar(' ');
void FileList_tree(FileList *fList, const char *path, int8_t depth, int indent)
{ // Print a tree-like file list.
    /* *************DOC***************
     * Print the list of files at path.
     * Recursive dive into folders.
     *
     * path : const char* -- folder path relative to the .exe
     * depth : int8_t -- recursion depth, use -1 for maximum recursion
     * indent : int -- must be 0, used by recursion to add indentation
     *
     * Each time this function recurses,
     * indent increases and (remaining) depth decreases.
     *
     * Usage
     * -----
     * #include "file_list.h"
     * #include "file_list-tree.h"
     * ...
     *      const char *img = "img";
     *      FileList *fList = FileList_new_from_path(img);
     *      if (fList == NULL) return EXIT_FAILURE;
     *
     *      // EXAMPLES USING THIS FUNCTION
     *      FileList_tree(fList, img, 1, 0);       // Print 1 level deep
     *      FileList_tree(fList, img, 2, 0);       // Print 2 levels deep
     *      FileList_tree(fList, img, -1, 0);      // Recurse up to 128 levels deep
     *      FileList_tree(fList, img, 0, 0);       // Print no levels, just count files and folders
     *      ...
     *      FileList_free(fList);   // Done with FileList
     *      ...
     *
     * *******************************/
    bool is_top=true;
    { // Determine if this is the top-level folder
        const char *c = path; while((*c!='\0')&&(is_top)) is_top=(*c++!='/');
    }
    const char *stripped = path;                // Strip parents if this is not the top-level
    if(!is_top)
    {
        while(*stripped++!='/');                // Strip the parent folder
        BRANCH(indent);                         // Print a branched indent
    }
    // Print folder name and stats like "img/ (3 files, 3 folders)"
    printf("%s/ -- (%d files, %d folders)\n", stripped, fList->num_files, fList->num_folders);
    if (depth==0) return;                       // Hit depth limit, no more recursion

    if (  FileList_foldercount(fList) > 0  )
    {
        INDENT(indent); puts("|");              // Draw tree trunk under first char of folder name
    }
    for (int i=0; i<fList->num_items; i++)
    {
        char *c = fList->names[i];                  // Next name in list
        while (*c++!='\0'){}; while (*--c!='/'){}; c++; // Strip parents
        // Print name if this is a file. Use "\" if it is the last file.
        INDENT(indent); if(  !PathUtils_is_dir(fList->names[i])  ) printf("%s-- %s\n", (i+1)<fList->num_items?"|":"\\",c);
        // Recursive dive if this is a directory.
        if (PathUtils_is_dir(fList->names[i]))
        {
            int new_indent = indent+INDENT_AMOUNT;  // Draw tree one level deeper
            int new_depth = depth-1;                // Count down to hitting the depth limit
            // Recurse
            FileList *_fList = FileList_new_from_path(fList->names[i]);         // New FileList
            FileList_tree(_fList, fList->names[i], new_depth, new_indent); // Print it
            FileList_free(_fList);          // Popped back, free that FileList.
        }
    }
}

#endif // __FILE_LIST_TREE_H__

