#ifndef __ERR_MSG_H__
#define __ERR_MSG_H__
#include <string.h>

/* *************ErrMsg_head***************
 *
 * Usage
 * -----
 * char errmsg[1024] = "";
 * ErrMsg_head(errmsg);
 * const char *descr = "\tCannot open dir";
 * strcat(errmsg, descr);              // FILE: In function FUNC(): descr
 * *******************************/
#define ErrMsg_head(errmsg) \
    const char *file = __FILE__; \
    strcat(errmsg, file); \
    strcat(errmsg, ": "); \
    const char *func = __func__; \
    strcat(errmsg, "In function "); \
    strcat(errmsg, func); \
    strcat(errmsg, "():\n");

#endif // __ERR_MSG_H__
