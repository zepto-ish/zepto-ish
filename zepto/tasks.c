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
 * Implementation of the zepto-ish tasks API surface.
 */

#include <kernel.h>

// System headers used internally by the kernel
#include <ucontext.h>

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
#define Z_TSK_STKSZ (1 << 20)

/**
 * Internal type for tasks
 */
typedef struct z_t_task {
	T_CTSK* pk_ctsk;
	UINT id;
	PRI priority;
	STAT state;
	ucontext_t task_context;
	UB task_stack[Z_TSK_STKSZ];
} Z_T_TASK;

static Z_T_TASK z_tasks[Z_MAX_TSKID];
static Z_T_TASK *z_current_task = NULL;

/**
 * Context used for implicit `ext_tsk` on return.
 */
static ucontext_t z_ext_tsk_context;
static UB z_ext_tsk_stack[Z_TSK_STKSZ]; // Its stack

/**
 * Context for returning to the main loop
 */
static ucontext_t z_return_context;

/**
 * Call to setup a task to its initial state.
 * Also resets the task context appropriately, if needed...
 */
static void z_init_task(Z_T_TASK *task) {
	TRACE("  → Setting up task #%d", task->id);
	// Create a user context for the task
	if (getcontext(&task->task_context) == -1) {
		PANIC("getcontext returned -1", E_SYS);
	}
	task->pk_ctsk->stk = task->task_stack;
	task->pk_ctsk->stksz = Z_TSK_STKSZ;
	task->task_context.uc_stack.ss_flags = 0;
	task->task_context.uc_stack.ss_sp = task->task_stack;
	task->task_context.uc_stack.ss_size = Z_TSK_STKSZ;
	// Implicit `ext_tsk` on `return`
	task->task_context.uc_link = &z_ext_tsk_context;

	// Sets the task as dormant by default
	task->state = TTS_DMT;

	makecontext(&task->task_context, task->pk_ctsk->task, 1, task->pk_ctsk->exinf);
}

/**
 * Create Task
 *
 * (See the µITRON 4.02 specification)
 */
ER cre_tsk(ID tskid, T_CTSK *pk_ctsk) {
	TRACE("New task:");
	TRACE("  | tskid: %d", tskid);
	TRACE_TASK(pk_ctsk);

	// Task ID 0 refers to the "current" task; it is thus always assigned.
	if (tskid < 1) {
		return E_ID;
	}

	// Task ID outside our limited supported range
	if (tskid >= Z_MAX_TSKID) {
		TRACE("Invalid task ID #%d", tskid);
		return E_ID;
	}

	// Task ID already in use
	if (z_tasks[tskid].pk_ctsk != NULL) {
		return E_ID;
	}

	// Task already in the list of tasks
	for (int i = 0; i < Z_MAX_TSKID; i++) {
		if (z_tasks[i].pk_ctsk == pk_ctsk) {
			return E_OBJ;
		}
	}

	z_tasks[tskid].id = tskid;
	// Task starts in a non-executing state.
	z_tasks[tskid].state = TTS_NXX;
	// Attach the data packet
	z_tasks[tskid].pk_ctsk = pk_ctsk;
	// Setup its initial priority
	z_tasks[tskid].priority = pk_ctsk->itskpri;
	// Initialize the task
	z_init_task(&z_tasks[tskid]);
	ATR attrs = pk_ctsk->tskatr;
	if (attrs & TA_ACT) {
		z_tasks[tskid].state = TTS_RDY;
	}

	return E_OK;
}

/**
 * Activate Task
 *
 * (See the µITRON 4.02 specification)
 */
ER act_tsk(ID tskid) {
	TRACE("Activating task:");
	TRACE("  | tskid: %d", tskid);
	if (tskid >= Z_MAX_TSKID) {
		TRACE("Invalid task ID #%d", tskid);
		return E_ID;
	}
	if (z_tasks[tskid].state == TTS_NXX) {
		TRACE("Non-existant task ID #%d", tskid);
		return E_NOEXS;
	}
	z_tasks[tskid].state = TTS_RDY;

	return E_OK;
}

/**
 * Activate Task
 *
 * (See the µITRON 4.02 specification)
 */
ER iact_tsk(ID tskid) {
	return act_tsk(tskid);
}

/**
 * Terminate Invoking Task
 *
 * (See the µITRON 4.02 specification)
 */
void ext_tsk() {
	TRACE("Exiting task #%d", z_current_task->id);

	// Resetting the task (so it can be queued back)
	z_init_task(z_current_task);

	// Return back to call site
	setcontext(&z_return_context);
}

/**
 * Put Task to Sleep
 *
 * (See the µITRON 4.02 specification)
 */
ER slp_tsk() {
	TRACE("Putting task #%d to sleep", z_current_task->id);
	// Don't clobber suspended state
	z_current_task->state &= TTS_SUS;
	// Set task as waiting
	z_current_task->state |= TTS_WAI;

	swapcontext(&z_current_task->task_context, &z_return_context);

	// TODO: detect `rel_wai` and return E_RLWAI
	return E_OK;
}

/**
 * Wake up Task
 *
 * (See the µITRON 4.02 specification)
 */
ER wup_tsk(ID tskid) {
	if (tskid == 0) {
		tskid = z_current_task->id;
	}
	TRACE("Waking-up task #%d from sleep", tskid);
	if (z_tasks[tskid].state == TTS_NXX) {
		return E_NOEXS;
	}

	// FIXME: this should queue a wake-up request instead.
	// Don't clobber suspended state
	z_tasks[tskid].state &= ~TTS_WAI;
	z_tasks[tskid].state |= TTS_RDY;

	return E_OK;
}

/**
 * Wake up Task
 *
 * (See the µITRON 4.02 specification)
 */
ER iwup_tsk(ID tskid) {
	if (tskid == 0) {
		return E_ID;
	}
	return wup_tsk(tskid);
}


void _kernel_initialize_tasks() {
	TRACE("Setting-up context for `ext_tsk`");
	getcontext(&z_ext_tsk_context);
	z_ext_tsk_context.uc_stack.ss_flags = 0;
	z_ext_tsk_context.uc_stack.ss_sp = &z_ext_tsk_stack;
	z_ext_tsk_context.uc_stack.ss_size = Z_TSK_STKSZ;
	makecontext(&z_ext_tsk_context, ext_tsk, 0);
}
void _kernel_schedule_tasks() {
#if 0
	for(int XXX = 0; XXX < 5; XXX++) {
#else
	for(;;) {
#endif
	TRACE("\n");
	TRACE("== Top of the loop ==");
		for (int i = 0; i < Z_MAX_TSKID; i++) {
			z_current_task = &z_tasks[i];
			STAT state = z_current_task->state;
			if (state != TTS_NXX) {
				TRACE("Task ID #%d state: %d", z_current_task->id, z_current_task->state);
				if (state == TTS_RUN || state == TTS_RDY) {
					z_current_task->state = TTS_RUN;
					z_current_task->task_context.uc_link = 0;
					TRACE("→ Running Task #%d", z_current_task->id);
					swapcontext(&z_return_context, &z_current_task->task_context);
					TRACE("← Returning from Task #%d", i);
					if (z_current_task->state == TTS_RUN) {
						z_current_task->state = TTS_RDY;
					}
					z_current_task = NULL;
				}
			}
		}
	}
}
