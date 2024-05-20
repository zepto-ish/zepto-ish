/* SPDX-License-Identifier: GPL-3.0-only */
/*  ╔════════════════════════════════════════════════════╗
 *  ║                     __             ._      ._      ║
 *  ║ _______ ____ _____./  |_ ___       |_| ____| |__   ║
 *  ║ \___  / __  \\___ \_  __/ _ \  ____| |/  __| |  \  ║
 *  ║  / __/\  ___/| |_> |  || <_> |/___/| |\__ \|  Y  \ ║
 *  ║ /____ \\___ \|  __/|__| \___/      |_/___  |__|  / ║
 *  ║      \/    \/|_|                         \/    \/  ║
 *  ╚════════════════════════════════════════════════════╝
 */

#ifndef _ITRON_H_

/**
 * Data types for µITRON 4.0 and our minimal API
 */

/* [SP] next to types or signatures explains the standard profile expectations.
 */

#include <stdbool.h>
#include <stdint.h>

/****************************
 * ITRON General Data Types *
 ****************************/

/* Signed integer types */
typedef   int8_t   B;
typedef  int16_t   H;
typedef  int32_t   W;
typedef  int64_t   D;

/* Unsigned integer types */
typedef  uint8_t  UB;
typedef uint16_t  UH;
typedef uint32_t  UW;
typedef uint64_t  UD;

/* Unknown data type of a given width */
typedef  uint8_t  VB;
typedef uint16_t  VH;
typedef uint32_t  VW;
typedef uint64_t  VD;

/* Pointer types */
typedef void* VP;
typedef void (*FP)();

/* For the given processor */
typedef          int   INT; // [SP] 16 or more bits
typedef unsigned int  UINT; // [SP] 16 or more bits

/* Other types */
typedef bool  BOOL;

typedef   INT       FN; // [SP] 16 or more bits
typedef   INT       ER; // [SP]  8 or more bits
typedef   INT       ID; // [SP] 16 or more bits
typedef  UINT      ATR; // [SP]  8 or more bits
typedef  UINT     STAT; // [SP] 16 or more bits
typedef  UINT     MODE; // [SP]  8 or more bits
typedef   INT      PRI; // [SP] 16 or more bits
typedef  UINT     SIZE; // [SP] equal to the number of bits in a pointer
typedef   INT      TMO; // [SP] 16 or more bits, unit of time is 1msec
typedef   INT   RELTIM; // [SP] 16 or more bits, unit of time is 1msec
typedef   INT   SYSTIM; // [SP] 16 or more bits, unit of time is 1msec
typedef    VP   VP_INT;
typedef   INT  ER_BOOL;
typedef   INT    ER_ID;
typedef  UINT  ER_UINT;

/***************************
 * ITRON General Constants *
 ***************************/

/* General constants */
#undef NULL
#define  NULL  0
#define  TRUE  1
#define FALSE  0
#define  E_OK  0

/* (I) Reserved Error Codes (from –5 to –96) */

#define    E_SYS   -5

#define  E_NOSPT   -9
#define   E_RSFN  -10
#define  E_RSATR  -11

#define    E_PAR  -17
#define     E_ID  -18

#define    E_CTX  -25
#define   E_MACV  -26
#define   E_OACV  -27
#define  E_ILUSE  -28

#define  E_NOMEM  -33
#define   E_NOID  -34

#define    E_OBJ  -41
#define  E_NOEXS  -42
#define   E_QOVR  -43

#define  E_RLWAI  -49
#define  E_TMOUT  -50
#define    E_DLT  -51
#define    E_CLS  -52

#define   E_WBLK  -57
#define   E_BOVR  -58


/* (J) Implementation-Specific Error Codes (from –97 to –128) */

/**
 * EV_NOIMPL  a function or feature has not been implemented yet.
 */
#define EV_NOIMPL  -128

/* Object Attributes */
#define  TA_NULL  0

/* Timeout Specification */
#define   TMO_POL   0
#define  TMO_FEVR  -1
#define  TMO_NBLK  -2

/************************
 * ITRON General Macros *
 ************************/

// #define INCLUDE(x) #error INCLUDE not supported

#endif
