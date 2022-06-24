/* *************DOC***************
 * Demonstrate how to use file_list.h
 * *******************************/
#include <stdio.h>
#include "file_list.h"      // My file_list lib

// Recursion example
#include "file_list-tree.h"

// Example for FileList_next() sets up a cmdline loop that reads characters from stdin
#include "bool.h"
#include <unistd.h>         // read()

uint8_t num_demos = 5;
uint8_t demo_cnt = 0;

int press_for_next_demo(void)
{ // Press ENTER to start demo
    /* *************DOC***************
     * Wait for the user to press ENTER.
     *
     * Return -1 if reading from stdin fails.
     * Return 0 on success.
     * *******************************/
    if(demo_cnt==0) puts("\nPress ENTER to start demo...");
    else puts("\nDemo finished. Press ENTER to start next demo...");
    fflush(stdout);
    bool quit = false;
    while(quit != true)
    {
        char c= '\0';
        if(  (read(STDIN_FILENO, &c, 1) == -1)  ) return -1;
        quit = true;
    }
    return 0;
}
#define next_demo if(press_for_next_demo() < 0) return EXIT_FAILURE;
int main()
{
    /* *************EXAMPLE: Error messages***************
     * Print an error message if path does not exist.
     * Print an error message if path is not a directory.
     *
     * There is no clean up to do after an error.
     * Just test for NULL and do whatever you want.
     * For example, you can just exit the program:
     *
     *      FileList *fList = FileList_new_from_path(bad);
     *      if (fList == NULL) return EXIT_FAILURE;
     *
     * *******************************/
    { // EXAMPLE: path does not exist
        next_demo;
        puts("");printf("------------ DEMO %d of %d --------------", ++demo_cnt, num_demos);
        puts("");  puts("--- Error Demo: path does not exist ---"); puts("");
        fflush(stdout);                                 // Match print order with execution order
        const char *bad = "bad";                        // There is no "bad/" folder
        FileList *fList = FileList_new_from_path(bad);  // Make new FileList
        if (fList == NULL) puts("(FileList is NULL)");  // Will get NULL. Do whatever you want here.
        fflush(stdout);
    }
    { // EXAMPLE: path is not a directory
        next_demo;
        puts("");printf("-------------- DEMO %d of %d ----------------", ++demo_cnt, num_demos);
        puts("");  puts("--- Error Demo: path is not a directory ---"); puts("");
        fflush(stdout);                                 // Match print order with execution order
        const char *file = "fake-img/a.bmpx";                     // "img/a" is a file
        FileList *fList = FileList_new_from_path(file); // Make a file list
        if (fList == NULL) puts("(FileList is NULL)");  // Will get NULL. Do whatever you want here.
        fflush(stdout);
    }

    /* *************EXAMPLE: FileList_tree (recursion example)***************
     * Do a "tree"-like print to stdout.
     * Pick tree depth.
     *
     * "tree" is recursive, but FileList is not designed for
     * recursion.
     *
     * When a new FileList is created from a directory path,
     * the FileList->names only contains the files and
     * folders in that directory. There is no struct member
     * to hold a new FileList type. This is intentional
     * because I feel that recursing into a folder is not
     * always desirable.
     *
     * I wrote this "tree"-like print (FileList_tree) to
     * demonstrate a way to do recursion with FileList.
     *
     * On each recursion:
     *
     * - create a new FileList
     * - do work with that FileList
     * - destroy the FileList after popping back up the stack
     *
     * When FileList_tree() returns, only the top-level
     * FileList remains in memory.
     * *******************************/
    { // EXAMPLE: Print a tree-like list of the files in folder `img`
        next_demo;
        const char *img = "fake-img";                        // Folder with my images
        FileList *fList = FileList_new_from_path(img);  // My FileList type
        if (fList == NULL) return EXIT_FAILURE;         // Lib file_list handles errors, I just exit

        puts("");printf("-------- DEMO %d of %d -----", ++demo_cnt, num_demos);
        puts(""); puts("--- Demo depth examples ---"); puts("");
        // Depth examples -- pick one (set false to true)
        if(false) FileList_tree(fList, img, 0, 0);       // 0: Just print how many files/folders in img
        if(false) FileList_tree(fList, img, 1, 0);       // 1: Print files 1 level deep
        if(false) FileList_tree(fList, img, 2, 0);       // 2: Print files 2 levels deep
        if(true) FileList_tree(fList, img, -1, 0);      // -1: Recurse as deep as I can (up to 128 levels)

        // ---> Free the FileList <--- (and optionally set to NULL)
        // FileList allocates mem for itself and all the strings of file and folder names
        FileList_free(fList); fList = NULL;
        fflush(stdout);
    }

    /* *************EXAMPLE: FileList_next***************
     * FileList_next() returns next file name.
     *
     * This example is a silly cmdline loop that just prints
     * the file name.
     *
     * In practice, the files are images and I'm in a game
     * loop. When the user presses a key, I cycle to display
     * the next image in the folder.
     * *******************************/
    { // EXAMPLE: Press ENTER to print next file, quit: any key (then Enter)
        { // Print user instructions
            next_demo;
            puts("");printf("-------- DEMO %d of %d -------", ++demo_cnt, num_demos);
            puts(""); puts("--- Demo: FileList_next() ---");puts("");
            puts("\tDemo Instructions");
            puts("\t-----------------");
            puts("\tPRESS ENTER TO PRINT NEXT FILE");
            puts("\tQUIT: PRESS ANY OTHER KEY (THEN PRESS ENTER)");
            puts(""); fflush(stdout);
        }

        // Setup
        const char *img = "fake-img";
        FileList* fList = FileList_new_from_path(img);

        // Doing something with the first file in the list.
        printf("%s",*fList->names); printf("\t\t<--- top of file list"); // Kick off with first file
        fflush(stdout);

        // Now I'm in a loop and I want the next file in the list.
        bool quit = false;
        while (quit != true)
        {
            char c = '\0';
            if (  (read(STDIN_FILENO, &c, 1) == -1)  ) return EXIT_FAILURE;
            switch(c)
            {
                case 10:                        // Hit ENTER: print next file name.
                {
                    if(fList->_idx==fList->num_items-1) puts("");       // Print blank at top of list
                    printf("%s",FileList_next(fList));                  // Print next file name
                    if(fList->_idx==0) printf("\t\t<--- top of file list"); // Call out top of list
                    fflush(stdout);
                    break;
                }
                default: quit = true; break;    // Any other key (then Enter), quit.
            }
        }

        // Shutdown
        FileList_free(fList);
        fflush(stdout);

        // Swallow ENTER from previous demo
        {char c = '\0'; if (  (read(STDIN_FILENO, &c, 1) == -1)  ) return EXIT_FAILURE;}
    }

    /* *************EXAMPLE: FileList_prev***************
     * Like FileList_next but it's the previous file.
     * *******************************/
    { // EXAMPLE: Press ENTER to print next file, quit: any key (then Enter)
        { // Print user instructions
            next_demo;
            puts("");printf("-------- DEMO %d of %d -------", ++demo_cnt, num_demos);
            puts(""); puts("--- Demo: FileList_prev() ---");puts("");
            puts("\tDemo Instructions");
            puts("\t-----------------");
            puts("\tPRESS ENTER TO PRINT PREVIOUS FILE");
            puts("\tQUIT: PRESS ANY OTHER KEY (THEN PRESS ENTER)");
            puts(""); fflush(stdout);
        }

        // Setup
        const char *img = "fake-img";
        FileList* fList = FileList_new_from_path(img);

        // Doing something with the last file in the list.
        fList->_idx=fList->num_items-1;                  // Point at the last file.
        printf("%s",fList->names[fList->_idx]); printf("\t\t<--- Now I start at the bottom of the file list\n"); // Kick off with last file
        fflush(stdout);

        // Now I'm in a loop and I want the previous file in the list.
        bool quit = false;
        while (quit != true)
        {
            char c = '\0';
            if (  (read(STDIN_FILENO, &c, 1) == -1)  ) return EXIT_FAILURE;
            switch(c)
            {
                case 10:                                // Hit ENTER: print next file name.
                {
                    if(fList->_idx==0) puts("");        // Print blank at top of list
                    printf("%s",FileList_prev(fList));  // Print previous file name
                    if(fList->_idx==0) printf("\t\t<--- top of file list (next ENTER wraps back to the bottom)"); // Call out top of list
                    fflush(stdout);
                    break;
                }
                default: quit = true; break;            // Any other key (then Enter), quit.
            }
        }

        // Shutdown
        FileList_free(fList);
        fflush(stdout);

        // Swallow ENTER from previous demo
        {char c = '\0'; if (  (read(STDIN_FILENO, &c, 1) == -1)  ) return EXIT_FAILURE;}
    }
    puts("");puts("COMPLETED ALL DEMOS");

    return EXIT_SUCCESS;
}
