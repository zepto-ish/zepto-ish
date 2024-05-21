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
 */

#include <kernel.h>

extern void task_1(VP_INT exinf);
extern void task_2(VP_INT exinf);

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

static T_CTSK task_1_info;
static T_CTSK task_2_info;
static T_DINH usr1_handler_info;
static T_DINH usr2_handler_info;
static T_DINH int_handler_info;

ER static_setup() {
	ER_ID ret;

	DBG("Starting up...");

	DBG("... creating task_1");
	task_1_info.task = (FP)task_1;
	task_1_info.exinf = NULL;
	task_1_info.tskatr |= TA_ACT; // Start active

	ret = cre_tsk(1, &task_1_info);
	if (ret < 0) {
		PANIC("Could not create task...", ret);
	}

	DBG("... creating task_2");
	task_2_info.task = (FP)task_2;
	task_2_info.exinf = NULL;

	ret = cre_tsk(2, &task_2_info);
	if (ret < 0) {
		PANIC("Could not create task...", ret);
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

	return E_OK;
}
