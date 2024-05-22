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

//
// WARNING: This task ***is not portable***.
//          It relies on a Linux host system to work.
//          Though with a proper `dly_tsk` implementation, it would be portable.
//

#include <itron.h>
#include <kernel.h>

// Host system libraries
#include <unistd.h>

// NOTE: This is not *compliant* with dly_tsk, as resuming the task will
//       break the delay.
//       This is only for demonstration purpose.
static void dly_tsk(UINT dlytim) {
	usleep(dlytim);
}

static void task_looping(VP_INT exinf);
static int LOOPING_TASK_ID = 7;
__attribute__((constructor)) void looping_init() {
	ER ret;
	static T_CTSK task_looping_info;
	task_looping_info.task = (FP)task_looping;
	task_looping_info.exinf = NULL;
	task_looping_info.tskatr |= TA_ACT;
	ret = cre_tsk(LOOPING_TASK_ID, &task_looping_info);
	if (ret < 0) {
		PANIC("Could not create looping task...", ret);
	}
}

static void task_looping(VP_INT exinf) {
	UINT count = 0;
	for (;;) {
		vconsole_stdoutf("Looping, #%d\n", count++);
		dly_tsk(1000 * 1000 * 0.5);
	}
}
