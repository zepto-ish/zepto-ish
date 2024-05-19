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

/**
 * Supported API from µITRON 4.0
 */

#include <itron.h>

/*************
 *   TASKS   *
 *************/

#define TMIN_TPRI  1
#define TMAX_TPRI 64

#define TTS_NXX  0x00 // NON-EXISTENT state [zepto]
#define TTS_RUN  0x01 // RUNNING state
#define TTS_RDY  0x02 // READY state
#define TTS_WAI  0x04 // WAITING state
#define TTS_SUS  0x08 // SUSPENDED state
#define TTS_WAS  0x0c // WAITING-SUSPENDED state
#define TTS_DMT  0x10 // DORMANT state

typedef struct t_ctsk {
	ATR     tskatr;  /* Task attribute */
	VP_INT  exinf;   /* Task extended information */
	FP      task;    /* Task start address */
	PRI     itskpri; /* Task initial priority */
	SIZE    stksz;   /* Task stack size (in bytes) */
	VP      stk;     /* Base address of task stack space */
	/* Other implementation specific fields may be added. */
} T_CTSK;

// Static API (??)
// CRE_TSK ( ID tskid, { ATR tskatr, VP_INT exinf, FP task, PRI itskpri, SIZE stksz, VP stk } );
#define TFN_CRE_TSK -0x05
ER cre_tsk (ID tskid, T_CTSK *pk_ctsk);
//#define TFN_ACRE_TSK -0xc1
//ER_ID acre_tsk(T_CTSK *pk_ctsk);

#define TFN_ACT_TSK -0x07
ER act_tsk(ID tskid);
#define TFN_IACT_TSK -0x71
ER iact_tsk(ID tskid);

#define TFN_EXT_TSK -0x0a
void ext_tsk();


/***********
 *   WIP   *
 ***********/

// XXX correctly file

#define TMIN_MPRI  1
#define TMAX_MPRI 64


#define TKERNEL_MAKER 0x0008 // Individuals (or personal businsses)
#define TKERNEL_SPVER 0x5_4_02 // µTRON 4.02
/**
 * `prid` is the number for identifying the kernel.
 * The kernel maker can assign values to prid.
 * A particular kernel implementation should be uniquely identified by the combination of maker and prid codes.
 */
#define TKERNEL_PRID  0x0420 // Meaningless for individuals...
#define TKERNEL_PRVER 0x0001 // First version of zepto-ish!

/***********
 *   WIP   *
 ***********/

void zepto_init();
void zepto_run();

/************************
 *   Additional stuff   *
 ************************/

#include <stdio.h>
#include <stdlib.h>

#if defined(WITH_TRACE)
#define TRACE(fmt, ...) fprintf(stderr, "%s@%d(%s) [zepto] " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__);
#else
#define TRACE(fmt, ...) {}
#endif
#define   DBG(fmt, ...) fprintf(stderr, "%s@%d(%s) [zepto] " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__);

#define PANIC(msg, err) { fprintf(stderr, "\n!!\n!! ERROR (%d): %s\n!!\n", err, msg); exit(-1); }
