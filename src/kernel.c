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
 * Implementation of the zepto-ish API surface.
 */

#include <kernel.h>

/*************
 *   TASKS   *
 *************/

#define TRACE_TASK(pk_ctsk) { \
	TRACE("  | task.tskatr:. %x", pk_ctsk->tskatr); \
	TRACE("  | task.exinf:.. %p", pk_ctsk->exinf); \
	TRACE("  | task.task:... %p", pk_ctsk->task); \
	TRACE("  | task.itskpri: %d", pk_ctsk->itskpri); \
	TRACE("  | task.stksz:.. %d", pk_ctsk->stksz); \
	TRACE("  | task.stk:.... %p", pk_ctsk->stk); \
}

/**
 * Define a static array of 8 tasks, internally.
 */
#define Z_MAX_TSKID 8

/**
 * Internal type for tasks
 */
typedef struct z_t_tasks {
	T_CTSK* task;
	PRI priority;
	STAT state;
	// XXX add context here
} Z_T_TASKS;

static Z_T_TASKS z_tasks[Z_MAX_TSKID];

ER cre_tsk(ID tskid, T_CTSK *pk_ctsk) {
	TRACE("TODO: implement");
	TRACE("New task:");
	TRACE("  | tskid: %d", tskid);
	TRACE_TASK(pk_ctsk);

	// Task ID 0 refers to the "current" task; it is thus always assigned.
	if (tskid < 1) {
		return E_ID;
	}

	// Task ID outside our limited supported range
	if (tskid > Z_MAX_TSKID) {
		return E_ID;
	}

	if (z_tasks[tskid].task != NULL) {
		return E_ID;
	}

	// Add the task to the list of tasks
	z_tasks[tskid].task = pk_ctsk;
	z_tasks[tskid].priority = pk_ctsk->itskpri;

	return E_OK;
}

ER act_tsk(ID tskid) {
	TRACE("TODO: implement");
	TRACE("Activate task:");
	TRACE("  | tskid: %d", tskid);
	return E_NOIMPL;
}

ER iact_tsk(ID tskid) {
	return act_tsk(tskid);
}

void ext_tsk() {
	TRACE("TODO: implement");
}

/***********
 *   WIP   *
 ***********/

static BOOL zepto_init_done = false;
void zepto_init() {
	zepto_init_done = TRUE;
}

void zepto_run() {
	if (zepto_init_done != TRUE) {
		PANIC("zepto_run() called before zepto_init()!!", E_SYS);
	}
	TRACE("Would be running the tasks and doing scheduley stuff");
	TRACE("Hanging!")

	for(;;) {
	}

	PANIC("Exited zepto_run() loop!!", E_SYS);
}
