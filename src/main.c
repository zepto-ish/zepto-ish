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
	vconsole_stdoutf("Hello, World!\n");
	ext_tsk();
	PANIC("End of sample task reached...", E_SYS);
}

void other_task(VP_INT exinf) {
	static INT s_i = 0;
	INT i = 0;
	i++;
	s_i++;
	vconsole_stdoutf("Other task?\n");
	DBG("This should stay at 1: %08d", i);
	DBG("This should increment: %08d", s_i);
	if (i != 1) {
		PANIC("Stack-allocated variables are not working properly...", E_SYS);
	}
	if (s_i <= 10) {
		act_tsk(1);
	}
}

void usr1_handler() {
	DBG("SIGUSR1 handler...");
	iact_tsk(1);
}

void usr2_handler() {
	DBG("SIGUSR2 handler...");
	iact_tsk(2);
}

void int_handler() {
	DBG("INT handler...");
	iact_tsk(1);
}

static T_CTSK task_info = {0};
static T_CTSK other_info = {0};
static T_DINH usr1_handler_info;
static T_DINH usr2_handler_info;
static T_DINH int_handler_info;

int main(int argc, char** argv) {
	ER_ID ret;

	zepto_init();

	// This would be in the application-specific initialization normally...
	// For now, implemented as `main`...
	DBG("Starting up...");

	DBG("... creating task");
	task_info.task = (FP)task;
	task_info.exinf = NULL;
	task_info.tskatr |= TA_ACT; // Start active

	ret = cre_tsk(1, &task_info);
	if (ret < 0) {
		PANIC("Could not create task...", ret);
	}

	DBG("... creating other_task");
	other_info.task = (FP)other_task;
	other_info.exinf = NULL;

	ret = cre_tsk(2, &other_info);
	if (ret < 0) {
		PANIC("Could not create other_task...", ret);
	}

	DBG("... defining USR1 handler");
	usr1_handler_info.inthdr = usr1_handler;
	ret = def_inh(INTNO_USR1, &usr1_handler_info);
	if (ret < 0) {
		PANIC("Could not define USR1 handler...", ret);
	}

	DBG("... defining USR2 handler");
	usr2_handler_info.inthdr = usr2_handler;
	ret = def_inh(INTNO_USR2, &usr2_handler_info);
	if (ret < 0) {
		PANIC("Could not define USR2 handler...", ret);
	}

	DBG("... defining INT handler");
	int_handler_info.inthdr = int_handler;
	ret = def_inh(INTNO_INT, &int_handler_info);
	if (ret < 0) {
		PANIC("Could not define INT handler...", ret);
	}

	// Launch
	zepto_run();

	// This should not happen
	PANIC("End of main() reached!", E_SYS);
}
