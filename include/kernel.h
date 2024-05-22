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

#ifndef _KERNEL_H_

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

#define TA_HLNG  0x00 // Start a processing unit through a high-level language interface
#define  TA_ASM  0x01 // Start a processing unit through an assembly language interface

#define  TA_ACT  0x02 // Task is activated after the creation
#define TA_RSTR  0x04 // Restricted task

typedef struct t_ctsk {
	ATR     tskatr;  /* Task attribute */
	VP_INT  exinf;   /* Task extended information */
	FP      task;    /* Task start address */
	PRI     itskpri; /* Task initial priority */
	SIZE    stksz;   /* Task stack size (in bytes) */
	VP      stk;     /* Base address of task stack space */
	/* Other implementation specific fields may be added. */
} T_CTSK;

#define TFN_CRE_TSK -0x05
ER cre_tsk (ID tskid, T_CTSK *pk_ctsk);

#define TFN_ACT_TSK -0x07
ER act_tsk(ID tskid);
#define TFN_IACT_TSK -0x71
ER iact_tsk(ID tskid);

#define TFN_EXT_TSK -0x0a
void ext_tsk();


#define TFN_SLP_TSK –0x11
ER slp_tsk();

#define TFN_WUP_TSK –0x13
ER wup_tsk(ID tskid);
#define TFN_IWUP_TSK –0x72
ER iwup_tsk(ID tskid);


/******************
 *   INTERRUPTS   *
 ******************/

enum INTNO_LIST {
	INTNO_USR1 = 1,
	INTNO_USR2 = 2,
	INTNO_ALRM = 3,
	INTNO_INT  = 4,
	INTNO_TERM = 5,
};

/**
 * Interrupt handler number
 */
typedef UINT  INHNO;

/**
 * Interrupt number
 */
typedef enum INTNO_LIST INTNO;

typedef struct t_dinh {
	/** No inhatr attributes supported. */
	ATR  inhatr; /* Interrupt handler attribute */
	FP   inthdr; /* Interrupt handler start address */
	/* Other implementation specific fields may be added. */
} T_DINH;

// Cannot use SIGRTMAX since it is a variable...
#define Z_INH_SIGNAL_MAX 64
#define Z_INH_MAX Z_INH_SIGNAL_MAX

#define TFN_DEF_INH  -0x65
ER def_inh (INHNO inhno, T_DINH *pk_dinh);

/**********************
 *   IDENTIFICATION   *
 **********************/

#define TKERNEL_MAKER 0x0008 // Individuals (or personal businsses)
#define TKERNEL_SPVER 0x5_4.03 // µITRON 4.03
/**
 * `prid` is the number for identifying the kernel.
 * The kernel maker can assign values to prid.
 * A particular kernel implementation should be uniquely identified by the combination of maker and prid codes.
 */
#define TKERNEL_PRID  0x0000 // Meaningless for individuals...
#define TKERNEL_PRVER 0x0001 // First version of zepto-ish!

/***************************
 *   Vendor System Calls   *
 ***************************/

void vconsole_stdoutf(char* fmt, ...);
void vconsole_stderrf(char* fmt, ...);
ER vconsole_reghooks(FP before, FP after);
void vprocess_exit(UB code);


/*********************************
 *   zepto-specific interfaces   *
 *********************************/

void zepto_init();
void zepto_run();

/************************
 *   Additional stuff   *
 ************************/

#if defined(WITH_TRACE)
#  if !defined(WITH_DBG)
#    define WITH_DBG
#  endif
#endif

#if defined(WITH_TRACE)
#define TRACE(fmt, ...) vconsole_stderrf("%s@%d(%s) [zepto] " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__);
#else
#define TRACE(fmt, ...) {}
#endif

#if defined(WITH_DBG)
#define   DBG(fmt, ...) vconsole_stderrf("%s@%d(%s) [zepto] " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__);
#else
#define   DBG(fmt, ...) {}
#endif

#define PANIC(msg, err) { vconsole_stderrf("\n!!\n!! ERROR (%d): %s\n!!\n", err, msg); vprocess_exit(255); }
#define PANIC(msg, err) { vconsole_stderrf("\n!!\n!! ERROR (%d): %s\n!!\n", err, msg); vprocess_exit(255); }

#endif
