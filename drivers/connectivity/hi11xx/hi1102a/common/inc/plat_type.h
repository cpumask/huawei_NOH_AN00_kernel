

#ifndef __PLAT_TYPE_H__
#define __PLAT_TYPE_H__

/* Include other Head file */
#include "platform_oneimage_define.h"

/* Define macro */
#define STATIC static

#if (defined(_PRE_PC_LINT) || defined(WIN32))
/*
 * Note: lint -e530 says don't complain about uninitialized variables for
 * this varible.  Error 527 has to do with unreachable code.
 * -restore restores checking to the -save state
 */
#define unref_param(P)          \
    /*lint -save -e527 -e530 */ \
    {                           \
        (P) = (P);              \
    }                           \
    /*lint -restore */
#else
#define unref_param(P)
#endif

#define PRINT_LINE ps_print_err("%s:%d!\n", __FUNCTION__, __LINE__);

/* typedef */
typedef unsigned char uint8;
typedef char int8;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned int uint32;
typedef int int32;
typedef unsigned long uint64;
typedef long int64;

#endif /* PLAT_TYPE_H */
