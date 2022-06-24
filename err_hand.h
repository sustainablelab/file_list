#ifndef __ERR_HAND_H__
#define __ERR_HAND_H__
#include <string.h>

/* *************ErrHand_quit_if_cannot_open(dir)***************
 * dir: DIR* (from dirent.h)
 * if dir == NULL:
 *      Build error message : FILE: In function FUNC(): Cannot open dir "path"
 *      Print error message.
 *      Return -1.
 *
 * Usage
 * -----
 * #include <dirent.h>                      // Defines DIR and opendir()
 * #include "err_msg.h"                     // This library
 * const char *path = "img"                 // Folder to open is "img/"
 * DIR *dir = opendir(path);                // Open folder
 * ErrHand_quit_if_cannot_open(dir);        // Return -1 on fail (e.g., folder does not exist)
 * *******************************/
#define ErrHand_quit_if_cannot_open(dir) \
    if (dir == NULL) \
    { \
        char errmsg[1024] = ""; \
        { \
            ErrMsg_head(errmsg); \
            strcat(errmsg, "\tCannot open dir"); \
            strcat(errmsg, " \""); strcat(errmsg, path); strcat(errmsg, "\""); \
        } \
        perror(errmsg); \
        return -1; \
    } \

#endif // __ERR_HAND_H__
