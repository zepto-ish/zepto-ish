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
 * Entrypoint of the simulated environment.
 * Does not use µTRON types.
 */

#include <kernel.h>

void task(VP_INT exinf) {
	/* Body of the task */
	DBG("Hello, World!");
	act_tsk(2);
	ext_tsk();
	PANIC("End of sample task reached...", E_SYS);
}

void other_task(VP_INT exinf) {
	static INT s_i = 0;
	INT i = 0;
	i++;
	s_i++;
	DBG("This should stay at 1: %08d", i);
	DBG("This should increment: %08d", s_i);
	if (i != 1) {
		PANIC("Stack-allocated variables are not working properly...", E_SYS);
	}
	if (s_i <= 10) {
		act_tsk(1);
	}
}

static T_CTSK task_info = {0};
static T_CTSK other_info = {0};

int main(int argc, char** argv) {
	ER_ID ret;

	zepto_init();

	// This would be in the application-specific initialization normally...
	// For now, implemented as `main`...
	DBG("Starting up...");

	DBG("... creating task");
	task_info.task = (FP)task;
	task_info.exinf = NULL;

	ret = cre_tsk(1, &task_info);
	if (ret < 0) {
		PANIC("Could not create task...", ret);
	}

	DBG("... creating other_task");
	other_info.task = (FP)other_task;
	other_info.exinf = NULL;
	other_info.tskatr |= TA_ACT; // Start active

	ret = cre_tsk(2, &other_info);
	if (ret < 0) {
		PANIC("Could not create other_task...", ret);
	}

	// Launch
	zepto_run();

	// This should not happen
	PANIC("End of main() reached!", E_SYS);
}
